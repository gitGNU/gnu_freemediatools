/****************************************************************************
** $Id: helpwindow.h,v 1.1 2005/06/15 11:08:15 jahshaka Exp $
**
** Copyright (C) 1992-2000 Trolltech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <qmainwindow.h>
#include <qtextbrowser.h>
#include <qurloperator.h>
#include <qnetwork.h>
#include <qstringlist.h>
#include <qmap.h>
#include <qdir.h>

class QComboBox;
class QPopupMenu;

class HelpWindow : public QMainWindow
{
  //    Q_OBJECT
public:
    HelpWindow( const QString& home_,  const QString& path, QWidget* parent = 0, const char *name=0 );
    virtual ~HelpWindow();

private slots:
    void setBackwardAvailable( bool );
    void setForwardAvailable( bool );

    void sourceChanged( const QString& );
    void about();
    void aboutQt();
    void openFile();
    void newWindow();
    void print();

    void pathSelected( const QString & );
    void histChosen( int );
    void bookmChosen( int );
    void addBookmark();

private:
    void readHistory();
    void readBookmarks();

    QTextBrowser* browser;
    QComboBox *pathCombo;
    int backwardId, forwardId;
    QStringList history, bookmarks;
    QMap<int, QString> mHistory, mBookmarks;
    QPopupMenu *hist, *bookm;

    //    QString tr( const char *);

};





#endif

