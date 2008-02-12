/*******************************************************************************
**
** The header file for the Jahshaka video head class
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef JAH_HEAD_
#define JAH_HEAD_

#include <GL/glew.h>
#include <qgl.h>
#include "jahheadable.h"

class JahHead : public QGLWidget
{
	public:
		JahHead( const QGLWidget *core, QWidget * parent = 0, const char * name = 0, WFlags f = 0 ) 
			: QGLWidget( parent, name, core, f )
			, other_( 0 )
			, width_( 0 )
			, height_( 0 )
		{
		}

		void showInHead( JahHeadable *other )
		{
			other_ = other;
			if ( isVisible( ) && !isMinimized( ) )
				updateGL( );
		}

	protected:
		void initializeGL()
		{
			glewInit( );
			
			glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

			glEnable( GL_NORMALIZE );
			glEnable( GL_RESCALE_NORMAL );
		}

		void resizeGL( int w, int h )
		{
			width_ = w;
			height_ = h;

			glViewport( 0, 0, width_, height_ );

			GLfloat light_ambient[ ]  = { 0.0f, 0.0f, 0.0f, 0.0f };
			GLfloat light_diffuse[ ]  = { 1.0f, 1.0f, 1.0f, 1.0f };
			GLfloat light_specular[ ] = { 1.0f, 1.0f, 1.0f, 1.0f };
			GLfloat light_position[ ] = { 0.0f, 0.0f, 10.0f, 0.0f };

			glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambient );
			glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diffuse );
			glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular );
			glLightfv( GL_LIGHT0, GL_POSITION, light_position );
			glEnable( GL_LIGHT0 );

			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		}
	
		void paintGL( )
		{
			float ar = ( float ) width_ / height_;
			
			glMatrixMode( GL_PROJECTION );
			glLoadIdentity( );
			gluPerspective( 25.5f, ar, 1.0f, 10000.0f);
			
			glMatrixMode( GL_MODELVIEW );
			glLoadIdentity( );
			glTranslatef( 0.0f, 0.0f, -Globals::getCameraDistance( ) );

			if ( other_ )
				other_->headRender( width_, height_ );
			else
				glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		}

	protected:
		void toggle_full_screen( )
		{
			if ( isFullScreen( ) ) 
				showNormal( ); 
			else 
				showFullScreen( ); 
		}

		void keyPressEvent( QKeyEvent *e )
		{
			e->accept( );
			switch( e->key() )
			{
				case 'F':	toggle_full_screen( ); break;
				default:	e->ignore( );
			}
		}

	private:
		JahHeadable *other_;
		int width_, height_;
};

#endif
