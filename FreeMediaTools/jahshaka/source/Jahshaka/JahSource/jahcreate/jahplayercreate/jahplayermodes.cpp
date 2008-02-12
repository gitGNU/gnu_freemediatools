/*******************************************************************************
**
** The source file for the Jahshaka Jahroutines module
** The Jahshaka Project
** Copyright (C) 2000-2004 The Jahshaka Project
** Released under the GNU General Public License
**
*******************************************************************************/


#include "jahplayercreate.h"
//#include "effect.h"
//#include "paint.h"
//#include "character.h"
#include "anime.h"
//#include "edit.h"
//#include "tracker.h"
#include <glcore.h>

//////////////////////////////////////////////////////////
// control of individual modules starts here

void JahControl::changeModeDesktop()
{
	//check to see if we are switching modules
    if (module != DESKTOP)	stopAnimation();
        else saveSplitterValues();

    module = DESKTOP;

    //GetButton->hide();

		raiseModule(DESKTOP);
		WorldLayout->raiseWidget(desktopdisplay); 

	//in case they were in prefs...
//	changeModePrefReturn();

    JahDesktopMod->setFocus();
}
void JahControl::changeModeDesktopTable()
{
	//check to see if we are switching modules
    if (module != DESKTOP)	stopAnimation();
        else saveSplitterValues();

    module = DESKTOP;

    //GetButton->hide();

		raiseModule(DESKTOP);
		WorldLayout->raiseWidget(desktopdisplay); 

	//in case they were in prefs...
//	changeModePrefReturn();

    JahDesktopMod->toggleDesktopTable();
    JahDesktopMod->setFocus();
}

///////////////////////////////////////////////////////////
// for desktop preferences switching

void JahControl::changeModePref()
{

	prefsdialog->show();

}

///////////////////////////////////////////////////////////
// for all other module switching

void JahControl::changeModePlayer()
{

    bool clipselected = false;

	core->switchAutoBufferSwap( true );

    assetData theclip = JahDesktopMod->getVideoData(clipselected);

    if (clipselected)
    {
		JahPlayerMod->activate( );

        if (module != PLAYER) stopAnimation();
            else saveSplitterValues();

        module = PLAYER;  
       //we should also add format and resolution

			raiseModule(PLAYER);
			WorldLayout->raiseWidget(playerdisplay);  

        JahPlayerMod->LoadMySequence(theclip);

        JahPlayerMod->setFocus();
    }
}

void JahControl::changeModeAnimation() 
{
	JahAnimationMod->activate( );

    //this stops playback in the other modules so we arent eating resources
    if (module != ANIMATION) stopAnimation(); 
        else  saveSplitterValues();

	core->switchAutoBufferSwap( true );

    module = ANIMATION;      
 
	raiseModule(module);

    WorldLayout->raiseWidget(theCoreLayout);  

    connect(core, SIGNAL(paintit()),				  JahAnimationMod, SLOT(paintGL()) );
    connect(core, SIGNAL(mousePress(QMouseEvent*)),   JahAnimationMod, SLOT(mousePressEvent(QMouseEvent*)) );
    connect(core, SIGNAL(mouseRelease(QMouseEvent*)), JahAnimationMod, SLOT(mouseReleaseEvent(QMouseEvent*)) );
    connect(core, SIGNAL(mouseMove(QMouseEvent*)),    JahAnimationMod, SLOT(mouseMoveEvent(QMouseEvent*)) );
	
#ifndef JAHPLAYER // For some reason this wasn't compiling for jahplayer: JPN 08-03-2006
	core->useimagesize =  JahAnimationMod->videoRenderStatus;
#endif
    
	JahAnimationMod->raiseCore();

}



//////////////////////////////////////////////////
//library module
void JahControl::changeModeLibrary()
{
	JahLibraryMod->activate( );

    if (module != LIBRARY) stopAnimation();
       else   saveSplitterValues();


	core->switchAutoBufferSwap( true );

	module = LIBRARY;

	//if a clip was selected let the library know...
    bool clipselected = false;
    assetData theclip = JahDesktopMod->getVideoData(clipselected);

    if (clipselected)
    {

	}

		raiseModule(LIBRARY);
		WorldLayout->raiseWidget(librarydisplay);  
}


