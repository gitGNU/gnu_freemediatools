/*******************************************************************************
**
** The header file for the Jahshaka glworld module 
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/


#include <qmessagebox.h>
#include "glworld.h"
#include "glworldlists.h"
#include "glcore.h"
#include <timelineSlider.h>
#include <qtimer.h>
#include <jahpreferences.h>
#include <qdir.h>
#include <qfile.h>
#include <openmedialib.h>

// Special case for the jahplayer
#ifdef JAHPLAYER
#undef MLTSUPPORT
#endif

#ifdef MLTSUPPORT
#include <render.h>
#endif

void GLWorld::checkRenderQuality() 
{
    if (renderquality < RENDER_QUALITY_JPEG) 
    { 
        renderquality = RENDER_QUALITY_PNG; 
    }

#ifndef MLTSUPPORT
    if (renderquality > RENDER_QUALITY_BMP) 
    { 
        renderquality = RENDER_QUALITY_PNG; 
    }
#endif

    switch (renderquality) {

    case RENDER_QUALITY_JPEG  : { renderext=".jpg"; renderformat="JPEG"; break; }
    case RENDER_QUALITY_PNG  : { renderext=".png"; renderformat="PNG";  break; }
    case RENDER_QUALITY_BMP  : { renderext=".bmp"; renderformat="BMP";  break; }
 
    default : { renderext=".png"; renderformat="PNG";  break; }

    }
}

void GLWorld::checkJahRenderer()
{
    //initalize the global vars from the prefs
    JahPrefs& jprefs = JahPrefs::getInstance();

    JahRenderer  = jprefs.getRenderer();

    if (JahRenderer<0)
    {
        JahRenderer = 0;
    }

    if (JahRenderer>3)
    {
        JahRenderer = 0;
    }
}

///////////////////////////////////////////////
// default is bmp

void GLWorld::setRenderQuality(int quality) 
{
    if (  quality < RENDER_QUALITY_JPEG)
    {
        renderquality = RENDER_QUALITY_PNG;
    }
    else
    {
        renderquality = quality;
    }

#ifndef MLTSUPPORT
    if (renderquality > RENDER_QUALITY_BMP) 
    { 
        renderquality = RENDER_QUALITY_PNG; 
    }
#endif
}

///////////////////////////////////////////////////
// used for render size overrides in effect module

void GLWorld::SetVideoRender(bool status)
{
	jtrace->debug("video render status was:",m_render_at_image_resolution);

	m_render_at_image_resolution = status;
    core->useimagesize = m_render_at_image_resolution;

	jtrace->debug("video render status is:",m_render_at_image_resolution);
}

//this one takes a single frame photo of where we are
void GLWorld::Render(void)
{
    Globals::setRendering( !Globals::getRendering() );

	resetZoom( );

    assetData thedata;

    checkRenderQuality();

    checkJahRenderer();

    QString moduleinfo = ModuleName+"Image";
    QString temp;

    thedata.theCategory = VideoCategory::IMAGE;
    thedata.theType     = VideoType::STILL;

    thedata.clipname    = moduleinfo;

    int clipnum = *clipnumber;  
    clipnum++;

    thedata.clipnumber  = clipnum;   //needs to be globalclipnumber!

    //set the render path here
    QString jahrenderpath = renderpath;

    thedata.location    = jahrenderpath+"images/";
    thedata.filename    = moduleinfo + temp.setNum(clipnum);
    thedata.extension   = renderext;
    thedata.startframe  = 1;
    thedata.endframe    = 1;
    thedata.xsize       = 180;
    thedata.ysize       = 120;

    setSceneToRender(true);

    QImage shot = Photo(thedata);

    setSceneToRender(false);

    emit updateDesktop(  thedata );

    *clipnumber = clipnum;

	changeZoom( 1 );

    Globals::setRendering( !Globals::getRendering() );

}



void GLWorld::RenderScene(void)
{
    Globals::setRendering( !Globals::getRendering() );

    checkRenderQuality();

    checkJahRenderer();

    //we override rendering of scenes for paint until that module is fixed
    if ( Globals::getRendering() ) 
    {

        resetZoom( );
        timer->stop(  );

        int startFrame = controllerslider->value();
        QString temp;
        QString moduleinfo = ModuleName + "Clip";
        assetData thedata;

        int clipnum = *clipnumber;
        clipnum++;

        //set the render path here
        QString jahrenderpath = renderpath;

        //set up the clip's data
        thedata.theCategory = VideoCategory::CLIP;
        thedata.theType     = VideoType::SEQUENCE;
        thedata.clipname    = moduleinfo;      //need to know this
        thedata.clipnumber  = clipnum;

        QString destDir;
        destDir             = jahrenderpath + moduleinfo + temp.setNum(clipnum);

        jtrace->debug("rendering to:"+destDir);

        //cleans folders up and returns new location
        thedata.location = checkandcleandir(destDir, clipnum);

        thedata.filename    = moduleinfo + temp.setNum(clipnum) + "_";
        thedata.extension   = renderext;
        thedata.startframe  = 1;
        thedata.endframe    = maxFrame - startFrame;   //is default length of animation
        thedata.videotex   = true;
        thedata.xsize      = 180;
        thedata.ysize      = 120;

        //this is where we figure out the video extension for the numbering sequence
        QString theblankframe;  theblankframe.setNum(maxFrame);
        thedata.blank_length = theblankframe.length();

        //this creates the blankstring for videoext
        QString theblank;

        for (int i = 1; i < thedata.blank_length; i++) 
        {
            theblank += "0";
        }

        thedata.videoext = theblank + "1";

        setSceneToRender(true);

        QImage shot = RenderAll(thedata,startFrame,maxFrame);

        setSceneToRender(false);

		if ( shot.width( ) != 0 )
		{
        	//did it end where it was supposed to or was it cancelled early
        	int endFrame = controllerslider->value();

            if ( !Globals::getRendering() )
        	{
            	thedata.endframe = endFrame - startFrame;
        	}

        	//add clip to the desktop now if not using MLT rendering
            if (renderquality <= RENDER_QUALITY_BMP) 
            {
        	    emit updateDesktop(  thedata );
            }

        	//and then incremnent global clipnumber
        	*clipnumber = clipnum;
		}

		changeZoom( 1 );
    }
}


void GLWorld::setSceneToRender(bool renStatus)
{
    //if set to true get ready to render

	//not shure why the getActiveJahLayer() call is in there
	//but when it is the engine doesnt turn off the active layers selection box

	//to record the titlesafe status
    static int titleStatus;

    if (renStatus)
    {   
		//need to turn of titlesafe
		titleStatus = textsafe->layerStatus;

		if (titleStatus) 
		{  
			textsafe->layerStatus = false;  
		}

        if ( ModuleName != "Paint" /*&& getActiveJahLayer() */)
        {
			//deselect the active layer

			//if its a light or effect need to address parent
			if (getActiveJahLayer())
				getActiveJahLayer()->selected=FALSE;
        }
    }
    else
    {
		//restore from the static variable
		textsafe->layerStatus = titleStatus;

        if ( ModuleName != "Paint" /*&& getActiveJahLayer()*/ )
        {
			//reselect the activelayer
			if (getActiveJahLayer())
				getActiveJahLayer()->selected=TRUE;
        }
    }

    updateGL();
}

