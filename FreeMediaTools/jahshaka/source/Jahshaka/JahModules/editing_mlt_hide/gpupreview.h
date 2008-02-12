/*******************************************************************************
**
** The header file for the Jahshaka/MLT editing module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef JAHNLE_PREVIEW_GPU_H
#define JAHNLE_PREVIEW_GPU_H

#include <qwidget.h>
#include <qtimer.h>
#include <qgl.h>
#include <deque>
#include "preview.h"
#include <mlt++/Mlt.h>

class EditBase;

namespace jahnle
{
	class Project;
	class MltAudio;

	class PreviewGpu : public QObject, public jahnle::Preview
	{
		Q_OBJECT

		public:
			PreviewGpu( EditBase *, const QString );
			~PreviewGpu( );

			void setHeadRender( bool on ) { m_head_render = on; }
			void setDimensions( int, int );

			void setZoom( int );

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
			void paint( );

		protected slots:
			void timeOut( );

		private:
			// Project changed handler
			static void updateHandler( mlt_properties, EditBase * );

			// Render the frame
			void renderFrame( Mlt::Frame *f );

			// Flag for indicating started condition
			bool m_started;

			// The editing module to which we belong
			EditBase *m_edit;
			Project *m_project;

			// Frame for app preview
			Mlt::Frame *m_frame;

			// Events for frame display and updates
			Mlt::Event *m_update_event;

			// OpenGL State
			GLuint texture_id_;
			int old_phy_w_, old_phy_h_;

			// Colour space converter
			Mlt::Filter *m_convert;

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

			// Flag for asking about audio if it's unavailable
			bool m_ask_audio;

			// Zoom level
			float m_zoom;

			// Head information
			bool m_head_render;
			int m_current_width;
			int m_current_height;
	};
}

#endif
