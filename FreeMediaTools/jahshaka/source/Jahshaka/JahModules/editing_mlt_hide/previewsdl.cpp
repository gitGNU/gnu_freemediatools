/*******************************************************************************
**
** The header file for the Jahshaka/MLT editing module
** The Jahshaka Project
** Copyright (C) 2005 The Jahshaka Project
** Released under the GNU General Public License
**
*******************************************************************************/

#include <glworld.h>
#include <glcore.h>
#include "base.h"
#include "edit.h"
#include "previewsdl.h"
#include "audio.h"
#include "project.h"
#include <qtimer.h>
#include <qmessagebox.h>
using namespace Mlt;
using namespace jahnle;
#include <FreeMediaTools.h>

PreviewSdl::PreviewSdl( EditBase *edit, const QString test_card ) :
	QObject( NULL ),
	Preview( edit->getMediaPath( ) ),
	m_started( false ),
	m_edit( edit ),
	m_project( NULL ),
	m_update_event( NULL ),
	m_test_card_file( test_card ),
	m_test_card( NULL ),
	m_audio( NULL ),
	m_refreshing( false ),
	sdl_screen( NULL ),
	sdl_overlay( NULL ),
	m_ask_audio( true )
{
	setOptions( );
	setPAL( getNormalisation( ) );
	connect( &m_frame_timer, SIGNAL( timeout( ) ), this, SLOT( timeOut( ) ) );
}

PreviewSdl::~PreviewSdl( )
{
	// Stop and close up
	delete m_project;
	delete m_test_card;
	delete m_audio;
	delete_event( m_update_event );

	// Clean up the overlay
	if ( sdl_overlay )
		SDL_FreeYUVOverlay( sdl_overlay );

	// Shutdown the sdl environment
	SDL_Quit( );
}

void PreviewSdl::init( )
{
	// Fetch the 'core' (being the widget we've stolen here)
	GLCore *core = m_edit->getCore( );

	// Initialise the sdl environment
	if ( sdl_screen == NULL )
	{
		QString win_id = QString( "%1" ).arg( ( long )core->winId( ) );
#ifdef WIN32
		Factory::setenv( "SDL_WINDOWID", win_id.ascii( ) );
#else
		setenv( "SDL_WINDOWID", win_id.ascii( ), 1 );
#endif
		SDL_Init( SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE );
	}

	// Create the test card (with the current normalisation)
	delete m_test_card;
	m_test_card = new Producer( ( char * )m_test_card_file.ascii( ) );

	// Clean up the audio
	delete m_audio;
	m_audio = NULL;
}

void PreviewSdl::start( )
{
	// Ensure we pick up the current settings
	setOptions( );

	// Flag as started
	m_started = true;

	// And refresh
	refresh( );
}

void PreviewSdl::stop( )
{
	m_started = false;
}

void PreviewSdl::show( Project *project )
{
	// Remove update event associated to previous project
	delete_event( m_update_event );
	m_update_event = NULL;
	
	// Delete the previous project
	delete m_project;
	m_project = NULL;

	// Connect the new project
	if ( project != NULL )
	{
		// Make sure the project still exists, even if the editor deletes the original
		m_project = new Producer( project );

		// Setup the event for project modifications
		m_update_event = project->listen( "producer-changed", m_edit, ( mlt_listener )updateHandler );
	}
}

void PreviewSdl::renderFrame( Frame *f )
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
		mlt_image_format format = mlt_image_yuv422;
		f->get_image( format, w, h );
		f->set( "rendered", 1 );
	}
}

void PreviewSdl::refresh( )
{
	if ( !m_started || m_project == NULL || m_refreshing ) return;

	// Make sure we don't hit the refresh via a change event from the project now
	m_refreshing = true;

	// Fetch the current frame
	Mlt::Frame *frame = m_project->get_frame( );

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
				int response = QMessageBox::warning( m_edit->getCore( ), 
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
			Mlt::Frame *pre = m_project->get_frame( );
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

	// Pass it to the editor for interogation/mangling
	m_edit->consumerFrameShow( f );

	// Trigger the paint
	if ( frame->get_int( "rendered" ) )
		paint_image( frame );

	delete frame;

	m_refreshing = false;
}

void PreviewSdl::timeOut( )
{
	refresh( );
}

// Modification to the project
void PreviewSdl::updateHandler( mlt_properties, EditBase *self )
{
	self->updateUI( );
	self->checkPoint( );
}

void PreviewSdl::paint_image( Frame *frame )
{
	// Make sure that qt doesn't double buffer the opengl here
	GLCore *core = m_edit->getCore( );
	core->switchAutoBufferSwap( false );

	// This is the absolute WxH we have
	int abs_w = core->width( );
	int abs_h = core->height( );

	// This is the physcial WxH of the image
	int phy_w = frame->get_int( "width" );
	int phy_h = frame->get_int( "height" );
	mlt_image_format format = ( mlt_image_format )frame->get_int( "format" );
	double ar = m_consumer.get_double( "display_ratio" );
	uint8_t *image = frame->get_image( format, phy_w, phy_h );

	// Calculate the required display WxH
	int req_h = abs_h;
	int req_w = int( req_h * ar );

	// Scale up or down to fit
	if ( req_w > abs_w )
	{
		req_w = abs_w;
		req_h = int( req_w / ar );
	}

	// Check if the settings related to the image have changed
	if ( sdl_screen != NULL )
	{
		if ( abs_w != sdl_screen->w || abs_h != sdl_screen->h || 
			 phy_w != sdl_overlay->w || phy_h != sdl_overlay->h || 
			 req_w != sdl_rect.w || req_h != sdl_rect.h )
		{
			SDL_FreeYUVOverlay( sdl_overlay );
			sdl_screen = NULL;
			sdl_overlay = NULL;
		}
	}

	// Create the screen and overlay if needed
	if ( sdl_screen == NULL )
	{
		// Set up the rectangle with the current settings
		sdl_rect.x = ( abs_w - req_w ) / 2; 
		sdl_rect.x -= sdl_rect.x % 2;
		sdl_rect.y = ( abs_h - req_h ) / 2; 
		sdl_rect.w = req_w;
		sdl_rect.h = req_h;

		// Create the surface
		sdl_screen = SDL_SetVideoMode( abs_w, abs_h, 0,  SDL_HWSURFACE | SDL_ASYNCBLIT | SDL_HWACCEL | SDL_RESIZABLE | SDL_DOUBLEBUF );

		// Set up the display rectangle
		if ( sdl_screen )
			SDL_SetClipRect( sdl_screen, &sdl_rect );

		// Create the overlay
		if ( sdl_screen )
			sdl_overlay = SDL_CreateYUVOverlay( phy_w, phy_h, SDL_YUY2_OVERLAY, sdl_screen );
	}

	// Display the image
	if ( sdl_overlay && SDL_LockYUVOverlay( sdl_overlay ) >= 0 )
	{
		uint8_t *buffer = sdl_overlay->pixels[ 0 ];
		if ( image != NULL && buffer != NULL )
		{
			int stride = sdl_overlay->pitches[ 0 ];
			while ( phy_h -- )
			{
				memcpy( buffer, image, phy_w * 2 );
				buffer += stride;
				image += phy_w * 2;
			}
		}

		SDL_UnlockYUVOverlay( sdl_overlay );
		SDL_DisplayYUVOverlay( sdl_overlay, &sdl_screen->clip_rect );
	}

	// This is used to ensure that the area used of the widget is known (for mouse presses)
	setRegion( sdl_rect.x, sdl_rect.y, sdl_rect.w, sdl_rect.h );
}

