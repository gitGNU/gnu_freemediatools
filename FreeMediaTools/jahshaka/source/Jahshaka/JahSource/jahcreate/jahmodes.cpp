/*******************************************************************************
**
** The source file for the Jahshaka Jahroutines module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/


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
#include "networking.h"
#include "keyer.h"

/*
-I. 
-I../../../../../openlibraries/trunk/src 
-I../../../AuxiliaryLibraries/FTGL 
-I../../../AuxiliaryLibraries/apollon 
-I../../../AuxiliaryLibraries/blur 
-I../../../AuxiliaryLibraries/gift 
-I../../../AuxiliaryLibraries/glew 
-I../../../AuxiliaryLibraries/particle 
-I../../../AuxiliaryLibraries/spaceball 
-I../../../AuxiliaryLibraries/sqlite 
-I../../../OpenLibraries/openassetlib 
-I../../../OpenLibraries/openassetlib/v2_openassetlib/src 
-I../../../OpenLibraries/opencore 
-I../../../OpenLibraries/opengpulib 
-I../../../OpenLibraries/openimagelib 
-I../../../OpenLibraries/openmedialib 
-I../../../OpenLibraries/openmedialib/mediaobject 
-I../../../OpenLibraries/opennetworklib 
-I../../../OpenLibraries/openobjectlib 
-I../../../OpenLibraries/openobjectlib/surface3d 
-I../../JahCore/jahobjects 
-I../../JahCore/jahrender 
-I../../JahCore/jahworld 
-I../../JahDesktop/desktop 
-I../../JahDesktop/encoder 
-I../../JahDesktop/library 
-I../../JahDesktop/network 
-I../../JahDesktop/player 
-I../../JahDesktop/videoio 
-I../../JahLibraries 
-I../../JahLibraries/jahdatabase 
-I../../JahLibraries/jahdataio 
-I../../JahLibraries/jahformatter 
-I../../JahLibraries/jahglcore 
-I../../JahLibraries/jahkeyframes 
-I../../JahLibraries/jahplayer 
-I../../JahLibraries/jahplayer/audioplayer 
-I../../JahLibraries/jahplayer/diskplayer 
-I../../JahLibraries/jahplayer/ramplayer 
-I../../JahLibraries/jahplugins 
-I../../JahLibraries/jahpreferences 
-I../../JahLibraries/jahthemes 
-I../../JahLibraries/jahtimer 
-I../../JahLibraries/jahtracer 
-I../../JahLibraries/jahtranslate 
-I../../JahModules/animation 
-I../../JahModules/colorize 
-I../../JahModules/editing 
-I../../JahModules/effect 
-I../../JahModules/keyer 
-I../../JahModules/painter 
-I../../JahModules/text 
-I../../JahModules/tracker 
-I../../JahSource/jahcreate 
-I../../JahSource/jahmain 
-I../../JahSource/jahsplash 
-I../../JahWidgets/calc 
-I../../JahWidgets/colortri 
-I../../JahWidgets/interfaceobjs 
-I../../JahWidgets/jahfileloader 
-I../../JahWidgets/keyframes 
-I../../JahWidgets/mediatable 
-I../../JahWidgets/nodes 
-I../../JahWidgets/timeline 
-I../../JahWidgets/waveform 
-I../../JahWidgets/wireup 
-I/usr/X11R6/include 
-I/usr/include 
-I/usr/include/freetype2 
-I/usr/include/qt3 
-I/usr/local/include 
-I/usr/share/qt3/mkspecs/default 
*/
//./source/Jahshaka/JahDesktop/encoder/MainEncoder.h
#include "MainEncoder.h"

//////////////////////////////////////////////////////////
// control of individual modules starts here

void JahControl::changeModeDesktop()
{
    JahDesktopMod->activate( );
    
    //check to see if we are switching modules
    if (module != DESKTOP)	
        stopAnimation();
    else 
        saveSplitterValues();

    wideframeLeft->show( );
    wideframeRight->hide( );
    
    module = DESKTOP;
    updateDesktopLabel( "MediaDesktop" );

    checkDesktopButton();

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

	//in case they were in prefs...
	changeModePrefReturn();

    JahDesktopMod->setFocus();
}

///////////////////////////////////////////////////////////
// for desktop preferences switching

void JahControl::changeModePref()
{
	theDesktopLayout->raiseWidget(1);
}

