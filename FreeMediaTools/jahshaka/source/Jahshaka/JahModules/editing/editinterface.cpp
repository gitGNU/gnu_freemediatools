/*******************************************************************************
**
** The source file for the Jahshaka editing interface module
** The Jahshaka Project
** Copyright (C) 2000-2005 The Jahshaka Project
** Released under the GNU General Public License
**
*******************************************************************************/

#include "edit.h"
#include <jahformatter.h>
#include <qslider.h>
#include <qpopupmenu.h>
#include <jahtranslate.h>
#include <jahpreferences.h>
#include <InputLCD.h>
#include <dialogs.h>
#include <qlistview.h>
#include <qwidgetstack.h>
#include <qhbox.h>
#include <qlayout.h>
#include <valueFormatters.h>

#include <qmultilineedit.h>

void GLEdit::buildInterfaceMenus( QHBox*, QHBox* rightcontroller) 
{

    //rightmenu here
    glworldRight =  new QFrame ((QWidget*)rightcontroller, (const char*)"gl right",0 );

    toolzoomin =   new JahToolButton( glworldRight, "zoomin" );

    JahFormatter::addJahPlayerButton( toolzoomin,   0, 15, 30, 27 ,    24, 28, 
        m_qsJahBasePath+"Pixmaps/desktop/zoomin.png", m_qsJahBasePath+"Pixmaps/desktop/zoomin.png" );

    zoom = new QSlider( glworldRight, "scrubslider" );
    zoom->setGeometry( QRect( 0, 50, 30, 150 ) );
    zoom->setOrientation( QSlider::Vertical );
    zoom->setMinValue( -10 );    
    zoom->setMaxValue( 10 );
    zoom->setValue   ( 0 );
    zoom->setPageStep( 2 );

    toolzoomout =   new JahToolButton( glworldRight, "zoomout" );
    JahFormatter::addJahPlayerButton( toolzoomout,   0, 205, 30, 27 ,    24, 28, 
        m_qsJahBasePath+"Pixmaps/desktop/zoomout.png", m_qsJahBasePath+"Pixmaps/desktop/zoomout.png" );


	//moving ui elements after zoom
    tooltranslate = new JahToolButton( glworldRight, "translate" );

    JahFormatter::addJahPlayerButton( tooltranslate, 0, 250, 30, 27 ,    24, 28, 
        m_qsJahBasePath+"Pixmaps/desktop/transtool.png", m_qsJahBasePath+"Pixmaps/desktop/transtool.png" );

    toolrotate =   new JahToolButton( glworldRight, "rotate" );

    JahFormatter::addJahPlayerButton( toolrotate,   0, 283, 30, 27 ,    24, 28, 
        m_qsJahBasePath+"Pixmaps/desktop/rotatetool.png", m_qsJahBasePath+"Pixmaps/desktop/rotatetool.png" );

    toolscale =   new JahToolButton( glworldRight, "scale" );
    JahFormatter::addJahPlayerButton( toolscale,   0, 313, 30, 27 ,    24, 28, 
        m_qsJahBasePath+"Pixmaps/desktop/scaletool.png", m_qsJahBasePath+"Pixmaps/desktop/scaletool.png" );

    m_pScrubRender = new JahToolButton( glworldRight, "controllerrewindbutton" );
	JahFormatter::addJahPlayerButton( m_pScrubRender, 0, 400, 27, 29,   28, 30, 
		m_qsJahBasePath+"Pixmaps/desktop/phototool.png", m_qsJahBasePath+"Pixmaps/desktop/phototool.png" );
    connect( m_pScrubRender,  SIGNAL(clicked()), SLOT(Render())  );

    m_pScrubRenderAll = new JahToolButton( glworldRight, "controllerpreviousbutton" );
    JahFormatter::addJahPlayerButton( m_pScrubRenderAll, 0, 435, 27, 29,  24, 28, 
		m_qsJahBasePath+"Pixmaps/desktop/rendertool.png", m_qsJahBasePath+"Pixmaps/desktop/rendertool.png" );
    connect( m_pScrubRenderAll,  SIGNAL(clicked()), SLOT(RenderScene())  );

    //connect tools to object and add object to display widget
    connect( tooltranslate,  SIGNAL(clicked()), this, SLOT(toolTranslate() )  );
    connect( toolscale,      SIGNAL(clicked()), this, SLOT(toolScale() )  );
    connect( toolrotate,     SIGNAL(clicked()), this, SLOT(toolRotate() )  );

    connect( toolzoomin,    SIGNAL(clicked()), this, SLOT(resetZoom() )  );
    connect( toolzoomout,    SIGNAL(clicked()), this, SLOT(resetZoom() )  );
    connect( zoom,  SIGNAL(valueChanged(int)), this, SLOT(changeZoom(int) )  );

}


