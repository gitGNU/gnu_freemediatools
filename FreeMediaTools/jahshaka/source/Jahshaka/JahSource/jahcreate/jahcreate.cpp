/*******************************************************************************
 **
 ** The source file for the Jahshaka jahcontrol module
 ** The Jahshaka Project
 ** Copyright (C) 2000-2006 VM Inc.
 ** Released under the GNU General Public License
 **
 *******************************************************************************/

////////////////////////////////////////////////////////////////
// This is the main class that creates and launches the jahshaka interface
// and sets up and connects all the modules and interface objects

#include "jahcreate.h"
#include <glcore.h>

#include "effect.h"
#include "paint.h"
#include "character.h"
#include "anime.h"
#include "edit.h"
#include "tracker.h"

#include "desktop.h"
#include "jahlibrary.h"
#include "MainPlayer.h"
#include "dialogs.h"
#include "glworld.h"
#include "color.h"
#ifdef USE_NETWORK
#include "networking.h"
#endif
#include "keyer.h"
#include "jahtranslate.h"
#include "jahheader.h"
#include "themes.h"

//for the encoder module
#include "MainEncoder.h"


#include <openpluginlib/pl/openpluginlib.hpp>
#include <openobjectlib/sg/sg.hpp>
#include <openobjectlib.h>

namespace opl = olib::openpluginlib;
namespace ool = olib::openobjectlib;

JahControl::JahControl( QGLWidget *share,
						QWidget *parent, JahHeader *menucontroller,
                        QFrame  *mainleftmenu, QFrame  *mainworld,			
                        QFrame  *mainrightmenu,
                        QFrame  *maincontroller,
                        QFrame  *moduleselection, QFrame  *modulesettings,	
                        QFrame  *,
                        
                        QWidget *,
                        QFrame  *D2mainleftmenu, QFrame  *D2mainworld,		
                        QFrame  *D2mainrightmenu,
                        QFrame  *D2maincontroller,
                        QFrame  *D2moduleselection, QFrame  *D2modulesettings,	
                        QFrame  * )
    
    : jahStyleprefs( false ),
      menu( NULL ),
      menufile( NULL ),
      menuedit( NULL ), 
      menuhelp( NULL ), 
      menueditor( NULL ), 
      menuanimation( NULL ), 
      menueffects( NULL ), 
      menupaint( NULL ), 
      menutext( NULL ), 
      menudesktop( NULL ),
      menucolor( NULL ), 
      menutracker( NULL ), 
      menukeyer( NULL ), 
      menuplayer( NULL ),
      moduleMenuId( -1 ),
      m_video_head( 0 )
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
    
    createJahUI( mainworld, mainleftmenu, mainrightmenu, 
                 maincontroller, modulesettings,
                 D2mainworld, D2mainleftmenu, D2mainrightmenu, 
                 D2maincontroller, D2modulesettings );
    
    //////////////////////////////////////////////////////////////
    //connect the splash screen through the  parent object
    connect( this,  SIGNAL(creatingModule(const QString &)), parent, SLOT(JahchangeModeText(const QString &))  );
    connect( this,  SIGNAL(updateProgress( )), parent, SLOT(JahsetProgressValue( ))  );
    connect( this,  SIGNAL(addProgressSteps(int)), parent, SLOT(JahaddProgressSteps(int))  );

	// Number of steps varies depending on quick startup preference
	// NB: This will be incremented by the number of assets on the desktop below
	if ( JahPrefs::getInstance().getQuickStart( ) )
		emit addProgressSteps( 12 );
	else
		emit addProgressSteps( 13 );

    /////////////////////////////////////////////////////////////////
    // the main jahshaka 3d core
    // set up the shared core here
    
    theCoreLayout = new QWidgetStack(WorldLayout);
    corebox = new QHBox(theCoreLayout,"glcore");
    paintbox = new QHBox(theCoreLayout,"paintcore");// (parent,"glcore");

    //create core in widget 0
    core = new GLCore ( share, corebox, "glcore");
    theCoreLayout->addWidget(corebox,0);
    
    ///////////////////////////////
    //used to manage paint 2d core
    paintcore = new GLCore ( share, paintbox,"paintcore",1);
    theCoreLayout->addWidget(paintbox,1);

    //raise the corebox gl interface as default
    connect( core,       SIGNAL(draggedAImage()), SLOT(draggedAImage())  );
    connect( paintcore,  SIGNAL(draggedAImage()), SLOT(draggedAImage())  );

    theCoreLayout->raiseWidget(0);

    /////////////////////////////////////////////////////////////////
    //Now we Create and setup the modules
    /////////////////////////////////////////////////////////////////
    
    jtrace->info( "Setting up Interface and loading modules" );
    
    /////////////////////////////////////////////////////////////////
    //Create Desktop interface and buttons
    //we should be doing this in desktopinterface.cpp so it can create
    //its own interface like all the other modules...
    jtrace->info( ">creating desktop");
    emit creatingModule(jt->tr("Desktop"));
    
    createJahDesktopUI(moduleselection, D2moduleselection);
    
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

	// Rebuild the desktop here if requested
	if ( !JahPrefs::getInstance().getQuickStart( ) ) 
	{
    	connect( JahDesktopMod, SIGNAL( updateProgress( ) ), parent, SLOT( JahsetProgressValue( ) ) );
    	connect( JahDesktopMod, SIGNAL( addProgressSteps( int ) ), parent, SLOT( JahaddProgressSteps( int ) ) );
		JahDesktopMod->RebuildDesktop( );
	}

	// Signal for the desktop itself
    emit updateProgress( );
    
    /////////////////////////////////////////////////////////////////
    //first lest create the main buttons in the bottom left menu
    //make this a subroutine ?
    createJahDesktopButtons(moduleselection); //moduleselection
    
    setMainworldQframe(mainworld);
    
    /////////////////////////////////////////////////////////////////
    //we set up the player module here
    jtrace->info( "Loading Player Module" );
    
    emit creatingModule(jt->tr("Player"));
    
    if (dualheadprefs)
    {
        playerdisplay = new QFrame (D2WorldLayout, "player");
        D2WorldLayout->addWidget(playerdisplay);
    }
    else
    {
        playerdisplay = new QFrame (WorldLayout, "player");
        WorldLayout->addWidget(playerdisplay);
    }
    
    CreatePlayerModule(playerdisplay);
    emit updateProgress( );
    
    /////////////////////////////////////////////////////////////////
    //we set up the encoding application here
    jtrace->info( "Loading Encoding Module" );
    
    emit creatingModule(jt->tr("Encoder"));
    
    if (dualheadprefs)
    {
        encodingdisplay = new QFrame (D2WorldLayout, "Encoder");
        D2WorldLayout->addWidget(encodingdisplay);
    }
    else
    {
        encodingdisplay = new QFrame (WorldLayout, "Encoder");
        WorldLayout->addWidget(encodingdisplay);
    }
    
    CreateEncodingModule(encodingdisplay);
    emit updateProgress( );
    
    /////////////////////////////////////////////////////////////////
    //we set up the library here
    //currently its not a real module but that will change later on
    
    jtrace->info( "Loading Library Module" );
    
    emit creatingModule(jt->tr("Library"));
    
    if (dualheadprefs)
    {
        librarydisplay = new QFrame (D2WorldLayout, "Library");
        D2WorldLayout->addWidget(librarydisplay,module_counter);
    }
    else
    {
        librarydisplay = new QFrame (WorldLayout, "Library");
        WorldLayout->addWidget(librarydisplay,module_counter);
    }
    
    CreateLibraryModule(librarydisplay);
    emit updateProgress( );
    
    /////////////////////////////////////////////////////////////////
    //we set up the network module  here
    
