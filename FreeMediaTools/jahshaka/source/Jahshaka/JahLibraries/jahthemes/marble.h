/****************************************************************************
** $Id: marble.h,v 1.1 2005/06/15 11:24:22 jahshaka Exp $
**
** Definition of the Metal Style for the themes example
**
** Created : 979899
**
** Copyright (C) 1997 by Trolltech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/
#ifndef MARBLE_H
#define MARBLE_H

#include <qpalette.h>
#include <qapplication.h>
#include <qcombobox.h>
#include <qpainter.h>
#include <qdrawutil.h> // for now
#include <qpixmap.h> // for now
#include <qpalette.h> // for now
#include <qwidget.h>
#include <qlabel.h>
#include <qimage.h>
#include <qpushbutton.h>
#include <qwidget.h>
#include <qrangecontrol.h>
#include <qscrollbar.h>
#include <qslider.h>
#include <limits.h>

#ifndef QT_NO_STYLE_WINDOWS

#include <qwindowsstyle.h>


class JahMetalStyle : public QWindowsStyle
{
public:
    JahMetalStyle();
    void polish( QApplication*);
    void unPolish( QApplication*);
    void polish( QWidget* );
    void unPolish( QWidget* );

    void drawPrimitive( PrimitiveElement pe,
            QPainter *p,
            const QRect &r,
            const QColorGroup &cg,
            SFlags flags = Style_Default,
            const QStyleOption& = QStyleOption::Default) const;

    void drawControl( ControlElement element,
              QPainter *p,
              const QWidget *widget,
              const QRect &r,
              const QColorGroup &cg,
              SFlags how = Style_Default,
              const QStyleOption& = QStyleOption::Default ) const;

    void drawComplexControl( ComplexControl cc,
                 QPainter *p,
                 const QWidget *widget,
                 const QRect &r,
                 const QColorGroup &cg,
                 SFlags how = Style_Default,
                 SCFlags sub = SC_All,
                 SCFlags subActive = SC_None,
                 const QStyleOption& = QStyleOption::Default ) const;
    int pixelMetric( PixelMetric, const QWidget * ) const;


private:
    void drawMetalFrame(  QPainter *p, int x, int y, int w, int h ) const;
    void drawMetalGradient( QPainter *p, int x, int y, int w, int h,
              bool sunken, bool horz, bool flat=FALSE ) const;
    void drawMetalButton( QPainter *p, int x, int y, int w, int h,
              bool sunken, bool horz, bool flat=FALSE ) const;
    QPalette oldPalette;
};

#endif

#endif
