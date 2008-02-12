/*******************************************************************************
**
** A Jahshaka RasFx Plugin
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** This is Open Source Software Released under the LGPL License
**
*******************************************************************************/

#include "rfxnvcompoundeye.h"
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
    pluginName    = "Compoundeye";      //name of plugin
    pluginClass   = JAH_GPU_PLUGIN;     //class of plugin ie CPU, GPU, MeshFX

    QString guid = "6f4749cd-e0b8-11d9-947b-00e08161165f";
    setGuid(guid);


    //we are using 1 slider
    usesliders = true;
    numsliders = 6;
    sliderlabel = new QString[numsliders];
    sliderlabel[0]="X Center";
    sliderlabel[1]="Y Center";
    sliderlabel[2]="Transparency";
    sliderlabel[3]="Zoom";
    sliderlabel[4]="Gamma";
    sliderlabel[5]="Number Of Eyes";


    //for the headings
    uselabels = true;
    numlabels = 1;
    label = new QString[numlabels];
    label[0]="NV30 Compoundeye";

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
    float                                       fisheye_radius;
    float                                       xcenter;
    float                                       ycenter;
    float                                       gamma;
    float                                       transparency;
    float                                       number_of_lenses;

    GLint                                       width;
    GLint                                       height;

//    GLuint                                      vertex_program_handle;
    GLuint                                      fragment_program_handle;

//    unsigned char*                              vertex_program;
    unsigned char*                              fragment_program;

    width = (GLint)gpuwidth;
    height = (GLint)gpuheight;

    float float_width = (float)width;
    float float_height = (float)height;

    xcenter = (slider[1] + (float_width / 2.0)) / float_width;
    ycenter = (slider[2] + (float_height / 2.0)) / float_height;
    transparency = (slider[3] / 100.0) + 1.0;

    fisheye_radius = ((slider[4] * 3.0) + (float_width / 5)) / float_width;
    gamma  = (slider[5] * 0.01) + 1.0;
    number_of_lenses  = (slider[6] / 10.0) + 3.0;


    glActiveTextureARB(GL_TEXTURE0_ARB);
    glBindTexture(GL_TEXTURE_2D,  getSrcTextureId() );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

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

            glMultiTexCoord4fARB(GL_TEXTURE0_ARB, texwidthratio,
                                        texheightratio, 0.0, 1.0);
            glVertex2f( float_width / 2.0, float_height / 2.0);

            glMultiTexCoord4fARB(GL_TEXTURE0_ARB, 0.0, texheightratio, 0.0, 1.0);
            glVertex2f( -float_width / 2.0, float_height / 2.0);


    glEnd(); //GL_QUADS


    glBindTexture(GL_TEXTURE_2D, getDestTextureId() );
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (render_width - width) / 2,
                        (render_height - height) / 2, width, height);

    glDisable(GL_FRAGMENT_PROGRAM_NV);
    delete [] fragment_program;
    glDeleteProgramsNV(1, &fragment_program_handle);

}



