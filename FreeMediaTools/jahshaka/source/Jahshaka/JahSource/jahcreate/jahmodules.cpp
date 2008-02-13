/*******************************************************************************
 **
 ** The source file for the Jahshaka desktop module file
 ** The Jahshaka Project
 ** Copyright (C) 2000-2006 VM Inc.
 ** Released under the GNU General Public License
 **
 *******************************************************************************/

////////////////////////////////////////////////////////////////
// This is the file that sets up all the individual modules
// and connects them to the desktop

#include "jahcreate.h"
#include <glcore.h>
#include <qlineedit.h>

#include "effect.h"
#include "paint.h"
#include "character.h"
#include "anime.h"
#include "edit.h"
#include "tracker.h"
#include "desktop.h"
#include "jahlibrary.h"
#include "MainPlayer.h"
#include "color.h"
#ifdef USE_NETWORK
#include "networking.h"
#endif
#include "keyer.h"
#include "MainEncoder.h"

/////////////////////////////////////////////
// create moudles utility routines 

void JahControl::CreateModule(bool dhead)
{
    if (!dhead)
    {
        //////////////////////////////////////////////
        //create single head display
        
        //the main worldspace
        MainModuleControler[module_counter] = new QHBox(MainControler);
        MainControler->addWidget(MainModuleControler[module_counter],module_counter);
        MainModuleControler[module_counter]->setFrameShape( QFrame::NoFrame );

        MainModuleControlerLeft[module_counter] = new QHBox(MainLeftControler);
        MainLeftControler->addWidget(MainModuleControlerLeft[module_counter],module_counter); //animation
        
        MainModuleControlerRight[module_counter] = new QHBox(MainRightControler);
        MainRightControler->addWidget(MainModuleControlerRight[module_counter],module_counter); //animation
        
        //added for new left frame and right frame
        MainModuleControlerLeftFrame[module_counter] = new QHBox(WorldLayoutLeft);
        WorldLayoutLeft->addWidget(MainModuleControlerLeftFrame[module_counter],module_counter); //animation
        
        MainModuleControlerRightFrame[module_counter] = new QHBox(WorldLayoutRight);
        WorldLayoutRight->addWidget(MainModuleControlerRightFrame[module_counter],module_counter); //animation
        
        //set some defaults for the ui
        MainModuleControlerRightFrameWidth[module_counter] = 0;      
        MainModuleControlerCenterFrameWidth[module_counter] = 600;
        MainModuleControlerLeftFrameWidth[module_counter] = 0;
        
        //module user controles
        ModulesOptions[module_counter] = new QHBox(optionals);
        optionals->addWidget(ModulesOptions[module_counter],module_counter);
    }
    else
    {
        //////////////////////////////////////////////
        //create in dualhead frame
        MainModuleControler[module_counter] = new QHBox(D2MainControler);
        D2MainControler->addWidget(MainModuleControler[module_counter],module_counter);
        
        MainModuleControlerLeft[module_counter]	= new QHBox(D2MainLeftControler);
        D2MainLeftControler->addWidget(MainModuleControlerLeft[module_counter],module_counter); //animation
        
        MainModuleControlerRight[module_counter] = new QHBox(D2MainRightControler);
        D2MainRightControler->addWidget(MainModuleControlerRight[module_counter],module_counter); //animation
        
        //these dont care about d2 as they are created in the right place
        MainModuleControlerLeftFrame[module_counter] = new QHBox(WorldLayoutLeft);
        WorldLayoutLeft->addWidget(MainModuleControlerLeftFrame[module_counter],module_counter); //animation
        
        MainModuleControlerRightFrame[module_counter] = new QHBox(WorldLayoutRight);
        WorldLayoutRight->addWidget(MainModuleControlerRightFrame[module_counter],module_counter); //animation
        
        //set some defaults for the ui
        MainModuleControlerRightFrameWidth[module_counter] = 0;      
        MainModuleControlerCenterFrameWidth[module_counter] = 600;
        MainModuleControlerLeftFrameWidth[module_counter] = 0;
        
        //module user controles
        ModulesOptions[module_counter] = new QHBox(D2optionals);
        D2optionals->addWidget(ModulesOptions[module_counter],module_counter);
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
                                    "Desktop", &globalclipnumber );
    
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
    
    theDesktopLayout = new QWidgetStack(ModulesOptions[module_counter]);
    
    //Set up Desktop Interface Options and buttons here
    //desktopframe  = new QHBox(ModulesOptions[module_counter]);
    desktopframe  = new QHBox(theDesktopLayout);
    theDesktopLayout->addWidget(desktopframe,0);
    
    CreateDesktopOptions(desktopframe);
    
    //Set up Desktop Preferences Options and buttons here
    preferencesframe  = new QHBox(theDesktopLayout);
    theDesktopLayout->addWidget(preferencesframe,1);
    
    CreateDesktopPreferences(preferencesframe);
    
    theDesktopLayout->raiseWidget(0);
    
}

