/*******************************************************************************
**
** The header file for the Jahshaka videodata file
** The Jahshaka Project
** Copyright (C) 2000-2005 The Jahshaka Project
** Released under the GNU General Public License
**
*******************************************************************************/
#ifndef MEDIADATAMPEG_H
#define MEDIADATAMPEG_H

////////////////////////////////////////////////
//generic includes here
#include "mediaobject_.h"

////////////////////////////////////////////////
//object specific includes here

//for the build in mpeg decoder
#include "mpeg2dec.h"

////////////////////////////////////////////
// Qt wonlt let us ifdef a subroutine in a class in the header
// so this is the only workaround for now

//REMOVED class OL_Trace;
  //TODO: this classs is not defined?


class mediaObjectMpeg : public mediaObject_ {

	QImage*								new_image_ptr;


	int									image_width;
	int									image_height;
	int									image_depth;
    
public:

  mediaObjectMpeg(void);
  virtual ~mediaObjectMpeg(void);

  void    printinfo(void);

  bool    openMedia(QString mediafiled);
  bool    closeMedia(void);

  QImage  getVideoFrame(int frame);
  int     getVideoLength();
  
  void    getAudioFrame(int frame); //not yet implemented
  
  bool    isMediaOpen();

  QString getFileName();
  int     getNumberFrames();

  bool    mediaopen;
  
private:
	////////////////////////////////////////
	//pointer to tracer singleton
  ///NOT USED...	OL_Trace			* jtrace;

    QString JahBasePath;

};

#endif



