/*******************************************************************************
**
** The source file for the Jahshaka Jahroutines module
** The Jahshaka Project
** Copyright (C) 2000-2004 The Jahshaka Project
** Released under the GNU General Public License
**
*******************************************************************************/

#include "jahplayercreate.h"

// qt
#include <qvbox.h>

void JahControl::createJahUI(	QFrame *mainworld,		  QFrame *mainleftmenu,		QFrame *mainrightmenu,
								QFrame *maincontroller,	  QFrame *modulesettings)
{
    //////////////////////////////////////////////////////////
    //create the main controler for the world space object
    wideframeLayout = new QHBoxLayout( mainworld, 0, 0, "frame3Layout");

    //the splitter
    splitter2 = new QSplitter( mainworld, "splitter2" );
    splitter2->setOrientation( QSplitter::Horizontal );

    wideframeLeft = new QHBox( splitter2, "frame4" );
    wideframeLeft->setFrameShape( QFrame::StyledPanel );
    wideframeLeft->setFrameShadow( QFrame::Raised );

    // evil hack to avoid too much destruction of code
    QVBox* inner = new QVBox( splitter2 );

    wideframeCenter = new QHBox( inner, "frame5" );
    wideframeCenter->setFrameShape( QFrame::StyledPanel );
    wideframeCenter->setFrameShadow( QFrame::Raised );

    // evil hack to avoid too much destruction of code
    maincontroller->reparent( inner, QPoint(), true );

    wideframeRight = new QHBox( splitter2, "frame5" );
    wideframeRight->setFrameShape( QFrame::StyledPanel );
    wideframeRight->setFrameShadow( QFrame::Raised );

    wideframeLayout->addWidget( splitter2, 0, 0 );

	////////////////////////////////////////////////////////////
    //this will be for the desktop alone for widescrreen mode
	////////////////////////////////////////////////////////////

	//it is the left screen acces module
    mainworldLayoutLeft = new QHBoxLayout( wideframeLeft, 0, 0, "mainworldLayoutLeft");
    WorldLayoutLeft = new QWidgetStack(wideframeLeft);
    mainworldLayoutLeft->addWidget( WorldLayoutLeft );

	//this is for the main worldspace
    mainworldLayout = new QHBoxLayout( wideframeCenter, 0, 0, "mainworldLayoutCenter");
    WorldLayout = new QWidgetStack(wideframeCenter);
    mainworldLayout->addWidget( WorldLayout );

	//it is the right screen acces module
    mainworldLayoutRight = new QHBoxLayout( wideframeRight, 0, 0, "mainworldLayoutRight");
    WorldLayoutRight = new QWidgetStack(wideframeRight);
    mainworldLayoutRight->addWidget( WorldLayoutRight );
	
	
	//////////////////////////////////////////////////////////////////////////
	//also we need to remember size of left/right ui's for when the application restarts
	QValueList<int> sizeList;

	sizeList.append(0);
	sizeList.append(300);
	sizeList.append(0);
	splitter2->setSizes(sizeList);

    splitter2->setOpaqueResize(true); 

	/////////////////////////////////////////////////////////////
	// this builds the controllers and menus for the rest of the ui

    //////////////////////////////////////////////////////////////
    //create the main left menu controler
    mainworldLeftLayout = new QHBoxLayout( mainleftmenu, 0, 0, "mainworldLeftLayout");
    MainLeftControler = new QWidgetStack(mainleftmenu);
    mainworldLeftLayout->addWidget( MainLeftControler );

    //////////////////////////////////////////////////////////////
    //create the main right menu controler
    mainworldRightLayout = new QHBoxLayout( mainrightmenu, 0, 0, "mainworldRightLayout");
    MainRightControler = new QWidgetStack(mainrightmenu);
    mainworldRightLayout->addWidget( MainRightControler );

    //////////////////////////////////////////////////////////////
    //create the middle frame menu/option controler
    maincontrollerLayout = new QHBoxLayout( maincontroller, 0, 0, "maincontrollerLayout");
    MainControler = new QWidgetStack(maincontroller);
    maincontrollerLayout->addWidget( MainControler );

}

void JahControl::splitterChanged(void)
{
  jtrace->debug("is changed");
}

void JahControl::createJahDesktopUI(	QFrame *moduleselection, QFrame *D2moduleselection)
{
    if (!dualheadprefs)
    {
        Desktop = new QPushButton( moduleselection, "Desktop" );
        Desktop->setGeometry( QRect( 10, 10, 111, 30 ) );
        Desktop->setText( jt->tr( "&Desktop"  ) );
        connect( Desktop, SIGNAL(clicked()), SLOT(changeModeDesktop()) );

        desktopdisplay = new QHBox (WorldLayout, "desktop");
        WorldLayout->addWidget(desktopdisplay);

        CreateDesktopModule(desktopdisplay);
    }
}
