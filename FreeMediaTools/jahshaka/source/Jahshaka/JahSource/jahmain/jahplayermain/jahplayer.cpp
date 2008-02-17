/*******************************************************************************
**
** The source file for the Jahshaka Jah module
** The Jahshaka Project
** Copyright (C) 2000-2004 The Jahshaka Project
** Released under the GNU General Public License
**
*******************************************************************************/
/*******************************************************************************
**
** The source file for the Jahshaka Jah module
** The Jahshaka Project
** Copyright (C) 2000-2005 The Jahshaka Project
** Released under the GNU General Public License
**
*******************************************************************************/

#include "jahplayer.h"

#ifndef JAHPREFIX
#define JAHPREFIX ""
#endif

Jahplayer::Jahplayer( QWidget* parent, const char* name, WFlags fl )
    : QMainWindow( parent, name, fl )
{
   
    //////////////////////////////////////////////////////////////
    //set up tracer
    jtrace = JahTrace::getInstance();	//set up tracer
    jtrace->info("Jah Constructor","Initializing FreeMediaTools");

    //////////////////////////////////////////////////////////////
    //check and restore preferences first
    jtrace->info("Checking and Restoring Prerences");

    //set up prefs
    JahPrefs& jprefs = JahPrefs::getInstance();
    
	//this sets up the look and feel
    jtrace->info("Loading Color Scheme");
	setupInterface( this );
    setupMyCustomColors();

    //////////////////////////////////////////////////////////////
    //load in base path preferences on startup
    //if we cant find the base path we cant function
    if (!setBasePath())
    {
        exitError();
    }

    //////////////////////////////////////////////////////////////
    //initialize open libraries
    OpenPrefs& openlibs = OpenPrefs::getInstance();

    //we really just tell the openlibs where the app base path is
	//so we can customiz the missing media image
    openlibs.initializeOpenLib(jprefs.getBasePath());

	//////////////////////////////////////////////////////////////
    //this calls our main Jahshaka constructor

    jtrace->info("Launching Core Application");

    //sets up the interface framework
    //this creates the application in either single or dual head mode
    //find out if we are running in dual head or not
	//setupInterface( this );

    //dualheadOption = checkDualHead();

    //if (dualheadOption)
    //    setupDualHeadInterface();


    //this is the main jahshaka constructor
    jahapp = new JahControl(    this ,
                                header, //was menubox header
                                lefttoolbar, worldspace, righttoolbar,
                                controller, 0, 0, 0);


    //this just checks to see if rememberprefs was set
    if (jahapp->getPrefs())
    {
        restoreGlobalPositionSize();
    }
    else
    {
        //center application in screen on startup
        QRect qRect(QApplication::desktop()->screenGeometry());
        int iXpos=qRect.width()/2-this->width()/2;
        int iYpos=qRect.height()/2-this->height()/2;
        this->move(iXpos,iYpos);
    }

    //build display stats now that all data has been reported to the stats object
    JahStats& jstats = JahStats::getInstance();
    jstats.initalizeStats();

	//this is where we restore the theme and user color settings
    jahapp->restoreStyle();
    
    //make sure we are all done!
    if(qApp->hasPendingEvents())
        qApp->processEvents();
}

////////////////////////////////////////////////////////
//Jahshaka Destructor
//cleans up as we leave and releases memory...
Jahplayer::~Jahplayer() 
{
    jtrace->info( "Jah Destructor","Closing Jahplayer" );

    //we store all preference data before exiting
    storeGlobalPositionSize();

    delete jahapp;

	//finished
    jtrace->info( "Jah Destructor","Closed Jahplayer" );
}
 
bool Jahplayer::event( QEvent *e )
{
	if ( e->type( ) == QEvent::WindowActivate )
		jahapp->JahDesktopMod->activate( );
	return QMainWindow::event( e );
}

//////////////////////////////////////////////////////////
//we always check for the base path first
//makes sure jahshaka is installed correctly
//and the copy protection was not cracked... heh :)