void JahControl::CreatePlayerModule(QFrame* frame)
{
    ////////////////////////////////////////////////////////////////////////   
    
    CreateModule(dualheadprefs);   
    
    ////////////////////////////////////////////////////////////////////////
    ModulesID[PLAYER] = module_counter; 
    
    ////////////////////////////////////////////////////////////////////////
    // create the player module/object
    JahPlayerMod = new MainPlayer( core, frame, "Player", 
                                   MainModuleControlerRightFrame[module_counter],
                                   ModulesOptions[module_counter], 
                                   MainModuleControler[module_counter],
                                   MainModuleControlerLeft[module_counter],
                                   MainModuleControlerRight[module_counter] );
    module_counter++;

    //////////////////////////////////////////////////////

    //added for widescreen mode player updates
    connect( JahPlayerMod,  SIGNAL(playerDraggedAImage()),  this, SLOT(notifyPlayerOfChange())  );
    connect( JahPlayerMod,  SIGNAL(playerRequestsNextItem( ) ),  this, SLOT(notifyPlayerOfNextItem())  );
    connect( JahPlayerMod,  SIGNAL(syncMediaTable( assetData ) ),  this, SLOT(syncMediaTable( assetData ))  );
    connect( JahPlayerMod,  SIGNAL( showInHead( JahHeadable * ) ),  this, SLOT( showInHead( JahHeadable * ))  );
}

void JahControl::CreateEncodingModule(QFrame* frame)
{
    CreateModule(dualheadprefs);   
    
    ////////////////////////////////////////////////////////////////////////
    ModulesID[ENCODING] = module_counter;
    
    ////////////////////////////////////////////////////////////////////////
    // create the player module/object
    JahEncoderMod = new MainEncoder( core, frame, "Encoder", 
                                     ModulesOptions[module_counter], 
                                     MainModuleControler[module_counter],
                                     MainModuleControlerLeft[module_counter],
                                     MainModuleControlerRight[module_counter] );
    module_counter++;

    /////////////////////////////////////////////////////////////////////////

    connect( JahEncoderMod,  SIGNAL(encoderDraggedAImage()),  this, SLOT(notifyEncoderOfChange())  );
}

#ifdef USE_NETWORK
void JahControl::CreateNetworkModule(QFrame* frame)
{
    
    CreateModule(dualheadprefs);   
    
    ////////////////////////////////////////////////////////////////////////
    ModulesID[NETWORK] = module_counter;
    
    ////////////////////////////////////////////////////////////////////////
    // create the network module/object
    
    JahNetworkMod = new JahNet( JahDesktopMod, frame, MainModuleControlerRightFrame[module_counter],
                                "Network", 
                                ModulesOptions[module_counter],
                                MainModuleControler[module_counter], 
                                &globalclipnumber,
                                this );
    module_counter++;
    
    
    ////////////////////////////////////////////////////////////////////////
    
    connect ( JahNetworkMod,    SIGNAL(updateDesktop(assetData)),	  
              SLOT( addImageToDesktop(assetData)) );
    connect ( JahNetworkMod,    SIGNAL(updateDesktopString(QString)), 
              SLOT( addStringToDesktop(QString)) );
    
    //for ui switching
    connect ( JahNetworkMod,    SIGNAL(switchToNetP2P()),			  
              SLOT( changeModeNetworkP2P()) );
    connect ( JahNetworkMod,    SIGNAL(switchToNetCom()),				  
              SLOT( changeModeNetworkCom()) );
    connect ( JahNetworkMod,    SIGNAL(switchToNetNet()),				  
              SLOT( changeModeNetworkNet()) );
    connect ( JahNetworkMod,    SIGNAL(switchToNetHttpd()),
              SLOT( changeModeNetworkHttpd()) );
    
    //for the httpd preferences
    connect(bindButton, SIGNAL(clicked()), JahNetworkMod, SLOT(bindHttpd()));
    connect(backlogLineEdit, SIGNAL(textChanged(const QString &)), 
            JahNetworkMod, SLOT(updatebacklogLineEdit(const QString &)));
    connect(portLineEdit, SIGNAL(textChanged(const QString &)), 
            JahNetworkMod, SLOT(updateportLineEdit(const QString &)));
}
#endif

