/*******************************************************************************
**
** The header file for the Jahshaka glworld module 
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef GLCORE_H
#define GLCORE_H

#ifdef WIN32
#ifdef NEWOPENOBJECTLIB
#include <GL/glew.h>
#include <GL/wglew.h>
#else
#include <glew.h>
#include <wglew.h>
#endif
#endif // WIN32

// There is a conflict between glx symbols and Qt symbols so it
// impossible to include glx in this file, as this file is dependent on other files
// which use Qt.

#include <qgl.h>

#ifndef __APPLE__
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif

class PBuffer;

PBuffer* createRgbaPbuffer(int width, int height);
PBuffer* createAndSetRgbaPbuffer(PBuffer** pbuffer, int width, int height);

///////////////////////////////////////////
// main class
class GLCore : public QGLWidget {
    Q_OBJECT

  protected:
    PBuffer*    m_pbuffer;
    static bool m_win32;
    GLfloat     m_camera_distance;

  public:
    PBuffer*    getPbuffer() { return m_pbuffer; }
    PBuffer**   getPbufferAddress() { return &m_pbuffer; }
    GLuint      copyFrameBufferToTexture(int& texture_width, int& texture_height);
    GLuint      copyFrameBufferToTexture(int frame_buffer_horizontal_offset, int frame_buffer_vertical_offset,
                                         int& texture_width, int& texture_height);
    void        copyFrameBufferToPbuffer();
    static bool isWin32() { return m_win32; }
    GLfloat     getCameraDistance() { return m_camera_distance; }


public:
    GLCore( QWidget* parent, const char* name, int mode=0 );    
    GLCore( QGLWidget *share, QWidget* parent, const char* name, int mode=0 );    
	virtual ~GLCore( );
private:
	// Shared constructor code
	void initialize( int );

public slots:
    void initializeGL( void );
    void paintGL( void ) { emit paintit(); } 
    void resizeGLInternals( int w, int h, bool dont_zoom = false ); 
    void resizeGL( int w, int h ); 

	// CY: AFAICT, the drag n drop handling causes the qt lock to be locked throughout the 
	// execution of the MediaTable::contentsMousePressEvent but since that function 
	// doesn't complete until the draggedAImage signal has been handled, this causes 
	// a deadlock in the MLT editor (the MLT editor may need to stop and start the 
	// preview which requires the lock).

	// The solution is to to add this notifyDrop slot which is scheduled by dropEvent
	// using a QTimer::singleshot - thus, contentsMousePressEvent terminates prior to
	// to the singleshot kicking in.
	void notifyDrop( ) { emit draggedAImage( ); }

	// Allow redirection from other widgets
	void customDragEnterEvent( QDragEnterEvent* event ) { dragEnterEvent( event ); }
	void customDropEvent( QDropEvent* event ) { dropEvent( event ); }

protected:
    ///////////////////////////////////////////////////////////////
    void dragEnterEvent(QDragEnterEvent* event);
    void dropEvent(QDropEvent* event);

    int                 m_render_width;
    int                 m_render_height;
    int                 m_pbuffer_render_width;
    int                 m_pbuffer_render_height;
    bool                m_pbuffer_bound;

public:

    void                bindPbuffer();
    void                unbindPbuffer();
    bool                getPbufferBound() { return m_pbuffer_bound; }
    void                setPbufferBound(bool flag) { m_pbuffer_bound = flag; }

    int                 getRenderWidth();
    void                setRenderWidth(int width) { m_render_width = width; }
    int                 getRenderHeight();
    void                setRenderHeight(int height) { m_render_height = height; }
    int                 getPbufferRenderWidth() { return m_pbuffer_render_width; } 
    void                setPbufferRenderWidth(int width) { m_pbuffer_render_width = width; };
    int                 getPbufferRenderHeight() { return m_pbuffer_render_height; }
    void                setPbufferRenderHeight(int height) { m_pbuffer_render_height = height; }



    //set zoom up
    void setZoom(GLfloat zoom);
	GLfloat getZoom( ) { return zoomVal; }

    //these should all be private
    //size of the render context

    bool useimagesize;
    int imagewidth;
    int imageheight;

    //first one is a dupe here
    GLfloat fAspectval;
    GLfloat fAspect;

    //for the background color!
    GLfloat backgroundRed, backgroundGreen, backgroundBlue;

    void setBackgroundColor ( GLfloat r, GLfloat g , GLfloat b );

    //zoom for the main window
    GLfloat zoomVal;
  
    int glmode;

    void    mousePressEvent( QMouseEvent *e );
    void    mouseReleaseEvent( QMouseEvent *e );
    void    mouseMoveEvent( QMouseEvent *e );

	// Special case to allow auto double buffering to be switched off
	void switchAutoBufferSwap( bool on ) { setAutoBufferSwap( on ); }

signals:

    void paintit(void);
    void resizeit(void);

    void mousePress(QMouseEvent*);
    void mouseRelease(QMouseEvent*);
    void mouseMove(QMouseEvent*);

    void draggedAImage();
};

#endif // GLWORLD_H




















































