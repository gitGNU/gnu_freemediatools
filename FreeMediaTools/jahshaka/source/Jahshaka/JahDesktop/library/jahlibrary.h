/*******************************************************************************
**
** The header file for the Jahshaka jahfilewindow module 
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef JAHLIBRARY_H
#define JAHLIBRARY_H

#include "jahfileiconview.h"
#include "dirview.h"

#include <qsplitter.h>
#include <qprogressbar.h>
#include <qlabel.h>
#include <qstatusbar.h>
#include <qcombobox.h>
#include <qpixmap.h>
#include <qtoolbutton.h>
#include <qdir.h>
#include <qfileinfo.h>

//added by jahshaka
#include <qdialog.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qhbox.h>
#include "jahpreferences.h"

//added for new assetmanager ui

class QtFileIconView;
class DirectoryView;
class QProgressBar;
class QLabel;
class QComboBox;
class QToolButton;
class AssetManMainWidget;

#include "iconprovider.h"

/////////////////////////////////////////////////////////
// currently we have duplicated the code in the jahfileloader
// specifically the jahfileframe.cpp widget
// a more efficient implementation would be to embed that
// widget in here

class JahLibrary : public QWidget
{
    Q_OBJECT

private:
	QWidget *m_parent;
	QHBox *m_leftframe;
	QHBox *m_rightframe;
	QHBox *m_controls;
	QHBox *m_leftcontroller;
	QHBox *m_rightcontroller;
	bool m_started;

public:

    JahLibrary( QWidget* parent = 0, const char* name = 0, QHBox* leftframe = 0,QHBox* rightframe = 0,
        QHBox* controls = 0, QHBox* = 0, QHBox* leftcontroller = 0, QHBox* rightcontroller = 0,
        bool = FALSE, WFlags = 0 )
        : QWidget( parent, name )
		, m_parent( parent )
		, m_leftframe( leftframe )
		, m_rightframe( rightframe )
		, m_controls( controls )
		, m_leftcontroller( leftcontroller )
		, m_rightcontroller( rightcontroller )
		, m_started( false )
		, fileview( 0 )
		, dirlist( 0 )
		, root( 0 )
    {
		ModuleName = name;
    }

    ~JahLibrary();


    virtual QString    getModuleName();

    void activate( );

    void buildInterface( QHBox* qtinterface);
    void buildInterfaceMenus( QHBox* leftcontroller, QHBox* rightcontroller );

    void initializeModule(QWidget* parent,
			  QWidget* userspace,
			  QHBox* theleftframe,
			  QHBox* therightframe);

    void createMenuItem( QPopupMenu * themenu );

    QtFileIconView *fileView() { return fileview; }
    DirectoryView *dirList() { return dirlist; }

	void setDirectory( const QString dir ) { directory = dir; }

	QValueList < QString > m_selected_items;
    
	QString    ModuleName;

	AssetManMainWidget *ammw;

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

    //new code added here
    QFrame* librarypanel; 
    QPushButton* importclip;
    QPushButton* exportclip;

	QString directory;

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
    void returnFile( void  );
    void returnNull( void );

    //new code added here
    void clickedImportButton();

signals:
    void loadButton();

    //new code added here
    void importButton();
};

#endif
