/*******************************************************************************
**
** The header file for the Jahshaka jahplayer module 
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "videohead.h"

VideoHead::VideoHead(QWidget *parent, const char* name)
    :GLCore (parent, name)
{
    //so we can use the tracer
    jtrace = JahTrace::getInstance();	//set up tracer

	//basic variables
    currentframe		= 0;
	cacheImage			= 0;
    frameWidth		    = 0;
    frameHeight		    = 0;
    zoommode            = true;
	playerisactive		= false;
	staticPlayer		= false;

	///////////////////////////////////////
	//fliping color data uses the cpu and is slower
    
    #ifdef EXP_TEXTURES_PLAYER
    if (glewIsSupported("GL_EXT_bgra"))
    {
        flipColorData = false;
    }
    else
    {
        flipColorData = true;
    }
    #else
    flipColorData = true;
    #endif

    //set the texture modes accordingly
    if (flipColorData)
    {
        texture_format = GL_RGBA;
        texture_mode = GL_RGBA; //1?
    }
    else
    {
        texture_format = GL_BGRA_EXT;
        texture_mode = 4;
    }
}


VideoHead::~VideoHead()
{
    currentframe		= 0;
    delete currentframe;
}

void VideoHead::Reset()
{
	//reset variables
	playerisactive		= false;
	currentframe		= 0;
    frameWidth		    = 0;
    frameHeight		    = 0;

    
	///////////////////////////////////////////////
	//set up opengl
	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping ( NEW )
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor( 0.3f, 0.3f, 0.3f, 0.0 );				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	updateGL();

}

bool VideoHead::InitializePlayer(QImage * theimage, bool status)
{
    //reset the engine
	Reset();

	staticPlayer = status;

	currentframe = theimage;
	currentframe->detach();
	
	QImage thecacheimage = theimage->copy();
	thecacheimage.detach();

	if (cacheImage)
		delete cacheImage;

	cacheImage = new QImage(thecacheimage);

    //load first frame
	if (!playerisactive)
		playerisactive = true;

	/////////////////////////////////////////////////////
    //find out necessary image data
    frameWidth  = theimage->width();
    frameHeight = theimage->height();

    ////////////////////////////////////////////////////
	//the player object deals with resizing on its own
    CalcZoomValues();

	//we need to build the texture the first time around
	buildTexture();

	//draw the current frame
	drawFrame();

	//should return false if there are any issues
    return true;
}

//////////////////////////////////////////////////
// this routine draws the frame to the screen

void	VideoHead::drawFrame(QImage * theimage)
{
	currentframe = theimage;

	if (playerisactive)
		drawFrame();
}

void	VideoHead::drawFrame()
{
	//make opengl context current
    makeCurrent();

	//build the image before we draw it
	updateTexture();

	//draw the image
	drawTexture();
	
	//update the screen
	updateGL();
}

///////////////////////////////////////////////////
//Texture Generation Using Data From The Bitmap
void	VideoHead::buildTexture()
{
	// 1. Create The Texture
    if (the_texture)     
    {   
        glDeleteTextures(1, &the_texture); 
    }
	glGenTextures(1, &the_texture);					

	// 2. Configure the texture
	glBindTexture(GL_TEXTURE_2D, the_texture);

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // scale linearly when image smalled than texture

    calculateTextureSizes();

	// 3. allocate space for the texture
    glTexImage2D( GL_TEXTURE_2D, 0, texture_mode,   wresolution,
        hresolution, 0, texture_format, GL_UNSIGNED_BYTE, NULL );  
}

void	VideoHead::calculateTextureSizes()
{
    //figure out texture sizes
    int fWidth, fHeight;

    getPowerOfTwoTextureSize(fWidth, fHeight, frameWidth, frameHeight);

    wresolution = fWidth;
    hresolution = fHeight;

    texture_x = frameWidth / wresolution;
    texture_y = frameHeight / hresolution;

    vertex_x = (GLfloat)frameWidth / 2;
    vertex_y= (GLfloat)frameHeight / 2;
}

//similar to above but it just does a copy into the texture
//since the texture shoud have already been created
void	VideoHead::updateTexture()
{				
	glBindTexture(GL_TEXTURE_2D, the_texture);

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // scale linearly when image smalled than texture

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frameWidth,
        frameHeight, texture_format, GL_UNSIGNED_BYTE, currentframe->bits());
}



//////////////////////////////////////////////////
//routine to draw the texture

void	VideoHead::drawTexture()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer

	glBindTexture(GL_TEXTURE_2D, the_texture);

	glBegin(GL_QUADS);
	{
        //normal
        if (flipColorData)
        {
            glTexCoord2f(0.0f, 0.0f); 
            glVertex3f( -zoomWidth, -zoomHeight,  1.0f);

            glTexCoord2f(texture_x,  0.0f);  
		    glVertex3f( zoomWidth, -zoomHeight,  1.0f);

            glTexCoord2f(texture_x,  texture_y);                    
            glVertex3f( zoomWidth, zoomHeight,  1.0f);

            glTexCoord2f(0.0f, texture_y);  
            glVertex3f( -zoomWidth, zoomHeight,  1.0f);
        }
        else
        {
            //flipped image
		    glTexCoord2f(0.0f, texture_y);
            glVertex3f( -zoomWidth, -zoomHeight,  1.0f);
            
            glTexCoord2f(texture_x,  texture_y);
		    glVertex3f( zoomWidth, -zoomHeight,  1.0f);
            
            glTexCoord2f(texture_x,  0.0f);                  
            glVertex3f( zoomWidth, zoomHeight,  1.0f);

            glTexCoord2f(0.0f, 0.0f); 
            glVertex3f( -zoomWidth, zoomHeight,  1.0f);
        }

    }
    glEnd();    
}

void	VideoHead::drawTexture(	GLuint		thenew_texture	)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer

	glBindTexture(GL_TEXTURE_2D, thenew_texture);

	GLboolean valid = 	glIsTexture (thenew_texture);

	glBegin(GL_QUADS);
	{
        //normal
        if (flipColorData)
        {
            glTexCoord2f(0.0f, 0.0f); 
            glVertex3f( -zoomWidth, -zoomHeight,  1.0f);

            glTexCoord2f(texture_x,  0.0f);  
		    glVertex3f( zoomWidth, -zoomHeight,  1.0f);

            glTexCoord2f(texture_x,  texture_y);                    
            glVertex3f( zoomWidth, zoomHeight,  1.0f);

            glTexCoord2f(0.0f, texture_y);  
            glVertex3f( -zoomWidth, zoomHeight,  1.0f);
        }
        else
        {
            //flipped image
		    glTexCoord2f(0.0f, texture_y);
            glVertex3f( -zoomWidth, -zoomHeight,  1.0f);
            
            glTexCoord2f(texture_x,  texture_y);
		    glVertex3f( zoomWidth, -zoomHeight,  1.0f);
            
            glTexCoord2f(texture_x,  0.0f);                  
            glVertex3f( zoomWidth, zoomHeight,  1.0f);

            glTexCoord2f(0.0f, 0.0f); 
            glVertex3f( -zoomWidth, zoomHeight,  1.0f);
        }

    }
    glEnd();    
	//updateGL();
}


/////////////////////////////////////////////
// utility routines

//this one is used for screen refresh
//should only be called if we are in the player module
//as it crashes other modules...
void	VideoHead::paintEvent(QPaintEvent * )
{
	if (staticPlayer)
	{
			if (cacheImage)
			{
				currentframe = cacheImage;
				drawFrame();
			}
	}
	else
	{
		if (playerisactive)
			drawFrame();
		else
		{
			if (cacheImage)
			{
				currentframe = cacheImage;
				drawFrame();
			}
		}
	}

}

/////////////////////////////////////////////
// zoom routines
void VideoHead::SetDisplayZoom(double z)
{
    //update global zoom value
	displayZoom = z;

    //set up zoom
    CalcZoomValues();

    //draw the image now
    drawFrame();
}


void VideoHead::CalcZoomValues()
{
	//float width, height;
	float ar = ( float ) frameWidth / frameHeight;

	int abs_w, abs_h;
	if( zoommode )
	{
		abs_w = QWidget::width( );
		abs_h = QWidget::height( );
	}
	else
	{
		abs_w = frameWidth + frameWidth * displayZoom;
		abs_h = frameHeight + frameHeight * displayZoom;
	}
	
	int req_h = abs_h;
	int req_w = int( req_h * ar );

	// Scale up or down to fit
	if ( req_w > abs_w )
	{
		req_w = abs_w;
		req_h = int( req_w / ar );
	}

	zoomWidth  = req_w / 2;
	zoomHeight = req_h / 2;
}

/////////////////////////////////////////////
//used to toggle zoom mode
void	VideoHead::toggleZoomMode()
{
    zoommode = !zoommode;
    SetDisplayZoom(displayZoom);
}

