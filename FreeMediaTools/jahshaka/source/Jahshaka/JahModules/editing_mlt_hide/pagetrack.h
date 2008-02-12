/*******************************************************************************
**
** The header file for the Jahshaka/MLT editing module track page
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef JAHNLE_PAGE_TRACK_H
#define JAHNLE_PAGE_TRACK_H

#include "page.h"

class GLEdit;

namespace jahnle
{
	class Table;

	class PageTrack : public Page
	{
		Q_OBJECT

		public:
			PageTrack( QWidget *parent, GLEdit *edit );
			~PageTrack( );
			void restart( );
			void refresh( );
			void setPosition( int );
			bool acceptsSelection( ) { return m_accepts_selection; }
			void startSelection( double, double );
			void moveSelection( double, double );
			void setSelection( double, double );

		protected:
			void updateSettings( Mlt::Playlist & );
			void showProperties( Mlt::Properties * );
			const char *getTitle( Mlt::Properties * );
			QString getGeometry( double, double );
			void showSelection( bool );

		public slots:
			void trackSelected( Qt::ButtonState, MultiTrackTrack * );
			void valueChanged( int, int );
			void enableClicked( MultiTrackTrack * );

		private:
			// Track Multitrack
			GLEdit *m_edit;
			MultiTrack *m_multitrack;
			Table *m_table;
			QCheckBox *m_disable;
			Mlt::Properties *m_properties;
			QString m_name_selection;
			bool m_accepts_selection;
			Mlt::Filter *m_indicate;
			Mlt::Geometry *m_start;
	};
}

#endif
