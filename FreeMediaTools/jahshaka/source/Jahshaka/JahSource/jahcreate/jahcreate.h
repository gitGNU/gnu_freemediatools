/*******************************************************************************
 **
 ** The header file for the Jahshaka Source jahcreate file
 ** The Jahshaka Project
 ** Copyright (C) 2000-2006 VM Inc.
 ** Released under the GNU General Public License
 **
 *******************************************************************************/

#ifndef JAHCONTROL_H
#define JAHCONTROL_H

#include <glworld.h>

// Forward declarations
class GLAnime;
class GLEffect;
class GLEdit;
class GLChar;
class GLPaint;
class GLTracker;
class JahDesktop;
class JahLibrary;
class JahHeader;
class GLColorize;
class GLKeyer;
class MainEncoder;
class MainPlayer;

#ifdef USE_NETWORK
class JahNet;
#endif

class GLCore;
class JahHead;
class JahHeadable;
class StretchableButton;

////////////////////////////////////////////////////////////////////
//these are the modules that are supported in this build of jahshaka
//but dont necessarily have to be here


enum Module {
  DESKTOP, PLAYER, NETWORK, ANIMATION, EFFECTS, EDITING,
  PAINT, TEXTCG, COLORIZE, KEYER, TRACKER, ENCODING, LIBRARY,
  BROADCASTING, VIDEOIO, NUMMODULES
};

//change interface look and feel
enum StyleColor { NOCOLOR, JGREY, JWHITE, JBLUE, JBLACK };

////////////////////////////////////////////////////////////////////
// This is the main Controling class that sets up all the modules

class JahControl : public QObject 
{
    Q_OBJECT
    
public:
    
    JahControl( QGLWidget *share, QWidget *parent=0,
                JahHeader *menucontroller=0,
                QFrame  *mainleftmenu=0, QFrame *mainworld=0, QFrame *mainrightmenu=0,
                QFrame  *maincontroller=0,
                QFrame  *moduleselection=0, QFrame *modulesettings=0,QFrame *feedbackbar=0,
                QWidget *D2parent=0,
                QFrame  *D2mainleftmenu=0, QFrame *D2mainworld=0, QFrame *D2mainrightmenu=0,
                QFrame  *D2maincontroller=0,
                QFrame  *D2moduleselection=0, QFrame *D2modulesettings=0,QFrame *D2feedbackbar=0);
    
    
    ~JahControl();
    
    ////////////////////////////////////////
    // main module classes
    
    //desktop and library
    JahDesktop* JahDesktopMod;
    JahLibrary* JahLibraryMod;
    
    //the player
    MainPlayer* JahPlayerMod;
    
    //main moudles
    GLAnime*  JahAnimationMod;
    GLEffect* JahEffectsMod;
    GLEdit*   JahEditingMod;
    GLChar*   JahTextMod;
    GLPaint*  JahPaintMod;
    
    //optional modules
    GLColorize* JahColorMod;
    GLKeyer* JahKeyerMod;
    GLTracker* JahTrackerMod;
    
    //encoder
    MainEncoder* JahEncoderMod;

#ifdef USE_NETWORK
    //networking module
    JahNet* JahNetworkMod;
#endif

    ////////////////////////////////////////
    //OpenGL Core is a 3d core
    QHBox* corebox;
    GLCore* core;
    
    ///////////////////////////////////////
    //Paint Core is a 2d core
    QHBox* paintbox;
    GLCore* paintcore;
    
    ////////////////////////////////////////
    //theme object
    JahThemes* jahthemes;
    QPopupMenu* stylez;
    
    QFrame* m_mainworld_qframe;
    
    
    ////////////////////////////////////////
    //pointer to tracer singleton
    JahTrace* jtrace;
    
    ////////////////////////////////////////
    //pointer to translater singleton
    JahTranslate* jt;
    