void JahControl::changeModePrefReturn()
{
	theDesktopLayout->raiseWidget(0);  
}

///////////////////////////////////////////////////////////
// for all other module switching

void JahControl::changeModePlayer()
{
    JahPlayerMod->activate( );
    setModulesRenderQuality( );
    
    bool clipselected = false;
    
    core->switchAutoBufferSwap( true );
    
    assetData theclip = JahDesktopMod->getVideoData(clipselected);
    
    if (clipselected)
    {
        if (module != PLAYER) 
            stopAnimation();
        else 
            saveSplitterValues();

        wideframeLeft->show( );
        wideframeRight->show( );
        
        module = PLAYER;  
        QString name = "MediaPlayer";
        
        //name = name.append(JahDesktopMod->getClipname());       //add clip name to title text
        updateDesktopLabel( name );                        //we should also add format and resolution
        
        if (dualhead)
        {
            raiseD2Module(PLAYER);
            D2WorldLayout->raiseWidget(playerdisplay);  
        }
        else
        {
            raiseModule(PLAYER);
            WorldLayout->raiseWidget(playerdisplay);  
        }

		wideframeLeft->show( );
		wideframeRight->show( );

        JahPlayerMod->LoadMySequence(theclip);

        JahPlayerMod->setFocus();
    }
}

void JahControl::changeModeAnimation() 
{
	JahAnimationMod->activate( );
	setModulesRenderQuality( );

    //this stops playback in the other modules so we arent eating resources
    if (module != ANIMATION) 
        stopAnimation(); 
    else  
        saveSplitterValues();

    wideframeLeft->show( );
    wideframeRight->hide( );
    
    core->switchAutoBufferSwap( true );
    
    module = ANIMATION;      
    updateDesktopLabel( "Animation" );
    
    checkDesktopButton();
    
    raiseModule(module);
    
    WorldLayout->raiseWidget(theCoreLayout);  

    connect(core, SIGNAL(paintit()),				  JahAnimationMod, SLOT(paintGL()) );
    connect(core, SIGNAL(mousePress(QMouseEvent*)),   JahAnimationMod, SLOT(mousePressEvent(QMouseEvent*)) );
    connect(core, SIGNAL(mouseRelease(QMouseEvent*)), JahAnimationMod, SLOT(mouseReleaseEvent(QMouseEvent*)) );
    connect(core, SIGNAL(mouseMove(QMouseEvent*)),    JahAnimationMod, SLOT(mouseMoveEvent(QMouseEvent*)) );
	
    core->useimagesize =  JahAnimationMod->getRenderAtImageResolution();
    
    JahAnimationMod->raiseCore();

    if ( !Globals::getEnableScriptingButton() )
    {
        JahAnimationMod->getRunScriptButton()->hide();
    }
}

void JahControl::changeModeEffects() 
{   
	JahEffectsMod->activate( );
	setModulesRenderQuality( );

    //this stops playback in the other modules so we arent eating resources
    if (module != EFFECTS) stopAnimation(); 
        else saveSplitterValues();

	wideframeLeft->show( );
	wideframeRight->show( );

	core->switchAutoBufferSwap( true );

    module = EFFECTS;  
    updateDesktopLabel( "Compositing" );
  
    checkDesktopButton();

	raiseModule(module);

    WorldLayout->raiseWidget(theCoreLayout);  

    connect(core, SIGNAL(paintit()), JahEffectsMod,   SLOT(paintGL()) );
    connect(core, SIGNAL(mousePress(QMouseEvent*)),   JahEffectsMod, SLOT(mousePressEvent(QMouseEvent*)) );
    connect(core, SIGNAL(mouseRelease(QMouseEvent*)), JahEffectsMod, SLOT(mouseReleaseEvent(QMouseEvent*)) );
    connect(core, SIGNAL(mouseMove(QMouseEvent*)),    JahEffectsMod, SLOT(mouseMoveEvent(QMouseEvent*)) );
	
	core->useimagesize =  JahEffectsMod->getRenderAtImageResolution();

    JahEffectsMod->raiseCore();
}

