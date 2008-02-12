/*******************************************************************************
**
** The source file for the Jahshaka Jahroutines module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "jahcreate.h"
#include <qsplitter.h>
#include <qlayout.h>
#include <qhbox.h>
#include <qvbox.h>
#include <qwidgetstack.h>
#include <qpushbutton.h>
#include "jahtranslate.h"
#include "jahtracer.h"
#include "jahformatter.h"

void JahControl::createJahUI(	QFrame *mainworld,		QFrame *mainleftmenu,	  QFrame *mainrightmenu,
								QFrame *maincontroller,	  QFrame *modulesettings,
								QFrame *D2mainworld,	  QFrame *D2mainleftmenu,	QFrame *D2mainrightmenu,
								QFrame  *D2maincontroller,QFrame *D2modulesettings )
{
    //////////////////////////////////////////////////////////
    //create the main controler for the world space object

    wideframeLayout = new QHBoxLayout( mainworld, 0, 0, "frame3Layout");

    //the splitter
    splitter2 = new QSplitter( mainworld, "splitter2" );
    splitter2->setOrientation( QSplitter::Horizontal );
    splitter2->setOpaqueResize(true);

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
    wideframeRight->setMinimumSize( QSize( 30, 0 ) ); 

    wideframeLayout->addWidget( splitter2, 0, 0 );

    wideframeLeft->show( );
    wideframeRight->hide( );

    ////////////////////////////////////////////////////////////
    //this will be for the desktop alone for widescrreen mode
    ////////////////////////////////////////////////////////////

    //it is the left screen acces module
    mainworldLayoutLeft = new QHBoxLayout( 0, "mainworldLayoutLeft");
    WorldLayoutLeft = new QWidgetStack(wideframeLeft);
    mainworldLayoutLeft->addWidget( WorldLayoutLeft );

    //this is for the main worldspace
    mainworldLayout = new QHBoxLayout( 0, "mainworldLayoutCenter");
    WorldLayout = new QWidgetStack(wideframeCenter);
    mainworldLayout->addWidget( WorldLayout );

    //it is the right screen acces module
    mainworldLayoutRight = new QHBoxLayout( 0, "mainworldLayoutRight");
    WorldLayoutRight = new QWidgetStack(wideframeRight);
    mainworldLayoutRight->addWidget( WorldLayoutRight );
	
	
    //////////////////////////////////////////////////////////////////////////
    //also we need to remember size of left/right ui's for when the application restarts
    QValueList<int> sizeList;
    
    sizeList.append(0);
    sizeList.append(600);
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

    //////////////////////////////////////////////////////////////
    //Create Interface frame and switching controllers for modules
    optionals = new QWidgetStack(modulesettings);

    ////////////////////////////////////////////////////////////////
    //for dualhead
    if (dualheadprefs)
    {
        jtrace->info( "Setting up dual head" );

        //////////////////////////////////////////////////////////////
        //create the main controler for the world space object
        D2mainworldLayout = new QHBoxLayout( D2mainworld, 0, 0, "D2mainworldLayout");
        D2WorldLayout = new QWidgetStack(D2mainworld);
        D2mainworldLayout->addWidget( D2WorldLayout );

        //////////////////////////////////////////////////////////////
        //create the main left menu controler
        D2mainworldLeftLayout = new QHBoxLayout( D2mainleftmenu, 0, 0, "D2mainworldLeftLayout");
        D2MainLeftControler = new QWidgetStack(D2mainleftmenu);
        D2mainworldLeftLayout->addWidget( D2MainLeftControler );

        //////////////////////////////////////////////////////////////
        //create the main right menu controler
        D2mainworldRightLayout = new QHBoxLayout( D2mainrightmenu, 0, 0, "D2mainworldRightLayout");
        D2MainRightControler = new QWidgetStack(D2mainrightmenu);
        D2mainworldRightLayout->addWidget( D2MainRightControler );

        //////////////////////////////////////////////////////////////
        //create the middle frame menu/option controler
        D2maincontrollerLayout = new QHBoxLayout( D2maincontroller, 0, 0, "D2maincontrollerLayout");
        D2MainControler = new QWidgetStack(D2maincontroller);
        D2maincontrollerLayout->addWidget( D2MainControler );

        //////////////////////////////////////////////////////////////
        //Create Interface frame and switching controllers for modules
        D2optionals = new QWidgetStack(D2modulesettings);
    }


}

void JahControl::splitterChanged(void)
{
    //jtrace->debug("is changed");
}

void createMinimizingLayout( QWidget* p )
{
    QBoxLayout* l = new QVBoxLayout( p );
    JahFormatter::setMarginSmall( l );
    JahFormatter::setSpacing( l );
}

void JahControl::createJahDesktopUI( QFrame *moduleselection, QFrame *D2moduleselection )
{
    if (!dualheadprefs)
    {
        if ( !moduleselection->layout() )
        {
            createMinimizingLayout( moduleselection );
        }
        
        DesktopButton = new QPushButton( moduleselection, "Desktop" );
        moduleselection->layout()->add( DesktopButton );
        
        DesktopButton->setText( jt->tr( "&Desktop"  ) );
        connect( DesktopButton, SIGNAL(clicked()), SLOT(changeModeDesktop()) );

        desktopdisplay = new QHBox (WorldLayout, "desktop");
        WorldLayout->addWidget(desktopdisplay);

        CreateDesktopModule(desktopdisplay);
    }

    if (dualheadprefs)
    {
        if ( !D2moduleselection->layout() )
        {
            createMinimizingLayout( D2moduleselection );
        }

        DualDesktopButton = new QPushButton( D2moduleselection, "Desktop" );
        D2moduleselection->layout()->add( DualDesktopButton );

        DualDesktopButton->setText( jt->tr( "&Desktop"  ) );
        connect( DualDesktopButton, SIGNAL(clicked()), SLOT(changeModeDesktop()) );

        desktopdisplay = new QHBox( D2WorldLayout, "d2desktop" );
        D2WorldLayout->addWidget(desktopdisplay);
        CreateDesktopModule(desktopdisplay);

        DualEncoding = new QPushButton( D2moduleselection, "DualEncoding" );
        D2moduleselection->layout()->add( DualEncoding );
        DualEncoding->setText( jt->tr( "Encode"  ) );
        connect( DualEncoding,  SIGNAL(clicked()), SLOT(changeModeEncoding())  );

#ifdef USE_NETWORK
        DualNetwork = new QPushButton( D2moduleselection, "Network" );
        D2moduleselection->layout()->add( DualNetwork );
        DualNetwork->setText( jt->tr( "Network"  ) );
        connect( DeualNetwork,  SIGNAL(clicked()), SLOT(changeModeNetwork())  );
#endif

        D2moduleselection->layout()->addItem( new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding ) );
    }

}

void JahControl::createJahDesktopButtons( QFrame *moduleselection )
{
    if ( !moduleselection->layout() )
    {
        createMinimizingLayout( moduleselection );
    }

    AnimationModuleButton = new QPushButton( moduleselection, "Animation" );
    moduleselection->layout()->add( AnimationModuleButton );
    AnimationModuleButton->setText( jt->tr("&Animation") );
    connect( AnimationModuleButton, SIGNAL(clicked()), SLOT(changeModeAnimation()) );  //was Effects

    EffectsModuleButton = new QPushButton( moduleselection, "Effects" );
    moduleselection->layout()->add( EffectsModuleButton );
    EffectsModuleButton->setText( jt->tr( "E&ffects"  ) );
    connect( EffectsModuleButton,  SIGNAL(clicked()), SLOT(changeModeEffects())  );

    EditingModuleButton = new QPushButton( moduleselection, "Editing" );
    moduleselection->layout()->add( EditingModuleButton );
    EditingModuleButton->setText( jt->tr( "&Editing"  ) );
    connect( EditingModuleButton,  SIGNAL(clicked()), SLOT(changeModeEditing())  );

    PaintModuleButton = new QPushButton( moduleselection, "paint" );
    moduleselection->layout()->add( PaintModuleButton );
    PaintModuleButton->setText( jt->tr( "&Paint"  ) );
    connect( PaintModuleButton,  SIGNAL(clicked()), SLOT(changeModePaint())  );

    TextModuleButton = new QPushButton( moduleselection, "text" );
    moduleselection->layout()->add( TextModuleButton );
    TextModuleButton->setText( jt->tr( "&Text CG"  ) );
    connect( TextModuleButton,  SIGNAL(clicked()), SLOT(changeModeText())  );

    moduleselection->layout()->addItem( new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding ) );
}