    ////////////////////////////////////////////////////////////////////
    // base routines
    QFrame*         getMainworldQframe() { return m_mainworld_qframe; }
    void            setMainworldQframe(QFrame* qframe) { m_mainworld_qframe = qframe; }
    
    QPushButton*    m_active_module_button;
    QPushButton*    getActiveModuleButton() { return m_active_module_button; }
    void            setActiveModuleButton(QPushButton* button) { m_active_module_button = button; }
    
    
    //used to initialize main variables on startup
    void initializeVariables();
    
    //used to initialize main variables on startup
    void initalizeLibraries();
    
	//used to initialize plugins
	void initializePlugins();

    //used to initialize main variables on startup
    void createJahUI(   QFrame *mainworld,        QFrame *mainleftmenu,   QFrame *mainrightmenu,
                        QFrame *maincontroller,   QFrame *modulesettings,
                        QFrame *D2mainworld,      QFrame *D2mainleftmenu, QFrame *D2mainrightmenu,
                        QFrame *D2maincontroller, QFrame *D2modulesettings );
    
    void createJahDesktopUI( QFrame *moduleselection, QFrame *D2moduleselection );
    
    void createJahDesktopButtons( QFrame *moduleselection );
    
    //used to configure system on startup after loading all modules
    void configSystem();
    
    //used to toggle ui display modes
    void hidetheUI();
    
    ////////////////////////////////////////////////////////////////////
    
    void buildMenuBar( JahHeader* parent );
    void buildDropDownMenus( QMenuBar* parent );
    void buildModeSelector( QWidget* parent );
    
    //default menus
    void createFileMenu(QPopupMenu * themenu);
    void createHelpMenu(QPopupMenu * themenu);
    void createEditMenu(QPopupMenu * themenu);
    
public:
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
    
    static void restoreStyle();
    static QColor findColorForStyleColor( StyleColor );
    void restoreColor(void);
    
    void raiseModule(Module themod);
    void raiseD2Module(Module themod);
    
signals:
	void addProgressSteps( int );
    void creatingModule(const QString &);
    void updateProgress( );
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
    void updateDesktopLabel( QString );
    
    //////////////////////////////////////////////////////
    
    //for the library
    void getClipFromLibrary();
    
    //for the modules
    void switchModule(QString modname);
    void switchModule(Module modname);
    void switchModule(int modname);
    void slotSmartLoader(); 
    void slotSmartSaver(); 
    void loadModuleFile(QString fn);
    void raiseMenu(Module mod);
    void selectMenuDropdown(Module mod);
    void loadModelFile(QString fn);
    
    void GrabClipEffects();
    void GrabClipEdit();
    void AddClipEdit();
    void ReturnClipEdit();// used for grab on add...
    void GrabClipAnim();
    void GrabClipAnimKey();
    void GrabClipCG();
    void GrabClipColorize();
    void GrabClipPaint();
    void GrabClipKeyer();
    void GrabClipKeyerBase();
    void GrabClipTracker();
    
    void renameDesktopAsset(QString item);
    void resetGlobalClipNumber();
    
    void draggedAImage(void);
    
    //for themes
    void setJahTheme();
    void recordTheme(const QString &);
    int getJahThemeColor (void) { return jahStyleColor; }
    
    void hidetheUI( bool );

    
public slots:

    //change mode for desktop and subsections
    void changeModeDesktop();
    
    void changeModePlayer();
	void syncMediaTable( assetData );
    void notifyPlayerOfChange();
    void notifyPlayerOfNextItem();
    void notifyEncoderOfChange();

#ifdef USE_NETWORK    
    void changeModeNetworkNet();
    void changeModeNetworkCom();
    void changeModeNetworkP2P();
    void changeModeNetworkHttpd();
#endif
    void changeModeEncoding();
    void changeModeVideoIO();
    void changeModeBroadcasting();
    void changeModeColorize();
    
