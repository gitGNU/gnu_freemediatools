/*******************************************************************************
**
** The source file for the Jahshaka paint module
** The Jahshaka Project
** Copyright (C) 2000-2005 The Jahshaka Project
** Released under the GNU General Public License
**
*******************************************************************************/


#include "mediaobject_mpeg.h"

////////////////////////////////////////
// constructors and destructors

mediaObjectMpeg::mediaObjectMpeg(void)
{
  //    jtrace = OL_Trace::getInstance();	//set up tracer
	
	mediaopen = false;
    new_image_ptr = NULL;
}

mediaObjectMpeg::~mediaObjectMpeg(void)
{
    if (mediaopen)
    {
        closeMedia();
    }
}

////////////////////////////////////////
// main code only uses AVI for now

void  mediaObjectMpeg::printinfo(void)
{
}

bool  mediaObjectMpeg::isMediaOpen(void)
{
    return mediaopen;
}

bool mediaObjectMpeg::openMedia(QString szFile)
{
    bool status = false;

    if (mediaopen)
    {
        closeMedia();
    }

    char              				szFile_str[500];
    //int								frame_number;

    strcpy(szFile_str, szFile);
    debug(szFile_str);
    strcpy(mpeg_data.filename, szFile_str);
    mpeg_data.purpose = MPEGDEC_DECODE_ONE_FRAME;
    mpeg_data.frame_number = 1;
    mpegdecmain();

    image_width = mpeg_data.horizontal_size;
    image_height = mpeg_data.vertical_size;
    image_depth = 32;

    if (!new_image_ptr)
    {
        new_image_ptr = new QImage(mpeg_data.pixel_buffer, image_width,
        image_height, 32, 0, 0, QImage::IgnoreEndian);
    }

    mediaopen = true;
    status = true;

    return status;

}

int mediaObjectMpeg::getVideoLength()
{
    mpeg_data.purpose = MPEGDEC_COUNT_FRAMES;
    mpeg_data.frame_number = 0;
    mpegdecmain();

/*    if (mediaopen)
     {
        //should check if stream is open...
        int maxFrames=AVIStreamLength(pavi);							// The Last Frame Of The Stream
        return maxFrames;
     }
*/
//    debug("Mpeg length = " + mpeg_data.number_of_frames);

    return mpeg_data.number_of_frames;

}

bool mediaObjectMpeg::closeMedia(void)
{
    bool status = false;

    mpeg_data.purpose = MPEGDEC_CLOSE;
    mpeg_data.frame_number = 0;
    mpegdecmain();

    mediaopen = false; 
	status = true;

    delete new_image_ptr;
    new_image_ptr = NULL;

/*
    if (mediaopen) {
        avistream->StopStreaming(); debug(">>AVI stopped stream");
        mediaopen = false; status = true;
    }
*/
    return status;

}


QImage mediaObjectMpeg::getVideoFrame(int frame)
{
    QImage thedata;

    mpeg_data.frame_number = frame;
    mpeg_data.purpose = MPEGDEC_STREAM_ONE_FRAME;
    mpeg_data.frame_number = frame;
    mpegdecmain();

    thedata = *new_image_ptr;
    return thedata;

    //this is where we grab a blank image for display
    //since nothing else worked...
    if ( !thedata.load( JahBasePath+"Pixmaps/missingmedia.png" ) )
    {	// Load first image from file
        qWarning( "Could not read default image, using single-color instead." );
        QImage dummy( 256, 256, 32 );
        dummy.fill( Qt::blue.rgb() );
        thedata = dummy;
        //returnval = FALSE;
    }

    return thedata;

}

void  mediaObjectMpeg::getAudioFrame(int)
{
}

QString mediaObjectMpeg::getFileName()
{
        QString loadfile;
        return loadfile;
}

int  mediaObjectMpeg::getNumberFrames()
{
    mpeg_data.purpose = MPEGDEC_COUNT_FRAMES;
    mpeg_data.frame_number = 0;
    mpegdecmain();

return mpeg_data.number_of_frames;
}


