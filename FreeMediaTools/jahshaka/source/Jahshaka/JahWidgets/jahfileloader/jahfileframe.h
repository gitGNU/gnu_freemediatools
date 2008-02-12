/*******************************************************************************
**
** The header file for the Jahshaka jahfilewindow module 
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef MAINWINFRAME_H
#define MAINWINFRAME_H

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

//added by jahsshaka
#include <qdialog.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qhbox.h>
#include "jahpreferences.h"

class QtFileIconView;
class DirectoryView;
class QProgressBar;
class QLabel;
class QComboBox;
class QToolButton;

class FileMainFrame : public QWidget
{
    Q_OBJECT

public:
    FileMainFrame( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~FileMainFrame();

    QtFileIconView *fileView() { return fileview; }
    DirectoryView *dirList() { return dirlist; }

	QValueList < QString > m_selected_items;

protected:
    void setup();
    void setPathCombo();

    QtFileIconView *fileview;
    DirectoryView *dirlist;
    Directory *root;

    QHBoxLayout *mainlibraryLayout;
    QFrame *thelibrary;

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


protected slots:
    virtual void languageChange();

    void directoryChanged( const QString & );
    void slotStartReadDir( int dirs );
    void slotReadNextDir();
    void slotReadDirDone();
    void cdUp();
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

signals:

    void loadButton();

};

#endif
