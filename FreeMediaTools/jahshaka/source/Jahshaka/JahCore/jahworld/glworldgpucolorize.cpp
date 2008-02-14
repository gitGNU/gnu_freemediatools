/*******************************************************************************
**
** The source file for the Jahshaka effects objects module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "glworld.h"
#include "glcore.h"
#include "coreobject.h"
#include <jahpreferences.h>

/////////////////////////////////////////////////////////////
//the gpu color corrector now selects the correct routine
//based on gpu capability
//we should add a GLSL version here as well

void 
GLWorld::doColorizeGPU(JahLayer* base_layer) 
{

    static bool testarbsupport = false;
    static bool arbwillwork = false;

    if ( jahstd::glslSupport() && m_bNV_GPUSupport ) //nvidia support for the color corrector
    {
        //doNV30ColorizeGPU(base_layer);
        doColorCorrectorGLSL(base_layer);
    }
    else if ( m_bGPUSupport ) //nvidia support for the color corrector
    {

        //we need to check if the ARB is supported on this GPU
        if (!testarbsupport)
        {
            JahPrefs& jprefs    = JahPrefs::getInstance();

            QString sourceName = m_qsJahBasePath+"source/Jahshaka/JahLibraries/shaders/color_frag_arb_gpu.fp";

            int themax_vp_instructions =  jprefs.getMaxVertexInstructionsARB();
            int themax_fp_instructions =  jprefs.getMaxFragmentInstructionsARB();

            arbwillwork  =  checkComplexArbSupport(sourceName, themax_fp_instructions, themax_vp_instructions );
            testarbsupport = true;
        }

        if (arbwillwork)
            doArbColorizeGPU(base_layer);
    }
}

////////////////////////////////////////////////////////
// the NVidia based colorize module

void 
GLWorld::doNV30ColorizeGPU(JahLayer* base_layer) 
{
    float                                       brightness;
    float4                                      color;
    float4                                      enhance;
    float4                                      hsv;

    GLint                                       width;
    GLint                                       height;

    GLuint                                      fragment_program_handle;

    unsigned char*                              fragment_program = NULL;

    static int                                  count = 0;

    ////////////////////////////////////////////
    //initialize variables and settings

    width = (int)base_layer->textureData.width;
    height = (int)base_layer->textureData.height;

    float float_width = (float)width;
    float float_height = (float)height;

    motionNode* motion_node = getCurrentEffectLayer()->layernodes->m_node;

    //getCurrentEffectLayer()->layernodes->m_node->rx
    color.x   = motion_node->rx / 1000.0f;
    color.y = motion_node->ry / 1000.0f;
    color.z  = motion_node->rz / 1000.0f;
    color.w  = motion_node->tx / 1000.0f;
    brightness = motion_node->ty / 1000.0f;
    enhance.y = (motion_node->tz / -1000.0f) + 1.0f;
    hsv.x = (motion_node->sx - 1) / 10;
    hsv.y = (motion_node->sy - 1) / 10;
    hsv.z = (motion_node->sz - 1) * 100;

    enhance.x = 0.0f;


    glActiveTextureARB(GL_TEXTURE0_ARB);
    base_layer->bindBaseOrCompositeTexture();


    ////////////////////////////////////////////
    //load the fragment shader

    if (count == 0)
    {
        fragment_program = loadshaderfile(m_qsJahBasePath+"source/Jahshaka/JahLibraries/shaders/color_frag_gpu.fp");
    }

    glGenProgramsNV(1, &fragment_program_handle);
    glLoadProgramNV(GL_FRAGMENT_PROGRAM_NV, fragment_program_handle,
                    (GLsizei)strlen((char*)fragment_program), (const GLubyte*)fragment_program);

    if (glGetError() == GL_INVALID_OPERATION)
    {
      //warning: deprecated conversion from string constant to 'char*'
      char buffer[35];
      strcpy(buffer,"color_frag_gpu.fp");
        find_shader_program_error(fragment_program, buffer);
    }

    ////////////////////////////////////////////
    //load the fragment shader with settings


    glProgramNamedParameter4fNV(fragment_program_handle, (GLsizei)strlen("color_scale"),
        (const GLubyte*)"color_scale", color.x, color.y, color.z, color.w);

    glProgramNamedParameter4fNV(fragment_program_handle, (GLsizei)strlen("brightness"),
        (const GLubyte*)"brightness", brightness, 0.0f, 0.0f, 0.0f);

    glProgramNamedParameter4fNV(fragment_program_handle, (GLsizei)strlen("enhance"),
        (const GLubyte*)"enhance", enhance.x, enhance.y, 0.0f, 0.0f);

    glProgramNamedParameter4fNV(fragment_program_handle, (GLsizei)strlen("hsv"),
        (const GLubyte*)"hsv", hsv.x, hsv.y, hsv.z, 0.0f);

    glEnable(GL_FRAGMENT_PROGRAM_NV);
    glBindProgramNV(GL_FRAGMENT_PROGRAM_NV, fragment_program_handle);

    ////////////////////////////////////////////
    //run the effects on the gpu now

    float texwidthratio = base_layer->textureData.texwidthratio;
    float texheightratio = base_layer->textureData.texheightratio;

    glBegin(GL_QUADS);

            glMultiTexCoord4fARB(GL_TEXTURE0_ARB, 0.0, 0.0, 0.0, 1.0);
            glVertex2f( -float_width / 2.0, -float_height / 2.0);

            glMultiTexCoord4fARB(GL_TEXTURE0_ARB,
                                texwidthratio, 0.0, 0.0, 1.0);
            glVertex2f( float_width / 2.0, -float_height / 2.0);

            glMultiTexCoord4fARB(GL_TEXTURE0_ARB,
                                texwidthratio,
                                texheightratio, 0.0, 1.0);
            glVertex2f( float_width / 2.0, float_height / 2.0);

            glMultiTexCoord4fARB(GL_TEXTURE0_ARB, 0.0, texheightratio, 0.0, 1.0);
            glVertex2f( -float_width / 2.0, float_height / 2.0);


    glEnd(); //GL_QUADS

    ////////////////////////////////////////////
    //grab a copy of the resulting image into the buffer

    glBindTexture(GL_TEXTURE_2D, base_layer->getTextureComposite().texID );
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (m_pCore->getRenderWidth() - width) / 2,
                        (m_pCore->getRenderHeight() - height) / 2, width, height);

    ////////////////////////////////////////////
    //clean up on the way out

    glDisable(GL_FRAGMENT_PROGRAM_NV);

    glDeleteProgramsNV(1, &fragment_program_handle);
    
}

////////////////////////////////////////////////////////
// the ARB based colorize module

void 
GLWorld::doArbColorizeGPU(JahLayer* base_layer) 
{
    float                                       brightness;
    float4                                      color;
    float4                                      enhance;
    float4                                      hsv;

    GLint                                       width;
    GLint                                       height;

    GLuint                                      fragment_program_handle;

    unsigned char*                              fragment_program = NULL;

    static int                                  count = 0;

    ////////////////////////////////////////////
    //initialize variables and settings

    width = (int)base_layer->textureData.width;
    height = (int)base_layer->textureData.height;

    float float_width = (float)width;
    float float_height = (float)height;

    motionNode* motion_node = getCurrentEffectLayer()->layernodes->m_node;

    //getCurrentEffectLayer()->layernodes->m_node->rx
    color.x   = motion_node->rx / 1000.0f;
    color.y = motion_node->ry / 1000.0f;
    color.z  = motion_node->rz / 1000.0f;
    color.w  = motion_node->tx / 1000.0f;
    brightness = motion_node->ty / 1000.0f;
    enhance.y = (motion_node->tz / -1000.0f) + 1.0f;
    hsv.x = (motion_node->sx - 1) / 10;
    hsv.y = (motion_node->sy - 1) / 10;
    hsv.z = (motion_node->sz - 1) * 100;

    enhance.x = 0.0f;

    glActiveTextureARB(GL_TEXTURE0_ARB);
    base_layer->bindBaseOrCompositeTexture();

    ////////////////////////////////////////////
    //load the fragment shader

    if (count == 0)
    {
        fragment_program = loadshaderfile(m_qsJahBasePath+"source/Jahshaka/JahLibraries/shaders/color_frag_arb_gpu.fp");
    }

    glGenProgramsARB(1, &fragment_program_handle);

    glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, fragment_program_handle);
    glProgramStringARB( GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB,
                        (GLsizei)strlen((char*)fragment_program), (const GLubyte*)fragment_program);

    if (glGetError() == GL_INVALID_OPERATION)
    {
      char buffer[45];
      strcpy(buffer,"color_frag_arb_gpu.fp");

        find_shader_program_error(fragment_program,buffer );
    }

    ////////////////////////////////////////////
    //load the fragment shader with settings


    glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 0, color.x, color.y, color.z, 0.0f);
    glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 1, brightness, 0.0f, 0.0f, 0.0f);
    glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 2, enhance.x, enhance.y, 0.0f, 0.0f);
    glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 3, hsv.x, hsv.y, hsv.z, 0.0f);

    glEnable(GL_FRAGMENT_PROGRAM_ARB);

    //glBindProgramNV(GL_FRAGMENT_PROGRAM_NV, fragment_program_handle);

    ////////////////////////////////////////////
    //run the effects on the gpu now

    float texwidthratio = base_layer->textureData.texwidthratio;
    float texheightratio = base_layer->textureData.texheightratio;

    glBegin(GL_QUADS);

            glMultiTexCoord4fARB(GL_TEXTURE0_ARB, 0.0, 0.0, 0.0, 1.0); 
            glVertex2f( -float_width / 2.0, -float_height / 2.0);

            glMultiTexCoord4fARB(GL_TEXTURE0_ARB,
                                texwidthratio, 0.0, 0.0, 1.0);
            glVertex2f( float_width / 2.0, -float_height / 2.0);

            glMultiTexCoord4fARB(GL_TEXTURE0_ARB, 
                                texwidthratio,
                                texheightratio, 0.0, 1.0);
            glVertex2f( float_width / 2.0, float_height / 2.0);

            glMultiTexCoord4fARB(GL_TEXTURE0_ARB, 0.0, texheightratio, 0.0, 1.0); 
            glVertex2f( -float_width / 2.0, float_height / 2.0);


    glEnd(); //GL_QUADS

    ////////////////////////////////////////////
    //grab a copy of the resulting image into the buffer

    glBindTexture(GL_TEXTURE_2D, base_layer->getTextureComposite().texID );
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (m_pCore->getRenderWidth() - width) / 2,
                        (m_pCore->getRenderHeight() - height) / 2, width, height);

    ////////////////////////////////////////////
    //clean up on the way out

    glDisable(GL_FRAGMENT_PROGRAM_ARB);

    glDeleteProgramsARB(1, &fragment_program_handle);
    
}

#if 0
void
rgb_to_hsv(float4& hsv, float4& rgb)
{
    // rgb range 0 -255
    float maximum = rgb.max3();
    float minimum = rgb.min3();

    hsv.z = maximum;
    float delta = maximum - minimum;

    if (delta > 0.0001)
    {
        hsv.y = delta / maximum;

        if (rgb.x == maximum)
        {
            hsv.x = (rgb.y - rgb.z) / delta;

            if (hsv.x < 0)
            {
                hsv.x += 6;
            }
        }
        else if (rgb.y == maximum)
        {
            hsv.x = 2 + (rgb.z - rgb.x) / delta;
        }
        else if (rgb.z == maximum)
        {
            hsv.x = 4 + (rgb.x - rgb.y) / delta;
        }

        hsv.x /= 6;
    }
    else
    {
        hsv.x = 0;
        hsv.y = 0;
    }

    hsv.w = rgb.w;
}

void 
hsv_to_rgb(float4& rgb, float4& hsv)
{
    // rgb range 0 -255
    if (hsv.y == 0)
    {
        rgb = hsv.z;
    }
    else
    {
        float hue = hsv.x;

        if (hue == 1)
        {
            hue = 0;
        }

        hue *= 6;

        int i = ( int )floorf(hue);
        float f = hue - i;
        float w = hsv.z * (1 - hsv.y);
        float q = hsv.z * ( 1 - (hsv.y * f) );
        float t = hsv.z * ( 1 - (hsv.y * (1 - f) ) );

        if (i == 0)
        {
            rgb.x = hsv.z;
            rgb.y = t;
            rgb.z = w;
        }
        else if (i == 1)
        {
            rgb.x = q;
            rgb.y = hsv.z;
            rgb.z = w;
        }
        else if (i == 2)
        {
            rgb.x = w;
            rgb.y = hsv.z;
            rgb.z = t;
        }
        else if (i == 3)
        {
            rgb.x = w;
            rgb.y = q;
            rgb.z = hsv.z;
        }
        else if (i == 4)
        {
            rgb.x = t;
            rgb.y = w;
            rgb.z = hsv.z;
        }
        else if (i == 5)
        {
            rgb.x = hsv.z;
            rgb.y = w;
            rgb.z = q;
        }
    }

    rgb.w = hsv.w;
}


void   
color_enhance2( float4& rgb, float4& hsv_in, float4& enhance )
{
      
    float4 hsv;
    rgb_to_hsv(hsv, rgb );

    hsv.x += hsv_in.x;
    hsv.y *= hsv_in.y; // multiplicative saturation. Range is -1 to +3, with 1.0 center.
    hsv.z += hsv_in.z;

    // important! wrap the h value, dont rely on 'clamp' later, so we do a *real* hue shift.
    if ( hsv.x < 0.0f ) 
    {
        hsv.x += 1.0f;
    }
    else if ( hsv.x > 1.0f ) 
    {
        hsv.x-= 1.0f;
    }
    // similarly, clamp the saturation before doing RGB convert
    if ( hsv.y < 0.0f ) 
    {
        hsv.y = 0.0f;
    }
    else if ( hsv.y > 1.0f ) 
    {
        hsv.y = 1.0f;
    }

    // We store the brightness so that after tint we can restore it.
    float storeval = hsv.z;

    // Now apply the tint. The old enhance.y component is unused. 
    float tint = enhance.x; // 0 - 3600
    float str = enhance.z / 100.0f; // 0 - 1
    
    if ( str > 0.0001f )
    {  
        float4 tinthsv;
        float4 tintrgb;
        tinthsv.x = 0.5 -  ((tint + 900.0f) / ( 3600.0f)); // convert from 1/10th degrees + shift 90 degrees and reverse hue.

        if ( tinthsv.x  < 0.0f ) 
        {
            tinthsv.x += 1.0f;
        }
        else if ( tinthsv.x > 1.0f ) 
        {
            tinthsv.x -= 1.0f;
        }

        tinthsv.y = 1.0;
        tinthsv.z = 1.0;
        hsv_to_rgb( tintrgb, tinthsv );

        if ( 0 ) printf ( "# rgb start %f %f %f \n", rgb.x, rgb.y, rgb.z );   
        if ( 0 ) printf ( "# tintrgb %f %f %f \n", tintrgb.x, tintrgb.y, tintrgb.z );

        hsv_to_rgb(rgb, hsv );
        
        // Add the rgb tint. The '200' is arbitrary, but colors top out before 255, so scale back the strength
        // a little.
        float delr = 200.0f * str* tintrgb.x;
        float delg = 200.0f * str* tintrgb.y;
        float delb = 200.0f * str* tintrgb.z;
        
        rgb.x += delr;
        rgb.y += delg;
        rgb.z += delb;
        rgb.clamp();

        // must now restore orig brightness. I think this part of the algorithm is not strictly correct, 
        // should we preserve contrast, luminosity? Compare with Combustn, its v close but not identical.
        // Also, not all tint changes will change the brightness by the same amount. Same applies to saturation.
        rgb_to_hsv( hsv, rgb  );
        if ( 0 ) printf ( "# hsv before resat %f %f %f \n", hsv.x, hsv.y, hsv.z );
        hsv.z = storeval; // (see note above)
        
        // Final value back to rgb.
        hsv_to_rgb(rgb, hsv );
        
        // (calling function can do rgb.clamp again, we dont here)

        if ( 0 ) printf ( "finalrgb %f %f %f \n", rgb.x, rgb.y, rgb.z );


        /* OLD TINT CODE. Tried literally adding HS vector to each color. Aside from being slow, I dont think
                            this is how its done anyway (despite seeming clever). See above (simpler!) method.
        // Pity need to keep doing this conversion every time (its the same)...
        float4 tinthsv;
        float4 tintrgb;
        tinthsv.x = 0.5 -  ((tint + 900.0) / ( 3600.0)); // our hue circle has green at 0 degrees, and units of 1/10th deg.

        if ( 0 ) printf("tinthsv.x = %f \n", tinthsv.x );
        if ( tinthsv.x  < 0 ) tinthsv.x += 1.0;
        else if ( tinthsv.x > 1.0 ) tinthsv.x -= 1.0;
        tinthsv.y = 1.0;
        tinthsv.z = 1.0;
     
        if ( 0 ) printf ( "# hsv %f %f %f \n", hsv.x, hsv.y, hsv.z );
        if ( 0 ) printf ( "# tinthsv %f %f %f \n", tinthsv.x, tinthsv.y, tinthsv.z );
        
        float angle = PI/2 + hsv.x * TWO_PI; // ie maps to 0-2PI. NB 90 deg shift due to circle.
        if ( angle > TWO_PI ) angle -= TWO_PI;
       
        float tintangle =  PI - enhance.x/3600.0 * TWO_PI; // ie maps to 0-2PI.
     
        float newx = hsv.y * cos(angle) + ( str * cos ( tintangle ) );
        float newy = hsv.y * sin(angle) + ( str * sin ( tintangle ) );
       
        float newsat;
        newsat = sqrt( newx*newx + newy*newy );

        float newangle = acos ( newx / newsat );
        if ( newy < 0.0 ) newangle = TWO_PI - newangle; // acos returns 0-2PI range.
        if ( newsat > 1.0 ) newsat = 1.0;
        
        if ( 0 ) printf ("newangle - pi/2 %f \n", newangle - PI/2.0 );

        float newhue = ( (newangle - PI/2.0 ) / TWO_PI ); // map back the 90 degree shift, and 0-1.0.

        if ( newhue > 1.0 ) newhue -= 1.0;
        else if ( newhue < 0.0 ) newhue += 1.0;

        if ( 0 ) printf("# angle = %f \n", angle );
        if ( 0 ) printf("# newx newy = %f %f \n", newx, newy );
        if ( 0 ) printf("# newangle = %f \n", newangle );
        if ( 0 ) printf("# newsat = %f \n", newsat );
        if ( 0 ) printf("# newhue = %f \n", newhue );

        hsv.x = newhue;
       // (maybe? ) hsv.y = newsat;

        // clamp hue and sat
        if ( hsv.y < 0.0 ) hsv.y = 0.0;
        if ( hsv.y > 1.0 ) hsv.y = 1.0;
        if ( hsv.x < 0.0 ) hsv.x = 0.0;
        if ( hsv.x > 1.0 ) hsv.x = 1.0;

        hsv_to_rgb(rgb, hsv );

        if ( 0 ) printf ( "finalrgb %f %f %f \n", rgb.x, rgb.y, rgb.z );
        */ // (OLD TINT CODE END HERE) 

    }
    else
    {   // just clamp
        if ( hsv.x < 0.0f ) hsv.x = 0.0f;
        if ( hsv.x > 1.0f ) hsv.x = 1.0f;
        hsv_to_rgb(rgb, hsv );
        
    }
}
#endif


