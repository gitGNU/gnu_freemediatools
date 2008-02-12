/*******************************************************************************
**
** The header file for the Jahshaka player module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef MainWindow_h
#define MainWindow_h

#ifndef WIN32
#include <unistd.h>
#include <sys/time.h>
#else
#include <sys/timeb.h>
#include <windows.h>
#endif

#include <qwidget.h>
#include <assetdata.h>

class QGLWidget;
class JahTranslate;
class LevelsWidget;
class JahTrace;
class QImage;
class QFrame;
class QGridLayout;
class QCheckBox;
class QPushButton;
class TimelineSlider;
class QLCDNumber;
class JahToolButton;
class InputLCD;
class QSlider;
class QPopupMenu;
class QVBoxLayout;
class QScrollBar;
class QLabel;
class QTabWidget;
class QMultiLineEdit;
class QtPlayer;
class QHBox;
class QHBoxLayout;
class QVBox;
class QMenuBar;
class QTable;
class QComboBox;
class JahHeadable;
class SupergRangeControl;

#define SIZE_50         0
#define SIZE_100        1
#define SIZE_200        2
#define LOOP_FORWARD    0

enum mainplayer_mode_t { SHOW_ALL, SHOW_RED, SHOW_GREEN, SHOW_BLUE, SHOW_ALPHA };

//====================================================================
class MainPlayer : public QWidget
{
        Q_OBJECT

private:
	const QGLWidget *m_core;
	QWidget *m_parent;
	QWidget *m_parent1;
	QHBox *m_control;
	QHBox *m_controller;
	QHBox *m_leftcontroller;
	QHBox *m_rightcontroller;
	bool m_started;

	mainplayer_mode_t m_mode;

public:
    MainPlayer( const QGLWidget *, QWidget *, const char *, QHBox *, QHBox *, QHBox *, QHBox *, QHBox * );
    ~MainPlayer();

    virtual QString    getModuleName();

	void activate( );

    //the asset to be loaded in the player
    QString    ModuleName;

	void createMenuItem(QPopupMenu * themenu );

    void	loadPreferences();

    void	LoadMySequence(assetData desktopclip); //was just assetData

    //added for default to play from disk
    void	LoadDiskSequence();
    void	PlayDiskSequence(assetData &desktopclip);

    //new widgets for the scopes
    LevelsWidget *colorscope, *vectorscope, *waveform;
    void updateScopes( QImage &image, bool check=0 );

    void    buildController( QHBox* controller);
    void    buildInterfaceMenus( QHBox* leftcontroller, QHBox* rightcontroller);
    void    buildInterface( QHBox* qtinterface );

    void	UpdateAssetData(assetData &desktopclip);


protected:
	void keyPressEvent( QKeyEvent * );
	void wheelEvent ( QWheelEvent * );

protected slots:

    //used to flip desktop display modes in player
    void    toggleDesktopImages();
    void    toggleDesktopTable();

    void	LoadSequenceCompleted( );

    void	SetFrameNumber(int);
    
    void    switchModeFps( int );
    void    switchModeStep( int );

    void	SetZoom(int);
    void	toggleZoomLock(void);
    void	toggleZoomReset(void);
    void	updateZoomIn(void);
    void	updateZoomOut(void);

    void	SetMinFrame(int);
    void	SetMaxFrame(int);

	//for color view modes
    void	toggleViewAll(void);
    void	toggleViewRed(void);
    void	toggleViewGreen(void);
    void	toggleViewBlue(void);
    void	toggleViewAlpha(void);

    //for playmode
    void    setPlayMode( int );

    void draggedAImage( );
	void requestNextItem( );

    void sliderValueChanged( int val );
    void sliderPressed( );
    void sliderReleased( );

public slots:
	
	void showImage( int position, QImage *image );
	void realTimeChanged( bool );

    void	LoopForward();
    void	Stop();
    void	FastForward();
    void	FrameForward();
    void	FrameBackward();
    void	Rewind();

	void toggleRamPlayer( );

	void showHead( JahHeadable * );

signals:
    void toggleTheDesktopImages();
    void toggleTheDesktopTable();

	void syncMediaTable( assetData );
    void playerDraggedAImage( );
	void playerRequestsNextItem( );

	void showInHead( JahHeadable * );

private:
    //pointer to translater singleton
    assetData theassetData;
    JahTranslate        * jt;

    QString JahBasePath;
	QString current_file_;
	QTable *asset_table;

    QtPlayer	*jahplayer;
	int			playMode;

	/////////////////////////////////////////////////

	unsigned long	minFrame; // in audio: 0
	unsigned long	maxFrame; // in audio: max samples
	int             startFrame;
	unsigned long	frameStep;// in audio: buffer size in samples (1 channel)

    //scene control variables
    TimelineSlider* controllerslider;
    QPushButton* keyframebutton;
    QLCDNumber* timecodedisplay;
    QPushButton* imagingphotobutton;
    QPushButton* imagingrenderbutton;

    //////////////////////////////////////////////////////////////////////
    //controller variables

    //for the left and right menus
    JahToolButton  *toolthumbview, *toolbinview;

    QFrame         *glworldRight;
    JahToolButton  *toolzoomin, *toolzoomout;
    JahToolButton  *toolzoomreset, *toolzoomlock;
    JahToolButton  *toolShowRed, *toolShowGreen, *toolShowBlue, *toolShowAlpha, *toolShowAll;
    QSlider        *zoom;

    QComboBox    * playmodecombo;

    JahToolButton  * scrubfirst, * scrubprevious, * scrubstop, * scrubplay, * scrubnext, * scrublast;
    QPushButton  * scrubkey;
	QComboBox *scrubrenderAll;
	QComboBox *scrubrender;

    //for the player
	InputLCD	*minFrameInput;
	InputLCD	*maxFrameInput;

    //for the rest of the interface
    SupergRangeControl*    animframe;

    //new asset data
    QLabel* l_assetname_edit;
    QLabel* l_filename_edit;
    QLabel* l_assettype_edit;
    QLabel* l_extension_edit;
    QLabel* l_location_edit;
    QLabel* l_resolution_edit;

    QFrame*        ControlFrame1;
};

#endif // MainWindow_h
