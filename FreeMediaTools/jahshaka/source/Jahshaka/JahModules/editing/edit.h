/*******************************************************************************
**
** The header file for the Jahshaka editing module
** The Jahshaka Project
** Copyright (C) 2000-2005 The Jahshaka Project
* Released under the GNU General Public License
**
*******************************************************************************/
#ifndef EDIT_H
#define EDIT_H

#include <glworld.h>

#include <jahtimeline.h>

//for the mediatable
#include "mediatable.h"

#ifdef MLTSUPPORT
#error "MLT is supported - the edit module isn't needed - uses edit_mlt instead"
#endif

class GLEdit :  public  GLWorld {
  Q_OBJECT

public:

  // remove functions from the header! DOH?
GLEdit( GLCore* core, QHBox*, QHBox*, const char* name,
        QHBox* controls, QHBox* controller, QHBox* leftcontroller, QHBox* rightcontroller,
	int* globalclipnumber );


  // you dont need the function name in the class decl, doh! GLEdit::
  ~GLEdit();

	//for the editing timeline
    EditDesktop*           theeditdesktop;

    void buildInterface( QHBox* qtinterface);
    void buildInterfaceMenus( QHBox* leftcontroller, QHBox* rightcontroller );

	void createMenuItem(QPopupMenu * themenu );

    void initializeObjects();

    JahDesktopSideTable *mediatable;

public slots:

    ///////////////////////////////////////////////////////////////////
    //used to set up custom mediatable
    void                initializeMediaTable(QHBox* parentbox);
		
    void addLayer(void);

    //void clicked( QListViewItem* item );

    void changeParticle();
    void changeFontDraw();
    void changeObjectDraw();
    void keyCliplayer();
    void keyClipkey();

    void updateUiSettings();

    void grabDesktop(void) { emit GrabClip(); };
    void addDesktop(void)  { emit AddClip(); };

    //used for grab on add clip
    void addClipEdit(assetData newclip);

    void updateanimframe(int frame);//          {       // TODO     }

    void glworldUpdate();//TODO: empty function


    JahLayer* m_layers; // missing member, did they ever compile this?
    int m_number_of_layers; // missing member, did they ever compile this?

    void addListViewItem(bool, bool); // new function


    void updateEditanimframe(int frame); //MOVE OUT OF LINE         { updateanimframe(frame); updateTimeSlider(frame); }
    void updateSetinFrames(int frame, int layer); //MOVE OUT OF LINE { m_active_layer = layer; setinFrames(frame); }

signals:
    void    GrabClip(void);    //linked to main jahcontrol to talk to desktop
    void    AddClip(void);    //linked to main jahcontrol to talk to desktop

//protected:

  //  void paintGL();

private:

	///////////////////////////////////////////////////////////////////////
	//interface variables for the left and right world space menus
	///////////////////////////////////////////////////////////////////////

    QLabel* sframetext; InputLCD * lcdstartframe;
    QLabel* eframetext; InputLCD * lcdendframe;


	
	QFrame			*glworldLeft;
	JahToolButton	*tooltranslate,	*toolscale,		*toolrotate;
	JahToolButton	*playerphoto;
    JahToolButton     *tooladdkey, *toolremovekey, *toolpreviouskey, *toolnextkey;

	QFrame			*glworldRight;
	JahToolButton	*toolzoomin,	*toolzoomout;
	QSlider			*zoom;

	///////////////////////////////////////////////////////////////////////
	//animation module ui options
	///////////////////////////////////////////////////////////////////////

	//declare all controllers that we are using here
	enum tabUI {SCENE, OBJECT, AXIS, CLIP, EFFECTS, TIMELINE, numTabs};
  
	///////////////////////////////////////////////////////////////////////
	//now the main widget variables

	//the first, main frame
	QFrame*			LayersFrame;
	QFrame*			EffectsFrame;

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
	QLabel			*ScaleText_5;        
	QSlider			*ScaleX_5;         
	InputLCD		*ScaleXlcd_5;
	QLineEdit		*textEd;
	QPushButton		*textFont;           
	QPushButton		*textColor;        
	QPushButton		*textDraw;

	//the particle options
	QPushButton		*loadimage_3;
	QPushButton		*loadimage_4;
	QPushButton		*particlestyle;

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
	InputLCD		*FramesLcd;		
	InputLCD		*inFramesLcd;	
	InputLCD		*outFramesLcd;
	InputLCD		*KeyFramesLcd;	
	InputLCD		*inKeyFramesLcd;	
	InputLCD		*outKeyFramesLcd;
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

};



#endif // EDIT_H















