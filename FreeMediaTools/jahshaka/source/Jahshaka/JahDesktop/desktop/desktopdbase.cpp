/*******************************************************************************
**
** The source file for the Jahshaka desktop database file
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/


#include "desktop.h"
#include <jahdatabase.h>
#include "imageitem.h"

void JahDesktop::addAssetTodatabase(assetData thedata )
{
    theDatabase->addImage2database(thedata);
}

void JahDesktop::renameImageindatabase( int imagenumber, QString newname)
{
    theDatabase->renameImageindatabase( imagenumber,newname);
}

void JahDesktop::updateClipPosition( assetData data)
{
    theDatabase->updateClipPosition( data );
}

void JahDesktop::updateClipSize( assetData data)
{
    theDatabase->updateClipSize( data );
}

void JahDesktop::updateLockStatus(assetData data)
{
	theDatabase->updateLockStatus(data);
}

void JahDesktop::updateFilmstripStatus(assetData data)
{
	theDatabase->updateFilmstripStatus(data);
}

void JahDesktop::slotUpdateFrameOffset(assetData data)
{
	theDatabase->updateFrameOffset(data);
}



//this is the main startup routine that rebuilds the desktop
//from the database by reading in all assets
//break this out into separate routines to make it easier to deal with...
//make this return bool for success to trap errors

void JahDesktop::RebuildDesktop()
{
    jtrace->info("Loading assets onto the desktop");

    int numclip = theDatabase->TestDatabase();

    if (numclip==0)
    {
        jtrace->debug("Database is empty");
        return;
    }

	if ( !JahPrefs::getInstance().getQuickStart( ) )
		emit addProgressSteps( numclip );

    /////////////////////////////////////////////////
    //loop through number of clips in videodatatable and add each one to the desktop
    //we need to update image locations for all clips when a clip is added and when we are leaving the app
    //using the sql:update command

    jtrace->info(">>Loading Assets from Database");

    for (int x=1; x<=numclip; x++)
    {
        //jtrace->debug(">>Loading Asset ",x);

        //grabs the asset, if its not there we get a -1 in theCategory
        assetData thedata = theDatabase->restoreImageFromDatabase(x);

        if ( (thedata.theCategory >=0) && (thedata.theCategory <=5) )
        {
			assetExchange exchange;
			QImage theimage = exchange.getImage( thedata, thedata.startframe + thedata.frameOffset );

            QString clipname = thedata.clipname;
            QString filename = thedata.filename;

            //add to table
            jtrace->info("Adding asset:", thedata.location + thedata.filename + thedata.extension);
            addItemToDesktopTable( x-1,  thedata, theimage );

			ImageItem* pImageItem = NULL;
            //a lousy way to check if the clip has been renamed
            if (clipname == filename)
                pImageItem = addImage2(thedata, theimage);
            else
                pImageItem = addImage(thedata, theimage); //wassup here?

			// Set the current frame of clip;
			pImageItem->setCurrentFrameNumber(thedata.startframe + thedata.frameOffset);
			editor->swapItem(pImageItem);

			if(thedata.filmstrip)
			{
				editor->createFilmStrip(qApp->desktop()->width()); // Would normally use editor->visibleWidth()
				                                                   // but at this point in time it ain't set
				                                                   // to anything sensible
			}
        }

		if ( !JahPrefs::getInstance().getQuickStart( ) )
			emit updateProgress( );

	    if(qApp->hasPendingEvents())
      		qApp->processEvents();
    }

    jtrace->debug(">> finished loading Assets from Database");

    //system crashes if correct number of clips in videodata is not specified
    //we need to update global clipnumber
    *clipnumber = numclip;
}