bool Jahplayer::setBasePath(void)
{
    QString    JahBasePath;
    QString    JahMediaPath;

    //set up tracer
    jtrace->debug("Checking for media path");

    //Fetching home directory
    QString newting =  QDir::homeDirPath();
    jtrace->debug("Home directory:"+newting);

    //set up prefs
    JahPrefs& jprefs = JahPrefs::getInstance();

    QString fileinfo;
    bool    foundpath;

    //this hack gets the application exectuable path
    //since QApplication::applicationFilePath() only works on Qt 3.2 and up
    jtrace->debug("QT_VERSION_STR: " QT_VERSION_STR );



	#ifndef __APPLE__
    	QString apppath = qApp->applicationFilePath();// only works on Qt 3.2 and up

    	int i = apppath.length() - apppath.findRev("/");

    	fileinfo = apppath.left(apppath.length()-(i));

    	foundpath = true;
	#else
		///////////////////////////////////
		//a quick osx patch
		fileinfo = newting + "/jahshaka/";
		foundpath= true;
	#endif


    //this is a old method of looking for the path
    //if the new method doesnt work we go with this...
    if (!foundpath)
    {
            jtrace->debug("direct path search failed");
            jtrace->debug("trying alternative path search");

            QDir    dir, temp;
            temp=dir.current();

            QString thepath =  temp.path();
            QString thedir  =  temp.dirName();  
            jtrace->debug("located the path:"+thepath);
            jtrace->debug("located the dir :"+thedir);
            //make sure we are in the right directory
            //since some compilers switch this around

            if (thedir=="jahshaka") 
            { 
                fileinfo = thepath; 
                foundpath = true;
            }
    }

    ///////////////////////////////////////////////////////////////
    // now lets look to see if jahmediapath was stored in the prefs

   	JahMediaPath = QString( jprefs.getMediaPath( ).c_str( ) );

	// Check for the existence of the following directories in the current
	// working directory - this will facilitate execution from the source tree

	QDir media( QDir::currentDirPath( ) + "/media" );
	QDir database( QDir::currentDirPath( ) + "/database" );
	QDir pixmaps( QDir::currentDirPath( ) + "/Pixmaps" );

	// If all 3 exist ..
	if ( media.exists( ) && database.exists( ) && pixmaps.exists( ) )
	{
		// We'll use the dev tree as our base path and override media path
		// only if the current media path is "invalid" (default from jprefs)
		JahBasePath = QDir::currentDirPath( ) + "/";
		if ( JahMediaPath == "invalid" )
			JahMediaPath = QDir::currentDirPath( ) + "/";
	}
	else if ( !strcmp( JAHPREFIX, "" ) )
	{
		// On win32 and apple, JAHPREFIX is not specified
		if ( !fileinfo.endsWith( "/" ) )
			fileinfo.append( "/" );
		JahBasePath = fileinfo;
		if ( JahMediaPath == "invalid" )
			JahMediaPath = fileinfo;
	}
	else
	{
		// Otherwise, we set the base to the installed share directory and
		// if the current media path is invalid
		JahBasePath = JAHPREFIX "/share/jahshaka/";
		if ( JahMediaPath == "invalid" )
		{
			JahMediaPath = QDir::homeDirPath( ) + "/jahstorage/";
			#ifndef JAHWINDOWS
			QString command = QString( "jahuserinstall " ) + QString( JAHPREFIX ) + " " + JahMediaPath;
			system( command );
			#endif
		}
	}

	jtrace->debug( "ANTE: the application base path is " + JahBasePath );
   	jtrace->debug( "ANTE: the media path is " + JahMediaPath );

    /////////////////////////////////////////////////////////////////
    //some error checking here as if the media path 
    //is bad we use the default base path

    QDir mediatester;
    bool foundmedia = mediatester.exists(JahMediaPath+"media") || mediatester.exists(JahMediaPath+"media/");

    if (foundmedia)
    {
        jtrace->debug( "Found media folder, using JahMediaPath" );
    }
    else
    {
        jtrace->debug( "Didnt find media folder, using default media folder" );
        JahMediaPath = JahBasePath;
    }

    //update preferences object with info
    jprefs.setBasePath(JahBasePath.data());
    jprefs.setMediaPath(JahMediaPath.data());

	jtrace->debug( "POST: the application base path is " + JahBasePath );
   	jtrace->debug( "POST: the media path is " + JahMediaPath );

    //this is run if it is the firsttime
    bool prefsfound = jprefs.getFoundPrefs();

    if (!prefsfound)
    {
        jtrace->debug( "Creating preferences for the first time" );
        jprefs.savePreferences();
    }

    return foundpath;
}


