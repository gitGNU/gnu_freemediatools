/*******************************************************************************
**
** The header file for the Jahshaka keyer module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/
#ifndef COLORIZE_H
#define COLORIZE_H

#include "glworld.h"

#include <qcheckbox.h>
#include <qtabwidget.h>

class QDial;

#include "jahnodes.h"
#include "jahtimer.h"

#include <vector>

#include "colorcorrectorwidget.h"

////////////////////////////////////////////////////////////
//for gpu support
#include "gpumathlib.h"

class EffectInfo;

class GLColorize :  public  GLWorld {
Q_OBJECT

private:
	QHBox *m_controls;
	QHBox *m_leftcontroller;
	QHBox *m_rightcontroller;
 public:
	GLColorize( GLCore* core,
		    const char* name,
		    QHBox* controls,
		    QHBox* controller,
		    QHBox* leftcontroller,
		    QHBox* rightcontroller,
		    int* globalclipnumber );
	~GLColorize();
protected:
	void start( );
public:
    //object and variable initialization routines
    void initializeVariables(void);
    void initializeObjects();
    //ui initialization routines
    ColorCorrectorWidget* createColorPanel ( QFrame* theuiframe);
    void buildInterface( QHBox* qtinterface);
    void buildInterfaceMenus( QHBox* leftcontroller, QHBox* rightcontroller );
    void createMenuItem(QPopupMenu * themenu );
    void configureModuleEngine(void);
    void addClip(assetData clip); //moved to the core?
    void updateSliders();
    void updateSliderValues();
    void updateUiOptions();
    void updateUiSettings();
    QCheckBox* getGpuSelectCheckbox(); 

    // Override of GLWorld method
    void setDefaultAxisSliderLabels() { }
    ColorCorrectorWidget* m_color_corrector_widget;

public slots:    
    void grabDesktop(void) { emit GrabClip(); };
    void grabDesktopBaselayer(void)   { emit GrabBaseClip();   };

    // The following slots deal with the color corrector wheel widget communicating with the sliders etc.
    // Often, unit converstions are required.
    void hueShiftHelperSlot( int value );
    void hueShiftSliderHelperSlot( int value );
    void hueStrengthHelperSlot( int value );
    void saturationHelperSlot( int value );
    void hueTintHelperSlot( int value );
    void hueWheelOffsetHelperSlot( int value );
    void hueWheelStrengthHelperSlot ( int value );
    void hueWheelChangeHelperSlot ( int offsetvalue, int strengthvalue );
    void setBaseRGB( const QColor &col );
    void gpuSelect( void );
    void setRed(int value);
    void setGreen(int value);
    void setBlue(int value);
    void setBrightness(int value);
    void setHue(int value);
    void setSaturation(int value);
    void setValue(int value);

    //for videorender option
    void    changeVideoRender(void);
 signals:
    void    GrabClip(void);    //linked to main jahcontrol to talk to desktop
    void    GrabBaseClip(void);    //linked to main jahcontrol to talk to desktop
protected:
    void objectMenu(int menu);
    void paintGL();
private:
    ////////////////////////////////////////////////////////////////////////////
    // effect based variables
    QImage      sourceimage,savedimage;
    bool        rebuildStatus;
    int         iScreen;
    //The effects timer object
    JahTimer    *fxtimer;

    ////////////////////////////////////////////////////////////////////////////
    //interface variables
    ///////////////////////////////////////////////////////////////////////
    //interface variables for the left and right world space menus
    ///////////////////////////////////////////////////////////////////////
    QFrame			*glworldLeft;
    JahToolButton	*tooltranslate,	*toolscale,		*toolrotate;
    JahToolButton	*playerphoto,	*switchtheview;
    JahToolButton     *tooladdkey, *toolremovekey, *toolpreviouskey, *toolnextkey;
    QFrame			*glworldRight;
    JahToolButton	*toolzoomin,	*toolzoomout;
    QSlider			*zoom;
    ///////////////////////////////////////////////////////////////////////
    //animation module ui options
	///////////////////////////////////////////////////////////////////////
    
    //declare all controllers that we are using here
    enum tabUI {SCENE, OBJECT, AXIS, CLIP, EFFECTS, KEYFRAMES, numTabs};
    
    ///////////////////////////////////////////////////////////////////////
    //now the main widget variables
    
    //the first, main frame
    QFrame*			LayersFrame;
    QFrame*			EffectsFrame;
    
    //for the control panel
    QPushButton*    AddRTButton;
    QPushButton*    AddGpuButton;
    //    QPushButton*    AddPluginButton;
    
    ///////////////////////////////////////////////////////////////////////
    //the tab frame to hold the tabs
    
    QTabWidget		*tabframe;
    
    //widegts for each of the tabs
    QWidget			*moduleOptionsUI[numTabs];
    
    //each module option ui can have up to 10 embedded ui's
    QFrame			*ControlFrameUI[numTabs][10];
    
    ///////////////////////////////////////////////////////////////////////
    // variables for the different tabs
    
    ///////////////////////////////////////////////////////////////////////
    //set up the scene controls

    QPushButton*	ResetButton;
    QPushButton*	SaveButton;
    QPushButton*	LoadButton;
    QPushButton*	ClearButton;
    QPushButton*	PackageButton;
    QCheckBox*      VideoRenderSelect;
    
    //lcdstartframe and endframe are initialized in world
    
    QCheckBox*		FogSelect;
    QSlider			*HFogSlider,		*VFogSlider;
    InputLCD		*HFogSlider_lcd,	*VFogSlider_lcd;
    
    //////////////////////////////////////////////////////////////////////
    //set up the different object controls, multiple layers are embedded
    
    //the world
    QCheckBox		*SmoothSelect;
    QCheckBox		*TextSelect;
    QCheckBox		*GridSelect;
    QPushButton		*ResetWorldButton;
    
    //the layer types
    QPushButton		*ObjectLayer;
    QPushButton		*ObjectCube;
    QPushButton		*ObjectCylinder;
    QPushButton		*ObjectSphere;
    QPushButton		*ObjectBezier;
    QPushButton		*ObjectMesh;
    QPushButton		*ResetLayerButton;
    
    //the font options
    //the particle options
    
    //the object options obj's
    QPushButton		*loadobject_1,	*objectstyle;
    QPushButton		*ResetObjectButton1;
    
    //the object options 3dsmax
    QPushButton		*loadmaxobject_1;
    QPushButton		*ResetObjectButton2;
    
    ///////////////////////////////////////////////////////////////////////
    //set up the texture and clip control panel
    
    //clip options
    QCheckBox		*ClipSelect;
    QPushButton		*grabdesktopClip;
    QPushButton		*keyimage_2;
    QCheckBox		*KeySelect;
    QPushButton		*grabdesktopKey;
    QPushButton		*keyimage_3;
    QPushButton		*keyStatusBox;
    
    //video options
    QLabel			*VideoLabel;
    QToolButton		*lockicon;
    
    QSlider*        m_gamma_slider;
    InputLCD*       m_gamma_lcd;
    QLabel*         m_gamma_label;
    
    
    QCheckBox		*extendHeadTail;
    QCheckBox		*toggleTheLoop;
    QCheckBox		*togglePing;
    
    ///////////////////////////////////////////////////////////////////////
    //set up the effects control panel
    
    QCheckBox		*ForegroundSelect;
    QCheckBox		*DepthSelect;
    QCheckBox		*BlurSelect;
    QSlider			*HBlurSlider, *VBlurSlider;
    InputLCD		*HBlurSlider_lcd, *VBlurSlider_lcd;

    QCheckBox		*NegativeSelect, *SwapRGBSelect, *MirrorSelect;
    JahToolButton	*lighticon;
    QCheckBox		*light1, *reflect1, *shader1, *shader2, *smooth1, *mesh1;
    QLabel			*Transparency_1;
    QSlider			*Trans_1;
    InputLCD		*Translcd_1;

    QCheckBox*      m_gpu_select_qcheckbox;


    //for the color qdials
    QLabel*        TranslateText_1;

    QDial*         TranX_1;       InputLCD*  TranXlcd_1;
    QDial*         TranY_1;       InputLCD*  TranYlcd_1;
    QDial*         TranZ_1;       InputLCD*  TranZlcd_1;

    QLabel*        TranslateText_2;

    QDial*       RotX_1;        InputLCD*  RotXlcd_1;
    QDial*       RotY_1;        InputLCD*  RotYlcd_1;
    QDial*       RotZ_1;        InputLCD*  RotZlcd_1;

    QLabel*        TranslateText_3;

    QDial*       ScaleX_1;      InputLCD*  ScaleXlcd_1;
    QDial*       ScaleY_1;      InputLCD*  ScaleYlcd_1;
    QDial*       ScaleZ_1;      InputLCD*  ScaleZlcd_1;

    // ColorCorrector widget
    ColorCorrectorWidget *ccw;
};



#endif // ANIMATION_H





