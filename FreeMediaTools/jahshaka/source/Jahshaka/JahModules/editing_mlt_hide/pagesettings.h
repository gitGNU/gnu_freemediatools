/*******************************************************************************
**
** The header file for the Jahshaka/MLT editing module settings page
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef JAHNLE_PAGE_SETTINGS_H
#define JAHNLE_PAGE_SETTINGS_H

#include "page.h"

class GLEdit;

namespace jahnle
{
	class PageSettings : public Page
	{
		Q_OBJECT

		public:
			PageSettings( QWidget *parent, GLEdit *edit );
			void restart( );
			void refresh( );
			void setPosition( int ) { }

		public slots:
			// Settings related slots
			void setNormalisation( int );
			void setProgressive( int );
			void setRescale( int );
			void setAspect( int );
			void setResolution( int );
			void setAudio( int );
			void setAudioDriver( int );
			void setMode( int );

		private:
			// Save and new
			QButton *m_save_desktop;
			QButton *m_save_project;
			QButton *m_open_project;
			QButton *m_new_project;

			// GUI Objects for settings
			QComboBox *m_normalisation_box;
			QComboBox *m_progressive_box;
			QComboBox *m_rescale_box;
			QComboBox *m_resolution_box;
			QComboBox *m_audio_box;
			QComboBox *m_audio_driver_box;
			QComboBox *m_aspect_box;
			QComboBox *m_mode_box;
	};
}

#endif