void JahControl::changeModeEditing() 
{
	JahEditingMod->activate( );
	setModulesRenderQuality( );

    //this stops playback in the other modules so we arent eating resources
    if (module != EDITING) stopAnimation(); 
        else   saveSplitterValues();

	wideframeLeft->show( );
	wideframeRight->show( );

	core->switchAutoBufferSwap( true );

    module = EDITING; 
    updateDesktopLabel( "Editing" );
   
    checkDesktopButton();

	raiseModule(module);
    WorldLayout->raiseWidget(theCoreLayout);  

    connect(core, SIGNAL(paintit()), JahEditingMod,   SLOT(paintGL()) );
    connect(core, SIGNAL(mousePress(QMouseEvent*)),   JahEditingMod, SLOT(mousePressEvent(QMouseEvent*)) );
    connect(core, SIGNAL(mouseRelease(QMouseEvent*)), JahEditingMod, SLOT(mouseReleaseEvent(QMouseEvent*)) );
    connect(core, SIGNAL(mouseMove(QMouseEvent*)),    JahEditingMod, SLOT(mouseMoveEvent(QMouseEvent*)) );
	
	core->useimagesize =  JahEditingMod->getRenderAtImageResolution();

    JahEditingMod->raiseCore();
    JahEditingMod->glworldUpdate(); // TODO : missing function
}

