/*******************************************************************************
**
** A Jahshaka RasFx Plugin
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** This is Open Source Software Released under the LGPL License
**
*******************************************************************************/

#include "rfxnvmosaic.h"
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

    pluginName = "Mosaic";
    pluginFamily  = "RasFx";            //family of plugin
    pluginGroup   = "Effect";           //group of plugin
    pluginClass   = JAH_GPU_PLUGIN;                  //class of plugin ie CPU, GPU, MeshFX

    QString guid = "a29a8943-e0b8-11d9-947b-00e08161165f";
    setGuid(guid);


    //we are using 1 slider
    usesliders = true;
    numsliders = 3;
    sliderlabel = new QString[numsliders];
    sliderlabel[0]="Horizontal";
    sliderlabel[1]="Vertical";
    sliderlabel[2]="Transparency";

    //for the headings
    uselabels = true;
    numlabels = 1;
    label = new QString[numlabels];
    label[0]="Mosaic";

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
    float                                       horizontal;
    float                                       vertical;
    float                                       transparency;

    GLint                                       width;
    GLint                                       height;

    GLuint                                      fragment_program_handle;

    unsigned char*                              fragment_program;

	float4										screen_offset;
	float4										image_size;

	float										openGL_screen_width = float( getScreenWidth() );
	float										openGL_screen_height = float( getScreenHeight() );

    width = (GLint)gpuwidth;
    height = (GLint)gpuheight;

	image_size.x = (float)width;
	image_size.y = (float)height;

	screen_offset.x = (openGL_screen_width - image_size.x) / 2.0f;
	screen_offset.y = (openGL_screen_height - image_size.y) / 2.0f;

    horizontal = (slider[1] / 10.0f) + 1.0f;
    vertical   = (slider[2] / 10.0f) + 1.0f;

    transparency = slider[3] / 100.0;

    glActiveTextureARB(GL_TEXTURE0_ARB);
    glBindTexture(GL_TEXTURE_2D, getSrcTextureId() );

    QString qualityshader;

    qualityshader="mosaic_frag.fp"; 

    fragment_program = loadshaderfile(JahBasePath+"rfxnvmosaic/"+qualityshader);

	if (!fragment_program)
	{
		return;
	}

    glGenProgramsNV(1, &fragment_program_handle);
    glLoadProgramNV(GL_FRAGMENT_PROGRAM_NV, fragment_program_handle,
                    strlen((char*)fragment_program), (const GLubyte*)fragment_program);

    if (glGetError() == GL_INVALID_OPERATION)
    {
        find_shader_program_error(fragment_program, (char*)qualityshader.ascii());
    }


    glProgramNamedParameter4fNV(fragment_program_handle, strlen("rectangle_size"),
        (const GLubyte*)"rectangle_size", horizontal, vertical, 0.0f, 0.0f);

     glProgramNamedParameter4fNV(fragment_program_handle, strlen("transparency"),
        (const GLubyte*)"transparency", transparency, 0.0f, 0.0f, 0.0f);

    glProgramNamedParameter4fNV(fragment_program_handle, strlen("image_size"),
        (const GLubyte*)"image_size", image_size.x, image_size.y, 0.0f, 0.0f);

    glProgramNamedParameter4fNV(fragment_program_handle, strlen("screen_offset"),
        (const GLubyte*)"screen_offset", screen_offset.x, screen_offset.y, 0.0f, 0.0f);

    glProgramNamedParameter4fNV(fragment_program_handle, strlen("texture_ratio"),
        (const GLubyte*)"texture_ratio", texwidthratio, texheightratio, 0.0f, 0.0f);

    glEnable(GL_FRAGMENT_PROGRAM_NV);
    glBindProgramNV(GL_FRAGMENT_PROGRAM_NV, fragment_program_handle);

	glBegin(GL_QUADS);
	{
		glVertex2f( -image_size.x / 2.0, -image_size.y / 2.0);
		glVertex2f( image_size.x / 2.0, -image_size.y / 2.0);
		glVertex2f( image_size.x / 2.0, image_size.y / 2.0);
		glVertex2f( -image_size.x / 2.0, image_size.y / 2.0);
	}
	glEnd(); //GL_QUADS

        glBindTexture(GL_TEXTURE_2D, getDestTextureId() );
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (render_width - width) / 2,
		(render_height - height) / 2, width, height);

    glDisable(GL_FRAGMENT_PROGRAM_NV);

    delete [] fragment_program;
    glDeleteProgramsNV(1, &fragment_program_handle);
}