void JahControl::CreateAnimationModule() 
{
    
    CreateModule(); 
    
    
    ////////////////////////////////////////////////////////////////////////
    ModulesID[ANIMATION] = module_counter;
    
    QString rere; rere.setNum(ModulesID[ANIMATION]);
    //qDebug(" Desktop ID : "+rere);
    
    ////////////////////////////////////////////////////////////////////////
    // create the animation module/object
    
    JahAnimationMod = new GLAnime( core, "Animation" ,
                                   ModulesOptions[module_counter],
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
    
    connect ( JahAnimationMod,   SIGNAL(GrabDesktop()), SLOT( GetDesktop()) );
    
    //this is the extra slot for the media bar...
    connect ( JahAnimationMod,   SIGNAL(GrabClip()), SLOT( GetDesktop()) );
    connect ( JahAnimationMod,   SIGNAL(GrabClipKey()), SLOT( GetDesktopKey()) );
    
    connect ( JahAnimationMod,     SIGNAL(hideUI( bool )), SLOT( hidetheUI( bool ) ) );
    connect( JahAnimationMod,  SIGNAL( showInHead( JahHeadable * ) ),  this, SLOT( showInHead( JahHeadable * ))  );
}

////////////////////////////////////////////////////////////////
//Code to set up the Effects module

void JahControl::CreateEffectsModule()
{
    
    CreateModule();   
    
    ////////////////////////////////////////////////////////////////////////
    ModulesID[EFFECTS] = module_counter;
    
    ////////////////////////////////////////////////////////////////////////
    // create the effects module/object
    
    JahEffectsMod =	new GLEffect(	core, 
                                        MainModuleControlerLeftFrame[module_counter], //currently this is the mainmodulecontroller-left 
                                        MainModuleControlerRightFrame[module_counter], //currently this is the mainmodulecontroller-left 
                                        "Effects" , 
                                        ModulesOptions[module_counter],
                                        MainModuleControler[module_counter],
                                        MainModuleControlerLeft[module_counter], 
                                        MainModuleControlerRight[module_counter],
                                        &globalclipnumber  );
    module_counter++;
    
    ////////////////////////////////////////////////////////////////////////
    
    //connect module to desktop framework
    connect ( JahEffectsMod,     SIGNAL(updateDesktop(assetData)), SLOT( addImageToDesktop(assetData)) );
    connect ( JahEffectsMod,     SIGNAL(GrabClip()),			   SLOT( GetDesktop()) );
    
    connect ( JahEffectsMod, SIGNAL(GrabDesktop()), SLOT( GetDesktop()) );
    
    connect ( JahEffectsMod,     SIGNAL(hideUI( bool )), SLOT( hidetheUI( bool ) ) );
    connect( JahEffectsMod,  SIGNAL( showInHead( JahHeadable * ) ),  this, SLOT( showInHead( JahHeadable * ))  );
}

void JahControl::CreateEditingModule() 
{
    
    CreateModule();   
    
    ////////////////////////////////////////////////////////////////////////
    ModulesID[EDITING] = module_counter;
    
    ////////////////////////////////////////////////////////////////////////
    // create the editing module/object
    
    JahEditingMod = new GLEdit( core, 
                                MainModuleControlerLeftFrame[module_counter], //currently this is the mainmodulecontroller-left 
                                MainModuleControlerRightFrame[module_counter], //currently this is the mainmodulecontroller-left 
                                "Editing" , 
                                ModulesOptions[module_counter],
                                MainModuleControler[module_counter],
                                MainModuleControlerLeft[module_counter], 
                                MainModuleControlerRight[module_counter],
				&globalclipnumber  );// Exec
    module_counter++;
    
    ////////////////////////////////////////////////////////////////////////
    
    //connect module to desktop framework
    connect ( JahEditingMod, SIGNAL(updateDesktop(assetData)), 
              SLOT( addImageToDesktop(assetData)) );
    
    connect ( JahEditingMod, SIGNAL(GrabClip()),SLOT( GetDesktop()) );     
    connect ( JahEditingMod, SIGNAL(AddClip()), SLOT( AddClipEdit()) );       
    connect ( JahEditingMod, SIGNAL(hideUI( bool )), SLOT( hidetheUI( bool ) ) );
    
    connect ( JahEditingMod, SIGNAL(GrabDesktop()), SLOT( GetDesktop()) );
    
    //added for video head support
    connect( JahEditingMod,  SIGNAL( showInHead( JahHeadable * ) ),  this, SLOT( showInHead( JahHeadable * ))  );
}

void JahControl::CreatePaintModule() 
{
    
    CreateModule();   
    
    ////////////////////////////////////////////////////////////////////////
    ModulesID[PAINT] = module_counter;
    
    ////////////////////////////////////////////////////////////////////////
    // create the paint module/object
    
    JahPaintMod = new GLPaint(	paintcore, "Paint" , 
                                getMainworldQframe(),
                                ModulesOptions[module_counter],
                                MainModuleControler[module_counter],
                                MainModuleControlerLeft[module_counter], 
                                MainModuleControlerRight[module_counter],
                                &globalclipnumber  );
    module_counter++;
    
    ////////////////////////////////////////////////////////////////////////
    //paint core has a fixed size
    //need to set it to whats set up in the preferences ie pal/ntsc/etc
    //paintcore->setFixedSize( QSize( 720, 486 ) );
    
    //connect module to desktop framework
    connect ( JahPaintMod,  SIGNAL(updateDesktop(assetData)), SLOT( addImageToDesktop(assetData)) );
    connect ( JahPaintMod,  SIGNAL(GrabClip()), SLOT( GetDesktop()) );
    
    connect ( JahPaintMod, SIGNAL(GrabDesktop()), SLOT( GetDesktop()) );
    
    //a hack here for the paint buffer updates
    connect ( paintcore,    SIGNAL(resizeit()), JahPaintMod,  
              SLOT( resizePaintGL()) );
    
    connect ( JahPaintMod,     SIGNAL(hideUI( bool )), SLOT( hidetheUI( bool ) ) );
    connect( JahPaintMod,  SIGNAL( showInHead( JahHeadable * ) ),  this, SLOT( showInHead( JahHeadable * ))  );
}

void JahControl::CreateTextModule() 
{
    
    CreateModule();   
    
    ////////////////////////////////////////////////////////////////////////
    ModulesID[TEXTCG] = module_counter;
    
    ////////////////////////////////////////////////////////////////////////
    // create the text module/object
    
    JahTextMod = new GLChar(	core, "Text" , 
                                ModulesOptions[module_counter],
                                MainModuleControler[module_counter],
                                MainModuleControlerLeft[module_counter], 
                                MainModuleControlerRight[module_counter],
                                &globalclipnumber  );
    module_counter++;
    
    ////////////////////////////////////////////////////////////////////////
    
    //connect module to desktop framework
    connect ( JahTextMod,     SIGNAL(updateDesktop(assetData)), SLOT( addImageToDesktop(assetData)) );
    connect ( JahTextMod,     SIGNAL(GrabClip()),               SLOT( GetDesktop()) );
    connect ( JahTextMod, SIGNAL(GrabDesktop()), SLOT( GetDesktop()) );
    
    connect ( JahTextMod,     SIGNAL(hideUI( bool )), SLOT( hidetheUI( bool ) ) );

    //added for video head support
    connect( JahTextMod,  SIGNAL( showInHead( JahHeadable * ) ),  this, SLOT( showInHead( JahHeadable * ))  );
}

void JahControl::CreateColorModule() 
{
    
    CreateModule();   
    
    ////////////////////////////////////////////////////////////////////////
    ModulesID[COLORIZE] = module_counter;
    
    ////////////////////////////////////////////////////////////////////////
    // create the color module/object
    
    JahColorMod =	new GLColorize( core, "Color" , 
                                        ModulesOptions[module_counter],
                                        MainModuleControler[module_counter],
                                        MainModuleControlerLeft[module_counter], 
                                        MainModuleControlerRight[module_counter],
                                        &globalclipnumber  );
    module_counter++;
    
    ////////////////////////////////////////////////////////////////////////
    
    //connect module to desktop framework
    connect ( JahColorMod,    SIGNAL(updateDesktop(assetData)), SLOT( addImageToDesktop(assetData)) );
    connect ( JahColorMod,    SIGNAL(GrabClip()),				SLOT( GetDesktop()) );
    connect ( JahColorMod, SIGNAL(GrabDesktop()), SLOT( GetDesktop()) );
    
    connect ( JahColorMod,     SIGNAL(hideUI( bool )), SLOT( hidetheUI( bool ) ) );

    //added for video head support
    connect( JahColorMod,  SIGNAL( showInHead( JahHeadable * ) ),  this, SLOT( showInHead( JahHeadable * ))  );
}

void JahControl::CreateKeyerModule()
{
    
    CreateModule();   
    
    ////////////////////////////////////////////////////////////////////////
    ModulesID[KEYER] = module_counter;
    
    ////////////////////////////////////////////////////////////////////////
    // create the effects module/object
    
    JahKeyerMod = new GLKeyer(	core, "Keyer" , 
                                ModulesOptions[module_counter],
                                MainModuleControler[module_counter],
                                MainModuleControlerLeft[module_counter], 
                                MainModuleControlerRight[module_counter],
                                &globalclipnumber  );
    module_counter++;
    
    /////////////////////////////////////////////////////////////////////////
    
    //connect module to desktop framework
    connect ( JahKeyerMod, SIGNAL(updateDesktop(assetData)), 
              SLOT( addImageToDesktop(assetData)) );
    
    connect ( JahKeyerMod, SIGNAL(GrabClip()),				 SLOT( GetDesktop()) );
    connect ( JahKeyerMod, SIGNAL(GrabBaseClip()),				 SLOT( GrabClipKeyerBase()) );
    connect ( JahKeyerMod, SIGNAL(GrabDesktop()), SLOT( GetDesktop()) );
    
    connect ( JahKeyerMod,     SIGNAL(hideUI( bool )), SLOT( hidetheUI( bool ) ) );

    //added for video head support
    connect( JahKeyerMod,  SIGNAL( showInHead( JahHeadable * ) ),  this, SLOT( showInHead( JahHeadable * ))  );
}

void JahControl::CreateTrackerModule() 
{
    
    CreateModule();   
    
    ////////////////////////////////////////////////////////////////////////
    ModulesID[TRACKER] = module_counter;
    
    ////////////////////////////////////////////////////////////////////////
    // create the animation module/object
    
    JahTrackerMod = new GLTracker(core, "Tracker" , 
                                  ModulesOptions[module_counter],
                                  MainModuleControler[module_counter],
                                  MainModuleControlerLeft[module_counter], 
                                  MainModuleControlerRight[module_counter],
                                  &globalclipnumber  );
    
    module_counter++;
    
    ////////////////////////////////////////////////////////////////////////
    
    //without this windows boxes loose tracking in the anime module (only?)
    JahTrackerMod->setMouseTracking( false );
    
    //connect module to desktop framework
    connect ( JahTrackerMod, SIGNAL(updateDesktop(assetData)), SLOT( addImageToDesktop(assetData)) );
    connect ( JahTrackerMod, SIGNAL(GrabClip()),			   SLOT( GetDesktop()) );
    //connect ( JahTrackerMod, SIGNAL(GrabClipKey()),			   
    //	SLOT( GrabClipAnimKey()) );
    connect ( JahTrackerMod, SIGNAL(GrabDesktop()), SLOT( GetDesktop()) );
    
    connect ( JahTrackerMod,     SIGNAL(hideUI( bool )), SLOT( hidetheUI( bool ) ) );
    connect( JahTrackerMod,  SIGNAL( showInHead( JahHeadable * ) ),  this, SLOT( showInHead( JahHeadable * ))  );
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
    
    //JahLibraryMod = new FileMainFrame ( frame,"shareview");
    JahLibraryMod = new JahLibrary ( frame,"shareview",
                                     MainModuleControlerLeftFrame[module_counter], //the media bin frame
                                     MainModuleControlerRightFrame[module_counter], //the media bin frame
                                     ModulesOptions[module_counter],
                                     MainModuleControler[module_counter],
                                     MainModuleControlerLeft[module_counter], 
                                     MainModuleControlerRight[module_counter] );
    
    module_counter++;
    
    ////////////////////////////////////////////////////////////////////////
    
    //this is where we set the share folder
    JahLibraryMod->setDirectory( JahMediaPath+"media/" );
    
    connect( JahLibraryMod, SIGNAL(loadButton( )), this, SLOT(getClipFromLibrary( )) );
    
    //connect( JahLibraryMod, SIGNAL(importButton()), this, SLOT( slotLoadIconview()) );
    
    //connect( importclip, SIGNAL(clicked()), JahDesktopMod, SLOT( slotLoadIconview()) );
}

