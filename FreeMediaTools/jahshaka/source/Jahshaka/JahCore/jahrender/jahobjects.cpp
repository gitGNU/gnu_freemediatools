/*******************************************************************************
**
** The source file for the Jahshaka RenderObject file
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "jahrender.h"
#include "gpumathlib.h"
#include "glworld.h"
#include "glworldgpueffects.h"
#include "glcore.h"

void JahRender::makeLargeLayer( JahLayer* layer) 
{
    float4 texture_ratio;
    float4 vertex_coordinates;

    TextureImage* texture_image = &layer->textureData;

    texture_ratio.x = texture_image->texwidthratio;
    texture_ratio.y = texture_image->texheightratio;

    int index = 0;
    glBindTexture( GL_TEXTURE_2D, texture_image->getTileTextureId(index) );

    glBegin(GL_QUADS);	
    {
        glTexCoord2f(0.0f, 0.0f);           
        glVertex2f( -texture_image->getTileImageWidth(index), -texture_image->getTileImageHeight(0) );

        glTexCoord2f(texture_ratio.x,  0.0f);   
        glVertex2f( 0.0f, -texture_image->getTileImageHeight(index) );

        glTexCoord2f(texture_ratio.x,  texture_ratio.y);                    
        glVertex2f( 0.0f,  0.0f );

        glTexCoord2f(0.0f, texture_ratio.y);  
        glVertex2f( -texture_image->getTileImageWidth(index), 0.0f );
    }
    glEnd();

    index = 1;
    glBindTexture( GL_TEXTURE_2D, texture_image->getTileTextureId(index) );

    glBegin(GL_QUADS);	
    {
        glTexCoord2f(0.0f, 0.0f);           
        glVertex2f( 0.0f, -texture_image->getTileImageHeight(index) );

        glTexCoord2f(texture_ratio.x,  0.0f);   
        glVertex2f( texture_image->getTileImageWidth(index), -texture_image->getTileImageHeight(index) );

        glTexCoord2f(texture_ratio.x,  texture_ratio.y);                    
        glVertex2f( texture_image->getTileImageWidth(index),  0.0f );

        glTexCoord2f(0.0f, texture_ratio.y);  
        glVertex2f( 0.0f, 0.0f );
    }
    glEnd();

    index = 2;
    glBindTexture( GL_TEXTURE_2D, texture_image->getTileTextureId(index) );

    glBegin(GL_QUADS);	
    {
        glTexCoord2f(0.0f, 0.0f);           
        glVertex2f( 0.0f, 0.0f );

        glTexCoord2f(texture_ratio.x,  0.0f);   
        glVertex2f( texture_image->getTileImageWidth(index), 0.0f );

        glTexCoord2f(texture_ratio.x,  texture_ratio.y);                    
        glVertex2f( texture_image->getTileImageWidth(index),  texture_image->getTileImageHeight(index) );

        glTexCoord2f(0.0f, texture_ratio.y);  
        glVertex2f( 0.0f, texture_image->getTileImageHeight(index) );
    }
    glEnd();

    index = 3;
    glBindTexture( GL_TEXTURE_2D, texture_image->getTileTextureId(index) );

    glBegin(GL_QUADS);	
    {
        glTexCoord2f(0.0f, 0.0f);           
        glVertex2f( -texture_image->getTileImageWidth(index), 0.0f );

        glTexCoord2f(texture_ratio.x,  0.0f);   
        glVertex2f( 0.0f, 0.0f );

        glTexCoord2f(texture_ratio.x,  texture_ratio.y);                    
        glVertex2f( 0.0f,  texture_image->getTileImageHeight(index) );

        glTexCoord2f(0.0f, texture_ratio.y);  
        glVertex2f( -texture_image->getTileImageWidth(index), texture_image->getTileImageHeight(index) );
    }
    glEnd();

}

void JahRender::makeLayer( JahLayer* layer) 
{
    float4 texture_ratio;
    float4 vertex_coordinates;

    if ( layer->isChromaKeyLayer() )
    {
        glBindTexture(GL_TEXTURE_2D, thelayer->textureKey.texID);
        texture_ratio.x = layer->textureKey.texwidthratio;
        texture_ratio.y = layer->textureKey.texheightratio;
        vertex_coordinates.x = layer->textureKey.mapwidth;
        vertex_coordinates.y = layer->textureKey.mapheight;
    }
    else
    {
        layer->bindBaseOrCompositeTexture();
        texture_ratio.x = layer->textureData.texwidthratio;
        texture_ratio.y = layer->textureData.texheightratio;
        vertex_coordinates.x = layer->textureData.mapwidth;
        vertex_coordinates.y = layer->textureData.mapheight;
    }

    glBegin(GL_QUADS);	
    {
        glTexCoord2f(0.0f, 0.0f);           
        glVertex3f(-vertex_coordinates.x, -vertex_coordinates.y,  0.0f);

        glTexCoord2f(texture_ratio.x,  0.0f);   
        glVertex3f( vertex_coordinates.x, -vertex_coordinates.y,  0.0f);

        glTexCoord2f(texture_ratio.x,  texture_ratio.y);                    
        glVertex3f( vertex_coordinates.x,  vertex_coordinates.y,  0.0f);

        glTexCoord2f(0.0f, texture_ratio.y);  
        glVertex3f(-vertex_coordinates.x,  vertex_coordinates.y,  0.0f);
    }
    glEnd();

}

static const char* key_layer_fragment_shader =
{
        "   float luminance(vec4 color)                                             \n"
        "   {                                                                       \n"
        "       return ( (color.x * 0.3) + (color.y * 0.59) + (color.z * 0.11) );   \n"
        "   }                                                                       \n"
        "                                                                           \n"
        "   uniform sampler2D      clip_texture;                                    \n"
        "   uniform sampler2D      key_texture;                                     \n"
        "                                                                           \n"
        "   void main()                                                             \n"
        "   {                                                                       \n"
        "       vec2    clip_position = gl_TexCoord[0].st;                          \n"
        "       vec4    clip_color = texture2D(clip_texture, clip_position);        \n"
        "       vec4    key_color = texture2D(key_texture, clip_position);          \n"
        "       float   key_luminance = luminance(key_color);                       \n"
        "       gl_FragColor = clip_color;                                          \n"
        "       gl_FragColor.w = key_luminance;                                     \n"
        "   }                                                                       \n"
        "                                                                           \n"
};

void JahRender::makeKeyLayerGLSL( JahLayer* jah_layer, bool set_position ) 
{
    GLCore* gl_core = getGlworldPtr()->getCore();

    // Need this pass to set the depth buffer
    glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
    makeMeshLayer();
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    gl_core->bindPbuffer();

    if (set_position)
    {
        positionObject( getGlworldPtr()->getCamera()->layernodes->m_node );
        positionObject( jah_layer->layernodes->m_node );
    }

    float4 black(0.0f);
    glClearColor(black.x, black.y, black.z, black.w);
    glClear(GL_COLOR_BUFFER_BIT);

    // Need this to reduce the amount of copying going on from the pbuffer for performance reasons
    // Side effect is that screen size is limited for scenes with keylayers.
    int frame_buffer_x_offset;
    int frame_buffer_y_offset;

    if ( jah_layer->getUseFastShaderKeyer() )
    {
        frame_buffer_x_offset = (gl_core->getPbufferRenderWidth() - 
				 getGlworldPtr()->getX_RESOLUTION()) / 2;
        frame_buffer_y_offset = (gl_core->getPbufferRenderHeight() - 
				 getGlworldPtr()->getY_RESOLUTION()) / 2;
    }
    else
    {
        frame_buffer_x_offset = gl_core->getPbufferRenderWidth() / 4;
        frame_buffer_y_offset = gl_core->getPbufferRenderHeight() / 4;
    }

    float width = float( gl_core->getRenderWidth() - (frame_buffer_x_offset * 2) );
    float height = float( gl_core->getRenderHeight() - (frame_buffer_y_offset * 2) );
    
    int texture_width;
    int texture_height;


    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc( thelayer->getSrcBlendFactor(), thelayer->getDstBlendFactor() );

    makeMeshLayer();

    GLuint clip_texture_id = 
        gl_core->copyFrameBufferToTexture(frame_buffer_x_offset, frame_buffer_y_offset, texture_width, texture_height);
    float texwidthratio = width / texture_width;
    float texheightratio = height / texture_height;

    glBlendFunc(GL_ONE, GL_ZERO);
    glBindTexture(GL_TEXTURE_2D, thelayer->textureKey.texID);
    makeKeyLayer();
    GLuint key_texture_id =
        gl_core->copyFrameBufferToTexture(frame_buffer_x_offset, frame_buffer_y_offset, texture_width, texture_height);

    gl_core->unbindPbuffer();

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();

    gl_core->resizeGLInternals( gl_core->getRenderWidth(), gl_core->getRenderHeight() );

    static GLhandleARB shader_program_handle = 0;
    static GlslProgram* shader_program = NULL;
    static GlslShader* fragment_shader;
    static std::vector<GlslShader*> shader_vector;

    GLsizei message_length;
    static char message[1000];

    if (shader_program == NULL)
    {
        fragment_shader = new GlslShader(GL_FRAGMENT_SHADER_ARB, key_layer_fragment_shader);
        shader_vector.push_back(fragment_shader);
        
        shader_program = new GlslProgram(shader_vector);
        shader_program_handle = *shader_program->getGlHandle();
        
        glGetInfoLogARB(shader_program_handle, 1000, &message_length, message);
    }

    glUseProgramObjectARB(shader_program_handle);

    glActiveTextureARB(GL_TEXTURE0_ARB);
    glBindTexture(GL_TEXTURE_2D, clip_texture_id);
    glEnable(GL_TEXTURE_2D);
    shader_program->setTextureHandle("clip_texture", 0);
    
    glActiveTextureARB(GL_TEXTURE1_ARB);
    glBindTexture(GL_TEXTURE_2D, key_texture_id);
    glEnable(GL_TEXTURE_2D);
    shader_program->setTextureHandle("key_texture", 1);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);

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

    glActiveTextureARB(GL_TEXTURE1_ARB);
    glDisable(GL_TEXTURE_2D);
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glDisable(GL_TEXTURE_2D);
    glUseProgramObjectARB(0);

    glDeleteTextures(1, &clip_texture_id);
    glDeleteTextures(1, &key_texture_id);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void JahRender::makeKeyLayer(float z_offset) 
{
    //jtrace->debug("makeKeyLayer");

    glBegin(GL_QUADS);		                // begin drawing a cube
	{
        glTexCoord2f(0.0f, 0.0f);              
        glVertex3f(-thelayer->textureKey.mapwidth, -thelayer->textureKey.mapheight,  z_offset);	// Bottom Left Of The Texture and Quad

        glTexCoord2f(thelayer->textureKey.texwidthratio, 0.0f);   
        glVertex3f( thelayer->textureKey.mapwidth, -thelayer->textureKey.mapheight,  z_offset);	// Bottom Right Of The Texture and Quad

        glTexCoord2f(thelayer->textureKey.texwidthratio,  thelayer->textureKey.texheightratio);                   
        glVertex3f( thelayer->textureKey.mapwidth,  thelayer->textureKey.mapheight,  z_offset);	// Top Right Of The Texture and Quad     1.0f 1.0f

        glTexCoord2f(0.0f, thelayer->textureKey.texheightratio);                              
        glVertex3f(-thelayer->textureKey.mapwidth,  thelayer->textureKey.mapheight,  z_offset);	// Top Left Of The Texture and Quad
	}
    glEnd();
}

// black background layer is the default black background with red trim

void JahRender::makeBlackBackground( void ) 
{
    glColor3f(0.0f, 0.0f, 0.0f);

    if (thelayer->depth) glDisable(GL_DEPTH_TEST);

    glDisable(GL_DEPTH_TEST);
    glDisable( GL_TEXTURE_2D);

    glBegin(GL_QUADS);		               
	{
        glVertex3f(-thelayer->widthOffset, -thelayer->heightOffset,  0.0f);	// Bottom Left Of The Texture and Quad
        glVertex3f( thelayer->widthOffset, -thelayer->heightOffset,  0.0f);	// Bottom Right Of The Texture and Quad
        glVertex3f( thelayer->widthOffset,  thelayer->heightOffset,  0.0f);	// Top Right Of The Texture and Quad     1.0f 1.0f
        glVertex3f(-thelayer->widthOffset,  thelayer->heightOffset,  0.0f);	// Top Left Of The Texture and Quad
	}
    glEnd();

    if ( !Globals::getRendering() )
    {
        makeLayerOutline();
    }

    if (thelayer->depth) glEnable(GL_DEPTH_TEST);


}


// backgound layer is the background with a texture map

void JahRender::makeBackground( void ) 
{
    glBegin(GL_QUADS);		                // begin drawing a cube
	{
        glTexCoord2f(0.0f, 0.0f);   
        glVertex3f(-thelayer->widthOffset, -thelayer->heightOffset,  0.0f);	// Bottom Left Of The Texture and Quad

        glTexCoord2f(thelayer->textureData.texwidthratio,  0.0f);   
        glVertex3f( thelayer->widthOffset, -thelayer->heightOffset,  0.0f);	// Bottom Right Of The Texture and Quad

        glTexCoord2f(thelayer->textureData.texwidthratio,  thelayer->textureData.texheightratio);                    
        glVertex3f( thelayer->widthOffset,  thelayer->heightOffset,  0.0f);	// Top Right Of The Texture and Quad     1.0f 1.0f

        glTexCoord2f(0.0f, thelayer->textureData.texheightratio);                    
        glVertex3f(-thelayer->widthOffset,  thelayer->heightOffset,  0.0f);	// Top Left Of The Texture and Quad
	}
    glEnd();

}




//////////////////////////////////////////////////////////////
// Basic Objects

void JahRender::makeCube( void ) 
{
    GLuint texture_id = thelayer->getBaseOrCompositeTextureId();
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glBegin(GL_QUADS);		                // begin drawing a cube

    // Back Face
    glNormal3f( 0.0f, 0.0f,-1.0f);                              // back face points into the screen on z.

    glTexCoord2f(thelayer->textureData.texwidthratio, 0.0f);  
    glVertex3f(-100.0f, -100.0f, -100.0f);	// Bottom Right Of The Texture and Quad

    glTexCoord2f(thelayer->textureData.texwidthratio, thelayer->textureData.texheightratio);					
    glVertex3f(-100.0f,  100.0f, -100.0f);	// Top Right Of The Texture and Quad

    glTexCoord2f(0.0f, thelayer->textureData.texheightratio);									
    glVertex3f( 100.0f,  100.0f, -100.0f);	// Top Left Of The Texture and Quad

    glTexCoord2f(0.0f, 0.0f);				  
    glVertex3f( 100.0f, -100.0f, -100.0f);	// Bottom Left Of The Texture and Quad


    // Top Face
    glNormal3f( 0.0f, 1.0f, 0.0f);                              // top face points up on y.

    glTexCoord2f(0.0f, thelayer->textureData.texheightratio);									
    glVertex3f(-100.0f,  100.0f, -100.0f);	// Top Left Of The Texture and Quad

    glTexCoord2f(0.0f, 0.0f);					
    glVertex3f(-100.0f,  100.0f,  100.0f);	// Bottom Left Of The Texture and Quad

    glTexCoord2f(thelayer->textureData.texwidthratio, 0.0f);  
    glVertex3f( 100.0f,  100.0f,  100.0f);	// Bottom Right Of The Texture and Quad

    glTexCoord2f(thelayer->textureData.texwidthratio, thelayer->textureData.texheightratio);					
    glVertex3f( 100.0f,  100.0f, -100.0f);	// Top Right Of The Texture and Quad

    // Bottom Face
    glNormal3f( 0.0f, -1.0f, 0.0f);                             // bottom face points down on y.

    glTexCoord2f(thelayer->textureData.texwidthratio, thelayer->textureData.texheightratio);					
    glVertex3f(-100.0f, -100.0f, -100.0f);	// Top Right Of The Texture and Quad

    glTexCoord2f(0.0f, thelayer->textureData.texheightratio);									
    glVertex3f( 100.0f, -100.0f, -100.0f);	// Top Left Of The Texture and Quad

    glTexCoord2f(0.0f, 0.0f);					
    glVertex3f( 100.0f, -100.0f,  100.0f);	// Bottom Left Of The Texture and Quad

    glTexCoord2f(thelayer->textureData.texwidthratio, 0.0f);  
    glVertex3f(-100.0f, -100.0f,  100.0f);	// Bottom Right Of The Texture and Quad

    // Right face
    glNormal3f( 1.0f, 0.0f, 0.0f);                              // right face points right on x.

    glTexCoord2f(thelayer->textureData.texwidthratio, 0.0f);  
    glVertex3f( 100.0f, -100.0f, -100.0f);	// Bottom Right Of The Texture and Quad

    glTexCoord2f(thelayer->textureData.texwidthratio, thelayer->textureData.texheightratio);					
    glVertex3f( 100.0f,  100.0f, -100.0f);	// Top Right Of The Texture and Quad

    glTexCoord2f(0.0f, thelayer->textureData.texheightratio);									
    glVertex3f( 100.0f,  100.0f,  100.0f);	// Top Left Of The Texture and Quad

    glTexCoord2f(0.0f, 0.0f);					
    glVertex3f( 100.0f, -100.0f,  100.0f);	// Bottom Left Of The Texture and Quad

    // Left Face
    glNormal3f(-1.0f, 0.0f, 0.0f);                              // left face points left on x.

    glTexCoord2f(0.0f, 0.0f);					
    glVertex3f(-100.0f, -100.0f, -100.0f);	// Bottom Left Of The Texture and Quad

    glTexCoord2f(thelayer->textureData.texwidthratio, 0.0f);  
    glVertex3f(-100.0f, -100.0f,  100.0f);	// Bottom Right Of The Texture and Quad

    glTexCoord2f(thelayer->textureData.texwidthratio, thelayer->textureData.texheightratio);					
    glVertex3f(-100.0f,  100.0f,  100.0f);	// Top Right Of The Texture and Quad

    glTexCoord2f(0.0f, thelayer->textureData.texheightratio);									
    glVertex3f(-100.0f,  100.0f, -100.0f);	// Top Left Of The Texture and Quad

    // Front Face (note that the texture's corners have to match the quad's corners)
    glNormal3f( 0.0f, 0.0f, 1.0f);                              // front face points out of the screen on z.

    glTexCoord2f(0.0f, 0.0f);					
    glVertex3f(-100.0f, -100.0f,  100.0f);	// Bottom Left Of The Texture and Quad

    glTexCoord2f(thelayer->textureData.texwidthratio, 0.0f);  
    glVertex3f( 100.0f, -100.0f,  100.0f);	// Bottom Right Of The Texture and Quad

    glTexCoord2f(thelayer->textureData.texwidthratio, thelayer->textureData.texheightratio);					
    glVertex3f( 100.0f,  100.0f,  100.0f);	// Top Right Of The Texture and Quad

    glTexCoord2f(0.0f, thelayer->textureData.texheightratio);									
    glVertex3f(-100.0f,  100.0f,  100.0f);	// Top Left Of The Texture and Quad

    glEnd();                                    // done with the polygon.
}


void JahRender::makeCylinder( void ) 
{
    GLuint texture_id = thelayer->getBaseOrCompositeTextureId();
    glBindTexture(GL_TEXTURE_2D, texture_id);

    GLUquadricObj* q = gluNewQuadric();

	glMatrixMode( GL_MODELVIEW );
    glPushMatrix();

    glScalef( 10.0f, 10.0f, 10.0f );

    // cylinder
    gluQuadricNormals( q, GLU_SMOOTH );
    gluQuadricTexture( q, GL_TRUE );

    gluCylinder(q, 6.0, 6.0, 20.0, 24, 1);

    // end cap
    glTranslatef( 0.0, 0.0, 20.0 );
    gluDisk( q, 0.0, 6.0, 24, 1 );

    // other end cap
    glTranslatef( 0.0, 0.0, -20.0 );
    gluQuadricOrientation( q, (GLenum)GLU_INSIDE );
    gluDisk( q, 0.0, 6.0, 24, 1 );

    glPopMatrix();

	gluDeleteQuadric( q );
}

void JahRender::makeSphere( void ) 
{
    GLuint texture_id = thelayer->getBaseOrCompositeTextureId();
    glBindTexture(GL_TEXTURE_2D, texture_id);

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix( );
	
    GLUquadricObj* q = gluNewQuadric();

    // cylinder
    gluQuadricNormals( q, GLU_SMOOTH );
    gluQuadricTexture( q, GL_TRUE );

    gluSphere(q,150.0f,32,32);

	gluDeleteQuadric( q );
	
	glPopMatrix( );
}

/*
   Create a sphere centered at c, with radius r, and precision n
   Draw a point for zero radius spheres
*/

