/*******************************************************************************
**
** The source file for the Jahshaka Jahroutines module
** The Jahshaka Project
** Copyright (C) 2000-2004 The Jahshaka Project
** Released under the GNU General Public License
**
*******************************************************************************/

// These are all the routines used by the jahcontrol class
//this file can be broken into module switching and utility routines

#include "jahplayercreate.h"
//#include "effect.h"
//#include "paint.h"
//#include "character.h"
#include "anime.h"
//#include "edit.h"
//#include "tracker.h"

//wow

////////////////////////////////////////////////////////////
// routines to send clips from desktop to modules

void JahControl::GetDesktop() 
{
    if (module==DESKTOP) 
    {
        return;
    }

	//need to do this
	saveSplitterValues();

    desktopButtonStatus = TRUE;
    //GetButton->hide();
    //ReturnButton->show();
	/*							
    if (dualheadprefs)
    {
	    D2WorldLayout->raiseWidget(desktopdisplay);  
        D2WorldLayout->raiseWidget(DESKTOP);
        D2MainControler->raiseWidget(DESKTOP);
		desktopdisplay->setFocus( );
    }
    else
    {*/
		WorldLayout->raiseWidget(desktopdisplay); 
        WorldLayout->raiseWidget(DESKTOP);
        MainControler->raiseWidget(DESKTOP);
		desktopdisplay->setFocus( );
    //}

    //always reset this
    getKeyStatus = false;

}

void JahControl::GetDesktopKey() 
{
    if (module==DESKTOP) 
    {
        return;
    }

    GetDesktop();

    //toggle this
    getKeyStatus = true;

}

void JahControl::ReturnDesktop()
{    
    desktopButtonStatus = TRUE; 
    
	//checkDesktopButton();

    //switch on module for the return  
	//jtrace->debug( "JahControl","returnDesktop "+getModuleName(module) );
      
	switch(module) {

         case ANIMATION :  { 
                             
                             //we can also return this as a key if necessary
                             //multi select on the desktop would return both the clip and the key
                             //but this needs to be implemented first

                                GrabClipAnim();

                             WorldLayout->raiseWidget(theCoreLayout);  
				 			 break; 
						   }

        default  :         { 
							 module=DESKTOP;
							 break; 
						   }

      }

    //always reset this value for no confusion
    getKeyStatus = false;

    raiseModule(module);

}




//these are used to grab the current active desktop clip...
//need error checking on this ie what if newclip is blank?
void JahControl::GrabClipAnim()    
{
    bool clipstatus = false;
    assetData newclip = JahDesktopMod->getVideoData(clipstatus);

    if (clipstatus)
        JahAnimationMod->addClip(newclip);
}

