/*******************************************************************************
**
** The header file for the Jahshaka jahplayer module 
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "diskplayer.h"



JahPlayer::JahPlayer(QWidget *parent, const char* name)
    :GLCore (parent, name)
{
    //so we can use the tracer
    jtrace = JahTrace::getInstance();	//set up tracer

	//basic variables
    currentframe		= 0; 
    frameWidth		    = 0;
    frameHeight		    = 0;
	playerisactive		= false;
    displayZoom	        = 0;
    zoommode            = false;
	theColormode		= D_ALL;
    
	//videohead preferences
    videohead = false;

    //for drag and drop
    setAcceptDrops(TRUE);

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
	//thextre_mode can also be used
	//for 8,10 or 12 bit playback support
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

	//initialize the asset exchange
    exchange = new assetExchange;

	if (GLEW_ARB_imaging)
		useColorModes = true;
	else
		useColorModes = false;
}


JahPlayer::~JahPlayer()
{
	if(exchange != 0)
		delete exchange;
}

void JahPlayer::Reset()
{
	//reset variables
	playerisactive		= false;
	currentframe		= 0;
    frameWidth		    = 0;
    frameHeight		    = 0;
    

	if(exchange != 0)
		delete exchange;

    exchange = new assetExchange;		

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


//this preps the player when called for the first time
bool JahPlayer::InitializePlayer(assetData desktopclip)
{
    //reset the engine
	Reset();

	//initalize local player data variable
    playerAssetData = desktopclip;

	if (!playerisactive)
		playerisactive = true;

    loadFrame = playerAssetData.startframe; 

	/////////////////////////////////////////////////////
    //find out necessary image data
    frameWidth  = exchange->getImageWidth(playerAssetData, loadFrame);
    frameHeight = exchange->getImageHeight(playerAssetData, loadFrame);

    ////////////////////////////////////////////////////
	//the player object deals with resizing on its own
    CalcZoomValues();

	//we need to build the texture the first time around
	buildTexture();

	//then we need to load the first frame
    LoadTheFrame(loadFrame);

	//should return false if there are any issues
    return true;

}

//////////////////////////////////////////////////
// main routine used to display a image
void	JahPlayer::LoadTheFrame(int frame)
{
    if (frame<0) 
		frame = 0;

	//this is where we grab the image from the assetExchange
    loadFrame = frame;

	//this is just a pointer to the image in the stream
    //currentframe = exchange->getStreamImagePtr(playerAssetData, frame, true);
    currentframe = exchange->getStreamImagePtr(playerAssetData, frame, flipColorData);

	//draw the current frame
	drawFrame();
}

void	JahPlayer::drawFrame(QImage * theimage, int frameNo)
{
	loadFrame = frameNo;

	currentframe = theimage;

	if (playerisactive)
		drawFrame();
}

//////////////////////////////////////////////////
// this routine draws the frame to the screen
void	JahPlayer::drawFrame()
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
void	JahPlayer::buildTexture()
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

void	JahPlayer::calculateTextureSizes()
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
void	JahPlayer::updateTexture()
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

void	JahPlayer::drawTexture()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer

    //is this necessary since its already bound?
	glBindTexture(GL_TEXTURE_2D, the_texture);

    glMatrixMode(GL_MODELVIEW);
    
    //glPushMatrix();

	//set up display color mode
	if (useColorModes)
		calculateColorMode();

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
    //glPopMatrix();

}

void	JahPlayer::calculateColorMode()
{

  glMatrixMode(GL_COLOR);
  glLoadIdentity();

  // Transform the texture colors: Scale r*=0.5, g*=2.0, b*=1.0
  // we really just want to hide the unused channels here
    switch(theColormode) {
        case D_ALL    : {  break; }
        case D_RED    : { glScalef(1.0,0.0,0.0); break; }
        case D_GREEN  : { glScalef(0.0,1.0,0.0); break; }
        case D_BLUE   : { glScalef(0.0,0.0,1.0); break; }
        case D_ALPHA  : { glScalef(1.0,1.0,1.0); break; }
        default : { break; }
    }
}

/////////////////////////////////////////////
// zoom routines
void JahPlayer::SetDisplayZoom(double z)
{
    //update global zoom value
	displayZoom = z;

    //set up zoom
    CalcZoomValues();

    //draw the image now
    drawFrame();
}

void JahPlayer::CalcZoomValues()
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
void	JahPlayer::toggleZoomMode()
{
    zoommode = !zoommode;
    SetDisplayZoom(displayZoom);
}

/////////////////////////////////////////////
// used to refresh the player 
void	JahPlayer::paintEvent(QPaintEvent * )
{
	if (playerisactive)
		SetDisplayZoom(displayZoom);
}

/////////////////////////////////////////////
// used to query player
int	JahPlayer::getCurrentFrame(void)
{
    return loadFrame;
}

/////////////////////////////////////////////
// for the drag and drop

void JahPlayer::dragEnterEvent(QDragEnterEvent* event)
{
    event->accept(QImageDrag::canDecode(event));

}

void JahPlayer::dropEvent(QDropEvent* event)
{
    QImage image;

    if ( QImageDrag::decode(event, image) )
    {
        emit draggedAImage();
    }
}
