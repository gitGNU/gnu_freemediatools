/*******************************************************************************
**
** The source file for the FMT_PROGRAM_TITLE Jah module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "jahshaka.h"
#include <desktop.h>
#include <qpopupmenu.h>
#include <qcursor.h>
#include <FreeMediaTools.h>
#ifndef JAHPREFIX
#define JAHPREFIX ""
#endif

Jahshaka::Jahshaka( QWidget* parent, const char* name, WFlags fl )
    : QMainWindow( parent, name, fl ),
      jahapp( NULL )
{
    //////////////////////////////////////////////////////////////
    //set up tracer
    jtrace = JahTrace::getInstance();	//set up tracer
    jtrace->info("Jah Constructor","Initializing " FMT_PROGRAM_TITLE);

    //////////////////////////////////////////////////////////////
    //check and restore preferences first
    jtrace->info("Checking and Restoring Prerences");

    //set up prefs
    JahPrefs& jprefs = JahPrefs::getInstance();
    
    // this sets up the look and feel
    // this also _creates_ all of the widgets that get passed around
    setupInterface( this );
    // setupMyCustomColors();

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

    //this calls the startupscreen where we can add startup options
    //such as projects, username, etc
    jtrace->info("Launching Startup Screen");

    // restore theme here
    JahControl::restoreStyle();

    thestartupscreen = new startup();
    thestartupscreen->setActiveWindow();

    //////////////////////////////////////////////////////////////
    //check the opengl extensions, uses the startup screen opengl space
    checkJahExtensions();


    QImageIO::defineIOHandler ( "TGA","^TGA[0-9][0-9][a-z]",0, &kimgio_tga_read, &kimgio_tga_write );
    
#ifndef JAHIRIX
    QImageIO::defineIOHandler ( "RGB","^RGB[0-9][0-9][a-z]",0, &kimgio_rgb_read, &kimgio_rgb_write );
#endif
    
    QImageIO::defineIOHandler ( "EXR","^EXR[0-9][0-9][a-z]", 0, vmfx_exr_read, 0 );
    QImageIO::defineIOHandler ( "TIF","^TIF[0-9][0-9][a-z]", 0, vmfx_exr_read, 0 );

    //////////////////////////////////////////////////////////////
    //this calls our main Jahshaka constructor

    jtrace->info("Launching Core Application");

    //sets up the interface framework
    //this creates the application in either single or dual head mode
    //find out if we are running in dual head or not
	//setupInterface( this );

    dualheadOption = checkDualHead();

    if (dualheadOption)
        setupDualHeadInterface();


    //this is the main jahshaka constructor
    jahapp = new JahControl( thestartupscreen->glsplash, this ,
                             header,
                             lefttoolbar, worldspace, righttoolbar,
                             controller,
                             modules_buttons, modules_controls, statusbox,
                             D2Widget ,
                             D2mainleftmenu, D2mainworld, D2mainrightmenu,
			     D2maincontroller, D2moduleselection, D2modulesettings, D2feedbackbar ); // EXEC

    if (dualheadOption)
    {

        D2Widget->show();
        D2Widget->repaint();
    }

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

    //////////////////////////////////////////////////////////////
    //close out now that all modules are loaded
    emit sendsetProgressValue(100);
    emit sendchangeModeText("Finished");

    //close out the startup screen
    thestartupscreen->hide();

    //this is where we restore the user color settings
    jahapp->restoreColor();

    //make sure we are all done!
    if(qApp->hasPendingEvents())
        qApp->processEvents();
}

////////////////////////////////////////////////////////
//Jahshaka Destructor
//cleans up as we leave and releases memory...
Jahshaka::~Jahshaka() 
{
    jtrace->info( "Destructor","Closing FreeMediaTools" );

    //we store all preference data before exiting
    storeGlobalPositionSize();

    delete jahapp;
    
    delete( vendor );
    delete( renderer );
    delete( version );
    delete( extensions );
    delete thestartupscreen;
    
	//finished
    jtrace->info( "Destructor","Closed FreeMediaTools" );
}

bool Jahshaka::event( QEvent *e )
{
	if ( e->type( ) == QEvent::WindowActivate )
		jahapp->JahDesktopMod->activate( );
	return QMainWindow::event( e );
}

//////////////////////////////////////////////////////////
//we always check for the base path first
//makes sure jahshaka is installed correctly
//and the copy protection was not cracked... heh :)

bool Jahshaka::setBasePath(void)
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
    bool foundmedia = mediatester.exists(JahMediaPath+"/media") || mediatester.exists(JahMediaPath+"/media/");

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

void Jahshaka::storeGlobalPositionSize(void) 
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

	jprefs.setGeometryMaximized( isMaximized( ) );

    //get dualhead option
    bool dualHead = jprefs.getDualhead();

    if (dualheadOption && dualHead)
    {
        int width2=D2Widget->width();
        int height2=D2Widget->height();
        int Xpos2=D2Widget->x();
        int Ypos2=D2Widget->y();

        jprefs.setGeometryX2 (Xpos2 );
        jprefs.setGeometryY2 (Ypos2 );
        jprefs.setGeometryWidth2 (width2);
        jprefs.setGeometryHeight2 (height2);
		jprefs.setGeometryMaximized2( D2Widget->isMaximized( ) );
    }

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
 
/////////////////////////////////////////////////////////////
// this routine is probably no longer needed...
//however it resets the pointers to be 0 for when D2 is not in use

bool Jahshaka::checkDualHead(void) 
{
    //initialize these all to zero
    D2Widget = 0;
    D2mainleftmenu = 0;
    D2mainworld = 0;
    D2mainrightmenu = 0;
    D2maincontroller = 0;
    D2moduleselection = 0;
    D2modulesettings = 0;
    D2feedbackbar = 0;

    bool dualHead = false;

    //set up prefs
    JahPrefs& jprefs = JahPrefs::getInstance();
    dualHead = jprefs.getDualhead();

    return dualHead;
}

//this routine only restores ths application position and size
void Jahshaka::restoreGlobalPositionSize(void) 
{   
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

	if ( jprefs.getGeometryMaximized( ) )
		this->showMaximized( );

    //ok now for dualhead...
    bool dualHead = jprefs.getDualhead();

    if (dualHead)
    {
        //we need to check these on the fly
        int xPrefs2 = jprefs.getGeometryX2();
        if (xPrefs2==0) { xPrefs2 = qRect.width()/2-D2Widget->width()/2; }

        int yPrefs2 = jprefs.getGeometryY2();
        if (yPrefs2==0) { yPrefs2 = qRect.height()/2-D2Widget->height()/2; }

        //the defaults here are ok
        int wPrefs2 = jprefs.getGeometryWidth2();
        int hPrefs2 = jprefs.getGeometryHeight2();

        //now we do our resizing...
        D2Widget->resize( QSize(wPrefs2, hPrefs2).expandedTo(minimumSizeHint()) );
        D2Widget->move(xPrefs2,yPrefs2);

		if ( jprefs.getGeometryMaximized2( ) )
			D2Widget->showMaximized( );
    }

}




void Jahshaka::exitError(void)
{
    jtrace->error ("-------------------------------------");
    jtrace->error( "   Incorrect System Configuration");

    //we could not find the default environment here
    //make this a popup window with a timer?
    jtrace->error ("Cannot find the basic data paths");
    jtrace->error ("Exiting Program");
    jtrace->error ("-------------------------------------");
    exit(0);
}


///////////////////////////////////////////////////////
//commands to trap spaceball events and
//send them on to the jahapp

#ifdef SPACEBALL
void Jahshaka::customEvent(QCustomEvent *event) {
    switch ((int)event->type()) {

        case 1001 : { getSpaceballData(event);   break; }
        case 1002 : { getSpaceballButton(event); break; }

        default:
            { qWarning("Unknown custom event type: %d", event->type()); }
    }
}
#else
void Jahshaka::customEvent(QCustomEvent *) 
{
}
#endif

#ifdef SPACEBALL
void Jahshaka::getSpaceballData(QCustomEvent *event)
{
    MagellanFloatEvent *s = (MagellanFloatEvent *) event->data();

    jahapp->spaceballmoved(s->MagellanData[ MagellanX ],s->MagellanData[ MagellanY ],
                            s->MagellanData[ MagellanZ ],
                            s->MagellanData[ MagellanA ],s->MagellanData[ MagellanB ],
                            s->MagellanData[ MagellanC ]);
}
#else
void Jahshaka::getSpaceballData(QCustomEvent *) { }
#endif

void Jahshaka::getSpaceballButton(QCustomEvent *event)
{
    int *s = (int *) event->data();
    QString theval; theval = theval.setNum(*s);
    //debug("Spaceball button:"+theval);
}

// We shouldn't be overriding the default processing here, but this provides
// a halfway sane approach - we handle the main windows normal processing first
// and if the events aren't handled, we pass it on to the custom processing of jah
void Jahshaka::keyPressEvent( QKeyEvent *k )
{
	k->ignore( );
	QMainWindow::keyPressEvent( k );

    //pass it down for processing
	if ( !k->isAccepted( ) )
    	jahapp->processKeyPress(k);
}

void Jahshaka::keyReleaseEvent( QKeyEvent *k )
{
	k->ignore( );
	QMainWindow::keyReleaseEvent( k );

    //pass it down for processing
	if ( !k->isAccepted( ) )
    	jahapp->processKeyRelease(k);
}

//we can use this as well for more power
//QListViewItem* item = itemAt(event->pos());

void Jahshaka::contextMenuEvent( QContextMenuEvent * )
{

    QPopupMenu* contextMenu = new QPopupMenu( this );
    Q_CHECK_PTR( contextMenu );
    QLabel *caption = new QLabel( "<font color=darkblue><u><b>"
        "Context Menu</b></u></font>", this );
    caption->setAlignment( Qt::AlignCenter );
    contextMenu->insertItem( caption );
    contextMenu->insertItem( "Load",  jahapp->JahDesktopMod, SLOT(slotLoadIconview())  );
    contextMenu->insertItem( "Save",  jahapp->JahDesktopMod, SLOT( slotSave() )  );
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



void Jahshaka::JahchangeModeText(const QString & text) 
{
  jtrace->info(

	       "changeModeText",
	       text 
	       );
  thestartupscreen->setProgressText(text); 
}
void Jahshaka::JahaddProgressSteps(int steps)
{
  thestartupscreen->addProgressSteps(steps); 
}

void Jahshaka::JahsetProgressValue( ) 
{ 
  thestartupscreen->updateProgressValue( );  
}


JahControl* Jahshaka::getJahApp() 
{
  return jahapp; 
}