//////////////////////////////////////////////////////////////////
//clean up diretories

QString GLWorld::checkandcleandir(QString destDir, int clipnum)
{
    QString newlocation;
    QString jahrenderpath = renderpath;
    QString moduleinfo = ModuleName+"Clip";


    //here we make the dir if it doesnt exist
    QDir d( destDir );                        // "./example"

    if (d.exists() )
    {
        jtrace->debug("directory exists we are cleaning up:"+destDir);

        const QFileInfoList *list = d.entryInfoList();
        QFileInfoListIterator it( *list );
        QFileInfo *fi;

        QString thaname,delname;

        while ( (fi = it.current()) != 0 )
        {
            thaname = fi->fileName().latin1() ;
            delname = destDir+"/"+thaname;

            QFile f( delname );

            if (f.exists())
            {
                f.remove();
            }
            ++it;
        }
    }

    QString temp;

    if ( !d.exists() )
    {
        jtrace->debug( "Cannot find existing render directory - good" );
        if (d.mkdir( destDir ))
        {
            jtrace->debug("created directory",destDir );
            //thedata.location    = jahrenderpath+moduleinfo + temp.setNum(clipnum) + "/";
            newlocation    = jahrenderpath+moduleinfo + temp.setNum(clipnum) + "/";
        }
        else
        {
            jtrace->error("couldnt create directory - filesystem error",destDir );
            newlocation    = jahrenderpath;
        }
    }
    else
    {
        //the render folder already existed amd we couldnt erase it
        //what do we do?
        //just write into it...
        newlocation    = jahrenderpath+moduleinfo + temp.setNum(clipnum) + "/";

    }

    return newlocation;

}

