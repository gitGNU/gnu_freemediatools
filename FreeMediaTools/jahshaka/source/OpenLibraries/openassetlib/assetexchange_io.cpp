/*******************************************************************************
**
** The source file for the Jahshaka assetexchange class
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "assetexchange.h"
#include <qmessagebox.h>
#include "assetexchange.h"
#include "mediaobject.h"
#include "opencore.h"

#ifdef AUDIOSUPPORT
#include "sndfile.h"
#endif


///////////////////////////////////////
//not sure if clipnumber is necessary

assetData assetExchange::loadJahMediaAsset(QString filename,QString shortName,
                                           QString pathString,QString extension, 
                                           int clipnumber, bool &itWorked )
{
    jtrace->debug ("Loading Media File:", filename);

    itWorked = false;

    //necessary variables
    assetData thedata;

    //update desktop clip number
    int clipn = clipnumber;

    QString temp;

    thedata.theCategory = VideoCategory::MEDIA; //was clip
    thedata.theType     = VideoType::MOVIE; // no?

    //figure out what this is and make it dynamic
    thedata.videoext    = "";

    thedata.clipname    = shortName;
    thedata.clipnumber  = clipn;
    thedata.location    = pathString;
    thedata.filename    = shortName;
    thedata.extension   = extension;
    thedata.videotex   = true;

    //thedata.keyStatus   = FALSE;
    thedata.xpos        = 10;
    thedata.ypos        = 10;


    //run as many checks as possible before using the media
    if ( !filename.isEmpty() )
    {
		setCorrectAspectRatio( !extension.lower().endsWith( ".jfx" ) );

        if ( isMediaValid(thedata) )
        {
            itWorked = true;
            
            //number of frame sis based on file info being valid
            int startFrame = 0;
            int maxFrames;
            getMediaLength(thedata,maxFrames);    // The Last Frame Of The Stream

            thedata.startframe  = startFrame;
            int numberFrames    =  startFrame + maxFrames;
            thedata.endframe    = numberFrames; //minus startframe

            QImage im = getImage( thedata, thedata.startframe );
            if ( !im.isNull() )
            {
                thedata.xNativeSize = im.width();
                thedata.yNativeSize = im.height();
            }
        }
    }

    return thedata;

}

assetData assetExchange::loadJahAudioAsset(QString filename,QString shortName,
                                           QString pathString,QString extension, 
                                           int clipnumber, bool &itWorked)
{
    jtrace->debug ("Loading Audio File:", filename);

	itWorked = false;

    //necessary variables
    assetData thedata;

    //update desktop clip number
    int clipn = clipnumber;

    QString atemp;

    thedata.theCategory = VideoCategory::AUDIO;
    thedata.theType     = VideoType::AIFF; // no?

    //figure out what this is and make it dynamic
    thedata.videoext    = "";
    thedata.clipname    = shortName;
    thedata.clipnumber  = clipn;
    thedata.location    = pathString;
    thedata.filename    = shortName;
    thedata.extension   = extension;
    thedata.videotex    = false;

    //thedata.keyStatus   = FALSE;
    thedata.xpos        = 10;
    thedata.ypos        = 10;

    //run as many checks as possible before using the media
    if ( !filename.isEmpty() )
    {
        //if ( !no_writing )
        //{
            //assethack
            assetExchange exchange;
            if (exchange.isMediaValid(thedata))
            {
                itWorked = true;

                //number of frame sis based on file info being valid
                int startFrame = 1;

                int maxFrames;

                //how do we figurew thisout
                //need to add it to the media exchange
                exchange.getMediaLength(thedata,maxFrames);    // The Last Frame Of The Stream
                //thedata.getMediaLength(maxFrames);    // The Last Frame Of The Stream
                maxFrames = 1000;

                thedata.startframe  = startFrame;
                int numberFrames    =  startFrame + maxFrames;
                thedata.endframe    = numberFrames; //minus startframe

                //debug("numberFrames  :" +data1.setNum(numberFrames) );

            }
        //}
    }

    return thedata;

}


assetData assetExchange::loadJahImageAsset(QString filename,QString shortName,
										   QString pathString,QString extension, 
										   int clipnumber, bool & itWorked)
{

    //jtrace->debug ("Loading Still File:", filename);

    itWorked = false;
        
    //necessary variables
    assetData thedata;

    //increase desktop clipnumber
    int clipn = clipnumber;

    QString temp;

    thedata.theCategory = VideoCategory::IMAGE;
    thedata.theType     = VideoType::STILL;

    thedata.clipname    = shortName;    //clipname is truncated filename
    thedata.clipnumber  = clipn;
    thedata.location    = pathString;    //the path without the filename...
    thedata.filename    = shortName;     //the filename on disk without extension
    thedata.extension   = extension; //we need to change this based on extension
    thedata.startframe  = 1;
    thedata.endframe    = 1;
    //thedata.keyStatus   = FALSE;
    thedata.xpos        = 10;
    thedata.ypos        = 10;
    thedata.videotex   = false;

    if ( !filename.isEmpty() )
    {
        //if ( !no_writing ) 
        //{   
        itWorked = true;
        QImage im = getImage( thedata, 0 );
        if ( !im.isNull() )
        {
            thedata.xNativeSize = im.width();
            thedata.yNativeSize = im.height();
        }
        
        //}
    }

    return thedata;


}


assetData assetExchange::loadJahSequenceAsset(QString filename,QString shortName,
                                              QString pathString,QString extension,
                                              QString theextension, 
                                              int startFrame, int numberFrames, 
                                              int clipnumber, bool & itWorked)
{
    //jtrace->debug ("Loading Sequence File:", filename);

    itWorked = false;

    //necessary variables
    assetData thedata;

    //update desktop clip number
    int clipn = clipnumber;

    QString temp;

    thedata.theCategory = VideoCategory::CLIP;
    thedata.theType     = VideoType::SEQUENCE;

    //figure out what this is and make it dynamic
    thedata.videoext    = theextension;

    thedata.clipname    = shortName;
    thedata.clipnumber  = clipn;
    thedata.location    = pathString;
    thedata.filename    = shortName;
    thedata.extension   = extension;
    thedata.startframe  = startFrame;
    thedata.endframe    = numberFrames-1; //minus startframe
    //thedata.keyStatus   = FALSE;
    thedata.xpos        = 10;
    thedata.ypos        = 10;
    thedata.videotex   = true;

    if ( !filename.isEmpty() )
    {

        //if ( !no_writing )
        //{
        itWorked = true;

        //}
    }

    return thedata;

}

///////////////////////////////////////////////////////
// asset loader routines 
// these are all stolen from the desktop from desktopfileio.cpp
// and should be migrated out of the desktop
// and used via this library
//
// by having the desktop routine use this one to get the asset
// and then add it to the desktop when its got it
//we would have cleaner code

/////////////////////////////////////////////////////////////////////////////////
//important routines here used to control supported extensions for the desktop
//CY: Modified from the original single method to the 4 current ones (classifies
//media, audio, image and scene) - not a great solution still...

bool assetExchange::isMediaExtension(QString & string)
{
    bool mpegsupport = false;
    bool avisupport = false;
    bool dvsupport = false;
    bool westleysupport = false;
	bool mplsupport = false;

    #ifdef WIN32
    avisupport = true;
    #endif

    #ifdef JAHMPEGDECODER
    mpegsupport = true;
    #endif

    #ifdef LINUXAVISUPPORT
    avisupport = true;
    #endif

	// This isn't good - varies from mlt installation to installation
	#ifdef MLTSUPPORT
    avisupport = true;
    mpegsupport = true;
    dvsupport = true;
    westleysupport = true;
    mplsupport = true;
	#endif

    //set up variables here
    bool ok = false;

    if (mpegsupport && string.lower().endsWith( ".mpg" )) { ok = true; } else // media formats
    if (mpegsupport && string.lower().endsWith( ".mpeg" )) { ok = true; } else
    if (mpegsupport && string.lower().endsWith( ".mp4" )) { ok = true; } else
    if (avisupport && string.lower().endsWith( ".avi")) { ok = true; } else
    if (dvsupport && string.lower().endsWith( ".dv" )) { ok = true; } else
    if (dvsupport && string.lower().endsWith( ".mov" )) { ok = true; } else
    if (dvsupport && string.lower().endsWith( ".dif" )) { ok = true; } else
    if (westleysupport && string.lower().endsWith( ".westley" )) { ok = true; } else
    if (westleysupport && string.lower().endsWith( ".jfx" )) { ok = true; } else
    if (westleysupport && string.lower().endsWith( ".jef" )) { ok = true; } else
    if (mplsupport && string.lower().endsWith( ".mpl" )) { ok = true; }

    return ok;
}

bool assetExchange::isAudioExtension(QString & string)
{
    bool audiosupport = false;

    #if AUDIOSUPPORT
    audiosupport = true;
    #endif

    #if MLTSUPPORT
    audiosupport = true;
    #endif

    //set up variables here
    bool ok = false;

    if (audiosupport && string.lower().endsWith( ".wav" )) { ok = true; } else //audio formats
    if (audiosupport && string.lower().endsWith( ".aif" )) { ok = true; } else
    if (audiosupport && string.lower().endsWith( ".mp3" )) { ok = true; } else
    if (audiosupport && string.lower().endsWith( ".ogg" )) { ok = true; }

    return ok;
}

bool assetExchange::isImageExtension(QString & string)
{
   //set up variables here
    bool ok = false;

    //check against known supported extensions
    if (string.lower().endsWith( ".bmp" )) { ok = true; } else // image formats
    if (string.lower().endsWith( ".jpg" )) { ok = true; } else
    if (string.lower().endsWith( ".png" )) { ok = true; } else
    if (string.lower().endsWith( ".gif" )) { ok = true; } else
    if (string.lower().endsWith( ".tga" )) { ok = true; } else
    if (string.lower().endsWith( ".sgi" )) { ok = true; } else
    if (string.lower().endsWith( ".rgb" )) { ok = true; } else
    if (string.lower().endsWith( ".ppm" )) { ok = true; } else
    if (string.lower().endsWith( ".exr" )) { ok = true; } else
    if (string.lower().endsWith( ".tif" )) { ok = true; } else
    if (string.lower().endsWith( ".tiff" )) { ok = true; }

    return ok;
}


bool assetExchange::getExtension(QString & string)
{
	int i = string.findRev( "." );

	bool ok = i != -1 && (
			  isMediaExtension(string) ||
    		  isAudioExtension(string) ||
    		  isImageExtension(string) );

	if ( ok )
		string = string.mid( i );

	return ok;
}

bool assetExchange::checkExtension(QString & string)
{
	int i = string.findRev( "." );

	bool ok = i != -1 && (
			  isMediaExtension(string) ||
    		  isAudioExtension(string) ||
    		  isImageExtension(string) );

	return ok;
}


bool 
assetExchange::loadAsset(QString filename, bool, assetData & newasset, int clipn) 
{

    //jtrace->info( ">> Loading Asset:", filename );

    //user didnt enter anything or cancelled
    if (filename=="" || filename==NULL)
        return false;

    //clean filename
    filename=filename.stripWhiteSpace();

    //////////////////////////////////////////////////////////////////
    //figure out the extension here

    bool validextension;
    QString extension(filename); //set to filename

    //get the extension and check to see if its supported
    validextension = getExtension(extension);

	//error if we dont support dis mon
    if (!validextension)
    {
        //QString errortext = "File type or sequence not supported:\n"+filename;
        //errortext +="\n\nLooking for something like 'file.bmp', 'file_0000.BMP', etc.";
        //jtrace->debug(errortext);

        //QMessageBox::information(this, "Jahshaka", errortext);
        return false;
    }


    //////////////////////////////////////////////////////////////////

    //filestring is full name of file
    QString filestring(filename);
    int index = filestring.findRev(extension);
    filestring.truncate(index); //total path without .bmp extension
    //debug("filestring is:"+filestring);

    QString	shortName(filestring);
    int slash = shortName.findRev('/');
    if(slash > 0) shortName.remove(0, slash+1);
    //debug("shortname is:"+shortName);

    QString	pathString(filename);
    int ras = pathString.length() - (shortName.length()+extension.length() );
    pathString.truncate(ras);
    //debug("pathstring is:"+pathString);

	//////////////////////////////////////////////////
	// initialize assetExchange
	//assetExchange * exchange = new assetExchange;
	assetData newAsset;
	bool itWorked = false;

    //////////////////////////////////////////////////
    // at this point, filestring and extension are ok
    //if its a media or audio clip all we need to do is
    //extract the clip name from filestring and send it over to the loader
    //debug("checking extensions");

    if ( isAudioExtension( filename ) )
    {
        jtrace->debug("its a audio file");

		newAsset = loadJahAudioAsset(filename,shortName,pathString,extension,clipn,itWorked);

		if (itWorked)
			newasset=newAsset;
        
		return itWorked;
    }

    if ( isMediaExtension( filename ) )
    {
        jtrace->debug("its a media file");
                
		newAsset = loadJahMediaAsset(filename,shortName,pathString,extension,clipn,itWorked);
		
		if (itWorked)
			newasset = newAsset;

		return itWorked;
    }

	
    //////////////////////////////////////////////////////////////////
    // if its not a recognized media file and its a image file we need
    // to find out if its a still or a sequence

    /////////////////////////////////////////////////
    // prolem we are having is we see any filename that ends in a number
    //as a potential sequence...
    //////////////////////////////////////////////////////////////////


    //debug("Checking for padding for sequences");
    int		maxFrames = 99999; //maximum number of frames allowed in sequences due to current extension filename limitations
    int		i;
    bool	ok;

    //QImage  image;
    //QPixmap buffer;
    QString	stillName(filestring);
    slash = stillName.findRev('/');
    if(slash > 0) stillName.remove(0, slash+1);
    //debug("stillname is:"+stillName);

    QString	baseString(filestring);
    index = filestring.length();

    //////////////////////////////////////////////////
    //figure out how many zeros...
    int extcount=0;
    QString tempstring(filestring);
    QString extchar;

    for (int ii=index; ii>=0; ii--)
    {
        extchar = tempstring.right(1);

        if (isanumber(extchar))
        {
            tempstring.truncate( tempstring.length()-1 );
            extcount++;
            //debug(tempstring);
        }

    }

    index -= extcount;					//remove number of zeros in  sequence
    baseString.truncate(index);			//basestring is now total path without numbers or extension
    filestring.remove(0, index);

    QString theextension (filestring);	//set theextension value, later to be videoext
    //debug("videoext theextension is:"+theextension);
    //debug("baseString is:"+baseString);
    //debug("filestring is:"+filestring);


    jtrace->debug (">>> now we check to see if its a sequence");
    int startFrame = filestring.toInt(&ok); //should this be theextension?

    if (!ok)
    {
        //we know for sure its not a image here
        //as the filename is clean ie no numbers
        //jtrace->debug("its a image");

        //itWorked = loadJahImageAsset(filename,shortName,pathString,extension,loadit);
        
		newAsset = loadJahImageAsset(filename,shortName,pathString,extension,clipn,itWorked);
		
		if (itWorked)
			newasset = newAsset;

		return itWorked;

    }
    else
        jtrace->debug("it could be a sequence");

    QString testnamestring;

    //we now assume the sequence data is correct?
    //QString pad; pad = "%s%04d%s";
    QString pad; pad = "%s%"; pad += "0";
    QString extc; extc.setNum(extcount);
    pad+=extc; pad += "d%s";
    for (i = startFrame; i < (int)startFrame+maxFrames; i++)
    {
        testnamestring.sprintf(pad, (const char*)baseString, i, (const char*)extension);

        //we dont open the file any more just make sure the sequence is there...
        //we can skip this for clips already loaded in the database
        if (!( QFile::exists(testnamestring) ))
        {
            if (i == 0)
            {
                //warning ("File \'%s\' not found. Sequence aborted.", (const char*)testnamestring);
                return false;
            }

            //we found the last frame
            maxFrames = i-startFrame;
            break;
        }
    }

    QString rere;

    int numberFrames =  startFrame + maxFrames;
    //jtrace->debug (">>> numberFrames",numberFrames);
    //jtrace->debug (">>> startFrame",startFrame);
    //debug ("num of frames is"+rere.setNum(numberFrames-startFrame));

    //debug("basestring:"+baseString);
    QString	newstillName(baseString);
    slash = newstillName.findRev('/');
    if(slash > 0) newstillName.remove(0, slash+1);
    //debug("newstillname is:"+newstillName);

	////////////////////////////////////////////////////////////////////////////////
	// problem here is it will happen for every photo!!!
	// we need to fix this, with a flag?

    // We do a final test on the first and second frames. If theres an unusual start 
    // frame then it might not be a sequence, we can then ask user. 
    testnamestring.sprintf(pad, (const char*)baseString, 0, (const char*)extension);
    bool firstok = QFile::exists(testnamestring);
    testnamestring.sprintf(pad, (const char*)baseString, 1, (const char*)extension);
    bool secondok = QFile::exists(testnamestring);
    bool usesequence = true; 
    if ( (numberFrames-startFrame) != 1 && !firstok && !secondok )
    {
        switch( QMessageBox::warning( 0, "Load",
            "Load as single image or sequence?.\n\n",
            "Image",
            "Sequence", 0, 0, 1 ) ) {
        case 0: 
            usesequence = false; 
            break;
        case 1: 
            usesequence = true; 
            break;
        }
    }

    if ((numberFrames-startFrame) == 1 || usesequence == false )
    {
        jtrace->debug("false alarm, its a image with numbers in the name");
        //itWorked = loadJahImageAsset(filename,shortName,pathString,extension,loadit);
        
		newAsset = loadJahImageAsset(filename,shortName,pathString,extension,clipn,itWorked);
		
		if (itWorked)
			newasset = newAsset;

		return itWorked;

    }
    else
    {
        jtrace->debug("its a sequence"); //filestring was
        //itWorked = loadJahSequenceAsset(filename,newstillName,pathString,extension,theextension, startFrame,numberFrames,loadit);
        
		newAsset = loadJahSequenceAsset(filename,newstillName,pathString,extension,theextension, startFrame,numberFrames,clipn,itWorked);
		
		if (itWorked)
			newasset = newAsset;

		return itWorked;
    }

}

bool assetExchange::isanumber(QString & string)
{
    bool ok;

    if (string=="0")	{ ok = true; } else
    if (string=="1")	{ ok = true; } else
    if (string=="2")	{ ok = true; } else
    if (string=="3")	{ ok = true; } else
    if (string=="4")	{ ok = true; } else
    if (string=="5")	{ ok = true; } else
    if (string=="6")	{ ok = true; } else
    if (string=="7")	{ ok = true; } else
    if (string=="8")	{ ok = true; } else
    if (string=="9")	{ ok = true; } else
                        { ok = false;}

    return ok;

}