void GLEdit::buildInterface( QHBox* qtinterface ) 
{
	//set up translator
    //JahTranslate& m_pJt = JahTranslate::getInstance();
    JahPrefs& jprefs = JahPrefs::getInstance();

    ///////////////////////////////////////////////////////////
    //The layers interface
    LayersFrame = new QFrame( (QWidget*)qtinterface, (const char *)"EffectsFrame",0 );
    LayersFrame->setGeometry( QRect( 0, 0, 75, 195 ) );
    LayersFrame->setMinimumSize( QSize( 75, 195 ) );  //was 140
    LayersFrame->setMaximumSize( QSize( 75, 195 ) );
    
    ///////////////////////////////////////////////////////////
    //The layer options
    //these are defined in the world object
    m_pAddButton = new QPushButton( LayersFrame, "m_pAddButton" );
    JahFormatter::addButton( m_pAddButton,  0, 5, 72, 25,  m_pJt->tr("Add") );
    connect  ( m_pAddButton,  SIGNAL(clicked()), this, SLOT( addLayer())  );

	m_pNameButton = new QPushButton( LayersFrame, "m_pNameButton" );
	JahFormatter::addButton( m_pNameButton, 0, 30, 72, 25, m_pJt->tr("Name") );
	connect  ( m_pNameButton,  SIGNAL(clicked()), this, SLOT( nameLayer())  );

	m_pNamePopUp = new FancyPopup( this, "Enter Layer Name",250,150 );  //send size and position as well
	connect ( m_pNamePopUp,      SIGNAL( returnText(QString)), SLOT( setlayerName(QString)) );

	m_pDelButton = new QPushButton( LayersFrame, "Delete Button" );
	JahFormatter::addButton( m_pDelButton, 0, 60, 72, 25 , m_pJt->tr("Del") );
	connect  ( m_pDelButton,  SIGNAL(clicked()), this, SLOT( delLayer())  );

	//////////////////////////////////////////////////////////////
	// set up the scene options, or prefernces
	
	EffectsFrame = new QFrame((QWidget*)qtinterface,"",0);
	EffectsFrame->setGeometry( QRect( 60, 0, 820, 195) );

    /////////////////////////////////////////////////////////
    //this is the list-view
    m_pLayerListView = new QListView( EffectsFrame );
    m_pLayerListView->setRootIsDecorated(true);

    connect( m_pLayerListView, SIGNAL( clicked( QListViewItem* ) ), this, SLOT( layerClicked( QListViewItem* ) ) );

    m_pLayerListView->setGeometry( QRect( 1, 1, 166, 190 ) );
    m_pLayerListView->addColumn( (m_pJt->tr("LAYERS")), -1 );
    m_pLayerListView->setColumnWidth ( 0,140 );
    m_pLayerListView->setSorting( -1,1 );   // disables the autosort
    m_pLayerListView->setVScrollBarMode (QScrollView::AlwaysOn);
    m_pLayerListView->setHScrollBarMode (QScrollView::AlwaysOff);

    //allow for multiple m_bSelections
    //m_pLayerListView->setSelectionMode (QListView::Extended );

    /////////////////////////////////////////////////////////
    //Set up tabbed interface
    
    tabframe = new QTabWidget( EffectsFrame, "axiscontrols" );	
    tabframe->setGeometry( QRect( 170, 1, 656, 190 ) );
    
    //tabframe = new QTabWidget( object_controls, "axiscontrols" );	
    //tabframe->setGeometry( QRect( 1, 2, 700, 180 ) );
    
    tabframe->setTabShape( QTabWidget::Triangular );
    tabframe->setAutoMask( FALSE );
    
    //this has become the settings panel need to update variable names
    moduleOptionsUI[0] = new QWidget( EffectsFrame, "encoderUI0" ); //scenecontrols
    tabframe->insertTab( moduleOptionsUI[0], tr( "  "+m_pJt->tr("SCENE")+"  " ) );
    
    moduleOptionsUI[1] = new QWidget( EffectsFrame, "objectcontrols" ); //objectcontrols
    tabframe->insertTab( moduleOptionsUI[1], tr( "  "+m_pJt->tr("OBJECT")+"  " ) );
    
    moduleOptionsUI[2] = new QWidget( EffectsFrame, "axiscontrols" ); //axiscontrols
    tabframe->insertTab( moduleOptionsUI[2], tr( "  "+m_pJt->tr("CONTROLS")+"  " ) );
    
    moduleOptionsUI[3] = new QWidget( EffectsFrame, "texturecontrols" ); //texturecontrols
    tabframe->insertTab( moduleOptionsUI[3], tr( "    "+m_pJt->tr("CLIP")+"    " ) );
    
    //moduleOptionsUI[4] = new QWidget( EffectsFrame, "effectscontrols" ); //effectscontrols
    //tabframe->insertTab( moduleOptionsUI[4], tr( "  "+m_pJt->tr("EFFECTS")+"  " ) );
    
    //this has become the settings panel need to update variable names
    moduleOptionsUI[4] = new QWidget( EffectsFrame, "keysettings" ); //keycontrols
    tabframe->insertTab( moduleOptionsUI[4], tr( "  "+m_pJt->tr("TIMELINE")+"  " ) );
    
    //this raises the timeline above all other objects
    tabframe->setCurrentPage( 4 );
    
    /////////////////////////////////////////////////////////////////////
    // the layout boxes for different types of layers
    // the object control tab bar
    // this is the object control panel
    
    m_pObjectControlStack = new QWidgetStack(moduleOptionsUI[1]); //objectcontrols
    
    //initialize the stack for object switching
    for (int i=0; i<=6; i++)
    {
        m_aObjectControls[i]  = new QHBox(m_pObjectControlStack);          
        m_pObjectControlStack->addWidget(m_aObjectControls[i], i); //fxobject_clip
    }
    
    m_pObjectControlStack->setGeometry( QRect( 0, 0, 660, 190));   // was 635
    
    
    QPalette p( QColor( 0, 0, 0 ) );
    p.setColor( QPalette::Active, QColorGroup::Foreground, Qt::green );

    ///////////////////////////////////////////////////////////////////
    // the scene control tab bar

    //mover this into the layers...
    ControlFrameUI[0][0] = new QFrame( moduleOptionsUI[0], "ControlFrame3" ); //scenecontrols
    ControlFrameUI[0][0]->setGeometry( QRect( 0, 0, 660, 190 ) );

    LoadButton = new QPushButton( ControlFrameUI[0][0], "LoadButton" );
    JahFormatter::addButton( LoadButton, 10, 10, 86, 25 , m_pJt->tr("Load") );
    connect  ( LoadButton,  SIGNAL(clicked()), this, SLOT( SceneLoad())  );

    SaveButton = new QPushButton( ControlFrameUI[0][0], "SaveButton" );
    JahFormatter::addButton( SaveButton, 10, 45, 86, 25, m_pJt->tr("Save") );
    connect  ( SaveButton,  SIGNAL(clicked()), this, SLOT( sceneSave())  );

    ResetButton = new QPushButton( ControlFrameUI[0][0], "ResetButton" );
    JahFormatter::addButton( ResetButton, 10, 80, 86, 25, m_pJt->tr("Reset All") );
    connect  ( ResetButton,  SIGNAL(clicked()), this, SLOT( ResetAll())  );

    ClearButton = new QPushButton( ControlFrameUI[0][0], "ClearButton" );
    JahFormatter::addButton( ClearButton, 10, 115, 86, 25, m_pJt->tr("Clear Scene") );
    connect( ClearButton,  SIGNAL(clicked()), this, SLOT( ClearAll())  );
    
    //PackageButton = new QPushButton( ControlFrameUI[0][0], "ClearButton" );
    //JahFormatter::addButton( PackageButton, 110, 80, 86, 25, "Export Scene" );
    //connect( PackageButton,  SIGNAL(clicked()), this, SLOT( PackageSave())  );

  //need to move this into the module
    //this is mandatory for all modules...

    sframetext = new QLabel( ControlFrameUI[0][0], "sframetext" );
    sframetext->setGeometry( QRect( 390, 6, 100, 27 ) );
    sframetext->setText( tr( m_pJt->tr("STARTFRAME") ) );
    sframetext->setAlignment( int( QLabel::AlignRight ) );
    QFont sframetext_font(  sframetext->font() );
    sframetext_font.setPointSize( 9 );
    sframetext->setFont( sframetext_font );

    lcdstartframe = new InputLCD( ControlFrameUI[0][0], "startframe" );
    lcdstartframe->setFormatter( TimecodeValueFormatter() );
    JahFormatter::addSliderLcd(lcdstartframe, 500, 5, 90, 21, m_nAStartFrame);
    lcdstartframe->setMinInt(m_nAStartFrame);
    lcdstartframe->setValue(m_nAStartFrame); // TODO:this data was not initialized. 
    connect( lcdstartframe, SIGNAL(valueChanged(int)),   SLOT(updatesliderStartframe(int)) );

    eframetext = new QLabel( ControlFrameUI[0][0], "eframetext" );
    eframetext->setGeometry( QRect( 390, 36, 100, 27 ) );
    eframetext->setText( tr( m_pJt->tr("ENDFRAME")  ) );
    eframetext->setAlignment( int( QLabel::AlignRight ) );
    QFont eframetext_font(  eframetext->font() );
    eframetext_font.setPointSize( 9 );
    eframetext->setFont( eframetext_font );

    lcdendframe = new InputLCD( ControlFrameUI[0][0], "endframe" );
    lcdendframe->setFormatter( TimecodeValueFormatter() );
    JahFormatter::addSliderLcd(lcdendframe, 500, 34, 90, 21, m_nAEndFrame);
    lcdendframe->setMinInt( m_nAStartFrame+1 );
    lcdendframe->setValue(m_nAEndFrame);
    connect( lcdendframe, SIGNAL(valueChanged(int)),   SLOT(updatesliderEndframe(int)) );

    m_pJahResLabel = new QLabel( ControlFrameUI[0][0], "languagelabel" );
    m_pJahResLabel->setGeometry( QRect( 450, 75, 120, 21 ) );
    m_pJahResLabel->setAlignment( int( QLabel::AlignCenter ) );
    m_pJahResLabel->setText( m_pJt->tr( "Resolution" ) );

    m_pJahResCombo = new QComboBox( FALSE, ControlFrameUI[0][0], "m_pJahResCombo" );
    m_pJahResCombo->setGeometry( QRect( 450, 100, 170, 21 ) );
	m_bHasResMenu = true;

	projectData thedata;
	thedata.buildComboBox(m_pJahResCombo);
    int defaultRes = jprefs.getJahResolution();
	m_pJahResCombo->setCurrentItem(defaultRes); //should be set to appdefault
    connect( m_pJahResCombo,  SIGNAL(activated(int)), this, SLOT(setResolution(int))  );


    ///////////////////////////////////////////////////////////////////////
    //set up world sub control set
	//these are all in ControlFrameUI[1]
	//since fxobject_world is in ControlFrameUI[1]

    //ControlFrameUI[1][0] = new QFrame( fxobject_world, "ControlFrame3" ); //was ControlFrame3
    
	ControlFrameUI[1][0] = new QFrame( m_aObjectControls[2], "ControlFrame3" ); //was ControlFrame3
    ControlFrameUI[1][0]->setGeometry( QRect( 0, 0, 660, 190 ) );


    TextSelect = new QCheckBox( ControlFrameUI[1][0], "TitleSafe" );
    JahFormatter::addCheckButton( TextSelect, 300, 80, 110, 21, m_pJt->tr("title safe") );
    connect( TextSelect,  SIGNAL(clicked()), this, SLOT(setTextSafe())  );

    GridSelect = new QCheckBox( ControlFrameUI[1][0], "Grid" );
    JahFormatter::addCheckButton( GridSelect, 300, 40, 110, 21, m_pJt->tr("grid") );
    connect( GridSelect,  SIGNAL(clicked()), this, SLOT(setGrid())  );

    SmoothSelect = new QCheckBox( ControlFrameUI[1][0], "SmoothSelect" );
    JahFormatter::addCheckButton( SmoothSelect, 300, 10, 110, 21, m_pJt->tr("aliasing") );
    connect( SmoothSelect,  SIGNAL(clicked()), this, SLOT(setSmooth())  );

    ResetWorldButton = new QPushButton( ControlFrameUI[1][0], "ResetLayerButton" );
    JahFormatter::addButton( ResetWorldButton, 410, 10, 86, 25, m_pJt->tr("Reset") );
    connect  ( ResetWorldButton,  SIGNAL(clicked()), this, SLOT( ResetLayer())  );

    //initalize the global vars from the prefs
    bool HWFOG = jprefs.getHwFog();

    if (HWFOG)
    {
        //toggle layer blur - need to add sliders here
        FogSelect = new QCheckBox( ControlFrameUI[1][0], "fog" );
        JahFormatter::addCheckButton( FogSelect, 500, 70, 110, 21, m_pJt->tr("fog") );
        connect( FogSelect,  SIGNAL(clicked()), this, SLOT(setFog())  );

        //QSlider       *HFogSlider, *VFogSlider;
        HFogSlider = new QSlider( ControlFrameUI[1][0] );    HFogSlider_lcd = new InputLCD( ControlFrameUI[1][0] );
        JahFormatter::addSliderLcdLabel( ControlFrameUI[1][0], HFogSlider, 500, 90, 10, 21,  100, HFogSlider_lcd,  "H", 9);
        connect( HFogSlider,     SIGNAL(sliderMoved(int)), this, SLOT(setHFog(int)) );
        connect( HFogSlider,     SIGNAL(sliderMoved(int)), HFogSlider_lcd, SLOT(setValue(int)) );
        connect( HFogSlider_lcd, SIGNAL(valueChanged(int)), this, SLOT(setHFog(int)) );

        VFogSlider = new QSlider( ControlFrameUI[1][0] );    VFogSlider_lcd = new InputLCD( ControlFrameUI[1][0] );
        JahFormatter::addSliderLcdLabel( ControlFrameUI[1][0], VFogSlider, 500, 120, 10, 21,  100, VFogSlider_lcd,  "V", 9);
        connect( VFogSlider,     SIGNAL(sliderMoved(int)), this, SLOT(setVFog(int)) );
        connect( VFogSlider,     SIGNAL(sliderMoved(int)), VFogSlider_lcd, SLOT(setValue(int)) );
        connect( VFogSlider_lcd, SIGNAL(valueChanged(int)), this, SLOT(setVFog(int)) );
    }

    ///////////////////////////////////////////////////////////////
    // object layer options sub control set
	//these are all in ControlFrameUI[1]
	//since fxobject_clip is in ControlFrameUI[1]

    //ControlFrameUI[1][1] = new QFrame( fxobject_clip, "ControlFrame5" ); //ControlFrame5
    
	ControlFrameUI[1][1] = new QFrame( m_aObjectControls[0], "ControlFrame5" ); //ControlFrame5
    ControlFrameUI[1][1]->setGeometry( QRect( 0, 0, 660, 190 ) );

	//the different object types

    ObjectLayer = new QPushButton( ControlFrameUI[1][1], "Layer" );
    JahFormatter::addButton( ObjectLayer, 10, 10, 86, 25 , m_pJt->tr("Layer") );
    connect  ( ObjectLayer,  SIGNAL(clicked()), this, SLOT( ChangeObjectLayer())  );

    ObjectCube = new QPushButton( ControlFrameUI[1][1], "Cube" );
    JahFormatter::addButton( ObjectCube, 10, 45, 86, 25, m_pJt->tr("Cube") );
    connect  ( ObjectCube,  SIGNAL(clicked()), this, SLOT( ChangeObjectCube())  );

    ObjectCylinder = new QPushButton( ControlFrameUI[1][1], "Cylinder" );
    JahFormatter::addButton( ObjectCylinder, 10, 80, 86, 25, m_pJt->tr("Cylinder") );
    connect  ( ObjectCylinder,  SIGNAL(clicked()), this, SLOT( ChangeObjectCylinder())  );

    ObjectSphere = new QPushButton( ControlFrameUI[1][1], "Sphere" );
    JahFormatter::addButton( ObjectSphere, 110, 10, 86, 25, m_pJt->tr("Sphere") );
    connect( ObjectSphere,  SIGNAL(clicked()), this, SLOT( ChangeObjectSphere())  );

    ObjectBezier = new QPushButton( ControlFrameUI[1][1], "Bezier" );
    JahFormatter::addButton( ObjectBezier, 110, 45, 86, 25, m_pJt->tr("Bezier") );
    connect( ObjectBezier,  SIGNAL(clicked()), this, SLOT( ChangeObjectBezier())  );

    ObjectMesh = new QPushButton( ControlFrameUI[1][1], "Mesh" );
    JahFormatter::addButton( ObjectMesh, 110, 80, 86, 25, m_pJt->tr("Mesh") );
    connect( ObjectMesh,  SIGNAL(clicked()), this, SLOT( ChangeObjectMesh())  );

    ResetLayerButton = new QPushButton( ControlFrameUI[1][1], "ResetLayerButton" );
    JahFormatter::addButton( ResetLayerButton, 410, 10, 86, 25, m_pJt->tr("Reset") );
    connect  ( ResetLayerButton,  SIGNAL(clicked()), this, SLOT( ResetLayer())  );


    /////////////////font options
	//these are all in ControlFrameUI[1]
    //ControlFrameUI[1][2] = new QFrame( fxobject_text, "ControlFrame6" ); //was ControlFrame6
    
	ControlFrameUI[1][2] = new QFrame( m_aObjectControls[1], "ControlFrame6" ); //was ControlFrame6
    ControlFrameUI[1][2]->setGeometry( QRect( 0, 0, 660, 190 ) );

    ScaleText_5 = new QLabel( ControlFrameUI[1][2], "ScaleText_4" );
    JahFormatter::addLabel( ScaleText_5, 0, 10, 110, 21, "EXTRUDE", 9);

    ScaleX_5 = new QSlider( ControlFrameUI[1][2] ); ScaleXlcd_5 = new InputLCD( ControlFrameUI[1][2] );
    JahFormatter::addSliderLcdLabel( ControlFrameUI[1][2], ScaleX_5,   300, 30, 10, 21,  1, 200,  30,  ScaleXlcd_5,  "E", 9);

    connect( ScaleX_5,    SIGNAL(sliderMoved(int)), ScaleXlcd_5, SLOT(setValue(int)) );
    connect( ScaleX_5,    SIGNAL(sliderMoved(int)), this, SLOT(setExtrude(int)) );
    connect( ScaleXlcd_5, SIGNAL(valueChanged(int)), this, SLOT(setExtrude(int)) );

    textDraw = new QPushButton( ControlFrameUI[1][2], "draw" );
    JahFormatter::addButton( textDraw, 115, 30, 36, 21, "Draw" );
    connect( textDraw,  SIGNAL(clicked()), this, SLOT(changeFontDraw())  );

    m_pTextEd	= new QMultiLineEdit( ControlFrameUI[1][2], "text" );
    m_pTextEd->setGeometry( QRect( 0, 60, 150, 21 ) );
    m_pTextEd->setText( "jahshaka" );
    connect( m_pTextEd, SIGNAL(textChanged(const QString&)), this, SLOT(setText(const QString&)) );

    textFont = new QPushButton( ControlFrameUI[1][2], "font" );
    JahFormatter::addButton( textFont, 0, 90, 55, 21, "Font" );
    connect( textFont,  SIGNAL(clicked()), this, SLOT(loadFont())  );

    textColor = new QPushButton( ControlFrameUI[1][2], "color" );
    JahFormatter::addButton( textColor, 60, 90, 55, 21, "Color" );
    connect( textColor,  SIGNAL(clicked()), this, SLOT(setFontColor())  );

    ///////////////////////////////////////////////////////
    //particle options

    //ControlFrameUI[1][3] = new QFrame( fxobject_particle, "ControlFrame7" ); //was ControlFrame7
    
	ControlFrameUI[1][3] = new QFrame( m_aObjectControls[4], "ControlFrame7" ); //was ControlFrame7
    ControlFrameUI[1][3]->setGeometry( QRect( 0, 0, 660, 190 ) );

    loadimage_3 = new QPushButton( ControlFrameUI[1][3], "emitter" );
    JahFormatter::addButton( loadimage_3, 0, 20, 111, 20,  "Emitter" );
    connect( loadimage_3,  SIGNAL(clicked()), this, SLOT(changeParticle())  );

    loadimage_4 = new QPushButton( ControlFrameUI[1][3], "style" );
    JahFormatter::addButton( loadimage_4, 0, 60, 111, 20,  "Style" );
    //connect( loadimage_4,  SIGNAL(clicked()), this, SLOT(changeParticleDraw())  );

    particlestyle = new QPushButton( ControlFrameUI[1][3], "tex" );
    JahFormatter::addButton( particlestyle, 115, 20, 35, 20, "Color" );
    connect( particlestyle,  SIGNAL(clicked()), this, SLOT(changeParticleColors())  );

    ////////////////////////////////////////////////////////
    //object options 5

    ControlFrameUI[1][4] = new QFrame( m_aObjectControls[5], "ControlFrame9" ); //ControlFrame9
    ControlFrameUI[1][4]->setGeometry( QRect( 0, 0, 660, 190 ) );


    loadobject_1 = new QPushButton( ControlFrameUI[1][4], "load" );
    JahFormatter::addButton( loadobject_1, 0, 20, 111, 30, "Load Object" );
    connect( loadobject_1,  SIGNAL(clicked()), this, SLOT(loadObjObject())  );

    objectstyle = new QPushButton( ControlFrameUI[1][4], "draw" );
    JahFormatter::addButton( objectstyle, 115, 20, 35, 30, "fill" );
    connect( objectstyle,  SIGNAL(clicked()), this, SLOT(changeObjectDraw())  );

    ResetObjectButton1 = new QPushButton( ControlFrameUI[1][4], "ResetLayerButton" );
    JahFormatter::addButton( ResetObjectButton1, 410, 10, 86, 25, "Reset" );
    connect  ( ResetObjectButton1,  SIGNAL(clicked()), this, SLOT( ResetLayer())  );

    ////////////////////////////////////////////////////////
    //max object options - should be merged in with the object code above
	//as we can determine what we are loading when we are loading it no?
	//but then we need to monitor the 'object style' above... 6

    ControlFrameUI[1][4] = new QFrame( m_aObjectControls[6], "ControlFrame10" ); // was ControlFrame10
    ControlFrameUI[1][4]->setGeometry( QRect( 0, 0, 660, 190 ) );

    loadmaxobject_1 = new QPushButton( ControlFrameUI[1][4], "load" );
    JahFormatter::addButton( loadmaxobject_1, 0, 20, 111, 30, "Load MObject" );
    connect( loadmaxobject_1,  SIGNAL(clicked()), this, SLOT(loadObjObject())  );

    ResetObjectButton2 = new QPushButton( ControlFrameUI[1][4], "ResetLayerButton" );
    JahFormatter::addButton( ResetObjectButton2, 410, 10, 86, 25, "Reset" );
    connect  ( ResetObjectButton2,  SIGNAL(clicked()), this, SLOT( ResetLayer())  );


    ///////////////////////////////////////////////////////////////////////
    //set up object control set and sliders
    //set up world sub control set
	//these are all in ControlFrameUI[2]
	//since fxaxis_clipone is in ControlFrameUI[2]

    ControlFrameUI[2][0] = new QFrame( moduleOptionsUI[2], "ControlFrame1" ); //ControlFrame1
    ControlFrameUI[2][0]->setGeometry( QRect( 0, 0, 660, 190 ) );    // was 510

    createAxisPanel ( ControlFrameUI[2][0] );

    //add the checkbox
    m_aJahModuleOptions[0] = new QCheckBox( ControlFrameUI[2][0], "PathSelect" );
    JahFormatter::addCheckButton( m_aJahModuleOptions[0], 520, 30, 145, 25, "Show Path" );
    connect( m_aJahModuleOptions[0],  SIGNAL(clicked()), this, SLOT(setKeyframeDrawStatus())  ); //for clip

	//increment the ui uption counter...
	//we can add this for sliders as well
	//as we use them in the plugins
	
	m_nNumUiOptions +=1;

	m_bHasButtons = true;

    ///////////////////////////////////////////////////////////////
    // texture layer options
	// need to add video-texture options here

	//image options

    ControlFrameUI[3][0] = new QFrame( moduleOptionsUI[3], "ControlFrame2" ); //was ControlFrame2
    ControlFrameUI[3][0]->setGeometry( QRect( 0, 0, 660, 190 ) );

    ClipSelect = new QCheckBox( ControlFrameUI[3][0], "ClipSelect" );
    JahFormatter::addCheckButton( ClipSelect, 0, 30, 45, 25, "Clip" );
    connect( ClipSelect,  SIGNAL(clicked()), this, SLOT(setClipStatus())  ); //for clip

    grabdesktopClip = new QPushButton( ControlFrameUI[3][0], "grabdesktop" );
    JahFormatter::addButton( grabdesktopClip, 45, 30, 110, 25, "Grab Desktop" );
    connect( grabdesktopClip,  SIGNAL(clicked()), this, SLOT(grabDesktop())  );

    keyimage_2 = new QPushButton( ControlFrameUI[3][0], "clipmapping" );
    JahFormatter::addButton( keyimage_2, 165, 30, 35, 25, "IM" );
    connect( keyimage_2,  SIGNAL(clicked()), this, SLOT(keyCliplayer())  );


    KeySelect = new QCheckBox( ControlFrameUI[3][0], "KeySelect" );
    JahFormatter::addCheckButton( KeySelect, 0, 75, 45, 25, "Key" );
    connect( KeySelect,  SIGNAL(clicked()), this, SLOT(setKeyStatus())  );

    grabdesktopKey = new QPushButton( ControlFrameUI[3][0], "grabdesktop" );
    JahFormatter::addButton( grabdesktopKey, 45, 75, 110, 25, "Grab Desktop" );
    //connect( grabdesktopKey,  SIGNAL(clicked()), this, SLOT(grabDesktopKey())  ); //for key...

    keyimage_3 = new QPushButton( ControlFrameUI[3][0], "keyOne_3" );
    JahFormatter::addButton( keyimage_3, 165, 75, 35, 25, "KM" );
    connect( keyimage_3,  SIGNAL(clicked()), this, SLOT(keyClipkey())  );

    keyStatusBox = new QPushButton( ControlFrameUI[3][0], "Select1" );
    JahFormatter::addButton( keyStatusBox, 220, 75, 35, 25, "Invert" );
    connect( keyStatusBox,  SIGNAL(clicked()), this, SLOT(InvertKeyData())  );

	// video options
	// need to hook this up now...

	VideoLabel = new QLabel( ControlFrameUI[3][0], "TranslateText_1" );
	JahFormatter::addLabel( VideoLabel, 380, 10, 110, 21, "VIDEO OPTIONS", 9);


	FramesLcd = new InputLCD( ControlFrameUI[3][0] );
	JahFormatter::addLabelLcd( ControlFrameUI[3][0], FramesLcd,       380, 30, 60, 21,  0,  "Frames", 8);
	FramesLcd->setActive	(false);

    inFramesLcd = new InputLCD( ControlFrameUI[3][0] );
    JahFormatter::addLcdLabel( ControlFrameUI[3][0], inFramesLcd,     460, 30, 30, 21,  0,  "In", 8);
    connect(inFramesLcd, SIGNAL(valueChanged(int)), this, SLOT(setinFrames(int)) );

    outFramesLcd = new InputLCD( ControlFrameUI[3][0] );
    JahFormatter::addLcdLabel( ControlFrameUI[3][0], outFramesLcd,    530, 30, 30, 21,  0,  "Out", 8);
    connect(outFramesLcd, SIGNAL(valueChanged(int)), this, SLOT(setoutFrames(int)) );

    KeyFramesLcd = new InputLCD( ControlFrameUI[3][0] );
    JahFormatter::addLabelLcd( ControlFrameUI[3][0], KeyFramesLcd,    380, 75, 60, 21,  0,  "Frames", 8);

    inKeyFramesLcd = new InputLCD( ControlFrameUI[3][0] );
    JahFormatter::addLcdLabel( ControlFrameUI[3][0], inKeyFramesLcd,  460, 75, 30, 21,  0,  "In", 8);
    connect(inKeyFramesLcd, SIGNAL(valueChanged(int)), this, SLOT(setinKeyFrames(int)) );

    outKeyFramesLcd = new InputLCD( ControlFrameUI[3][0] );
    JahFormatter::addLcdLabel( ControlFrameUI[3][0], outKeyFramesLcd, 530, 75, 30, 21,  0,  "Out", 8);
    connect(outKeyFramesLcd, SIGNAL(valueChanged(int)), this, SLOT(setoutKeyFrames(int)) );
    
    extendHeadTail = new QCheckBox( ControlFrameUI[3][0], "extendHeadTail" );
    JahFormatter::addCheckButton( extendHeadTail, 380, 95, 100, 25, "Extend Head-Tail" );
    connect( extendHeadTail,  SIGNAL(clicked()), this, SLOT(toggleExtendHeadTail())  );

    toggleTheLoop = new QCheckBox( ControlFrameUI[3][0], "toggleTheLoop" );
    JahFormatter::addCheckButton( toggleTheLoop, 480, 95, 100, 25, "Loop" );
    connect( toggleTheLoop,  SIGNAL(clicked()), this, SLOT(toggleLoop())  );
    
	togglePing = new QCheckBox( ControlFrameUI[3][0], "togglePing" );
    JahFormatter::addCheckButton( togglePing, 480, 130, 100, 25, "Ping Pong" );
    connect( togglePing,  SIGNAL(clicked()), this, SLOT(togglePingPong())  );

    //what is this used for?
    //need to make the icon a lock... 3x current height as well for links
  	lockicon	= new QToolButton(ControlFrameUI[3][0],"lockicon");
    lockicon->setGeometry( QRect( 600, 55, 18, 26 ) );
  	lockicon->setFixedSize(18, 26);  	//lighticon->setUsesBigPixmap(true);
  	lockicon->setAutoRaise(true);


    ////////////////////////////////////////////////////////
    // raise the control bar and raise backgound as default

    m_pObjectControlStack->raiseWidget(0);

}



void GLEdit::createMenuItem( QPopupMenu * themenu )
{
	Q_CHECK_PTR(themenu);
	themenu->insertItem("&"+m_pJt->tr("Add Object"),this, SLOT(addLayer()) );
    themenu->insertItem("&"+m_pJt->tr("Name Object"),this,SLOT(nameLayer()) );
    themenu->insertItem("&"+m_pJt->tr("Delete Object"),this,SLOT(delLayer()) );
    themenu->insertSeparator();
	//make this a popup 'Move'
    themenu->insertItem("&"+m_pJt->tr("Move Up"),this, SLOT(moveLayerUp()) );
    themenu->insertItem("&"+m_pJt->tr("Move Down"),this, SLOT(moveLayerDown()) ); 
}










