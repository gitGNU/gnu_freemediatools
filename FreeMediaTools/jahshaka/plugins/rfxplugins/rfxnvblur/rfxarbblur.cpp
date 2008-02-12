/*******************************************************************************
**
** A Jahshaka RasFx Plugin
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** This is Open Source Software Released under the LGPL License
**
*******************************************************************************/

#include "rfxnvblur.h"
#include <math.h>

void    MyPlugin::isComplexArbSupported(void)
{
	QString sourceName = tempBasePath+"plugins/rfxplugins/rfxnvblur/blur_frag_arb_gpu.fp";
	HASARB =  checkComplexArbSupport(sourceName, max_fp_instructions, max_vp_instructions );
}


void    MyPlugin::processGpuArbFx(void)
{
    float                                       horizontal;
    float                                       vertical;
    float                                       stretch;
    float                                       transparency;
    float                                       weight;

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

    horizontal = slider[1] / image_size.x;
    vertical   = slider[2] / image_size.y;

    stretch = slider[4] + 10.0f;
    transparency = slider[5] / 100.0;
    weight  = (slider[6] / 10.0) + 0.5;

    glActiveTextureARB(GL_TEXTURE0_ARB);
    glBindTexture(GL_TEXTURE_2D, getSrcTextureId() );

    QString qualityshader;

    qualityshader="blur_frag_arb_gpu.fp"; 

    fragment_program = loadshaderfile(JahBasePath + "rfxnvblur/" + qualityshader);

	if (!fragment_program)
	{
		return;
	}

    glGenProgramsARB(1, &fragment_program_handle);
    glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, fragment_program_handle);
    glProgramStringARB( GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB,
                        strlen((char*)fragment_program), (const GLubyte*)fragment_program);

    if (glGetError() == GL_INVALID_OPERATION)
    {
        find_shader_program_error(fragment_program, (char*)qualityshader.ascii());
    }


    glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 0, horizontal, 0.0f, 0.0f, 0.0f);
    glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 1, vertical, 0.0f, 0.0f, 0.0f);
    glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 2, stretch, 0.0f, 0.0f, 0.0f);
    glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 6, transparency, 0.0f, 0.0f, 0.0f);
    glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 7, screen_offset.x, screen_offset.y, 0.0f, 0.0f);
    glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 8, image_size.x, image_size.y, 0.0f, 0.0f);
    glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 9, texwidthratio, texheightratio, 0.0f, 0.0f);

    glEnable(GL_FRAGMENT_PROGRAM_ARB);

    for (int counter = 0; counter < int(stretch); counter++)
    {
		float ratio  = float(counter) / stretch;

		weight = ratio;

    	glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 3, weight, 0.0f, 0.0f, 0.0f);
    	glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 4, counter, 0.0f, 0.0f, 0.0f);
    	glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 5, ratio, 0.0f, 0.0f, 0.0f);

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
    }

    glDisable(GL_FRAGMENT_PROGRAM_ARB);

    delete [] fragment_program;

    glDeleteProgramsARB(1, &fragment_program_handle);

//     fxtimer->endtime();
//     fxtimer->printtime("Time for ARB Blur: ");
}

