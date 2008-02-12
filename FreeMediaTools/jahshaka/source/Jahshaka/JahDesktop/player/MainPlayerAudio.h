/*******************************************************************************
**
** The header file for the Jahshaka player module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

/* contains code from flip - pageflipper for TIFF animations
 * Copyright (C) 1999 Mark B. Allan (mba@reptilelabour.com)
 *
 * "flip" is free software; you can redistribute it and/or use
 * it and/or modify it under the terms of the "Artistic License"
 */

#ifndef MainWindowAudio_h
#define MainWindowAudio_h


#ifndef _WIN32
#include <unistd.h>
#include <sys/time.h>
#else
#include <sys/timeb.h>
#endif

#include <stdio.h>

#include <qsettings.h>
#include <qtimer.h>
#include <qmultilineedit.h>
#include <qkeycode.h>
#include <qmessagebox.h>
#include <qtabwidget.h>


//used for videodata support
#include "openmedialib.h"

//custom ui objects
#include "widget.h"
#include "InputLCD.h"

//included from libraries/jahplayer
//#include "TiffFlip.h"
#include "diskplayer.h"

//for the tracer
#include "jahtracer.h"

//for preferences global
#include "jahpreferences.h"
#include "jahformatter.h"
#include "jahtranslate.h"

#include "slickslider.h"

//for the plotter to be waveform / vectorscope
#include "levelswidget.h"
#include "videohead.h"

/////////////////////////
// for audio support
#include "Audio_OSS.h"
#include "Lock.h"

#define SIZE_50         0
#define SIZE_100        1
#define SIZE_200        2
#define LOOP_FORWARD    0
#define PING_PONG       1

//====================================================================
class MainPlayer : public QWidget
{
        Q_OBJECT

private:
	QWidget *m_parent;
	QWidget *m_parent1;
	QHBox *m_control;
	QHBox *m_controller;
	QHBox *m_leftcontroller;
	QHBox *m_rightcontroller;
	bool m_started;

public:
    MainPlayer(QWidget *parent, const char* name,
    QHBox* parent1,
    QHBox* control, QHBox* controler, QHBox* leftcontroller, QHBox* rightcontroller );
    ~MainPlayer();

	void activate( );

    //the asset to be loaded in the player
    assetData theassetData;
    QString    ModuleName;

    enum FPS { F90, F60, F48, F30, F24, F20, F15, F12, F10,NumFPS };
	
	//pointer to translater singleton
    JahTranslate        * jt;
	void createMenuItem(QPopupMenu * themenu );

    static double fpsVal[NumFPS];

    bool playonce;

    void	loadPreferences();

    void	ClearOut();
    void	BlastImage(QImage&);
    void	LoadMySequence(assetData desktopclip); //was just assetData

    //added for default to play from disk
    void	LoadDiskSequence();
    void	PlayDiskSequence(assetData &desktopclip);

    //new widgets for the scopes
    LevelsWidget *colorscope, *vectorscope, *waveform;
    void updateScopes(int theframe=0, bool check=0);

    //preferences variables

    bool videohead;
	int  vhXres, vhYres;
    //int videoheadx, videoheady;



    //builds the controler
    //void addJahPlayerButton( JahToolButton * button, int x1, int y1, int x2, int y2,
    //                        int w, int h, QString iconOff, QString iconOn );

    //void    addSliderLcd( InputLCD * lcd, int x, int y, int w, int h, int range);
    void    buildController( QHBox* controller);
    void    buildInterfaceMenus( QHBox* leftcontroller, QHBox* rightcontroller);
    void    buildInterface( QHBox* qtinterface );

    void	UpdateAssetData(assetData &desktopclip);

    // audio
    void LoadAudio (AudioData audio_data);

    //for videohead support
    void	launchVideoHead(void);
    void    initializeVideoHead(void);
    
    void	activateVideoHead(void);
    void	deactivateVideoHead(void);

protected slots:

    //void	UpdatePlayer2(QPixmap thepix);
    void	UpdateDualHead();

    //used to flip desktop display modes in player
    void    toggleDesktopImages();
    void    toggleDesktopTable();

    void	UpdateLoadStatus(int);
    void	LoadSequenceCompleted(int sf, bool aborted);

    void	SetFrameNumber(int);
    void    switchModeFps(void);
    void	SetFPS(FPS);
    void	switchFps(int fpsval);
    void    switchModeStep(void);
    void	SetStep(int);
    
    void	SetZoom(int);
    void	toggleZoomLock(void);
    void	toggleZoomReset(void);
    void	updateZoomIn(void);
    void	updateZoomOut(void);
   

    void	SetMinFrame(int);
    void	SetMaxFrame(int);
    void	SetRamMinFrame(int);
    void	SetRamMaxFrame(int);

