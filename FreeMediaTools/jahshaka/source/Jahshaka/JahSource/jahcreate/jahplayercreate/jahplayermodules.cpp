/*******************************************************************************
**
** The source file for the Jahshaka desktop module file
** The Jahshaka Project
** Copyright (C) 2000-2004 The Jahshaka Project
** Released under the GNU General Public License
**
*******************************************************************************/

////////////////////////////////////////////////////////////////
// This is the file that sets up all the individual modules
// and connects them to the desktop

#include "jahplayercreate.h"
#include "anime.h"
#include <glcore.h>

/////////////////////////////////////////////
// create moudles utility routines 

void JahControl::CreateModule(bool dhead)
{
	if (!dhead)
	{
		//////////////////////////////////////////////
		//create single head display

		//the main worldspace
		MainModuleControler[module_counter]			= new QHBox(MainControler);
		MainControler->addWidget(MainModuleControler[module_counter],module_counter);

		MainModuleControlerLeft[module_counter]		= new QHBox(MainLeftControler);
		MainLeftControler->addWidget(MainModuleControlerLeft[module_counter],module_counter); //animation

		MainModuleControlerRight[module_counter]	= new QHBox(MainRightControler);
		MainRightControler->addWidget(MainModuleControlerRight[module_counter],module_counter); //animation

		//added for new left frame and right frame
		MainModuleControlerLeftFrame[module_counter]		= new QHBox(WorldLayoutLeft);
		WorldLayoutLeft->addWidget(MainModuleControlerLeftFrame[module_counter],module_counter); //animation

		MainModuleControlerRightFrame[module_counter]	= new QHBox(WorldLayoutRight);
		WorldLayoutRight->addWidget(MainModuleControlerRightFrame[module_counter],module_counter); //animation

        //set some defaults for the ui
        MainModuleControlerRightFrameWidth[module_counter] = 0;      
        MainModuleControlerCenterFrameWidth[module_counter] = 600;
        MainModuleControlerLeftFrameWidth[module_counter] = 0;

		//module user controles
//		ModulesOptions[module_counter] = new QHBox(optionals);
		//optionals->addWidget(ModulesOptions[module_counter],module_counter);
	}
	else
	{

	}
}

/////////////////////////////////////////////
// create moudles routines starts here

void JahControl::CreateDesktopModule(QHBox* frame,QHBox*)
{

    CreateModule(dualheadprefs);

	////////////////////////////////////////////////////////////////////////
	//get the id to be used for switching
	//currently just based on the MainModuleController
	//but we may need to record ID's for left, right, etc 
	//in cases where they are not used

	ModulesID[DESKTOP] = module_counter;
	
    ////////////////////////////////////////////////////////////////////////
    // create the desktop module/object

    JahDesktopMod = new JahDesktop( frame, 
                                    MainModuleControlerLeftFrame[module_counter],
                                    MainModuleControlerRightFrame[module_counter],
									MainModuleControlerLeft[module_counter], 
									MainModuleControlerRight[module_counter],
									"desktop", &globalclipnumber );


    ///////////////////////////////////////////////////////////////
    //now we set up the desktop interface
    //all other modules do this themselves ie effects,animation
    //but since desktop controls all of them its easier to do it here
	CreateDesktopModuleOptions();

	module_counter++;

}

void JahControl::CreateDesktopModuleOptions(void)
{

    // setup the desktop middle frame first ie load, delete, play, etc
    CreateDesktopMiddleFrame(MainModuleControler[module_counter]);

	////////////////////////////////////////////////
	//we are going to create a separate qwidget stack for the desktop itself...
	//vs sending options down to the desktop
	//since its easier to connect this stuff here
	//we can move this down later on
	// Create Desktop Interface Preferences Here
    prefsdialog   = new QWidget(0,"Preferences");	
	prefsdialog->setMinimumSize( QSize( 700, 200 ) );
	prefsdialog->setMaximumSize( QSize( 700, 200 ) );
	prefsdialog->setBaseSize( QSize( 700, 200 ) );
	prefsdialog->resize( QSize(700, 200) );

	//prefsdialog->hide();
    
    optionals_prefs = new QFrame( prefsdialog, "prefsController" );
    optionals_prefs->setMinimumSize( QSize( 700, 200 ) );
    optionals_prefs->setMaximumSize( QSize( 700, 200 ) );

	preferencesframe  = new QHBox(optionals_prefs);
	preferencesframe->setGeometry( QRect( 0, 0, 860, 200 ) );
    CreateDesktopPreferences(preferencesframe);
}