    //for the desktop preferences
    void changeModePref();
    void changeModePrefReturn();
    
    //change mode for modules
    void changeModeEffects();
    void changeModeAnimation();
    void changeModeEditing();
    void changeModePaint();
    void changeModeText();
    void changeModeLibrary();
    void changeModeTracker();
    void changeModeKeyer();
    
    //jahroutine routines
    void checkClearPlayer();
    void checkDesktopButton();
    void stopAnimation();
    
    void saveSplitterValues();
    
    void nameClip();
    
    void saveAndUpdateColor();
    void setstyleGrey();
    void setstyleBlue();
    void setstyleBlack();
    void setstyleWhite();
    
    void styleFont(int size, QString font);
    
    //store preferences
    void setStoragePath();
    
    //Get the storage loaction fron user through a file Dialog
    void getStoragePath();
    
    void setGlobalRenderQuality(int);
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
    void showInHead( JahHeadable * );
	void setQuickStart( void );

protected:
    
    void CreateModule( bool dhead=false );
    QString getModuleName(Module module);
    
    void CreateDesktopModule(QHBox* frame=0,QHBox* frame2=0);
    void CreateDesktopModuleOptions(void);
    
    void CreateDesktopMiddleFrame(QHBox*);
    void CreateDesktopFeedbackBox( QWidget* );
    void CreateDesktopOptions(QHBox*);
    
    void CreateDesktopPreferences(QHBox*);
    void PreferenceslanguageChange();
    
    void CreateAnimationModule();
    void CreateEffectsModule();
    void CreateEditingModule();
    void CreatePaintModule();
    void CreateTextModule();
    
    void CreateKeyerModule();
    void CreateTrackerModule();
    
    void CreateColorModule();
    void CreatePlayerModule(QFrame* frame);
    void CreateEncodingModule(QFrame* frame);
#ifdef USE_NETWORK
    void CreateNetworkModule(QFrame* frame);
#endif
    void CreateLibraryModule(QFrame* frame);

    void startVideoHead( );
    void stopVideoHead( );

private:
    
    //////////////////////////////////////////
    // All variable Initializations here
    
    //////////////////////////////////////////////////
    // new interface variables
    //move this to the bottom later under private vars
    QHBox       *MainModuleControler[NUMMODULES];
    QHBox       *MainModuleControlerLeft[NUMMODULES];
    QHBox       *MainModuleControlerRight[NUMMODULES];
    
    QHBox       *MainModuleControlerRightFrame[NUMMODULES];
    QHBox       *MainModuleControlerLeftFrame[NUMMODULES];
    
    int MainModuleControlerRightFrameWidth[NUMMODULES];
    int MainModuleControlerCenterFrameWidth[NUMMODULES];
    int MainModuleControlerLeftFrameWidth[NUMMODULES];
    
    QHBox       *ModulesOptions[NUMMODULES];
    int         ModulesID[NUMMODULES];
    
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
    //bool widescreen;
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
    QWidgetStack*  optionals;
    
    // Dual head Main Interface layout variables
    QHBoxLayout* D2mainworldLayout;       
    QWidgetStack*  D2WorldLayout;
    
    QHBoxLayout * D2mainworldLeftLayout;  
    QWidgetStack * D2MainLeftControler;
    QHBoxLayout * D2mainworldRightLayout; 
    QWidgetStack * D2MainRightControler;
    
    // Dual head main window layout variables
    QHBoxLayout*   D2maincontrollerLayout;
    QWidgetStack*  D2MainControler;
    QWidgetStack*  D2optionals;
    
    //new menu ui variables
    QFrame* menubox;
    QFrame* modebox;
    QFrame* statusbox;
    
    QHBoxLayout* menulayout;
    QSpacerItem* menuspacer1;
    QSpacerItem* menuspacer2;
    
