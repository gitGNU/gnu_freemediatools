/*******************************************************************************
**
** The source file for the Jahshaka Jahstyle module
** The Jahshaka Project
** Copyright (C) 2000-2004 The Jahshaka Project
** Released under the GNU General Public License
**
*******************************************************************************/

#include "jahplayer.h"
#include "jahplayerlogo.h"

//#include "qsizepolicy.h"
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

/////////////////////////////////////////////////////////////
// Builds the frame of the interface

void Jahplayer::setupInterface( QWidget* parent  )
{
		QPixmap image1( (const char **) image0_data );

    setMinimumSize( QSize( 320, 240 ) );
    setIcon( image1 );
    setAcceptDrops( TRUE );
    setOpaqueMoving( TRUE );

    JahPrefs& jprefs = JahPrefs::getInstance();
    QString theJahVersion    = jprefs.getJahVersion().c_str();

	setName( "Jahplayer "+theJahVersion );


	setCentralWidget( new QWidget( parent, "qt_central_widget" ) );
    JahshakaLayout = new QGridLayout( centralWidget(), 1, 1, 0, 0, "JahshakaLayout"); 

    masterlayout = new QVBoxLayout( 0, 0, 0, "masterlayout"); 

    header = new QFrame( centralWidget(), "header" );
    header->setMinimumSize( QSize( 0, 25 ) );
    header->setMaximumSize( QSize( 32767, 25 ) );
    header->setPaletteBackgroundColor( QColor( 236, 233, 216 ) );
    header->setFrameShape( QFrame::StyledPanel );
    header->setFrameShadow( QFrame::Raised );
	header->setMinimumHeight( 25 );

    masterlayout->addWidget( header );

    center = new QSplitter( centralWidget(), "center" );
    center->setMinimumSize( QSize( 320, 240 ) ); //1005, 730
    center->setOrientation( QSplitter::Vertical );

    masterworldspace = new QFrame( center, "masterworldspace" );
    masterworldspace->setFrameShape( QFrame::StyledPanel );
    masterworldspace->setFrameShadow( QFrame::Raised );
    masterworldspaceLayout = new QVBoxLayout( masterworldspace, 0, 0, "masterworldspaceLayout"); 

    worldlayout = new QHBoxLayout( 0, 0, 0, "worldlayout"); 

    lefttoolbar = new QFrame( masterworldspace, "lefttoolbar" );
    lefttoolbar->setMinimumSize( QSize( 30, 0 ) );
    lefttoolbar->setPaletteBackgroundColor( QColor( 236, 233, 216 ) );
    lefttoolbar->setCursor( QCursor( 0 ) );
    lefttoolbar->setFrameShape( QFrame::StyledPanel );
    lefttoolbar->setFrameShadow( QFrame::Raised );
    lefttoolbar->setLineWidth( 0 );
	lefttoolbar->hide();
    //worldlayout->addWidget( lefttoolbar );

    worldspace = new QFrame( masterworldspace, "worldspace" );
    worldspace->setMinimumSize( QSize( 720, 486 ) );
    worldspace->setFrameShape( QFrame::StyledPanel );
    worldspace->setFrameShadow( QFrame::Raised );
    worldspace->setLineWidth( 0 );

    worldlayout->addWidget( worldspace );

    righttoolbar = new QFrame( masterworldspace, "righttoolbar" );
    righttoolbar->setMinimumSize( QSize( 30, 0 ) );
    righttoolbar->setPaletteBackgroundColor( QColor( 236, 233, 216 ) );
    righttoolbar->setCursor( QCursor( 2 ) );
    righttoolbar->setFrameShape( QFrame::StyledPanel );
    righttoolbar->setFrameShadow( QFrame::Raised );
    righttoolbar->setLineWidth( 0 );
    worldlayout->addWidget( righttoolbar );
    masterworldspaceLayout->addLayout( worldlayout );

    controller = new QWidgetStack( masterworldspace, "controller" );
    controller->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, controller->sizePolicy().hasHeightForWidth() ) );
    controller->setMinimumSize( QSize( 0, 55 ) );
    controller->setMaximumSize( QSize( 32767, 55 ) );
    controller->setLineWidth( 0 );
    controller->setMidLineWidth( 0 );

    masterworldspaceLayout->addWidget( controller );
 
    masterlayout->addWidget( center );
/*
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
*/ 
    JahshakaLayout->addLayout( masterlayout, 0, 0 );
    
	languageChange();
	
    resize( QSize(320, 240).expandedTo(minimumSizeHint()) );

	setSizeIncrement( QSize( 1, 1 ) );
	
	clearWState( WState_Polished );

}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */

void Jahplayer::languageChange()
{
    setCaption( tr( "Jahplayer" ) );
    QWhatsThis::add( this, tr( "Jahplayer" ) );
}




void Jahplayer::shrinkUI(bool value)
{
	
    //////////////////////////////////////////////////////
    //shrink the ui
    if (value)
    {
        //code used to shrink or grow the ui
        header->show();
        //footer->show();
        righttoolbar->show();
    }
    else
    {
        //code used to shrink or grow the ui
        header->hide();
        //footer->hide();
        righttoolbar->hide();
    }
}



/////////////////////////////////////////////////////////////
// sets up the default look and feel of the interface