//this routine stores all the interface settings
//desktop position,size,color scheme,style
//update settings  we can write bool, int and qstring only

void Jahplayer::storeGlobalPositionSize(void) 
{
    jtrace->debug( "Storing the global preferences" );

    //set up prefs
    JahPrefs& jprefs = JahPrefs::getInstance();

    int width=this->width();
    int height=this->height();
    int Xpos=this->x();
    int Ypos=this->y();

    //////////////////////////////////////////////////
    //these need to be updated on exit...

    jprefs.setGeometryX (Xpos );
    jprefs.setGeometryY (Ypos );
    jprefs.setGeometryWidth (width);
    jprefs.setGeometryHeight (height);

    //lets update the color and theme preferences here
    
    //if we flag the system not to remember prefs
    //we need to reset the prefs...
    if (jprefs.getStyle())
    {
        jprefs.setThemecolor (jahapp->getJahThemeColor());
    }
    else
    {
        jtrace->debug("not remembering style");
        jprefs.setThemecolor (-1);
    }

    /////////////////////////////////////////////
    //we need to store the modulesui layout
    jahapp->saveModulesUI();

    //now we can write this stuff out!
    jprefs.savePreferences();
}
 


//this routine only restores ths application position and size
void Jahplayer::restoreGlobalPositionSize(void) 
{   
    //jtrace->debug( "Restoring the global positioning" );

    //the desktop info
    QRect qRect(QApplication::desktop()->screenGeometry());

    //set up prefs
    JahPrefs& jprefs = JahPrefs::getInstance();

    //we need to check these on the fly
    int xPrefs = jprefs.getGeometryX();
    if (xPrefs==0) { xPrefs = qRect.width()/2-this->width()/2; }

    int yPrefs = jprefs.getGeometryY();
    if (yPrefs==0) { yPrefs = qRect.height()/2-this->height()/2; }

    //the defaults here are ok
    int wPrefs = jprefs.getGeometryWidth();
    int hPrefs = jprefs.getGeometryHeight();

    //now we do our resizing...
    this->resize( QSize(wPrefs, hPrefs).expandedTo(minimumSizeHint()) );
    this->move(xPrefs,yPrefs);

}




void Jahplayer::exitError(void)
{
    jtrace->error ("-------------------------------------");
    jtrace->error( "   Incorrect System Configuration");

    //we could not find the default environment here
    //make this a popup window with a timer?
    jtrace->error ("Cannot find the basic jahshaka paths");
    jtrace->error ("Exiting jahshaka");
    jtrace->error ("please email your configuration to");
    jtrace->error ("jahshaka@users.sourceforge.net");
    jtrace->error ("-------------------------------------");
    exit(0);
}



//dont know why we are trapping this here
//we should be trapping it in jahcreate.cpp in the jahapp object
//so for now we just pass it down...

void Jahplayer::keyPressEvent( QKeyEvent *k )
{
    //pass it down for processing
    jahapp->processKeyPress(k);
}

void Jahplayer::keyReleaseEvent( QKeyEvent *k )
{
    //pass it down for processing
    jahapp->processKeyRelease(k);
}

//we can use this as well for more power
//QListViewItem* item = itemAt(event->pos());

void Jahplayer::contextMenuEvent( QContextMenuEvent * )
{

    QPopupMenu* contextMenu = new QPopupMenu( this );
    Q_CHECK_PTR( contextMenu );
    QLabel *caption = new QLabel( "<font color=darkblue><u><b>"
        "Context Menu</b></u></font>", this );
    caption->setAlignment( Qt::AlignCenter );
    contextMenu->insertItem( caption );
    contextMenu->insertItem( "Load",  jahapp->JahDesktopMod, SLOT(slotLoadIconview())  );
    contextMenu->insertItem( "Play...", jahapp, SLOT(changeModePlayer()) );

    
    //QPopupMenu *submenu = new QPopupMenu( this );
    //Q_CHECK_PTR( submenu );
    //submenu->insertItem( "&Print to printer", this, SLOT(printer()) );
    //submenu->insertItem( "Print to &file", this, SLOT(file()) );
    //submenu->insertItem( "Print to fa&x", this, SLOT(fax()) );
    //contextMenu->insertItem( "&Print", submenu );
    

    contextMenu->exec( QCursor::pos() );
    delete contextMenu;
}








