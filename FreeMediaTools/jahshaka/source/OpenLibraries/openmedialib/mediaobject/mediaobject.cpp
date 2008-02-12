/*******************************************************************************
**
** The source file for the Jahshaka mediaobject library
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "mediaobject.h"

#ifdef JAHMPEGDECODER  
#include "mediaobject_mpeg.h"
#endif

#ifdef WINAVISUPPORT
#include "mediaobject_winavi.h"
#endif

#ifdef LINUXAVISUPPORT
#include "mediaobject_linavi.h"
#endif

#ifdef MLTSUPPORT
#include "mediaobject_mlt.h"
#endif

#include "qfile.h"

////////////////////////////////////////
// constructors and destructors

mediaData::mediaData(void) 
	: m_correct_ar( true )
{
    mediaopen = false;
	object = NULL;
}

mediaData::~mediaData(void)
{
    if (mediaopen) 
    { 
        object->closeMedia();
        mediaopen = false;
    }

    delete object;
}

////////////////////////////////////////
// main code only uses AVI for now

void  mediaData::printinfo(void)
{
    if ( object != NULL )
        object->printinfo();
}


bool mediaData::openMedia(QString mediafile) 
{
    bool status = false;

    //we need to know here what the filetype is
    //and the os/media type support
    //so we can use the correct libraries
    //debug("mediadata::opening media");

	//would save time to check if file exists...
    QFile f( mediafile );

    if (!f.exists())
        return false;

    bool mpeg = false;
    bool avi = false;

    //now check for known extensions
    //currently we support
    //avi and mpg

    avi  = mediafile.endsWith("avi");

    if (!avi)
        mpeg = mediafile.endsWith("mpg");

    if (!mpeg)
        mpeg = mediafile.endsWith("mpeg");

    //in order to open media we need to create the necessary object first

	#ifdef MLTSUPPORT
		object = new mediaObjectMlt;
	#endif

    #ifdef JAHMPEGDECODER
    if ( object == NULL && mpeg)
        object = new mediaObjectMpeg;
    #endif

    #ifdef WINAVISUPPORT
    if ( object == NULL && avi)
        object = new mediaObjectWinavi;
    #endif

    #ifdef LINUXAVISUPPORT  
    if ( object == NULL && avi )
        object = new mediaObjectLinavi;
    #endif

	if ( object )
        status = object->openMedia(mediafile);

    if (status)
        mediaopen = true;

    //need to poll object that was created to make sure we were successful before
    //returning true here
    return status;
}

bool mediaData::closeMedia(void)
{
    bool status = false;

    if (mediaopen)
    {
        if ( object )
            status = object->closeMedia();
    }

    if (status)
    {
        mediaopen = false;
    }

    return status;
}

QImage mediaData::getVideoFrame(int frame)
{
    if (mediaopen && object)
	{
		object->setCorrectAspectRatio( getCorrectAspectRatio( ) );
        return object->getVideoFrame(frame);
	}

    QImage thedata;
    return thedata;
}

int mediaData::getVideoLength()
{
    if (mediaopen && object)
        return object->getVideoLength();
    
    return 0;
}

void  mediaData::getAudioFrame(int)
{
}

bool  mediaData::isMediaOpen(void)
{
    if (mediaopen && object)
        return object->isMediaOpen();

    return false;
}


QString mediaData::getFileName()
{
    if (mediaopen && object)
        return object->getFileName();

    QString loadfile;
    return loadfile;
}

int  mediaData::getNumberFrames()
{
    if (mediaopen && object)
        return object->getNumberFrames();

    return 0;
}


