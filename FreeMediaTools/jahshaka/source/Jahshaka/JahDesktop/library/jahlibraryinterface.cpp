/*******************************************************************************
**
** The source file for the Jahshaka core file file
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "jahlibrary.h"


void JahLibrary::buildInterfaceMenus( QHBox*, QHBox* ) 
{

}


void JahLibrary::buildInterface( QHBox* qtinterface ) 
{
#ifndef JAHPLAYER
    librarypanel = new QFrame( qtinterface, "desktoppanel" );
    librarypanel->setGeometry( QRect( 10, 10, 850, 190 ) );
    librarypanel->setFrameShape( QFrame::NoFrame );

    importclip = new QPushButton( librarypanel, "importclip" );
    importclip->setGeometry( QRect( 20, 110, 100, 25 ) );
    //importclip->setText( jt->tr( "Import Clip" ) );
    importclip->setText( "Import Clip" );
    connect( importclip, SIGNAL(clicked()), this, SLOT( clickedImportButton()) );

    exportclip = new QPushButton( librarypanel, "exportclip" );
    exportclip->setGeometry( QRect( 20, 140, 100, 25 ) );
    exportclip->setText( "Export Clip" );
#endif
}

void JahLibrary::createMenuItem( QPopupMenu * themenu )
{
	Q_CHECK_PTR(themenu); 
}
