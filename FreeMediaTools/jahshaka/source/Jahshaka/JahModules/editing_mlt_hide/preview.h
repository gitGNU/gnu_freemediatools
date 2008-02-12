/*******************************************************************************
**
** The header file for the Jahshaka/MLT editing module preview abstraction
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef JAHNLE_PREVIEW_H
#define JAHNLE_PREVIEW_H

#include <qcombobox.h>
#include <mlt++/Mlt.h>

class QComboBox;

namespace jahnle
{
	class Project;

	class Preview
	{
		public:
			// Constructor/destructor
			explicit Preview( const QString &base_path );
			virtual ~Preview( );

			// Zoom levels to be handled on a per class basis
			void setAutoZoom( bool on ) { m_user_prefs.set( "auto_zoom", on ); refresh( ); }
			bool getAutoZoom( ) { return m_user_prefs.get_int( "auto_zoom" ) != 0; }
			virtual void setZoom( int ) { }
			virtual void setHeadRender( bool ) { }
			virtual void setDimensions( int, int ) { }

			// Populate combo boxes and set each to user preference
			void populateNormalisation( QComboBox *box );
			void populateProgressive( QComboBox *box );
			void populateRescale( QComboBox *box );
			void populateAspect( QComboBox *box );
			void populateResolution( QComboBox *box );
			void populateAudio( QComboBox *box );
			void populateAudioDriver( QComboBox *box );
			void populateMode( QComboBox *box );

			// Set all options from preferences
			void setOptions( );

			// Methods to handle combo box translations
			void setNormalisation( int );
			int getNormalisation( );
			void setProgressive( int );
			void setRescale( int );
			void setAspect( int );
			void setSize( int );
			void setAudio( int );
			void setAudioDriver( int );
			void setMode( int );

			// Save properties
			void save( );

			// PAL or NTSC
			bool isPAL( ) { return getNormalisation( ); }

			// UGLY
			void setPAL( bool pal ) 
			{ 
				Preview::setNormalisation( pal ? 1 : 0 ); 
#ifdef WIN32
				Mlt::Factory::init( );
				Mlt::Factory::setenv( "MLT_NORMALISATION", pal ? "PAL" : "NTSC" );
#else
				setenv( "MLT_NORMALISATION", pal ? "PAL" : "NTSC", 1 );
				Mlt::Factory::init( );
#endif
				init( );
			}

			// Progressive or interlaced
			bool isProgressive( ) { return m_consumer.get_int( "progressive" ); }
			const char *getDeinterlaceMethod( ) { return m_consumer.get( "deinterlace_method" ); }

			// Rescaling method
			const char *getRescale( ) { return m_consumer.get( "rescale" ); }

			// Width and height
			double getAspectRatio( ) { return m_consumer.get_double( "aspect_ratio" ); }
			double getDisplayRatio( ) { return m_consumer.get_double( "display_ratio" ); }
			int getWidth( ) { return m_consumer.get_int( "width" ); }
			int getHeight( ) { return m_consumer.get_int( "height" ); }
			void getSize( int &w, int &h ) { w = getWidth( ); h = getHeight( ); }

			void setRegion( int x, int y, int w, int h )
			{
				m_consumer.set( "rect_x", x );
				m_consumer.set( "rect_y", y );
				m_consumer.set( "rect_w", w );
				m_consumer.set( "rect_h", h );
			}

			void getRegion( double &x, double &y, double &w, double &h )
			{
				x = m_consumer.get_double( "rect_x" );
				y = m_consumer.get_double( "rect_y" );
				w = m_consumer.get_double( "rect_w" );
				h = m_consumer.get_double( "rect_h" );
			}

			// Initialisation
			virtual void init( ) = 0;

			// Store the profile on the project provided
			void storeProfile( Project * );

			// Connect to this project
			virtual void show( Project * ) = 0;

			// Start, Stop and Refresh
			virtual void start( ) = 0;
			virtual bool is_valid( ) = 0;
			virtual bool is_stopped( ) = 0;
			virtual void stop( ) = 0;
			virtual void refresh( ) = 0;

			// Paint related methods
			virtual void paint( ) = 0;

		protected:
			// Implementation specific handling of settings
			void setNormalisation( const char *norm ) 
			{ m_consumer.set( "normalisation", norm ); }
			void setDeinterlace( const char *method ) 
			{ m_consumer.set( "deinterlace_method", method ); m_consumer.set( "progressive", strcmp( method, "none" ) ); }
			void setRescale( const char *method )
			{ m_consumer.set( "rescale", method ); m_consumer.set( "resize", strcmp( method, "none" ) ); }
			void setSize( int w, int h ) { setWidth( w ); setHeight( h ); }
			void setAspectRatio( double ar, double dr )
			{ m_consumer.set( "aspect_ratio", ar ); m_consumer.set( "display_ratio", dr ); }
			void setAudioFrequency( int frequency );

			// TODO: Implement support for driver
			void setAudioDriver( const char * ) { }

			void setAudioMode( const char *setting ) 
			{
				m_play_mode = 0;
				if ( !strcmp( setting, "on" ) )
					m_play_mode = 1;
				else if ( !strcmp( setting, "off" ) )
					m_play_mode = 2;
			}

			// Convenience methods for width/height
			void setWidth( int w ) { m_consumer.set( "width", w ); }
			void setHeight( int h ) { m_consumer.set( "height", h ); }

			// Consumer settings
			Mlt::Properties m_consumer;

			// Audio playback mode (0 = drop images, 1 = show all, 2 = audio off)
			int m_play_mode;

		private:
			QString m_file_name;
			Mlt::Properties m_user_prefs;
	};
}

#endif
