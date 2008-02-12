/*******************************************************************************
**
** The source file for the Jahshaka jahcontrol module
** The Jahshaka Project
** Copyright (C) 2000-2004 The Jahshaka Project
** Released under the GNU General Public License
**
*******************************************************************************/

////////////////////////////////////////////////////////////////
// This is the main class that creates and launches the jahshaka interface
// and sets up and connects all the modules and interface objects

#include "jahplayercreate.h"
#include "anime.h"
#include <glcore.h>

#include <openpluginlib/pl/openpluginlib.hpp>
#include <openobjectlib/sg/sg.hpp>

namespace opl = olib::openpluginlib;
namespace ool = olib::openobjectlib;

JahControl::JahControl( QWidget *parent,			QWidget *menucontroller,
                        QFrame  *mainleftmenu,		QFrame  *mainworld,			
						QFrame  *mainrightmenu,
                        QFrame  *maincontroller,
                        QFrame  *moduleselection,	QFrame  *modulesettings,	
						QFrame  *feedbackbar)

{
	////////////////////////////////////////////////////////////////
	// initialise OpenPluginLib
#ifndef WIN32
	const opl::string oil_plugin_path( OPENIMAGELIB_PLUGINS );
	const opl::string oel_plugin_path( OPENEFFECTSLIB_PLUGINS );
	const opl::string oml_plugin_path( OPENMEDIALIB_PLUGINS );
	const opl::string ool_plugin_path( OPENOBJECTLIB_PLUGINS );
#endif

#ifdef WIN32
	opl::init( "" );
#else
	opl::init( oil_plugin_path );
	opl::init( oel_plugin_path );
	opl::init( oml_plugin_path );
	opl::init( ool_plugin_path );
#endif	

	ool::init( );

	//////////////////////////////////////////////////////////
	//initalize the main variables
	initializeVariables();

	//////////////////////////////////////////////////////////
    //retreive preferences
    getGlobalPreferences();

    //////////////////////////////////////////////////////////
    //create the main frame object controllers
    //used to switch the framesets out according to
    //what module we are currently in

	//////////////////////////////////////////////////////////
	//initalize the main variables
    jtrace->info( "Setting up Interface and loading modules" );

	createJahUI(	mainworld, mainleftmenu, mainrightmenu, 
					maincontroller, modulesettings);

    //////////////////////////////////////////////////////////////
    //connect this to the splash screen through the  parent object
    connect( this,  SIGNAL(creatingModule(const QString &)), 
		parent, SLOT(JahchangeModeText(const QString &))  );
    connect( this,  SIGNAL(finishedModule(int)), 
		parent, SLOT(JahsetProgressValue(int))  );

    /////////////////////////////////////////////////////////////////
    //Now we Create and setup the desktop related modules
	/////////////////////////////////////////////////////////////////

    jtrace->info( "Setting up Interface and loading modules" );

	////////////////////////////////////////////////////////////////

    jtrace->info( ">creating desktop");

    emit creatingModule(jt->tr("Desktop"));

    /////////////////////////////////////////////////////////////////
    //Create Desktop interface and buttons
    //we should be doing this in desktopinterface.cpp so it can create
    //its own interface like all the other modules...
	createJahDesktopUI(moduleselection, 0);

    //create desktop feedback bar
    //CreateDesktopFeedbackBox(feedbackbar);

    //connect the desktop preferences to parent object
    connect( this,           SIGNAL(emitStyleColor(QPalette)),    
        parent, SLOT(setStyleColor(QPalette))  );
    connect( JahDesktopMod,  SIGNAL(resetGlobalClipNumber()),     
        this,   SLOT(resetGlobalClipNumber())  );
    connect( JahDesktopMod,  SIGNAL(switchMainModule(QString)),   
        this,   SLOT(switchModule(QString))  );

    connect( JahDesktopMod,  SIGNAL(loadMainModuleFile(QString)), 
        this,   SLOT(loadModuleFile(QString))  );
    connect( JahDesktopMod,  SIGNAL(loadMainModelFile(QString)), 
        this,   SLOT(loadModelFile(QString))  );

    emit finishedModule(module_counter);

    /////////////////////////////////////////////////////////////////
    //Now we Create and setup the desktop related modules
    jtrace->info( ">>Loading Main Modules" );

    /////////////////////////////////////////////////////////////////
    //first lest create the main buttons in the bottom left menu
	//make this a subroutine ?
	//createJahDesktopButtons(moduleselection); //moduleselection

    // the main jahshaka 3d core
	// set up the shared core here

    theCoreLayout = new QWidgetStack(WorldLayout);
    corebox = new QHBox(theCoreLayout,"glcore");

    //create core in widget 0
    core = new GLCore (corebox,"glcore");

	glewInit( );

    /////////////////////////////////////////////////////////////////
    //we set up the player module here
    jtrace->info( "Loading Player Module" );

    emit creatingModule(jt->tr("Player"));

    /*if (dualheadprefs)
    {
        playerdisplay = new QFrame (D2WorldLayout, "player");
        D2WorldLayout->addWidget(playerdisplay);
    }
    else
    {*/
        playerdisplay = new QFrame (WorldLayout, "player");
        WorldLayout->addWidget(playerdisplay);
    //}
    
	CreatePlayerModule(playerdisplay);
    emit finishedModule(module_counter);


    /////////////////////////////////////////////////////////////////
    //we set up the library here
    //currently its not a real module but that will change later on

    jtrace->info( "Loading Library Module" );

    emit creatingModule(jt->tr("Library"));
    
    /*if (dualheadprefs)
    {
        librarydisplay = new QFrame (D2WorldLayout, "Library");
        D2WorldLayout->addWidget(librarydisplay,module_counter);
    }
    else
    {*/
        librarydisplay = new QFrame (WorldLayout, "Library");
        WorldLayout->addWidget(librarydisplay,module_counter);
    //}
        
	CreateLibraryModule(librarydisplay);
    emit finishedModule(module_counter);

    /////////////////////////////////////////////////////////////////
    //Now we can Create and setup the different jahshaka modules
	//we do this after the others sice they all use the shared core
	//and we are having selection issues

	/////////////////////////////////////////////////////////////////
    //this is where we set up the core color mon
    //new to the preferences
    //core->setBackgroundColor (0.0, 0.0, 0.0 );

    connect( core,  SIGNAL(draggedAImage()), SLOT(draggedAImage())  );

    theCoreLayout->addWidget(corebox,0);

    //raise the corebox gl interface as default
    theCoreLayout->raiseWidget(0);

    /////////////////////////////////////////////////////////////////
    // the main jahshaka 3modules start here
	/////////////////////////////////////////////////////////////////


    ////////////////////////////////////////////////////////////////
    //we set up the animation module here
    jtrace->info( "Loading Animation Module" );

    emit creatingModule(jt->tr("Animation"));
    CreateAnimationModule();
    emit finishedModule(module_counter);

	//////////////////////////////////////////////////////////////////
    //finished loading modules
	//////////////////////////////////////////////////////////////////

    jtrace->info( "Finished loading modules" );

    if(qApp->hasPendingEvents())
        qApp->processEvents();
    
	//////////////////////////////////////////////////////////////////
    //setup menubar if needed
    buildMenuBar(menucontroller);

    if (menuprefs)
		menucontroller->show();
	else
		menucontroller->hide();

    //restore modules ui
    restoreModulesUI();

    //configure modules for user
    configSystem();

    /////////////////////////////////////////////////////////////////
    //connect the hide-ui option
    connect( this,  SIGNAL(hideUI(bool)), 
		parent, SLOT(shrinkUI(bool))  );
}