/* 
void CreateSphere(XYZ c,double r,int n)
{
   int i,j;
   double theta1,theta2,theta3;
   XYZ e,p;

   if (r < 0)
      r = -r;
   if (n < 0)
      n = -n;
   if (n < 4 || r <= 0) {
      glBegin(GL_POINTS);
      glVertex3f(c.x,c.y,c.z);
      glEnd();
      return;
   }

   for (j=0;j<n/2;j++) {
      theta1 = j * TWOPI / n - PID2;
      theta2 = (j + 1) * TWOPI / n - PID2;

      glBegin(GL_QUAD_STRIP);
      for (i=0;i<=n;i++) {
         theta3 = i * TWOPI / n;

         e.x = cos(theta2) * cos(theta3);
         e.y = sin(theta2);
         e.z = cos(theta2) * sin(theta3);
         p.x = c.x + r * e.x;
         p.y = c.y + r * e.y;
         p.z = c.z + r * e.z;

         glNormal3f(e.x,e.y,e.z);
         glTexCoord2f(i/(double)n,2*(j+1)/(double)n);
         glVertex3f(p.x,p.y,p.z);

         e.x = cos(theta1) * cos(theta3);
         e.y = sin(theta1);
         e.z = cos(theta1) * sin(theta3);
         p.x = c.x + r * e.x;
         p.y = c.y + r * e.y;
         p.z = c.z + r * e.z;

         glNormal3f(e.x,e.y,e.z);
         glTexCoord2f(i/(double)n,2*j/(double)n);
         glVertex3f(p.x,p.y,p.z);
      }
      glEnd();
   }
}

*/ 