/////////////////////////////////////////////////////////////////////////////
//the soft photo is identical to the one below
// but it returns a image instead of saving the data out to disk
// and always does full screen shots

void GLWorld::SoftPhoto(QImage &thephoto)
{
	int thewidth, theheight;
	getRenderResolution(thewidth,theheight);

	if (m_render_at_image_resolution )
    {
        thephoto = screenShot(m_currentFrame, thewidth, thewidth, theheight, theheight, 
                            core->imagewidth, core->imageheight, 1 );
    }
    else 
    //////////////////////////////////////////////////////////
    // use the default render mode   
    {
        thephoto = screenShot(m_currentFrame, thewidth, thewidth, theheight, theheight, 
                            X_RESOLUTION, Y_RESOLUTION, 1 );
    }

}

QImage GLWorld::Photo(assetData theclip) 
{
    QString writefile = theclip.location+theclip.filename+theclip.extension;

    QImage thephoto;

	int thewidth, theheight;
	getRenderResolution(thewidth,theheight);

	if (m_render_at_image_resolution )
    {

        thephoto = screenShot(m_currentFrame, thewidth, thewidth, theheight, theheight, 
                            core->imagewidth, core->imageheight, 1 );
    }
    else 
    //////////////////////////////////////////////////////////
    // use the default render mode   
    {
        thephoto = screenShot(m_currentFrame, thewidth, thewidth, theheight, theheight, 
                            X_RESOLUTION, Y_RESOLUTION, 1 );
    }

    QImageIO iio; iio.setImage(thephoto);
    iio.setFileName(writefile);
    iio.setFormat(renderformat);
    iio.write();

    return thephoto; // send straight to desktop when controls are local
}

///////////////////////////////////////////////////////////////////////
//function to figure out the width and height to take the screenshot
void GLWorld::getRenderResolution(int &thewidth, int& theheight) 
{
  
	if (m_render_at_image_resolution )
    {
        ///////////////////////////////////////////////////////////////////
        //make sure we do this only 1 time for renders... and optimize it
        ////////////////////////////////////////////////////////////////////
        
        //this gives us onscreen 720x486 cords...
        theheight = ( core->getRenderHeight() / 2)-243;   //700/2   - y = 107
        thewidth  = ( core->getRenderWidth() / 2)-360;    //1210/2  - x = 245

        if (core->imagewidth>720) 
        {
            thewidth  = thewidth  -(((core->imagewidth)-720) /2);
        } 
        else if (core->imagewidth<720) 
        {
            thewidth  = thewidth  +((720-(core->imagewidth)) /2);
        }
        else {}

        if (core->imageheight>486) 
        {
            theheight = theheight -(((core->imageheight)-486)/2);
        } 
        else if (core->imageheight<486)
        {
            theheight = theheight +((486-(core->imageheight))/2);
        }

    }
    // use the default render mode for all other modules
    else 

    //////////////////////////////////////////////////////////
    // use the default render mode   
    {
        //////////////////////////////////////////////////////////////////
        //all our math used to be based on a base of 720x486 so we haev to clean this all up

        //m_render_width and m_render_height are the size of the opengl window
        theheight = ( core->getRenderHeight() / 2)-243;   //700/2   - y = 107
        thewidth  = ( core->getRenderWidth() / 2)-360;    //1210/2  - x = 245

        //now we have image cords... apply offests
        //imagewidth and imageheight are the size of the image


        if (X_RESOLUTION>720) 
        {
            thewidth  = thewidth  -(((X_RESOLUTION)-720) /2);
        } 
        else if (X_RESOLUTION<720) 
        {
            thewidth  = thewidth  +((720-(X_RESOLUTION)) /2);
        }
        else {}

        if (Y_RESOLUTION>486) 
        {
            theheight = theheight -(((Y_RESOLUTION)-486)/2);
        } 
        else if (Y_RESOLUTION<486)
        {
            theheight = theheight +((486-(Y_RESOLUTION))/2);
        }


    }

}



