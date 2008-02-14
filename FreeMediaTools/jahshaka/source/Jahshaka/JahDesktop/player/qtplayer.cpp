
// ml - A media library representation.

// Copyright (C) 2000-2006 VM Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#include <qvaluelist.h>
#include <qfile.h>
#include <cmath>

#include <boost/thread/thread.hpp>
#include <boost/thread/condition.hpp>

#include <openpluginlib/pl/openpluginlib.hpp>
#include <openpluginlib/pl/utf8_utils.hpp>
#include <openpluginlib/pl/GL_utility.hpp>

#include <openmedialib/ml/ml.hpp>

#include <qgl.h>

//TODO: dont have this #include <../../JahModules/editing_mlt/audio.h>
#include <jahheadable.h>

#include <qtimer.h>
#include <qdragobject.h>
#include <qmime.h>
#include <qstrlist.h>
#include <qevent.h>
#include <qcursor.h>
#include <qmessagebox.h>
#include <cmath>
#include <iostream>
#include <deque>
#include "qtplayer.h"

namespace ml  = olib::openmedialib::ml;
namespace il  = olib::openimagelib::il;
namespace opl = olib::openpluginlib;

QtPlayer::QtPlayer( const QGLWidget *, const std::string &input, int speed ,
		    QWidget *parent, 
		    char *name )
		: QGLWidget( parent, name, 0, Qt::WDestructiveClose )
	  //TODO DOnt have		, audio_( 0 )
		, mouse_moved_( 1 )
		, speed_( 1 )
		, accept_move_( false )
		, accept_jog_( false )
		, old_target_( 0 )
		, old_phy_w_( 0 )
		, old_phy_h_( 0 )
		, old_speed_( 0 )
		, last_frame_( -1 )
		, volume_( 1.0f )
		, deinterlace_mode_( 1 )
		, writable_images_( false )
		, zoom_( 1.0 )
		, eof_( loop )
		, sequence_fps_( 0.0 )
		, ask_audio_( true )
		, mute_( false )
		, buffer_id_( 0 )
		, gpu_can_handle_( true )
	{
		// Enable mouse tracking and drops
		setMouseTracking( true );
		setAcceptDrops( true );

		// Create a timer to control a/v playout
		timer_ = new QTimer( this );
		connect( timer_, SIGNAL( timeout( ) ), this, SLOT( timeOutSlot( ) ) );

		// Mouse timer hides the mouse after approx 5 seconds when no mouse movement in fullscreen
		mouse_ = new QTimer( this );
		connect( mouse_, SIGNAL( timeout( ) ), this, SLOT( mouseSlot( ) ) );
		mouse_->start( 5000 );

		// Play the input
		if ( !play( input ), speed )
		  {
		    ///test is a reference to teh 
		    /*
/mnt/sda10/mdupont/experiments/video/js/openlibraries/trunk/src/openmedialib/plugins/template/template_plugin.opl 
open plug in 
*/
			play( std::string( "test:" ), speed );
		  }
	}
	
	 QtPlayer::~QtPlayer( )
	{
		delete timer_;
		delete mouse_;
		//		delete audio_;
	}

	void QtPlayer::headRender( int w , int h  )
	{
		double orig_zoom = zoom_;
		zoom_ = 0.0;
		current_width_ = w;
		current_height_ = h;
		paintGL( );
		current_width_ = width( );
		current_height_ = height( );
		zoom_ = orig_zoom;
	}

	bool QtPlayer::play( const QString &input, int speed  )
	{
		if ( !play( std::string( input.latin1( ) ), speed ) )
			return play( std::string( ( input + QString( "/sequence:" ) ).latin1( ) ), 0 );
		return true;
	}

	bool QtPlayer::play( const std::string &input, int speed  )
	{
	  std::wcerr << "play input : " << input.c_str() << "\n";
	  
	  const char * name=input.c_str( );

	  olib::openmedialib::ml::input_type_ptr item = ml::create_input(name);
	    
	  if (item != 0)
	    {
	      return play( item , speed );
	    }
	  else
	    {
	      std::wcerr << "load failed  : " << input.c_str() << "\n";
	      return false;
	    }
	}

	bool QtPlayer::play( ml::input_type_ptr input, int speed )
	{
		if ( input != 0 )
		{
			// Pause the current playback
			if ( input_ ) set_speed( 0 );

			// Set everything back to defaults
			input_ = input;
			field_ = ml::frame_type_ptr( );
			current_frame_ = input->fetch( );
			preroll_.clear( );
			last_frame_ = -1;

			// Start the video
			set_speed( speed );
		}
		return input != 0;
	}

    void QtPlayer::stop()
    {
        set_speed(0);
    }


    /*
      ever heard of  not blowing up header files with huge inline functions?
    */
    void QtPlayer::set_speed( int speed, bool save )
      {

#ifdef HAVE_AUDIO_MLT
		if ( save ) old_speed_ = speed_;
		speed_ = speed;


		if ( speed_ == 1 && audio_ == 0 )
		{
			audio_ = new jahnle::MltAudio( );

			if ( !audio_->init( ) )
			{
				while ( ask_audio_ && !audio_->init( ) )
				{
					int response = QMessageBox::warning( this, "Jahshaka", "Audio device is unavailable.\nDo you want to retry?", "Retry", "Ignore", 0, 0, 1 );

					delete audio_;
					audio_ = NULL;

					switch( response )
					{
    					case 0:
        					audio_ = new jahnle::MltAudio( );
        					break;
    					case 1: 
							ask_audio_ = false;
        					break;
    				}
				}
			}
		}
		else if ( speed_ != 1 )
		{
			delete audio_;
			audio_ = 0;
		}

		timer_->stop( );
		audio_flush( );
		if ( input_ ) input_->seek( position( ) );
		preroll_.clear( );

		if ( input_ && speed_ )
		{
			// Make sure we process everything during the preroll
			input_->set_process_flags( ml::process_image | ml::process_audio );

			// Fetch a frame from the input
			ml::frame_type_ptr frame = input_->fetch( );

			// Ensure that this frame is shown first
			current_frame_ = frame;

			// TODO: Parameterise preroll
			for ( int i = 0; speed_ == 1 && i < 8; i ++ )
			{
				visualise( frame );
				audio_play( frame );
				preroll_.push_back( frame );
				input_->seek( 1, true );
				frame = input_->fetch( );
			}

			// Delay between timer events
			int delay = 0;

			// If we have have audio, we'll rely on the audio device for clocking, otherwise:
			if ( speed_ == 1 && ( !audio_ || mute_ || !frame->get_audio( ) ) )
			{
				// Calculate the delay in milliseconds
				delay = int( ( sequence_fps_ == 0.0 || input_->get_frames( ) == 1 ? frame->get_duration( ) : 1.0 / sequence_fps_ ) * 1000 );

				// Attempt to play out all fields for interlaced sources
				il::image_type_ptr image = frame->get_image( );
				if ( deinterlace_mode_ == 0 && ( image && image->field_order( ) != il::progressive ) )
					delay /= 2;
			}

			// Start the timer
			timer_->start( delay );
		}
		else if ( input_ )
		{
			input_->set_process_flags( ml::process_image | ml::process_audio );
			current_frame_ = input_->fetch( );
		}
#endif 
		updateGL( );
		emit showInHead( this );
	}

	void QtPlayer::restore_speed( )
	{
		if ( old_speed_ != speed_ )
			set_speed( old_speed_, false );
	}

	int QtPlayer::speed( )
	{
		return speed_;
	}

	void QtPlayer::set_fps( float fps )
	{
		sequence_fps_ = fps;
		if ( mute_ || ( current_frame_ && current_frame_->get_audio( ) == 0 ) )
		{
			int delay = int( ( sequence_fps_ == 0.0 ? current_frame_->get_duration( ) : 1.0 / sequence_fps_ ) * 1000 );
			timer_->stop( );
			timer_->start( delay );
		}
	}

	int QtPlayer::position( )
	{
		int pos = 0;
		if ( current_frame_ )
			pos = current_frame_->get_position( );
		else if ( !preroll_.empty( ) )
			pos = ( *preroll_.begin( ) )->get_position( );
		else if ( input_ )
			pos = input_->get_position( );
		return pos;
	}

	void QtPlayer::seek( int position )
	{
		input_->seek( position ); 
		if ( speed_ == 0 )
		{
			input_->set_process_flags( ml::process_image | ml::process_audio );
			current_frame_ = input_->fetch( );
			updateGL( );
			emit showInHead( this );
		}
	}

	int QtPlayer::length( )
	{
		int len = 0;
		if ( input_ ) len = input_->get_frames( );
		return len;
	}

	void QtPlayer::toggle_full_screen( )
	{
		if ( isFullScreen( ) ) 
			showNormal( ); 
		else 
			showFullScreen( ); 
		setCursor( QCursor( Qt::ArrowCursor ) );
	}

	void QtPlayer:: toggle_mute( )
	{
		mute_ = !mute_;
		set_speed( 0 );
		restore_speed( );
	}

	float QtPlayer:: volume( )
	{
		return volume_;
	}

	void QtPlayer:: set_volume( float volume )
	{
		volume_ = volume;
	}

	void  QtPlayer::refresh( )
	{
		updateGL( );
		emit showInHead( this );
	}

	void QtPlayer:: ensureWritableImages( bool on )
	{
		writable_images_ = on;
	}

	void QtPlayer:: zoom( double value )
	{
		zoom_ = value;
		refresh( );
	}


	void QtPlayer:: set_eof( eof_condition eof )
	{
		eof_ = eof;
	}

	QtPlayer::eof_condition  QtPlayer::get_eof( )
	{
		return eof_;
	}

	void  QtPlayer::switch_deinterlace_mode( )
	{
		int current = speed_;
		if ( ++ deinterlace_mode_ == 3 )
			deinterlace_mode_ = 0;
		input_->seek( position( ) );
		play( input_, current );
	}


    void  QtPlayer::dragEnterEvent( QDragEnterEvent *event )
    {
    	event->accept( true );
    }

	void  QtPlayer::dropEvent( QDropEvent *event )
	{
		QStringList list;
		if ( QUriDrag::decodeLocalFiles( event, list ) )
			play( list[ 0 ] );
		else
			emit draggedAImage( );
	}

	void  QtPlayer::mousePressEvent( QMouseEvent *e )
	{
		e->accept( );
		if ( e->type( ) == QEvent::MouseButtonDblClick )
		{
			toggle_full_screen( );
		}
		else if ( e->button( ) & Qt::MidButton )
		{
			set_speed( !speed_ );
		}
		else if ( e->button( ) & Qt::RightButton )
		{
			accept_move_ = true;
			old_speed_ = speed_;
			mouseMoveEvent( e );
		}
		else if ( e->button( ) & Qt::LeftButton )
		{
			accept_jog_ = true;
			jog_x_ = e->x( );
			old_speed_ = speed_;
		}
	}

	void  QtPlayer::mouseMoveEvent( QMouseEvent *e )
	{
		e->accept( );
		if ( accept_move_ )
		{
			int position = int( double( input_->get_frames( ) * e->x( ) ) / width( ) );
			input_->seek( position );
			set_speed( 0, false );
		}
		else if ( accept_jog_ )
		{
			if ( jog_x_ != e->x( ) )
			{
				int sign = e->x() < jog_x_ ? -2 : 2;
				int offset = int( std::log( std::fabs( double( jog_x_ - e->x( ) ) ) ) );
				set_speed( sign * offset, false );
			}
		}

		if ( isFullScreen( ) )
		{
			if ( mouse_moved_ == -1 )
				setCursor( QCursor( Qt::ArrowCursor ) );
			mouse_moved_ = 1;
		}
	}

	void  QtPlayer::mouseReleaseEvent( QMouseEvent *e )
	{
		e->accept( );
		if ( accept_move_ || accept_jog_ )
		{
			accept_move_ = false;
			accept_jog_ = false;
			restore_speed( );
		}
	}

	void  QtPlayer::initializeGL()
	{
		input_->set_process_flags( ml::process_image | ml::process_audio );
		current_frame_ = input_->fetch( );

		visualise( current_frame_ );

		if ( current_frame_->get_image( ) != 0 )
		{
			int phy_w, phy_h, req_w, req_h;
			calculate_dimensions( current_frame_, phy_w, phy_h, req_w, req_h );

			float tex_w, tex_h;
			GLenum target;
			if ( opl::texture_target( phy_w, phy_h, target, tex_w, tex_h ) )
			{
				glGenTextures( 1, &texture_id_ );
				glBindTexture( target, texture_id_ );
				glTexParameteri( target, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
				glTexParameteri( target, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
				glTexParameteri( target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
				glTexParameteri( target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
				old_target_ = target;
			}
		}
	}
	
	void  QtPlayer::resizeGL( int w, int h )
	{
		current_width_ = w;
		current_height_ = h;
		glViewport( 0, 0, w, h );
	}
	
	// Very silly
	void  QtPlayer::visualise( ml::frame_type_ptr frame )
	{
		if ( frame->get_image( ) == 0 && frame->get_audio( ) != 0 )
		{
			ml::audio_type_ptr audio = frame->get_audio( );
			il::image_type_ptr image = il::allocate( L"r8g8b8", 640, 480 );
			memset( image->data( ), 0, image->size( ) );

			short *buff = ( short * )audio->data( );
			int samples = audio->samples( );
			int channels = audio->channels( );

			double sample_offset = 640.0 / samples;
			int pitch = image->pitch( );
			unsigned char *middle = image->data( ) + pitch * image->height( ) / 2;

			short sample;
			unsigned char *p;
			int i, j;
			int offset;

			for ( i = 0; i < 640; i ++ )
			{
				offset = i * 3;
				for ( j = 0; j < channels; j ++ )
				{
					sample = ( buff + 2 * int( sample_offset * i * channels ) )[ j ] / 140;
					p = middle - sample * pitch + offset;
					*p ++ = j == 0 ? 255 : 0;
					*p ++ = j == 0 ? 0 : 255;
					*p ++ = 0;
				}
			}

			frame->set_image( image );
		}
	}

	ml::frame_type_ptr  QtPlayer::levels( ml::frame_type_ptr frame )
	{
		ml::audio_type_ptr audio = frame->get_audio( );
		if ( audio )
		{
			int samples = audio->samples( ) * audio->channels( );
			short int *ptr = ( short int * )( audio->data( ) );
			while ( samples -- )
			{
				*ptr = short( volume_ * *ptr );
				ptr ++;
			}
		}
		return frame;
	}

	bool  QtPlayer::proxy_texture_target_test( GLuint id, GLenum target, GLenum internal_format, GLenum format, GLenum type, int width, int height )
	{
		GLenum proxy_target;

		//TODO : this is not defined at all
#ifdef HAVE_proxy_texture_target
		if( opl::proxy_texture_target( width, height, proxy_target ) )
		{
			glBindTexture( target, id );
			glTexImage2D( proxy_target, 0, internal_format, width, height, 0, format, type, 0 );
		
			GLint w, h;
			glGetTexLevelParameteriv( proxy_target, 0, GL_TEXTURE_WIDTH, &w );
			glGetTexLevelParameteriv( proxy_target, 0, GL_TEXTURE_HEIGHT, &h );
		
			return w != 0 && h != 0;
		}
#endif
		
		return false;
	}

	void  QtPlayer::paintGL()
	{
		if ( current_frame_ == 0 ) return;

		visualise( current_frame_ );
		ml::image_type_ptr image = current_frame_->get_image( );

		if ( image != 0 )
		{
			glDisable( GL_DEPTH_TEST );
			glDepthMask( 0 );

			glClearColor( 0, 0, 0, 1 ); 
			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

			glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );

			int phy_w, phy_h, req_w, req_h;
			calculate_dimensions( current_frame_, phy_w, phy_h,
					      req_w, req_h );

			std::pair<int, GLenum> glformat;

#ifdef USE_pf_to_gl_format_DEFINED
			// this function has changed, TODO 

			  glformat=
			    opl::pf_to_gl_format( image->pf( ).c_str( ) );
#endif

			ml::image_type_ptr new_im;
			// Convert to a format which (hopefully) the GPU knows
			if( !glformat.first )
			{
				new_im = il::convert( image, L"b8g8r8" );

				glformat.first  = 3;
				glformat.second = GL_BGR_EXT;
			}
			else
				new_im = image;

			GLenum type = GL_UNSIGNED_BYTE;
			if( new_im->pf( ) == L"yuv422" )
				type = GL_UNSIGNED_SHORT_8_8_APPLE;

			// If the input is caching stuff, allow it to refresh with the colour space
			// converted image
			current_frame_->set_image( new_im );
			new_im->set_writable( false );
			input_->refresh_cache( current_frame_ );

			float tex_w, tex_h;
			GLenum target;
			if ( opl::texture_target( phy_w, phy_h, target, tex_w, tex_h ) )
			{
				// Set up the conform flags
				int flags = il::cropped;
				if ( writable_images_ )
					flags |= il::writable;

				// Conform to cropped and not flipped or flopped (optimal for video playout)
				new_im = il::conform( new_im, flags );

				// Emit the image
				int bits = get_bpp( new_im );
				if ( bits == 24 || bits == 32 )
				{
					QImage qim( new_im->data( ), new_im->width( ), new_im->height( ), bits, 0, 0, QImage::IgnoreEndian );
					emit showImage( current_frame_->get_position( ), &qim );
				}
				else
				{
					emit showImage( current_frame_->get_position( ), 0 );
				}

				// Determine if we should be showing field 0, 1 or the whole frame
				if ( image->field_order( ) != il::progressive )
				{
					if ( deinterlace_mode_ == 0 )
					{
						new_im = il::field( new_im, !field_ && last_frame_ <= current_frame_->get_position( ) ? 0 : 1 );
						field_ = field_ ? ml::frame_type_ptr( ) : current_frame_;
						last_frame_ = current_frame_->get_position( );
					}
					else if ( deinterlace_mode_ == 1 )
					{
						new_im = il::deinterlace( new_im );
					}
				}

				glEnable( target );

				if ( old_target_ != target )
				{
					glDeleteTextures( 1, &texture_id_ );
					glGenTextures( 1, &texture_id_ );
					old_target_ = target;
					old_phy_w_ = old_phy_h_ = 0;
				}

				glBindTexture( target, texture_id_ );
				if ( phy_w != old_phy_w_ || phy_h != old_phy_h_ )
				{
					glTexParameteri( target, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
					glTexParameteri( target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
					glTexParameteri( target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
					glTexParameteri( target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
					glTexImage2D( target, 0, glformat.first, phy_w, phy_h, 0, glformat.second, type, new_im->data( ) );
					old_phy_w_ = phy_w;
					old_phy_h_ = phy_h;
				}
				else if ( input_->get_process_flags( ) | ml::process_image )
				{
					glTexSubImage2D( target, 0, 0, 0, phy_w, phy_h, glformat.second, type, new_im->data( ) );
				}

				glMatrixMode( GL_PROJECTION );
				glPushMatrix( );
				glLoadIdentity( );
				gluOrtho2D( 0.0f, width( ), 0.0f, height( ) );
			
				glMatrixMode( GL_MODELVIEW );
				glPushMatrix( );
				glLoadIdentity( );

				float off_x = 0.5 * ( width( ) - req_w );
				float off_y = 0.5 * ( height( ) - req_h );
				glTranslatef( off_x, off_y, 0.0f );

				float zoom_w = ( float ) req_w / ( float ) phy_w; 
				float zoom_h = ( float ) req_h / ( float ) phy_h;
				glScalef( zoom_w, zoom_h, 1.0f );

				if ( bits == 32 )
				{
					glEnable( GL_BLEND );
					glBlendFunc( GL_SRC_ALPHA, GL_ONE );
				}

				glBegin( GL_QUADS );
				glTexCoord2f( 0.0f, 0.0f );
				glVertex2f( 0.0f, phy_h );
				glTexCoord2f( tex_w, 0.0f );
				glVertex2f( phy_w, phy_h );
				glTexCoord2f( tex_w, tex_h );
				glVertex2f( phy_w, 0.0f );
				glTexCoord2f( 0.0f, tex_h );
				glVertex2f( 0.0f, 0.0f );
				glEnd( );

				if ( bits == 32 )
					glDisable( GL_BLEND );

				glMatrixMode( GL_MODELVIEW );
				glPopMatrix( );

				glMatrixMode( GL_PROJECTION );
				glPopMatrix( );

				glDisable( target );
			}
			else
			{
				// TODO: Correct for the interlaced case

				// Since we don't have a texture, crop, flip and don't flop
				new_im = il::conform( new_im, il::cropped | il::flipped );
				current_frame_->set_image( new_im );

				glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );

				// Display the image at the correct aspect ratio and scaled to fill the core as much as possible
				float off_x = 0.5 * ( width( ) - req_w );
				float off_y = 0.5 * ( height( ) - req_h );

				glWindowPos2f( off_x, height( ) - off_y - req_h );
				glPixelZoom( ( ( float )req_w / phy_w ), ( ( float )req_h / phy_h ) );
   				glDrawPixels( phy_w, phy_h, glformat.second, type, new_im->data( ) );
   				
   				glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
			}
			
			glDepthMask( 1 );
			glEnable( GL_DEPTH_TEST );
		}

#	ifndef NDEBUG
		GLenum error;
		while( ( error = glGetError( ) ) != GL_NO_ERROR )
			fprintf( stderr, "GLerror:%s\n", gluErrorString( error ) );
#	endif

	}
	
	int  QtPlayer::get_bpp( ml::image_type_ptr image )
	{
		if ( image->pf( ) == L"r8g8b8a8" )
			return 32;
		else if ( image->pf( ) == L"b8g8r8a8" )
			return 32;
		else if ( image->pf( ) == L"r8g8b8" )
			return 24;
		else if ( image->pf( ) == L"b8g8r8" )
			return 24;
		return 0;
	}



	void  QtPlayer::contextMenuEvent( QContextMenuEvent * )
	{
		// We want to disable the jah context menu
	}

	void  QtPlayer::timeOutSlot()
	{
		if ( speed_ != 0 )
		{
			// Seek to the next frame
			if ( field_ == 0 )
				input_->seek( speed_, true );

			// We need to determine which frame components to process
			// based on the state of the audio player (if it's running 
			// empty, drop an image)

			int flags = ml::process_image | ml::process_audio;

#ifdef HAVE_AUDIO_MLT
			if ( speed_ == 1 && audio_ && audio_->empty( ) )
				flags = ml::process_audio;
#endif
			input_->set_process_flags( flags );

			// If we're playing in full field mode, then reuse the previous frame
			ml::frame_type_ptr frame = field_ ? field_ : input_->fetch( );

			// If we're not reusing the previous frame and a new frame exists...
			if ( !field_ && frame )
			{
				// Special case - handle the loop here so that we don't repeat frames
				if ( ( eof_ == loop || eof_ == ramplayer ) && frame->get_position( ) == input_->get_frames( ) - 1 )
					input_->seek( 0 );

				// Do audio visualisation if only audio exists
				if ( flags & ml::process_image )
					visualise( frame );
	
				// If we're in normal playback mode, process the audio
				if ( speed_ == 1 )
				{
#ifdef HAVE_AUDIO_MLT
					if ( audio_ && !mute_ ) audio_->push( levels( frame ) );
#endif
					preroll_.push_back( frame );
					frame = *( preroll_.begin( ) );
					preroll_.erase( preroll_.begin( ) );
				}
			}
	
			// Update the current frame and display it
			if ( frame->get_image( ) != 0 )
			{
				current_frame_ = frame;
				updateGL();
			}

			// Handle termination conditions
			if ( frame->get_position( ) == input_->get_frames( ) - 1 )
			{
				switch( eof_ )
				{
					case pause:
						set_speed( 0 );
						break;

					case loop:
					case ramplayer:
						break;

					case next:
						set_speed( 0 );
						emit requestNextItem( );
						break;
				}
			}

			emit showInHead( this );
		}
	}

	 void  QtPlayer::mouseSlot()
	{
		if ( isFullScreen( ) )
		{
			if ( !mouse_moved_ )
			{
				setCursor( QCursor( Qt::BlankCursor ) );
				mouse_moved_ = -1;
			}
			else if ( mouse_moved_ != -1 )
			{
				mouse_moved_ = 0;
			}
		}
	}


	int  QtPlayer::deinterlace_mode( )
	{
		return deinterlace_mode_;
	}

	 void  QtPlayer::calculate_dimensions( ml::frame_type_ptr frame, int &phy_w, int &phy_h, int &req_w, int &req_h )
	{
		ml::image_type_ptr image = frame->get_image( );
	
		// This is the absolute WxH we have
		int abs_w = current_width_;
		int abs_h = current_height_;
	
		// This is the physcial WxH of the image
		double ar = frame->aspect_ratio( );
		phy_w = image ? image->width( ) : old_phy_w_;
		phy_h = image ? image->height( ) : old_phy_h_;
	
		// In lieu of a deinterlacer, we'll attempt to show both fields
		if ( image && deinterlace_mode_ == 0 && image->field_order( ) != il::progressive )
			phy_h /= 2;

		if ( zoom_ > 0.0 )
		{
			req_h = phy_h;
			req_w = int( req_h * ar );
			req_h = int( zoom_ * req_h );
			req_w = int( zoom_ * req_w );
		}
		else
		{
			// Calculate the required display WxH
			req_h = abs_h;
			req_w = int( req_h * ar );
	
			// Scale up or down to fit
			if ( req_w > abs_w )
			{
				req_w = abs_w;
				req_h = int( req_w / ar );
			}
		}
	}

	void  QtPlayer::audio_play( ml::frame_type_ptr frame )
	{
#ifdef HAVE_AUDIO_MLT
		if ( audio_ && !mute_ && frame->get_audio( ) != 0 ) 
			audio_->push( levels( frame ) );
#endif
	}

	void  QtPlayer::audio_flush( )
	{
#ifdef HAVE_AUDIO_MLT
		if ( audio_ ) audio_->flush( );
#endif
	}


/*	void  QtPlayer::showImage( int position, QImage *image );
	void  QtPlayer::draggedAImage( );
	void  QtPlayer::requestNextItem( );
	void  QtPlayer::showInHead( JahHeadable * );
*/
