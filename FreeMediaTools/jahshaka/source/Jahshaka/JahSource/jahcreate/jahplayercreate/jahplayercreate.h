/*******************************************************************************
**
** The header file for the Jahshaka Source jahcreate file
** The Jahshaka Project
** Copyright (C) 2000-2004 The Jahshaka Project
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef JAHCONTROL_H
#define JAHCONTROL_H

//////////////////////////////////////
//generic includes
#include <stdlib.h>
#include <qsettings.h>
#include <qlineedit.h>
#include <qstylefactory.h>
#include <qcursor.h>
#include <qmenubar.h>
#include <qcombobox.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qlabel.h>
class QTextView;

//////////////////////////////////////
//open lib includes
#include "openmedialib.h"
#include "openobjectlib.h"

///////////////////////////////////////
//includes for the modules
#include "desktop.h"
#include "jahlibrary.h"
#include "MainPlayer.h"
#include "dialogs.h"
#include "glworld.h"
//#include "color.h"
//#include "networking.h"
//#include "keyer.h"

//for the encoder module
class MainEncoder;

//other essential libraries
#include "themes.h"
#include "jahtracer.h"
#include "jahtranslate.h"
#include "jahformatter.h"

//for the plugin library
#include "jahpluginlib.h"

// Forward declarations
class GLAnime;
//class GLEffect;
//class GLEdit;
//class GLChar;
//class GLPaint;
//class GLTracker;

////////////////////////////////////////////////////////////////////
//these are the modules that are supported in this build of jahshaka
//but dont necessarily have to be here

enum Module { DESKTOP, PLAYER, NETWORK, ANIMATION, EFFECTS, EDITING, 
              PAINT, TEXTCG, COLORIZE, KEYER, TRACKER, ENCODING,
			  BROADCASTING, LIBRARY, VIDEOIO, NUMMODULES };

//change interface look and feel
enum StyleColor { NOCOLOR, JGREY, JWHITE, JBLUE, JBLACK };

////////////////////////////////////////////////////////////////////
// This is the main Controling class that sets up all the modules

class JahControl : public QObject {
    Q_OBJECT

public:

JahControl( QWidget *parent=0,
            QWidget *menucontroller=0,
            QFrame  *mainleftmenu=0, QFrame *mainworld=0, QFrame *mainrightmenu=0,
            QFrame  *maincontroller=0,
            QFrame  *moduleselection=0, QFrame *modulesettings=0,QFrame *feedbackbar=0);


~JahControl();

    ////////////////////////////////////////
    // main module classes

	//desktop and library
    JahDesktop			*JahDesktopMod;
    //FileMainFrame		*JahLibraryMod;
    JahLibrary		*JahLibraryMod;
    
	//the player
	MainPlayer			*JahPlayerMod;

    
	//main moudles
	GLAnime				*JahAnimationMod;
    //GLEffect			*JahEffectsMod;
    //GLEdit				*JahEditingMod;
    //GLChar				*JahTextMod;
    //GLPaint				*JahPaintMod;
    
	//optional modules
	//GLColorize			*JahColorMod;
    //GLKeyer				*JahKeyerMod;
    //GLTracker			*JahTrackerMod;

	//encoder
    MainEncoder			*JahEncoderMod;

	//networking module
    //JahNet				*JahNetworkMod;

    ////////////////////////////////////////
    //OpenGL Core is a 3d core
    QHBox				*corebox;
    GLCore				*core;

    ///////////////////////////////////////
    //Paint Core is a 2d core
    QHBox				*paintbox;
    GLCore				*paintcore;

	////////////////////////////////////////
	//theme object
	JahThemes			*jahthemes;
	QPopupMenu			*stylez;


	////////////////////////////////////////
	//pointer to tracer singleton
	JahTrace			* jtrace;

	////////////////////////////////////////
	//pointer to translater singleton
	JahTranslate		* jt;

    ////////////////////////////////////////////////////////////////////
    // base routines

    //used to initialize main variables on startup
    void initializeVariables();

    //used to initialize main variables on startup
	void createJahUI(	QFrame *mainworld,		  QFrame *mainleftmenu,		QFrame *mainrightmenu,
						QFrame *maincontroller,	  QFrame *modulesettings );

	void createJahDesktopUI(	QFrame *moduleselection,	QFrame *D2moduleselection);

	//void createJahDesktopButtons(QFrame *moduleselection);

    //used to configure system on startup after loading all modules
    void configSystem();

    //used to toggle ui display modes
    void hidetheUI();

	////////////////////////////////////////////////////////////////////

    void buildMenuBar(QWidget *  parent);

    //ui feedback routines
    void processKeyPress(QKeyEvent *k);
    void processKeyRelease(QKeyEvent *k);
    void spaceballmoved(double x, double y, double z, double a, double b , double c);

    //routines to get style and app preferences
    bool getPrefs(void)      { return rememberprefs; }

    //used to save and restore ui
    void saveModulesUI(void);
    void restoreModulesUI(void);

    //used to track edit returns on singlehead system
    int globalclipnumber;

    bool editReturnClip;
    bool buildmenubars;

    int jahrenderquality;
    int renderer;

    void restoreStyle();
    void restoreColor(void);

	void raiseModule(Module themod);


signals:
    void creatingModule(const QString &);
    void finishedModule(int);
    void emitStyleColor(QPalette);
    void hideUI(bool);

public slots:

    void switchMode(void);
    void splitterChanged(void);

	//for the desktop
    void switchDesktop(void);
    void GetDesktop();
    void GetDesktopKey();
    void ReturnDesktop();
    void addImageToDesktop( assetData );
    void addStringToDesktop( QString );

	//for the library
	void getClipFromLibrary();
    
    //load slot for the player
    //void loadJahSceneFile(QString filename);

	//for the modules
	void switchModule(QString modname);
	void loadModuleFile(QString fn);
	void loadModelFile(QString fn);

    void GrabClipAnim();

    void renameDesktopAsset(QString item);
    void resetGlobalClipNumber();

    void draggedAImage(void);

	//passthru slots for filemenu
	void slotLoadIconview(void);
	void slotExport(void);
	void slotImport(void);
	void clearItem(void);
	void tidyDesktop(void);
	void clearAll(void);

	//for themes
	void setJahTheme();
	void recordTheme(const QString &);
    int getJahThemeColor (void) { return jahStyleColor; }

private slots:

    //change mode for desktop and subsections
    void changeModeDesktop();
    void changeModeDesktopTable();

    void changeModePlayer();
	void syncMediaTable( assetData );
    void notifyPlayerOfChange();
	void notifyPlayerOfNextItem();

	//for the desktop preferences
    void changeModePref();

    void changeModeAnimation();
    void changeModeLibrary();

    //jahroutine routines
    void checkClearPlayer();
    //void checkDesktopButton();
    void stopAnimation();

    void saveSplitterValues();

    void nameClip();

    void styleGrey();
    void styleBlue();
    void styleBlack();
    void styleWhite();
    
	void setstyleGrey();
    void setstyleBlue();
    void setstyleBlack();
    void setstyleWhite();

    void styleFont(int size, QString font);

    //store preferences
    void setStoragePath();

    //Get the storage loaction fron user through a file Dialog
    void getStoragePath();

    void setGlobalRenderQuality(void);
    void getGlobalPreferences(void);
    void setModulesRenderQuality(void);

    void setGlobalPositionSize(void);
    void setVideoHeadOption(void);
    void setStyleOption(void);
    void setWidescreenOption(void);
    void setDualHeadOption(void);
    void setMenuOption(void);
    void setRenderer(int renval);
    void setLanguage(const QString &);
    void setResolution(int renval);

protected:

    void CreateModule( bool dhead=false );
	QString getModuleName(Module module);

    void CreateDesktopModule(QHBox* frame=0,QHBox* frame2=0);
	void CreateDesktopModuleOptions(void);

    void CreateDesktopMiddleFrame(QHBox*);
    //void CreateDesktopFeedbackBox(QFrame*);
    //void CreateDesktopOptions(QHBox*);

    void CreateDesktopPreferences(QHBox*);
    void PreferenceslanguageChange();

    void CreateAnimationModule();
    void CreatePlayerModule(QFrame* frame);
    void CreateLibraryModule(QFrame* frame);

private:

    //////////////////////////////////////////
    // All variable Initializations here

	//////////////////////////////////////////////////
	// new interface variables
	//move this to the bottom later under private vars
	QHBox	*MainModuleControler[NUMMODULES];
	QHBox	*MainModuleControlerLeft[NUMMODULES];
	QHBox	*MainModuleControlerRight[NUMMODULES];

	QHBox	*MainModuleControlerRightFrame[NUMMODULES];
	QHBox	*MainModuleControlerLeftFrame[NUMMODULES];
    
	int	MainModuleControlerRightFrameWidth[NUMMODULES];
	int	MainModuleControlerCenterFrameWidth[NUMMODULES];
	int	MainModuleControlerLeftFrameWidth[NUMMODULES];
    
    int		ModulesID[NUMMODULES];

	int module_counter;

	//the desktop ui stack
	QWidgetStack* theDesktopLayout;
	
	//////////////////////////////////////////////////
    QString JahBasePath;
    QString JahMediaPath;
    int     JahDefaultRes;

    bool desktopButtonStatus;
    bool getKeyStatus;
    bool UIstatus;

    QFont          appFont;
    Module         module;

    //style prefs
    bool rememberprefs;
    bool videohead;
    bool dualhead;
    bool dualheadprefs;
    bool menuprefs;
    bool widescreenprefs;
    bool jahStyleprefs;

    std::string jahStyleTheme;
    StyleColor jahStyleColor;

    // Main Interface layout variables
    QHBoxLayout* mainworldLayout;       
	QWidgetStack*  WorldLayout;

    QHBoxLayout * mainworldLeftLayout;  
	QWidgetStack * MainLeftControler;
    QHBoxLayout * mainworldRightLayout; 
	QWidgetStack * MainRightControler;

    //main window layout variables
    QHBoxLayout*   maincontrollerLayout;
    QWidgetStack*  MainControler;
    //QWidgetStack*  optionals;

    //the main desktop module
    //desktop controllers
    QHBox*         desktopdisplay;
    QHBox*         desktopdisplay2;
    QPushButton*   Desktop;
    QPushButton*   DualDesktop;

    //mandatory modules in the desktop
    //network controllers
    QHBox*         networkdisplay;
    QPushButton*   Network;
    QPushButton*   DualNetwork;

    //the main encoding module
    QPushButton*   Share;
    QPushButton*   Encoding;
    QPushButton*   DualEncoding;

    //encoding controllers
    QFrame*        librarydisplay;
    QHBox*         MainLibraryControler;

    //encoding controllers
    QFrame*        encodingdisplay;

    //the main broadcast module
    QPushButton*   Broadcasting;

    //player controllers
    QFrame*        playerdisplay;

    //main modules located in the left option panel
    //animation controllers
    QHBox*         animationdisplay;
    QPushButton*   AnimationModule;

    //effects controllers
    QHBox*         effectsdisplay;
    QPushButton*   EffectsModule;
    
	//keyer controllers
    QHBox*         keyerdisplay;
    QPushButton*   KeyerModule;

    //tracker controllers
    QHBox*         trackerdisplay;
    QPushButton*   TrackerModule;

    //treying a ting
    QHBoxLayout  * themaineffectsLayout;
    QWidgetStack * theCoreLayout;

    //QHBox * worldlayoutleftdisplay;
    //QHBox * worldlayoutrightdisplay;

    QFrame * thenetworkdisplay;

    //editing controllers
    QHBox*         editingdisplay;
    QPushButton*   EditingModule;

    //The Text Module
    QHBox*         textdisplay;
    QPushButton*   TextModule;

    //The Paint Module
    QHBox*         paintdisplay;
    QPushButton*   PaintModule;

    //sub modules located on the desktop
    //The Color Module
    QHBox*         colordisplay;
    QPushButton*   Colors;


    // for the desktop controlbar interface
    QWidget* page;
    QFrame*      modeselectframe;
    QPushButton* modeselectbutton; //no longer used
    QPushButton  *lockbutton, *expandbutton, *sharebutton;
    QFrame*      buttonframe;

    JahToolButton* DesktopName;
    JahToolButton* DesktopLoad;
    JahToolButton* DesktopPlay;
    JahToolButton* DeleteButton;
    /*
	void addJahPlayerButton( JahToolButton * button, int x1, int y1, int x2, int y2,
                                  int w, int h, QString iconOff, QString iconOn ); 
*/
	QFrame*      desktopselectframe;
    //QPushButton* desktopselectbutton; 
    QComboBox* desktopselectbutton; 
    QHBoxLayout* pageLayout;

    //for the middleframe
    QFrame*        MiddleFrame;
    FancyPopup*    popup2;
    QPushButton*   clipname;
    QPushButton*   clipplay;
    QPushButton*   clipdelete;
    QPushButton*   clipload;
    QPushButton*   SwitchDesk;

    QWidget*       prefsdialog;

    //these need to be sorted and remove the inactive ones

    QFrame*         Display;
    QFrame*        FrameMain;

    QHBox * desktopcontrolerbar;

    QTextView*     TextView1_2;
    //QPushButton*   GetButton;
    //QPushButton*   SwitchButton;
    //QPushButton*   ReturnButton;
    QFrame*        FrameSubmenu;
    QFrame*         optionals_prefs;
    QHBox*         desktopframe;


	//////////////////////////////////////////
	//variables used in the desktopui
    QFrame* desktoppanel;
    QFrame* divider3;
    QFrame* divider2;
    QFrame* divider1;
    QFrame* divider0;
    QPushButton* library;
    QPushButton* findclip;
    //QLineEdit* findeditbox;
    QPushButton* lockclip;
    QPushButton* unlockclip;
    QPushButton* lockall;
    QPushButton* unlockall;
    QPushButton* expandclip;
    QPushButton* collapseclip;
    QPushButton* expandall;
    QPushButton* collapseall;
    QPushButton* colorizeit;
    QPushButton* trimmer;
    QPushButton* tracker;
    QPushButton* keyer;
    QPushButton* network;
    QPushButton* encoder;
    QPushButton* broadcaster;
    QPushButton* videoio;
    QPushButton* loaddesktop;

    QPushButton* exportclip;
    QPushButton* savedesktop;
    QPushButton* tidydesktop;
    QPushButton* cleardesktop;
    QPushButton* desktopundo;
    QPushButton* desktopredo;
    QPushButton* Preferences;
    QPushButton* Quit;

    //preferences stuff starts here
    QHBox* preferencesframe;

    QFrame* prefframe1;
    //QPushButton* Pref1;
    //QPushButton* Pref4;
    //QPushButton* Pref2;
    //QPushButton* Pref3;
    QPushButton* Pref5;
    QPushButton* Pref6;
    QPushButton* Pref7;
    QPushButton* Pref8;
    QCheckBox*   Pref10;
    QLineEdit* StorageBox;
    QToolButton* StorageLocation;
    QPushButton* StorageButton;
    QSlider* renderqualityslider;
    QComboBox* renderercombo;
    QCheckBox* MenuOption;
    QCheckBox* DualHead;
    QCheckBox* VideoHead;
    QCheckBox* WideScreenBox;
    QCheckBox* PositionPrefs;
    QPushButton* PrefExit;
    QComboBox* LanguageCombo;
    QComboBox* JahresCombo;
    QLabel* interfacelabel;
    QPushButton* stylebutton;
	QLabel* storagelabel;
	QLabel* stylelabel;
	QLabel* colorlabel;
    QLabel* renderlabel;
    QLabel* httpdlabel;
    QLabel* uisettingslabel;
    QLabel* languagelabel;
    QLabel* jahreslabel;
    QLabel* rendererlabel;
    QLabel* renderqualitylabel;
    QFrame* uiframe1;
    QFrame* uiframe2;
    QFrame* uiframe3;
    
    QPushButton *bindButton;
    QLineEdit *backlogLineEdit, *portLineEdit;

    QFrame* uiframe33;
    QFrame* uiframe4;
    QFrame* uiframe6;

    //QFrame* prefframe1;
    QFrame* prefframe2;
    QFrame* prefframe3;

    //trying a ting for the widescreen mode
    //QGridLayout    *wideframeLayout;
    QHBoxLayout    *wideframeLayout;

    //the splitter
    QSplitter *splitter2;

    QHBox* wideframeLeft;
    QHBox *wideframeCenter;
    QHBox *wideframeRight;

       //this will be for the desktop alone
    QHBoxLayout*    mainworldLayoutRight;
    QWidgetStack*    WorldLayoutRight;

    QHBoxLayout*    mainworldLayoutLeft;
    QWidgetStack*    WorldLayoutLeft;

};


#endif




























