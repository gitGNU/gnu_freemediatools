/*******************************************************************************
**
** The header file for the Jahshaka MLT videodata file
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/
#ifndef MEDIADATAMLT_H
#define MEDIADATAMLT_H

////////////////////////////////////////////////
//generic includes here
#include "mediaobject_.h"

// Forward declarations
namespace Mlt
{
	class Producer;
}

class mediaObjectMlt : public mediaObject_ {

	public:
		mediaObjectMlt(void);
		virtual ~mediaObjectMlt(void);
		void    printinfo(void);
		bool    openMedia(QString mediafiled);
		bool    closeMedia(void);
		QImage  getVideoFrame(int frame);
		int     getVideoLength();
		void    getAudioFrame(int frame); //not yet implemented
		bool    isMediaOpen();
		QString getFileName();
		int     getNumberFrames();
 
	private:
		Mlt::Producer *producer;
};

#endif



