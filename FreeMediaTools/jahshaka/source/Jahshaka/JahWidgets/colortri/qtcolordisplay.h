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
#ifndef QTCOLORDISPLAY_H
#define QTCOLORDISPLAY_H
#include <qwidget.h>
#include <qframe.h>

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
class ColorDisplay : public QFrame
{
    Q_OBJECT
public:
    ColorDisplay(QWidget *parent = 0, const char *name = 0);
    ~ColorDisplay();

    QColor color() const;

    QSize sizeHint() const;

public slots:
    void setColor(const QColor &c);

protected:
    void drawContents(QPainter *);

private:
    QColor c;
};

#endif
