/*******************************************************************************
**
** The source file for the Jahshaka glworld.cpp module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "gpumathlib.h"
#include "glcore.h"
#include "pbuffer.h"
#include <qtimer.h>
#include <qdragobject.h>
#include <qdropsite.h>
#include "jahtracer.h"
#include "jahpreferences.h"

GLCore::GLCore( QWidget* parent, const char* name, int mode)
    : QGLWidget( parent, name ) 
	, m_pbuffer( 0 )
{
	initialize( mode );
}

GLCore::GLCore( QGLWidget *share, QWidget* parent, const char* name, int mode)
    : QGLWidget( parent, name, share ) 
	, m_pbuffer( 0 )
{
	initialize( mode );
}

GLCore::~GLCore( )
{
	delete m_pbuffer;
}

void GLCore::initialize( int mode )
{
    //lets us know if its 3d perp or 2d ortho
    glmode = mode;

    //this just sets the format
    QGLFormat format;

    if (glmode == 1)
    {
        format.setAlpha(TRUE);
        format.setDoubleBuffer(TRUE);
        format.setDepth(TRUE);
    }
    else
    {
        format.setAlpha(TRUE);
        format.setDoubleBuffer(TRUE);
        format.setDepth(TRUE);
    }

    format.setRgba(TRUE);
    format.setAccum(FALSE);
    format.setStereo(FALSE);
    format.setStencil(FALSE); 
    format.setDirectRendering( TRUE ); 
    setFormat(format);

    //background color defaults
    backgroundRed   = 0.3f;
    backgroundGreen = 0.3f;
    backgroundBlue  = 0.3f;

    //set some variables
    useimagesize=false;
    imagewidth=0;
    imageheight=0;
    zoomVal=1;

    m_pbuffer = NULL;
    setPbufferBound(false);

    //for drag and drop
    setAcceptDrops(TRUE);

#ifdef WIN32
    m_win32 = true;
    Globals::setisWin32(true);
#else
    m_win32 = false;
    Globals::setisWin32(false);
#endif

	glInit( );
}

void GLCore::dragEnterEvent(QDragEnterEvent* event)
{
    //debug("glcore drag event");

    event->accept(
        QTextDrag::canDecode(event) ||
        QImageDrag::canDecode(event)
    );

}

void GLCore::dropEvent(QDropEvent* event)
{
    QImage image;
    QString text;

    //debug("glcore drop event");

    if ( QImageDrag::decode(event, image) )
    {
		// CY: rather than emiting the draggedAImage signal, it is done via QTimer
		// and the notifyDrop slot defined on this class (see comments in glcore.h)

        //this is connected to the main app in Source/jahroutines.cpp
		QTimer::singleShot( 0, this, SLOT( notifyDrop( ) ) );
    }
    else if ( QTextDrag::decode(event, text) )
    {
        //debug("its text");
    }

}

void GLCore::mousePressEvent( QMouseEvent *e )
{
    emit mousePress(e);
}

void GLCore::mouseReleaseEvent( QMouseEvent *e ) 
{
    emit mouseRelease(e);
}

void GLCore::mouseMoveEvent( QMouseEvent *e ) 
{
    emit mouseMove(e);
}

void GLCore::initializeGL() 
{       
    //initalize the global vars from the prefs
    JahPrefs& jprefs  = JahPrefs::getInstance();

    bool HWFOG = jprefs.getHwFog();

    if (glmode == 0)
    {
    //Set up the OpenGL rendering state, and define display list

    //set up environment and turn on depth testing
    glDepthFunc(GL_LESS); glEnable( GL_DEPTH_TEST ); glShadeModel(GL_SMOOTH);

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glEnable( GL_TEXTURE_2D );

    //for the default light - light0
    //we need a global light counter for lights to really work...
    GLfloat light_ambient[]  = {1.0, 1.0, 1.0, 1.0};     //    GLfloat light_ambient[] = {0.3, 0.3, 1.0, 1.0};
    GLfloat light_diffuse[]  = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_position[] = {0.0f, 0.0f, 10.0f, 0.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient );
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse );
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular );
    glLightfv(GL_LIGHT0, GL_POSITION, light_position );

    //glMaterialf(GL_FRONT, GL_SHININESS, 20.0);


    glEnable(GL_LIGHT0);

    GLuint  fogMode[]= { GL_EXP, GL_EXP2, GL_LINEAR }; // Storage For Three Types Of Fog
    GLuint  fogfilter = 2;                         // Which Fog Mode To Use
    GLfloat fogColor[4] = {0.5f,0.5f,0.5f,1.0f};   // Fog Color

    //turns fog env on
    if (HWFOG)
    {
        glFogi(GL_FOG_MODE, fogMode[fogfilter]);       // Fog Mode
        glFogfv(GL_FOG_COLOR, fogColor);               // Set Fog Color
        glFogf(GL_FOG_DENSITY, 0.45f);                 // How Dense Will The Fog Be
        glHint(GL_FOG_HINT, GL_NICEST);                // Fog Hint Value
        glFogf(GL_FOG_START, 1000.0f);                    // Fog Start Depth
        glFogf(GL_FOG_END, 0.0f);                      // Fog End Depth
    }

    // Set up various other stuff
    glClearColor( backgroundRed, backgroundGreen, backgroundBlue, 0.0 ); // Let OpenGL clear to grey

    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

    glEnable( GL_NORMALIZE );     //glEnable(GL_LINE_SMOOTH);

    //used to enable reflection mapping
    //what does this do exactly ie to speed and memory...
    //need to do this and rebuild textures if necessary
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP); // Set The Texture Generation Mode For S To Sphere Mapping (NEW)
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP); // Set The Texture Generation Mode For T To Sphere Mapping (NEW)

    }

    if (glmode==1)
    {
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

      glEnable(GL_POINT_SMOOTH);       //use smooth, rounded points
      glShadeModel(GL_SMOOTH);         //set default shading model
      //glEnable(GL_COLOR_LOGIC_OP);     //enable color logic operations (ex: XOR)
      glPolygonMode(GL_FRONT, GL_FILL); //draw filled polygons, front only

      glClearColor( 0.0, 0.0, 0.0, 1.0 );
      //glClear(GL_COLOR_BUFFER_BIT);

    }
}

////////////////////////////////////////////
// this routine is supposed to set the background color of the core
// used to set the background color of any module

void GLCore::setBackgroundColor ( GLfloat r, GLfloat g , GLfloat b )
{
    backgroundRed   = r;
    backgroundGreen = g;
    backgroundBlue  = b;

    //not sure why this isnt working mon?
    glClearColor( backgroundRed, backgroundGreen, backgroundBlue, 0.0 ); // Let OpenGL clear to grey
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLCore::setZoom(GLfloat zoom) 
{
    zoomVal = zoom;

    //resizeGL( render_width, render_height, fAspectval );
    resizeGL( getRenderWidth(), getRenderHeight() );

}

int                 
GLCore::getRenderWidth()
{
    // FIXME
    if ( getPbuffer() && getPbufferBound() )
    {
        return m_pbuffer_render_width;
    }
    else
    {
        return m_render_width;
    }
}

int                 
GLCore::getRenderHeight()
{
    // FIXME
    if ( getPbuffer() && getPbufferBound() )
    {
        return m_pbuffer_render_height;
    }
    else
    {
        return m_render_height;
    }
}

void
GLCore::bindPbuffer()
{
    if ( !getPbuffer() )
    {
        return;
    }

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();

    setPbufferBound(true);
    getPbuffer()->Bind();
    resizeGLInternals( getPbufferRenderWidth(), getPbufferRenderHeight(), true );
}

void
GLCore::unbindPbuffer()
{
    if ( getPbuffer() )
    {
        getPbuffer()->Unbind();
    }

    setPbufferBound(false);
    glViewport(0, 0, m_render_width, m_render_height);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void 
GLCore::resizeGLInternals(int width, int height, bool dont_zoom)
{
    //start the resizing
    double window_width  = (double)jahstd::Even(width);
    double window_height = (double)jahstd::Even(height);

    glViewport(0, 0, width, height);

    // scale_width just needs to be constant - value doesn't matter
    double scale_width = 486.0;
    // scale_height affects the camera distance.  486 is a legacy number to make
    // animtest look the way Karsten intended it to look
    double scale_height = 486.0;
    // Aspect ratio doesn't matter because it gets adjusted by the glScalef() below.
    double aspect_ratio = (scale_width / scale_height);

    glMatrixMode(GL_PROJECTION);    // Set the perspective coordinate system
    glLoadIdentity();

    double view_angle = 25.0f;
    double view_angle_radians = (view_angle / 180.0) * PI;

    float zoom = zoomVal;

    if (dont_zoom)
    {
        zoom = 1.0f;
    }

    gluPerspective((GLfloat)view_angle * zoom, (GLfloat)aspect_ratio, 1.0f, 10000.0f);   
    m_camera_distance = (scale_height / 2.0) / tan(view_angle_radians / 2.0);

    Globals::setCameraDistance(m_camera_distance);

    glMatrixMode(GL_MODELVIEW);    // Modelview matrix reset
    glLoadIdentity();

    //need to adjust the camera z depth here
    //this is a bug and shouldnt be here 
    double transval =  -m_camera_distance;// * (double)zoomVal; 
    glTranslatef( 0.0f, 0.0f, (GLfloat)transval);

    double width_ratio = scale_width / window_width;
    double height_ratio = scale_height / window_height;

    glScalef((GLfloat)width_ratio, (GLfloat)height_ratio, 1.0f);
}

void GLCore::resizeGL( int width, int height ) 
{
    makeCurrent();

    if (glmode == 0)
    {
		if ( getPbuffer() )
		{
			delete m_pbuffer;
		}

        int max_texture_size = Globals::getGlMaxTextureSize() > 2048 ? 2048 : Globals::getGlMaxTextureSize();

        setRenderWidth( int( width ) );
        setRenderHeight( int( height ) );
        setPbufferRenderWidth( max_texture_size );
        setPbufferRenderHeight( max_texture_size );

		m_pbuffer = createRgbaPbuffer( getPbufferRenderWidth(), getPbufferRenderHeight() );
		bindPbuffer();
        resizeGLInternals( getPbufferRenderWidth(), getPbufferRenderHeight() );
		unbindPbuffer();
        resizeGLInternals( getRenderWidth(), getRenderHeight() );
    }

    if (glmode==1)
    {
          setRenderWidth( jahstd::Even(width) );
          setRenderHeight( jahstd::Even(height) );
         
          //glClear(GL_COLOR_BUFFER_BIT);
          glViewport(0, 0, (GLint) width, (GLint) height);
          glMatrixMode(GL_PROJECTION);
          glLoadIdentity();

          glOrtho(0, width, 0, height, -2, 2);

          glMatrixMode(GL_MODELVIEW);    // Modelview matrix reset
          glLoadIdentity();

          emit resizeit();
    }

}

bool GLCore::m_win32;

GLuint        
GLCore::copyFrameBufferToTexture(int frame_buffer_horizontal_offset, int frame_buffer_vertical_offset,
                                         int& texture_width, int& texture_height)
{
    int render_width = getRenderWidth() - (frame_buffer_horizontal_offset * 2);
    int render_height = getRenderHeight() - (frame_buffer_vertical_offset * 2);

    GLuint texture_id;
    getPowerOfTwoTextureSize(texture_width, texture_height, render_width, render_height );
    createEmpty2DTexture(&texture_id, GL_RGBA, texture_width, texture_height);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glEnable(GL_TEXTURE_2D);

    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 
                        frame_buffer_horizontal_offset, frame_buffer_vertical_offset, 
                        render_width, render_height );

    return texture_id;
}
    
GLuint        
GLCore::copyFrameBufferToTexture(int& texture_width, int& texture_height)
{   
    GLuint texture_id;
    getPowerOfTwoTextureSize(texture_width, texture_height, getRenderWidth(), getRenderHeight() );
    createEmpty2DTexture(&texture_id, GL_RGBA, texture_width, texture_height);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glEnable(GL_TEXTURE_2D);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, getRenderWidth(), getRenderHeight() );
    return texture_id;
}
    
void        
GLCore::copyFrameBufferToPbuffer()
{
    int texture_width;
    int texture_height;

    GLuint texture_id = copyFrameBufferToTexture(texture_width, texture_height);

    bindPbuffer();

    float2 texture_ratio;
    texture_ratio.x = float( getRenderWidth() ) / texture_width;
    texture_ratio.y = float( getRenderHeight() ) / texture_height;
    int buffer_width_div_2 = getRenderWidth() / 2;
    int buffer_height_div_2 = getRenderHeight() / 2;

    glBindTexture(GL_TEXTURE_2D, texture_id);

    glBegin(GL_QUADS);
    {
        glTexCoord2f(0.0f, 0.0f);           
        glVertex3f(-buffer_width_div_2, -buffer_height_div_2, 0.0f);

        glTexCoord2f(texture_ratio.x, 0.0f);           
        glVertex3f(buffer_width_div_2, -buffer_height_div_2,  0.0f);

        glTexCoord2f(texture_ratio.x, texture_ratio.y);           
        glVertex3f(buffer_width_div_2, buffer_height_div_2,  0.0f);

        glTexCoord2f(0.0f, texture_ratio.y);           
        glVertex3f(-buffer_width_div_2, buffer_height_div_2,  0.0f);
    }
    glEnd(); // GL_QUADS

    unbindPbuffer();

}

void GLCore::switchAutoBufferSwap( bool on )
{
  setAutoBufferSwap( on ); 
}
