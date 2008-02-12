/***************************************************************************
**
** Copyright (C) 2003-2004 Trolltech AS.  All rights reserved.
**
** Licensees holding valid Qt Enterprise Edition licenses may use this
** file in accordance with the Qt Solutions License Agreement provided
** with the Solution.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR
** PURPOSE.
**
** Please email sales@trolltech.com for information
** about Qt Solutions License Agreements.
**
** Contact info@trolltech.com if any conditions of this licensing are
** not clear to you.
**
*/
#include "qtcolordisplay.h"
#include <qmap.h>
#include <qpixmap.h>
#include <qimage.h>
#include <qevent.h>
#include <qpainter.h>
#include <qlayout.h>
#include <qframe.h>
#include <qtooltip.h>

#include <math.h>




/*! \internal

    Constructs the color display frame. Gives it a sunken panel frame.
*/
ColorDisplay::ColorDisplay(QWidget *parent, const char *name)
    : QFrame(parent, name, WNoAutoErase)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    setLineWidth(1);
    setMidLineWidth(1);
    setFrameShape(Box);
    setFrameShadow(Sunken);
}

/*! \internal

    Destructs the color display
*/
ColorDisplay::~ColorDisplay()
{
}

/*! \internal

    Fills the contentsrect with the current color.
*/
void ColorDisplay::drawContents(QPainter *p)
{
    p->fillRect(contentsRect(), QBrush(c));
}



// Sets the current color for the display and sets a tooltip.
void ColorDisplay::setColor(const QColor &col)
{
    c = col;

    QString tips;
    int h, s, v;
    c.getHsv( h, s, v);
    QString conv1, conv2, conv3;
    conv1.setNum(h); conv2.setNum(s); conv3.setNum(v);
    tips = "HSV: " + conv1 + " " + conv2 + " " + conv3;
    QToolTip::remove(this);
    QToolTip::add(this, tips);

    update();
}

/*! \internal

    Returns the current color.
*/
QColor ColorDisplay::color() const
{
    return c;
}

/*! \reimp

*/
QSize ColorDisplay::sizeHint() const
{
    return QSize(100, 10);
}

