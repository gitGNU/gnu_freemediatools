/*******************************************************************************
**
** The source file for the Jahshaka OpenMediaLib library
** The Jahshaka Project
** Copyright (C) 2000-2004 The Jahshaka Project
** Released under the GNU General Public License
**
*******************************************************************************/

#include "mediaobject_linavi.h"

////////////////////////////////////////
// constructors and destructors

mediaObjectLinavi::mediaObjectLinavi(void)
{
    mediaopen = false;
}

mediaObjectLinavi::~mediaObjectLinavi(void)
{
    if (mediaopen)
    {
        closeMedia();
    }
}

////////////////////////////////////////
// main code only uses AVI for now

void  mediaObjectLinavi::printinfo(void)
{
};

bool  mediaObjectLinavi::isMediaOpen(void)
{
    return mediaopen;
};

bool mediaObjectLinavi::openMedia(QString szFile)
{
    bool status = false;

    if (mediaopen)
    {
        closeMedia();
    }

    avifile = avm::CreateReadFile(szFile);

    if (avifile->IsOpened())
    {
        avistream = avifile->GetStream(0, avm::IReadStream::Video );
    }
    else
    {
        debug("avifile error");
        status = false;
    }

    avistream->StartStreaming();
    streaminfo    = avistream->GetStreamInfo();
    aviwidth      = streaminfo->GetVideoWidth();
    aviheight     = streaminfo->GetVideoHeight();
    stream_fps    = streaminfo->GetFps();
    avilastframe  = ( int )( streaminfo->GetLengthTime()*30 );
    //avistream->StopStreaming();

    //QString avibug;
    //debug("Opened the avi file");
    //debug("the avi width  is:"+avibug.setNum(aviwidth));
    //debug("the avi height is:"+avibug.setNum(aviheight));
    //debug("lastframe is"+avibug.setNum(avilastframe));

    //only if it opened
    mediaopen = true;
    status = true;

    return status;

};

int mediaObjectLinavi::getVideoLength()
{
    int maxFrames;

    if (mediaopen)
    {
        maxFrames = ( int )( streaminfo->GetLengthTime()*30 );
        return maxFrames;
    }

    return 0;

};

bool mediaObjectLinavi::closeMedia(void)
{
    bool status = false;

    if (mediaopen)
    {
        avistream->StopStreaming();

        mediaopen = false;
        status = true;
    }

    return status;
};


QImage mediaObjectLinavi::getVideoFrame(int frame)
{
    QImage          thedata;
    static int      previous_frame_number = -1;
    avm::CImage*    pix;
    QString         rere;

    //to prevent buffer overflows
    int maxFrames = ( int )( streaminfo->GetLengthTime()*30 );
    maxFrames -=1;

    if (frame>=maxFrames)
    {
        frame = maxFrames;
    }

    if (mediaopen)
    {
        if (!avistream->IsStreaming())
        {
            avistream->StartStreaming();
        }

        if (frame < previous_frame_number)
        {
            // Synchronization hack.  Something like this is necessary to prevent
            // hangs when the rewind button is pressed.  FIXME
            for (int delay = 0; delay < 200000000; delay++);

            avistream->Seek(frame);
        }

        previous_frame_number = frame;

        //Seek() slows things down
        //should only do when grabbing frames and not when streaming
        //as the GetFrame command automatically increments the frame counter anyways

        //we can keep streaming if we are moving ahead sequentially
        if (frame == previous_frame_number+1)
        {
            //keep on streaming!
            pix = avistream->GetFrame(true);
        }
        else
        {
            //not streamin so Seek to position
            avistream->Seek(frame);
            pix = avistream->GetFrame(true);
        }

        //now we can grab and coinvert the image
        QImage* s32 = getQImage(pix);
        thedata = s32->copy();
        delete s32;
        pix->Release();

        return thedata;
    }

    //this is where we grab a blank image for display
    //since nothing else worked...
    if ( !thedata.load( JahBasePath+"Pixmaps/missingmedia.png" ) )
    {    // Load first image from file
        qWarning( "Could not read default image, using single-color instead." );
        QImage dummy( 256, 256, 32 );
        dummy.fill( Qt::blue.rgb() );
        thedata = dummy;
        //returnval = FALSE;
    }

    return thedata;

};

void  mediaObjectLinavi::getAudioFrame(int frame)
{
    //int max;
    //return max;
};

QString mediaObjectLinavi::getFileName()
{
    QString loadfile;
    return loadfile;
};

int  mediaObjectLinavi::getNumberFrames()
{
    int maxFrames;

    if (mediaopen)
    {
        maxFrames = ( int )( streaminfo->GetLengthTime()*30 );
        maxFrames -=1;
        return maxFrames;
    }

    return false;
};

////////////////////////////////////////////////
// utilities here need to be in separate file ?

//only on linux with avifile...
//really like the use of pointers in this code...
QImage* mediaObjectLinavi::getQImage(const avm::CImage* im)
{
    int w = im->Width();
    int h = im->Height();
    int bitdepth = 32;

    QImage* qi = new QImage( w, h, bitdepth );
    avm::BitmapInfo bi(w, h, bitdepth);

    avm::CImage ci(&bi, qi->scanLine(0), false);
    ci.Convert(im);

    return qi;

}

// Notes
        //debug(">>AVI frame "+rere.setNum(frame));
        //double time = avistream->GetTime();
        //debug(">>AVI time "+rere.setNum(time));
        //double pos = avistream->GetPos();
        //debug(">>AVI frame pos "+rere.setNum(time));
        //try this command as well
        //int maxFrames=AVIStreamLength(pavi);
