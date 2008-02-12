/*******************************************************************************
**
** A Jahshaka RasFx Plugin
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** This is Open Source Software Released under the LGPL License
**
*******************************************************************************/

#include "rfxnvfog.h"
#include <math.h>


void MyPlugin::createTurbulenceTextureGPU(int width, int height, float intensity,
                                        int horizontal_mesh_size, int vertical_mesh_size, int number_of_layers)

{
    ////////////////////////////////////////////
    //setup variables


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

    int                                         ii, jj;
    GLfloat                                     i, j;

    float                                       transparency;

    GLuint                                      texture_id[10];
    float                                       xcorrection;
    float                                       ycorrection;

    static int                                  previous_width = 0;
    static int                                  previous_height = 0;

    static int                                  previous_horizontal_mesh_size = 0;
    static int                                  previous_vertical_mesh_size = 0;
    static float                                previous_intensity = 0;

    static int                                  noisecount = 0;
    static bool                                 noiseMatrixInitialized = FALSE;


    ////////////////////////////////////////////
    //initialize main variables and settings

    ////////////////////////////////////////////
    //initialize variables and settings

    if (previous_width == width && previous_height == height &&
        previous_horizontal_mesh_size == horizontal_mesh_size && previous_vertical_mesh_size == vertical_mesh_size &&
        intensity == previous_intensity)
    {
        return;
    }

    previous_width = width;
    previous_height = height;
    previous_horizontal_mesh_size = horizontal_mesh_size;
    previous_vertical_mesh_size = vertical_mesh_size;
    previous_intensity = intensity;


    if (noisecount != 0)
    {
        glDeleteTextures(1, &turbulence_texture);
    }

    float float_width = (float)width;
    float float_height = (float)height;

    Displacement.x = 0.0;
    Displacement.y = 0.0;
    Displacement.z = 0.0;

    no_distortion_radius = 0.0;
    transparency = 1.0;

    float_horizontal_mesh_size = (float)horizontal_mesh_size;
    float_vertical_mesh_size = (float)vertical_mesh_size;

    noise_scale.x = 1.3;
    noise_scale.y = 1.3;
    noise_scale.z = 1.3;


    ////////////////////////////////////////////
    //load in the vertex shader

    {
        vertex_program = loadshaderfile(JahBasePath+"rfxcore/create_turbulence_texture_vert_gpu.vp");

        if (!vertex_program)
        {
            return;
        }
    }

    glGenProgramsNV(1, &vertex_program_handle);
    glLoadProgramNV(GL_VERTEX_PROGRAM_NV, vertex_program_handle, strlen((char*)vertex_program), vertex_program);

    if (glGetError() == GL_INVALID_OPERATION)
    {
        find_shader_program_error(vertex_program, "create_turbulence_texture_vert_gpu.vp");
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
    glEnable(GL_VERTEX_PROGRAM_NV);
    glBindProgramNV(GL_VERTEX_PROGRAM_NV, vertex_program_handle);

    if (!noiseMatrixInitialized)
    {
        initVertexNoiseConstants(DISTORTION_TABLE_SIZE, permutation_table, gradient_table,GRADIENT_TABLE_SIZE);
        noiseMatrixInitialized = true;

        noise_translate.x = -100.0;
        noise_translate.y = -100.0;
        noise_translate.y = 0.0;
    }

    glProgramParameters4fvNV(GL_VERTEX_PROGRAM_NV, 8, DISTORTION_TABLE_SIZE * 2, (float*)gradient_table);
    glProgramParameter4fvNV(GL_VERTEX_PROGRAM_NV, 8 + (DISTORTION_TABLE_SIZE * 2), (float*)&gradient_table[0]);
    glProgramParameter4fvNV(GL_VERTEX_PROGRAM_NV, 8 + (DISTORTION_TABLE_SIZE * 2) + 1, (float*)&gradient_table[1]);

    glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 74, float_height, 0.0, 0.0, 0.0);
    glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 75, float_height / 2.0, 0.0, 0.0, 0.0);
    glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 76, no_distortion_radius, 0.0, 0.0, 0.0);
    glProgramParameter4fvNV(GL_VERTEX_PROGRAM_NV, 77, (float*)&Displacement.x);
