/*******************************************************************************
**
** The source file for the Jahshaka Jahstyle module
** The Jahshaka Project
** Copyright (C) 2000-2004 The Jahshaka Project
** Released under the GNU General Public License
**
*******************************************************************************/

#include "jahplayer.h"

/////////////////////////////////////////////////////////////
// Builds the frame of the interface

//void Jah::setupInterface( QWidget* parent, const char* name )
void Jah::setupInterface( )
{

  this->setMinimumSize( QSize( 320, 240  ) );
  //this->setMaximumSize( QSize( 1270, 970 ) );
  this->setSizeIncrement( QSize( 1, 1 ) );

  //should be based on screen resolution/gcard texturemem
  //and saved as a preference along with media dir and colorscheme
  this->setBaseSize( QSize( 720, 486 ) );

  jahshakaLayout = new QGridLayout( this, 1, 1, 0, 0, "jahshakaLayout");

  mainleftmenu = new QFrame( this, "mainleftmenu" );
  mainleftmenu->setMinimumSize( QSize( 30, 0 ) );
  mainleftmenu->setMaximumSize( QSize( 30, 1000 ) );
  mainleftmenu->setPaletteBackgroundColor( QColor( 100, 100, 100 ) );
  mainleftmenu->setFrameShape( QFrame::NoFrame );
  mainleftmenu->setFrameShadow( QFrame::Plain );

  jahshakaLayout->addWidget( mainleftmenu, 0, 0 );

  mainworld = new QFrame( this, "mainworld" );
  mainworld->setMinimumSize( QSize( 320, 240 ) );
  mainworld->setMaximumSize( QSize( 1964, 1486 ) );
  mainworld->setPaletteBackgroundColor( QColor( 35, 35, 35 ) );
  mainworld->setCursor( QCursor( 2 ) );
  mainworld->setFrameShape( QFrame::NoFrame );
  mainworld->setFrameShadow( QFrame::Plain );

  jahshakaLayout->addWidget( mainworld, 0, 1 );

  mainrightmenu = new QFrame( this, "mainrightmenu" );
  mainrightmenu->setMinimumSize( QSize( 30, 0 ) );
  mainrightmenu->setMaximumSize( QSize( 30, 1000 ) );
  mainrightmenu->setPaletteBackgroundColor( QColor( 100, 100, 100 ) );
  mainrightmenu->setFrameShape( QFrame::NoFrame );
  mainrightmenu->setFrameShadow( QFrame::Plain );

  jahshakaLayout->addWidget( mainrightmenu, 0, 2 );

  maincontroller = new QFrame( this, "maincontroller" );
  maincontroller->setMinimumSize( QSize( 0, 35 ) );
  maincontroller->setMaximumSize( QSize( 32767, 35 ) );
  maincontroller->setPaletteBackgroundColor( QColor( 100, 100, 100 ) );
  maincontroller->setFrameShape( QFrame::StyledPanel );
  maincontroller->setFrameShadow( QFrame::Raised );

  jahshakaLayout->addMultiCellWidget( maincontroller, 1, 1, 0, 2 );

  ///////////////////////////////////////////////////////////////
  //set the caption
  this->setCaption( tr( "JahPlayer", "Media Playback Platform" ) );

  this->resize( QSize(720, 486).expandedTo(minimumSizeHint()) );


}

/////////////////////////////////////////////////////////////
// the color and theme routines are the same for player and jahshaka
// could be consolidated into their own file
// or moved into jahstyle 

/////////////////////////////////////////////////////////////
// sets up the default look and feel of the interface

void Jah::setupMyCustomColors() 
{

    //first we set up the font
    QFont appFont = QApplication::font();
    appFont.setPointSize(10);
    appFont.setFamily("Helvetica");
    QFont::setDefaultFont(QFont("Helvetica", 10, QFont::Normal, false));
    qApp->setFont( QFont( "Helvetica", 10 ), TRUE );

    //now we set up the style
    QStyle *QSGIStyle=QStyleFactory::create("sgi");

    if(QSGIStyle)   
        qApp->setStyle( QSGIStyle );

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


}

/////////////////////////////////////////////////////////////
// used to change the color of the interface

void Jah::setStyleColor(QPalette pal) 
{

    /////////////////////////////////////////////////////////////
    // First we set the color of the lower frames

    QColorGroup active      (pal.active().foreground(), pal.active().background(),
                            pal.active().light(), pal.active().dark(),
                            pal.active().mid(), pal.active().text(),
                            pal.active().base() );
    active.setColor( QColorGroup::ButtonText,  Qt::black  );
    active.setColor( QColorGroup::Shadow,  Qt::black  );

    QColorGroup disabled    (pal.disabled().foreground(), pal.disabled().background(),
                            pal.disabled().light(), pal.disabled().dark(),
                            pal.disabled().mid(), pal.disabled().text(),
                            pal.disabled().base() );
    disabled.setColor( QColorGroup::ButtonText,  Qt::black  );

    QPalette newPalette( active, disabled, active );
    qApp->setPalette( newPalette, TRUE );

    //these all get set by the qApp command above
    //moduleselection->setPalette( newPalette, TRUE );
    //modulesettings->setPalette( newPalette, TRUE );
    //feedbackbar->setPalette( newPalette, TRUE );

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

    maincontroller->setPalette( onewPalette, TRUE );
    mainleftmenu->setPalette( onewPalette, TRUE );
    mainrightmenu->setPalette( onewPalette, TRUE );

}






