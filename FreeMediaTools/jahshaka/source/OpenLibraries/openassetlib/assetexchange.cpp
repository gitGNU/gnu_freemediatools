/*******************************************************************************
**
** The source file for the Jahshaka assetexchange class
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/
#ifdef _MSC_VER
#pragma warning (disable : 4786)
#endif

#include "assetexchange.h"
#include "mediaobject.h"
#include "opencore.h"

#ifdef AUDIOSUPPORT
#include "sndfile.h"
#endif

// defines
#define STRINGDATA(X) ( X.data() ? X.data() : "" )  

assetExchange::assetExchange(void) 
	: m_correct_ar( true )
{
    jtrace = OL_Trace::getInstance();	//set up tracer

    streamOpen = false;
	openStream = NULL;

    //initialize JahBasePath
    OpenPrefs& jprefs = OpenPrefs::getInstance();    
    JahBasePath = jprefs.getBasePath().data();

}

assetExchange::~assetExchange(void) 
{
	delete openStream;
}

// This method tidies up some of the less efficient aspects of this class.
//
// All methods which need to open a media file should use this since it only 
// closes and opens when it needs to (rather than openning and closing to 
// fetch individual items like length, width, height and an image)
//
// NB: The same asset exchange can theoretically be used for multiple assets
bool assetExchange::openMedia( assetData &asset )
{
    if ( asset.theType == VideoType::MOVIE )
    {
    	QString loadfilename;
        getMediaFilename(asset, 0, loadfilename);
        
    	if ( !streamOpen )
    	{
            openStream = new mediaData;
    	}
        else if ( streamOpen && theFileName != loadfilename )
        {
            delete openStream;
            openStream = new mediaData;
            streamOpen = false;
        }
        
        if ( !streamOpen )
        {
            streamOpen = openStream->openMedia(loadfilename);
            if ( streamOpen )
                theFileName = loadfilename;
        }
        
		if ( streamOpen )
			openStream->setCorrectAspectRatio( getCorrectAspectRatio( ) );

        return streamOpen;
    }
    
    return false;
}

//////////////////////////////////////////////////////////////
//this routine figures out if the media object is present
//and functionable by testing the first frame

bool assetExchange::isMediaValid(assetData& theasset)
{
    bool returnval = false;
    QString loadfilename;

    //ok get the name of the asset
    returnval = getMediaFilename(theasset, 1, loadfilename);

#ifdef WIN32
#pragma message( "clean me up asap!!!!" )

    ////////////////////////////////////////////
    //hack required for windows Qt since Qt doesnt follow 
    //drive names in a path name you have to manually
    //set the directory

    //we added this routine to keep the : in the drive name on windows
    //QString JahDataIO::ParseWinLocationString(QString data) 

    //set dir for aset to the locationof the asset
    QString assetdir;

    //some error checking here
    if (loadfilename.contains(':', TRUE))
    {
        if (loadfilename.find(':',1))
        {
            assetdir = loadfilename.left(2);
            QDir::setCurrent(assetdir);
            //debug("drive "+assetdir);
        }
    }
#endif

    /////////////////////////////////////////////////
    //if we can get a name thats a good sign...
    if (returnval)
    {
        if ( openMedia( theasset )  )
            returnval = true;
        else if ( theasset.theType != VideoType::MOVIE )
			returnval = QFile::exists(loadfilename);
    }

    return returnval;
}

//figure out how many frames are in a media object...
bool assetExchange::getMediaLength(assetData& theasset, int &frame)
{
    bool returnval = false;

	//if its a movie file
    if ( openMedia( theasset ) )
    {
        frame = openStream->getVideoLength();
    	if (frame < 0) frame = 0;
        returnval = true;
    }
    else
    {
        frame = 0;
    }

    return returnval;
}

std::string assetExchange::resolveFullPath(assetData &ad, int frame)
{
    if (VideoType::SEQUENCE == ad.theType)
    {
        return getsequencefilename(ad, frame).ascii();
    }

    return (ad.location + ad.filename + ad.extension).ascii();
}

//this routine figures out the access filename based
//on the type of asset we are dealing with
bool assetExchange::getMediaFilename(assetData& theasset, int frame, QString &returnName)
{
    QString loadfilename;
    bool itworked = false;

    switch (theasset.theCategory)
    {
        case VideoCategory::IMAGE  :   
        { //for still images
            loadfilename = theasset.location + theasset.filename + theasset.extension;
            itworked = true;
            break;
        }

        case VideoCategory::AUDIO  :   
        { //for audio
            loadfilename = theasset.location + theasset.filename + theasset.extension;
            itworked = true;
            break;
        }

        case VideoCategory::CLIP   :   
        { //for clips
            if (theasset.theType == VideoType::MOVIE) // for avi, mpeg, etc
            {
                loadfilename = theasset.location + theasset.filename + theasset.extension;
                itworked = true;
            }
            else
            {   //clip thats not a movie is a sequence
                loadfilename = getsequencefilename(theasset,frame);
                itworked = true;
            }
            break;
        }

        case VideoCategory::MEDIA   :   
        { //for clips
            if (theasset.theType == VideoType::MOVIE) // for avi, mpeg, etc
            {
                loadfilename = theasset.location + theasset.filename + theasset.extension;
                itworked = true;
            }
            else
            {   //clip thats not a movie is a sequence
                loadfilename = getsequencefilename(theasset,frame);
                itworked = true;
            }
            break;
        }

        case VideoCategory::KEY    :   
        {   //for key layers
            if (theasset.theType == VideoType::SEQUENCE)
            {
                loadfilename = getsequencefilename(theasset,frame);
                itworked = true;
            }
            else
            {
                loadfilename = theasset.location + theasset.filename + theasset.extension;
                itworked = true;
            }

            break;
        }

        default     :   
        { //unknown media type
            jtrace->debug("assetExchange::","getMediaFilename() unknown asset category");
            itworked = true;
            break;
        }

    }

    returnName = loadfilename;
    return itworked;
}

int    assetExchange::getImageWidth(assetData &theasset, int frame)
{
    QImage image;
    image = getImage(theasset, frame);

    int width = image.width();
    return width;

}

int    assetExchange::getImageHeight(assetData &theasset, int frame)
{
    QImage image;
    image = getImage(theasset, frame);


    int height = image.height();
    return height;
}

QImage assetExchange::getImage(assetData& theasset, int frame)
{
    QImage loadimage;
    bool itworked;

    QString loadfilename;

    itworked = getMediaFilename(theasset, frame, loadfilename);

    if (!itworked)
        return NULL;

    jtrace->debug("::::AssetExchange getting image:"+loadfilename);

    //movie is a media file
    if (theasset.theType == VideoType::MOVIE) 
    {   
       	itworked = streamMediaintoimage( loadfilename, loadimage, frame );
    }
    else if (theasset.theCategory == VideoCategory::AUDIO)
    {
      jtrace->debug("loading audio asset");

        if ( !loadimage.load( JahBasePath+"Pixmaps/jahaudio.png" ) ) 
        {	// Load first image from file
            jtrace->error( "Could not read default image, using single-color instead." );
            QImage dummy( 256, 256, 32 );
            dummy.fill( Qt::blue.rgb() );
            loadimage = dummy;
            itworked = FALSE;
        }

    }
    //if its not movie or audio use this as it returns a default if we
    //dont find anything
    else
    {
        // this routine loads the image
        if ( loadFileintoimage( loadfilename, loadimage ) )
        {
            itworked = true;
        }
        else
        {
            itworked = false;
        }
    }

    return loadimage;
};

bool assetExchange::loadFileintoimage( const QString loadfilename, QImage& image)
{

    bool returnval = TRUE;
    bool do_unnecessary_copy = false;

    QImageIO iio; 
    iio.setFileName( loadfilename );

    if ( loadfilename.endsWith( ".tga" ) || loadfilename.endsWith( ".TGA" )) 
    { 
        iio.setFormat("TGA"); 
        do_unnecessary_copy = true;
    }
    else if ( loadfilename.endsWith( ".rgb" ) || loadfilename.endsWith( ".RGB" )) 
    { 
        iio.setFormat("RGB"); 
        do_unnecessary_copy = true;
    }
    else if ( loadfilename.endsWith( ".sgi" ) || loadfilename.endsWith( ".SGI" )) 
    { 
        iio.setFormat("RGB"); 
        do_unnecessary_copy = true;
    }
    else if ( loadfilename.endsWith( ".exr" ) || loadfilename.endsWith( ".EXR" )) 
    { 
        iio.setFormat("EXR"); 
        do_unnecessary_copy = true;
    }
    else if ( loadfilename.endsWith( ".tif" ) || loadfilename.endsWith( ".TIF" )) 
    { 
        iio.setFormat("TIF"); 
        do_unnecessary_copy = true;
    }
    else if ( loadfilename.endsWith( ".tiff" ) || loadfilename.endsWith( ".TIFF" )) 
    { 
        iio.setFormat("TIF"); 
        do_unnecessary_copy = true;
    }

    std::string file_name;

    if ( loadfilename.length() )
    {
         file_name = loadfilename.ascii();
         //printf("assetExchange::loadFileintoimage loaded image %s\n", file_name.data() );
    }

    bool loaded_successfully = false;

    if (do_unnecessary_copy)
    {
        if ( loaded_successfully = iio.read() )
        {
            image = iio.image().copy(); 
        }
    }
    else
    {	
        loaded_successfully = image.load(loadfilename);
    } 
    
    if (!loaded_successfully)
    {
        jtrace->debug( "Unable to load file," + loadfilename + " attempting default image" );

        if ( !image.load( JahBasePath + "Pixmaps/missingmedia.png" ) ) 
        {	// Load first image from file
            jtrace->error( "Could not read default image, using single-color instead." );
            QImage dummy( 256, 256, 32 );
            dummy.fill( Qt::blue.rgb() );
            image = dummy;
            returnval = FALSE;
        }
        else
        {
            // missingmedia.png should really be a 32 bit image, as all the texture code requries 32 bit color
            image = image.convertDepth(32);
        }

    }

    return returnval;
}

QImage* assetExchange::getStreamImagePtr(assetData& theasset, QImage& qimage, int frame)
{
  jtrace->debug("getting stream image in assetExchange::getStreamImage");

    bool itworked;

    QString loadfilename;

    //////////////////////////////////////////////////////////////
    //if its a media or movie file we open a stream
    //if its a image file we dont

	itworked = getMediaFilename(theasset, frame, loadfilename);

    //////////////////////////////////////////////////////////////
    //movie is a media file
    if (theasset.theType == VideoType::MOVIE)
    {
       	itworked = streamMediaintoimage( loadfilename, qimage, frame );
    }
    else if (theasset.theCategory == VideoCategory::AUDIO)
    {
        //hack to load audio image
        //if ( !image.load( JahMediaPath+"/images/sky.bmp" ) ) {	// Load first image from file
        if ( !thecacheimage.load( JahBasePath+"Pixmaps/jahaudio.png" ) ) {	// Load first image from file
            //qWarning( "Could not read default image, using single-color instead." );
            QImage dummy( 256, 256, 32 );
            dummy.fill( Qt::blue.rgb() );
            qimage = dummy;
            itworked = FALSE;
        }
    }
    else
    {
        // this routine loads the image
        qimage.reset();
        if ( loadFileintoimage( loadfilename, qimage ) )
        {
            itworked = true;
        }
        else
        {
            itworked = false;
        }
    }


    return &qimage;
};

QImage assetExchange::getStreamImage(assetData& theasset, int frame, bool glstatus)
{
  jtrace->debug("getting stream image in assetExchange::getStreamImage");

    QImage loadimage;
    bool itworked = false;


    QString loadfilename;

    //////////////////////////////////////////////////////////////
    //if its a media or movie file we open a stream
    //if its a image file we dont

	itworked = getMediaFilename(theasset, frame, loadfilename);

    //////////////////////////////////////////////////////////////
    //movie is a media file
    if (theasset.theType == VideoType::MOVIE)
    {
      jtrace->debug("its a movie getting picture");

        if ( openMedia( theasset ) )
        {
			loadimage = openStream->getVideoFrame(frame);
            itworked = true;
        }
		else if ( !loadimage.load( JahBasePath+"Pixmaps/missingmedia.png" ) )
        {
	  jtrace->error( "Could not read default image, using single-color instead." );
            QImage dummy( 256, 256, 32 );
            dummy.fill( Qt::blue.rgb() );
            loadimage = dummy;
        }
    }
    else if (theasset.theCategory == VideoCategory::AUDIO)
    {
        //hack to load audio image
        //if ( !image.load( JahMediaPath+"/images/sky.bmp" ) ) {	// Load first image from file
        if ( !loadimage.load( JahBasePath+"Pixmaps/jahaudio.png" ) ) {	// Load first image from file
            //qWarning( "Could not read default image, using single-color instead." );
            QImage dummy( 256, 256, 32 );
            dummy.fill( Qt::blue.rgb() );
            loadimage = dummy;
            itworked = FALSE;
        }
    }
    else
    {
        // this routine loads the image
        itworked = loadFileintoimage( loadfilename, loadimage );
    }

    if (glstatus)
        convertToGLFormat( loadimage );

    return loadimage;
};


//maybe add a initializing function so we dont have to send the
//assetData over for every frame?
QImage * assetExchange::getStreamImagePtr(assetData& theasset, int frame, bool glstatus)
{
    bool itworked;
    QString loadfilename;

    //////////////////////////////////////////////////////////////
    //if its a media or movie file we open a stream
    //if its a image file we dont

    if (!streamOpen && theasset.theType == VideoType::MOVIE)
    {
        itworked = openMedia( theasset );
    }
    else
    {
        itworked = getMediaFilename(theasset, frame, loadfilename);
    }

    //////////////////////////////////////////////////////////////
    //movie is a media file
    if (theasset.theType == VideoType::MOVIE)
    {
        itworked = streamMediaintoimage( loadfilename, thecacheimage, frame );
    }
    else if (theasset.theCategory == VideoCategory::AUDIO)
    {
        //hack to load audio image
        //if ( !image.load( JahMediaPath+"/images/sky.bmp" ) ) {	// Load first image from file
        if ( !thecacheimage.load( JahBasePath+"Pixmaps/jahaudio.png" ) ) {	// Load first image from file
            //qWarning( "Could not read default image, using single-color instead." );
            QImage dummy( 256, 256, 32 );
            dummy.fill( Qt::blue.rgb() );
            thecacheimage = dummy;
            itworked = FALSE;
        }

    }
    else
    {
        // this routine loads the image
        if ( loadFileintoimage( loadfilename, thecacheimage ) )
        {
            itworked = true;
        }
        else
        {
            itworked = false;
        }
    }

    //we would need to do this ourselves...
    if (glstatus)
    {
        convertToGLFormat( thecacheimage );
    }

    return &thecacheimage;

};

void assetExchange::convertToGLFormat( QImage& img )
{
    img = img.convertDepth( 32 );
    img = img.mirror();

    if ( QImage::systemByteOrder() == QImage::BigEndian ) {
        // Qt has ARGB; OpenGL wants RGBA
        for ( int i=0; i < img.height(); i++ ) {
            uint *p = (uint*)img.scanLine( i );
            uint *end = p + img.width();
            while ( p < end ) {
                *p = (*p << 8) | ((*p >> 24) & 0xFF);
                p++;
            }
        }
    }
    else 
    {
        // Qt has ARGB; OpenGL wants ABGR (i.e. RGBA backwards)
        img = img.swapRGB();
    }
}

bool assetExchange::streamMediaintoimage( const QString loadfilename, QImage & image, int frame)
{
    bool returnval = TRUE;

    //mediaData * mediaClip = new mediaData;
    if (streamOpen != true)
    {
		if ( openStream == NULL )
			openStream = new mediaData;

		openStream->setCorrectAspectRatio( getCorrectAspectRatio( ) );

		jtrace->debug("stream wasnt open, opening");
        if (openStream->openMedia(loadfilename))  //was filename
        {
			theFileName = loadfilename;
            streamOpen = true;
            image = openStream->getVideoFrame(frame);	
        }
		else if ( !image.load( JahBasePath+"Pixmaps/missingmedia.png" ) )
        {
	  jtrace->error( "Could not read default image, using single-color instead." );
            QImage dummy( 256, 256, 32 );
            dummy.fill( Qt::blue.rgb() );
            image = dummy;
            returnval = FALSE;
        }
    }
    else 
    {   
      jtrace->debug("stream was open");
        image = openStream->getVideoFrame(frame);	
    }

    return returnval;
}



QString assetExchange::getsequencefilename( assetData& theasset, int frame )
{


    //    debug(">>>getting sequence filename");

    //////////////////////////////////////////////
    // we really need to optimize this routine mon
    // as its important and accessed all the time

    //keyFlag false means not the key and true means the key we can fix this later

    //calculate the sequence template for sequences
    //when support is added for avi and movie files need to update LoadLayerAsset as well

    //first we need to map the frame to the associated frame number in the sequence
    frame = (theasset.startframe + frame)-1;

    //make sure we are in range, if we are out of range use the head or tail
    //we can add a flag here to hide clip if we want
    if ( frame> theasset.endframe)         { frame= theasset.endframe; }
    if ( frame< theasset.startframe)       { frame= theasset.startframe; }

    //first figure out length of current frame
    QString currentframe;  currentframe.setNum(frame); //was theblankframe
    int     length = currentframe.length();

    //then remove it from the blank template for the clip
    QString blank(theasset.videoext);

    std::string videoext_string;

    if ( blank.length() )
    {
        videoext_string = blank.data();
    }

    blank          = blank.remove(0, length);

    //now we need to build the blank from scratch to account for sequences begining with 0001 etc
    int blanklength = blank.length();
    QString theblank;

    for (int i = 0; i < blanklength; i++) 
    {
        theblank += "0";
    }

    blank = theblank;
    std::string blank_string;
    
    if ( blank.length() )
    {
        blank_string = blank.data();
    }

    //then create the sequence extension
    QString temp; 
    temp = blank + currentframe;

    //now we set the image name
    QString imagefilepath;

    imagefilepath = theasset.location + theasset.filename + temp + theasset.extension;
    std::string imagefilepath_string = imagefilepath.data();

    //printf( "assetExchange::getsequencefilename created name %s\n", imagefilepath_string.data() );

    return imagefilepath;

}


/////////////////////////////////////////////////////////////////////////
// used to export a asset to a predetermined location in the filesystem

void assetExchange::ExportAsset(assetData &asset, QString destination)
{
  jtrace->debug( "assetExchange::","Calling ExportAsset()");

    VideoType::TYPE LayerType = asset.theType;
    VideoCategory::TYPE LayerCat= asset.theCategory;

    assetExchange exchange;
    QString assetname;

    QDir d( destination );                        // "./example"

    if ( d.exists() )
    {

        if (LayerType == VideoType::STILL )
        {
            //debug("easy copy still");
            exchange.getMediaFilename(asset, 0, assetname);
            //debug("copy from:"+assetname);
            //copy assetname to destination

            QString destinationFile;
            destinationFile = destination + "/" + asset.filename + asset.extension;

            //debug("copy to:"+destinationFile);

            copyFile( assetname, destinationFile );


        }

        if (LayerType == VideoType::MOVIE || LayerCat == VideoCategory::MEDIA )
        { 
            //debug("easy copy still");
            exchange.getMediaFilename(asset, 0, assetname);
            //debug("copy from:"+assetname);
            //copy assetname to destination

            QString destinationFile;
            destinationFile = destination + "/" + asset.filename + asset.extension;

            //debug("copy to:"+destinationFile);

            copyFile( assetname, destinationFile );


        }

        if (LayerType == VideoType::SEQUENCE)  
        {
            QString destDir, location;

            destDir = destination + "/" + asset.clipname;
            std::string destination_directory_string = destDir.data();

            //debug("saving:"+destDir);

            //here we make the dir if it doesnt exist
            QDir d( destDir );                        // "./example"

            if ( !d.exists() )
            {
                //debug( "Cannot find the render directory - good" );
                if (d.mkdir( destDir ))
                {
                    jtrace->debug("created directory",destDir );
                    //location    = jahrenderpath+moduleinfo + temp.setNum(clipnum) + "/";
                }
                else
                {
                    //debug("couldnt create directory using root:"+destDir );
                    destDir             = destination;
                }
            }


            int startframe = asset.startframe;
            int endframe   = asset.endframe;

            for (int x = startframe; x<=endframe; x++)
            {
                exchange.getMediaFilename(asset, x, assetname);

                //cut the file name off the end and use it to copy to
                //need a function to return it alone...

                //debug("copy from:"+assetname);

                QString destinationFile;

                //int i = jahLoadfilename.findRev("/");
                int i = assetname.findRev("/");
                QString newname = assetname.right(assetname.length()-(i+1));
                //debug("assetname:"+newname);


                destinationFile = destDir + "/" + newname;//asset.filename + asset.extension;

                //debug("copy to:"+destinationFile);
                copyFile( assetname, destinationFile );
            }
        }
    }
}


bool assetExchange::copyFile( const QString oldFilePath, const QString newFilePath)
{

    //same file, no need to copy
    if(oldFilePath.compare(newFilePath) == 0)
        return true;

    //load both files
    QFile oldFile(oldFilePath);
    QFile newFile(newFilePath);
    bool openOld = oldFile.open( IO_ReadOnly );
    bool openNew = newFile.open( IO_WriteOnly );

    //if either file fails to open bail
    if(!openOld || !openNew) { return false; }

    //copy contents
    uint BUFFER_SIZE = 16000;
    char* buffer = new char[BUFFER_SIZE];
    while(!oldFile.atEnd())
    {
        Q_ULONG len = oldFile.readBlock( buffer, BUFFER_SIZE );
        newFile.writeBlock( buffer, len );
    }

    //deallocate buffer
    delete[] buffer;
    buffer = NULL;
    return true;


}


void assetExchange::saveXML( std::fstream & ai_fstream ) 
{
    ai_fstream << "<ASSETEXCHANGE\n" ;

    ai_fstream << " JahBasePath = '" << STRINGDATA(JahBasePath) << "'\n" ;  
    ai_fstream << " streamOpen = '" << streamOpen << "'\n" ;  

    ai_fstream << " />" << "\n" ;

    //ai_fstream << " jtrace = '" << jtrace << "'" ;  // OL_Trace * 
    //ai_fstream << " openStream = '" << openStream << "'" ;  // mediaData * 
}