QString GLWorld::buildFrameName(int frameval, assetData theclip ) 
{
    QString writefile,extension,filename;
    QString currentframe,theblank;
    int length, blanklength; 

    std::string location_string;
    std::string filename_string;

    if ( theclip.location.length() )
    {
        location_string = theclip.location.data();
    }

    if ( theclip.filename.length() )
    {
        filename_string = theclip.filename.data();
    }

    //set the filename
    filename = theclip.location + theclip.filename;

    //figure out length of current frame
    currentframe.setNum(frameval);
    length = currentframe.length();  //was theblankframe

    //then remove it from the blank template for the clip
    QString blank(theclip.videoext);
    blank = blank.remove(0, length);

    //now we need to build the blank from scratch to account 
    //for sequences begining with 0001 etc
    blanklength = blank.length();

    for (int ii = 0; ii < blanklength; ii++) 
    {
        theblank += "0";
    }

    blank = theblank + currentframe;

    writefile = filename + blank + theclip.extension;

    return writefile;
}

QImage GLWorld::RenderAll(assetData theclip, int startframe, int endframe ) 
{
  jtrace->debug( "Rendering Scene");
    
    //////////////////////////////////////////
    QImage returnshot;
    QImage thephoto;

    QString writefile;

    int frameval;

	// We can only use MLT if it's available, but we will also allow old style
	// render seqences
	bool use_mlt = false;

#ifdef MLTSUPPORT
	// Construct the render object
	jahnle::Render render;
	Mlt::Properties mlt_properties;

    
    //the editor will always default to mlt rendering
	if (renderquality == RENDER_QUALITY_MLT_SELECT)
	{
		use_mlt = QMessageBox::question( this, "Render type", "Do you want a desktop image\nsequence or a video export?", 
			  	"&Sequence", "&Export", QString::null, 0, 1 );

		// Get the user to select the profile
		if ( use_mlt && !render.fetchProfile( JahBasePath, Y_RESOLUTION == 576, PIXEL_RATIO ) )
			return thephoto;
	} 
	else if (
                renderquality == RENDER_QUALITY_MLT_FLASH_2000_KBS_320X240
             || renderquality == RENDER_QUALITY_MLT_FLASH_2000_KBS_640X480
             || renderquality == RENDER_QUALITY_MLT_FLASH_VCD_1000_KBS
             || renderquality == RENDER_QUALITY_MLT_FLASH_VCD_500_KBS
             || renderquality == RENDER_QUALITY_MLT_FLASH_VCD_200_KBS
             )
	{
		QStringList qstringlist;
		QString path = JahBasePath + "plugins/encoders/"; 

		render.createFormatsList(mlt_properties, qstringlist, path); 
		mlt_properties.set("format", "FFmpeg Flash");
		qstringlist.clear();
		render.createProfilesList(mlt_properties, qstringlist);

        std::string profile_string = 
            (renderquality == RENDER_QUALITY_MLT_FLASH_2000_KBS_320X240) ? "2000kbs 320x240"
            : (renderquality == RENDER_QUALITY_MLT_FLASH_2000_KBS_640X480) ? "2000kbs 640x480"
            : (renderquality == RENDER_QUALITY_MLT_FLASH_VCD_1000_KBS) ? "VCD 1000kbs"
            : (renderquality == RENDER_QUALITY_MLT_FLASH_VCD_500_KBS) ? "VCD 500kbs"
            : (renderquality == RENDER_QUALITY_MLT_FLASH_VCD_200_KBS) ? "VCD 200kbs"
            : "2000kbs 320x240";

        mlt_properties.set("profile", profile_string.data() );
		mlt_properties.set("file_name", getRenderOutputFileNameString().data() );

		render.executeProfile( mlt_properties, Y_RESOLUTION == 576, PIXEL_RATIO );
		use_mlt = true;
	}

#endif

	// Sort of fixes low resolution (where the image doesn't fit)
	// Still broken for large image resolutions though

	//the real solution here is to render to a pbuffer and grab the image from out of the graphics cards
	//memory at the correct resolution
	//vs being locked into whats visable to the huan eye

	//if (renderres > visableres)

	bool hideTheUI = false;

	if ( (core->width() < X_RESOLUTION) || ( core->height() < Y_RESOLUTION) )
	{
		hideTheUI = true;
		emit hideUI( true );
	}

	// Make sure it happens before we go into the loop
	qApp->processEvents( );

    ////////////////////////////////////////////////////
    //supposed to stop the timer so renders are FAST!
    frameval    = 0;
    Globals::setRendering(true);
    timer->stop();     

	int thewidth, theheight;
	getRenderResolution(thewidth,theheight);

    theclip.getFrameNameVector().clear();
    theclip.getFrameNameVector().push_back( std::string("PLACEHOLDER") );

    for (int i = startframe; i <= endframe; i++) 
    {
#ifndef JAHPLAYER
        if ( getStopScript() )
        {
            break;
        }
#endif

		updateAnimation(i);
        updateTimeSlider(i);    // used for visual feedback
	    updateGL();

        //this is the clip counter for saved frames
        frameval++;

        //we need to check our interupts here to kill render..
        qApp->processEvents();

        if ( !Globals::getRendering() ) 
        {
            break;
        }

        // This is where we create the extension for the sequence
        writefile = buildFrameName(frameval, theclip ) ;
        std::string writefile_string;

        if ( writefile.data() )
        {
            writefile_string = writefile.data();
        }

        theclip.getFrameNameVector().push_back(writefile_string);

        //////////////////////////////////////////////////////////
        //now we take the photo of the frame (i)

		if (m_render_at_image_resolution )
		{
            thephoto = screenShot(i, thewidth, thewidth, theheight, theheight, 
                                core->imagewidth, core->imageheight, 1 );
        }
        // use the default render mode for all other modules
        else 
        //////////////////////////////////////////////////////////
        // use the default render mode   
        {
            thephoto = screenShot(i, thewidth, thewidth, theheight, theheight, 
                                X_RESOLUTION, Y_RESOLUTION, 1 );
        }

		if ( use_mlt )
		{
			#ifdef MLTSUPPORT
			render.push( thephoto );
    		#endif
		}
		else
		{
        	//finished taking photo now we must write file to disk     
        	QImageIO iio; 
        	iio.setImage(thephoto);
        	iio.setFileName(writefile);
        	iio.setFormat(renderformat);
        	iio.write();

        	if (i == startframe) 
        	{ 
            	returnshot = thephoto; 
        	}
		}
    }

	// Show the UI again
	if (hideTheUI)
    {
		emit hideUI( false );
    }
	
	qApp->processEvents( );

    m_animation = false;
    Globals::setRendering(false);

    timer->start( getRedrawDelay(), TRUE );        //supposed to reactivate the timer
    return returnshot;
}



