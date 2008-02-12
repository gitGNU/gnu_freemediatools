/*******************************************************************************
**
** A Jahshaka RasFx Plugin
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** This is Open Source Software Released under the LGPL License
**
*******************************************************************************/

#include "rfxnvfisheye.h"
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
    pluginName    = "Fisheye";          //name of plugin
    pluginClass   = JAH_GPU_PLUGIN;     //class of plugin ie CPU, GPU, MeshFX

    QString guid = "8d7ff89e-e0b8-11d9-947b-00e08161165f";
    setGuid(guid);


    //we are using 1 slider
    usesliders = true;
    numsliders = 5;
    sliderlabel = new QString[numsliders];
    sliderlabel[0]="X Center";
    sliderlabel[1]="Y Center";
    sliderlabel[2]="Radius";
    sliderlabel[3]="Gamma";
    sliderlabel[4]="Transparency";


    //for the headings
    uselabels = true;
    numlabels = 1;
    label = new QString[numlabels];
    label[0]="NV30 Fisheye";

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


void    MyPlugin::processGpuFx(void)
{
    ////////////////////////////////////////////
    //main imageprocessing loop

    float                                       fisheye_radius;
    float                                       xcenter;
    float                                       ycenter;
    float                                       gamma;
    float                                       transparency;
    float                                       number_of_lenses;

    GLint                                       width;
    GLint                                       height;

    static GLuint                               vertex_program_handle;
    static GLuint                               fragment_program_handle;

    static unsigned char*                       vertex_program;
    static unsigned char*                       fragment_program;

    width = (GLint)gpuwidth;
    height = (GLint)gpuheight;

    float float_width = (float)width;
    float float_height = (float)height;

    xcenter = (slider[1] + (float_width / 2.0)) / float_width;
    ycenter = (slider[2] + (float_height / 2.0)) / float_height;

    fisheye_radius = ((slider[3] * 3.0) + (float_width / 5)) / float_width;
    gamma = (slider[4] * 0.01) + 2.0;
    transparency = (100 - slider[5]) / 100.0;

    number_of_lenses = 1.0;


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
    glBindTexture(GL_TEXTURE_2D, getSrcTextureId() );

    {
        fragment_program = loadshaderfile(JahBasePath+"rfxnvfisheye/fisheye_frag_gpu.fp");

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
        find_shader_program_error(fragment_program, "fisheye_frag_gpu.fp");
    }

    glProgramNamedParameter4fNV(fragment_program_handle, strlen("fisheye_radius"),
        (const GLubyte*)"fisheye_radius", fisheye_radius, 0.0f, 0.0f, 0.0f);

    glProgramNamedParameter4fNV(fragment_program_handle, strlen("xcenter"),
        (const GLubyte*)"xcenter", xcenter, 0.0f, 0.0f, 0.0f);

    glProgramNamedParameter4fNV(fragment_program_handle, strlen("ycenter"),
        (const GLubyte*)"ycenter", ycenter, 0.0f, 0.0f, 0.0f);

    glProgramNamedParameter4fNV(fragment_program_handle, strlen("gamma"),
        (const GLubyte*)"gamma", gamma, 0.0f, 0.0f, 0.0f);

    glProgramNamedParameter4fNV(fragment_program_handle, strlen("transparency"),
        (const GLubyte*)"transparency", transparency, 0.0f, 0.0f, 0.0f);

    glProgramNamedParameter4fNV(fragment_program_handle, strlen("size_ratio"),
        (const GLubyte*)"size_ratio",
        texwidthratio,
        texheightratio, 0.0f, 0.0f);

    glProgramNamedParameter4fNV(fragment_program_handle, strlen("number_of_lenses"), 
        (const GLubyte*)"number_of_lenses", number_of_lenses, 0.0f, 0.0f, 0.0f);

    glEnable(GL_FRAGMENT_PROGRAM_NV);
    glBindProgramNV(GL_FRAGMENT_PROGRAM_NV, fragment_program_handle);


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


    glBindTexture(GL_TEXTURE_2D, getDestTextureId() );
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (render_width - width) / 2,
                        (render_height - height) / 2, width, height);

    glDisable(GL_VERTEX_PROGRAM_NV);
    glDisable(GL_FRAGMENT_PROGRAM_NV);

    glDeleteProgramsNV(1, &vertex_program_handle);
    glDeleteProgramsNV(1, &fragment_program_handle);

}



