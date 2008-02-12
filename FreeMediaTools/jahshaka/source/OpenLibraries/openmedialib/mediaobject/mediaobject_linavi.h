/*******************************************************************************
**
** The header file for the Jahshaka linux mediaobject file
** The Jahshaka Project
** Copyright (C) 2000-2004 The Jahshaka Project
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef MEDIADATALIN_H
#define MEDIADATALIN_H

////////////////////////////////////////////////
//generic includes here
#include "mediaobject_.h"

////////////////////////////////////////////////
//object specific includes here

//linux avifile library includes
#include <avifile-0.7/avifile.h>
#include <avifile-0.7/version.h>

#if AVIFILE_MINOR_VERSION > 6
  #include <avifile-0.7/avm_default.h>
#else
  #include <avifile-0.7/default.h>
#endif

#if AVIFILE_VERSION_MINOR > 6
  #define ZERO 0.0f 
  #define ONE  1.0f
#else
  #define ZERO 0.0f
  #define ONE  1.0f
#endif

#include <avifile-0.7/image.h>
#include <avifile-0.7/StreamInfo.h>

#include <fstream>
#include <iostream>
#include <sys/types.h>


class mediaObjectLinavi : public mediaObject_ {

public:

  mediaObjectLinavi(void);
  virtual ~mediaObjectLinavi(void);

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

  ///////////////////////////////////////////
  // linux only stuff here
  
  //linux routine here thats causing the problems
  //maybe we can virtualize this instead using class inhertiance?
  QImage* getQImage(const avm::CImage* im);

  double  stream_fps;             // fps (for playback, since we don't use avi player)
  avm::IReadFile *avifile;      // represents the avi file
  avm::IReadStream *avistream;  // represents the avi stream
  StreamInfo *streaminfo;       // represents the avi streaminfo
  CImage *image;                // an image (provided by avifile)
  uint8_t *ui_image_copy;       // image data (for use with gluScaleImage and glTexImage2D)
  int     aviwidth;            // Video Width
  int     aviheight;           // Video Height
  int     avilastframe;        // Video Height

private:
    QString JahBasePath;

};




#endif