#ifdef USE_NETWORK
    jtrace->info( "Loading Network Module" );
    
    emit creatingModule(jt->tr("Network"));

    thenetworkdisplay = new QFrame (WorldLayout, "net");
    WorldLayout->addWidget(thenetworkdisplay,module_counter);
    
    CreateNetworkModule(thenetworkdisplay);
    emit updateProgress( );
#endif

    /////////////////////////////////////////////////////////////////
    //Now we can Create and setup the different jahshaka modules
    //we do this after the others sice they all use the shared core
    //and we are having selection issues

    /////////////////////////////////////////////////////////////////
    // the main jahshaka 3modules start here
    /////////////////////////////////////////////////////////////////
    
    ////////////////////////////////////////////////////////////////
    //we set up the animation module here
    jtrace->info( "Loading Animation Module" );
    
    emit creatingModule(jt->tr("Animation"));
    CreateAnimationModule();
    emit updateProgress( );
    
    /////////////////////////////////////////////////////////////////
    //we set up the Effects module here
    jtrace->info( "Loading Effects Module" );
    
    emit creatingModule(jt->tr("Effects"));
    CreateEffectsModule();
    emit updateProgress( );
    
    /////////////////////////////////////////////////////////////////
    //we set up the editing module here
    jtrace->info( "Loading Editing Module" );
    
    emit creatingModule(jt->tr("Editing"));
    CreateEditingModule(); // EXEC
    emit updateProgress( );
    
    /////////////////////////////////////////////////////////////////
    //we set up the CG module here
    jtrace->info( "Loading Text Module" );
    
    emit creatingModule(jt->tr("Text"));
    CreateTextModule();
    emit updateProgress( );
    
    /////////////////////////////////////////////////////////////////
    //these modules are in the desktop menu
    
    /////////////////////////////////////////////////////////////////
    //we set up the colorcorrection module here
    jtrace->info( "Loading Colorize Module" );
    
    emit creatingModule(jt->tr("Colorize"));
    CreateColorModule();
    emit updateProgress( );
    
    /////////////////////////////////////////////////////////////////
    //we set up the Keyer module here
    jtrace->info( "Loading Keyer Module" );
    
    emit creatingModule(jt->tr("Keyer"));
    CreateKeyerModule();
    emit updateProgress( );
    
    /////////////////////////////////////////////////////////////////
    //we set up the Tracker module here
    jtrace->info( "Loading Tracker Module" );
    
    emit creatingModule(jt->tr("Tracker"));
    CreateTrackerModule();
    emit updateProgress( );
    
    
    /////////////////////////////////////////////////////////////////
    //we set up the paint module here
    jtrace->info( "Loading Paint Module" );
    
    emit creatingModule(jt->tr("Paint"));
    CreatePaintModule();
    emit updateProgress( );
    
    
    //////////////////////////////////////////////////////////////////
    //finished loading modules
    //////////////////////////////////////////////////////////////////
    
    jtrace->info( "Finished loading modules" );
    
    if(qApp->hasPendingEvents())
        qApp->processEvents();
    
    //////////////////////////////////////////////////////////////////
    //setup menubar if needed
    buildMenuBar( menucontroller );
    
    if (menuprefs)
        menucontroller->show();
    else
        menucontroller->hide();
    
    //restore modules ui
    restoreModulesUI();
    
	//load plugins
