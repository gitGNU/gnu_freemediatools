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


void    MyPlugin::isComplexArbSupported(void)
{
	QString sourceName = tempBasePath+"plugins/rfxplugins/rfxnvcolor/color_frag_arb_gpu.fp";
	HASARB =  checkComplexArbSupport(sourceName, max_fp_instructions, max_vp_instructions );
}


void    MyPlugin::processGpuArbFx(void)
{
    float                                       brightness;
	float4										color;
	float4										enhance;
	float4										hsv;

    GLint                                       width;
    GLint                                       height;

    GLuint                                      fragment_program_handle;

    unsigned char*                              fragment_program;

//     fxtimer->starttime();

    width = (GLint)gpuwidth;
    height = (GLint)gpuheight;

    float float_width = (float)width;
    float float_height = (float)height;

	//debug("ARB color");

    color.x   	= (slider[1] / 1000.0f) - 0.001f;  // Offset fixes Cg bug
    color.y 	= (slider[2] / 1000.0f);
    color.z  	= (slider[3] / 1000.0f);
    color.w  	= (slider[4] / 1000.0f);
    brightness 	= (slider[5] / 1000.0f);
	enhance.y 	= (slider[6] / -1000.0f) + 1.0f;
	hsv.x 		= (slider[7] - 1) / 10;
	hsv.y 		= (slider[8] - 1) / 10;
	hsv.z 		= (slider[9] - 1) * 100;

	enhance.x = 0.0f;


    glActiveTextureARB(GL_TEXTURE0_ARB);
    glBindTexture(GL_TEXTURE_2D,  getSrcTextureId() );

    fragment_program = loadshaderfile(JahBasePath+"rfxnvcolor/color_frag_arb_gpu.fp");

    glGenProgramsARB(1, &fragment_program_handle);
    glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, fragment_program_handle);
    glProgramStringARB( GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB,
                        strlen((char*)fragment_program), (const GLubyte*)fragment_program);

    if (glGetError() == GL_INVALID_OPERATION) 
    {
        find_shader_program_error(fragment_program, "color_frag_arb_gpu.fp");
    }

    glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 0, color.x, color.y, color.z, 0.0f);
    glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 1, brightness, 0.0f, 0.0f, 0.0f);
    glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 2, enhance.x, enhance.y, 0.0f, 0.0f);
    glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 3, hsv.x, hsv.y, hsv.z, 0.0f);

    glEnable(GL_FRAGMENT_PROGRAM_ARB);

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

    glEnd();

    glBindTexture(GL_TEXTURE_2D, getDestTextureId() );
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (render_width - width) / 2, 
                        (render_height - height) / 2, width, height);

    glDisable(GL_FRAGMENT_PROGRAM_ARB);

    delete [] fragment_program;

    glDeleteProgramsARB(1, &fragment_program_handle);

//     fxtimer->endtime();
//     fxtimer->printtime("Time for ARB Color: ");
}

