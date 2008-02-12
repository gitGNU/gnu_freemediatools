/*******************************************************************************
**
** A Jahshaka RasFx Plugin
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** This is Open Source Software Released under the LGPL License
**
*******************************************************************************/

#include "rfxnvlighting.h"
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
    pluginName    = "Lighting";         //name of plugin
    pluginClass   = JAH_GPU_PLUGIN;     //class of plugin ie CPU, GPU, MeshFX

    QString guid = "96fb1283-e0b8-11d9-947b-00e08161165f";
    setGuid(guid);


    //we are using 1 slider
    usesliders = true;
    numsliders = 9;
    sliderlabel = new QString[numsliders];
    sliderlabel[0]="Light X Position";
    sliderlabel[1]="Light Y Position";
    sliderlabel[2]="Light Z Position";
    sliderlabel[3]="Red";
    sliderlabel[4]="Green";
    sliderlabel[5]="Blue";
    sliderlabel[6]="Density";
    sliderlabel[7]="Exponent";
    sliderlabel[8]="Darker";


    //for the headings
    uselabels = true;
    numlabels = 1;
    label = new QString[numlabels];
    label[0]="NV30 Lighting";

    //important gpu support variable here
    HASNV  = true;        //set to true to test <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    HASARB = false;

    //for mesh support
    usesMesh = true;
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

    float4                                      light_position;
    float4                                      color;

    double                                      density;
    static double                               previous_density = 0.0;
    double                                      exponent;
    static double                               previous_exponent = 0.0;
    double                                      darker;

    GLuint                                      vertex_program_handle;
    GLuint                                      fragment_program_handle;

    unsigned char*                              vertex_program;
    unsigned char*                              fragment_program;

    int                                         row;
    int                                         column;

    float                                       f_row;
    float                                       f_column;

    float2                                      lower_left_tex_coords;
    float2                                      lower_right_tex_coords;
    float2                                      upper_left_tex_coords;
    float2                                      upper_right_tex_coords;

    float4                                      lower_left_vertex_coords;
    float4                                      lower_right_vertex_coords;
    float4                                      upper_left_vertex_coords;
    float4                                      upper_right_vertex_coords;

    float4                                      lower_left_normal_coords;
    float4                                      lower_right_normal_coords;
    float4                                      upper_left_normal_coords;
    float4                                      upper_right_normal_coords;

    ////////////////////////////////////////////
    //initialize main variables and settings

    width  = (GLint)gpuwidth;
    height = (GLint)gpuheight;

    ////////////////////////////////////////////
    //initialize main variables and settings

    light_position.x = (slider[1] * 10) + 200.0;
    light_position.y = (slider[2] * 10);
    light_position.z = (slider[3] * 50) + 20000.0;
    light_position.w = 1.0;

    color.x = (slider[4] / 1000.0) + 1.0;
    color.y = (slider[5] / 1000.0) + 1.0;
    color.z = (slider[6] / 1000.0) + 1.0;
    color.w = 1.0;

    density  = (double)(((slider[7])) + 2.0);
    exponent = (double)(((slider[8] - 1.0)) + 3.5);
    darker   = (double)(((slider[9] - 1.0) / 10.0) + 0.1);
    //printf("darker = %f\n", darker);

    ////////////////////////////////////////////////////

    vertex_program = loadshaderfile(JahBasePath+"rfxnvlighting/lighting_vert_gpu.vp");

    if (!vertex_program)
    {
        return;
    }

    glGenProgramsNV(1, &vertex_program_handle);
    glLoadProgramNV(GL_VERTEX_PROGRAM_NV, vertex_program_handle, strlen((char*)vertex_program), vertex_program);

    if (glGetError() == GL_INVALID_OPERATION)
    {
        find_shader_program_error(vertex_program, "lighting_vert_gpu.vp");
    }
    ///////////////////////////////////////////////////

    float               modelview[16];
    float               projection[16];
    float               mvp[16];
    float               modelviewIT[16];
    float               texture[16];

    //glTrackMatrixNV(GL_VERTEX_PROGRAM_NV, 0, GL_MODELVIEW_PROJECTION_NV, GL_IDENTITY_NV);
    //glTrackMatrixNV(GL_VERTEX_PROGRAM_NV, 4, GL_MODELVIEW, GL_IDENTITY_NV);
    //glTrackMatrixNV(GL_VERTEX_PROGRAM_NV, 8, GL_MODELVIEW, GL_INVERSE_TRANSPOSE_NV);

    getMVPMatrices(modelview, projection, mvp, modelviewIT, texture);
    glProgramParameters4fvNV(GL_VERTEX_PROGRAM_NV, 0, 4, mvp);
    glProgramParameters4fvNV(GL_VERTEX_PROGRAM_NV, 4, 4, modelview);
    glProgramParameters4fvNV(GL_VERTEX_PROGRAM_NV, 8, 4, modelviewIT);
    glEnable(GL_VERTEX_PROGRAM_NV);
    glBindProgramNV(GL_VERTEX_PROGRAM_NV, vertex_program_handle);

    glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 12, 0.0, 1.0, 1.0, 1.0);
    glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 13, 0.0, 1.0, 1.0, 1.0);
    glProgramParameter4fvNV(GL_VERTEX_PROGRAM_NV, 14, (float*)&light_position);
    glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 15, width, 0, 0, 0);
    glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 16, height, 0, 0, 0);
    glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 17, camera_distance, 0, 0, 0);

    glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 18, 2, 0, 1, 0);
    glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 19, 0.5, 0.5, 0, 0);

    glActiveTextureARB(GL_TEXTURE0_ARB);
    glBindTexture(GL_TEXTURE_2D,  getSrcTextureId() );

    glActiveTextureARB(GL_TEXTURE1_ARB);
    glBindTexture(GL_TEXTURE_2D, contrast_texture);

    glActiveTextureARB(GL_TEXTURE2_ARB);
    glBindTexture(GL_TEXTURE_2D, random_texture);



    /////////////////////////////////////////////////////////////////

    fragment_program = loadshaderfile(JahBasePath+"rfxnvlighting/lighting_frag_gpu.fp");

    if (!fragment_program)
    {
        return;
    }

    glGenProgramsNV(1, &fragment_program_handle);
    glLoadProgramNV(GL_FRAGMENT_PROGRAM_NV, fragment_program_handle,
                    strlen((char*)fragment_program), (const GLubyte*)fragment_program);

    if (glGetError() == GL_INVALID_OPERATION)
    {
        find_shader_program_error(fragment_program, "lighting_frag_gpu.fp");
    }

    ////////////////////////////////////////////////////////////////////

   glEnable(GL_FRAGMENT_PROGRAM_NV);
   glBindProgramNV(GL_FRAGMENT_PROGRAM_NV, fragment_program_handle);

   glProgramNamedParameter4fNV(fragment_program_handle, strlen("light_color"),
                              (const GLubyte*)"light_color", color.x, color.y, color.z, color.w);

    ///////////////////////////////////////////////////

    glBegin(GL_QUADS);

    for (row = 0; row < MESH_Y_DIMENSION; row++)
    {
        f_row = (float)row;

        for (column = 0; column < MESH_X_DIMENSION; column++)
        {

            f_column = (float)column;

            lower_left_tex_coords.x = (f_column + 0.0) / MESH_X_DIMENSION;
            lower_left_tex_coords.y = (f_row + 0.0) / MESH_Y_DIMENSION;

            lower_right_tex_coords.x = (f_column + 1.0) / MESH_X_DIMENSION;
            lower_right_tex_coords.y = (f_row + 0.0) / MESH_Y_DIMENSION;

            upper_right_tex_coords.x = (f_column + 1.0) / MESH_X_DIMENSION;
            upper_right_tex_coords.y = (f_row + 1.0) / MESH_Y_DIMENSION;

            upper_left_tex_coords.x = (f_column + 0.0) / MESH_X_DIMENSION;
            upper_left_tex_coords.y = (f_row + 1.0) / MESH_Y_DIMENSION;



            get_mesh_normal(lower_left_normal_coords, column, row, MESH_X_DIMENSION, MESH_Y_DIMENSION);
            get_mesh_normal(lower_right_normal_coords, column + 1, row, MESH_X_DIMENSION, MESH_Y_DIMENSION);
            get_mesh_normal(upper_right_normal_coords, column + 1, row + 1, MESH_X_DIMENSION, MESH_Y_DIMENSION);
            get_mesh_normal(upper_left_normal_coords, column, row + 1, MESH_X_DIMENSION, MESH_Y_DIMENSION);

            getMeshCoord(column, row,
                                                    lower_left_vertex_coords.x,
                                                    lower_left_vertex_coords.y,
                                                    lower_left_vertex_coords.z);

            getMeshCoord(column + 1, row,
                                                    lower_right_vertex_coords.x,
                                                    lower_right_vertex_coords.y,
                                                    lower_right_vertex_coords.z);

            getMeshCoord(column + 1, row + 1,
                                                    upper_right_vertex_coords.x,
                                                    upper_right_vertex_coords.y,
                                                    upper_right_vertex_coords.z);

            getMeshCoord(column, row + 1,
                                                    upper_left_vertex_coords.x,
                                                    upper_left_vertex_coords.y,
                                                    upper_left_vertex_coords.z);


            glMultiTexCoord4fARB(GL_TEXTURE0_ARB, lower_left_tex_coords.x * texwidthratio,
                                                lower_left_tex_coords.y * texheightratio, 0.0, 1.0); 
            glMultiTexCoord4fARB(GL_TEXTURE1_ARB, lower_left_tex_coords.x, lower_left_tex_coords.y, 0.0, 1.0);
            glMultiTexCoord4fARB(GL_TEXTURE2_ARB, lower_left_tex_coords.x / density, lower_left_tex_coords.y, 0.0, 1.0); 
            glNormal3f(lower_left_normal_coords.x, lower_left_normal_coords.y, lower_left_normal_coords.z);
//             glVertex3f(lower_left_vertex_coords.x, lower_left_vertex_coords.y, lower_left_vertex_coords.z);
            glVertex2f(lower_left_vertex_coords.x, lower_left_vertex_coords.y);

            glMultiTexCoord4fARB(GL_TEXTURE0_ARB, lower_right_tex_coords.x * texwidthratio, 
                                                lower_right_tex_coords.y * texheightratio, 0.0, 1.0);
            glMultiTexCoord4fARB(GL_TEXTURE1_ARB, lower_right_tex_coords.x, lower_right_tex_coords.y, 0.0, 1.0);
            glMultiTexCoord4fARB(GL_TEXTURE2_ARB, lower_right_tex_coords.x / density, lower_right_tex_coords.y, 0.0, 1.0);
            glNormal3f(lower_right_normal_coords.x, lower_right_normal_coords.y, lower_right_normal_coords.z);
//             glVertex3f(lower_right_vertex_coords.x, lower_right_vertex_coords.y, lower_right_vertex_coords.z);
            glVertex2f(lower_right_vertex_coords.x, lower_right_vertex_coords.y);

            glMultiTexCoord4fARB(GL_TEXTURE0_ARB, upper_right_tex_coords.x * texwidthratio, 
                                                upper_right_tex_coords.y * texheightratio, 0.0, 1.0);
            glMultiTexCoord4fARB(GL_TEXTURE1_ARB, upper_right_tex_coords.x, upper_right_tex_coords.y, 0.0, 1.0); 
            glMultiTexCoord4fARB(GL_TEXTURE2_ARB, upper_right_tex_coords.x / density, upper_right_tex_coords.y, 0.0, 1.0);
            glNormal3f(upper_right_normal_coords.x, upper_right_normal_coords.y, upper_right_normal_coords.z);
//             glVertex3f(upper_right_vertex_coords.x, upper_right_vertex_coords.y, upper_right_vertex_coords.z);
            glVertex2f(upper_right_vertex_coords.x, upper_right_vertex_coords.y);

            glMultiTexCoord4fARB(GL_TEXTURE0_ARB, upper_left_tex_coords.x * texwidthratio, 
                                                upper_left_tex_coords.y * texheightratio, 0.0, 1.0);
            glMultiTexCoord4fARB(GL_TEXTURE1_ARB, upper_left_tex_coords.x, upper_left_tex_coords.y, 0.0, 1.0); 
            glMultiTexCoord4fARB(GL_TEXTURE2_ARB, upper_left_tex_coords.x / density, upper_left_tex_coords.y, 0.0, 1.0);
            glNormal3f(upper_left_normal_coords.x, upper_left_normal_coords.y, upper_left_normal_coords.z);
//             glVertex3f(upper_left_vertex_coords.x, upper_left_vertex_coords.y, upper_left_vertex_coords.z);
            glVertex2f(upper_left_vertex_coords.x, upper_left_vertex_coords.y);


        }

       // debug(rowdata);
    }


    glEnd();


    glActiveTextureARB(GL_TEXTURE0_ARB);
    glBindTexture(GL_TEXTURE_2D, getDestTextureId() );
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (render_width - width) / 2,
                        (render_height - height) / 2, width, height);


    //////////////////////////////////////////////////
    // clean up

    glDisable(GL_VERTEX_PROGRAM_NV);
    glDisable(GL_FRAGMENT_PROGRAM_NV);

    delete [] vertex_program;
    delete [] fragment_program;

    glDeleteProgramsNV(1, &vertex_program_handle);
    glDeleteProgramsNV(1, &fragment_program_handle);

    previous_density = density;
    previous_exponent = exponent;


}




