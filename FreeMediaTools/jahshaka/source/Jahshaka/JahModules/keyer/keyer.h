/*******************************************************************************
**
** The header file for the Jahshaka keyer module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/
#ifndef KEYER_H
#define KEYER_H

#include "glworld.h"

///////////////////////////////////////////////
//Then Main Class Definitions

#include <qcheckbox.h>
#include <qtabwidget.h>
#include <qstylefactory.h>
#include <stdlib.h>

#include "jahnodes.h"
#include "jahtimer.h"

//for dynamic plugin support
//#include "jahpluginlib.h"

////////////////////////////////////////////////////////////
//for gpu support
#include "gpumathlib.h"

class KeyerColorWidget;

class GLKeyer :  public  GLWorld {
Q_OBJECT

protected:
    float4          m_fRGBALow;
    float4          m_fRGBAHigh;
    float4          m_fHSVBase;
    float4          m_fHSBRange;
    float4          m_fRGBARange;

public:
    float4&         getRgbaLow() ;
    float4&         getRgbaHigh() ;
    void            setRgbaLow(float4& value) ;
    void            setRgbaHigh(float4& value);
    float4&         getHsvBase() ;
    void            setHsvBase(float4& value);
    float4&         getHsvRange() ;
    void            setHsvRange(float4& value) ;
    float4&         getRgbaRange() ;
    void            setRgbaRange(float4& value);
    void            setRgbaValues();

    KeyerColorWidget*   getKeyerColorWidget();
    void                setKeyerColorWidget(KeyerColorWidget* widget);

    void            setColorLimitSliderLabels();

private:
	QHBox *m_controls;
	QHBox *m_leftcontroller;
	QHBox *m_rightcontroller;

public:
	GLKeyer( GLCore* core, const char* name, QHBox* controls,
            QHBox* controller,QHBox* leftcontroller, QHBox* rightcontroller,
		 int* globalclipnumber );
	~GLKeyer();

protected:
	void start( );

public:
    //object and variable initialization routines
    void initializeVariables(void);
    void initializeObjects();

    //ui initialization routines
    void buildInterface( QHBox* qtinterface);
    void buildInterfaceMenus( QHBox* leftcontroller, QHBox* rightcontroller );
    void createMenuItem(QPopupMenu * themenu );
    
    void createKeyerPanel ( QFrame* theuiframe);

    void configureModuleEngine(void);

    void addClip(assetData clip);
    void addBaseClip(assetData clip);

    //these arent slots are they? they should just be public
    void updateSliders();
    void updateUiOptions();
    void updateUiSettings();

    void grabDesktop(void)      ;

    // Override of GLWorld method
    void setDefaultAxisSliderLabels();

public slots:

    void setGpuSelect(void);
    void setHueBase(int);
    void setSaturationBase(int);
    void setValueBase(int);
    void setHueRange(int);

    // The main slot for setting color (best to use this instead of separate H, S, V slots if all 3 change.
    void setColorBase( const QColor& );

    void setSaturationRange(int);
    void setValueRange(int);
    void setSoftenValue(int);
    void layerClicked( QListViewItem* item );
    void createAlphaMask(void);

    //for videorender option
    void changeVideoRender(void);

    void grabDesktopBaselayer(void) ;
    void setBackgroundStatus(void);
    void setSelectColorsIndividually();

    ////////////////////////////////////////////
    // routines to draw effects


public:
    JahLayer*               getChromaWorldLayer() ;
    JahLayer*               getChromaBaseLayer() ;
    JahLayer*               getChromaKeyLayer() ;
    JahLayer*               getChromaSettingsLayer() ;
    bool                    getSelectColorsIndividually() ;
    bool                    getGpuSelect() ;
    GLuint*                 getBaseTextureIDPtr();
    GLuint                  getBaseTextureID() ;
    GLuint*                 getKeyTextureIDPtr();
    GLuint                  getKeyTextureID();
    bool                    getTexturesInitialized() ;

protected:    
    JahLayer*               m_chroma_world_layer;


signals:
    void    GrabClip(void);    //linked to main jahcontrol to talk to desktop
    void    GrabBaseClip(void);    //linked to main jahcontrol to talk to desktop

protected:

    void objectMenu(int menu);
    void paintGL();
    void buildEffect();

private slots:
    void showColorDropper();

private:

    ////////////////////////////////////////////////////////////////////////////
    // effect based variables

    QImage      sourceimage,savedimage;

    bool        rebuildStatus;
    int         effectobj;
    int         iScreen;

    float       z_offset;
    float       camera_distance;

    KeyerColorWidget*   m_keyer_color_widget;

    ////////////////////////////////////////////////////////////////////////////
    //interface variables
    ///////////////////////////////////////////////////////////////////////
    //interface variables for the left and right world space menus
    ///////////////////////////////////////////////////////////////////////
    
    QFrame          *glworldLeft;
    JahToolButton   *tooltranslate, *toolscale,     *toolrotate;
    JahToolButton   *playerphoto,   *switchtheview;
    JahToolButton     *tooladdkey, *toolremovekey, *toolpreviouskey, *toolnextkey;

    QFrame          *glworldRight;
    JahToolButton   *toolzoomin,    *toolzoomout;
    QSlider         *zoom;

    ///////////////////////////////////////////////////////////////////////
    //animation module ui options
    ///////////////////////////////////////////////////////////////////////

    //declare all controllers that we are using here
    enum tabUI {SCENE, OBJECT, AXIS, CLIP, EFFECTS, KEYFRAMES, numTabs};
  
    ///////////////////////////////////////////////////////////////////////
    //now the main widget variables

    //the first, main frame
    QFrame*         LayersFrame;
    QFrame*         EffectsFrame;

    //for the control panel
    QPushButton*    AddRTButton;
    QPushButton*    AddGpuButton;
//    QPushButton*    AddPluginButton;

    ///////////////////////////////////////////////////////////////////////
    //the tab frame to hold the tabs

    QTabWidget      *tabframe;

    //widegts for each of the tabs
    QWidget         *moduleOptionsUI[numTabs];

    //each module option ui can have up to 10 embedded ui's
    QFrame          *ControlFrameUI[numTabs][10];

    ///////////////////////////////////////////////////////////////////////
    // variables for the different tabs

    ///////////////////////////////////////////////////////////////////////
    //set up the scene controls

    QPushButton*    ResetButton;             
    QPushButton*    SaveButton;    
    QPushButton*    LoadButton;              
    QPushButton*    ClearButton;
    QPushButton*    PackageButton;
    QCheckBox*      VideoRenderSelect;
    QCheckBox*      m_use_gpu_qcheckbox;

    //lcdstartframe and endframe are initialized in world

    QCheckBox*      FogSelect;
    QSlider         *HFogSlider,        *VFogSlider;
    InputLCD        *HFogSlider_lcd,    *VFogSlider_lcd;

    //////////////////////////////////////////////////////////////////////
    //set up the different object controls, multiple layers are embedded

    //the world
    QCheckBox       *SmoothSelect;            
    QCheckBox       *TextSelect;
    QCheckBox       *GridSelect;
    QPushButton     *ResetWorldButton;

    //the layer types
    QPushButton     *ObjectLayer;
    QPushButton     *ObjectCube;
    QPushButton     *ObjectCylinder;
    QPushButton     *ObjectSphere;
    QPushButton     *ObjectBezier;
    QPushButton     *ObjectMesh;
    QPushButton     *ResetLayerButton;

    //the font options
    //the particle options

    //the object options obj's
    QPushButton     *loadobject_1,  *objectstyle;
    QPushButton     *ResetObjectButton1;

    //the object options 3dsmax
    QPushButton     *loadmaxobject_1;
    QPushButton     *ResetObjectButton2;

    ///////////////////////////////////////////////////////////////////////
    //set up the texture and clip control panel

    //clip options
    QCheckBox       *ClipSelect;
    QPushButton     *grabdesktopClip; 
    QPushButton     *keyimage_2;       
    QCheckBox       *KeySelect;
    QPushButton     *grabdesktopKey;
    QPushButton     *keyimage_3;
    QPushButton     *keyStatusBox;

    //video options
    QLabel          *VideoLabel;
    QToolButton     *lockicon;

    QCheckBox       *extendHeadTail;
    QCheckBox       *toggleTheLoop;
    QCheckBox       *togglePing;

    ///////////////////////////////////////////////////////////////////////
    //set up the effects control panel

    QCheckBox       *ForegroundSelect;    
    QCheckBox       *DepthSelect;
    QCheckBox       *BlurSelect;
    QSlider         *HBlurSlider, *VBlurSlider;
    InputLCD        *HBlurSlider_lcd, *VBlurSlider_lcd;

    QCheckBox       *NegativeSelect, *SwapRGBSelect, *MirrorSelect; 
    JahToolButton   *lighticon;
    QCheckBox       *light1, *reflect1, *shader1, *shader2, *smooth1, *mesh1;

    QLabel          *Transparency_1;     
    QSlider         *Trans_1;         
    InputLCD        *Translcd_1;

	///////////////////////////////////////////////////////////////////////
	// for background select options
    QCheckBox *backgroundSelect;
    QPushButton *grabBackgroundClip;


};


#endif // KEYER_H





