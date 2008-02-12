/*******************************************************************************
**
** The header file for the Jahshaka/MLT editing module
** The Jahshaka Project
** Copyright (C) 2005 The Jahshaka Project
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef JAHNLE_PREVIEW_SDL_H
#define JAHNLE_PREVIEW_SDL_H

#include <qwidget.h>
#include <qtimer.h>
#include <qgl.h>
#include <deque>
#include "preview.h"
#include <mlt++/Mlt.h>
#include <SDL/SDL.h>

class EditBase;

namespace jahnle
{
	class Project;
	class MltAudio;

	class PreviewSdl : public QObject, public jahnle::Preview
	{
		Q_OBJECT

		public:
			PreviewSdl( EditBase *, const QString );
			~PreviewSdl( );

			// Initialiser
			void init( );

			// Connect to this project
			void show( Project * );

			// Start, Stop and Refresh
			bool is_valid( ) { return true; }
			void start( );
			bool is_stopped( ) { return !m_started; }
			void stop( );
			void refresh( );

			// OpenGL Paint method
			void paint( ) { refresh( ); }

		protected slots:
			void timeOut( );

		private:
			// Show the image
			void paint_image( Mlt::Frame *);

			// Project changed handler
			static void updateHandler( mlt_properties, EditBase * );

			// Render the frame
			void renderFrame( Mlt::Frame * );

			// Flag for indicating started condition
			bool m_started;

			// The editing module to which we belong
			EditBase *m_edit;
			Mlt::Producer *m_project;

			// Events for frame display and updates
			Mlt::Event *m_update_event;

			// Test card producer
			QString m_test_card_file;
			Mlt::Producer *m_test_card;

			// Timer for non-paused modes
			QTimer m_frame_timer;

			// Audio player
			MltAudio *m_audio;

			// Preroll
			std::deque < Mlt::Frame * > m_preroll;

			// Flag to ensure we don't get events bubbling during refresh
			bool m_refreshing;

			// SDL environment
			SDL_Surface *sdl_screen;
			SDL_Overlay *sdl_overlay;
			SDL_Rect sdl_rect;

			// Flag for asking about audio if it's unavailable
			bool m_ask_audio;
	};
}

#endif