//It is a ssmall modification to enable one to create subsets of a sphere....3 dimensional wedges. As an example see the following code. 

/*
   Create a sphere centered at c, with radius r, and precision n
   Draw a point for zero radius spheres
   Use CCW facet ordering
   "method" is 0 for quads, 1 for triangles
      (quads look nicer in wireframe mode)
   Partial spheres can be created using theta1->theta2, phi1->phi2
   in radians 0 < theta < 2pi, -pi/2 < phi < pi/2
*/

/* 
void CreateSphere(XYZ c,double r,int n,int method,
   double theta1,double theta2,double phi1,double phi2)
{
   int i,j;
   double t1,t2,t3;
   XYZ e,p;

   // Handle special cases 
   if (r < 0)
      r = -r;
   if (n < 0)
      n = -n;
   if (n < 4 || r <= 0) {
      glBegin(GL_POINTS);
      glVertex3f(c.x,c.y,c.z);
      glEnd();
      return;
   }

   for (j=0;j<n/2;j++) {
      t1 = phi1 + j * (phi2 - phi1) / (n/2);
      t2 = phi1 + (j + 1) * (phi2 - phi1) / (n/2);

      if (method == 0)
         glBegin(GL_QUAD_STRIP);
      else
         glBegin(GL_TRIANGLE_STRIP);

      for (i=0;i<=n;i++) {
         t3 = theta1 + i * (theta2 - theta1) / n;

         e.x = cos(t1) * cos(t3);
         e.y = sin(t1);
         e.z = cos(t1) * sin(t3);
         p.x = c.x + r * e.x;
         p.y = c.y + r * e.y;
         p.z = c.z + r * e.z;
         glNormal3f(e.x,e.y,e.z);
         glTexCoord2f(i/(double)n,2*j/(double)n);
         glVertex3f(p.x,p.y,p.z);

         e.x = cos(t2) * cos(t3);
         e.y = sin(t2);
         e.z = cos(t2) * sin(t3);
         p.x = c.x + r * e.x;
         p.y = c.y + r * e.y;
         p.z = c.z + r * e.z;
         glNormal3f(e.x,e.y,e.z);
         glTexCoord2f(i/(double)n,2*(j+1)/(double)n);
         glVertex3f(p.x,p.y,p.z);

      }
      glEnd();
   }
}

*/

