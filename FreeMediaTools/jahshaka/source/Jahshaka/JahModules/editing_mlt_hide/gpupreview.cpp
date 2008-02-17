/*******************************************************************************
**
** The header file for the Jahshaka/MLT editing module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include <glworld.h>
#include <glcore.h>
#include "base.h"
#include "edit.h"
#include "gpupreview.h"
#include "audio.h"
#include "project.h"
#include <qtimer.h>
#include <qmessagebox.h>

#include <openpluginlib/pl/GL_utility.hpp>

#include <FreeMediaTools.h>

using namespace Mlt;
using namespace jahnle;

namespace opl = olib::openpluginlib;

PreviewGpu::PreviewGpu( EditBase *edit, const QString test_card ) :
	QObject( NULL ),
	Preview( edit->getMediaPath( ) ),
	m_started( false ),
	m_edit( edit ),
	m_project( NULL ),
	m_frame( NULL ),
	m_update_event( NULL ),
	texture_id_( 0 ),
	old_phy_w_( -1 ),
	old_phy_h_( -1 ),
	m_convert( NULL ),
	m_test_card_file( test_card ),
	m_test_card( NULL ),
	m_audio( NULL ),
	m_refreshing( false ),
	m_ask_audio( true ),
	m_zoom( 1.0 ),
	m_head_render( false )
{
	setOptions( );
	setPAL( getNormalisation( ) );
	connect( &m_frame_timer, SIGNAL( timeout( ) ), this, SLOT( timeOut( ) ) );
}

PreviewGpu::~PreviewGpu( )
{
	// Stop and close up
	delete m_test_card;
	delete m_audio;
	delete m_convert;
	delete m_frame;
	delete_event( m_update_event );
}

void PreviewGpu::init( )
{
	delete m_test_card;
	m_test_card = new Producer( ( char * )m_test_card_file.ascii( ) );

	delete m_audio;
	m_audio = NULL;

	delete m_convert;
	m_convert = new Filter( "avcolour_space" );
	m_convert->set( "forced", mlt_image_yuv422 );
}

void PreviewGpu::start( )
{
	// If not started, get all the current settings
	if ( !m_started )
		setOptions( );

	// Flag as started
	m_started = true;

	// And refresh
	refresh( );
}

void PreviewGpu::stop( )
{
	m_started = false;
}

void PreviewGpu::show( Project *project )
{
	// Remove update event associated to previous project
	delete_event( m_update_event );
	m_update_event = NULL;
	
	if ( project != NULL )
	{
		m_project = project;
		m_convert->connect( *project );

		// Setup the event for project modifications
		m_update_event = project->listen( "producer-changed", m_edit, ( mlt_listener )updateHandler );
	}
}

void PreviewGpu::renderFrame( Frame *f )
{
	if ( f )
	{
		f->set( "test_card_producer", ( void * )( m_test_card->get_producer( ) ), 0 );
		f->set( "deinterlace_method", m_consumer.get( "deinterlace_method" ) );
		f->set( "consumer_deinterlace", m_consumer.get_int( "progressive" ) );
		f->set( "rescale.interp", m_consumer.get( "rescale" ) );
		f->set( "resize", m_consumer.get_int( "resize" ) );
		f->set( "consumer_aspect_ratio", m_consumer.get_double( "aspect_ratio" ) );
		int w = m_consumer.get_int( "width" );
		int h = m_consumer.get_int( "height" );
		mlt_image_format format = mlt_image_opengl;
		f->get_image( format, w, h );
		f->set( "rendered", 1 );
	}
}

void PreviewGpu::refresh( )
{
	if ( !m_started || m_project == NULL || m_refreshing ) return;

	// Make sure we don't hit the refresh via a change event from the project now
	m_refreshing = true;

	// Fetch the current frame
	Mlt::Frame *frame = m_convert->get_frame( );

	// Ugh - multiple return points
	if ( frame == NULL ) 
	{
		m_refreshing = false;
		return;
	}

	// Fetch the speed
	int speed = frame->get_int( "_speed" );

	// Ensure the audio device is only locked when we need it
	if ( speed == 1 && m_play_mode != 2 && m_audio == NULL )
	{
		m_audio = new MltAudio( );

		if ( !m_audio->init( ) )
		{
			while ( m_ask_audio && !m_audio->init( ) )
			{
				int response = 
				  QMessageBox::warning(
						       m_edit->getCore( ), 
						       FMT_PROGRAM_TITLE,
	       "Audio device is unavailable.\nDo you want to retry?",
	       "Retry", "Ignore", 0, 0, 1 );

				delete m_audio;
				m_audio = NULL;

				switch( response )
				{
   					case 0:
       					m_audio = new jahnle::MltAudio( );
       					break;
   					case 1: 
						m_ask_audio = false;
       					break;
   				}
			}
		}

		if ( m_audio )
			m_audio->setRender( isPAL( ) ? 25.0 : 30000.0 / 1001.0, 2, m_consumer.get_int( "frequency" ) );
	}
	else if ( speed != 1 && m_audio != NULL )
	{
		delete m_audio;
		m_audio = NULL;
	}

	// Render the frame
	if ( m_play_mode != 0 || speed != 1 || m_audio == NULL || !m_audio->empty( ) )
		renderFrame( frame );

	// We need to trigger a refresh of the position quickly when scrubbing
	// but we don't know what it is while playing yet
	if ( speed != 1 )
		m_edit->updateSlider( frame->get_int( "_position" ) );

	// Play the audio or flush
	if ( m_play_mode != 2 && ( speed == 1 && m_audio && m_audio->init( ) ) )
		m_audio->push( *frame );
	else if ( m_audio )
		m_audio->flush( );

	// Preroll handling
	if ( m_play_mode != 2 && ( speed == 1 && m_preroll.size( ) == 0 && m_audio && m_audio->init( ) ) )
	{
		int size = 7;
		while ( size -- )
		{
			Mlt::Frame *pre = m_convert->get_frame( );
			renderFrame( pre );
			m_audio->push( *pre );
			m_preroll.push_back( pre );
		}
	}
	else if ( speed == 1 && m_preroll.size( ) )
	{
		m_preroll.push_back( frame );
		frame = m_preroll[ 0 ];
		m_preroll.pop_front( );
	}
	else
	{
		while( m_preroll.size( ) )
		{
			delete m_preroll[ 0 ];
			m_preroll.pop_front( );
		}
	}

	// Update the slider
	Mlt::Frame &f = *frame;
	if ( speed == 1 )
		m_edit->updateSlider( f.get_int( "_position" ) );

	// Ensure the timer is active when it needs to be
	if ( speed == 0 && m_frame_timer.isActive( ) )
		m_frame_timer.stop( );
	else if ( m_play_mode == 2 && speed != 0 && !m_frame_timer.isActive( ) )
		m_frame_timer.start( int( 1000.0 / m_project->get_double( "fps" ) ) );
	else if ( speed != 0 && !m_frame_timer.isActive( ) )
		m_frame_timer.start( 0 );

	// Pass it to the edit project
	m_edit->consumerFrameShow( f );

	// Trigger the paintgl
	if ( frame->get_int( "rendered" ) )
	{
		delete m_frame;
		m_frame = frame;
		m_edit->glworldUpdate( );
	}
	else
	{
		delete frame;
	}

	m_refreshing = false;
}

void PreviewGpu::timeOut( )
{
	refresh( );
}

// Modification to the project
void PreviewGpu::updateHandler( mlt_properties, EditBase *self )
{
	self->updateUI( );
	self->checkPoint( );
}

void PreviewGpu::setZoom( int value )
{
	m_zoom = float( 4000 - value ) / 1000.0;
	refresh( );
}

void PreviewGpu::setDimensions( int width, int height )
{
	m_current_width = width;
	m_current_height = height;
}

void PreviewGpu::paint( )
{
	// Do the OpenGL rendering when required
	if ( m_frame != NULL )
	{
		GLCore *core = m_edit->getCore( );

		// If we're not in head render mode, make sure our width/height are correct
		if ( !m_head_render )
			setDimensions( core->width( ), core->height( ) );

		// This is the absolute WxH we have
		int abs_w = m_current_width;
		int abs_h = m_current_height;

		// This is the physcial WxH of the image
		int phy_w = m_frame->get_int( "width" );
		int phy_h = m_frame->get_int( "height" );
		mlt_image_format format = ( mlt_image_format )m_frame->get_int( "format" );
		double ar = m_consumer.get_double( "display_ratio" );
		uint8_t *image = m_frame->get_image( format, phy_w, phy_h );

		// Calculate the required display WxH
		int req_h;
		int req_w;

		if ( !m_head_render && !getAutoZoom( ) && m_zoom )
		{
			req_w = int( phy_h * ar * m_zoom );
			req_h = int( phy_h * m_zoom );
		}
		else
		{
			// Calculate the required display WxH
			req_h = abs_h - 10;
			req_w = int( req_h * ar );

			// Scale up or down to fit
			if ( req_w > abs_w - 10 )
			{
				req_w = abs_w - 10;
				req_h = int( req_w / ar );
			}
		}

		if ( texture_id_ == 0 )
			glGenTextures( 1, &texture_id_ );

		GLenum target;
		float tex_w, tex_h;

		if( opl::texture_target( phy_w, phy_h, target, tex_w, tex_h ) )
		{
   			glClear( GL_COLOR_BUFFER_BIT );

			glDisable( GL_TEXTURE_2D );
			glDisable( GL_DEPTH_TEST );
			glDepthMask( 0 );

			glColor3f( 1.0f, 1.0f, 1.0f ); // GC - either this or the env function.

			glEnable( target );

			glBindTexture( target, texture_id_ );

			glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );

			if ( phy_w != old_phy_w_ || phy_h != old_phy_h_ )
			{
				glTexParameteri( target, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
				glTexParameteri( target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
				glTexParameteri( target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
				glTexParameteri( target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
				glTexImage2D( target, 0, 3, phy_w, phy_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image );
				old_phy_w_ = phy_w;
				old_phy_h_ = phy_h;
			}
			else
			{
				glTexSubImage2D( target, 0, 0, 0, phy_w, phy_h, GL_RGBA, GL_UNSIGNED_BYTE, image );
			}

			glMatrixMode( GL_PROJECTION );
			glPushMatrix( );
			glLoadIdentity( );
			gluOrtho2D( 0.0f, abs_w, 0.0f, abs_h );
			
			glMatrixMode( GL_MODELVIEW );
			glPushMatrix( );
			glLoadIdentity( );

			float off_x = 0.5 * ( m_current_width - req_w );
			float off_y = 0.5 * ( m_current_height - req_h );
			glTranslatef( off_x, off_y, 0.0f );

			float zoom_w = ( float ) req_w / ( float ) phy_w; 
			float zoom_h = ( float ) req_h / ( float ) phy_h;

			glScalef( zoom_w, zoom_h, 1.0f );

			glBegin( GL_QUADS );
				glTexCoord2f( 0.0f, 0.0f );
				glVertex2f( 0.0f, 0.0f );
				glTexCoord2f( tex_w, 0.0f );
				glVertex2f( phy_w, 0.0f );
				glTexCoord2f( tex_w, tex_h );
				glVertex2f( phy_w, phy_h );
				glTexCoord2f( 0.0f, tex_h );
				glVertex2f( 0.0f, phy_h );
			glEnd( );

			glMatrixMode( GL_MODELVIEW );
			glPopMatrix( );

			glMatrixMode( GL_PROJECTION );
			glPopMatrix( );

			glDisable( target );

			glDepthMask( 1 );
			glEnable( GL_DEPTH_TEST );
			glEnable( GL_TEXTURE_2D );
		}
		else
		{
			// These ensure that the image is shown if the user enters the Editor as the first module
			glDisable( GL_TEXTURE_2D );
			glDisable( GL_DEPTH_TEST );
			glDepthMask( 0 );

			// Display the image at the correct aspect ratio and scaled to fill the core as much as possible
   			glClear( GL_COLOR_BUFFER_BIT );
   			glMatrixMode( GL_MODELVIEW );
			glRasterPos2i( - req_w / 2, - req_h / 2 );
			glPixelZoom( double( ( double )req_w / ( double )phy_w ), double( ( double )req_h / ( double )phy_h ) );
			glDrawPixels( phy_w, phy_h, GL_RGBA, GL_UNSIGNED_BYTE, image );

			// Reverses the actions above
			glDepthMask( 1 );
			glEnable( GL_DEPTH_TEST );
			glEnable( GL_TEXTURE_2D );
		}

		// This is used to ensure that the area used of the widget is known (for mouse presses)
		if ( !m_head_render )
			setRegion( ( abs_w - req_w ) / 2, ( abs_h - req_h ) / 2, req_w, req_h );
	}
	else
	{
		glClear( GL_COLOR_BUFFER_BIT );
	}
}

