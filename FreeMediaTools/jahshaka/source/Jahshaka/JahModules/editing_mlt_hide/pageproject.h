/*******************************************************************************
**
** The header file for the Jahshaka/MLT editing module project page
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef JAHNLE_PAGE_PROJECT_H
#define JAHNLE_PAGE_PROJECT_H

#include "page.h"

class GLEdit;
class SupergRangeControl;

// MLT forward declarations
namespace Mlt
{
	class Producer;
}

namespace jahnle
{
	class PageProject : public Page
	{
		Q_OBJECT

		public:
			PageProject( QWidget *parent, GLEdit *edit );
			~PageProject( );
			void restart( );
			void refresh( );
			void setPosition( int );

		public slots:
			// Multitrack related slots
			void multitrackItemsMoved( Qt::ButtonState, const QValueList<MultiTrackItem*>&, int );
			void multitrackTrackSelected( Qt::ButtonState, MultiTrackTrack * );
			void setAddMode( int );

			// Track manipulations
			void trackManipulations( );

			// In/out related slots
			void positionPushed( QMouseEvent * );
			void positionReleased( );
			void inPositionChanged( int );
			void outPositionChanged( int );

			// Zoom in/out
			void zoomIn( );
			void zoomOut( );
			void resetZoom( );

			// Key events
			void processKeyEvent( QKeyEvent * );

			// Checks for double clicks
			void trackSelectEvent( QMouseEvent * );
			void itemSelectEvent( QMouseEvent * );

			// Drag n drop behaviour
			void multitrackDragEnterEvent( QDragEnterEvent * );
			void multitrackDropEvent( QDropEvent * );

			// Slots for silence and lock signals
			void multitrackSilenceClicked( MultiTrackTrack * );
			void multitrackEnableClicked( MultiTrackTrack * );
			void multitrackLockedClicked( MultiTrackTrack * );

		protected:
			// Add/remove track
			void addTrack( );
			void insertTrack( );
			void removeTrack( );
			void moveTrackUp( );
			void moveTrackDown( );

			void copy( );
			void cut( );
			void paste( );

		private:
			// Cut management
			QComboBox *m_add_mode_box;
			SupergRangeControl *m_in_input;
			SupergRangeControl *m_out_input;
			SupergRangeControl *m_duration_output;

			// Project Multitrack
			MultiTrack *m_multitrack;

			// Tracks
			QButton *m_tracks;

			// Paste object
			Mlt::Producer *m_paste;
	};
}

#endif
