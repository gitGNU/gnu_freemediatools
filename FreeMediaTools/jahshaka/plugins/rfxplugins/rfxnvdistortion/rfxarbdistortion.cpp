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



void    MyPlugin::processGpuArbFx(void)
{
    ////////////////////////////////////////////
    //main imageprocessing loop

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

    static int                                  count = 0;

    static float4                               noise_scale;
    static float4                               noise_translate;

    float2                                      movement;

    int                                         ii, jj;
    GLfloat                                     i, j;

    int                                         num_distortion_layers;
    int                                         speed;

    static bool                                 noiseMatrixInitialized = FALSE;

    float               modelview[16];
    float               projection[16];
    float               mvp[16];
    float               modelviewIT[16];
    float               texture[16];


    //////////////////////////////////////////////////////////
    //we need to send over...

    width  = (GLint)gpuwidth;
    height = (GLint)gpuheight;

    float float_width = (float)width;
    float float_height = (float)height;

    //////////////////////////////////////////////////////////

    Displacement.x = -(slider[1] / 1.0);
    Displacement.y = -(slider[2] / 1.0);
    Displacement.z = (slider[3]);

    num_distortion_layers = (int)(1.0 + (slider[4] / 10.0));
    speed = 10 + (int)(slider[5] / 10.0);
    no_distortion_radius = slider[6];

    horizontal_mesh_size = ((int)(slider[7] * 100.0));
    horizontal_mesh_size = (horizontal_mesh_size > width) ? width : horizontal_mesh_size;

    vertical_mesh_size = ((int)(slider[8] * 100.0));
    vertical_mesh_size = (vertical_mesh_size > height) ? height : vertical_mesh_size;

    movement.x = (slider[9] * 10.0) + 100.0;
    movement.y = movement.x;

    float_horizontal_mesh_size = (float)horizontal_mesh_size;
    float_vertical_mesh_size = (float)vertical_mesh_size;

    noise_scale.x = 1.3f;
    noise_scale.y = 1.3f;
    noise_scale.z = 1.3f;

    //////////////////////////////////////////////////////////

    glActiveTextureARB(GL_TEXTURE0_ARB);
    glBindTexture(GL_TEXTURE_2D,  getSrcTextureId() );

    if (!noiseMatrixInitialized)
    {
        initVertexNoiseConstants(DISTORTION_TABLE_SIZE, permutation_table, gradient_table, GRADIENT_TABLE_SIZE);
        noiseMatrixInitialized = true;
    }

    count++;

    {
        vertex_program = loadshaderfile(JahBasePath+"rfxnvdistortion/distortion_vert_arb_gpu.vp");

        if (!vertex_program)
        {
            return;
        }
    }

    glGenProgramsARB(1, &vertex_program_handle);
    glBindProgramARB(GL_VERTEX_PROGRAM_ARB, vertex_program_handle);
    glProgramStringARB( GL_VERTEX_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB,
                        strlen((char*)vertex_program), (const GLubyte*)vertex_program);

    if (glGetError() == GL_INVALID_OPERATION)
    {
        find_shader_program_error(vertex_program, "distortion_vert_arb_gpu.vp");
    }

    /////////////////////////////////////////////////////////

    for (int index = 0; index < (DISTORTION_TABLE_SIZE * 2); index ++)
    {
        glProgramLocalParameter4fvARB(GL_VERTEX_PROGRAM_ARB, 9 + index, (float*)&gradient_table[index]);
    }

    glProgramLocalParameter4fvARB(GL_VERTEX_PROGRAM_ARB, 9 + (DISTORTION_TABLE_SIZE * 2), (float*)&gradient_table[0]);
    glProgramLocalParameter4fvARB(GL_VERTEX_PROGRAM_ARB, 9 + (DISTORTION_TABLE_SIZE * 2) + 1, (float*)&gradient_table[1]);
    glProgramLocalParameter4fvARB(GL_VERTEX_PROGRAM_ARB, 76, (float*)&Displacement.x);
    glProgramLocalParameter4fARB(GL_VERTEX_PROGRAM_ARB, 77, no_distortion_radius, 0.0, 0.0, 0.0);

    glEnable(GL_VERTEX_PROGRAM_ARB);

    //////////////////////////////////////////////////////////////////////////
    // for the fragment program

    {
        fragment_program = loadshaderfile(JahBasePath+"rfxnvdistortion/distortion_frag_arb_gpu.fp");

        if (!fragment_program)
        {
            return;
        }
    }

    glGenProgramsARB(1, &fragment_program_handle);
    glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, fragment_program_handle);
    glProgramStringARB( GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB,
                        strlen((char*)fragment_program), (const GLubyte*)fragment_program);

    if (glGetError() == GL_INVALID_OPERATION)
    {
        find_shader_program_error(fragment_program, "distortion_frag_arb_gpu.fp");
    }

    glEnable(GL_FRAGMENT_PROGRAM_ARB);

    check_gl();


    //////////////////////////////////////////////////////////////////////////

    GLint           current_matrix_mode;

    glGetIntegerv(GL_MATRIX_MODE, &current_matrix_mode);
    glMatrixMode( GL_TEXTURE );
    glLoadIdentity();

    for (int layer = 0; layer < num_distortion_layers; ++layer)
    {
        glPushMatrix();

//         if ((speed >= 10) || ((count % (10 - speed)) == 0))
        {
//             noise_translate.x = (float)(-100 + (count % 200)) / movement.x;
//             noise_translate.y = (float)(-100 + (count % 200)) / movement.y;
//             noise_translate.z = (float)((count % 200)) / 300.0;
        }

        noise_translate.x = (float)count / movement.x;
        noise_translate.y = (float)count / movement.y;
        noise_translate.z = (float)count / 300.0 / movement.y;


            //animate noise by scaling and translating
        glScalef(noise_scale.x, noise_scale.y, noise_scale.z);
        glTranslatef(noise_translate.x, noise_translate.y, noise_translate.z);

        getMVPMatrices(modelview, projection, mvp, modelviewIT, texture);

        for (int index = 0; index < 4; index++)
        {
            glProgramLocalParameter4fvARB(GL_VERTEX_PROGRAM_ARB, index, (float*)&mvp[index * 4]);
            glProgramLocalParameter4fvARB(GL_VERTEX_PROGRAM_ARB, index + 4, (float*)&texture[index * 4]);
        }

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



    /////////////////////////////////////////////////////////////////////

    glDisable(GL_VERTEX_PROGRAM_ARB);
    glDisable(GL_FRAGMENT_PROGRAM_ARB);

    delete [] vertex_program;
    delete [] fragment_program;

    count++;

    glDeleteProgramsARB(1, &vertex_program_handle);
    glDeleteProgramsARB(1, &fragment_program_handle);

    glMatrixMode(current_matrix_mode);
}



