/*******************************************************************************
**
** The source file for the Jahshaka animation interface module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "desktop.h"
#include <qhbox.h>
#include <qframe.h>
#include <qpopupmenu.h>
#include <qlayout.h>
#include "jahtranslate.h"
#include "jahformatter.h"
#include "widget.h"

void JahDesktop::buildInterfaceMenus( QHBox* , QHBox* rightcontroller) 
{
	// Container widget
	QWidget *container = new QWidget( rightcontroller, "page" );
	QVBoxLayout *container_layout = new QVBoxLayout( container, 0, 0, "container_layout");

	container_layout->addStretch( );

    //we are only using these 2 for now but we may use the others later on
    toolthumbview = new JahToolButton( container, "thumbview" );
    JahFormatter::addJahPlayerButton( toolthumbview, JahBasePath+"Pixmaps/desktop/thumbview.png", JahBasePath+"Pixmaps/desktop/thumbview.png" );
    container_layout->addWidget( toolthumbview );

    toolbinview = new JahToolButton( container, "binview" );
    JahFormatter::addJahPlayerButton( toolbinview, JahBasePath+"Pixmaps/desktop/binview.png", JahBasePath+"Pixmaps/desktop/binview.png" );
    container_layout->addWidget( toolbinview );

    //connect tools to object and add object to display widget
    connect( toolthumbview,   SIGNAL(clicked()), this, SLOT(toggleDesktopImages() )  );
    connect( toolbinview,     SIGNAL(clicked()), this, SLOT(toggleDesktopTable() )  );

	container_layout->addStretch( );
}

void JahDesktop::createMenuItem( QPopupMenu * themenu )
{
    Q_CHECK_PTR(themenu);
    themenu->insertItem("&"+jt->tr("Load clip"), this, SLOT(slotLoadIconview()) );
    themenu->insertItem(jt->tr("Delete clip"), this, SLOT(clearItem()) );
    themenu->insertSeparator();
    themenu->insertItem(jt->tr("Play clip"), this, SLOT(changeModePlayer()) );
    themenu->insertSeparator();
    themenu->insertItem("&"+jt->tr("Tidy desktop"), this, SLOT(tidyDesktop()) );
    themenu->insertItem("&"+jt->tr("Clear desktop"), this, SLOT(clearAll()) );
}

void JahDesktop::changeModePlayer( )
{
    switchMainModule( "PLAYER" );
}

