/*******************************************************************************
 **
 ** The source file for the Jahshaka Jahstyle module
 ** The Jahshaka Project
 ** Copyright (C) 2000-2006 VM Inc.
 ** Released under the GNU General Public License
 **
 *******************************************************************************/

#include "jahshaka.h"
#include "jahlogo.h"
#include "jahheader.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qframe.h>
#include <qsplitter.h>
#include <qwidgetstack.h>
#include <qwidget.h>
#include <qcheckbox.h>
#include <qslider.h>
#include <qlcdnumber.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qaction.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qtoolbar.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qstylefactory.h>

/////////////////////////////////////////////////////////////
// Builds the frame of the interface

//void Jah::setupInterface( QWidget* parent, const char* name )
void Jahshaka::setupInterface( QWidget* parent )
{
    QPixmap image1( (const char **) image0_data );

#ifndef WIN32 // windows should set all icons via the res file.
    setIcon( image1 );
#endif
    setAcceptDrops( TRUE );
    setOpaqueMoving( TRUE );
    
    JahPrefs& jprefs = JahPrefs::getInstance();
    QString theJahVersion    = jprefs.getJahVersion().c_str();
    
#ifndef JAHPLAYER 
    setName( "Jahshaka "+theJahVersion );
#else
    setName( "Jahplayer "+theJahVersion );
#endif
    
    
    setCentralWidget( new QWidget( parent, "qt_central_widget" ) );
    JahshakaLayout = new QGridLayout( centralWidget(), 1, 1, 0, 0, "JahshakaLayout"); 
    
    masterlayout = new QVBoxLayout( 0, 0, 0, "masterlayout"); 
    
    header = new JahHeader( centralWidget() );
    masterlayout->addWidget( header );

    center = new QSplitter( centralWidget(), "center" );
    center->setMinimumSize( QSize( 320, 240 ) );
    center->setOrientation( QSplitter::Vertical );
    
    masterworldspace = new QFrame( center, "masterworldspace" );
    masterworldspace->setFrameShape( QFrame::StyledPanel );
    masterworldspace->setFrameShadow( QFrame::Raised );
    masterworldspaceLayout = new QVBoxLayout( masterworldspace, 0, 0, "masterworldspaceLayout"); 
    
    worldlayout = new QHBoxLayout( 0, 0, 0, "worldlayout"); 
    
    lefttoolbar = new QFrame( masterworldspace, "lefttoolbar" );
    lefttoolbar->setMinimumSize( QSize( 30, 0 ) );
    lefttoolbar->setCursor( QCursor( 0 ) );
    lefttoolbar->setFrameShape( QFrame::StyledPanel );
    lefttoolbar->setFrameShadow( QFrame::Raised );
    lefttoolbar->setLineWidth( 0 );
    lefttoolbar->hide();
    worldlayout->addWidget( lefttoolbar );

    worldspace = new QFrame( masterworldspace, "worldspace" );
    worldspace->setMinimumSize( QSize( 320, 240 ) );
    worldspace->setFrameShape( QFrame::StyledPanel );
    worldspace->setFrameShadow( QFrame::Raised );
    worldspace->setLineWidth( 0 );
    worldlayout->addWidget( worldspace );

    controller = new QWidgetStack( masterworldspace, "controller" );
    controller->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );
    controller->setMinimumSize( QSize( 0, 55 ) );
    controller->setMaximumSize( QSize( 32767, 55 ) );
    controller->setLineWidth( 0 );
    controller->setMidLineWidth( 0 );
    // deliberately not added to a layout here
    
    righttoolbar = new QFrame( masterworldspace, "righttoolbar" );
    righttoolbar->setMinimumSize( QSize( 30, 0 ) );
    righttoolbar->setCursor( QCursor( 2 ) );
    righttoolbar->setFrameShape( QFrame::StyledPanel );
    righttoolbar->setFrameShadow( QFrame::Raised );
    righttoolbar->setLineWidth( 0 );
    worldlayout->addWidget( righttoolbar );

    masterworldspaceLayout->addLayout( worldlayout );

    // below the main splitter
    
    modules = new QFrame( center, "modules" );

    QBoxLayout* modules_layout = new QHBoxLayout( modules );
    modules_layout->setSpacing( 5 );
    modules_layout->setMargin( 5 );
    
    modules_buttons = new QFrame( modules, "modules_buttons" );
    modules_buttons->setMaximumSize( QSize( 115, 32767 ) );
    modules_layout->addWidget( modules_buttons );
    
    modules_controls = new QVBox( modules, "modules_controls" );
    modules_layout->addWidget( modules_controls );
    
    masterlayout->addWidget( center );
    
    footer = new QFrame( centralWidget(), "footer" );
    footer->setMaximumSize( QSize( 32767, 25 ) );
    footer->setFrameShape( QFrame::ToolBarPanel );
    footer->setFrameShadow( QFrame::Plain );
    footer->setLineWidth( 1 );
    footerLayout = new QHBoxLayout( footer, 0, 0, "footerLayout"); 
    
    footer_menus = new QFrame( footer, "footer_menus" );
    footer_menus->setMinimumSize( QSize( 500, 25 ) );
    footer_menus->setMaximumSize( QSize( 32767, 25 ) );
    footer_menus->setFrameShape( QFrame::StyledPanel );
    footer_menus->setFrameShadow( QFrame::Raised );
    footer_menus->setLineWidth( 1 );
    footerLayout->addWidget( footer_menus );
    footerspacer1 = new QSpacerItem( 50, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
    footerLayout->addItem( footerspacer1 );
    
    footer_modes = new QFrame( footer, "footer_modes" );
    footer_modes->setMinimumSize( QSize( 200, 25 ) );
    footer_modes->setMaximumSize( QSize( 200, 25 ) );
    footer_modes->setFrameShape( QFrame::StyledPanel );
    footer_modes->setFrameShadow( QFrame::Raised );
    footer_modes->setLineWidth( 1 );
    footerLayout->addWidget( footer_modes );
    footerspacer2 = new QSpacerItem( 50, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
    footerLayout->addItem( footerspacer2 );
    
    footer_status = new QFrame( footer, "footer_status" );
    footer_status->setMinimumSize( QSize( 200, 25 ) );
    footer_status->setMaximumSize( QSize( 200, 25 ) );
    footer_status->setFrameShape( QFrame::StyledPanel );
    footer_status->setFrameShadow( QFrame::Raised );
    footer_status->setLineWidth( 1 );
    footerLayout->addWidget( footer_status );
    masterlayout->addWidget( footer );

    footer->hide( );
    JahshakaLayout->addLayout( masterlayout, 0, 0 );
    
    languageChange();
    setSizeIncrement( QSize( 1, 1 ) );
    
    clearWState( WState_Polished );
    
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */

void Jahshaka::languageChange()
{
    setCaption( tr( "Jahshaka" ) );
    QWhatsThis::add( this, tr( "Jahshaka" ) );
}



void Jahshaka::shrinkUI(bool value)
{
    //////////////////////////////////////////////////////
    //shrink the ui
    
    if (value)
    {
        //code used to shrink or grow the ui
        modules->show();
        righttoolbar->show();
        header->show();
    }
    else
    {
        //code used to shrink or grow the ui
        modules->hide();
        righttoolbar->hide();
        header->hide();
    }

}



/////////////////////////////////////////////////////////////
// sets up the default look and feel of the interface

void Jahshaka::setupMyCustomColors() 
{
    QPalette pal( QColor( 80, 80, 80 ) );
    
    QColorGroup active( pal.active().foreground(), pal.active().background(),
                        pal.active().light(), pal.active().dark(),
                        pal.active().mid(), pal.active().text(),
                        pal.active().base() );
    active.setColor( QColorGroup::ButtonText,  Qt::white  );
    active.setColor( QColorGroup::Shadow,  Qt::black  );

    QColorGroup disabled( pal.disabled().foreground(), pal.disabled().background(),
                          pal.disabled().light(), pal.disabled().dark(),
                          pal.disabled().mid(), pal.disabled().text(),
                          pal.disabled().base() );
    disabled.setColor( QColorGroup::ButtonText,  Qt::white  );

    QPalette newPalette( active, disabled, active );
    qApp->setPalette( newPalette, TRUE );
}

/////////////////////////////////////////////////////////////
// used to change the color of the interface

void Jahshaka::setStyleColor(QPalette pal) 
{
    /////////////////////////////////////////////////////////////
    // First we set the color of the lower frames

    QColorGroup active( pal.active().foreground(), pal.active().background(),
                        pal.active().light(), pal.active().dark(),
                        pal.active().mid(), pal.active().text(),
                        pal.active().base() );
    active.setColor( QColorGroup::ButtonText,  Qt::white  );
    active.setColor( QColorGroup::Shadow,  Qt::black  );

    QColorGroup disabled( pal.disabled().foreground(), pal.disabled().background(),
                          pal.disabled().light(), pal.disabled().dark(),
                          pal.disabled().mid(), pal.disabled().text(),
                          pal.disabled().base() );
    disabled.setColor( QColorGroup::ButtonText,  Qt::black  );

    QPalette newPalette( active, disabled, active );
    qApp->setPalette( newPalette, TRUE );
}