//there are three different routines that can be used here
//the Qt renderer, a hardrenderer and a soft renderer

//look into using QImage QGLWidget::grabFrameBuffer ( bool withAlpha = FALSE ) [virtual]
//and then cropping for grab with alpha!
//may also be better supported over
//    mymap = QPixmap::grabWindow(this->winId(),xloffset,ytoffset,
//                                xres,yres); //where canvas is this

QImage GLWorld::screenShot(int, int xloffset,int xroffset,int ytoffset,int yboffset,
                           int xres, int yres, int ) 
{
    ///////////////////////////////////////////////////////////////////
    // first update settings - make sure this hasnt already been done 
    // and we are duplicating it

    if (JahRenderer == 0)
    {
        static int                                    previous_image_width = 0;
        static int                                    previous_image_height = 0;

        static unsigned char*                         image_buffer;
        static QImage*                                mymap;

        if (xres == 0 && yres == 0)
        {
            return NULL;
        }

        if (previous_image_width != xres || previous_image_height != yres)
        {
            if (previous_image_width != 0)
            {
                delete image_buffer;
                delete mymap;
            }

            image_buffer = new unsigned char[xres * yres * 4];
            mymap = new QImage(image_buffer, xres, yres, 32, 0, 0, QImage::IgnoreEndian);
			mymap->setAlphaBuffer( true );
        }
        glReadPixels(xloffset, yboffset, xres, yres, GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid*)image_buffer);

        previous_image_width = xres;
        previous_image_height = yres;

        return (*mymap).mirror(false, true);
    }
    else if (JahRenderer == 1)
    {

        ////////////////////////////////////////////////////////////
        QImage img;

        img = core->grabFrameBuffer (FALSE); //false or true flags alpha grabs!!!

        return img.copy(xloffset, ytoffset, xres, yres, FALSE);

        ////////////////////////////////////////////////////////////

    }
    else if (JahRenderer == 2)
    {

        //this routine is a last effort routine if no other render is working
        //it grabs pixels from the viewport and is real, real slow
        //though it could be optimized ie by not destructing the variables as above to be faster
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT,viewport);

        QImage img(viewport[2]-(xloffset+xroffset),(viewport[3]-(ytoffset+yboffset)),32);
        // QImage img(720,486,32);
        GLfloat *r=new GLfloat[viewport[2]];
        GLfloat *g=new GLfloat[viewport[2]];
        GLfloat *b=new GLfloat[viewport[2]];

        for(int y=0;y<(viewport[3]-(ytoffset+yboffset));y++) {
            glReadPixels(viewport[0],y+ytoffset,viewport[2],1,GL_RED,GL_FLOAT,(void *) r);
            glReadPixels(viewport[0],y+ytoffset,viewport[2],1,GL_GREEN,GL_FLOAT,(void *) g);
            glReadPixels(viewport[0],y+ytoffset,viewport[2],1,GL_BLUE,GL_FLOAT,(void *) b);

            for(int x=0;x<(viewport[2]-(xloffset+xroffset));x++) {
                uint *p = (uint *)img.scanLine(viewport[3]-1-y-(ytoffset+yboffset) )+x;
                *p = qRgb((uint) (r[x+xloffset]*255),
                        (uint) (g[x+xloffset]*255),
                        (uint) (b[x+xloffset]*255));
            }
        }

        delete[] r;    delete[] g;    delete[] b;

        return img; 
    }
    else if (JahRenderer == 3)
    {
        ////////////////////////////////////////////////////////////
        //the initial way
        //needed to sync up image

        QImage img;
        QPixmap mymap;  //move this out of the loop

        mymap = QPixmap::grabWindow(core->winId(),xloffset,ytoffset, xres,yres); 

        img = mymap;   //convert QPixmap to a QImage

        return img;

        ////////////////////////////////////////////////////////////
    }

        
        ////////////////////////////////////////////////////////////
        //the older routine now defunct

        QPixmap mymap;  //move this out of the loop
        mymap = QPixmap::grabWindow(core->winId(),xloffset,ytoffset, xres,yres);
        QImage img;
        img= mymap;   //convert QPixmap to a QImage
        return img;
        
    
}

// we are aliasing in hardware so this routine should no longer be called
// but it can be used for heavy software rendering later on
// ie for 200x aliasing 
void GLWorld::renderGL() 
{  

if (polySmooth) 
{   
    int i;

    GLfloat jitter[4][2] = {   { -0.20f,  0.35f },       {  0.20f, -0.35f },
                            { -0.30f, -0.15f },       {  0.30f,  0.15f }   };

    for (i=0; i<4; i++)
        {

        // the world object is 0 we position it here
        getLayerList()->at(0)->thelayer->layernodes->m_node->tx += (jitter[i][0] * 100.5);
        getLayerList()->at(0)->thelayer->layernodes->m_node->ty += (jitter[i][1] * 100.5);

        paintGL();

        getLayerList()->at(0)->thelayer->layernodes->m_node->tx -= (jitter[i][0] * 100.5);
        getLayerList()->at(0)->thelayer->layernodes->m_node->ty -= (jitter[i][1] * 100.5);

        if (i == 0)
        glAccum(GL_LOAD, 0.25);
        else
        glAccum(GL_ACCUM, 0.25);
        }

        glAccum(GL_RETURN, 1.0);
    }

else { paintGL(); }

}

