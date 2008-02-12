/*******************************************************************************
**
** The header file for the Jahshaka player module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef DESKTOP_H
#define DESKTOP_H

#include "glworld.h"

#include <qmessagebox.h>
#include <qwidgetstack.h>
#include <qtable.h>
#include <qdropsite.h>
#include <qfile.h>

#include "desktopcanvas.h"
#include "desktoptable.h"
#include "assettable.h"

#include <qcanvas.h>

class JahTrace;
class JahDatabase;

class JahDesktop : public QWidget, public AssetTable {
  Q_OBJECT

protected:
	QString	m_current_root_directory;
	QString	m_current_load_directory;

public:
	QString getCurrentLoadDirectory() { return m_current_load_directory; }
	void setCurrentLoadDirectory(const QString directory) { m_current_load_directory = directory; }
	QString getCurrentRootDirectory() { return m_current_root_directory; }
	void setCurrentRootDirectory(const QString directory) { m_current_root_directory = directory; }

public:
    JahDesktop( QHBox* parent=0,QHBox* parent1=0,QHBox* parent2=0, 
                QHBox* leftcontroller=0, QHBox* rightcontroller=0,
                const char* name=0, int * globalclipnumber=0);

    ~JahDesktop();

	void activate( );

    //used to build the interface
    void buildInterfaceMenus( QHBox* leftcontroller, QHBox* rightcontroller);
	void createMenuItem(QPopupMenu * themenu );

    //routines to initalize widgets
    void initDesktopCanvas();
    void initDesktopTable();

    //other important variables
    int *clipnumber;
    bool widescreen;
    int globaltablenumber;

    //used to name clips and get their names
    void setClipname(QString&);
    QString getClipname();

    //used to stop animation on the desktop
    void stopanimation();

    //asset management routines
    bool loadJahAsset(QString filename = 0, bool loadit = 1);
	void addAssetToDesktop(assetData thedata, int clipn, bool loadit, QImage);
    assetData getVideoData(bool & status);
	void selectNextAsset( );
	void syncMediaTable( assetData );
    ImageItem* getCurrentItem();	// audio hack

    //used to direct loading of scene files to correct module
    bool handleJahSceneFile(QString);
    bool handle3dFile(QString);
	QString findModuleName      (QString loadfilename);
    
    //used to do a canves refresh
    void doRefresh(void);

	///////////////////////////////////////////////////////////////////
	//used to set up custom mediatable
    void                initializeMediaTable(QHBox* parentbox);
	JahDesktopSideTable *mediatable;

	////////////////////////////////////////
	//pointer to translater singleton
	JahTranslate		* jt;
    
	QString    ModuleName;

	JahDatabase *getJahDatabase( ) { return theDatabase; }
	QCanvasItemList allItems( );

	void selectAsset( assetData & );

protected:
    ///////////////////////////////////////////////////////////////
    void dragEnterEvent(QDragEnterEvent* event);
    void dropEvent(QDropEvent* event);

public slots:

    void RebuildDesktop();

    ImageItem* addImage2( assetData thedata, QImage );
    ImageItem* addImage2( assetData thedata, double& xsize, double& ysize, QImage );
    ImageItem* addImage( assetData thedata, QImage );
    void addItemToDesktopTable(int index, assetData thedata, QImage );

    //database calls
    void renameImageindatabase( int imagenumber, QString newname);
    void addAssetTodatabase(assetData thedata);
    void updateClipPosition(assetData data);
    void updateClipSize(assetData data);
    void updateLockStatus(assetData data);
    void updateFilmstripStatus(assetData data);
    void slotUpdateFrameOffset(assetData data);

    virtual QString    getModuleName();

	//loaders
    void slotExport();
    void slotImport();
    void slotLoadIconview();
    void clearItem();
    void clearAll();
    void tidyDesktop();
    void slotExpand(); // (to create filmstrip).

    //for mode switching
    void toggleDesktopImages();
    void toggleDesktopTable();

	void changeModePlayer( );

signals:

    void resetGlobalClipNumber();
    void notifyPlayerOfChange();

	void switchMainModule(QString);
	
	void loadMainModuleFile(QString);
	void loadMainModelFile(QString);

	// Signals to connect rebuild desktop to the splash screen
	void addProgressSteps( int );
	void updateProgress( );

private slots:

    bool clearTheDesktop();

    bool eraseTheAsset(assetData erase);
    bool exportTheAsset(QString exportto, assetData theasset);

    //void init();
    void addRectangle();
    void slotSave();
    void slotLoad();

    //used to flip desktop display modes
    void selectTableRow(int row, int col);

	// Asset selection event
	void assetSelected( assetData );

private:

    bool isSceneExtension(QString & string);
    bool is3dExtension(QString & string);

	////////////////////////////////////////
	//pointers to singletons
	JahTrace			* jtrace;

    /////////////////////////////////////////////////
    //desktop interface variables

    //for the left and right menus
    QFrame         *glworldLeft;

    JahToolButton  *toolbinview, *toolthumbview;

    QFrame         *glworldRight;
    QPushButton    *toolzoomin, *toolzoomout;
    QSlider        *zoom;

    //for the canvas/table switching
    QHBoxLayout * themaindesktopLayout;
    QWidgetStack * theDesktopLayout;
    QHBoxLayout * themaindesktopLayout2;
    QWidgetStack * theDesktopLayout2;

    QHBox * thedesktopdisplay1;
    QHBox * thedesktopdisplay2;
    //QHBox * thedesktopdisplay3;

    QHBoxLayout *themaindesktopWideLayout;
    QWidgetStack *theDesktopLayoutWide;

    QHBox *thedesktopdisplaywide;

    /////////////////////////////////////////////////
    // desktop core variables

    //pointer to the sqlite database
    JahDatabase * theDatabase;

    //QTable * table;
    JahDesktopTable * table;

    FigureEditor *editor, *shareeditor;
    QCanvas *canvas, *canvasshare;

    QImage* img;

    QString JahBasePath;
    QString JahMediaPath;

	bool m_started;
};

#endif




