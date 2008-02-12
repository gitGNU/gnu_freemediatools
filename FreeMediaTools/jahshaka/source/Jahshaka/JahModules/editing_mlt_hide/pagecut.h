/*******************************************************************************
**
** The header file for the Jahshaka/MLT editing module cut page
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef JAHNLE_PAGE_CUT_H
#define JAHNLE_PAGE_CUT_H

#include "page.h"

class GLEdit;

namespace Mlt
{
	class Producer;
	class Properties;
	class Filter;
	class Geometry;
}

namespace jahnle
{
	class Table;

	class PageCut : public Page
	{
		Q_OBJECT

		public:
			PageCut( QWidget *parent, GLEdit *edit );
			~PageCut( );
			void restart( );
			void refresh( );
			void setPosition( int );
			void processKeyEvent( QKeyEvent * );
			bool dropAsset( assetData & );
			bool acceptsSelection( );
			void startSelection( double, double );
			void moveSelection( double, double );
			void setSelection( double, double );
			void consumerFrameShow( Mlt::Frame & );

		protected:
			void updateSettings( Mlt::ClipInfo * );
			void showProperties( Mlt::Properties * );
			const char *getTitle( Mlt::Properties * );
			void setInPoint( );
			void setOutPoint( );

			void colourStartSelection( double, double );
			void colourMoveSelection( double, double );
			void colourSetSelection( double, double );
			void colourConsumerFrameShow( Mlt::Frame & );

			void showSelection( bool );
			QString getGeometry( double, double );
			void geometryStartSelection( double, double );
			void geometryMoveSelection( double, double );
			void geometrySetSelection( double, double );

		public slots:
			void sliderValueChanged( int );
			void trackSelected( Qt::ButtonState, MultiTrackTrack * );
			void valueChanged( int, int );
			void enableClicked( MultiTrackTrack * );
			void removeEffect( );

		private:
			MultiTrack *m_multitrack;
			Table *m_table;
			QPushButton *m_remove;
			Mlt::Producer *m_cut;
			Mlt::Properties *m_properties;

			// Colour selection
			bool m_fetch_colour;
			bool m_final_colour;
			double m_sx, m_sy;
			double m_dx, m_dy;

			// Geometry selection
			bool m_fetch_geometry;
			QString m_name_selection;
			Mlt::Geometry *m_geometry;
			Mlt::Filter *m_indicate;
	};
}

#endif
