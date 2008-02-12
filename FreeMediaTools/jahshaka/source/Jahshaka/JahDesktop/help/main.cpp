/****************************************************************************
** $Id: main.cpp,v 1.1 2005/06/15 11:08:15 jahshaka Exp $
**
** Copyright (C) 1992-2000 Trolltech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "helpwindow.h"
#include <qapplication.h>
#include <qdir.h>
#include <stdlib.h>


int main( int argc, char ** argv )
{
    QApplication::setColorSpec( QApplication::ManyColor );
    QApplication a(argc, argv);

    QString home;
    if (argc > 1) {
        home = argv[1];
    } else {
	// Use a hard coded path. It is only an example.
	//home = "http://www.jahshaka.org";
    	home = QDir( "/usr/lib/qt-3.3/doc/html/index.html" ).absPath();
    }

    HelpWindow *help = new HelpWindow(home, ".", 0, "help viewer");
    help->setCaption("Qt Example - Helpviewer");
    if ( QApplication::desktop()->width() > 400
	 && QApplication::desktop()->height() > 500 )
	help->show();
    else
	help->showMaximized();

    QObject::connect( &a, SIGNAL(lastWindowClosed()),
                      &a, SLOT(quit()) );

    return a.exec();
}