void JahControl::changeModePaint() 
{
    //JahPaintMod->resizePaintGL();
	JahPaintMod->activate( );
	setModulesRenderQuality( );

    //this stops playback in the other modules so we arent eating resources
    if (module != PAINT) stopAnimation(); 
        else   saveSplitterValues();

	wideframeLeft->show( );
	wideframeRight->hide( );
	//paintcore->switchAutoBufferSwap( true );

    module = PAINT;  
    updateDesktopLabel( "Paint" );
   
    checkDesktopButton();

	raiseModule(PAINT);

    disconnect(paintcore, SIGNAL(paintit()), JahPaintMod,     SLOT(paintGL()) );
    disconnect(paintcore, SIGNAL(mousePress(QMouseEvent*)),   JahPaintMod, SLOT(mousePressEvent(QMouseEvent*)) );
    disconnect(paintcore, SIGNAL(mouseRelease(QMouseEvent*)), JahPaintMod, SLOT(mouseReleaseEvent(QMouseEvent*)) );
    disconnect(paintcore, SIGNAL(mouseMove(QMouseEvent*)),    JahPaintMod, SLOT(mouseMoveEvent(QMouseEvent*)) );

    connect(paintcore, SIGNAL(paintit()), JahPaintMod,     SLOT(paintGL()) );
    connect(paintcore, SIGNAL(mousePress(QMouseEvent*)),   JahPaintMod, SLOT(mousePressEvent(QMouseEvent*)) );
    connect(paintcore, SIGNAL(mouseRelease(QMouseEvent*)), JahPaintMod, SLOT(mouseReleaseEvent(QMouseEvent*)) );
    connect(paintcore, SIGNAL(mouseMove(QMouseEvent*)),    JahPaintMod, SLOT(mouseMoveEvent(QMouseEvent*)) );
	
	//paintcore->useimagesize =  JahPaintMod->getRenderAtImageResolution();
	
	WorldLayout->raiseWidget(theCoreLayout);

    //JahPaintMod->resizePaintGL();
	//theCoreLayout->raiseWidget(1);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void JahControl::changeModeText() 
{
	JahTextMod->activate( );
	setModulesRenderQuality( );

	if (module != TEXTCG) stopAnimation(); 
        else   saveSplitterValues();

	wideframeLeft->show( );
	wideframeRight->hide( );

	core->switchAutoBufferSwap( true );

    module = TEXTCG;
    updateDesktopLabel( "Text CG" );
 
    checkDesktopButton();

	raiseModule(module);
    WorldLayout->raiseWidget(theCoreLayout);  

    connect(core, SIGNAL(paintit()), JahTextMod,      SLOT(paintGL()) );
    connect(core, SIGNAL(mousePress(QMouseEvent*)),   JahTextMod, SLOT(mousePressEvent(QMouseEvent*)) );
    connect(core, SIGNAL(mouseRelease(QMouseEvent*)), JahTextMod, SLOT(mouseReleaseEvent(QMouseEvent*)) );
    connect(core, SIGNAL(mouseMove(QMouseEvent*)),    JahTextMod, SLOT(mouseMoveEvent(QMouseEvent*)) );
	
	core->useimagesize =  JahTextMod->getRenderAtImageResolution();

    JahTextMod->raiseCore();
}

void JahControl::changeModeColorize() 
{
	JahColorMod->activate( );
	setModulesRenderQuality( );

    if (module != COLORIZE) stopAnimation(); 
        else   saveSplitterValues();

	wideframeLeft->show( );
	wideframeRight->hide( );
   
	core->switchAutoBufferSwap( true );

    module = COLORIZE;    
    updateDesktopLabel( "Colorize" );

    checkDesktopButton();

	raiseModule(module);
    WorldLayout->raiseWidget(theCoreLayout);  

    connect(core, SIGNAL(paintit()), JahColorMod,     SLOT(paintGL()) );
    connect(core, SIGNAL(mousePress(QMouseEvent*)),   JahColorMod, SLOT(mousePressEvent(QMouseEvent*)) );
    connect(core, SIGNAL(mouseRelease(QMouseEvent*)), JahColorMod, SLOT(mouseReleaseEvent(QMouseEvent*)) );
    connect(core, SIGNAL(mouseMove(QMouseEvent*)),    JahColorMod, SLOT(mouseMoveEvent(QMouseEvent*)) );

	core->useimagesize =  JahColorMod->getRenderAtImageResolution();

    JahColorMod->raiseCore();
}

void JahControl::changeModeTracker() 
{
	JahTrackerMod->activate( );
	setModulesRenderQuality( );

    //this stops playback in the other modules so we arent eating resources
    if (module != TRACKER) stopAnimation(); 
        else   saveSplitterValues();

	wideframeLeft->show( );
	wideframeRight->hide( );

	core->switchAutoBufferSwap( true );

    module = TRACKER;      
    updateDesktopLabel( "Tracker" );
 
    checkDesktopButton();

	raiseModule(module);
    WorldLayout->raiseWidget(theCoreLayout);  

    connect(core, SIGNAL(paintit()), JahTrackerMod,   SLOT(paintGL()) );
    connect(core, SIGNAL(mousePress(QMouseEvent*)),   JahTrackerMod, SLOT(mousePressEvent(QMouseEvent*)) );
    connect(core, SIGNAL(mouseRelease(QMouseEvent*)), JahTrackerMod, SLOT(mouseReleaseEvent(QMouseEvent*)) );
    connect(core, SIGNAL(mouseMove(QMouseEvent*)),    JahTrackerMod, SLOT(mouseMoveEvent(QMouseEvent*)) );

	core->useimagesize =  JahTrackerMod->getRenderAtImageResolution();

    JahTrackerMod->raiseCore();
}

void JahControl::changeModeKeyer() 
{   
	JahKeyerMod->activate( );
	setModulesRenderQuality( );

    //this stops playback in the other modules so we arent eating resources
    if (module != KEYER) stopAnimation(); 
        else   saveSplitterValues();

	wideframeLeft->show( );
	wideframeRight->hide( );

	core->switchAutoBufferSwap( true );

    module = KEYER;  
    updateDesktopLabel( "Keyer" );
  
    checkDesktopButton();

	raiseModule(module);
    WorldLayout->raiseWidget(theCoreLayout);  

    connect(core, SIGNAL(paintit()), JahKeyerMod,     SLOT(paintGL()) );
    connect(core, SIGNAL(mousePress(QMouseEvent*)),   JahKeyerMod, SLOT(mousePressEvent(QMouseEvent*)) );
    connect(core, SIGNAL(mouseRelease(QMouseEvent*)), JahKeyerMod, SLOT(mouseReleaseEvent(QMouseEvent*)) );
    connect(core, SIGNAL(mouseMove(QMouseEvent*)),    JahKeyerMod, SLOT(mouseMoveEvent(QMouseEvent*)) );

	core->useimagesize =  JahKeyerMod->getRenderAtImageResolution();

    JahKeyerMod->raiseCore();
}

/////////////////////////////////////////////////
// for the encoder
 
void JahControl::changeModeEncoding()
{
	JahEncoderMod->activate( );
	setModulesRenderQuality( );

    bool clipselected = false;

	core->switchAutoBufferSwap( true );

    assetData theclip = JahDesktopMod->getVideoData(clipselected);

    if (clipselected)
    {
        if (module != ENCODING) stopAnimation();
            else   saveSplitterValues();

		wideframeLeft->show( );
		wideframeRight->hide( );

        module = ENCODING;

        QString name = "Encoding";

        //name = name.append(JahDesktopMod->getClipname());       //add clip name to title text
        updateDesktopLabel( name );                        //we should also add format and resolution

        if (dualhead)
        {
			//qDebug("doing dualhead");
			raiseD2Module(ENCODING);
			D2WorldLayout->raiseWidget(encodingdisplay);  
        }
        else
        {
			//qDebug("raising encoder");
			raiseModule(ENCODING);
			WorldLayout->raiseWidget(encodingdisplay);  
        }

        JahEncoderMod->LoadMySequence(theclip);

        JahEncoderMod->setFocus();
    }
}

//////////////////////////////////////////////////
//library module
void JahControl::changeModeLibrary()
{
	JahLibraryMod->activate( );
	setModulesRenderQuality( );

    if (module != LIBRARY) stopAnimation();
	else saveSplitterValues();

	wideframeLeft->show( );
	wideframeRight->show( );

	core->switchAutoBufferSwap( true );

	module = LIBRARY;

    QString name = "Library";
    updateDesktopLabel( name );                        //we should also add format and resolution

	//if a clip was selected let the library know...
    bool clipselected = false;
    assetData theclip = JahDesktopMod->getVideoData(clipselected);

    if (dualhead)
    {
		raiseD2Module(LIBRARY);
		D2WorldLayout->raiseWidget(librarydisplay);  
    }
    else
    {
		raiseModule(LIBRARY);
		WorldLayout->raiseWidget(librarydisplay);  
    }
}

//////////////////////////////////////////////////
//network modules routines
#ifdef USE_NETWORK
void JahControl::changeModeNetworkCom()
{
	JahNetworkMod->activate( );
	setModulesRenderQuality( );

	core->switchAutoBufferSwap( true );

	wideframeLeft->hide( );
	wideframeRight->hide( );

    module = NETWORK;

    //this one is a hybrid since it uses the desktop in the main frame
    //and the comm tools in the subframe
    if (dualhead)
    {
        raiseD2Module(NETWORK);

        D2MainControler->raiseWidget(ModulesID[DESKTOP]);
        D2WorldLayout->raiseWidget(desktopdisplay);  
    }
    else
    {
        raiseModule(NETWORK);

        MainControler->raiseWidget(ModulesID[DESKTOP]);
        WorldLayout->raiseWidget(desktopdisplay);  
    }

    JahNetworkMod->setFocus();
}

void JahControl::changeModeNetworkP2P()
{
	JahNetworkMod->activate( );
	setModulesRenderQuality( );

	core->switchAutoBufferSwap( true );

	wideframeLeft->hide( );
	wideframeRight->hide( );


    module = NETWORK;
    if (dualhead)
    {
        raiseD2Module(NETWORK);
        D2WorldLayout->raiseWidget(thenetworkdisplay);  
    }
    else
    {
        raiseModule(NETWORK);
        WorldLayout->raiseWidget(thenetworkdisplay);  
    }

    JahNetworkMod->setFocus();

}

void JahControl::changeModeNetworkNet()
{
	JahNetworkMod->activate( );
	setModulesRenderQuality( );

	core->switchAutoBufferSwap( true );

	wideframeLeft->hide( );
	wideframeRight->hide( );

    module = NETWORK;

    if (dualhead)
    {
        raiseD2Module(NETWORK);
        D2WorldLayout->raiseWidget(thenetworkdisplay);  
    }
    else
    {
        raiseModule(NETWORK);
        WorldLayout->raiseWidget(thenetworkdisplay);  
    }

    JahNetworkMod->setFocus();
}

void JahControl::changeModeNetworkHttpd()
{
	JahNetworkMod->activate( );
	setModulesRenderQuality( );

	core->switchAutoBufferSwap( true );

	wideframeLeft->hide( );
	wideframeRight->hide( );

    module = NETWORK;

    //this one is a hybrid since it uses the desktop in the main frame
    //and the comm tools in the subframe
    if (dualhead)
    {
        raiseD2Module(NETWORK);

        D2MainControler->raiseWidget(ModulesID[DESKTOP]);
        D2WorldLayout->raiseWidget(desktopdisplay);
    }
    else
    {
        raiseModule(NETWORK);

        MainControler->raiseWidget(ModulesID[DESKTOP]);
        WorldLayout->raiseWidget(desktopdisplay);
    }

    JahNetworkMod->setFocus();
}
#endif




void JahControl::changeModeVideoIO()
{
    //module = VIDEOIO;
}

void JahControl::changeModeBroadcasting()
{
    //module = BROADCASTING;
}