void Jahplayer::setupMyCustomColors() 
{
       
    //now we set up the style
    QStyle *QSGIStyle=QStyleFactory::create("sgi"); //wsa sgi

    if(QSGIStyle)   
        qApp->setStyle( QSGIStyle );

    //first we set up the font
    /*
    QFont appFont = QApplication::font();
    appFont.setPointSize(10);
    appFont.setFamily("Helvetica");
    QFont::setDefaultFont(QFont("Helvetica", 10, QFont::Normal, false));
    qApp->setFont( QFont( "Helvetica", 10 ), TRUE );
*/ 


    QPalette pal( QColor( 80, 80, 80 ) );

    QColorGroup active      (pal.active().foreground(), pal.active().background(),
                            pal.active().light(), pal.active().dark(),
                            pal.active().mid(), pal.active().text(),
                            pal.active().base() );
    active.setColor( QColorGroup::ButtonText,  Qt::white  );
    active.setColor( QColorGroup::Shadow,  Qt::black  );

    QColorGroup disabled    (pal.disabled().foreground(), pal.disabled().background(),
                            pal.disabled().light(), pal.disabled().dark(),
                            pal.disabled().mid(), pal.disabled().text(),
                            pal.disabled().base() );
    disabled.setColor( QColorGroup::ButtonText,  Qt::white  );

    QPalette newPalette( active, disabled, active );
    qApp->setPalette( newPalette, TRUE );

    lefttoolbar->setPalette( newPalette, TRUE );
    righttoolbar->setPalette( newPalette, TRUE );
    header->setPalette( newPalette, TRUE );

    /*
    QStyle *platinum=QStyleFactory::create("platinum");
    if (platinum)
    {   qApp->setStyle( platinum );
    }

    QPalette p = qApp->palette();
    p.setColor(QColorGroup::Background, QColor( 20,20,20)); 
    p.setColor(QColorGroup::Base, QColor( 15, 15, 15)); 
    p.setColor(QColorGroup::Text, QColor( Qt::white)); 
    p.setColor(QColorGroup::ButtonText, QColor( Qt::white)); 
    p.setColor(QColorGroup::Highlight, QColor( "cornflowerblue")); 
    p.setColor(QColorGroup::Foreground, QColor( Qt::white));
    p.setColor(QColorGroup::Mid, QColor( 40, 40, 40));
    p.setColor(QColorGroup::Button, QColor( 35,35,35)); 
    p.setColor(QColorGroup::Light, QColor( 35, 35, 35));
    p.setColor(QColorGroup::Dark, QColor( 70, 70, 70));
//    p.setColor(QColorGroup::Dark, QColor( 35, 35, 35));
//    p.setColor(QColorGroup::Light, QColor( 70, 70, 70));
    qApp->setPalette(p, TRUE);

    lefttoolbar->setPalette( p, TRUE );
    righttoolbar->setPalette( p, TRUE );
    header->setPalette( p, TRUE );
*/
}

/////////////////////////////////////////////////////////////
// used to change the color of the interface

void Jahplayer::setStyleColor(QPalette pal) 
{

  /////////////////////////////////////////////////////////////
    // First we set the color of the lower frames

    QColorGroup active      (pal.active().foreground(), pal.active().background(),
                            pal.active().light(), pal.active().dark(),
                            pal.active().mid(), pal.active().text(),
                            pal.active().base() );
    active.setColor( QColorGroup::ButtonText,  Qt::white  );
    active.setColor( QColorGroup::Shadow,  Qt::black  );

    QColorGroup disabled    (pal.disabled().foreground(), pal.disabled().background(),
                            pal.disabled().light(), pal.disabled().dark(),
                            pal.disabled().mid(), pal.disabled().text(),
                            pal.disabled().base() );
    disabled.setColor( QColorGroup::ButtonText,  Qt::black  );

    QPalette newPalette( active, disabled, active );
    qApp->setPalette( newPalette, TRUE );


    /////////////////////////////////////////////////////////////
    // Now we set the color of the upper frame  as lighter than above

    QColor lighten = pal.color( QPalette::Active, QColorGroup::Background    );

    int red = lighten.red()+20;
    int green = lighten.green()+20;
    int blue = lighten.blue()+20;

    QPalette opp (QColor( red, green, blue ));

    QColorGroup oactive     (opp.active().foreground(), opp.active().background(),
                            opp.active().light(), opp.active().dark(),
                            opp.active().mid(), opp.active().text(),  //as white
                            opp.active().base() );
    oactive.setColor( QColorGroup::ButtonText,  Qt::black  );
    oactive.setColor( QColorGroup::Shadow,  Qt::black  );

    QColorGroup odisabled   (opp.disabled().foreground(), opp.disabled().background(),
                            opp.disabled().light(), opp.disabled().dark(),
                            opp.disabled().mid(), opp.disabled().text(),
                            opp.disabled().base() );
    odisabled.setColor( QColorGroup::ButtonText,  Qt::black  );

    QPalette onewPalette( oactive, odisabled, oactive );


    //header->setPalette( onewPalette, TRUE );
//    menubox->setPalette( onewPalette, TRUE );
   // modebox->setPalette( onewPalette, TRUE );
    //statusbox->setPalette( onewPalette, TRUE );
    lefttoolbar->setPalette( onewPalette, TRUE );
    righttoolbar->setPalette( onewPalette, TRUE );
    header->setPalette( onewPalette, TRUE );
    //menuspacer1->setPalette( onewPalette, TRUE );

}






