/*******************************************************************************
**
** The header file for the Jahshaka linux mediaobject file
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef MEDIAOBJECTBASE_H
#define MEDIAOBJECTBASE_H

////////////////////////////////////////////////
//generic includes here
#include "mediaobject_includes.h"

// This is the base abstraction for all media reading classes

class mediaObject_ 
{
	public:
		mediaObject_( ) : m_correct_ar( true ) { }
		virtual ~mediaObject_( ) { }

		// All implementations must provide these methods
	  	virtual bool openMedia( QString file ) = 0;
	  	virtual bool closeMedia( void ) = 0;
	  	virtual int getVideoLength() = 0;
	  	virtual bool isMediaOpen() = 0;
	  	virtual QString getFileName() = 0;
	  	virtual int getNumberFrames() = 0;
	  	virtual QImage getVideoFrame( int frame ) = 0;
	  	
	  	// This class would be a pure interface if it weren't for these two
	  	// optional methods
	  	virtual void printinfo(void) { }
	  	virtual void getAudioFrame(int) { }

		bool getCorrectAspectRatio( ) { return m_correct_ar; }
		void setCorrectAspectRatio( bool on ) { m_correct_ar = on; }

	private:
		bool m_correct_ar;
};

#endif


