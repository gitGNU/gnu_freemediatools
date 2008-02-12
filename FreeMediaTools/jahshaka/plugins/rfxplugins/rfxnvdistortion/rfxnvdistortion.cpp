/*******************************************************************************
**
** A Jahshaka RasFx Plugin
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** This is Open Source Software Released under the LGPL License
**
*******************************************************************************/

#include "rfxnvdistortion.h"
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
    pluginName    = "Distortion";       //name of plugin
    pluginClass   = JAH_GPU_PLUGIN;     //class of plugin ie CPU, GPU, MeshFX

    QString guid = "764cf6e1-e0b8-11d9-947b-00e08161165f";
    setGuid(guid);


    //we are using 1 slider
    usesliders = true;
    numsliders = 9;
    sliderlabel = new QString[numsliders];
    sliderlabel[0]="X Displacement";
    sliderlabel[1]="Y Displacement";
    sliderlabel[2]="Z Displacement";
    sliderlabel[3]="Number Of Layers";
    sliderlabel[4]="Speed";
    sliderlabel[5]="Clear Radius";
    sliderlabel[6]="Horizontal Mesh Size";
    sliderlabel[7]="Vertical Mesh Size";
    sliderlabel[8]="Movement";


    //for the headings
    uselabels = true;
    numlabels = 1;
    label = new QString[numlabels];
    label[0]="NV30 Distortion";

    numoptions = 1;
    button = new QString[numoptions];
    button[0]="No Animation";

    //important gpu support variable here
    HASNV  = true;
    HASARB = true;


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

    int                                         horizontal_mesh_size;
    int                                         vertical_mesh_size;
    float                                       float_horizontal_mesh_size;
    float                                       float_vertical_mesh_size;

    float                                       no_distortion_radius;

    float4                                      Displacement;

    GLuint                                      vertex_program_handle;
    GLuint                                      fragment_program_handle;

    unsigned char*                              vertex_program;
    unsigned char*                              fragment_program;

    static float4                               noise_scale;
    static float4                               noise_translate;

    float2                                      movement;

    int                                         ii, jj;
    GLfloat                                     i, j;

    int                                         num_distortion_layers;
    int                                         speed;

    bool                                        animatenoise;
    static float                                noisecount = 0;
    static bool                                 noiseMatrixInitialized = FALSE;

    ////////////////////////////////////////////
    //initialize variables and settings

    width  = (GLint)gpuwidth;
    height = (GLint)gpuheight;

    float float_width = (float)width;
    float float_height = (float)height;

    ////////////////////////////////////////////
    
    Displacement.x = -(slider[1] / 1.0);
    Displacement.y = -(slider[2] / 1.0);
    Displacement.z =  (slider[3] / 1.0);

    num_distortion_layers = (int)(1.0 + (slider[4] / 10.0));
    speed = 10 + (int)(slider[5] / 10.0);
    no_distortion_radius = slider[6];

    horizontal_mesh_size = ((int)(slider[7] * 100.0));
    horizontal_mesh_size = (horizontal_mesh_size > width) ? width : horizontal_mesh_size;

    vertical_mesh_size = ((int)(slider[8] * 100.0));
    vertical_mesh_size = (vertical_mesh_size > height) ? height : vertical_mesh_size;

    movement.x = (slider[9] * 10.0) + 5.0;
    movement.y = movement.x;

    float_horizontal_mesh_size = (float)horizontal_mesh_size;
    float_vertical_mesh_size = (float)vertical_mesh_size;

    noise_scale.x = 1.3f;
    noise_scale.y = 1.3f;
    noise_scale.z = 1.3f;

    if (option[1])  animatenoise = false;
        else        animatenoise = true;

    ////////////////////////////////////////////
    //load in the vertex shader

    {
        vertex_program = loadshaderfile(JahBasePath+"rfxnvdistortion/distortion_vert_gpu.vp");

		if (!vertex_program)
		{
			return;
		}
    }

    glGenProgramsNV(1, &vertex_program_handle);
    glLoadProgramNV(GL_VERTEX_PROGRAM_NV, vertex_program_handle, strlen((char*)vertex_program), vertex_program);

    if (glGetError() == GL_INVALID_OPERATION)
    {
        find_shader_program_error(vertex_program, "distortion_vert_gpu.vp");
    }

    ////////////////////////////////////////////
    //configure the vertex shader

    float               modelview[16];
    float               projection[16];
    float               mvp[16];
    float               modelviewIT[16];
    float               texture[16];

    glEnable(GL_VERTEX_PROGRAM_NV);
    glBindProgramNV(GL_VERTEX_PROGRAM_NV, vertex_program_handle);

    if (!noiseMatrixInitialized)
    {
        initVertexNoiseConstants(DISTORTION_TABLE_SIZE, permutation_table, gradient_table,GRADIENT_TABLE_SIZE);
        noiseMatrixInitialized = true;
    }

    //this increments the noise for us
    //this is a great candidate for a option box
    if (animatenoise)
    {
        noisecount += 0.1f;
    }

    glProgramParameters4fvNV(GL_VERTEX_PROGRAM_NV, 11, DISTORTION_TABLE_SIZE * 2, (float*)gradient_table);
    glProgramParameter4fvNV(GL_VERTEX_PROGRAM_NV, 11 + (DISTORTION_TABLE_SIZE * 2), (float*)&gradient_table[0]);
    glProgramParameter4fvNV(GL_VERTEX_PROGRAM_NV, 11 + (DISTORTION_TABLE_SIZE * 2) + 1, (float*)&gradient_table[1]);