const char* 
GLWorld::m_szColorCorrectionFragmentShader =
{
        "   float                                                                   \n"
        "   maxColor( vec3 color)                                                   \n"
        "   {                                                                       \n"
        "       float max = (color.y > color.x) ? color.y : color.x;                \n"
        "       max = (color.z > max) ? color.z : max;                              \n"
        "       return max;                                                         \n"
        "   }                                                                       \n"
        "                                                                           \n"
        "   float                                                                   \n"
        "   minColor( vec3 color)                                                   \n"
        "   {                                                                       \n"
        "       float min = (color.y < color.x) ? color.y : color.x;                \n"
        "       min = (color.z < min) ? color.z : min;                              \n"
        "       return min;                                                         \n"
        "   }                                                                       \n"
        "                                                                           \n"
        "   vec4                                                                    \n"
        "   rgb_to_hsv( vec4 rgb )                                                  \n"
        "   {                                                                       \n"
        "       vec4 hsv;                                                           \n"
        "                                                                           \n"
        "       float maximum = maxColor(rgb.xyz);                                  \n"
        "       float minimum = minColor(rgb.xyz);                                  \n"
        "       hsv.z = maximum;                                                    \n"
        "       float delta = maximum - minimum;                                    \n"
        "                                                                           \n"
        "       if (delta > 0.0001)                                                 \n"
        "       {                                                                   \n"
        "           hsv.y = delta / maximum;                                        \n"
        "                                                                           \n"
        "           if (rgb.x == maximum)                                           \n"
        "           {                                                               \n"
        "               hsv.x = (rgb.y - rgb.z) / delta;                            \n"
        "                                                                           \n"
        "               if (hsv.x < 0.0)                                            \n"
        "               {                                                           \n"
        "                   hsv.x += 6.0;                                           \n"
        "               }                                                           \n"
        "           }                                                               \n"
        "           else if (rgb.y == maximum)                                      \n"
        "           {                                                               \n"
        "               hsv.x = 2.0 + ( (rgb.z - rgb.x) / delta );                  \n"
        "           }                                                               \n"
        "           else if (rgb.z == maximum)                                      \n"
        "           {                                                               \n"
        "               hsv.x = 4.0 + ( (rgb.x - rgb.y) / delta );                  \n"
        "           }                                                               \n"
        "                                                                           \n"
        "           hsv.x /= 6.0;                                                   \n"
        "       }                                                                   \n"
        "       else                                                                \n"
        "       {                                                                   \n"
        "           hsv.x = 0.0;                                                    \n"
        "           hsv.y = 0.0;                                                    \n"
        "       }                                                                   \n"
        "                                                                           \n"
        "       hsv.w = rgb.w;                                                      \n"
        "       return hsv;                                                         \n"
        "   }                                                                       \n"
        "                                                                           \n"
        "   vec4                                                                    \n"
        "   hsv_to_rgb( vec4 hsv_in )                                               \n"
        "   {                                                                       \n"
        "       vec4 rgb;                                                           \n"
        "                                                                           \n"
        "       // rgb range 0 -255                                                 \n"
        "       if (hsv_in.y == 0.0)                                                \n"
        "       {                                                                   \n"
        "           rgb = hsv_in.zzzz;                                              \n"
        "       }                                                                   \n"
        "       else                                                                \n"
        "       {                                                                   \n"
        "           float hue = hsv_in.x;                                           \n"
        "                                                                           \n"
        "           if (hue == 1.0)                                                 \n"
        "           {                                                               \n"
        "               hue = 0.0;                                                  \n"
        "           }                                                               \n"
        "                                                                           \n"
        "           hue *= 6.0;                                                     \n"         
        "                                                                           \n"
		"           int i = int( floor(hue) );                                      \n"
		"           float f = hue - float(i);                                       \n"
        "           float w = hsv_in.z * (1.0 - hsv_in.y);                          \n"
        "           float q = hsv_in.z * ( 1.0 - (hsv_in.y * f) );                  \n"
        "           float t = hsv_in.z * ( 1.0 - (hsv_in.y * (1.0 - f) ) );         \n"
        "                                                                           \n"
        "           if (i == 0)                                                     \n"
        "           {                                                               \n"
        "               rgb.x = hsv_in.z;                                           \n"
        "               rgb.y = t;                                                  \n"
        "               rgb.z = w;                                                  \n"
        "           }                                                               \n"
        "           else if (i == 1)                                                \n"
        "           {                                                               \n"
        "               rgb.x = q;                                                  \n"
        "               rgb.y = hsv_in.z;                                           \n"
        "               rgb.z = w;                                                  \n"
        "           }                                                               \n"
        "           else if (i == 2)                                                \n"
        "           {                                                               \n"
        "               rgb.x = w;                                                  \n"
        "               rgb.y = hsv_in.z;                                           \n"
        "               rgb.z = t;                                                  \n"
        "           }                                                               \n"
        "           else if (i == 3)                                                \n"
        "           {                                                               \n"
        "               rgb.x = w;                                                  \n"
        "               rgb.y = q;                                                  \n"
        "               rgb.z = hsv_in.z;                                           \n"
        "           }                                                               \n"
        "           else if (i == 4)                                                \n"
        "           {                                                               \n"
        "               rgb.x = t;                                                  \n"
        "               rgb.y = w;                                                  \n"
        "               rgb.z = hsv_in.z;                                           \n"
        "           }                                                               \n"
        "           else if (i == 5)                                                \n"
        "           {                                                               \n"
        "               rgb.x = hsv_in.z;                                           \n"
        "               rgb.y = w;                                                  \n"
        "               rgb.z = q;                                                  \n"
        "           }                                                               \n"
        "       }                                                                   \n"
        "                                                                           \n"
        "       rgb.w = hsv_in.w;                                                   \n"
        "       return rgb;                                                         \n"
        "   }                                                                       \n"
        "                                                                           \n"
        "   vec4                                                                    \n"
        "   color_enhance( vec4 rgb_in, vec3 hsv_in, vec3 enhance )                 \n"
        "   {                                                                       \n"
        "       vec4 rgb_out;                                                       \n"
        "       vec4 hsv = rgb_to_hsv(rgb_in);                                      \n"
        "       hsv.xz += hsv_in.xz;                                                \n"
        "       hsv.y *= hsv_in.y;                                                  \n"
        "                                                                           \n"
        "       if ( hsv.x < 0.0 )                                                  \n"
        "       {                                                                   \n"
        "           hsv.x += 1.0;                                                   \n"
        "       }                                                                   \n"
        "       else if ( hsv.x > 1.0 )                                             \n"
        "       {                                                                   \n"
        "           hsv.x-= 1.0;                                                    \n"
        "       }                                                                   \n"
        "                                                                           \n"
        "       if ( hsv.y < 0.0 )                                                  \n"
        "       {                                                                   \n"
        "           hsv.y = 0.0;                                                    \n"
        "       }                                                                   \n"
        "       else if ( hsv.y > 1.0 )                                             \n"
        "       {                                                                   \n"
        "           hsv.y = 1.0;                                                    \n"
        "       }                                                                   \n"
        "                                                                           \n"
        "       // We store the brightness so that after tint we can restore it.    \n"
        "       float storeval = hsv.z;                                             \n"
        "                                                                           \n"
        "       // Now apply the tint. The old enhance.y component is unused.       \n"
        "       float tint = enhance.x; // 0 - 3600                                 \n"
        "       float str = enhance.z / 100.0; // 0 - 1                             \n"
        "                                                                           \n"
        "       if ( str > 0.0001 )                                                 \n"
        "       {                                                                   \n"
        "           vec4 tinthsv;                                                   \n"
        "           vec4 tintrgb;                                                   \n"
        "           tinthsv.x = 0.5 -  ( (tint + 900.0) / 3600.0 );                 \n"
        "                                                                           \n"
        "           if ( tinthsv.x  < 0.0 )                                         \n"
        "           {                                                               \n"
        "               tinthsv.x += 1.0;                                           \n"
        "           }                                                               \n"
        "           else if ( tinthsv.x > 1.0 )                                     \n"
        "           {                                                               \n"
        "               tinthsv.x -= 1.0;                                           \n"
        "           }                                                               \n"
        "                                                                           \n"
        "           tinthsv.y = 1.0;                                                \n"
        "           tinthsv.z = 1.0;                                                \n"
        "           tintrgb = hsv_to_rgb(tinthsv);                                  \n"
        "                                                                           \n"
        "           rgb_out = hsv_to_rgb(hsv);                                      \n"
        "                                                                           \n"
        "           float delr = 200.0 * str * tintrgb.x;                           \n"     
        "           float delg = 200.0 * str * tintrgb.y;                           \n"
        "           float delb = 200.0 * str * tintrgb.z;                           \n"
        "                                                                           \n"
        "           rgb_out.x += delr;                                              \n"
        "           rgb_out.y += delg;                                              \n"
        "           rgb_out.z += delb;                                              \n"
        "           rgb_out = clamp(rgb_out, 0.0, 255.0);							\n"    
        "                                                                           \n"
        "           hsv = rgb_to_hsv(rgb_out);                                      \n"
        "           hsv.z = storeval;                                               \n"
        "           rgb_out = hsv_to_rgb(hsv);                                      \n"
        "                                                                           \n"
        "           return rgb_out;                                                 \n"
        "       }                                                                   \n"
        "       else                                                                \n"
        "       {                                                                   \n"
        "           if ( hsv.x < 0.0 )                                              \n"
        "           {                                                               \n"
        "               hsv.x = 0.0;                                                \n"
        "           }                                                               \n"
        "                                                                           \n"
        "           if ( hsv.x > 1.0 )                                              \n"
        "           {                                                               \n"
        "               hsv.x = 1.0;                                                \n"    
        "           }                                                               \n"
        "                                                                           \n"
        "           rgb_out = hsv_to_rgb(hsv);                                      \n"
        "           return rgb_out;                                                 \n"
        "       }                                                                   \n"
        "   }                                                                       \n"
        "                                                                           \n"
        "   uniform sampler2D      base_texture;                                    \n"
        "   uniform float          brightness;                                      \n" 
        "   uniform float          gamma;                                           \n" 
        "   uniform vec3           hsv;                                             \n" 
        "   uniform vec4           color_scale;                                     \n" 
        "   uniform vec3           enhance;                                         \n" 
        "                                                                           \n"
        "   void main()                                                             \n"
        "   {                                                                       \n"
        "       vec2    base_position = gl_TexCoord[0].st;                          \n"
        "       vec4    base_color = texture2D(base_texture, base_position);        \n"
        "                                                                           \n"
        "       base_color *= 255.0;                                                \n"
        "                                                                           \n"
        "       base_color += brightness;                                           \n"
        "       base_color += color_scale;                                          \n"
        "       base_color = color_enhance(base_color, hsv, enhance);               \n"
        "       base_color = clamp(base_color, 0.0, 255.0);                         \n"
        "                                                                           \n"
        "       base_color /= 255.0;                                                \n"
		"       base_color = pow( base_color, vec4(gamma, gamma, gamma, gamma) );   \n"
        "                                                                           \n"
        "       gl_FragColor = base_color;                                          \n"
        "       gl_FragColor.w = 1.0;                                               \n"
        "   }                                                                       \n"
};

