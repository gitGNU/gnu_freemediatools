/*******************************************************************************
**
** The header file for the Jahshaka OpenMediaLib library
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/
#ifndef MEDIADATA_H
#define MEDIADATA_H

///////////////////////////////////////////////////////////
// core definitions

//move all of these into a single header called
#include "mediaobject_includes.h"

////////////////////////////////////////////////////////////
// platform specific media support routines
//these are classes to abstract media types from the rest of the application
//allowing cross platform implementations for all media files to be
//created and maintained but allowing us to use our own calls
//defined in this file

//media objects for supported platforms are defined here
//need to make these plugins and load them in dynamically

////////////////////////////////////////////////////////
//The core controlling mediaobject class

class mediaObject_;

#include "mediaobject_.h"
/*
{
 public:
  virtual ~mediaObject_();
  virtual QImage getVideoFrame(int);
  virtual void setCorrectAspectRatio(bool x); // http://www.videolan.org/developers/vlc/doc/doxygen/html/VideoWindow_8h-source.html
  virtual long getVideoLength();
  virtual long getNumberFrames();
  virtual bool isMediaOpen();
  virtual QString getFileName();
  virtual void printinfo();
  virtual bool openMedia(QString file);
  virtual bool closeMedia();
};
*/

class mediaData {
    
public:

	mediaData(void);
	~mediaData(void);

	bool    openMedia(QString mediafile);
	bool    closeMedia(void);

	QImage  getVideoFrame(int frame);
	int     getVideoLength();

	void    getAudioFrame(int frame); //not yet implemented

	bool    isMediaOpen();

	QString getFileName();
	int     getNumberFrames();

	void    printinfo(void);

	bool getCorrectAspectRatio( ) { return m_correct_ar; }
	void setCorrectAspectRatio( bool on ) { m_correct_ar = on; }

private:

    bool    mediaopen;

	QString JahBasePath;

	mediaObject_ *object;
	bool m_correct_ar;
};

#endif



