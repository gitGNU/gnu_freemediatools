/*******************************************************************************
**
** The header file for the Jahshaka animation module 
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/
#ifndef ANIME_H
#define ANIME_H

#include <qtabwidget.h>
#include "glworld.h"
#include "jahpluginlib.h"

///////////////////////////////////////////////
//Then Main Class Definitions

class GLAnime :  public  GLWorld {
    Q_OBJECT


private:
	QHBox *m_controls; 
	QHBox *m_leftcontroller; 
	QHBox *m_rightcontroller;

public:

    GLAnime( GLCore* core, const char* name,
	     QHBox* controls, QHBox* controller, QHBox* leftcontroller, 
	     QHBox* rightcontroller,
	     int* globalclipnumber, const QGLWidget* =0   );

    ~GLAnime();

protected:
	void start( );

public:
    virtual void buildInterface( QHBox* qtinterface);
    void buildInterfaceMenus( QHBox* leftcontroller, QHBox* rightcontroller );
    void buildPlayerInterfaceMenus( QHBox* leftcontroller, QHBox* rightcontroller );

    void createMenuItem(QPopupMenu * themenu );

    void updateSliders();
    void updateSliderValues();
    void updateUiOptions();
    void updateUiSettings();

    void initializeObjects();

    //for the 3d player in jahplayer
    void initializePlayer();
    void initalizePlayerLibraries();


    enum MODULE_OPTION_TYPE
    {
        LIGHTING_OPTION,
        TRANSLATE_FIRST_OPTION,
        FOREGROUND_OPTION,
        DEPTH_OPTION,
        REFLECTION_OPTION,
        SHOW_PATH_OPTION,
        LAYER_SELECTED_VISIBLE_OPTION,
        NOT_AN_OPTION
    };


    JahLayer*   createALayer(LAYER_TYPE type, JahLayer* parent);
    void        setReflectionState(JahLayer* jah_layer, bool flag);

    GLfloat setZoom( GLfloat );
    
    using GLWorld::initializeListview;
    virtual void initializeListview(JahLayer* jah_layer, bool layer_status);

public slots:
    virtual void nameLayerFromTopMenu();
    virtual void addLayerFromTopMenu(int val);
    virtual void addLayer(void);

    //for add effects
    void addCpuEffect();
    void addMeshEffect();
    void addGpuEffect();
    void addCpuEffectFromTopMenu();
    void addMeshEffectFromTopMenu();
    void addGpuEffectFromTopMenu();

    //option selection routines
    void setOption1(void);
    void setOption2(void);
    void setOption3(void);
    void setOption4(void);
    void setOption5(void);
    void slotSetLayerSelectedVisible();

    void changeParticle();
    void changeParticleDraw();
    void changeFontDraw();
    void changeObjectDraw();
    void keyCliplayer();
    void keyClipkey();

    void grabDesktop(void)      { emit GrabClip();      };
    void grabDesktopKey(void)   { emit GrabClipKey();   };

    //we override this routine for the animation module
    //as it uses a custom selection workflow
    void layerClicked( QListViewItem* item );

    //this is experimental used for media location
    void locateMissingMedia(void);

    QCheckBox* getKeySelect() { return KeySelect; }

    //move into core once it works
    void slotChooseLightColorAmbient();
    void slotChooseLightColorDiffuse();
    void slotChooseLightColorSpecular();
    void slotLightColorAmbient(QColor color);
    void slotLightColorDiffuse(QColor color);
    void slotLightColorSpecular(QColor color);

	void textEdChanged( );

signals:
    void    GrabClip(void);       //linked to main jahcontrol to talk to desktop
    void    GrabClipKey(void);    //linked to main jahcontrol to talk to desktop

protected:

    ///////////////////////////////////////////////////////////////////////
    //interface variables for the left and right world space menus
    ///////////////////////////////////////////////////////////////////////

    QFrame            *glworldLeft;
    JahToolButton     *tooltranslate, *toolscale,     *toolrotate;
    JahToolButton     *playerphoto;
    JahToolButton     *tooladdkey, *toolremovekey, *toolpreviouskey, *toolnextkey;

    JahToolButton	*toolrotatein,	*toolrotateout;

    JahToolButton	*toolpanin,	*toolpanout;
    QSlider			*toolpan, *toolrotateplayer;

    QFrame            *glworldRight;
    JahToolButton     *toolzoomin,    *toolzoomout;
    QSlider           *zoom;

    ///////////////////////////////////////////////////////////////////////
    //animation module ui options
    ///////////////////////////////////////////////////////////////////////

    //declare all controllers that we are using here
    enum tabUI {SCENE, OBJECT, AXIS, CLIP, EFFECTS, KEYFRAMES, LIGHTING_TAB, numTabs};

    ///////////////////////////////////////////////////////////////////////
    //now the main widget variables

    //the first, main frame
    QFrame*           LayersFrame;
    QFrame*           EffectsFrame;

    ///////////////////////////////////////////////////////////////////////
    //the tab frame to hold the tabs

    QTabWidget        *tabframe;

    //widegts for each of the tabs
    QWidget           *moduleOptionsUI[numTabs];

    //each module option ui can have up to 10 embedded ui's
    QFrame            *ControlFrameUI[numTabs][10];

    ///////////////////////////////////////////////////////////////////////
    // variables for the different tabs

    ///////////////////////////////////////////////////////////////////////
    //set up the scene controls
    QPushButton*  ResetButton;             
    QPushButton*  SaveButton;    
    QPushButton*  LoadButton;              
    QPushButton*  AppendButton;              
    QPushButton*  loadEffectsButton;              
    QPushButton*  saveEffectsButton;              
    QPushButton*  ClearButton;
    QPushButton*  PackageButton;
    QPushButton*  m_stop_script_button;
    QPushButton*  ImportFxButton;
    QPushButton*  ExportFxButton;

    QPushButton*  AddCpuEffectButton;
    QPushButton*  AddMeshEffectButton;
    QPushButton*  AddGpuEffectButton;

    //QPushButton*  AddCpuEffectButton2;
    //QPushButton*  AddMeshEffectButton2;
    //QPushButton*  AddGpuEffectButton2;

    //lcdstartframe and endframe are initialized in world

    QCheckBox*        FogSelect;
    QSlider           *HFogSlider,        *VFogSlider;
    InputLCD      *HFogSlider_lcd,    *VFogSlider_lcd;

    //////////////////////////////////////////////////////////////////////
    //set up the different object controls, multiple layers are embedded

    //the world
    QCheckBox     *SmoothSelect;            
    QCheckBox     *TextSelect;
    QCheckBox     *GridSelect;
    QPushButton       *ResetWorldButton;

    //the layer types
    QPushButton       *ObjectLayer;
    QPushButton       *ObjectCube;
    QPushButton       *ObjectCylinder;
    QPushButton       *ObjectSphere;
    QPushButton       *ObjectBezier;
    QPushButton       *ObjectMesh;
    QPushButton       *ResetLayerButton;


    //the font options
    QLabel            *ScaleText_5;        
    QSlider           *ScaleX_5;         
    InputLCD          *ScaleXlcd_5;
    QPushButton       *textFont;           
    QPushButton       *textColor;        
    QPushButton       *textDraw;


    //the particle options
    QPushButton       *loadimage_3;
    QPushButton       *loadimage_4;
    QPushButton       *particlestyle;

    //the object options obj's
    QPushButton       *loadobject_1,  *objectstyle;
    QPushButton       *ResetObjectButton1;

    //the object options 3dsmax
    QPushButton       *loadmaxobject_1;
    QPushButton       *ResetObjectButton2;

    ///////////////////////////////////////////////////////////////////////
    //set up the texture and clip control panel

    //clip options
    QCheckBox         *ClipSelect;
    QPushButton       *grabdesktopClip; 
    QPushButton       *keyimage_2;       
    QCheckBox         *KeySelect;
    QPushButton       *grabdesktopKey;
    QPushButton       *keyimage_3;
    QPushButton       *keyStatusBox;

    //video options
    QLabel            *VideoLabel;
    QToolButton       *lockicon;

    QCheckBox         *extendHeadTail;
    QCheckBox         *toggleTheLoop;
    QCheckBox         *togglePing;

    QLabel			*FindMediaLabel;
    QLineEdit		*FindMediaText;
    QPushButton		*FindMediaButton;

    ///////////////////////////////////////////////////////////////////////
    //set up the effects control panel

    //QCheckBox     *ForegroundSelect;    
    //QCheckBox     *DepthSelect;
    QCheckBox     *BlurSelect;
    QSlider           *HBlurSlider, *VBlurSlider;
    InputLCD      *HBlurSlider_lcd, *VBlurSlider_lcd;
    JahToolButton *lighticon;
    QCheckBox     *reflect1, *shader1, *shader2, *smooth1, *mesh1;
    QLabel            *Transparency_1;     
    QSlider           *Trans_1;         
    InputLCD      *Translcd_1;

    /////////////////////////////////////////////////////////////////////
    // added for the stats option
    QCheckBox     *ShowStats;

    /////////////////////////////////////////////////////////////////////
    // added for lights
    QLabel      *ControlFrame5L1, *ControlFrame5L2, *ControlFrame5L3;
    QPushButton *m_ambient_color_button, *m_diffuse_color_button, *m_specular_color_button;

};



#endif // ANIMATION_H





