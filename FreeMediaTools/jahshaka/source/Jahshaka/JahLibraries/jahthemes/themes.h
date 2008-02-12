/****************************************************************************
** $Id: themes.h,v 1.5 2006/03/29 08:35:44 timdewhirst Exp $
**
** Copyright (C) 1992-2000 Trolltech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef THEMES_H
#define THEMES_H

#include <qfont.h>
#include <qobject.h>

class JahThemes: public QObject
{
    Q_OBJECT

public:
    JahThemes( QObject *parent = 0, const char *name = 0 );

public slots:
    void makeTheme(const QString &);

private:
    QFont appFont;
};

#endif
