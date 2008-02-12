/*******************************************************************************
 **
 ** The source file for the Jahshaka jahcreate module
 ** The Jahshaka Project
 ** Copyright (C) 2000-2006 VM Inc.
 ** Released under the GNU General Public License
 **
 *******************************************************************************/

#include "jahshaka.h"
#include <qlayout.h>
#include <qhbox.h>
#include <qcursor.h>

//same as routine above but we initialize using a 2nd set of variables
void Jahshaka::setupDualHeadInterface( )
{
    //widget object for the dual head mode
    D2Widget = new QWidget(0,"Jahshaka Dualhead");
    
    //only difference is instead of using this we need
    //to create a new main widget and use it instead
    D2Widget->setMinimumSize( QSize( 1026, 768 ) );
    D2Widget->setSizeIncrement( QSize( 1, 1 ) );
    
    //should be based on screen resolution/gcard texturemem
    //and saved as a preference along with media dir and colorscheme
    D2Widget->setBaseSize( QSize( 1024, 786 ) );
    
    D2jahshakaLayout = new QGridLayout( D2Widget, 1, 1, 0, 0, "d2jahshakaLayout");
    
    D2menucontroller = new QFrame( D2Widget, "d2menucontroller" );
    D2menucontroller->setMinimumSize( QSize( 0, 0 ) );
    D2menucontroller->setMaximumSize( QSize( 32767, 15 ) );
    D2menucontroller->setPaletteBackgroundColor( QColor( 100, 100, 100 ) );
    D2menucontroller->setFrameShape( QFrame::StyledPanel );
    D2menucontroller->setFrameShadow( QFrame::Raised );
    
    D2jahshakaLayout->addWidget( D2menucontroller, 0, 0 );
    
    
    D2mainleftmenu = new QFrame( D2Widget, "d2mainleftmenu" );
    D2mainleftmenu->setMinimumSize( QSize( 30, 0 ) );
    D2mainleftmenu->setMaximumSize( QSize( 30, 1000 ) );
    D2mainleftmenu->setPaletteBackgroundColor( QColor( 100, 100, 100 ) );
    D2mainleftmenu->setFrameShape( QFrame::NoFrame );
    D2mainleftmenu->setFrameShadow( QFrame::Plain );
    
    D2jahshakaLayout->addWidget( D2mainleftmenu, 0, 0 );
    
    D2mainworld = new QFrame( D2Widget, "d2mainworld" );
    D2mainworld->setMinimumSize( QSize( 964, 486 ) );
    D2mainworld->setMaximumSize( QSize( 1964, 1486 ) );
    D2mainworld->setPaletteBackgroundColor( QColor( 35, 35, 35 ) );
    D2mainworld->setCursor( QCursor( 2 ) );
    D2mainworld->setFrameShape( QFrame::NoFrame );
    D2mainworld->setFrameShadow( QFrame::Plain );
    
    D2jahshakaLayout->addWidget( D2mainworld, 0, 1 );
    
    D2mainrightmenu = new QFrame( D2Widget, "d2mainrightmenu" );
    D2mainrightmenu->setMinimumSize( QSize( 30, 0 ) );
    D2mainrightmenu->setMaximumSize( QSize( 30, 1000 ) );
    D2mainrightmenu->setPaletteBackgroundColor( QColor( 100, 100, 100 ) );
    D2mainrightmenu->setFrameShape( QFrame::NoFrame );
    D2mainrightmenu->setFrameShadow( QFrame::Plain );
    
    D2jahshakaLayout->addWidget( D2mainrightmenu, 0, 2 );
    
    D2maincontroller = new QFrame( D2Widget, "d2maincontroller" );
    D2maincontroller->setMinimumSize( QSize( 0, 35 ) );
    D2maincontroller->setMaximumSize( QSize( 32767, 35 ) );
    D2maincontroller->setPaletteBackgroundColor( QColor( 100, 100, 100 ) );
    D2maincontroller->setFrameShape( QFrame::StyledPanel );
    D2maincontroller->setFrameShadow( QFrame::Raised );
    
    D2jahshakaLayout->addMultiCellWidget( D2maincontroller, 1, 1, 0, 2 );
    
    D2moduleinterface = new QFrame( D2Widget, "d2moduleinterface" );
    D2moduleinterface->setMinimumSize( QSize( 0, 235 ) );  ////////was 222 now is 209
    D2moduleinterface->setMaximumSize( QSize( 32767, 209 ) );
    D2moduleinterface->setFrameShape( QFrame::Box );
    D2moduleinterface->setFrameShadow( QFrame::Plain );
    
    D2moduleinterfaceLayout = new QHBoxLayout( D2moduleinterface, 0, 0, "d2moduleinterfaceLayout");
    
    D2moduleselection = new QFrame( D2moduleinterface, "d2moduleselection" );
    D2moduleselection->setMinimumSize( QSize( 125, 0 ) );
    D2moduleselection->setMaximumSize( QSize( 125, 32767 ) );
    D2moduleselection->setFrameShape( QFrame::NoFrame );
    D2moduleselection->setFrameShadow( QFrame::Raised );
    D2moduleinterfaceLayout->addWidget( D2moduleselection );
    
    D2modules = new QFrame( D2moduleinterface, "d2modules" );
    D2modules->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, D2modules->sizePolicy().hasHeightForWidth() ) );
    D2modules->setBackgroundOrigin( QFrame::WidgetOrigin );
    D2modules->setFrameShape( QFrame::NoFrame );
    D2modules->setFrameShadow( QFrame::Raised );
    
    D2modulesLayout = new QGridLayout( D2modules, 1, 1, 0, 0, "d2modulesLayout");
    
    D2feedbackbar = new QFrame( D2modules, "d2feedbackbar" );
    D2feedbackbar->setMinimumSize( QSize( 0, 40 ) );
    D2feedbackbar->setMaximumSize( QSize( 32767, 40 ) );
    D2feedbackbar->setFrameShape( QFrame::NoFrame );
    D2feedbackbar->setFrameShadow( QFrame::Plain );
    D2feedbackbar->setLineWidth( 0 );
    
    D2modulesLayout->addMultiCellWidget( D2feedbackbar, 0, 0, 0, 1 );
    
    D2modulesettings = new QVBox( D2modules, "d2modulesettings" );
    D2modulesettings->setFrameShape( QFrame::StyledPanel );
    D2modulesettings->setFrameShadow( QFrame::Raised );
    D2modulesettings->setMargin( 0 );
    
    D2modulesLayout->addWidget( D2modulesettings, 1, 0 );
    QSpacerItem* D2spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    D2modulesLayout->addItem( D2spacer, 1, 1 );
    D2moduleinterfaceLayout->addWidget( D2modules );
    
    D2jahshakaLayout->addMultiCellWidget( D2moduleinterface, 2, 2, 0, 2 );
    
    
    
    ///////////////////////////////////////////////////////////////
    //set the caption
    D2Widget->setCaption( tr( "JahShaka2", "Realtime Editing and Effects" ) );
    
    //resize the app here based on prefs either small or large
    
    int jw,jh;
    
    if ( QApplication::desktop()->width()  >= 1280 ) jw = 1270; else jw = 1026;
    if ( QApplication::desktop()->height() >= 970 ) jh = 970;   else jh = 768;
    
    D2Widget->resize( QSize(jw, jh).expandedTo(minimumSizeHint()) );
    
}






