/*******************************************************************************
**
** The header file for the gpumathlib class
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc..
** Released under the GNU General Public License
**
*******************************************************************************/

#include "gpumathlib.h"
#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>

std::string Globals::m_gl_vendor;
std::string Globals::m_gl_renderer;
std::string Globals::m_gl_version;
int Globals::m_gl_max_texture_size;
bool Globals::m_enable_scripting_button;
bool Globals::m_is_win32;
bool Globals::m_suppress_popups;
bool Globals::m_auto_save_enable;
float Globals::m_camera_distance;
bool Globals::m_show_lights;
JahControl* Globals::m_jah_control;
bool Globals::m_rendering;
float Globals::m_keyframer_zoom_factor;
int Globals::m_keyframer_y_offset;

static Globals  global_instance;

Globals::Globals()
{
    setKeyframerZoomFactor(1.0f);
    setKeyframerYOffset(0);
}

namespace jahstd
{
    float Roundf(float number)
    {
        float result = ( ( number - floorf(number) ) < 0.5f ) ? floorf(number) : ceilf(number);
        return result;
    }

    int Even(int number)
    {
        if (number & 1)
        {
            return (number - 1);
        }

        return number;
    }

    bool glslSupport()
    {
        return ( GLEW_VERSION_2_0 || 
            ( GLEW_ARB_fragment_shader &&
            GLEW_ARB_vertex_shader && GLEW_ARB_shader_objects &&
            GLEW_ARB_shading_language_100 )
            );
    }
};

int 
jahstd::numberOfDecimalDigits(int number)
{
    number = abs(number);

    int number_of_digits = 
          ( number < 10 ) ? 1
        : ( number < 100 ) ? 2
        : ( number < 1000 ) ? 3
        : ( number < 10000 ) ? 4
        : ( number < 100000 ) ? 5
        : ( number < 1000000 ) ? 6
        : ( number < 10000000 ) ? 7
        : ( number < 100000000 ) ? 8
        : ( number < 1000000000 ) ? 9
        : 10;

    return number_of_digits;
}

bool 
jahstd::doesFileExist(std::string filename)
{
    std::ifstream file( filename.data() );

    if ( file.fail() )
    {
        return false;
    }
    else
    {
        file.close();
        return true;
    }
}

bool 
jahstd::copyBinaryFile(std::string dest_filename, std::string src_filename)
{
    if (src_filename == dest_filename)
    {
        return true;
    }

    std::ifstream in( src_filename.data(), std::ios_base::binary );
    std::ofstream out( dest_filename.data(), std::ios_base::binary  );

    if ( in.fail() )
    {
        fprintf(stderr, "jahstd::copyBinaryFile %s could not be opened for reading\n", src_filename.data() ); 
        return false;
    }

    if ( out.fail() )
    {
        fprintf(stderr, "jahstd::copyBinaryFile %s could not be opened for writing\n", dest_filename.data() ); 
        return false;
    }

    out << in.rdbuf(); 
    in.close();
    out.close();
    return true;
}

void 
jahstd::copyTextFile(std::string dest_filename, std::string src_filename)
{
    std::ifstream in( src_filename.data() );

    if ( in.fail() )
    {
        printf( "jahstd::copyTextFile %s could not be opened for reading\n", src_filename.data() ); 
        return;
    }

    std::ofstream out( dest_filename.data() );
    out << in.rdbuf(); 
}

void
readTextureSubImageUchar(
                         GLuint texture_id, int texture_width, int texture_height,
                         int lower_left_pixel_offset_x, int lower_left_pixel_offset_y,
                         unsigned char* buffer, int buffer_width, int buffer_height)
{


    glBindTexture(GL_TEXTURE_2D, texture_id); 

    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
        lower_left_pixel_offset_x, 
        lower_left_pixel_offset_y, 
        buffer_width, buffer_height);

    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

    getRectangleSubImage( buffer, buffer, 0, 0, 
        buffer_width, buffer_height,
        texture_width, texture_height, buffer_width);
}


float4 operator - (float4& vector, float scalar)
{
    float4 result;

    result.x = vector.x - scalar;
    result.y = vector.y - scalar;
    result.z = vector.z - scalar;
    result.w = vector.w - scalar;

    return result;
}

float4 operator - (float scalar, float4& vector)
{
    float4 result;

    result.x = scalar - vector.x;
    result.y = scalar - vector.y;
    result.z = scalar - vector.z;
    result.w = scalar - vector.w;

    return result;
}

float4 operator + (float4& vector, float scalar)
{
    float4 result;

    result.x = vector.x + scalar;
    result.y = vector.y + scalar;
    result.z = vector.z + scalar;
    result.w = vector.w + scalar;

    return result;
}

float4 operator / (float4 vector, float scalar)
{
    float4 result;

    result.x = vector.x / scalar;
    result.y = vector.y / scalar;
    result.z = vector.z / scalar;
    result.w = vector.w / scalar;

    return result;
}

float4 operator - (float4& vector1, float4& vector2)
{
    float4 result;

    result.x = vector1.x - vector2.x;
    result.y = vector1.y - vector2.y;
    result.z = vector1.z - vector2.z;
    result.w = vector1.w - vector2.w;

    return result;
}

