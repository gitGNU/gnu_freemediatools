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
#ifndef QTCOLORTRIANGLE_H
#define QTCOLORTRIANGLE_H
#include <qwidget.h>

class ColorTriangle;
class ColorDisplay;

/*
#if defined(Q_WS_WIN)
#  if !defined(QT_QTCOLORTRIANGLE_EXPORT) && !defined(QT_QTCOLORTRIANGLE_IMPORT)
#    define QT_QTCOLORTRIANGLE_EXPORT
#  elif defined(QT_QTCOLORTRIANGLE_IMPORT)
#    if defined(QT_QTCOLORTRIANGLE_EXPORT)
#      undef QT_QTCOLORTRIANGLE_EXPORT
#    endif
#    define QT_QTCOLORTRIANGLE_EXPORT __declspec(dllimport)
#  elif defined(QT_QTCOLORTRIANGLE_EXPORT)
#    undef QT_QTCOLORTRIANGLE_EXPORT
#    define QT_QTCOLORTRIANGLE_EXPORT __declspec(dllexport)
#  endif
#else
#  define QT_QTCOLORTRIANGLE_EXPORT
#endif
*/
//class QT_QTCOLORTRIANGLE_EXPORT QtColorTriangle : public QWidget
class QtColorTriangle : public QWidget
{
    Q_OBJECT
public:
    QtColorTriangle(QWidget *parent = 0, const char *name = 0);
    ~QtColorTriangle();

    int heightForWidth(int w) const;

    QColor color() const;
    
    // These are public for easy access.
    ColorTriangle *triangle;
    ColorDisplay *display;

signals:
    void colorChanged(const QColor &col);

public slots:
    void setColor(const QColor &col);
    void setColorSilently(const QColor &col); // as setColor but emits no signals afterwards.
    // as above but H wont be changed. The problem is that near the corners of the triangle the color
    // goes to black (for example), which means we forget our HUE when moving back into the triangle.
    void setColorSilentlyConstantHue(const QColor &col);


};
#endif
