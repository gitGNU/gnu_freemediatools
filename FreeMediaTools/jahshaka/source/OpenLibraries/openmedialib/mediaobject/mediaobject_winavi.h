/*******************************************************************************
**
** The header file for the Jahshaka MediaObjectWinavi class
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef MEDIADATAWIN_H
#define MEDIADATAWIN_H

////////////////////////////////////////////////
//generic includes here
#include "mediaobject_.h"
#include <opentracer.h>

////////////////////////////////////////////////
//object specific includes here

//for windows media support
#include <windows.h>
#include <vfw.h>

class mediaObjectWinavi : public mediaObject_ {

public:

  mediaObjectWinavi(void);
  virtual ~mediaObjectWinavi(void);

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
  // windows only stuff here
  
  //routines
  //QImage OpenBMP(QString szFile);        // Opens An Windows BMP File (szFile)
  //bool   OpenAVI(QString szFile);          // Opens An AVI File (szFile)
  //QImage GrabAVIFrame(int frame);        // Grabs An AVI Frame
  //void   CloseAVI(void);                  // Closes the avi file
 
  //windows routine here thats causing the problems
  //maybe we can virtualize this instead using class inhertiance?
  QImage bitmapToImage(HDC chdc, HBITMAP chBitmap);
  
  //variables
  AVISTREAMINFO         psi;             // Pointer To A Structure Containing Stream Info
  PAVISTREAM            pavi;            // Handle To An Open Stream
  PGETFRAME             pgf;             // Pointer To A GetFrame Object
  BITMAPINFOHEADER	bmih;            // Header Information For DrawDibDraw Decoding
  long                  avilastframe;    // Last Frame Of The Stream
  int                   aviwidth;        // Video Width
  int                   aviheight;       // Video Height
  char                  *pdata;          // Pointer To Texture Data
  int                   mpf;             // Will Hold Rough Milliseconds Per Frame

  HDRAWDIB              hdd;             // Handle For Our Dib
  HBITMAP               hBitmap;         // Handle To A Device Dependant Bitmap
  HDC                   hdc;             // Creates A Compatible Device Context
  unsigned char*        data;            // Pointer To Our Resized Image

  bool aviopen;                          // keep track of open avi files


private:
	OL_Trace			* jtrace;
	QString JahBasePath;
};


#endif