float4 operator + (float4& vector1, float4& vector2)
{
    float4 result;

    result.x = vector1.x + vector2.x;
    result.y = vector1.y + vector2.y;
    result.z = vector1.z + vector2.z;
    result.w = vector1.w + vector2.w;

    return result;
}

float2 operator - (float scalar, float2& vector)
{
    float2 result;

    result.x = scalar - vector.x;
    result.y = scalar - vector.y;

    return result;
}

float2 operator - (float2& vector, float scalar)
{
    float2 result;

    result.x = vector.x - scalar;
    result.y = vector.y - scalar;

    return result;
}

float2 operator + (float2& vector, float scalar)
{
    float2 result;

    result.x = vector.x + scalar;
    result.y = vector.y + scalar;

    return result;
}

float2 operator / (float2& vector, float scalar)
{
    float2 result;

    result.x = vector.x / scalar;
    result.y = vector.y / scalar;

    return result;
}

float2 operator / (float scalar, float2& vector)
{
    float2 result;

    result.x = scalar / vector.x;
    result.y = scalar / vector.y;

    return result;
}

float2 operator - (float2& vector1, float2& vector2)
{
    float2 result;

    result.x = vector1.x - vector2.x;
    result.y = vector1.y - vector2.y;

    return result;
}

float2 operator + (float2& vector1, float2& vector2)
{
    float2 result;

    result.x = vector1.x + vector2.x;
    result.y = vector1.y + vector2.y;

    return result;
}

float2 operator / (float2& vector1, float2& vector2)
{
    float2 result;

    result.x = vector1.x / vector2.x;
    result.y = vector1.y / vector2.y;

    return result;
}


// Utility routines here
float sfrand(void)
{
    return (rand() * 2.0f / (float)RAND_MAX) - 1.0f;
}

float frand(void)
{
    return (rand() / (float)RAND_MAX);
}

float
float4::dot(float4& vector1, float4& vector2)
{
    float result;

    result = vector1.x * vector2.x +
             vector1.y * vector2.y +
             vector1.z * vector2.z +
             vector1.w * vector2.w;

    return result;
}

float4 
float4::crossProduct(float4& vector1, float4& vector2)
{
    float4 result;

    result.x = (vector1.y * vector2.z) - (vector2.y * vector1.z);
    result.y = (vector1.z * vector2.x) - (vector2.z * vector1.x);
    result.z = (vector1.x * vector2.y) - (vector2.x * vector1.y);

    return result;
}

float4 
float4::pow(float4& vector, float scalar)
{
    float4 result;

    result.x = powf(vector.x, scalar);
    result.y = powf(vector.y, scalar);
    result.z = powf(vector.z, scalar);

    return result;
}

float
float2::dot(float2& vector1, float2& vector2)
{
    float result;

    result = vector1.x * vector2.x +
             vector1.y * vector2.y;

    return result;
}

void normalize3f(float& x, float& y, float& z)
{
    float                       length;

    length = sqrt((x * x) + (y * y) + (z * z));

    x /= length;
    y /= length;
    z /= length;
}

float normalizefloat4(float4& vector)
{
    float                       length;

    length = sqrt((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z) + (vector.w * vector.w));

    if (length > .000001f)
    {
        vector.x /= length;
        vector.y /= length;
        vector.z /= length;
        vector.w /= length;
    }

    return length;
}

void get_cross_product_4( float4& result, float4 vector1, float4 vector2)
{
    // Assumes that w is 1
    result.x = (vector1.y * vector2.z) - (vector1.z * vector2.y);
    result.y = -((vector1.x * vector2.z) - (vector1.z * vector2.x));
    result.z = (vector1.x * vector2.y) - (vector1.y * vector2.x);
    result.w = 1.0;
}

void normalize_vector4(float4& vector)
{
    float                   length;

    length = sqrt( (vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z) );

    vector.x /= length;
    vector.y /= length;
    vector.z /= length;
    vector.w = 1.0;
    //  vector.w /= length;
}

void get_vector_from_points(float4& vector, float4 point1, float4 point2)
{

    vector.x = point2.x - point1.x;
    vector.y = point2.y - point1.y;
    vector.z = point2.z - point1.z;
    vector.w = 1.0;
    //  vector.w = point2.w - point1.w;

    normalize_vector4(vector);
}

void
yuv_to_hsv(float4& hsv, float4& yuv)
{
    float4 rgb;

    yuv_to_rgb(rgb, yuv);
    rgb_to_hsv(hsv, rgb);
}

void
hsv_to_yuv(float4& yuv, float4& hsv)
{
    float4 rgb;

    hsv_to_rgb(rgb, hsv);
    rgb_to_yuv(yuv, rgb);
}

void
rgb_to_yuv(float4& yuv, float4& rgb)
{
    yuv.x = (0.299f * rgb.x) + (0.587f * rgb.y) + (0.114f * rgb.z);
    yuv.y = 0.564f * (rgb.z - yuv.x);
    yuv.z = 0.713f * (rgb.x - yuv.x);

    yuv.x = (YUV_SCALE_Y * yuv.x) + 16.5f;          // 16-235
    yuv.y = (YUV_SCALE_UV * yuv.y) + 128.5f;        // 16-240
    yuv.z = (YUV_SCALE_UV * yuv.z) + 128.5f;        // 16-240
}