void JahRender::makeBezier( void ) 
{

    GLfloat ctrlpoints[4][4][3] =
    {
        {
            {-150.0f,  -150.0f,  400.0f},
            { -50.0f,  -150.0f,  200.0f},
            {  50.0f,  -150.0f, -100.0f},
            { 150.0f,  -150.0f,  200.0f}},
        {
            {-150.0f,  -50.0f,  100.0f},
            { -50.0f,  -50.0f,  300.0f},
            {  50.0f,  -50.0f,   0.0f},
            { 150.0f,  -50.0f, -100.0f}},
        {
            {-150.0f,   50.0f,  400.0f},
            { -50.0f,   50.0f,   0.0f},
            {  50.0f,   50.0f,  300.0f},
            { 150.0f,   50.0f,  400.0f}},
        {
            {-150.0f,  150.0f, -200.0f},
            { -50.0f,  150.0f, -200.0f},
            {  50.0f,  150.0f,   0.0f},
            { 150.0f,  150.0f, -100.0f}}
    };

    GLfloat texcoords[4][4][2] =
    {
        {
            { 0.0f,  0.0f },
            { 0.33f, 0.0f },
            { 0.66f, 0.0f },
            { 1.0f,  0.0f }
        },      
        {
            { 0.0f,  0.33f },
            { 0.33f, 0.33f },
            { 0.66f, 0.33f },
            { 1.0f,  0.33f }
        },
        {
            { 0.0f,  0.66f },
            { 0.33f, 0.66f },
            { 0.66f, 0.66f },
            { 1.0f,  0.66f }
        },
        {
            { 0.0f,  1.0f },
            { 0.33f, 1.0f },
            { 0.66f, 1.0f },
            { 1.0f,  1.0f }
        }            
    };

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix( );

    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4,
        0, 1, 12, 4, &ctrlpoints[0][0][0]);

    glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 2, 4,
        0, 1, 8, 4, &texcoords[0][0][0]);

    glEnable(GL_MAP2_VERTEX_3);
    glEnable( GL_MAP2_TEXTURE_COORD_2 );
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);

    glMapGrid2f(100, 0.0, 1.0, 100, 0.0, 1.0);

    glEvalMesh2(GL_FILL, 0, 100, 0, 100);  //100 was 20

    glDisable(GL_MAP2_VERTEX_3);
    glDisable( GL_MAP2_TEXTURE_COORD_2 );
    glDisable(GL_AUTO_NORMAL);
    glDisable(GL_NORMALIZE);

	glPopMatrix( );
}

