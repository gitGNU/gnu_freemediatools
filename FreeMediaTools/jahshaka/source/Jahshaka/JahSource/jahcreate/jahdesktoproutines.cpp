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
#include "jahtranslate.h"

//for the encoder module
#include "MainEncoder.h"


//called when you clear the desktop as this wipes the database
//and needs to reset the global clip number to zero
void JahControl::resetGlobalClipNumber()
{
    jtrace->debug( "JahControl::resetGlobalClipNumber()",
                   "resetting global clipnumber" );

    globalclipnumber=0;

}

//called when you load a asset in
//this visually adds it to all necessary places
void JahControl::addImageToDesktop( assetData thedata )
{
    JahDesktopMod->loadJahAsset( thedata.getFullFileName( ), true );
}

////////////////////////////////////////////////////////////////////////////
// used in the network p2p module to grab clips from the p2p share
// thedata is the path+filename of the clip
// and also by the desktop library module

void JahControl::addStringToDesktop( QString thedata )
{
    //need to call the table function when we load clips in the desktop module
    JahDesktopMod->loadJahAsset(thedata);

}

//used to grab a file from the library module
//and add it to the desktop
void JahControl::updateDesktopLabel(QString newlabel)
{
    m_desktopFeedbackLabel->setText( jt->tr( newlabel ) );
}


////////////////////////////////////////////////////////////////////////////
// used for drag and drop
// from the desktop to any module
void JahControl::draggedAImage( )
{
    //first see if a layer is selected in the calling module
    //all we really do here is a return desktop
    //this should be cleaned up to avoid flicker...
    ReturnDesktop();
}

//checks the return button on the desktops status
void JahControl::checkDesktopButton() 
{
    if ( desktopButtonStatus )
    {
        ReturnButton->hide();
        // desktopselectbutton->show();
    }
}