void JahControl::configSystem()
{

    jtrace->debug("configuring system");

    /////////////////////////////////////////////////////////////////
    //Raise and activate the default modules
    if (dualheadprefs==true)
    {
        module = ANIMATION;
        changeModeAnimation();
        dualhead = true;

        //a dual head hack
        //mainworld->resize(964,586);

    }
	else
	{
		dualhead=false;
	}

    //set the main module to desktop
    module = DESKTOP;
//    updateDesktopLabel( " Jahshaka Interactive Desktop" );
    //GetButton->hide();
/*
    if (dualhead)
    {
		raiseD2Module(DESKTOP);
	    D2WorldLayout->raiseWidget(desktopdisplay);  
    }
    else
    {*/
		raiseModule(DESKTOP);
		WorldLayout->raiseWidget(desktopdisplay); 
    //}

    JahDesktopMod->setFocus();


    //set up ui status
    UIstatus = true;
    
    //clear out paint... we could also do this the first timepaint is called
//    JahPaintMod->clear();
    //paint->updateGL();

    //update all modules with renderquality now that they have been created
    //maybe we can change globalclipnumber with a global prefs struct
    setModulesRenderQuality();

}

////////////////////////////////////////////////////////
//Jahshaka Destructor
//cleans up as we leave and releases memory...
JahControl::~JahControl() 
{
    jtrace->debug( "JahControl Destructor","Closing out the core modules" );

    delete JahDesktopMod;   jtrace->debug( ">> Closed thedesktop");
    delete JahAnimationMod;    jtrace->debug( ">> Closed animation");
   
    delete JahPlayerMod;    jtrace->debug( ">> Closed vidplayer");


    jtrace->debug( "JahControl Destructor","Closed out the core modules" );


}

void JahControl::hidetheUI()
{
    //////////////////////////////////////////////////////
    //display startup image.in video player
    //but only if video player is active...
    UIstatus = !UIstatus;

    emit hideUI(UIstatus);

    #ifdef JAHPLAYER
    //we hide the splitters in the player as well
    //for a true fullscreen
    if (UIstatus)
    {
        wideframeLeft->show();
        wideframeRight->show();
        raiseModule(module);
    }
    else
    {
        wideframeLeft->hide();
        wideframeRight->hide();
    }
    #endif

}

