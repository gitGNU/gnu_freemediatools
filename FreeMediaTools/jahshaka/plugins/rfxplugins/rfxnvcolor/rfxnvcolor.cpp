/*******************************************************************************
**
** A Jahshaka RasFx Plugin
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** This is Open Source Software Released under the LGPL License
**
*******************************************************************************/

#include "rfxnvcolor.h"
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
    pluginName    = "Colorize";           //group of plugin
    pluginClass   = JAH_GPU_PLUGIN;     //class of plugin ie CPU, GPU, MeshFX

    QString guid = "6f4749cc-e0b8-11d9-947b-00e08161165f";
    setGuid(guid);


    //we are using 1 slider
    usesliders = true;
    numsliders = 9;
    sliderlabel = new QString[numsliders];
    sliderlabel[0]="Red";
    sliderlabel[1]="Green";
    sliderlabel[2]="Blue";
    sliderlabel[3]="Tint";
    sliderlabel[4]="Brightness";
    sliderlabel[5]="Strength";
    sliderlabel[6]="Hue";
    sliderlabel[7]="Saturation";
    sliderlabel[8]="Value";

    //for the headings
    uselabels = true;
    numlabels = 1;
    label = new QString[numlabels];
    label[0]="Colorize";

    ///////////////////////////////////////////////////////////
    //this plugin supports...
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

        if (USENV)     
        { 
            processGpuFx();         
            return; 
        }
        if (USEARB)    
        { 
            processGpuArbFx();      
            return; 
        }

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

    float                                       brightness;
	float4										color;
	float4										enhance;
	float4										hsv;

    GLint                                       width;
    GLint                                       height;

    GLuint                                      fragment_program_handle;

    unsigned char*                              fragment_program = NULL;

    static int                                  count = 0;

    ////////////////////////////////////////////
    //initialize variables and settings

    width = (GLint)gpuwidth;
    height = (GLint)gpuheight;

    float float_width = (float)width;
    float float_height = (float)height;


    color.x   = slider[1] / 1000.0f;
    color.y = slider[2] / 1000.0f;
    color.z  = slider[3] / 1000.0f;
    color.w  = slider[4] / 1000.0f;
    brightness = slider[5] / 1000.0f;
	enhance.y = (slider[6] / -1000.0f) + 1.0f;
	hsv.x = (slider[7] - 1) / 10;
	hsv.y = (slider[8] - 1) / 10;
	hsv.z = (slider[9] - 1) * 100;

	enhance.x = 0.0f;


    glActiveTextureARB(GL_TEXTURE0_ARB);
    glBindTexture(GL_TEXTURE_2D, getSrcTextureId() );


    ////////////////////////////////////////////
    //load the fragment shader

    if (count == 0)
    {
        fragment_program = loadshaderfile(JahBasePath+"rfxnvcolor/color_frag_gpu.fp");
    }

    glGenProgramsNV(1, &fragment_program_handle);
    glLoadProgramNV(GL_FRAGMENT_PROGRAM_NV, fragment_program_handle,
                    strlen((char*)fragment_program), (const GLubyte*)fragment_program);

    if (glGetError() == GL_INVALID_OPERATION)
    {
        find_shader_program_error(fragment_program, "color_frag_gpu.fp");
    }

    ////////////////////////////////////////////
    //load the fragment shader with settings


    glProgramNamedParameter4fNV(fragment_program_handle, strlen("color_scale"),
        (const GLubyte*)"color_scale", color.x, color.y, color.z, color.w);

    glProgramNamedParameter4fNV(fragment_program_handle, strlen("brightness"),
        (const GLubyte*)"brightness", brightness, 0.0f, 0.0f, 0.0f);

    glProgramNamedParameter4fNV(fragment_program_handle, strlen("enhance"),
        (const GLubyte*)"enhance", enhance.x, enhance.y, 0.0f, 0.0f);

    glProgramNamedParameter4fNV(fragment_program_handle, strlen("hsv"),
        (const GLubyte*)"hsv", hsv.x, hsv.y, hsv.z, 0.0f);

    glEnable(GL_FRAGMENT_PROGRAM_NV);
    glBindProgramNV(GL_FRAGMENT_PROGRAM_NV, fragment_program_handle);

    ////////////////////////////////////////////
    //run the effects on the gpu now

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

    glBindTexture(GL_TEXTURE_2D, getDestTextureId() );
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (render_width - width) / 2,
                        (render_height - height) / 2, width, height);

    ////////////////////////////////////////////
    //clean up on the way out

    glDisable(GL_FRAGMENT_PROGRAM_NV);

    glDeleteProgramsNV(1, &fragment_program_handle);
}

