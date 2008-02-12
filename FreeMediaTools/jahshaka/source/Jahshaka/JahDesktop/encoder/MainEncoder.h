/*******************************************************************************
**
** The header file for the Jahshaka Encoder Module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef MainEncoder_h
#define MainEncoder_h

#ifndef WIN32
#include <unistd.h>
#include <sys/time.h>
#else
#include <sys/timeb.h>
#include <windows.h>
#endif

#include <qwidget.h>

/*
namespace jahnle
{
	class Render;
}
*/

/*
namespace Mlt
{
	class Properties;
}
*/

class QHBox;
class QVBox;
class QListView;
class QPopupMenu;
class JahToolButton;
class SupergRangeControl;
class QFrame;
class QWidgetStack;
class QCheckBox;
class TimelineSlider;
class QLCDNumber;
class QSlider;
class QScrollBar;
class QTabWidget;
class QPushButton;
class QHBoxLayout;
class QGridLayout;
class QVBoxLayout;
class QMenuBar;
class QLabel;
class QLineEdit;
class QSpinBox;
class QComboBox;
class QGLWidget;

class JahTrace;
class QStringList;

/////////////////////////////////

//used for videodata support
#include "assetdata.h"

class QtPlayer;
class JahTranslate;

#define SIZE_50         0
#define SIZE_100        1
#define SIZE_200        2
#define LOOP_FORWARD    0
#define PING_PONG       1

//====================================================================
class MainEncoder : public QWidget
{
        Q_OBJECT
private:
	const QGLWidget *m_core;
	QWidget *m_parent;
	QHBox *m_control;
	QHBox *m_controller;
	QHBox *m_leftcontroller;
	QHBox *m_rightcontroller;
	bool m_started;

public:
    virtual QString    getModuleName();

	/*
    jahnle::Render*         getJahNleRender() { return m_jahnle_render; }
    Mlt::Properties*        getMltProperties() { return m_mlt_properties; }
	*/
    JahTranslate*           getJahTranslate() { return m_jah_translate; }
    QStringList*            getRenderFormatsStringList() { return m_render_formats_stringlist; }
    QStringList*            getRenderProfilesStringList() { return m_render_profiles_stringlist; }

protected:
    //    jahnle::Render*          m_jahnle_render;
    //	Mlt::Properties*         m_mlt_properties;
    QStringList*             m_render_formats_stringlist;
    QStringList*             m_render_profiles_stringlist;

    JahTranslate*           m_jah_translate;

public:
    MainEncoder(const QGLWidget *, QWidget *parent, const char* name,
    QHBox* control, QHBox* controller, QHBox* leftcontroller, QHBox* rightcontroller );
    ~MainEncoder();

	void activate( );

    //the asset to be loaded in the player
    assetData theassetData;
    QString    ModuleName;

    enum FPS { F90, F60, F48, F30, F24, F20, F15, F12, F10, NumFPS };

    static double fpsVal[NumFPS];
    bool playonce;
    ///////////////////////////////////////////////
    //added for the encoder
    void	LoadMySequence(assetData desktopclip); //was just videoData

	void createMenuItem( QPopupMenu * themenu );

    ////////////////////////////////////////////
    //added for default to play from disk
    void	ClearOut();

    void    buildController( QHBox* controller);
    void    buildInterface( QHBox* qtinterface );

signals:
    void encoderDraggedAImage();

public slots:

	void showImage( int position, QImage *image );

    void slotGenerateRenderFormatList();
    void slotGenerateRenderProfileList();
    void slotSelectRenderFormat(int);
    void slotSelectRenderProfile(int);
    void slotStartRender();

	void sliderPressed( );
	void sliderReleased( );

protected slots:

    //////////////////////////////////////////////////////////
    // for the player

    void	LoadSequenceCompleted(int sf, bool aborted);

	void sliderValueChanged(int fr);

    void	SetFrameNumber(int);
    void	SetFPS(FPS);
	void	switchFps(int fpsval);
    void	SetStep(int);
    void	SetZoom(int);
    void	ResizeDisplay(mediaSize);
    void	loadPreferences();

    void	SetMinFrame(int);
    void	SetMaxFrame(int);

    void    draggedAImage();


    void	Clear();
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

private:
	////////////////////////////////////////
	//pointer to tracer singleton
	JahTrace			* jtrace;

    QString JahBasePath,JahMediaPath;

    QtPlayer		*encplayer;

    bool encodingStatus;

    int			fpsID[7];

    int			zoom1_ID;
    int			zoom2_ID;


    unsigned long	curFrame; // in audio: current file pointer (in samples)
    unsigned long	minFrame; // in audio: 0
    unsigned long	maxFrame; // in audio: max samples
    int             startFrame;
    int             pingpong;
    double          currentSpeed;
    unsigned long	frameStep;// in audio: buffer size in samples (1 channel)

	unsigned short framerateCounter;
	int		accumulated;

    bool _is_audio;		// object to play is audio

    //////////////////////////////////////////////////////////////////////
    //player interface objects

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

    TimelineSlider* controllerslider;
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

    QLabel*             m_render_normalisation_label;
    QComboBox*          m_render_normalisation_combobox;

    QComboBox*          m_render_formats_combobox;
    QPushButton*        m_render_formats_button;
    QPushButton*        m_render_profiles_button;

    QComboBox*          m_render_profiles_combobox;
    QLabel*             m_render_profiles_label;
    QLabel*             m_render_formats_label;

    QPushButton*        m_render_button;

    //////////////////////////////////////////////////////////////////////
    //controller variables
    int    Astartframe;  int  Aendframe;  int  Aanimframe; int  MaxFrames;

    //for the left and right menus
    QFrame         *glworldLeft;
    JahToolButton   *tooltranslate, *toolscale, *toolrotate;
    JahToolButton  *toolthumbview, *toolbinview;

    QFrame         *glworldRight;
    JahToolButton  *toolzoomin, *toolzoomout;
    QSlider        *zoom;

	//other stuff
    QHBoxLayout * mainplayerLayout;
    QFrame * playerframe;
    QFrame * mainplayerframe;
    QWidget * playerClip;

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

    //for the player
    QVBoxLayout	*topLayout;
    QHBoxLayout	*middle;
    QVBoxLayout	*midR;
    QVBoxLayout	*midM;
    QVBoxLayout	*midL;
    QHBoxLayout	*buttons;
    QHBoxLayout	*status;

    int			playMode;
    QScrollBar	*frameScroll;
    QTimer		*timer;
    QString		frameNumString;
    QLabel		*frameNum;
    QFrame* controls;
    QHBox* controlers;


    QPushButton* rewButton;
    QPushButton* pingButton;
    QPushButton* stopButton;

    QMenuBar	*menuBar;
    QPopupMenu	*filePopup;
    QPopupMenu	*utilPopup;

    //QCheckBox    * forcedplay;
    QPushButton    * playmodecombo;

    JahToolButton  * scrubfirst, * scrubprevious, * scrubstop, * scrubplay, * scrubnext, * scrublast;
    QPushButton  * scrubkey, * scrubrender, * scrubrenderAll;

    //for the rest of the interface
    QHBox*       MiddleFrame;       QSlider *     Slider2;
    SupergRangeControl*    animframe;

    //for the new interface
    QFrame*        EffectsFrame;
    QPushButton* EncodeButton;
};

#endif

