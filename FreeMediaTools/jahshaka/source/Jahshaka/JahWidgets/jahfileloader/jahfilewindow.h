/*******************************************************************************
**
** The header file for the Jahshaka jahfilewindow module 
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef MAINWIN_H
#define MAINWIN_H

#include "jahfileiconview.h"
#include "dirview.h"

#include <qsplitter.h>
#include <qprogressbar.h>
#include <qlabel.h>
#include <qstatusbar.h>
//#include <qtoolbar.h>
#include <qcombobox.h>
#include <qpixmap.h>
#include <qtoolbutton.h>
#include <qdir.h>
#include <qfileinfo.h>

//added by jahshaka
#include <qdialog.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include "jahpreferences.h"

class QtFileIconView;
class DirectoryView;
class QProgressBar;
class QLabel;
class QComboBox;
class QToolButton;

class FileMainWindow : public QDialog
{
    Q_OBJECT

public:
    FileMainWindow( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~FileMainWindow();

	void setPath( const QString path );
	QString getRoot( );
	QString getPath( );
    QtFileIconView *fileView() { return fileview; }
    DirectoryView *dirList() { return dirlist; }

	QValueList < QString > m_selected_items;

protected:
    void setup();
    void setPathCombo();

    QtFileIconView *fileview;
    DirectoryView *dirlist;
    //QProgressBar *progress;
    //QLabel *label;
    QComboBox *pathCombo;
    QComboBox *driveCombo;
    QToolButton *upButton, *mkdirButton;
    QToolButton *viewText, *viewIcon, *viewImage;
    
    //added by jahshaka
    QGridLayout* Form1Layout;
    QGridLayout* Form2Layout;
    QGridLayout* Form3Layout;
    QGridLayout* titlebarLayout;

    QFrame* titlebar;
    QSplitter* preview;
    QFrame* feedback;

    QProgressBar *progress;
    QLabel *label;
    QPushButton *loadbutton;
    QPushButton *closebutton;

    Directory *root;

public slots:
    void cdUp();

protected slots:
    virtual void languageChange();

    void directoryChanged( const QString & );
    void slotStartReadDir( int dirs );
    void slotReadNextDir();
    void slotReadDirDone();
    void newFolder();
    void changePath( const QString &path );
    void enableUp();
    void disableUp();
    void enableMkdir();
    void disableMkdir();

    void changeWinDir(int drive);

    //added by jahshaka
    void textWasChanged(const QString & );
    void loadthisfile( QString );
    void setthefilename( QString );
    void returnFile( void );
    void returnNull( void );


};

#endif