//     glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 4, float_height, 0.0, 0.0, 0.0);
//     glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 5, float_height / 2.0, 0.0, 0.0, 0.0);
    glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 10, no_distortion_radius, 0.0, 0.0, 0.0);
    glProgramParameter4fvNV(GL_VERTEX_PROGRAM_NV, 8, (float*)&Displacement.x);

    glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 9, 2.0f, 1.0f, 10000.0f, 3.0f);
    glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 77, 0.03125f, 32.0f, 0.0f, 1.0f);

    glActiveTextureARB(GL_TEXTURE0_ARB);
    glBindTexture(GL_TEXTURE_2D, getSrcTextureId() );

    ////////////////////////////////////////////
    //load the fragment shader

    {
        fragment_program = loadshaderfile(JahBasePath+"rfxnvdistortion/distortion_frag_gpu.fp");

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
        find_shader_program_error(fragment_program, "distortion_frag_gpu.fp");
    }

    ////////////////////////////////////////////
    //load the fragment shader with settings

    glEnable(GL_FRAGMENT_PROGRAM_NV);
    glBindProgramNV(GL_FRAGMENT_PROGRAM_NV, fragment_program_handle);

    ////////////////////////////////////////////
    //run the effects on the gpu now

    GLint           current_matrix_mode;

    glGetIntegerv(GL_MATRIX_MODE, &current_matrix_mode);
    glMatrixMode( GL_TEXTURE );
    glLoadIdentity();

    //draws texturized flame layers and blends them
    for (int layer = 0; layer < num_distortion_layers; ++layer)
    {
        glPushMatrix();

        noise_translate.x = noisecount / movement.x;
        noise_translate.y = noisecount / movement.y;
        noise_translate.z = noisecount / 300.0 / movement.y;

        //animate noise by scaling and translating
        glScalef(noise_scale.x, noise_scale.y, noise_scale.z);
        glTranslatef(noise_translate.x, noise_translate.y, noise_translate.z);

        getMVPMatrices(modelview, projection, mvp, modelviewIT, texture);
        glProgramParameters4fvNV(GL_VERTEX_PROGRAM_NV, 0, 4, mvp);
        glProgramParameters4fvNV(GL_VERTEX_PROGRAM_NV, 4, 4, texture);

        GLfloat                 x_unit_size = width / (float)horizontal_mesh_size;
        GLfloat                 y_unit_size = height / (float)vertical_mesh_size;

        glBegin(GL_TRIANGLES);

        for (jj = 0; jj < vertical_mesh_size; jj++)
        {
            j = (float)jj;

            for (ii = 0; ii < horizontal_mesh_size; ii++)
            {
                i = (float)ii;

                // First Triangle
                glMultiTexCoord4fARB(GL_TEXTURE0_ARB, 
                    ((i + 0) / float_horizontal_mesh_size) * texwidthratio + 0.00055,
                    ((j + 0) / float_vertical_mesh_size) * texheightratio, 0.0, 1.0);
                glVertex3f( (-float_width / 2.0) + (i * x_unit_size), (-float_height / 2.0) + (j * y_unit_size), 0.0);

                glMultiTexCoord4fARB(GL_TEXTURE0_ARB,
                    ((i + 1) / float_horizontal_mesh_size) * texwidthratio + 0.00055,
                    ((j + 0) / float_vertical_mesh_size) * texheightratio, 0.0, 1.0);
                glVertex3f( (-float_width / 2.0) + ((i + 1) * x_unit_size), (-float_height / 2.0) + (j * y_unit_size), 0.0);

                glMultiTexCoord4fARB(GL_TEXTURE0_ARB,
                    ((i + 0) / float_horizontal_mesh_size) * texwidthratio + 0.00055,
                    ((j + 1) / float_vertical_mesh_size) * texheightratio, 0.0, 1.0);
                glVertex3f( (-float_width / 2.0) + (i * x_unit_size), (-float_height / 2.0) + ((j + 1) * y_unit_size), 0.0);


                // Second Triangle
                glMultiTexCoord4fARB(GL_TEXTURE0_ARB,
                    ((i + 1) / float_horizontal_mesh_size) * texwidthratio + 0.00055,
                    ((j + 1) / float_vertical_mesh_size) * texheightratio, 0.0, 1.0);
                glVertex3f( (-float_width / 2.0) + ((i + 1) * x_unit_size), (-float_height / 2.0) + ((j + 1) * y_unit_size), 0.0);

                glMultiTexCoord4fARB(GL_TEXTURE0_ARB, 
                    ((i + 1) / float_horizontal_mesh_size) * texwidthratio + 0.00055,
                    ((j + 0) / float_vertical_mesh_size) * texheightratio, 0.0, 1.0);
                glVertex3f( (-float_width / 2.0) + ((i + 1) * x_unit_size), (-float_height / 2.0) + (j * y_unit_size), 0.0);

                glMultiTexCoord4fARB(GL_TEXTURE0_ARB, 
                    ((i + 0) / float_horizontal_mesh_size) * texwidthratio + 0.00055,
                    ((j + 1) / float_vertical_mesh_size) * texheightratio, 0.0, 1.0);
                glVertex3f( (-float_width / 2.0) + (i * x_unit_size), (-float_height / 2.0) + ((j + 1) * y_unit_size), 0.0);

            }
        }

        glEnd(); //GL_QUADS

        glPopMatrix();   // GL_TEXTURE

        glBindTexture(GL_TEXTURE_2D, getDestTextureId() );
        glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (render_width - width) / 2,
                        (render_height - height) / 2, width, height);

    }


    ////////////////////////////////////////////
    //grab a copy of the resulting image into the buffer


    ////////////////////////////////////////////
    //clean up on the way out

    glDisable(GL_VERTEX_PROGRAM_NV);
    glDisable(GL_FRAGMENT_PROGRAM_NV);

    delete [] vertex_program;
    delete [] fragment_program;

    glDeleteProgramsNV(1, &vertex_program_handle);
    glDeleteProgramsNV(1, &fragment_program_handle);

    glMatrixMode(current_matrix_mode);


}