    void	Clear();

    //for the ram player
    void	launchRamPlayer();
    void	toggleRamPlayer();
    void	toggleDiskPlayer();

	//for color view modes
    void	toggleViewAll(void);
    void	toggleViewRed(void);
    void	toggleViewGreen(void);
    void	toggleViewBlue(void);
    void	toggleViewAlpha(void);

    //for playmode
    void    setPlayMode(void);

    void    draggedAImage();

    void    sliderValueChanged(int val);

    void JumpToFrame ();	// jump to frame when the user releases the slider

    void ScrollPressed ();	// sets the flag that user pressed the slider

public slots:

    void	LoopForward();
    void	setLoopForward();
    void	PingPong();
    void	Stop();
    void	FastForward();
    void	FrameForward();
    void	FrameBackward();
    void	Rewind();
    void	Action();
    void	ActionLoopForward();
    void	ActionPingPong();

	//for use in the ram player
	void	loadRamFrame(void);

signals:
    void toggleTheDesktopImages();
    void toggleTheDesktopTable();

    void playerDraggedAImage();


private:
    ////////////////////////////////////////
    //pointer to tracer singleton
    JahTrace            * jtrace;
    
    QString JahBasePath;

    bool videoheadInitialized;

    //TiffFlip		*tiffFlip;
    JahPlayer		*jahplayer;

	unsigned long slider_res;

    int			fpsID[NumFPS];

    int			step1_ID;
    int			step2_ID;
    int			step3_ID;
    int			step4_ID;
    int			step5_ID;
    int			step6_ID;


    int			zoom1_ID;
    int			zoom2_ID;
    int			playMode;

    QTimer		*timer;

	/////////////////////////////////////////////////
	// trying to implement ramplayer locally
	bool	ToggleRamPlayerNow(void);
	void	CleanupRamPlayerNow(void);
	

	//for memory tests
	void	OutOfMemory();
    bool	checkMem();
	//only works on linux
	bool	getMemUsage(float* mem, float* swap);

	QImage	**pixlist;
	bool inDaRam;
	int numFrames;
	int loadFrame;

	int raminframe;
	int ramoutframe;

	QTimer		ramloadtimer;

	//for memory tests
    bool	doMemCheck;
    float	begSwap;
    float	begMem;
    float	curSwap;
    float	curMem;
    float	swapThresh;
    QString	memString;
    QString swapString;

	/////////////////////////////////////////////////
    unsigned long	curFrame; // in audio: current file pointer (in samples)
    unsigned long	minFrame; // in audio: 0
    unsigned long	maxFrame; // in audio: max samples
    unsigned long	ramminFrame; // in audio: 0
    unsigned long	rammaxFrame; // in audio: max samples
    int             startFrame;
    int             pingpong;
    double          currentSpeed;
    unsigned long	frameStep;// in audio: buffer size in samples (1 channel)

    unsigned short framerateCounter;

	int		accumulated;
    struct	timeval	oldTime;


    //////////////////////////////////////////////////////////////////////////
    // audio hack starts here

    IO::AUDIO_OSS _oss_player; // oss audio device representation
    bool _is_audio;		// object to play is audio

    AudioData _audio_data;	// info about audio file
    MutexLock _audio_lock;	// audio player mutex lock

    static unsigned long  _PlayerCallback (float* buf, unsigned long size, void* arg) {
        return reinterpret_cast<MainPlayer*>(arg)->PlayerCallback (buf, size);
    }

    // this method is called by player in order to get a new block
    // of file to play.
    unsigned long PlayerCallback (float* buf, unsigned long size);

    int _frame_to_set;	// when the user drags the slider - it's value stored there
    bool _is_slider_pressed; // indicates that the user has pressed the slider

    ///////////////////////////////////////////////////////////////////////////
    // audio hack ends here



    //////////////////////////////////////////////////////////////////////
    //player interface objects
    QHBoxLayout * mainplayerLayout;


    QFrame * playerframe;
    QFrame * mainplayerframe;
    VideoHead * playerClip;

    QGridLayout * jahshakaLayout;
    QFrame * mainworld;
    QHBoxLayout * mainworldLayout;
    QFrame * player;

    QHBox * playerh;
    QVBox * playerv;

    //scene control variables
    QWidgetStack* controlpanel;
    QFrame* scene_controls;
    QFrame* object_controls;


    QFrame* maincontroller;
    QWidgetStack* MainController;
    QWidget* page;
    QFrame* forceplayframe;
    QCheckBox* forceplaycheckbox;
    QFrame* controllerframe;
    QFrame* TimecodeFrame;