void
GLWorld::doColorCorrectorGLSL(JahLayer* image_layer)
{
    GLsizei message_length;
    char message[10000];

    static bool nvidia_cg15_error = false;

    float4          rgba;
    float4          color_scale;
    float4          enhance;
    float4          hsv;
    float           gamma;

    motionNode* sliders = getCurrentEffectLayer()->layernodes->m_node;

    color_scale.x = sliders->rx / 4.0f;
    color_scale.y = sliders->ry / 4.0f;
    color_scale.z = sliders->rz / 4.0f;


    float brightness = sliders->ty / 4.0f;  // brightness is transY.

    hsv.x = (sliders->sx - 1) / 10; // -0.5 to +0.5
    hsv.y = ( (sliders->sy - 1) * 2 / 5 ) + 1.0 ; // changed range. See sathelperslot too
    hsv.z = (sliders->sz - 1) * 100;

    gamma = sliders->Alpha / 100.0f;

    // hue offset.
    enhance.x = sliders->tx; // 0-3600
    // strength slider (NB we cant really have negative values, but deal with it later. keep centred around zero).
    enhance.z = sliders->tz; // 0-100 original, shifted
  
    color_scale.w = 0.0f;

    float height = image_layer->textureData.height;
    float width = image_layer->textureData.width;

    static GLint hsv_handle = -1;
    static GLint enhance_handle = -1;
    static GLint color_scale_handle = -1;
    static GLint brightness_handle = -1;
    static GLint gamma_handle = -1;

    static GLhandleARB shader_program_handle = 0;
    static GlslProgram* shader_program = NULL;
    static GlslShader* fragment_shader;
    static std::vector<GlslShader*> shader_vector;

    if (shader_program == NULL)
    {
        fragment_shader = new GlslShader(GL_FRAGMENT_SHADER_ARB, m_szColorCorrectionFragmentShader);
        shader_vector.push_back(fragment_shader);
        
        shader_program = new GlslProgram(shader_vector);
        shader_program_handle = *shader_program->getGlHandle();
        
        brightness_handle = glGetUniformLocationARB(shader_program_handle, "brightness");
        hsv_handle = glGetUniformLocationARB(shader_program_handle, "hsv");
        color_scale_handle = glGetUniformLocationARB(shader_program_handle, "color_scale");
        enhance_handle = glGetUniformLocationARB(shader_program_handle, "enhance");
        gamma_handle = glGetUniformLocationARB(shader_program_handle, "gamma");

        glGetInfoLogARB(shader_program_handle, 10000, &message_length, message);

		if (message_length)
		{
			printf("Color Corrector fragment shader compilation error:\n%s\n", message);

            // Switch to CPU effect.  There is a bug in Nvidia 8.x drivers from cg 1.5
            // which forces this
            nvidia_cg15_error = true;
		}
    }

    if (nvidia_cg15_error)
    {
        getCurrentEffectLayer()->getEffectLayer()->setType(EffectInfo::COLORIZE_CPU_TYPE);
        image_layer->layername = "Colorize CPU";
    }

    glActiveTextureARB(GL_TEXTURE0_ARB);
    image_layer->bindBaseOrCompositeTexture();
    glEnable(GL_TEXTURE_2D);

    glUseProgramObjectARB(shader_program_handle);
    shader_program->setTextureHandle("base_texture", 0);

    glUniform3fARB(hsv_handle, hsv.x, hsv.y, hsv.z);
    glUniform3fARB(enhance_handle, enhance.x, enhance.y, enhance.z);
    glUniform1fARB(brightness_handle, brightness);
    glUniform1fARB(gamma_handle, gamma);
    glUniform4fARB(color_scale_handle, color_scale.x, color_scale.y, color_scale.z, color_scale.w);
    
    float texwidthratio = image_layer->textureData.texwidthratio;
    float texheightratio = image_layer->textureData.texheightratio;

    glBegin(GL_QUADS);
    {
        glMultiTexCoord4fARB(GL_TEXTURE0_ARB, 0.0, 0.0, 0.0, 1.0); 
        glVertex2f( -width / 2.0, -height / 2.0);

        glMultiTexCoord4fARB(GL_TEXTURE0_ARB, texwidthratio, 0.0, 0.0, 1.0);
        glVertex2f( width / 2.0, -height / 2.0);

        glMultiTexCoord4fARB(GL_TEXTURE0_ARB, texwidthratio, texheightratio, 0.0, 1.0);
        glVertex2f( width / 2.0, height / 2.0);

        glMultiTexCoord4fARB(GL_TEXTURE0_ARB, 0.0, texheightratio, 0.0, 1.0); 
        glVertex2f( -width / 2.0, height / 2.0);
    }
    glEnd(); //GL_QUADS

    ////////////////////////////////////////////
    //grab a copy of the resulting image into the buffer

    glBindTexture(GL_TEXTURE_2D, image_layer->getTextureComposite().texID );

    int render_width = m_pCore->getRenderWidth();
    int render_height = m_pCore->getRenderHeight();

    int texture_lower_left_x = jahstd::Max<int>(0, int( width - render_width )) / 2;
    int texture_lower_left_y = jahstd::Max<int>(0, int( height - render_height )) / 2;
    int screen_lower_left_x = jahstd::Max<int>( int(render_width - width ), 0 ) / 2;
    int screen_lower_left_y = jahstd::Max<int>( int(render_height - height ), 0 ) / 2;
    int copy_width = jahstd::Min<int>(int( width ), int( render_width ));
    int copy_height = jahstd::Min<int>(int( height ), int( render_height ));

    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 
        texture_lower_left_x,
        texture_lower_left_y,
        screen_lower_left_x,
        screen_lower_left_y, 
        copy_width, 
        copy_height );

    glUseProgramObjectARB(0);
}