    //the main desktop module
    //desktop controllers
    QHBox*         desktopdisplay;
    QHBox*         desktopdisplay2;
    QPushButton*   DesktopButton;
    QPushButton*   DualDesktopButton;
    
    //mandatory modules in the desktop
    //network controllers
#ifdef USE_NETWORK
    QHBox*         networkdisplay;
    QPushButton*   Network;
    QPushButton*   DualNetwork;
#endif

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
    QPushButton*   AnimationModuleButton;
    
    //effects controllers
    QHBox*         effectsdisplay;
    QPushButton*   EffectsModuleButton;
    
    //keyer controllers
    QHBox*         keyerdisplay;
    QPushButton*   KeyerModuleButton;
    
    //tracker controllers
    QHBox*         trackerdisplay;
    QPushButton*   TrackerModuleButton;
    
    //treying a ting
    QHBoxLayout  * themaineffectsLayout;
    QWidgetStack * theCoreLayout;
    
    //QHBox * worldlayoutleftdisplay;
    //QHBox * worldlayoutrightdisplay;
    
#ifdef USE_NETWORK
    QFrame * thenetworkdisplay;
#endif

    //editing controllers
    QHBox*         editingdisplay;
    QPushButton*   EditingModuleButton;
    
    //The Text Module
    QHBox*         textdisplay;
    QPushButton*   TextModuleButton;
    
    //The Paint Module
    QHBox*         paintdisplay;
    QPushButton*   PaintModuleButton;
    
    //sub modules located on the desktop
    //The Color Module
    QHBox*         colordisplay;
    QPushButton*   ColorModuleButton;
    
    
    // for the desktop controlbar interface
    QWidget* page;
    QFrame*      modeselectframe;
    QPushButton* modeselectbutton; //no longer used
    QPushButton  *lockbutton, *expandbutton, *sharebutton;
    QFrame*      buttonframe;
    
    StretchableButton* DesktopName;
    StretchableButton* DesktopLoad;
    StretchableButton* DesktopPlay;
    StretchableButton* DeleteButton;
    
    QFrame*      desktopselectframe;
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
    
    
    
    //these need to be sorted and remove the inactive ones
    
    QFrame*         Display;
    QFrame*        FrameMain;
    
    QHBox * desktopcontrolerbar;
    
    QLabel*        m_desktopFeedbackLabel;
    QPushButton*   SwitchButton;
    QPushButton*   ReturnButton;
    QFrame*        FrameSubmenu;
    QVBox*         optionals_prefs;
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
#ifdef USE_NETWORK
    QPushButton* network;
#endif
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
    QWidget* createCol1( QWidget* p );
    QWidget* createCol2( QWidget* p );
    QWidget* createCol3( QWidget* p );
    QWidget* createCol4( QWidget* p );
    QWidget* createCol5( QWidget* p );

    QHBox* preferencesframe;
    
    QPushButton* Pref5;
    QPushButton* Pref6;
    QPushButton* Pref7;
    QPushButton* Pref8;
    QCheckBox*   Pref10;
    QLineEdit* StorageBox;
    QToolButton* StorageLocation;
    QPushButton* StorageButton;
    QComboBox* renderqualitycombo;
    QComboBox* renderercombo;
    QCheckBox* MenuOption;
    QCheckBox* DualHead;
    QCheckBox* VideoHead;
    QCheckBox* WideScreenBox;
    QCheckBox* QuickStartBox;
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

    //for the menu switching
    QMenuBar* menu;
    QPopupMenu *menufile, *menuedit, *menuhelp, 
               *menueditor, *menuanimation, *menueffects, *menupaint, *menutext, *menudesktop,
               *menucolor, *menutracker, *menukeyer, *menuplayer;
 
public:
    QPopupMenu* menuOptions[NUMMODULES];
private:
    QString     menuOptionNames[ NUMMODULES ];
    int         moduleMenuId;
    
    QComboBox* modeSelector; 

    JahHead *m_video_head;
};


#endif
