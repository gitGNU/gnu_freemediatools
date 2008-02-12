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
#include "qtcolortriangle.h"
#include "colortriangle.h"
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

/*! \class QtColorTriangle

    \brief The QtColorTriangle class provides a triangular color
    selection widget.

    This widget uses the HSV color model, and is therefore useful for
    selecting colors by eye.

    The triangle in the center of the widget is used for selecting
    saturation and value, and the surrounding circle is used for
    selecting hue.

    Use setColor() and color() to set and get the current color.

    \img colortriangle.png
*/

/*! \fn QtColorTriangle::colorChanged(const QColor &color)

    Whenever the color triangles color changes this signal is emitted
    with the new \a color.
*/





QtColorTriangle::QtColorTriangle(QWidget *parent, const char *name)
    : QWidget(parent, name)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    triangle = new ColorTriangle(this);
    layout->addWidget(triangle, 3);

    display = new ColorDisplay(this);
    display->setFixedHeight( 15 ); // want color wheels to take the space.
    display->setColor(triangle->color());
    layout->addWidget(display, 1);

    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    connect(triangle, SIGNAL(colorChangedInternal(const QColor &)), display, SLOT(setColor(const QColor &)));
    connect(triangle, SIGNAL(colorChangedInternal(const QColor &)), SIGNAL(colorChanged(const QColor &)));
}

/*!
    Destructs the QtColorTriangle.
*/
QtColorTriangle::~QtColorTriangle()
{
}

/*!
    Forces the triangle widget to always be square. Returns the value
    \a w.
*/
int QtColorTriangle::heightForWidth(int w) const
{
    return w;
}

/*!
    Sets the current color to \a col. Also updates the triangle
    display and the color selector position.

    \sa color()
*/
void QtColorTriangle::setColor(const QColor &col)
{
    triangle->setColor(col);
}


void QtColorTriangle::setColorSilently(const QColor &col)
{
    triangle->setColorSilently(col);
}

void QtColorTriangle::setColorSilentlyConstantHue(const QColor &col)
{
    triangle->setColorSilentlyConstantHue(col);
}

/*!
    Returns the currently selected color.

    \sa setColor()
*/
QColor QtColorTriangle::color() const
{
    return triangle->color();
}

//#include "qtcolortriangle.moc"