    QPushButton* controllerpreviousbutton;
    QPushButton* controllerstopbutton;
    QPushButton* controllerplaybutton;
    QPushButton* controllerffworardbutton;
    QPushButton* controllernextbutton;
    QPushButton* controllerrewindbutton;

    SlickSlider* controllerslider;
    QFrame* timecodeframe;
    QPushButton* keyframebutton;
    QLCDNumber* timecodedisplay;
    QFrame* imagingframe;
    QPushButton* imagingphotobutton;
    QPushButton* imagingrenderbutton;

    //protected:
    QHBoxLayout* Form1Layout;
    QHBoxLayout* maincontrollerLayout;
    QHBoxLayout* pageLayout;

    //////////////////////////////////////////////////////////////////////
    //controller variables

    //for the left and right menus
    QFrame         *glworldLeft;
    JahToolButton   *tooltranslate, *toolscale, *toolrotate;
    JahToolButton  *toolthumbview, *toolbinview;

    QFrame         *glworldRight;
    JahToolButton  *toolzoomin, *toolzoomout;
    JahToolButton  *toolzoomreset, *toolzoomlock;
    JahToolButton  *toolShowRed, *toolShowGreen, *toolShowBlue, *toolShowAlpha, *toolShowAll;
    QSlider        *zoom;

    //for the player

    QFrame* controls;
    QHBox* controlers;

    //this is to put the player in a 2nd head
    QWidget * playerdialog;

    QPushButton* rewButton;
    QPushButton* pingButton;
    QPushButton* stopButton;

    QMenuBar	*menuBar;
    QPopupMenu	*filePopup;
    QPopupMenu	*utilPopup;

    QVBoxLayout	*topLayout;
    QHBoxLayout	*middle;
    QVBoxLayout	*midR;
    QVBoxLayout	*midM;
    QVBoxLayout	*midL;
    QHBoxLayout	*buttons;
    QHBoxLayout	*status;


    QScrollBar	*frameScroll;

    InputLCD	*minFrameInput;
    InputLCD	*maxFrameInput;

    InputLCD	*ramminFrameInput;
    InputLCD	*rammaxFrameInput;
    InputLCD	*ramtotalFrameInput;

    QString		frameNumString;
    QLabel		*statusline;
    QLabel		*frameNum;
    QLabel		*actualFps;


    //QCheckBox    * forcedplay;
    QPushButton    * playmodecombo;

    JahToolButton  * scrubfirst, * scrubprevious, * scrubstop, * scrubplay, * scrubnext, * scrublast;
    QPushButton  * scrubkey, * scrubrender, * scrubrenderAll;

    //for the rest of the interface
    QHBox*       MiddleFrame;		QSlider *     Slider2;
    InputLCD*    animframe;		QLabel *      aframetext;  //was QLCDNumber
    QLabel*      sframetext;		InputLCD *    lcdstartframe;
    QLabel*      eframetext;		InputLCD *    lcdendframe;

    QLabel *maxtext, *mintext; 
    QLabel *rammaxtext, *rammintext; 
    QLabel *statustext, *fpstext;
    QLabel *playerText, *ramplayerText, *playeroutputtext;


    //new asset data
    QLabel* l_filename;
    QLabel* l_extension;
    QLabel* l_assettype;
    QLabel* l_location;
    QLabel* l_timecode;
    QLabel* l_resolution;
    QLabel* l_width;
    QLabel* l_height;
    QLabel* l_startframe;
    QLabel* l_endframe;
    QLabel* l_assetname;
    QLineEdit* l_assetname_edit;
    QLineEdit* l_filename_edit;
    QLineEdit* l_assettype_edit;
    QLineEdit* l_extension_edit;
    QLineEdit* l_location_edit;

    InputLCD* lcd_startframe;
    InputLCD* lcd_endframe;

    QLineEdit* l_height_edit;
    QLineEdit* l_width_edit;
    QFrame *divider, *divider2;

    //for the new interface
    QFrame*      LayersFrame;

    QWidgetStack*  EffectsMain;
    QFrame*        SceneFrame;
    QFrame*        EffectsFrame;
    bool           EffectsStatus;

    QTabWidget*        tabframe;
    QWidget*        objectcontrols;
    QWidget*        texturecontrols;
    QWidget*        scenecontrols;
    QWidget*        effectscontrols;

    QFrame*        ControlFrame0;
    QFrame*        ControlFrame1;
    QFrame*        ControlFrame2;
    QFrame*        sceneframe;

    QMultiLineEdit* playerOutput;

    //for the mpeg encoder
    QPushButton * mpegPlay, *aviEncode;

    //for the ram player
    QCheckBox *RamPlayerStatus;
    
    QImage * cacheImage;
 
	//this shopuld be static or global to the class
    assetExchange * exchange;
};

#endif // MainWindow_h