void
yuv_to_rgb(float4& rgb, float4& yuv)
{
    yuv.x = (yuv.x - 16.5f) * YUV_SCALE_Y_INVERSE;
    yuv.y = (yuv.y - 128.5f) * YUV_SCALE_UV_INVERSE;
    yuv.z = (yuv.z - 128.5f) * YUV_SCALE_UV_INVERSE;

    rgb.x = (yuv.z * 1.403f) + yuv.x;
    rgb.z = (yuv.y * 1.773f) + yuv.x;
    rgb.y = ( yuv.x - (0.299f * rgb.x) - (0.114f * rgb.z) ) * 1.704f;
}

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
find_rgb_range_from_hsv(float4& hsv_low, float4& hsv_high, float4& rgb_low, float4& rgb_high)
{
    rgb_low = 1.0f;
    rgb_high = 0.0f;

    float4 hsv;
    float4 rgb;
    float h_increment = 1.0f / 360.0f;
    float sv_increment = 1.0f / 255.0f;

    for (hsv.x = hsv_low.x; hsv.x <= hsv_high.x; hsv.x += h_increment)
    {
        for (hsv.y = hsv_low.y; hsv.y <= hsv_high.y + sv_increment; hsv.y += sv_increment)
        {
            for (hsv.z = hsv_low.z; hsv.z <= hsv_high.z + sv_increment; hsv.z += sv_increment)
            {
                 hsv_to_rgb(rgb, hsv);

                 if (rgb.x < rgb_low.x)
                 {
                    rgb_low.x = rgb.x;
                 }

                 if (rgb.x > rgb_high.x)
                 {
                    rgb_high.x = rgb.x;
                 }

                 if (rgb.y < rgb_low.y)
                 {
                    rgb_low.y = rgb.y;
                 }

                 if (rgb.y > rgb_high.y)
                 {
                    rgb_high.y = rgb.y;
                 }

                 if (rgb.z < rgb_low.z)
                 {
                    rgb_low.z = rgb.z;
                 }

                 if (rgb.z > rgb_high.z)
                 {
                    rgb_high.z = rgb.z;
                 }
            }
        }
    }
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
color_enhance(float4& rgb, float4& hsv_in, float4& enhance)
{
    float4 cmy = 255 - rgb;

    float k = cmy.min3();
    cmy -= k;

    float4 hsv;

    rgb_to_hsv(hsv, cmy);

    if (enhance.x != enhance.y)
    {
        hsv.z = (hsv.z - enhance.x) / (enhance.y - enhance.x);
    }

    // hsv += hsv_in.xyzz;
    hsv_in.w = hsv_in.z;
    hsv += hsv_in;

    hsv_to_rgb(cmy, hsv);
    cmy += k;

    cmy.x = minf(255, cmy.x);
    cmy.y = minf(255, cmy.y);
    cmy.z = minf(255, cmy.z);

    rgb = 255 - cmy;
}

// Color_enhance2 was added for use with new CPU color corrector. It does the same operations with hsv_in as
// color_enhance, but treats enhance.x and enhance.z as the angle and strength of a hue tint. 
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


void 
invertRGBA(int size, unsigned int* buffer)
{
    for (int i = 0; i < size; i++)
    {
        unsigned int color = buffer[i];
        unsigned char red = (color & 0x000000ff) >> 0;
        unsigned char green = (color & 0x0000ff00) >> 8;
        unsigned char blue = (color & 0x00ff0000) >> 16;

        red = ~red;
        green = ~green;
        blue = ~blue;

        color &= 0xff000000;
        color |= ( (unsigned int)red ) << 0;
        color |= ( (unsigned int)green ) << 8;
        color |= ( (unsigned int)blue ) << 16;

        buffer[i] = color;
    }
}

void check_gl( )
{
    GLenum error;
	while( ( error = glGetError( ) ) != GL_NO_ERROR )
    	fprintf(stderr, "GL error %s\n", gluErrorString( error ) );
}

///////////////////////////////////////////////////
// this crashes when returning NULL
// we need to pass the return value over after name
// and return a bool if it worked or not

unsigned char* loadshaderfile(const char *name)
{
    FILE*                       file;
    int                         size;
    unsigned char*              data;

    file = fopen(name, "rb");

    if(!file)
    {
        fprintf(stderr, "loadshaderfile:: error loading %s file",name);
        return NULL; 
    }

    fseek(file,0,SEEK_END);
    size = ftell(file);
    data = new unsigned char[size + 1];
    fseek(file,0,SEEK_SET);
    fread(data,1,size,file);
    data[size] = '\0';
    fclose(file);
    return data;
}



void find_shader_program_error(unsigned char * source, char* program_name)
{

    GLint                           pos;
    int                             i;
    int                             line;
    int                             linestart;
    char*                           progcopy;

	if ( source == 0 )
	{
		fprintf( stderr, "There is no source loaded for %s\n", program_name );
		return;
	}

    progcopy = strdup((char*)source);
    glGetIntegerv(GL_PROGRAM_ERROR_POSITION_NV, &pos);

    line = 1;
    linestart = 0;

    for (i = 0; i < pos; i++)
    {
        if (progcopy[i] == '\n')
        {
            line++;
            linestart = i+1;
        }
    }

    if (line == 1)
    {
        return;
    }

    fprintf ( stderr, "find_shader_program_error: Program Error : %s : line %d\n",
        program_name, line);

    for (i = linestart; progcopy[i] != '\0' && progcopy[i] != '\n'; i++);

    progcopy[i] = '\0';

    fprintf ( stderr, "%s\n", progcopy+linestart);

    for (i=linestart; i<pos; i++)
    {
        fprintf ( stderr, " ");
    }

    for (;progcopy[i] != '\0' && progcopy[i] != '\n'; i++) 
    {
        fprintf ( stderr, "^");
    }

    fprintf ( stderr, "\n");
    free(progcopy);
    fprintf ( stderr, "%s\n", glGetString(GL_PROGRAM_ERROR_STRING_ARB));

}

void 
matrixTranspose4x4(float* matrix)
{
    float temp[4][4];
    int row;
    int column;

    for (row = 0; row < 4; row++)
    {
        for (column = 0; column < 4; column++)
        {
            temp[row][column] = matrix[(column * 4) + row];
        }
    }

    for (row = 0; row < 4; row++)
    {
        for (column = 0; column < 4; column++)
        {
            matrix[(row * 4) + column] = temp[row][column];
        }
    }
}

void
matrixMultiply4x4(float* matrix_out, float* matrix1, float* matrix2)
{
    int         row;
    int         column;

    for (row = 0; row < 4; row++)
    {
        for (column = 0; column < 4; column++)
        {
            matrix_out[(row * 4) + column] = 0.0f;

            for (int index = 0; index < 4; index++)
            {
                matrix_out[(row * 4) + column] += 
                    matrix1[(row * 4) + index] * 
                    matrix2[(index * 4) + column];
            }
        }
    }
}

void 
vectorMultiply(float* matrix_out, int vector_length, float* vertical_vector, float* horizontal_vector)
{
    int         row;
    int         column;

    for (row = 0; row < vector_length; row++)
    {
        float vertical_vector_value = vertical_vector[row];

        for (column = 0; column < vector_length; column++)
        {
            float horizontal_vector_value = horizontal_vector[column];
            matrix_out[(row * vector_length) + column] = vertical_vector_value * horizontal_vector_value;
        }
    }
}


void 
getMVPMatrices(float* modelview, float* projection, float* mvp, float* modelviewIT, float* texture)
{
    glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
    glGetFloatv(GL_PROJECTION_MATRIX, projection);
    glGetFloatv(GL_TEXTURE_MATRIX, texture);

    matrixMultiply4x4(mvp, modelview, projection);

    // Put them in row major to pass to the shader
    matrixTranspose4x4(modelview);
    matrixTranspose4x4(projection);
    matrixTranspose4x4(mvp);
    matrixTranspose4x4(texture);

    //matrixPrint4x4(modelview, "MODELVIEW");
    //matrixPrint4x4(projection, "PROJECTION");
    //matrixPrint4x4(texture, "texture");

    matrixInvert4x4(modelviewIT, modelview);
    // Already transposed by OpenGL?
    // matrixTranspose4x4(modelviewIT);
}

void
matrixPrintTranspose4x4(float* matrix, const char* name)
{
    fprintf(stderr, "%s :\n", name);

    for (int row = 0; row < 4; row++)
    {
        fprintf(stderr, "%8.3f, %8.3f, %8.3f, %8.3f\n", 
            matrix[0 + row],
            matrix[4 + row],
            matrix[8 + row],
            matrix[12 + row]);
    }
}

void
matrixPrint4x4(float* matrix, const char* name)
{
    fprintf(stderr, "%s :\n", name);

    for (int row = 0; row < 4; row++)
    {
        fprintf(stderr, "%8.3f, %8.3f, %8.3f, %8.3f\n", 
            matrix[(row * 4) + 0],
            matrix[(row * 4) + 1],
            matrix[(row * 4) + 2],
            matrix[(row * 4) + 3]);
    }
}

void
matrixSwapRows4x4(float* row1, float* row2)
{
    int column;
    float temp;

    for (column = 0; column < 4; column++)
    {
        temp = row1[column];
        row1[column] = row2[column];
        row2[column] = temp;
    }
}

void 
matrixInvert4x4(float* inverse, float* matrix)
{
    int         row;
    int         column;
    float       factor;
    int         diagonal;


    static const bool debug_level = false;

    // Using gauss-jordan elimination

    // Load the identity matrix
    for (row = 0; row < 4; row++)
    {
        for (column = 0; column < 4; column++)
        {
            if (row == column)
            {
                inverse[(row * 4) + column] = 1.0f;
            }
            else
            {
                inverse[(row * 4) + column] = 0.0f;
            }
        }
    }

    if (debug_level)
    {
        matrixPrint4x4(matrix, "Initial MATRIX");
        matrixPrint4x4(inverse, "Initial INVERSE");
    }

    for (diagonal = 0; diagonal < 4; diagonal++)
    {
        // If the diagonal element is 0, try to swap with another row from below
        if (matrix[(diagonal * 4) + diagonal] == 0.0f)
        {
            for (row = diagonal; row < 4; row++)
            {
                if (matrix[(row * 4) + diagonal] != 0.0f)
                {
                    matrixSwapRows4x4(matrix + (diagonal * 4), matrix + (row * 4));
                    continue;
                }

                debug("matrixInvert4x4:: Matrix is non-invertible\n");
                return;
            }
        }

        for (row = 0; row < 4; row++)
        {
            if (row != diagonal)
            {
                factor = (matrix[(row * 4) + diagonal] / matrix[(diagonal * 4) + diagonal]);

                for (column = 0; column < 4; column++)
                {
                    matrix[(row * 4) + column] -= (matrix[(diagonal * 4) + column] * factor);
                    inverse[(row * 4) + column] -= (inverse[(diagonal * 4) + column] * factor);
                }

                if (debug_level)
                {
                    matrixPrint4x4(matrix, "MATRIX row conversion");
                    matrixPrint4x4(inverse, "INVERSE row conversion");
                }
            }
        }

        if (debug_level)
        {
            matrixPrint4x4(matrix, "MATRIX");
            matrixPrint4x4(inverse, "INVERSE");
        }
    }

    for (diagonal = 0; diagonal < 4; diagonal++)
    {
        // Normalize the diagonal 
        factor = matrix[(diagonal * 4) + diagonal];

        if (factor != 0.0f)
        {
            for (column = 0; column < 4; column++)
            {
                matrix[(diagonal * 4) + column] /= factor;
                inverse[(diagonal * 4) + column] /= factor;
            }
        }
    }

    if (debug_level)
    {
        matrixPrint4x4(matrix, "MATRIX after normalization");
        matrixPrint4x4(inverse, "INVERSE after normalization");
    }
}

int ImageLoad(char* filename, Image* image)
{
    FILE*                           file;
    unsigned long                   size;   // size of the image in bytes.
    size_t                          i;      // standard counter.
    unsigned short int              planes; // number of planes in image (must be 1)
    unsigned short int              bpp;    // number of bits per pixel (must be 24)
    char                            temp;   // temporary color storage for bgr-rgb conversion.

    // make sure the file is there.
    if ((file = fopen(filename, "rb")) == NULL)
    {
        printf("File Not Found : %s\n",filename);
        return 0;
    }

    // seek through the bmp header, up to the width/height:
    fseek(file, 18, SEEK_CUR);

    // read the width
    if ((i = fread(&image->sizeX, 4, 1, file)) != 1) 
    {
        printf("Error reading width from %s.\n", filename);
        return 0;
    }

    //     printf("Width of %s: %lu\n", filename, image->sizeX);

    // read the height
    if ((i = fread(&image->sizeY, 4, 1, file)) != 1)
    {
        printf("Error reading height from %s.\n", filename);
        return 0;
    }

    //     printf("Height of %s: %lu\n", filename, image->sizeY);

    // calculate the size (assuming 24 bits or 3 bytes per pixel).
    size = image->sizeX * image->sizeY * 3;
    int full_size = image->sizeX * image->sizeY * 4;

    // read the planes
    if ((fread(&planes, 2, 1, file)) != 1) 
    {
        printf("Error reading planes from %s.\n", filename);
        return 0;
    }

    if (planes != 1) 
    {
        printf("Planes from %s is not 1: %u\n", filename, planes);
        return 0;
    }

    // read the bpp
    if ((i = fread(&bpp, 2, 1, file)) != 1)
    {
        printf("Error reading bpp from %s.\n", filename);
        return 0;
    }

    if (bpp != 24) {
        printf("Bpp from %s is not 24: %u\n", filename, bpp);
        return 0;
    }

    // seek past the rest of the bitmap header.
    fseek(file, 24, SEEK_CUR);

    // read the data.
    //image->data = (char *) malloc(size);
    unsigned char* original_data = new unsigned char[size];
    image->data = new unsigned char[full_size];

    if (image->data == NULL) 
    {
        printf("Error allocating memory for color-corrected image data");
        return 0;
    }

    if ((i = fread(original_data, size, 1, file)) != 1) 
    {
        printf("Error reading image data from %s.\n", filename);
        return 0;
    }

    int      write_ptr = 0;

    for (i = 0; i < size; i += 3)
    { // reverse all of the colors. (bgr -> rgb)
        image->data[write_ptr + 3] = 0xff;

        temp = original_data[i + 0];
        image->data[write_ptr + 0] = original_data[i + 2];
        image->data[write_ptr + 1] = original_data[i + 1];
        image->data[write_ptr + 2] = temp;
        //      image->data[write_ptr + 1] = 0xff;
        //      image->data[write_ptr + 2] = 0xff;

        //      if (image->data[write_ptr + 0] != 0)
        //      {
        //          printf("%d: %08x\n", i, *(unsigned int*)&image->data[write_ptr]);
        //      }

        write_ptr += 4;
    }

    delete [] original_data;

    // we're done.
    return 1;
}

///////////////////////////////////////////////////////////////
// arb support routines
void loadJahshakaBasicArb( GLint width, GLint height, float camera_distance,
                          unsigned char* vertex_program,
                          GLuint& vertex_program_handle )
{
    glGenProgramsARB(1, &vertex_program_handle);
    glBindProgramARB(GL_VERTEX_PROGRAM_ARB, vertex_program_handle);
    glProgramStringARB( GL_VERTEX_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB,
        (GLsizei)strlen((char*)vertex_program), (const GLubyte*)vertex_program);

    if (glGetError() == GL_INVALID_OPERATION)
    {
        find_shader_program_error(vertex_program, "jahshaka_basic_vert_arb.vp");
    }

    glProgramLocalParameter4fARB(GL_VERTEX_PROGRAM_ARB, 12, width, 0, 0, 0);
    glProgramLocalParameter4fARB(GL_VERTEX_PROGRAM_ARB, 13, height, 0, 0, 0);
    glProgramLocalParameter4fARB(GL_VERTEX_PROGRAM_ARB, 14, camera_distance, 0, 0, 0);

    glEnable(GL_VERTEX_PROGRAM_ARB);

}

void debug_arbdata(void)
{
    GLint                                         max_fragment_instructions;
    GLint                                         max_vertex_instructions;


    glGetProgramivARB(  GL_FRAGMENT_PROGRAM_ARB,
        GL_MAX_PROGRAM_INSTRUCTIONS_ARB,
        &max_fragment_instructions);

    printf("Maximum number of instructions for a fragment program is: %d\n",
        max_fragment_instructions);

    glGetProgramivARB(  GL_VERTEX_PROGRAM_ARB,
        GL_MAX_PROGRAM_INSTRUCTIONS_ARB,
        &max_vertex_instructions);

    printf("Maximum number of instructions for a vertex program is: %d\n",
        max_vertex_instructions);
}

bool isAnARBFPInstruction(GLubyte* string)
{
    static const int NUM_ARBFP_INSTRUCTIONS = 33;

    static const char* arbfp_instruction_names[] =
    {
        "ABS", "ADD", "CMP", "COS", "DP3", "DP4", "DPH", "DST", "EX2",
        "FLR", "FRC", "KIL", "LG2", "LIT", "LRP", "MAD", "MAX", "MIN",
        "MOV", "MUL", "POW", "RCP", "RSQ", "SCS", "SGE", "SIN", "SLT",
        "SUB", "SWZ", "TEX", "TXB", "TXP", "XPD"
    };

    for (int i = 0; i < NUM_ARBFP_INSTRUCTIONS; i++)
    {
        if ( !strncmp((const char*)string, arbfp_instruction_names[i], 3) )
        {
            return true;
        }
    }

    return false;
}

int countARBFPInstructions(GLubyte* fragment_program)
{
    bool                    done = false;
    int                     count = 0;

    // This really should filter commented out instructions FIXME
    while (!done)
    {
        while (*fragment_program != '\0' && !isAnARBFPInstruction(fragment_program) )
        { 
            fragment_program++;
        }

        if (*fragment_program == '\0')
        {
            return count;
        }

        count++;
        fragment_program++;
    }

    // Keep the compiler from complaining-
    return count;
}


////////////////////////////////////////////////////////////
// check to see if complex arb is supported on the hardware

bool checkComplexArbSupport(QString name, int max_fp_instructions, int )
{
    //max_vp_instructions is ignored for now

    bool isSupported;

    unsigned char* fragment_program = loadshaderfile(name);

    int arb_fragment_instruction_count = countARBFPInstructions(fragment_program);

    fprintf(stderr, "%s instruction count = %d "
        "max_fp_instructions = %d\n", 
        name.ascii(), 
        arb_fragment_instruction_count,
        max_fp_instructions);

    isSupported = bool(arb_fragment_instruction_count <= max_fp_instructions);



    delete [] fragment_program;

    return isSupported;
}

///////////////////////////////////////////////////////////////////
// not implemented yet
///////////////////////////////////////////////////////////////////
/*
enum ARBVPInstructionType
{
ABS, ADD, ARL, DP3, DP4, DPH, DST, EX2, EXP,
FLR, FRC, LG2, LIT, LOG, MAD, MAX, MIN, MOV,
MUL, POW, RCP, RSQ, SGE, SLT, SUB, SWZ, XPD,
not_an_arbvp_instruction
};

///////////////////////////////////////////////////////////////////

static const int NUM_ARBVP_INSTRUCTIONS = 27;

static const char* arbvp_instruction_names[] =
{
"ABS", "ADD", "ARL", "DP3", "DP4", "DPH", "DST", "EX2", "EXP",
"FLR", "FRC", "LG2", "LIT", "LOG", "MAD", "MAX", "MIN", "MOV",
"MUL", "POW", "RCP", "RSQ", "SGE", "SLT", "SUB", "SWZ", "XPD"
};
*/
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
//gpgpu routines
//not sure which one of these to use...

#ifdef NVIDIA_GPGPU
void create_pbuffer(int width, int height)
{
    int                             iConfigCount;

    if (shader_display == 0)
    {
        shader_display = XOpenDisplay(NULL);
    }

    iScreen = DefaultScreen(shader_display);

    int pfAttribList[] =
    {
        GLX_RED_SIZE,               32,
        GLX_GREEN_SIZE,             32,
        GLX_BLUE_SIZE,              32,
        GLX_ALPHA_SIZE,             32,
        GLX_STENCIL_SIZE,           0,
        GLX_DEPTH_SIZE,             0,
        GLX_FLOAT_COMPONENTS_NV,    true,
        GLX_DRAWABLE_TYPE,          GLX_PBUFFER_BIT,
        0,
    };

    glxConfig = glXChooseFBConfig(shader_display, iScreen, pfAttribList, &iConfigCount);
    check_gl();

    int pbAttribList[] =
    {
        GLX_PRESERVED_CONTENTS, true,
        GLX_PBUFFER_WIDTH, width,
        GLX_PBUFFER_HEIGHT, height,
        0,
    };

    shader_pbuffer = glXCreatePbuffer(shader_display, glxConfig[0], pbAttribList);
    check_gl();

    shader_context = glXCreateNewContext(shader_display, glxConfig[0], GLX_RGBA_TYPE, 0, true);
    check_gl();
}
#endif // NVIDIA_GPU


#ifdef NVIDIA_GPGPU
#ifdef WIN32
//no gpgpu support on windows yet!
#else
void create_pbuffer(int width, int height)
{
    int                             iConfigCount;

    if (shader_display == 0)
    {
        shader_display = XOpenDisplay(NULL);
    }

    iScreen = DefaultScreen(shader_display);

    int pfAttribList[] =
    {
        GLX_RED_SIZE,               8,
        GLX_GREEN_SIZE,             8,
        GLX_BLUE_SIZE,              8,
        GLX_ALPHA_SIZE,             8,
        GLX_STENCIL_SIZE,           0,
        GLX_DEPTH_SIZE,             0,
        GLX_FLOAT_COMPONENTS_NV,    false,
        GLX_DRAWABLE_TYPE,          GLX_PBUFFER_BIT,
        0,
    };

    glxConfig = glXChooseFBConfig(shader_display, iScreen, pfAttribList, &iConfigCount);
    check_gl();

    int pbAttribList[] =  
    {
        GLX_PRESERVED_CONTENTS, true,
        GLX_PBUFFER_WIDTH, width,
        GLX_PBUFFER_HEIGHT, height,
        0,
    };

    shader_pbuffer = glXCreatePbuffer(shader_display, glxConfig[0], pbAttribList);
    check_gl();

    shader_context = glXCreateNewContext(shader_display, glxConfig[0], GLX_RGBA_TYPE, jahshaka_glx_context, true);

    check_gl();
}


#endif // !WIN32

float    Globals::getCameraDistance()
{
  return m_camera_distance;
}

#endif // NVIDIA_GPU


float4::float4()
{
  x = 0.0f;
  y = 0.0f;
  z = 0.0f;
  w = 0.0f;
}

float4::float4(float X, float Y, float Z, float W)
{
  x = X;
  y = Y;
  z = Z;
  w = W;
}

float4::  float4(float scalar)
{
  x = scalar;
  y = scalar;
  z = scalar;
  w = scalar;
}

void float4::clamp()
{
  if (x < 0.0f) { x = 0.0f; }
  else if (x > 255.0f) { x = 255.0f; }

  if (y < 0.0f) { y = 0.0f; }
  else if (y > 255.0f) { y = 255.0f; }

  if (z < 0.0f) { z = 0.0f; }
  else if (z > 255.0f) { z = 255.0f; }

  if (w < 0.0f) { w = 0.0f; }
  else if (w > 255.0f) { w = 255.0f; }
}

void float4::clampToOne()
{
  if (x < 0.0f) { x = 0.0f; }
  if (y < 0.0f) { y = 0.0f; }
  if (z < 0.0f) { z = 0.0f; }
  if (w < 0.0f) { w = 0.0f; }
  if (x > 1.0f) { x = 1.0f; }
  if (y > 1.0f) { y = 1.0f; }
  if (z > 1.0f) { z = 1.0f; }
  if (w > 1.0f) { w = 1.0f; }
}

float float4::max4()
{
  float maximum = x;
  if (y > maximum) { maximum = y; };
  if (z > maximum) { maximum = z; };
  if (w > maximum) { maximum = w; };

  return maximum;
}

float float4::min4()
{
  float minimum = x;
  if (y < minimum) { minimum = y; };
  if (z < minimum) { minimum = z; };
  if (w < minimum) { minimum = w; };

  return minimum;
}

float float4::max3()
{
  float maximum = x;
  if (y > maximum) { maximum = y; };
  if (z > maximum) { maximum = z; };

  return maximum;
}

float float4::min3()
{
  float minimum = x;
  if (y < minimum) { minimum = y; };
  if (z < minimum) { minimum = z; };

  return minimum;
}

void float4::operator = (float scalar)
{
  x = scalar;
  y = scalar;
  z = scalar;
  w = scalar;
}

void float4::operator = (float4 vector)
{
  x = vector.x;
  y = vector.y;
  z = vector.z;
  w = vector.w;
}

void float4::operator -= (float scalar)
{
  x -= scalar;
  y -= scalar;
  z -= scalar;
  w -= scalar;
}

void float4::operator += (float scalar)
{
  x += scalar;
  y += scalar;
  z += scalar;
  w += scalar;
}

void float4::operator *= (float scalar)
{
  x *= scalar;
  y *= scalar;
  z *= scalar;
  w *= scalar;
}

void float4::operator /= (float scalar)
{
  x /= scalar;
  y /= scalar;
  z /= scalar;
  w /= scalar;
}

void float4::operator += (float4& vector)
{
  x += vector.x;
  y += vector.y;
  z += vector.z;
  w += vector.w;
}


    float color4::red() { return x; }
    float color4::green() { return y; }
    float color4::blue() { return z; }
    float color4::alpha() { return w; }


    color4::color4()
    {
    }

    color4::color4(float red, float green, float blue, float alpha)
    {
        x = red;
        y = green;
        z = blue;
        w = alpha;
    }

    void color4::operator = (float4 vector)
    {
        x = vector.x;
        y = vector.y;
        z = vector.z;
        w = vector.w;
    }

    bool color4::operator == (float4& vector)
    {
        float tolerance = 1.0f / 255.0f;

        if (   fabs(x - vector.x) >= tolerance 
            || fabs(y - vector.y) >= tolerance
            || fabs(z - vector.z) >= tolerance
            || fabs(w - vector.w) >= tolerance
            )
        {
            return false;
        }

        return true;
    }




float2::float2()
{
  x = 0.0f;
  y = 0.0f;
    }

    float2::float2(float X, float Y)
    {
        x = X;
        y = Y;
    }

    float2::float2(float V)
    {
      x = V;
        y = V;
    }

    float float2::length()
    {
        return sqrtf( (x * x) + (y * y) );
    }

    void float2::clampToOne()
    {
        if (x < 0.0f) { x = 0.0f; }
        if (y < 0.0f) { y = 0.0f; }
        if (x > 1.0f) { x = 1.0f; }
        if (y > 1.0f) { y = 1.0f; }
    }

    void float2::floor()
    {
        x = floorf(x);
        y = floorf(y);
    }

    void float2::ceil()
    {
        x = ceilf(x);
        y = ceilf(y);
    }

    void float2::round()
    {
        x = jahstd::Roundf(x);
        y = jahstd::Roundf(y);
    }

    float float2::max2()
    {
        return ( (x > y) ? x : y );
    }

    float float2::min2()
    {
        return ( (x < y) ? x : y );
    }

    float float2::aspect()
    {
        return x / y;
    }

    static float dot(float2& vector1, float2& vector2);

    void float2::operator = (float scalar)
    {
        x = scalar;
        y = scalar;
    }

    void float2::operator -= (float scalar)
    {
        x -= scalar;
        y -= scalar;
    }

    void float2::operator += (float scalar)
    {
        x += scalar;
        y += scalar;
    }

    void float2::operator += (float2& vector)
    {
        x += vector.x;
        y += vector.y;
    }

    void float2::operator -= (float2& vector)
    {
        x -= vector.x;
        y -= vector.y;
    }

    void float2::operator /= (float scalar)
    {
        x /= scalar;
        y /= scalar;
    }

    void float2::operator *= (float2& vector)
    {
        x *= vector.x;
        y *= vector.y;
    }

    void float2::operator *= (float scalar)
    {
        x *= scalar;
        y *= scalar;
    }

    void float2::operator /= (float2& vector)
    {
        x /= vector.x;
        y /= vector.y;
    }

    bool float2::operator == (float2& vector)
    {
        if (x == vector.x && y == vector.y)
        {
            return true;
        }

        return false;
    }

    bool float2::operator != (float2& vector)
{
  if (x != vector.x || y != vector.y)
        {
            return true;
        }

        return false;
    }

 std::string&     Globals::getGlVendor() { return m_gl_vendor; }
 void             Globals::setGlVendor(std::string& vendor) { m_gl_vendor = vendor; }
 std::string&     Globals::getGlRenderer() { return m_gl_renderer; }
 void             Globals::setGlRenderer(std::string& renderer) { m_gl_renderer = renderer; }
 std::string&     Globals::getGlVersion() { return m_gl_version; }
 void             Globals::setGlVersion(std::string& version) { m_gl_version = version; }
 int              Globals::getGlMaxTextureSize() { return m_gl_max_texture_size; }
 void             Globals::setGlMaxtextureSize(int size) { m_gl_max_texture_size = size; }
  
 bool             Globals::isWin32() { return m_is_win32; }
 void             Globals::setisWin32(bool flag) { m_is_win32 = flag; }
 void             Globals::setEnableScriptingButton(bool flag) { m_enable_scripting_button= flag; }
 bool             Globals::getEnableScriptingButton() { return m_enable_scripting_button; }
 void             Globals::setSuppressPopups(bool flag) { m_suppress_popups = flag; }
 bool             Globals::getSuppressPopups() { return m_suppress_popups; }
 void             Globals::setAutoSaveEnable(bool flag) { m_auto_save_enable = flag; }
 bool             Globals::getAutoSaveEnable() { return m_auto_save_enable; }
   void             Globals::setCameraDistance(float distance) { m_camera_distance = distance; }
   float            Globals::getCameraDistance() { return m_camera_distance; }
   void             Globals::setShowLights(bool flag) { m_show_lights = flag; }
   bool             Globals::getShowLights() { return m_show_lights; }
   void             Globals::setJahControl(JahControl* jah_control) { m_jah_control = jah_control; }
 JahControl*      Globals::getJahControl() { return m_jah_control; }
   void             Globals::setRendering(bool flag) { m_rendering = flag; }
   bool             Globals::getRendering() { return m_rendering; }
   float            Globals::getKeyframerZoomFactor() { return m_keyframer_zoom_factor; }
   void             Globals::setKeyframerZoomFactor(float value) { m_keyframer_zoom_factor = value; }
   int              Globals::getKeyframerYOffset() { return m_keyframer_y_offset; }
   void             Globals::setKeyframerYOffset(int value) { m_keyframer_y_offset = value; }


