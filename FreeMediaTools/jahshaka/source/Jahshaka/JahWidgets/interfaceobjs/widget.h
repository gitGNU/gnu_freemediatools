/*******************************************************************************
**
** The header file for the Jahshaka widget module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/


#ifndef WIDGETS_H
#define WIDGETS_H


#include <qpushbutton.h>
#include <qtoolbutton.h>
#include <qwidget.h>
#include <qcanvas.h>


//class QPushButton;

/**
 * Tool button that uses PaletteBackground rather than the useless default
 * PaletteButton.
 */


class JahPushButton : public QPushButton
{
public:
	JahPushButton( QWidget * parent, const char * name = 0 );
	~JahPushButton();

    void enterEvent( QEvent* );
    void leaveEvent( QEvent* );

};

class JahToolButton: public QToolButton
{

public:
    JahToolButton( QWidget * parent, const char *name )
    	: QToolButton( parent, name ){

    setBackgroundMode(PaletteBackground);
    setAutoRaise(true);

    };

//
// Lifted from QToolButton
// only sizes the size of the content, not any button border.
//
QSize sizeHint() const
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


protected:

    void drawButton( QPainter * painter ) { drawButtonLabel( painter ); };

};

//this class is for the slider in the timeline and keyframe boxes
static const int sliderRTTI2 = 984000;

class SliderItem: public QCanvasLine {
public:

  SliderItem( QCanvas *canvas ) : QCanvasLine( canvas ) {};
  int rtti () const { return sliderRTTI2; }

};

#endif