//     glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 74, 100.0, 0, 0, 0);
    glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 78, 10000.0, 10000.0, 10000.0, 0);
    glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 80, 1, 0, 0, 0);
    glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 81, 3.0, 3.0, 3.0, 0);
    glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 82, 2.0, 2.0, 2.0, 0);
    glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 79, 0.03125, 32.0, 0.0, 0.0);


    ////////////////////////////////////////////
    //load the fragment shader

    {
        fragment_program = loadshaderfile(JahBasePath+"rfxcore/create_turbulence_texture_frag_gpu.fp");

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
        find_shader_program_error(fragment_program, "create_turbulence_texture_frag_gpu.fp");
    }

    ////////////////////////////////////////////
    //load the fragment shader with settings

    glEnable(GL_FRAGMENT_PROGRAM_NV);
    glBindProgramNV(GL_FRAGMENT_PROGRAM_NV, fragment_program_handle);

    ////////////////////////////////////////////
    //run the effects on the gpu now

    glGenTextures(number_of_layers, &texture_id[0]);

    glActiveTextureARB(GL_TEXTURE0_ARB);
    glBindTexture(GL_TEXTURE_2D, texture_id[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TURBULENCE_TEXTURE_SIZE, TURBULENCE_TEXTURE_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    glActiveTextureARB(GL_TEXTURE1_ARB);
    glBindTexture(GL_TEXTURE_2D, texture_id[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TURBULENCE_TEXTURE_SIZE, TURBULENCE_TEXTURE_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    glActiveTextureARB(GL_TEXTURE2_ARB);
    glBindTexture(GL_TEXTURE_2D, texture_id[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TURBULENCE_TEXTURE_SIZE, TURBULENCE_TEXTURE_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    glActiveTextureARB(GL_TEXTURE3_ARB);
    glBindTexture(GL_TEXTURE_2D, texture_id[3]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TURBULENCE_TEXTURE_SIZE, TURBULENCE_TEXTURE_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);


    ////////////////////////////////////////////
    //run the effects on the gpu now

    GLint           current_matrix_mode;

    glGetIntegerv(GL_MATRIX_MODE, &current_matrix_mode);
    glMatrixMode( GL_TEXTURE );
    glLoadIdentity();

    float                   width_ratio = float_width / TURBULENCE_TEXTURE_SIZE;
    float                   height_ratio = float_height / TURBULENCE_TEXTURE_SIZE;


    //draws texturized fire layers and blends them
    for (int layer = 0; layer < number_of_layers; layer++)
    {
        glPushMatrix();

//         if ((speed >= 10) || ((count % (10 - speed)) == 0))
        {
            noise_translate.x = (float)(-100 + (rand() % 200));
            noise_translate.y = (float)(-100 + (rand() % 200));
            noise_translate.z = (float)((rand() % 200)) / 300.0;
//             noise_translate.x = 100.0 - (float)((int)(noise_translate.x + 1.0) % 200);
//             noise_translate.y = 100.0 - (float)((int)(noise_translate.y + 1.0) % 200);
//             noise_translate.z = (float)(((int)(noise_translate.z + 0.01) * 100) % 100) / 100.0;
        }

            //animate noise by scaling and translating
        glScalef(noise_scale.x, noise_scale.y, noise_scale.z);
        glTranslatef(noise_translate.x, noise_translate.y, noise_translate.z);

        GLfloat                 x_unit_size = width / (float)horizontal_mesh_size;
        GLfloat                 y_unit_size = height / (float)vertical_mesh_size;

        float_vertical_mesh_size = (float)vertical_mesh_size;
        float_horizontal_mesh_size = (float)horizontal_mesh_size;

        glBegin(GL_TRIANGLES);

        for (jj = 0; jj < vertical_mesh_size; jj++)
        {
            j = (float)jj;

            for (ii = 0; ii < horizontal_mesh_size; ii++)
            {
                i = (float)ii;

                // First Triangle
                glMultiTexCoord4fARB(GL_TEXTURE0_ARB,
                    ((i + 0) / float_horizontal_mesh_size) * width_ratio,
                    ((j + 0) / float_vertical_mesh_size) * height_ratio, 0.0, 1.0);
                glVertex3f( (-float_width / 2.0) + (i * x_unit_size), (-float_height / 2.0) + (j * y_unit_size), 0.0);

                glMultiTexCoord4fARB(GL_TEXTURE0_ARB,
                    ((i + 1) / float_horizontal_mesh_size) * width_ratio,
                    ((j + 0) / float_vertical_mesh_size) * height_ratio, 0.0, 1.0);
                glVertex3f( (-float_width / 2.0) + ((i + 1) * x_unit_size), (-float_height / 2.0) + (j * y_unit_size), 0.0);

                glMultiTexCoord4fARB(GL_TEXTURE0_ARB,
                    ((i + 0) / float_horizontal_mesh_size) * width_ratio,
                    ((j + 1) / float_vertical_mesh_size) * height_ratio, 0.0, 1.0);
                glVertex3f( (-float_width / 2.0) + (i * x_unit_size), (-float_height / 2.0) + ((j + 1) * y_unit_size), 0.0);


                // Second Triangle
                glMultiTexCoord4fARB(GL_TEXTURE0_ARB,
                    ((i + 1) / float_horizontal_mesh_size) * width_ratio,
                    ((j + 1) / float_vertical_mesh_size) * height_ratio, 0.0, 1.0);
                glVertex3f( (-float_width / 2.0) + ((i + 1) * x_unit_size), (-float_height / 2.0) + ((j + 1) * y_unit_size), 0.0);

                glMultiTexCoord4fARB(GL_TEXTURE0_ARB,
                    ((i + 1) / float_horizontal_mesh_size) * width_ratio,
                    ((j + 0) / float_vertical_mesh_size) * height_ratio, 0.0, 1.0);
                glVertex3f( (-float_width / 2.0) + ((i + 1) * x_unit_size), (-float_height / 2.0) + (j * y_unit_size), 0.0);

                glMultiTexCoord4fARB(GL_TEXTURE0_ARB,
                    ((i + 0) / float_horizontal_mesh_size) * width_ratio,
                    ((j + 1) / float_vertical_mesh_size) * height_ratio, 0.0, 1.0);
                glVertex3f( (-float_width / 2.0) + (i * x_unit_size), (-float_height / 2.0) + ((j + 1) * y_unit_size), 0.0);

            }
        }

        glEnd(); //GL_QUADS

        glPopMatrix();   // GL_TEXTURE

        glBindTexture(GL_TEXTURE_2D, texture_id[layer]);
        glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (render_width - width) / 2,
                            (render_height - height) / 2, width, height);

        vertical_mesh_size *= 2;
        horizontal_mesh_size *= 2;
    }


    ////////////////////////////////////////////
    //now the combiner
    delete [] vertex_program;
    delete [] fragment_program;

    glDeleteProgramsNV(1, &vertex_program_handle);
    glDeleteProgramsNV(1, &fragment_program_handle);

    /////////////////////////////////////////////
    // load the shader

    {
        vertex_program = loadshaderfile(JahBasePath+"rfxcore/jahshaka_basic_vert.vp");

        if (!vertex_program)
        {
            return;
        }
    }

    glGenProgramsNV(1, &vertex_program_handle);
    glLoadProgramNV(GL_VERTEX_PROGRAM_NV, vertex_program_handle, strlen((char*)vertex_program), vertex_program);

    if (glGetError() == GL_INVALID_OPERATION)
    {
        find_shader_program_error(vertex_program, "jahshaka_basic_vert.vp");
    }

    ////////////////////////////////////////////
    // configure the shader
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
    glBindTexture(GL_TEXTURE_2D, texture_id[0]);

    glActiveTextureARB(GL_TEXTURE1_ARB);
    glBindTexture(GL_TEXTURE_2D, texture_id[1]);

    glActiveTextureARB(GL_TEXTURE2_ARB);
    glBindTexture(GL_TEXTURE_2D, texture_id[2]);

    glActiveTextureARB(GL_TEXTURE3_ARB);
    glBindTexture(GL_TEXTURE_2D, texture_id[3]);

    glActiveTextureARB(GL_TEXTURE4_ARB);
    glBindTexture(GL_TEXTURE_2D, texture_id[4]);

    ////////////////////////////////////////////
    //load the fragment shader

    {
        fragment_program = loadshaderfile(JahBasePath+"rfxcore/turbulence_combiner_frag_gpu.fp");

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
        find_shader_program_error(fragment_program, "turbulence_combiner_frag_gpu.fp");
    }

    ////////////////////////////////////////////
    //load the fragment shader with settings

    float scale_factor_x = 1.0 / (float)(TURBULENCE_TEXTURE_SIZE / width);
    float scale_factor_y = 1.0 / (float)(TURBULENCE_TEXTURE_SIZE / height);

    glProgramNamedParameter4fNV(fragment_program_handle, strlen("scale_factor"),
        (const GLubyte*)"scale_factor", scale_factor_x, scale_factor_y, 0.0f, 0.0f);

    glProgramNamedParameter4fNV(fragment_program_handle, strlen("transparency"),
        (const GLubyte*)"transparency", transparency, 0.0f, 0.0f, 0.0f);

    glProgramNamedParameter4fNV(fragment_program_handle, strlen("intensity"),
        (const GLubyte*)"intensity", intensity, 0.0f, 0.0f, 0.0f);

    glProgramNamedParameter4fNV(fragment_program_handle, strlen("number_of_layers"),
        (const GLubyte*)"number_of_layers", (float)number_of_layers, 0.0f, 0.0f, 0.0f);

    xcorrection = TURBULENCE_TEXTURE_SIZE / width;
    ycorrection = TURBULENCE_TEXTURE_SIZE / height;

    //////////////////////////////////////////////
    // run the effect

    glBindProgramNV(GL_FRAGMENT_PROGRAM_NV, fragment_program_handle);

    glBegin(GL_QUADS);

            glMultiTexCoord4fARB(GL_TEXTURE0_ARB, 0.0, 0.0, 0.0, 1.0);
            glMultiTexCoord4fARB(GL_TEXTURE1_ARB, 0.0, 0.0, 0.0, 1.0);
            glVertex2f( -float_width / 2.0, -float_height / 2.0);

            glMultiTexCoord4fARB(GL_TEXTURE0_ARB, width_ratio, 0.0, 0.0, 1.0);
            glMultiTexCoord4fARB(GL_TEXTURE1_ARB, 1.0, 0.0, 0.0, 1.0);
            glVertex2f( float_width / 2.0, -float_height / 2.0);

            glMultiTexCoord4fARB(GL_TEXTURE0_ARB, width_ratio, height_ratio, 0.0, 1.0);
            glMultiTexCoord4fARB(GL_TEXTURE1_ARB, 1.0, 1.0, 0.0, 1.0);
            glVertex2f( float_width / 2.0, float_height / 2.0);

            glMultiTexCoord4fARB(GL_TEXTURE0_ARB, 0.0, height_ratio, 0.0, 1.0);
            glMultiTexCoord4fARB(GL_TEXTURE1_ARB, 0.0, 1.0, 0.0, 1.0);
            glVertex2f( -float_width / 2.0, float_height / 2.0);

    glEnd();


    glGenTextures(1, &turbulence_texture);
    glBindTexture(GL_TEXTURE_2D, turbulence_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TURBULENCE_TEXTURE_SIZE, TURBULENCE_TEXTURE_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (render_width - width) / 2,
                        (render_height - height) / 2, width, height);



    ////////////////////////////////////////////
    //clean up on the way out
    glDisable(GL_VERTEX_PROGRAM_NV);
    glDisable(GL_FRAGMENT_PROGRAM_NV);
    delete [] vertex_program;
    delete [] fragment_program;

    glDeleteProgramsNV(1, &vertex_program_handle);
    glDeleteProgramsNV(1, &fragment_program_handle);

    glDeleteTextures(number_of_layers, &texture_id[0]);

    glMatrixMode(current_matrix_mode);

    noisecount++;
}



