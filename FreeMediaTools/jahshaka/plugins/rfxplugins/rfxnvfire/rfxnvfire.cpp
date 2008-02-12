/*******************************************************************************
**
** A Jahshaka RasFx Plugin
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** This is Open Source Software Released under the LGPL License
**
*******************************************************************************/

#include "rfxnvfire.h"
#include <math.h>


///////////////////////////////////////////////////
// Mandatory routine used to return instance of
// plugin to calling application


jahPlugin* getPluginClass()
{
    return new MyPlugin();
    //return new MyPlugin;
}

//////////////////////////////////////////////////
// Plugin image subroutines start here
void    MyPlugin::initializePlugin(void)
{
    pluginFamily  = "RasFx";            //family of plugin
    pluginGroup   = "Effect";           //group of plugin
    pluginName    = "Fire";             //name of plugin
    pluginClass   = JAH_GPU_PLUGIN;     //class of plugin ie CPU, GPU, MeshFX

    QString guid = "869e0ed0-e0b8-11d9-947b-00e08161165f";
    setGuid(guid);


    //we are using 1 slider
    usesliders = true;
    numsliders = 9;
    sliderlabel = new QString[numsliders];
    sliderlabel[0]="Time Scale";
    sliderlabel[1]="Transparency";
    sliderlabel[2]="Blend";
    sliderlabel[3]="X Distortion";
    sliderlabel[4]="Y Distortion";
    sliderlabel[5]="Mesh Size";
    sliderlabel[6]="X Position";
    sliderlabel[7]="Y Position";
    sliderlabel[8]="Size";


    //for the headings
    uselabels = true;
    numlabels = 1;
    label = new QString[numlabels];
    label[0]="NV30 Fire";

    //important gpu support variable here
    HASNV  = true;
    HASARB = false;

}

void    MyPlugin::processImage(void)
{
   /////////////////////////////////////////////////////
    //we initialize the effect the first time we use it

    if (!gpuactive)
    {
        initializeGpu();
        gpuactive = true;
    }

    ////////////////////////////////////////////////////
    //we only run the effect if initialization went ok
    if (gpuinitialized)
    {
        ////////////////////////////////////
        //need to switch between arb and nv
        //arb gets the preference but not all plugins have arb versions

        if (USENV)     { processGpuFx();         return; }
        if (USEARB)    { processGpuArbFx();      return; }

        //if we got here it means the hardware isnt suported
        debug("plugin doesnt support current configuration");
    }
}


////////////////////////////////////////////
//main imageprocessing loop