void JahControl::CreatePlayerModule(QFrame* frame)
{
	////////////////////////////////////////////////////////////////////////   

    CreateModule(dualheadprefs);   

	////////////////////////////////////////////////////////////////////////
	ModulesID[PLAYER] = module_counter; 

    ////////////////////////////////////////////////////////////////////////
    // create the player module/object

    JahPlayerMod = new MainPlayer(	core, frame, "Player", 
    							    MainModuleControlerRightFrame[module_counter],
									0, 
									MainModuleControler[module_counter],
									MainModuleControlerLeft[module_counter],
									MainModuleControlerRight[module_counter] );
	module_counter++;

	//////////////////////////////////////////////////////
    //added for widescreen mode player updates
    connect( JahPlayerMod,  SIGNAL(playerDraggedAImage()),  this, SLOT(notifyPlayerOfChange())  );

    //ui connects
	connect( JahPlayerMod,  SIGNAL(toggleTheDesktopImages()), this, SLOT(changeModeDesktop())  );

    //this should be changemodedeskoptable
	connect( JahPlayerMod,  SIGNAL(toggleTheDesktopTable()), this, SLOT(changeModeDesktopTable())  );

    connect( JahPlayerMod,  SIGNAL(playerRequestsNextItem( ) ),  this, SLOT(notifyPlayerOfNextItem())  );
    connect( JahPlayerMod,  SIGNAL(syncMediaTable( assetData ) ),  this, SLOT(syncMediaTable( assetData ))  );


    //connect( JahPlayerMod,   SIGNAL(toggleTheDesktopImages()), JahDesktopMod, SLOT(toggleDesktopImages() )  );
    //connect( JahPlayerMod,     SIGNAL(toggleTheDesktopTable()), JahDesktopMod, SLOT(toggleDesktopTable() )  );
    
    //////////////////////////////////////////////////////
    //display startup image.in video player.
    //gives trouble under linux... due to timer in player?
    //displayVideo2startupImage();

}



void JahControl::CreateAnimationModule() 
{
    
	CreateModule(); 
    
	
	////////////////////////////////////////////////////////////////////////
	ModulesID[ANIMATION] = module_counter;

	QString rere; rere.setNum(ModulesID[ANIMATION]);
	//qDebug(" Desktop ID : "+rere);

    ////////////////////////////////////////////////////////////////////////
    // create the animation module/object

    JahAnimationMod = new GLAnime(	core, "Animation" ,
                                    0,
									MainModuleControler[module_counter],
									MainModuleControlerLeft[module_counter], 
									MainModuleControlerRight[module_counter],
									&globalclipnumber  );
	module_counter++;

	////////////////////////////////////////////////////////////////////////

    //without this windows boxes loose tracking in the anime module (only?)
    JahAnimationMod->setMouseTracking( false );

    //connect module to desktop framework
    connect ( JahAnimationMod,   SIGNAL(updateDesktop(assetData)), 
		SLOT( addImageToDesktop(assetData)) );

    //this is the extra slot for the media bar...
    connect ( JahAnimationMod,   SIGNAL(GrabClip()),			   
		//SLOT( GrabClipAnim()) );
		SLOT( GetDesktop()) );


    connect ( JahAnimationMod,   SIGNAL(GrabClipKey()),			   
		SLOT( GetDesktopKey()) );
		//SLOT( GrabClipAnimKey()) );
}




///////////////////////////////////////////////////////////////////////////
//this module hasnt really been built in its currently a ui widget hack
//based on the FileMainFrame object in uiwidgets

void JahControl::CreateLibraryModule(QFrame* frame)
{

    ////////////////////////////////////////////////////////////////////////
    // create the player module/object
    CreateModule(dualheadprefs);   

	////////////////////////////////////////////////////////////////////////
	ModulesID[LIBRARY] = module_counter;

	JahLibraryMod = new JahLibrary ( frame,"shareview",
									 MainModuleControlerLeftFrame[module_counter], //the media bin frame
									 MainModuleControlerRightFrame[module_counter], //the media bin frame
								     0,//ModulesOptions[module_counter],
								     MainModuleControler[module_counter],
								     MainModuleControlerLeft[module_counter], 
								     MainModuleControlerRight[module_counter] );

	module_counter++;
    
	////////////////////////////////////////////////////////////////////////

	//this is where we set the share folder
    JahLibraryMod->setDirectory( JahMediaPath+"media/" );

	connect( JahLibraryMod, SIGNAL(loadButton( )), this, SLOT(getClipFromLibrary( )) );

    connect( JahLibraryMod, SIGNAL(importButton()), this, SLOT( slotLoadIconview()) );

    //connect( importclip, SIGNAL(clicked()), JahDesktopMod, SLOT( slotLoadIconview()) );
}

