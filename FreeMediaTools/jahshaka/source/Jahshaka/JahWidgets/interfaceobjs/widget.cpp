/*******************************************************************************
**
** The header file for the Jahshaka widget module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/


#include "widget.h"


//==============================================================================
JahPushButton::JahPushButton(QWidget * parent, const char * name)
	: QPushButton(parent, name)
{
	//setBackgroundMode(PaletteBackground);
	//setAutoRaise(true);
}

JahPushButton::~JahPushButton()
{
}

void JahPushButton::enterEvent( QEvent* )
{
	setFlat(true);
}

//
//  Flattens push button when mouse is not over it
//
//

void JahPushButton::leaveEvent( QEvent* )
{
	setFlat(true);
}

JahToolButton::    JahToolButton( QWidget * parent, const char *name )
  : QToolButton( parent, name ){

    setBackgroundMode(PaletteBackground);
    setAutoRaise(true);

    };

QSize JahToolButton::sizeHint() const
{

    int w = 0, h = 0;

    if ( iconSet().isNull() && !text().isNull() && !usesTextLabel() ) {
      w = fontMetrics().width( text() );
      h = fontMetrics().height(); // boundingRect()?
    } else if ( usesBigPixmap() ) {
      QPixmap pm = iconSet().pixmap( QIconSet::Large, QIconSet::Normal );
      w = pm.width();
      h = pm.height();
      if ( w < 32 )
	w = 32;
      if ( h < 32 )
	h = 32;
    } else {
      QPixmap pm = iconSet().pixmap( QIconSet::Small, QIconSet::Normal );
      w = pm.width();
      h = pm.height();
      if ( w < 16 )
	w = 16;
      if ( h < 16 )
	h = 16;
    }
    
    if ( usesTextLabel() ) {
      h += 4 + fontMetrics().height();
      int tw = fontMetrics().width( textLabel() ) + fontMetrics().width("  ");
      if ( tw > w )
	w = tw;
    }

    return QSize(w, h);
}


void JahToolButton::drawButton( QPainter * painter ) 
{
  drawButtonLabel( painter ); 
};

SliderItem:: SliderItem( QCanvas *canvas ) 
  :
  QCanvasLine( canvas ) 
{};

int  SliderItem:: rtti () const
{
  return sliderRTTI2; 
}
