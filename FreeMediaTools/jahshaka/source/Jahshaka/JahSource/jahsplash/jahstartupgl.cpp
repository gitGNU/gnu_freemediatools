/* -*- mode: C++; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t -*- */
/*******************************************************************************
**
** The source file for the Jahshaka OpenGL Intro from the startup
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include <GL/glew.h>

#include "jahstartupgl.h"
#include "gpumathlib.h"

const int redrawWait = 25;

// Create a GLTexobj widget
GLTexobj::GLTexobj( QWidget* parent, const char* name )
    : QGLWidget( parent, name )
{
    //this just sets the format
    QGLFormat format;

    format.setAlpha(TRUE);
    format.setDoubleBuffer(TRUE);
    format.setRgba(TRUE);
    format.setDirectRendering( TRUE ); 

    setFormat(format);

    //for the timer
    firsttime = true;

    xRot    = yRot = zRot = 0.0;		// default object rotation

#ifdef DEBUG
    zPos    = 45;
#else
    zPos    = 45;
#endif

    scale   = 5.0;			// default object scale
    object1 = 0;  
	object2 = 0;
    animation = TRUE;

    timer = new QTimer( this );
    connect( timer, SIGNAL(timeout()), SLOT(update()) );
    timer->start( redrawWait, TRUE );
    
    //initialize JahBasePath
    JahPrefs& jprefs = JahPrefs::getInstance();
    JahBasePath = jprefs.getBasePath().data();
}


// Release allocated resources


GLTexobj::~GLTexobj()
{
    makeCurrent();
    glDeleteLists( object1, 1 );
    glDeleteLists( object2, 1 );
}


// Paint the texobj. The actual openGL commands for drawing the texobj are
//  performed here.

void GLTexobj::paintGL()
{

    if (zPos>5.0) 
    {  
        zPos-=3.0; 
    }
    else 
    { 
        if (firsttime) 
        { 
            emit introFinished(); 
            firsttime=false;
        }
    }


    //clear the scene and set up matrices
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glMatrixMode(GL_MODELVIEW);
    
    //this draws the jahshaka text as a background image
    glPushMatrix();
    
        //glScalef( scale*2.2, scale*1.65, scale );   

        glBindTexture( GL_TEXTURE_2D, object1 );

	    glBegin(GL_QUADS);					// Start Drawing Quads
	        glTexCoord2f(0.0f, 0.0f); glVertex3f(-9.0f, -7.0f,  0.0f);	// Bottom Left Of The Texture and Quad
	        glTexCoord2f(1.0f, 0.0f); glVertex3f( 9.0f, -7.0f,  0.0f);	// Bottom Right Of The Texture and Quad
	        glTexCoord2f(1.0f, 1.0f); glVertex3f( 9.0f,  7.0f,  0.0f);	// Top Right Of The Texture and Quad
	        glTexCoord2f(0.0f, 1.0f); glVertex3f(-9.0f,  7.0f,  0.0f);	// Top Left Of The Texture and Quad
	    glEnd();	

    glPopMatrix();

    //this draws the jahshaka logo
    glPushMatrix();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 

        glBindTexture( GL_TEXTURE_2D, object2 );

        glTranslatef( 0.0, 1.25, zPos);
        glScalef( scale*1.1, scale*0.825, scale );           

	    glBegin(GL_QUADS);					// Start Drawing Quads
		    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// Bottom Left Of The Texture and Quad
		    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// Bottom Right Of The Texture and Quad
		    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	// Top Right Of The Texture and Quad
		    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	// Top Left Of The Texture and Quad
	    glEnd();	

        glDisable(GL_BLEND);

    glPopMatrix();

    if ( animation ) 
    {
	    timer->start( redrawWait, TRUE ); // Wait this many msecs before redraw
    }
}


// Set up the OpenGL rendering state, and define display list
void GLTexobj::initializeGL()
{
  //Set up the OpenGL rendering state, and define display list
  glShadeModel(GL_SMOOTH);
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glEnable( GL_TEXTURE_2D );

  // Set up the lights
  GLfloat whiteDif[4] = {1.0, 1.0, 1.0, 1.0};
  GLfloat whiteAmb[4] = {1.0, 1.0, 1.0, 1.0};
  GLfloat lightPos[4] = {30.0, 30.0, 30.0, 0.0};

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteAmb);

  glMaterialfv(GL_FRONT, GL_DIFFUSE, whiteDif);
  glMaterialfv(GL_FRONT, GL_SPECULAR, whiteDif);
  glMaterialf(GL_FRONT, GL_SHININESS, 20.0);

  glLightfv(GL_LIGHT0, GL_DIFFUSE, (const GLfloat*)whiteDif );	
  glLightfv(GL_LIGHT0, GL_SPECULAR, (const GLfloat*)whiteDif );	
  glLightfv(GL_LIGHT0, GL_POSITION, (const GLfloat*)lightPos );

  //////////////////////////////////////////////////////////////////
  // Set up the textures
  //jahback and jahlogo are unique to the starutp screen

  QImage buf;
  if ( !buf.load( JahBasePath+"Pixmaps/jahback.jpg" ) ) {	}
  tex1 = QGLWidget::convertToGLFormat( buf );  // flipped 32bit RGBA

  if ( !buf.load( JahBasePath+"Pixmaps/jahlogo.png" ) ) {	}
  tex2 = QGLWidget::convertToGLFormat( buf );  // flipped 32bit RGBA

  // Set up various other stuff
  glClearColor( 0.0, 0.0, 0.0, 0.0 ); // Let OpenGL clear to black
  
  //glEnable( GL_CULL_FACE );  	// don't need Z testing for convex objects
  glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
  glEnable( GL_NORMALIZE );

  // Make the object display list
  makeObjectLayer ( object1, tex1 );	// Generate an OpenGL display list
  makeObjectLayer ( object2, tex2 );	// Generate an OpenGL display list

  /////////////////////////////////////////////////////////////////////
  //we need to do this here
  //this is where we get the rendering contect's and return them to the system
  //so it knows what to launch up with

  if ( glGetString(GL_VENDOR) != NULL )
  	vendor     = new QString ((const char *) glGetString(GL_VENDOR));
  else
    vendor     = new QString( "" );

  if ( glGetString(GL_RENDERER) != NULL )
    renderer   = new QString ((const char *) glGetString(GL_RENDERER));
  else
    renderer   = new QString( "" );

  if ( glGetString(GL_VERSION) != NULL )
    version    = new QString ((const char *) glGetString(GL_VERSION));
  else
    version    = new QString( "" );

  if ( glGetString(GL_EXTENSIONS) != NULL )
    extensions = new QString ((const char *) glGetString(GL_EXTENSIONS));
  else
    extensions = new QString( "" );

}

// Set up the OpenGL view port, matrix mode, etc.
void GLTexobj::resizeGL( int w, int h )
{
    glViewport( 0, 0, w, h );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glFrustum( -1.0, 1.0, -1.0, 1.0, 10, 100.0 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    
    glTranslatef( 0.0, 0.0, -70.0 );
}


// Generate an OpenGL display list for the object to be shown, i.e. the texobj
void GLTexobj::makeObjectLayer( GLuint & theObj, const QImage& tex1)
{
	glGenTextures( 1, &theObj );

	glBindTexture( GL_TEXTURE_2D, theObj );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

   glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, tex1.width(), tex1.height(), 0,
		  GL_RGBA, GL_UNSIGNED_BYTE, tex1.bits() );

}

//Turns animation on or off
void GLTexobj::toggleAnimation()
{
    animation = !animation;

    if ( animation )
	    updateGL();
    else
	    timer->stop();
}

