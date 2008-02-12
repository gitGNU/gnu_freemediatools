/*******************************************************************************
**
** The source file for the Jahshaka Jahroutines module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

// These are all the routines used by the jahcontrol class
//this file can be broken into module switching and utility routines

#include "jahcreate.h"
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

//for the encoder module
#include "MainEncoder.h"

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

    // desktopselectbutton->hide();
    ReturnButton->show();
								
    if (dualheadprefs)
    {
	    D2WorldLayout->raiseWidget(desktopdisplay);  
        D2WorldLayout->raiseWidget(DESKTOP);
        D2MainControler->raiseWidget(DESKTOP);
		desktopdisplay->setFocus( );
    }
    else
    {
		WorldLayout->raiseWidget(desktopdisplay); 
        WorldLayout->raiseWidget(DESKTOP);
        MainControler->raiseWidget(DESKTOP);
		desktopdisplay->setFocus( );
    }

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
    
	checkDesktopButton();

    //switch on module for the return  
	//jtrace->debug( "JahControl","returnDesktop "+getModuleName(module) );
      
	switch(module) {

         case ANIMATION :  { 
                             
                             //we can also return this as a key if necessary
                             //multi select on the desktop would return both the clip and the key
                             //but this needs to be implemented first
                             if (getKeyStatus)
                                GrabClipAnimKey();
                             else
                                GrabClipAnim();

                             WorldLayout->raiseWidget(theCoreLayout);
							 theCoreLayout->raiseWidget(0);
				 			 break; 
						   }

         case EFFECTS   :  { 
							 GrabClipEffects();
                             WorldLayout->raiseWidget(theCoreLayout);  
				 			 break; 
						   }

         case EDITING   :  { 
                             if (editReturnClip) 
                             {
                                ReturnClipEdit(); 
                             }
                             else 
                             {
                                GrabClipEdit();
                             }
                                                          
                             WorldLayout->raiseWidget(theCoreLayout);  
				 			 break; 
                           }


        case TEXTCG     :  { 
                             GrabClipCG();                            
                             WorldLayout->raiseWidget(theCoreLayout); 
							 theCoreLayout->raiseWidget(0);
				 			 break; 
                           }

        case PAINT      :  { 
                             GrabClipPaint();                             
							 WorldLayout->raiseWidget(theCoreLayout); 
							 theCoreLayout->raiseWidget(1);
                             JahPaintMod->setNeedsClear(true);
							 break; 
                           }

        case COLORIZE   :  { 
							 GrabClipColorize();                             
                             WorldLayout->raiseWidget(theCoreLayout);  
				 			 break; 
                           }

         case KEYER     :  { 
							 GrabClipKeyer();                             
                             WorldLayout->raiseWidget(theCoreLayout);  
				 			 break; 
						   }
         
		 case TRACKER   :  { 
							 GrabClipTracker();                             
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

    saveSplitterValues();
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

void JahControl::GrabClipEffects() 
{    
    bool clipstatus = false;
    assetData newclip = JahDesktopMod->getVideoData(clipstatus);

    if(clipstatus)
        JahEffectsMod->addClip(newclip);
}

///////////////////////////////////////////////////////////////////
//the editor needs to return a clip on add so its different...
//future drag and drop implementation here
void JahControl::AddClipEdit()     
{

    editReturnClip=TRUE;
    GetDesktop();
}

void JahControl::GrabClipEdit()    
{
    bool clipstatus = false;
    assetData newclip = JahDesktopMod->getVideoData(clipstatus);

    if (clipstatus)
        JahEditingMod->addClip(newclip);
}

void JahControl::ReturnClipEdit()  
{    

    bool clipstatus = false;
    assetData newclip = JahDesktopMod->getVideoData(clipstatus);

	editReturnClip=FALSE;

    if (clipstatus)
    {
		JahEditingMod->addClipEdit(newclip);
    }
}

/////////////////////////////////////////////////////////////////////

void JahControl::GrabClipAnimKey() 
{    
    bool clipstatus = false;
    assetData newclip = JahDesktopMod->getVideoData(clipstatus);

    if (clipstatus)
        JahAnimationMod->addClipKey(newclip);
}

void JahControl::GrabClipCG()      
{    
    bool clipstatus = false;
    assetData newclip = JahDesktopMod->getVideoData(clipstatus);

    if (clipstatus)
        JahTextMod->addClip(newclip);
}

void JahControl::GrabClipColorize()
{    
    bool clipstatus = false;
    assetData newclip = JahDesktopMod->getVideoData(clipstatus);

    if (clipstatus)
        JahColorMod->addClip(newclip);
}

void JahControl::GrabClipPaint()   
{    
    bool clipstatus = false;
    assetData newclip = JahDesktopMod->getVideoData(clipstatus);

    if (clipstatus)
        JahPaintMod->addClip(newclip);
}

void JahControl::GrabClipTracker()    
{
    bool clipstatus = false;
    assetData newclip = JahDesktopMod->getVideoData(clipstatus);

    if (clipstatus)
        JahTrackerMod->addClip(newclip);
}

void JahControl::GrabClipKeyer() 
{    
    bool clipstatus = false;
    assetData newclip = JahDesktopMod->getVideoData(clipstatus);

    if(clipstatus)
        JahKeyerMod->addClip(newclip);
}

void JahControl::GrabClipKeyerBase() 
{    
    bool clipstatus = false;
    assetData newclip = JahDesktopMod->getVideoData(clipstatus);

    if(clipstatus)
        JahKeyerMod->addBaseClip(newclip);
}