#ifndef JAHPLAYER
	if ( !JahPrefs::getInstance().getQuickStart( ) )
	{
		emit creatingModule(jt->tr("Plugins"));
		initializePlugins( );
    	emit updateProgress( );
	}
#endif

    //configure modules for user
    configSystem();
    
    /////////////////////////////////////////////////////////////////
    //connect the hide-ui option
    connect( this,  SIGNAL(hideUI(bool)), parent, SLOT(shrinkUI(bool))  );

    Globals::setJahControl(this);
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
    updateDesktopLabel( "Interactive Desktop" );
    // GetButton->hide();
    
    if (dualhead)
    {
        raiseD2Module(DESKTOP);
        D2WorldLayout->raiseWidget(desktopdisplay);  
    }
    else
    {
        raiseModule(DESKTOP);
        WorldLayout->raiseWidget(desktopdisplay); 
    }
    
    JahDesktopMod->setFocus();
    
    
    //set up ui status
    UIstatus = true;
    
    //clear out paint... we could also do this the first timepaint is called
    //JahPaintMod->clear();
    
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
    
    stopVideoHead( );
    
    delete JahDesktopMod;   jtrace->debug( ">> Closed thedesktop");
    delete JahAnimationMod;    jtrace->debug( ">> Closed animation");
    delete JahEffectsMod;      jtrace->debug( ">> Closed effects");
    delete JahEditingMod;      jtrace->debug( ">> Closed editing");
    delete JahTextMod;         jtrace->debug( ">> Closed text");

    delete JahColorMod;     jtrace->debug( ">> Closed colorize");
    delete JahPlayerMod;    jtrace->debug( ">> Closed vidplayer");

#ifdef USE_NETWORK
    delete JahNetworkMod;   jtrace->debug( ">> Closed jahnetwork");
#endif

    delete JahPaintMod;        jtrace->debug( ">> Closed paint");
    delete JahKeyerMod;        jtrace->debug( ">> Closed keyer");
    delete JahLibraryMod;        jtrace->debug( ">> Closed library");
    delete JahEncoderMod;        jtrace->debug( ">> Closed encoding");
    delete JahTrackerMod;        jtrace->debug( ">> Closed tracker");
    delete core;
    delete paintcore;
    
    opl::uninit( ); // olib::openpluginlib::uninit(

	delete jahthemes;

    jtrace->debug( "JahControl Destructor","Closed out the core modules" );    
}

void JahControl::hidetheUI()
{
    UIstatus = !UIstatus;
    emit hideUI(UIstatus);
}

void JahControl::hidetheUI( bool on )
{
    UIstatus = !on;
    emit hideUI( UIstatus );
}

//we need to load the plugins here for jahshaka
void JahControl::initializePlugins( )
{
    static bool initialized = false;
    
    if ( initialized ) return;
    initialized = true;
    
    /////////////////////////////////////////////////////////////////
    //ok lets load the plugins in now...
    jtrace->info( ">Initializing Plugins");
    
    JahPluginLib* jplugin = JahPluginLib::getInstance();
    jplugin->initializePlugins();
    
    jtrace->info( ">Initialized Plugins");
    
    /////////////////////////////////////////////////////////////////
	//can we assume we dont need to do this any more as we no longer
	//use the old openobject plugin lib?

    //initialize the openobjectlib 
    //jtrace->info( ">Initializing OpenObjectLib");
    
    //OpenObjectLib* oolib = OpenObjectLib::getInstance();
    //oolib->initializePlugins();
    //oolib->listPlugins();
    
    jtrace->info( ">Initialized OOlib Plugins");
}