void    MyPlugin::processGpuFx(void)
{
    ////////////////////////////////////////////
    //setup variables
    GLint                                       width;
    GLint                                       height;

    int                                         number_of_layers;

    int                                         horizontal_mesh_size;
    int                                         vertical_mesh_size;
    static int                                  previous_horizontal_mesh_size = 0;
    static int                                  previous_vertical_mesh_size = 0;
    float                                       float_horizontal_mesh_size;
    float                                       float_vertical_mesh_size;

    float4                                      distortion;
    float                                       transparency;
    float                                       blend;
    float4                                      displacement;
    float4                                      size;
    float                                       aspect_ratio;

    static GLuint                               vertex_program_handle;
    static GLuint                               fragment_program_handle;

    unsigned char*                              vertex_program = NULL;
    unsigned char*                              fragment_program = NULL;

    float                                       intensity;
    float                                       timescale;

    QImageIO                                    qimage_io;
    static QImage                               fire_image = NULL;
    static GLuint                               fire_texture = 0;

    //static int                                  count = 0;
    static int                                  animcount = 0;

    ////////////////////////////////////////////
    //initialize main variables and settings

    width  = (GLint)gpuwidth;
    height = (GLint)gpuheight;

    float float_width = (float)width;
    float float_height = (float)height;

    ////////////////////////////////////////////
    //initialize variables and settings

    aspect_ratio = float_height / float_width;

    timescale = (slider[1] / 20.0) + 0.5;
    transparency = (slider[2] / 100.0) + 0.5;
    blend = (slider[3] / 100.0) + 0.5;

    number_of_layers = 5;
    intensity = 3.0;

    distortion.x = ((slider[4]) / 100.0) + 0.93;
    distortion.y = ((slider[5]) / 100.0) + 3.01;
    horizontal_mesh_size = (int)((slider[6] / 2.0) + 25.0);
    horizontal_mesh_size = (horizontal_mesh_size > width) ? width : horizontal_mesh_size;
    vertical_mesh_size = horizontal_mesh_size;
    vertical_mesh_size = (vertical_mesh_size > height) ? height : vertical_mesh_size;

    displacement.x = slider[7] / 10.0;
    displacement.y = slider[8] / 10.0;
    size.x = ((slider[9] - 1.0) * 5.0) + 0.4;
    size.y = size.x;

    float_horizontal_mesh_size = (float)horizontal_mesh_size;
    float_vertical_mesh_size = (float)vertical_mesh_size;

    ////////////////////////////////////////////
    //load fire texture

    if (fire_image == NULL)
    {
        qimage_io.setFileName( JahBasePath+"rfxnvfire/images/Fire.bmp" );

        if ( qimage_io.read() )
        {
            fire_image = qimage_io.image();
            fire_image = QGLWidget::convertToGLFormat(fire_image);
        }
        else
        {
            debug(JahBasePath+"rfxnvfire/images/Fire.bmp"+"could not be loaded\n");
        }
    }

    createTurbulenceTextureGPU(fire_image.width(), fire_image.height(), intensity,
                                horizontal_mesh_size, vertical_mesh_size, number_of_layers);

    ////////////////////////////////////////////
    //load in the vertex shader
    {
        vertex_program = loadshaderfile(JahBasePath+"rfxnvfire/fire_vert_gpu.vp");

        if (!vertex_program)
        {
            return;
        }
    }

    glGenProgramsNV(1, &vertex_program_handle);
    glLoadProgramNV(GL_VERTEX_PROGRAM_NV, vertex_program_handle, strlen((char*)vertex_program), vertex_program);

    if (glGetError() == GL_INVALID_OPERATION)
    {
        find_shader_program_error(vertex_program, "fire_vert_gpu.vp");
    }

    ////////////////////////////////////////////
    //configure the vertex shader

    float               modelview[16];
    float               projection[16];
    float               mvp[16];
    float               modelviewIT[16];
    float               texture[16];

    getMVPMatrices(modelview, projection, mvp, modelviewIT, texture);

    glProgramParameters4fvNV(GL_VERTEX_PROGRAM_NV, 0, 4, mvp);
    glProgramParameters4fvNV(GL_VERTEX_PROGRAM_NV, 4, 4, modelview);
    glProgramParameters4fvNV(GL_VERTEX_PROGRAM_NV, 8, 4, modelviewIT);
    glEnable(GL_VERTEX_PROGRAM_NV);
    glBindProgramNV(GL_VERTEX_PROGRAM_NV, vertex_program_handle);

    glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 12, width, 0, 0, 0);
    glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 13, height, 0, 0, 0);
    glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 14, camera_distance, 0, 0, 0);
    glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 15, 2.0, 0, 0, 0);

    glActiveTextureARB(GL_TEXTURE0_ARB);
	GLuint src_texture_id = getSrcTextureId();