/*
#include "jahplayer.h"

///////////////////////////////////////////////////////////////////
// This is the first main class, that calls the startup screen
// and build the main jahshaka desktop widget defined in jahcontrol.cpp
// which sets up the interface and builds all the other modules

Jahplayer::Jahplayer( QWidget *parent, const char *name )
        : QWidget( parent, name )
{

    //////////////////////////////////////////////////////////////
    //set up tracer

    jtrace = JahTrace::getInstance();	//set up tracer
    jtrace->info("Jah Constructor","Initializing Jahshaka");

    //////////////////////////////////////////////////////////////
    //check and restore preferences first
    jtrace->info("Checking and Restoring Prerences");

    //set up prefs
    JahPrefs& jprefs = JahPrefs::getInstance();

    //this sets up the look and feel
    jtrace->info("Loading Color Scheme");

    setupMyCustomColors();

    //////////////////////////////////////////////////////////////
    //load in base path preferences on startup
    //if we cant find the base path we cant function
    if (!setBasePath())
    {
        exitError();
    }

    //////////////////////////////////////////////////////////////
    //initialize open libraries
    OpenPrefs& openlibs = OpenPrefs::getInstance();

    //we really just tell the openlibs where the app base path is
	//so we can customiz the missing media image
    openlibs.initializeOpenLib(jprefs.getBasePath());

    // load targa image support
    QImageIO::defineIOHandler ( "TGA","^TGA[0-9][0-9][a-z]",0, &kimgio_tga_read, &kimgio_tga_write );
    
	#ifndef JAHIRIX
	QImageIO::defineIOHandler ( "RGB","^RGB[0-9][0-9][a-z]",0, &kimgio_rgb_read, &kimgio_rgb_write );
	#endif
	
	QImageIO::defineIOHandler ( "EXR","^EXR[0-9][0-9][a-z]", 0, vmfx_exr_read, 0 );
    
	//////////////////////////////////////////////////////////////
    //this calls our main Jahshaka constructor

    jtrace->info("Launching Core Application");

    //sets up the interface framework
    //this creates the application in either single or dual head mode
    //find out if we are running in dual head or not
    setupInterface();

    //this is the main jahshaka constructor
    jahapp = new JahControl(    this ,
                                menucontroller,
                                mainleftmenu, mainworld, mainrightmenu,
                                maincontroller);
                              //  moduleselection, modulesettings, feedbackbar);
 
    //this just checks to see if rememberprefs was set
    if (jahapp->getPrefs())
    {
        restoreGlobalPositionSize();
    }
    else
    {
        //center application in screen on startup
        QRect qRect(QApplication::desktop()->screenGeometry());
        int iXpos=qRect.width()/2-this->width()/2;
        int iYpos=qRect.height()/2-this->height()/2;
        this->move(iXpos,iYpos);
    }

    //build display stats now that all data has been reported to the stats object
    JahStats& jstats = JahStats::getInstance();
    jstats.initalizeStats();

    //we have to do this 2x for the mac...
    setupMyCustomColors();

	//this is where we restore the theme and user color settings
    jahapp->restoreStyle();
    
    //make sure we are all done!
    if(qApp->hasPendingEvents())
        qApp->processEvents();

}

void Jahplayer::updatev2vid(void)
{

    //timer bug in player wipes vid windows this is a hack
    //jahapp->displayVideo2startupImage();

}

////////////////////////////////////////////////////////
//Jahshaka Destructor
//cleans up as we leave and releases memory...
Jahplayer::~Jahplayer() 
{
    //set up tracer
    //JahTrace& jtrace = JahTrace::getInstance();
    jtrace->info( "Jah Destructor","Closing Jahplayer" );


    //we store all preference data before exiting
    storeGlobalPositionSize();

    delete jahapp;

	//finished
    jtrace->info( "Jah Destructor","Closed Jahshaka" );
}

//////////////////////////////////////////////////////////
//we always check for the base path first
//makes sure jahshaka is installed correctly
//and the copy protection was not cracked... heh :)

bool Jahplayer::setBasePath(void)
{
    QString    JahBasePath;
    QString    JahMediaPath;

    //set up tracer
    //JahTrace& jtrace = JahTrace::getInstance();
    jtrace->debug("Checking for media path");

    //new routine?
    QDir::homeDirPath();
    QString newting =  QDir::homeDirPath();
    jtrace->debug("New routine says app is in:"+newting);

	QString macpath = newting + "/jahshaka/";

    //set up prefs
    JahPrefs& jprefs = JahPrefs::getInstance();

    QString fileinfo;
    bool    foundpath;

    //this hack gets the application exectuable path
    //since QApplication::applicationFilePath() only works on Qt 3.2 and up
    jtrace->debug("QT_VERSION_STR: ");
    jtrace->debug( QT_VERSION_STR );

    #if (QT_VERSION >= 0x030200)  //was 320
    jtrace->debug("Qt is up to date");

	#ifndef __APPLE__

    QString apppath = qApp->applicationFilePath();// only works on Qt 3.2 and up

    int i = apppath.length() - apppath.findRev("/");

    fileinfo = apppath.left(apppath.length()-(i));

    foundpath = true;
	#else

	///////////////////////////////////
	//a quick osx patch
	fileinfo = macpath;
	foundpath= true;
	#endif

#else
    jtrace->debug("Older version of Qt < 3.2");

    QString thearg = qApp->argv()[0];

    QFile file(thearg);
    QFileInfo fi(file);

    fileinfo = fi.dirPath(true);


    //make sure we are in the jahshaka directory
    //this should always be the case
    if ( fileinfo.endsWith("jahshaka") || fileinfo.endsWith("jahshaka/") ) 
    { 
        foundpath = true; 
    }
    else                    
    { 
        foundpath = false; 
    }  

#endif




    //this is a old method of looking for the path
    //if the new method doesnt work we go with this...
    if (!foundpath)
    {
            jtrace->debug("direct path search failed");
            jtrace->debug("trying alternative path search");

            QDir    dir, temp;
            temp=dir.current();

            QString thepath =  temp.path();
            QString thedir  =  temp.dirName();  
            jtrace->debug("located the path:"+thepath);
            jtrace->debug("located the dir :"+thedir);
            //make sure we are in the right directory
            //since some compilers switch this around

            if (thedir=="jahshaka") 
            { 
                fileinfo = thepath; 
                foundpath = true;
            }
    }


    if (foundpath)
    {
        if (!fileinfo.endsWith("/"))
        {
            fileinfo += "/";
        }

        JahBasePath  = fileinfo;
        JahMediaPath = fileinfo;
        jtrace->debug("the application base path is:"+JahBasePath);
        
        if ((JahBasePath=="/usr/bin/") && (QFile::exists("/usr/bin/jahshaka")) )
        {
            jtrace->debug("found in /usr/bin");

            if (QFile::exists("/usr/share/jahshaka/Pixmaps/jahlayer.png"))
            {
                jtrace->debug("found resources in /usr/share/jahshaka");
                JahBasePath = "/usr/share/jahshaka/";
                //foundjah = true;
            }
        }

    }
    else
    {
        jtrace->debug("couldnt find path...");
        jtrace->debug( "the application base path was:",fileinfo );
        jtrace->debug("trying system directories");

        //////////////////////////////////////////////////////
        //if jahshaka is in /usr/bin
        //then jahshaka resources should be in /usr/share/jahshaka
        bool foundjah = false;

        if (QFile::exists("/usr/bin/jahshaka"))
        {
            jtrace->debug("found in /usr/bin");

            if (QFile::exists("/usr/share/jahshaka/Pixmaps/jahlayer.png"))
            {
                jtrace->debug("found resources in /usr/share/jahshaka");
                JahBasePath = "/usr/share/jahshaka/";
                foundjah = true;
            }
        }

        //////////////////////////////////////////////////////////
        //other option is jahshaka in /opt/jahshaka/bin
        //resources in  /opt/jahshaka/share

        if (!foundjah)
        {
            if (QFile::exists("/opt/jahshaka/bin/jahshaka"))
            {
                jtrace->debug("found in /opt/jahshaka/bin");

                if (QFile::exists("/opt/jahshaka/share/Pixmaps/jahlayer.png"))
                {
                    jtrace->debug("found resources in /opt/jahshaka");
                    JahBasePath = "/opt/jahshaka/share/";
                    foundjah = true;
                }
            }
        }



        //lets check against these
        if (foundjah)
        {
            JahMediaPath = JahBasePath;
            jtrace->debug("the application base path is:"+JahBasePath);
            foundpath = true;

        }
        else
        {
            return false;
        }
    }

    ///////////////////////////////////////////////////////////////
    // now lets look to see if jahmediapth was stored in the prefs

    JahMediaPath   = jprefs.getMediaPath().data();

    jtrace->debug( "JahMediaPath is ",JahMediaPath );

    /////////////////////////////////////////////////////////////////
    //some error checking here as if the media path 
    //is bad we use the default base path

    QDir mediatester;
    bool foundmedia = false;;

    if (mediatester.exists(JahMediaPath+"media"))
    {
        foundmedia = true;
    }
    else if (mediatester.exists(JahMediaPath+"media/"))
    {
        foundmedia = true;
    }


    if (foundmedia)
    {
        jtrace->debug( "Found media folder, using JahMediaPath" );
    }
    else
    {
        jtrace->debug( "Didnt find media folder, using default media folder" );
        JahMediaPath = JahBasePath;
    }

    //update preferences object with info
    jprefs.setBasePath(JahBasePath.data());
    jprefs.setMediaPath(JahMediaPath.data());

    //this is run if it is the firsttime
    bool prefsfound = jprefs.getFoundPrefs();

    if (!prefsfound)
    {

        jtrace->debug( "Creating preferences for the first time" );

        jprefs.savePreferences();

    }

    return foundpath;
}

//this routine stores all the interface settings
//desktop position,size,color scheme,style
//update settings  we can write bool, int and qstring only

void Jah::storeGlobalPositionSize(void) 
{
    //set up tracer
    //JahTrace& jtrace = JahTrace::getInstance();
    jtrace->debug( "Storing the global preferences" );

    //set up prefs
    JahPrefs& jprefs = JahPrefs::getInstance();

    int width=this->width();
    int height=this->height();
    int Xpos=this->x();
    int Ypos=this->y();

    //////////////////////////////////////////////////
    //these need to be updated on exit...

    jprefs.setGeometryX (Xpos );
    jprefs.setGeometryY (Ypos );
    jprefs.setGeometryWidth (width);
    jprefs.setGeometryHeight (height);

    //get dualhead option
    bool dualHead = false;

    //lets update the color and theme preferences here
    
    //if we flag the system not to remember prefs
    //we need to reset the prefs...
    if (jprefs.getStyle())
        jprefs.setThemecolor (jahapp->getJahThemeColor());
    else
    {
        jtrace->debug("not remembering style");
        jprefs.setThemecolor (-1);
    }

    //now we can write this stuff out!
    jprefs.savePreferences();
}

/////////////////////////////////////////////////////////////
// this routine is probably no longer needed...
//however it resets the pointers to be 0 for when D2 is not in use

bool Jah::checkDualHead(void) 
{
    //initialize these all to zero
    bool dualHead = false;

    //set up prefs
    JahPrefs& jprefs = JahPrefs::getInstance();
    dualHead = jprefs.getDualhead();

    return dualHead;
}

//this routine only restores ths application position and size
void Jah::restoreGlobalPositionSize(void) 
{
    //set up tracer
    //JahTrace& jtrace = JahTrace::getInstance();    
    jtrace->debug( "Restoring the global positioning" );

    //the desktop info
    QRect qRect(QApplication::desktop()->screenGeometry());

    //set up prefs
    JahPrefs& jprefs = JahPrefs::getInstance();

    //we need to check these on the fly
    int xPrefs = jprefs.getGeometryX();
    if (xPrefs==0) { xPrefs = qRect.width()/2-this->width()/2; }

    int yPrefs = jprefs.getGeometryY();
    if (yPrefs==0) { yPrefs = qRect.height()/2-this->height()/2; }

    //the defaults here are ok
    int wPrefs = jprefs.getGeometryWidth();
    int hPrefs = jprefs.getGeometryHeight();

    //now we do our resizing...
    this->resize( QSize(wPrefs, hPrefs).expandedTo(minimumSizeHint()) );
    this->move(xPrefs,yPrefs);

}



void Jah::exitError(void)
{
    //set up tracer
    //JahTrace& jtrace = JahTrace::getInstance();
    jtrace->error ("-------------------------------------");
    jtrace->error( "   Incorrect System Configuration");

    //we could not find the default environment here
    //make this a popup window with a timer?
    jtrace->error ("Cannot find the basic jahshaka paths");
    jtrace->error ("Exiting jahshaka");
    jtrace->error ("please email your configuration to");
    jtrace->error ("jahshaka@users.sourceforge.net");
    jtrace->error ("-------------------------------------");
    exit(0);
}


///////////////////////////////////////////////////////
//commands to trap spaceball events and
//send them on to the jahapp

void Jah::customEvent(QCustomEvent *event) {

    switch (event->type()) {

        case 1001 : { getSpaceballData(event);   break; }
        case 1002 : { getSpaceballButton(event); break; }

        default:
            { qWarning("Unknown custom event type: %d", event->type()); }
    }
}

#ifdef SPACEBALL
void Jah::getSpaceballData(QCustomEvent *event)
{
    MagellanFloatEvent *s = (MagellanFloatEvent *) event->data();

    jahapp->spaceballmoved(s->MagellanData[ MagellanX ],s->MagellanData[ MagellanY ],
                            s->MagellanData[ MagellanZ ],
                            s->MagellanData[ MagellanA ],s->MagellanData[ MagellanB ],
                            s->MagellanData[ MagellanC ]);
}
#else
void Jah::getSpaceballData(QCustomEvent *) { }
#endif

void Jah::getSpaceballButton(QCustomEvent *event)
{
    int *s = (int *) event->data();
    QString theval; theval = theval.setNum(*s);
    //debug("Spaceball button:"+theval);
}

//dont know why we are trapping this here
//we should be trapping it in jahcreate.cpp in the jahapp object
//so for now we just pass it down...

void Jah::keyPressEvent( QKeyEvent *k )
{
    //set up tracer
    //JahTrace& jtrace = JahTrace::getInstance();
    //jtrace->debug ("pressed key in Jah::keyPressEvent");

    //pass it down for processing
    jahapp->processKeyPress(k);

}

void Jah::keyReleaseEvent( QKeyEvent *k )
{
    //pass it down for processing
    jahapp->processKeyRelease(k);
}

//we can use this as well for more power
//QListViewItem* item = itemAt(event->pos());

void Jah::contextMenuEvent( QContextMenuEvent * )
{

    QPopupMenu* contextMenu = new QPopupMenu( this );
    Q_CHECK_PTR( contextMenu );
    QLabel *caption = new QLabel( "<font color=darkblue><u><b>"
        "Context Menu</b></u></font>", this );
    caption->setAlignment( Qt::AlignCenter );
    contextMenu->insertItem( caption );
    contextMenu->insertItem( "Load",  jahapp, SLOT(slotLoadIconview())  );
    contextMenu->insertItem( "Play...", jahapp, SLOT(changeModePlayer()) );

    
    //QPopupMenu *submenu = new QPopupMenu( this );
    //Q_CHECK_PTR( submenu );
    //submenu->insertItem( "&Print to printer", this, SLOT(printer()) );
    //submenu->insertItem( "Print to &file", this, SLOT(file()) );
    //submenu->insertItem( "Print to fa&x", this, SLOT(fax()) );
    //contextMenu->insertItem( "&Print", submenu );
    

    contextMenu->exec( QCursor::pos() );
    delete contextMenu;
}

*/ 