void            
JahRender::drawQuadToPbufferAndUpdateCompositeTexture(JahLayer* image_layer)
{
    int image_width = image_layer->getTextureData().getImageWidth();
    int image_height = image_layer->getTextureData().getImageHeight();
    
    float image_width_div_2 = float(image_width) / 2.0f;
    float image_height_div_2 = float(image_height) / 2.0f;

    float4 texture_ratios;
    image_layer->getTextureData().getTextureRatios(texture_ratios);

    GLCore* core = getGlworldPtr()->getCore();
    core->bindPbuffer();

    float4 current_color;
    glGetFloatv(GL_CURRENT_COLOR, &current_color.x);
    glColor4f( image_layer->Red, image_layer->Green, image_layer->Blue, image_layer->layernodes->m_node->Alpha / 100 );
    glBlendFunc(GL_ONE, GL_ZERO);

    image_layer->bindBaseOrCompositeTexture();
    glEnable(GL_TEXTURE_2D);

    float4 clear_color;
    glGetFloatv(GL_COLOR_CLEAR_VALUE, &clear_color.x); 
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glBegin(GL_QUADS);
    {
        glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 0.0f); 
        glVertex3f(-image_width_div_2, -image_height_div_2, 0.0f);
        
        glMultiTexCoord2fARB(GL_TEXTURE0_ARB, texture_ratios.x, 0.0f);           
        glVertex3f(image_width_div_2, -image_height_div_2,  0.0f);
        
        glMultiTexCoord2fARB(GL_TEXTURE0_ARB, texture_ratios.x, texture_ratios.y);           
        glVertex3f(image_width_div_2, image_height_div_2,  0.0f);
        
        glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, texture_ratios.y);           
        glVertex3f(-image_width_div_2, image_height_div_2,  0.0f);
    }
    glEnd(); // GL_QUADS

    glBindTexture(GL_TEXTURE_2D, image_layer->getTextureComposite().texID);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (core->getRenderWidth() - image_width) / 2,
        (core->getRenderHeight() - image_height) / 2, image_width, image_height);

    /*
    int buffer_size = image_height * image_width;
    unsigned int* buffer = new unsigned int[buffer_size];
    jahstd::glGetTextureSubImage(0, 0, image_width, image_height, image_width, image_height, (unsigned char*)buffer);

    for (int i = 0; i < 10; i++)
    {
        printf("%08x\n", buffer[i]);
    }
    */

    glColor4f(current_color.x, current_color.y, current_color.z, current_color.w);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    core->unbindPbuffer();

    image_layer->setTextureCompositeDirty(true);
}
