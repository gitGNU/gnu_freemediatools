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
  JahToolButton( QWidget * parent, const char *name );
  //
  // Lifted from QToolButton
  // only sizes the size of the content, not any button border.
  //
  QSize sizeHint() const;
 protected:
  void drawButton( QPainter * painter );
  
};

//this class is for the slider in the timeline and keyframe boxes
static const int sliderRTTI2 = 984000;

class SliderItem: public QCanvasLine {
 public:

  SliderItem( QCanvas *canvas );
  int rtti () const;
  
};

#endif