// 	printf("NVfire src_texture_id = %d\n", src_texture_id);
    glBindTexture(GL_TEXTURE_2D, src_texture_id);

    glActiveTextureARB(GL_TEXTURE1_ARB);
    glBindTexture(GL_TEXTURE_2D, turbulence_texture);

    glActiveTextureARB(GL_TEXTURE2_ARB);


   if (fire_texture == 0)
    {
        glGenTextures(1, &fire_texture);
    }

    glBindTexture(GL_TEXTURE_2D, fire_texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, fire_image.width(), fire_image.height(), GL_RGBA, GL_UNSIGNED_BYTE, fire_image.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    ////////////////////////////////////////////
    //load the fragment shader

    {
        fragment_program = loadshaderfile(JahBasePath+"rfxnvfire/fire_frag_gpu.fp");

        if (!fragment_program)
        {
            return;
        }
    }

    glGenProgramsNV(1, &fragment_program_handle);
    glLoadProgramNV(GL_FRAGMENT_PROGRAM_NV, fragment_program_handle,
                    strlen((char*)fragment_program), (const GLubyte*)fragment_program);

    if (glGetError() == GL_INVALID_OPERATION)
    {
        find_shader_program_error(fragment_program, "fire_frag_gpu.fp");
    }

    ////////////////////////////////////////////
    //load the fragment shader with settings

    glEnable(GL_FRAGMENT_PROGRAM_NV);
    glBindProgramNV(GL_FRAGMENT_PROGRAM_NV, fragment_program_handle);

    float scale_factor_x = (float)fire_image.width() / (float)TURBULENCE_TEXTURE_SIZE;
    float scale_factor_y = (float)fire_image.height() / (float)TURBULENCE_TEXTURE_SIZE;

    float fire_scale_factor_x = (float)fire_image.width() / 256.0;
    float fire_scale_factor_y = (float)fire_image.height() / 512.0;

    glProgramNamedParameter4fNV(fragment_program_handle, strlen("scale_factor"),
        (const GLubyte*)"scale_factor", scale_factor_x, scale_factor_y, 0.0f, 0.0f);

    glProgramNamedParameter4fNV(fragment_program_handle, strlen("base_scale_factor"),
        (const GLubyte*)"base_scale_factor",
        texwidthratio,
        texheightratio, 0.0f, 0.0f);

    glProgramNamedParameter4fNV(fragment_program_handle, strlen("fire_scale_factor"),
        (const GLubyte*)"fire_scale_factor", fire_scale_factor_x, fire_scale_factor_y, 0.0f, 0.0f);

    glProgramNamedParameter4fNV(fragment_program_handle, strlen("distortion"),
        (const GLubyte*)"distortion", distortion.x, distortion.y, 0.0f, 0.0f);

    glProgramNamedParameter4fNV(fragment_program_handle, strlen("transparency"),
        (const GLubyte*)"transparency", transparency, 0.0, 0.0f, 0.0f);

    glProgramNamedParameter4fNV(fragment_program_handle, strlen("blend"),
        (const GLubyte*)"blend", blend, 0.0, 0.0f, 0.0f);

    glProgramNamedParameter4fNV(fragment_program_handle, strlen("time"),
        (const GLubyte*)"time", (float)animcount / 100.0, 0.0f, 0.0f, 0.0f);

    glProgramNamedParameter4fNV(fragment_program_handle, strlen("timescale"),
        (const GLubyte*)"timescale", timescale, 0.0f, 0.0f, 0.0f);

    glProgramNamedParameter4fNV(fragment_program_handle, strlen("displacement"),
        (const GLubyte*)"displacement", displacement.x, displacement.y, 0.0f, 0.0f);

    glProgramNamedParameter4fNV(fragment_program_handle, strlen("size"),
        (const GLubyte*)"size", size.x, size.y, 0.0f, 0.0f);

    glProgramNamedParameter4fNV(fragment_program_handle, strlen("aspect_ratio"),
        (const GLubyte*)"aspect_ratio", aspect_ratio, 0.0f, 0.0f, 0.0f);

    ////////////////////////////////////////////
    //run the effects on the gpu now

    glBegin(GL_QUADS);

            glMultiTexCoord4fARB(GL_TEXTURE0_ARB, 0.0, 0.0, 0.0, 1.0);
            glMultiTexCoord4fARB(GL_TEXTURE1_ARB, 0.0, 0.0, 0.0, 1.0);
            glVertex2f( -float_width / 2.0, -float_height / 2.0);

            glMultiTexCoord4fARB(GL_TEXTURE0_ARB,
                                texwidthratio, 0.0, 0.0, 1.0);
            glMultiTexCoord4fARB(GL_TEXTURE1_ARB, 1.0, 0.0, 0.0, 1.0);
            glVertex2f( float_width / 2.0, -float_height / 2.0);

            glMultiTexCoord4fARB(GL_TEXTURE0_ARB,
                                texwidthratio,
                                texheightratio, 0.0, 1.0);
            glMultiTexCoord4fARB(GL_TEXTURE1_ARB, 1.0, 1.0, 0.0, 1.0);
            glVertex2f( float_width / 2.0, float_height / 2.0);

            glMultiTexCoord4fARB(GL_TEXTURE0_ARB, 0.0, texheightratio, 0.0, 1.0);
            glMultiTexCoord4fARB(GL_TEXTURE1_ARB, 0.0, 1.0, 0.0, 1.0);
            glVertex2f( -float_width / 2.0, float_height / 2.0);

    glEnd();

    ////////////////////////////////////////////
    //grab a copy of the resulting image into the buffer
    glActiveTextureARB(GL_TEXTURE0_ARB);
	GLuint dest_texture_id = getDestTextureId();

// 	printf("NVfire dest_texture_id = %d\n", dest_texture_id);
// 	fflush(stdout);

    glBindTexture(GL_TEXTURE_2D, dest_texture_id);

    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (render_width - width) / 2,
                        (render_height - height) / 2, width, height);

    ////////////////////////////////////////////
    //clean up on the way out

    glDisable(GL_VERTEX_PROGRAM_NV);
    glDisable(GL_FRAGMENT_PROGRAM_NV);

    glDeleteProgramsNV(1, &vertex_program_handle);
    glDeleteProgramsNV(1, &fragment_program_handle);

    animcount++;

    previous_horizontal_mesh_size = horizontal_mesh_size;
    previous_vertical_mesh_size = vertical_mesh_size;

	delete [] vertex_program;
	delete [] fragment_program;

}



