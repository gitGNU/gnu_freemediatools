/*******************************************************************************
**
** The header file for the Jahshaka effects module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/
#ifndef EFFECT_H
#define EFFECT_H

#include "glworld.h"

#include <qcheckbox.h>
#include <qtabwidget.h>

#include "jahnodes.h"
#include "jahtimer.h"

//for dynamic plugin support
#include "jahpluginlib.h"

#include <vector>

////////////////////////////////////////////////////////////
//for gpu support
#include "gpumathlib.h"

class EffectInfo;

class GLEffect :  public  GLWorld {
Q_OBJECT

private:
	QHBox *m_parent1;
	QHBox *m_controls;
    QHBox *m_controller;
	QHBox *m_leftcontroller;
	QHBox *m_rightcontroller;

public:

	GLEffect( GLCore* core, QHBox*, QHBox* parent1, const char* name, QHBox* controls,
		  QHBox* controller,QHBox* leftcontroller, QHBox* rightcontroller,
		  int* globalclipnumber );
	QHBox * nodeparent;
	~GLEffect();

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

    void addClip(assetData clip);
    void updateSliders();
    void updateSliderValues();
    void updateUiOptions();
    void updateUiSettings();

	GLfloat setZoom( GLfloat );

public slots:

    void addCpuEffect();
    void addMeshEffect();
    void addGpuEffect();
    void addCpuEffectFromTopMenu();
    void addMeshEffectFromTopMenu();
    void addGpuEffectFromTopMenu();

    void grabDesktop(void) { emit GrabClip(); };

    // plugin based routines

    //option selection routines
    void setOption1(void);
    void setOption2(void);
    void setOption3(void);
    void setOption4(void);

	//for videorender option
	void    changeVideoRender(void);
	void    changeRenderOnlyToSelectedEffect(void);

signals:
    void    GrabClip(void);    //linked to main jahcontrol to talk to desktop

protected:

    void objectMenu(int menu);
    void paintGL();

protected:

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
    QCheckBox*      m_render_only_to_selected_effect_qcheckbox;

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

    QLabel			*FindMediaLabel;
    QLineEdit		*FindMediaText;
    QPushButton		*FindMediaButton;


};



#endif // EFFFECT_H





