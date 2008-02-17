/*******************************************************************************
**
** The source file for the Jahshaka animation interface module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "effect.h"
#include <InputLCD.h>
#include <supergrangecontrol.h>
#include <jahformatter.h>
#include <dialogs.h>
#include <openmedialib.h>
#include <jahtranslate.h>
#include <valueFormatters.h>

#include <qlistview.h>
#include <qframe.h>
#include <qhbox.h>
#include <qslider.h>
#include <qlayout.h>
#include <widget.h>

GLfloat GLEffect::setZoom( GLfloat )
{
	return GLfloat( zoom->value( ) + 1 ) / 1000.0;
}

void GLEffect::buildInterfaceMenus( QHBox*, QHBox* rightcontroller) 
{

	// Container widget
	QWidget *container = new QWidget( rightcontroller, "container" );
	QVBoxLayout *container_layout = new QVBoxLayout( container, 0, 0, "container_layout");

	container_layout->addStretch( 0 );

    //rightmenu here
	QString image = m_qsJahBasePath+"Pixmaps/desktop/zoomin.png";
    toolzoomout = new JahToolButton( container, "zoomout" );
    JahFormatter::addJahPlayerButton( toolzoomout, image, image );
	toolzoomout->setAutoRepeat( true );
	container_layout->addWidget( toolzoomout );

	container_layout->addSpacing( 3 );

    zoom = new QSlider( container, "scrubslider" );
    zoom->setOrientation( QSlider::Vertical );
    zoom->setMinValue( 1 );    
    zoom->setMaxValue( 5000 );   
    zoom->setValue   ( 1000 );
    zoom->setPageStep( 10 );
	container_layout->addWidget( zoom, 0, Qt::AlignHCenter );

	container_layout->addSpacing( 3 );

    toolzoomin = new JahToolButton( container, "zoomin" );
	image = m_qsJahBasePath+"Pixmaps/desktop/zoomout.png";
    JahFormatter::addJahPlayerButton( toolzoomin, image, image );
	toolzoomin->setAutoRepeat( true );
	container_layout->addWidget( toolzoomin );

	container_layout->addSpacing( 10 );

    //moving ui elements after zoom
    tooltranslate = new JahToolButton( container, "translate" );
    JahFormatter::addJahPlayerButton( tooltranslate, m_qsJahBasePath+"Pixmaps/desktop/transtool.png", m_qsJahBasePath+"Pixmaps/desktop/transtool.png" );
	container_layout->addWidget( tooltranslate );
    
    toolrotate =   new JahToolButton( container, "rotate" );
    JahFormatter::addJahPlayerButton( toolrotate, m_qsJahBasePath+"Pixmaps/desktop/rotatetool.png", m_qsJahBasePath+"Pixmaps/desktop/rotatetool.png" );
	container_layout->addWidget( toolrotate );
    
    toolscale =   new JahToolButton( container, "scale" );
    JahFormatter::addJahPlayerButton( toolscale, m_qsJahBasePath+"Pixmaps/desktop/scaletool.png", m_qsJahBasePath+"Pixmaps/desktop/scaletool.png" );
	container_layout->addWidget( toolscale );
    
	container_layout->addSpacing( 10 );

    m_pScrubRender = new JahToolButton( container, "controllerrewindbutton" );
    JahFormatter::addJahPlayerButton( m_pScrubRender, m_qsJahBasePath+"Pixmaps/desktop/phototool.png", m_qsJahBasePath+"Pixmaps/desktop/phototool.png" );
    connect( m_pScrubRender,  SIGNAL(clicked()), SLOT(Render())  );
	container_layout->addWidget( m_pScrubRender );
    
    m_pScrubRenderAll = new JahToolButton( container, "controllerpreviousbutton" );
    JahFormatter::addJahPlayerButton( m_pScrubRenderAll, m_qsJahBasePath+"Pixmaps/desktop/rendertool.png", m_qsJahBasePath+"Pixmaps/desktop/rendertool.png" );
    connect( m_pScrubRenderAll,  SIGNAL(clicked()), SLOT( RenderScene() )  );
	container_layout->addWidget( m_pScrubRenderAll );


    //connect tools to object and add object to display widget
    connect( tooltranslate,  SIGNAL(clicked()), this, SLOT(toolTranslate() )  );
    connect( toolscale,      SIGNAL(clicked()), this, SLOT(toolScale() )  );
    connect( toolrotate,     SIGNAL(clicked()), this, SLOT(toolRotate() )  );

    connect( toolzoomin,    SIGNAL(clicked()), this, SLOT(resetZoom() )  );
    connect( toolzoomout,    SIGNAL(clicked()), this, SLOT(resetZoom() )  );
    connect( zoom,  SIGNAL(valueChanged(int)), this, SLOT(changeZoom(int) )  );

	container_layout->addStretch( );
}

void GLEffect::buildInterface( QHBox* f ) 
{
    JahPrefs& jprefs = JahPrefs::getInstance();

    QWidget* placeholder = new QWidget( f );
    QHBoxLayout* mainLayout = new QHBoxLayout( placeholder );
    // JahFormatter::setMarginAndSpacing( mainLayout );

    ///////////////////////////////////////////////////////////
    //The layers interface

    QBoxLayout* layersLayout = new QVBoxLayout();
    mainLayout->addLayout( layersLayout );
    JahFormatter::setMarginAndSpacingSmall( layersLayout );

    ///////////////////////////////////////////////////////////
    //The layer options
    //these are defined in the world object
    m_pAddButton = new QPushButton( placeholder, "m_pAddButton" );
    m_pAddButton->setText( m_pJt->tr("Add CPU") );
    layersLayout->addWidget( m_pAddButton );
    connect  ( m_pAddButton,  SIGNAL(clicked()), this, SLOT( addCpuEffect())  );

    AddRTButton = new QPushButton( placeholder, "AddRTButton" );
    AddRTButton->setText( m_pJt->tr("Add RT") );
    layersLayout->addWidget( AddRTButton );
    connect  ( AddRTButton,  SIGNAL(clicked()), this, SLOT( addMeshEffect() )  );

    AddGpuButton = new QPushButton( placeholder, "AddGpuButton" );
    AddGpuButton->setText( m_pJt->tr("Add GPU") );
    layersLayout->addWidget( AddGpuButton );
    connect  ( AddGpuButton,  SIGNAL(clicked()), this, SLOT( addGpuEffect() )  );

    JahFormatter::addSpacingSmall( layersLayout );

    m_pNameButton = new QPushButton( placeholder, "m_pNameButton" );
    m_pNameButton->setText( m_pJt->tr("Name") );
    layersLayout->addWidget( m_pNameButton );
    connect  ( m_pNameButton,  SIGNAL(clicked()), this, SLOT( nameLayer())  );

    m_pNamePopUp = new FancyPopup( this, m_pJt->tr("Enter Layer Name"), 250, 150 );  //send size and position as well
    connect ( m_pNamePopUp,      SIGNAL( returnText(QString)), SLOT( setlayerName(QString)) );

    m_pDelButton = new QPushButton( placeholder, "Delete Button" );
    m_pDelButton->setText( m_pJt->tr("Del") );
    layersLayout->addWidget( m_pDelButton );
    connect  ( m_pDelButton,  SIGNAL(clicked()), this, SLOT( delLayer())  );

    JahFormatter::addSpacingSmall( layersLayout );

    m_pMoveUpButton = new QPushButton( placeholder, "Move Layer Up Button" );
    m_pMoveUpButton->setText( m_pJt->tr("Up") );
    layersLayout->addWidget( m_pMoveUpButton );
    connect  ( m_pMoveUpButton,  SIGNAL(clicked()), this, SLOT( moveLayerUp())  );

    m_pMoveDownButton = new QPushButton( placeholder, "Move Layer Down Button" );
    m_pMoveDownButton->setText( m_pJt->tr("Down") );
    layersLayout->addWidget( m_pMoveDownButton );
    connect  ( m_pMoveDownButton,  SIGNAL(clicked()), this, SLOT( moveLayerDown())  );

    layersLayout->addStretch();

    //////////////////////////////////////////////////////////////
    // set up the scene options, or prefernces

    EffectsFrame = new QFrame( placeholder );
    QBoxLayout* EffectsFrameLayout = new QHBoxLayout( EffectsFrame );
    JahFormatter::setSpacing( EffectsFrameLayout );
    mainLayout->addWidget( EffectsFrame );

    JahFormatter::addSpacingSmall( EffectsFrameLayout );

    /////////////////////////////////////////////////////////
    //this is the list-view
    m_pLayerListView = new QListView( EffectsFrame );
    m_pLayerListView->setRootIsDecorated(true);

    connect( m_pLayerListView, SIGNAL( clicked( QListViewItem* ) ), this, SLOT( layerClicked( QListViewItem* ) ) );

    m_pLayerListView->addColumn( (m_pJt->tr("EFFECTS")), -1 );
    m_pLayerListView->setSorting( -1,1 );   // disables the autosort
    //effects currently only works in single m_bSelection mode
    m_pLayerListView->setSelectionMode (QListView::Single );

    JahFormatter::setListViewAsSingleColumn( m_pLayerListView );

    EffectsFrameLayout->addWidget( m_pLayerListView );

    /////////////////////////////////////////////////////////
    //Set up tabbed interface

    tabframe = new QTabWidget( EffectsFrame, "axiscontrols" );	
    EffectsFrameLayout->addWidget( tabframe );

    tabframe->setTabShape( QTabWidget::Triangular );
    tabframe->setAutoMask( FALSE );

    //this has become the settings panel need to update variable names
    moduleOptionsUI[0] = new QHBox( EffectsFrame, "encoderUI0" ); //scenecontrols
    tabframe->insertTab( moduleOptionsUI[0], tr( "  "+m_pJt->tr("SCENE")+"  " ) );

    moduleOptionsUI[1] = new QHBox( EffectsFrame, "axiscontrols" ); //axiscontrols
    tabframe->insertTab( moduleOptionsUI[1], tr( "  "+m_pJt->tr("CONTROLS")+"  " ) );

    moduleOptionsUI[2] = new QHBox( EffectsFrame, "texturecontrols" ); //texturecontrols
    tabframe->insertTab( moduleOptionsUI[2], tr( "    "+m_pJt->tr("CLIP")+"    " ) );

    //this has become the settings panel need to update variable names
    moduleOptionsUI[3] = new QHBox( EffectsFrame, "keysettings" ); //keycontrols
    tabframe->insertTab( moduleOptionsUI[3], tr( "  "+m_pJt->tr("KEYFRAMES")+"  " ) );

    /////////////////////////////////////////////////////////////////////
    // the layout boxes for different types of layers
    // the object control tab bar
    // this is the object control panel

    //QPalette p( QColor( 0, 0, 0 ) );
    //p.setColor( QPalette::Active, QColorGroup::Foreground, Qt::green );

    ///////////////////////////////////////////////////////////////////
    // the scene control tab bar

    //mover this into the layers...
    ControlFrameUI[0][0] = new QFrame( moduleOptionsUI[0], "ControlFrame3" ); //scenecontrols
    QBoxLayout* outer = new QVBoxLayout( ControlFrameUI[0][0] );
    QBoxLayout* top = new QHBoxLayout();
    JahFormatter::setMarginAndSpacing( top );
    outer->addLayout( top );

    QBoxLayout* buttons = new QVBoxLayout();
    buttons->setResizeMode( QLayout::Fixed );
    top->addLayout( buttons );

    LoadButton = new QPushButton( ControlFrameUI[0][0], "LoadButton" );
    LoadButton->setText( m_pJt->tr("Load") );
    buttons->addWidget( LoadButton );
    connect  ( LoadButton,  SIGNAL(clicked()), this, SLOT( SceneLoad())  );

    SaveButton = new QPushButton( ControlFrameUI[0][0], "SaveButton" );
    SaveButton->setText( m_pJt->tr("Save") );
    buttons->addWidget( SaveButton );
    connect  ( SaveButton,  SIGNAL(clicked()), this, SLOT( sceneSave())  );

    ResetButton = new QPushButton( ControlFrameUI[0][0], "ResetButton" );
    ResetButton->setText( m_pJt->tr("Reset All") );
    buttons->addWidget( ResetButton );
    connect  ( ResetButton,  SIGNAL(clicked()), this, SLOT( ResetAll())  );

    ClearButton = new QPushButton( ControlFrameUI[0][0], "ClearButton" );
    ClearButton->setText( m_pJt->tr("Clear Scene") );
    buttons->addWidget( ClearButton );
    connect( ClearButton,  SIGNAL(clicked()), this, SLOT( ClearAll())  );

    QBoxLayout* col2 = new QVBoxLayout();
    top->addLayout( col2 );

    QBoxLayout* checkAndFrameLayout = new QHBoxLayout();
    col2->addLayout( checkAndFrameLayout );

    QBoxLayout* checkLayout = new QVBoxLayout();
    checkAndFrameLayout->addLayout( checkLayout );

    VideoRenderSelect = new QCheckBox( ControlFrameUI[0][0], "SmoothSelect" );
    VideoRenderSelect->setText( m_pJt->tr("Render using source res") );
    checkLayout->addWidget( VideoRenderSelect );
    connect( VideoRenderSelect,  SIGNAL(clicked()), this, SLOT(changeVideoRender())  );

    m_render_only_to_selected_effect_qcheckbox = new QCheckBox( ControlFrameUI[0][0], "SmoothSelect" );
    m_render_only_to_selected_effect_qcheckbox->setText( m_pJt->tr("Draw only to m_bSelected effect") );
    checkLayout->addWidget( m_render_only_to_selected_effect_qcheckbox );
    connect( m_render_only_to_selected_effect_qcheckbox,  SIGNAL(clicked()), this, SLOT(changeRenderOnlyToSelectedEffect())  );

    QGridLayout* lcdLayout = new QGridLayout( 2, 2 );
    checkAndFrameLayout->addLayout( lcdLayout );
    
    QLabel* sframetext = new QLabel( ControlFrameUI[0][0], "sframetext" );
    lcdLayout->addWidget( sframetext, 0, 0, Qt::AlignVCenter );
    sframetext->setText( tr( m_pJt->tr("STARTFRAME") ) );
    sframetext->setAlignment( int( QLabel::AlignRight ) );
    
    m_pStartFrameControl = new SupergRangeControl( ControlFrameUI[0][0], "startframe" );
    m_pStartFrameControl->setFormatter( TimecodeValueFormatter() );
    lcdLayout->addWidget( m_pStartFrameControl, 0, 1 );
    JahFormatter::configure( m_pStartFrameControl, m_nAStartFrame, m_nAStartFrame );
    connect( m_pStartFrameControl, SIGNAL(valueChanged(int)),   SLOT(updatesliderStartframe(int)) );
    
    QLabel* eframetext = new QLabel( ControlFrameUI[0][0], "eframetext" );
    lcdLayout->addWidget( eframetext, 1, 0, Qt::AlignVCenter );
    eframetext->setText( tr( m_pJt->tr("ENDFRAME")  ) );
    eframetext->setAlignment( int( QLabel::AlignRight ) );
    
    m_pEndFrameControl = new SupergRangeControl( ControlFrameUI[0][0], "endframe" );
    m_pEndFrameControl->setFormatter( TimecodeValueFormatter() );
    lcdLayout->addWidget( m_pEndFrameControl, 1, 1 );
    JahFormatter::configure( m_pEndFrameControl, m_nAEndFrame, m_nAStartFrame );
    connect( m_pEndFrameControl, SIGNAL(valueChanged(int)),   SLOT(updatesliderEndframe(int)) );

    QBoxLayout* checkAndResLayout = new QHBoxLayout();
    col2->addLayout( checkAndResLayout );

    mesh1 = new QCheckBox( ControlFrameUI[0][0], "light1" );
    mesh1->setText( m_pJt->tr("toggle mesh") );
    checkAndResLayout->addWidget( mesh1 );
    connect( mesh1,  SIGNAL(clicked()), this, SLOT(toggleMesh())  );

    m_pShowLightsCheckBox = new QCheckBox( ControlFrameUI[0][0], "m_pShowLightsCheckBox" );
    m_pShowLightsCheckBox->setText( m_pJt->tr("Auto Key") );
    checkAndResLayout->addWidget( m_pShowLightsCheckBox );

    QGridLayout* g = new QGridLayout( 2, 2 );
    checkAndResLayout->addLayout( g );

    m_pSelectFPSLabel = new QLabel( ControlFrameUI[0][0], "FPSPushButtonLabel" );
    g->addWidget( m_pSelectFPSLabel, 0, 0 );
    m_pSelectFPSLabel->setAlignment( int( QLabel::AlignCenter ) );
    m_pSelectFPSLabel->setText( m_pJt->tr( "Playback Speed" ) );

    m_pSelectFPSPushbutton = new QPushButton( ControlFrameUI[0][0], "FPSPushButton" );
    g->addWidget( m_pSelectFPSPushbutton, 1, 0 );
    QString text_string = getFramesPerSecondName(FRAMES_PER_SECOND_30).data();
    m_pSelectFPSPushbutton->setText(text_string);
    connect(m_pSelectFPSPushbutton, SIGNAL( pressed() ), SLOT( slotChooseFramesPerSecond() ) );

    m_pJahResLabel = new QLabel( ControlFrameUI[0][0], "languagelabel" );
    g->addWidget( m_pJahResLabel, 0, 1 );
    m_pJahResLabel->setAlignment( int( QLabel::AlignCenter ) );
    m_pJahResLabel->setText( m_pJt->tr( "Resolution" ) );

    m_pJahResCombo = new QComboBox( FALSE, ControlFrameUI[0][0], "m_pJahResCombo" );
    g->addWidget( m_pJahResCombo, 1, 1 );
    m_bHasResMenu = true;
    projectData thedata;
    thedata.buildComboBox(m_pJahResCombo);
    int defaultRes = jprefs.getJahResolution();
    m_pJahResCombo->setCurrentItem(defaultRes); //should be set to appdefault
    connect( m_pJahResCombo,  SIGNAL(activated(int)), this, SLOT(setResolution(int))  );

    top->addStretch();
    outer->addStretch();
	

    ///////////////////////////////////////////////////////////////
    // object layer options sub control set

    ControlFrameUI[1][0] = new QFrame( moduleOptionsUI[1], "ControlFrame1" ); //ControlFrame1
    ControlFrameUI[1][0]->setGeometry( QRect( 0, 0, 660, 190 ) );    // was 510

    createAxisPanel ( ControlFrameUI[1][0] );

    //add the placeholders for plugins
    m_aJahModuleOptions[0] = new QCheckBox( ControlFrameUI[1][0], "Placeholder" );
    JahFormatter::addCheckButton( m_aJahModuleOptions[0], 520, 30, 145, 25, "Placeholder1" );
    connect( m_aJahModuleOptions[0],  SIGNAL(clicked()), this, SLOT(setOption1())  ); //for clip

    //increment the ui uption counter...
    m_nNumUiOptions +=1;

    //add the placeholders for plugins
    m_aJahModuleOptions[1] = new QCheckBox( ControlFrameUI[1][0], "Placeholder" );
    JahFormatter::addCheckButton( m_aJahModuleOptions[1], 520, 60, 145, 25, "Placeholder2" );
    connect( m_aJahModuleOptions[1],  SIGNAL(clicked()), this, SLOT(setOption2())  ); //for clip

    //increment the ui uption counter...
    m_nNumUiOptions +=1;

    //add the placeholders for plugins
    m_aJahModuleOptions[2] = new QCheckBox( ControlFrameUI[1][0], "Placeholder" );
    JahFormatter::addCheckButton( m_aJahModuleOptions[2], 520, 90, 145, 25, "Placeholder3" );
    connect( m_aJahModuleOptions[2],  SIGNAL(clicked()), this, SLOT(setOption3())  ); //for clip

    //increment the ui uption counter...
    m_nNumUiOptions +=1;

    //add the placeholders for plugins
    m_aJahModuleOptions[3] = new QCheckBox( ControlFrameUI[1][0], "Placeholder" );
    JahFormatter::addCheckButton( m_aJahModuleOptions[3], 520, 120, 145, 25, "Placeholder4" );
    connect( m_aJahModuleOptions[3],  SIGNAL(clicked()), this, SLOT(setOption4())  ); //for clip

    //increment the ui uption counter...
    m_nNumUiOptions +=1;

    m_bHasButtons = true;


    ///////////////////////////////////////////////////////////////
    // texture layer options
    // need to add video-texture options here
    //image options

    ControlFrameUI[2][0] = new QFrame( moduleOptionsUI[2], "ControlFrame2" ); //was ControlFrame2
    ControlFrameUI[2][0]->setGeometry( QRect( 0, 0, 660, 190 ) );

    ClipSelect = new QCheckBox( ControlFrameUI[2][0], "ClipSelect" );
    JahFormatter::addCheckButton( ClipSelect, 0, 30, 45, 25, m_pJt->tr("Clip") );
    connect( ClipSelect,  SIGNAL(clicked()), this, SLOT( setClipStatus() )  ); //for clip
    
    grabdesktopClip = new QPushButton( ControlFrameUI[2][0], "grabdesktop" );
    JahFormatter::addButton( grabdesktopClip, 50, 30, 110, 25, m_pJt->tr("Grab Desktop") );
    connect( grabdesktopClip,  SIGNAL(clicked()), this, SLOT( grabDesktop() )  );
    
    keyimage_2 = new QPushButton( ControlFrameUI[2][0], "clipmapping" );
    JahFormatter::addButton( keyimage_2, 165, 30, 35, 25, "IM" );
    connect( keyimage_2,  SIGNAL(clicked()), this, SLOT(keyCliplayer())  );

    m_pUseOpenGLKeyLayerCheckBox = new QCheckBox( ControlFrameUI[2][0], "KeySelect" );
    JahFormatter::addCheckButton( m_pUseOpenGLKeyLayerCheckBox, 220, 30, 120, 25, m_pJt->tr("OpenGL Key") );
    connect( m_pUseOpenGLKeyLayerCheckBox,  SIGNAL(clicked()), this, SLOT( slotSetUseOpenGlKeyer() )  );
    
    m_pUseFastShaderKeyLayerCheckBox = new QCheckBox( ControlFrameUI[2][0], "m_pUseFastShaderKeyLayerCheckBox" );
    JahFormatter::addCheckButton( m_pUseFastShaderKeyLayerCheckBox, 220, 52, 120, 25, m_pJt->tr("Fast Shader Key") );
    connect( m_pUseFastShaderKeyLayerCheckBox,  SIGNAL(clicked()), this, SLOT( slotSetUseFastShaderKeyer() )  );

    KeySelect = new QCheckBox( ControlFrameUI[2][0], "KeySelect" );
    JahFormatter::addCheckButton( KeySelect, 0, 75, 45, 25, m_pJt->tr("Key") );
    connect( KeySelect,  SIGNAL(clicked()), this, SLOT( setKeyStatus() )  );
    
    grabdesktopKey = new QPushButton( ControlFrameUI[2][0], "grabdesktop" );
    JahFormatter::addButton( grabdesktopKey, 50, 75, 110, 25, m_pJt->tr("Grab Desktop") );
    connect( grabdesktopKey,  SIGNAL(clicked()), this, SLOT( grabDesktopKey() )  ); //for key...
    
    keyimage_3 = new QPushButton( ControlFrameUI[2][0], "keyOne_3" );
    JahFormatter::addButton( keyimage_3, 165, 75, 35, 25, "KM" );
    connect( keyimage_3,  SIGNAL(clicked()), this, SLOT(keyClipkey())  );
    
    keyStatusBox = new QPushButton( ControlFrameUI[2][0], "Select1" );
    JahFormatter::addButton( keyStatusBox, 220, 75, 35, 25, "Invert" );
    connect( keyStatusBox,  SIGNAL(clicked()), this, SLOT( InvertKeyData() )  );
    
    // video options
    // need to hook this up now...
    
    VideoLabel = new QLabel( ControlFrameUI[2][0], "TranslateText_1" );
    JahFormatter::addLabel( VideoLabel, 380, 10, 120, 21, "VIDEO OPTIONS", 9);
    
    
    m_pSlipFramesLCD = new InputLCD( ControlFrameUI[2][0] );
    JahFormatter::addLabelLcd( ControlFrameUI[2][0], m_pSlipFramesLCD, 380, 30, 60, 21,  0,  "Slip", 8);
    connect(m_pSlipFramesLCD, SIGNAL(valueChanged(int)), this, SLOT( slotSetSlipFrames(int) ) );
    // These are necessary because of some Qt problem
    m_pSlipFramesLCD->setValue(1);
    m_pSlipFramesLCD->setValue(0);
    
    m_pInFramesLCD = new InputLCD( ControlFrameUI[2][0] );
    JahFormatter::addLcdLabel( ControlFrameUI[2][0], m_pInFramesLCD,     460, 30, 30, 21,  0,  "In", 8);
    m_pInFramesLCD->setMinInt(1);
    connect(m_pInFramesLCD, SIGNAL(valueChanged(int)), this, SLOT( setinFrames(int) ) );
    
    m_pOutFramesLCD = new InputLCD( ControlFrameUI[2][0] );
    JahFormatter::addLcdLabel( ControlFrameUI[2][0], m_pOutFramesLCD,    530, 30, 30, 21,  0,  "Out", 8);
    m_pOutFramesLCD->setMinInt(1);
    connect(m_pOutFramesLCD, SIGNAL(valueChanged(int)), this, SLOT( setoutFrames(int) ) );
    
    m_pKeySlipFramesLCD = new InputLCD( ControlFrameUI[2][0] );
    JahFormatter::addLabelLcd( ControlFrameUI[2][0], m_pKeySlipFramesLCD,    380, 75, 60, 21,  0,  "Slip", 8);
    connect(m_pKeySlipFramesLCD, SIGNAL(valueChanged(int)), this, SLOT( slotSetKeySlipFrames(int) ) );
    // These are necessary because of some Qt problem
    m_pKeySlipFramesLCD->setValue(1);
    m_pKeySlipFramesLCD->setValue(0);
    
    m_pKeyInFramesLCD = new InputLCD( ControlFrameUI[2][0] );
    JahFormatter::addLcdLabel( ControlFrameUI[2][0], m_pKeyInFramesLCD,  460, 75, 30, 21,  0,  "In", 8);
    m_pKeyInFramesLCD->setMinInt(1);
    connect(m_pKeyInFramesLCD, SIGNAL(valueChanged(int)), this, SLOT( setinKeyFrames(int) ) );
    
    m_pKeyOutFramesLCD = new InputLCD( ControlFrameUI[2][0] );
    JahFormatter::addLcdLabel( ControlFrameUI[2][0], m_pKeyOutFramesLCD, 530, 75, 30, 21,  0,  "Out", 8);
    m_pKeyOutFramesLCD->setMinInt(1);
    connect(m_pKeyOutFramesLCD, SIGNAL(valueChanged(int)), this, SLOT( setoutKeyFrames(int) ) );
    
    extendHeadTail = new QCheckBox( ControlFrameUI[2][0], "extendHeadTail" );
    JahFormatter::addCheckButton( extendHeadTail, 380, 95, 150, 25, "Extend Head-Tail" );
    connect( extendHeadTail,  SIGNAL(clicked()), this, SLOT(toggleExtendHeadTail())  );

    m_pLockKeyAndClipCheckBox = new QCheckBox( ControlFrameUI[2][0], "m_pLockKeyAndClipCheckBox" );
    JahFormatter::addCheckButton( m_pLockKeyAndClipCheckBox, 380, 130, 100, 25, "Lock Key" );
    connect( m_pLockKeyAndClipCheckBox,  SIGNAL(clicked()), this, SLOT( slotUpdateAnimation() )  );
    
    toggleTheLoop = new QCheckBox( ControlFrameUI[2][0], "toggleTheLoop" );
    JahFormatter::addCheckButton( toggleTheLoop, 510, 95, 100, 25, "Loop" );
    connect( toggleTheLoop,  SIGNAL(clicked()), this, SLOT( toggleLoop() )  );
    
    togglePing = new QCheckBox( ControlFrameUI[2][0], "togglePing" );
    JahFormatter::addCheckButton( togglePing, 510, 130, 100, 25, "Ping Pong" );
    connect( togglePing,  SIGNAL(clicked()), this, SLOT( togglePingPong() )  );
    
   
    FindMediaLabel = new QLabel( ControlFrameUI[2][0], "FindMediaLabel" );
    JahFormatter::addLabel( FindMediaLabel, 0, 110, 110, 21, "Media Location", 9);
    
    FindMediaText	= new QLineEdit( ControlFrameUI[2][0], "FindMediaText" );
    FindMediaText->setGeometry( QRect( 2, 135, 250, 21 ) );
    //FindMediaText->setText( "FMT FreeMediaTools" );
    
    FindMediaButton = new QPushButton( ControlFrameUI[2][0], "find" );
    JahFormatter::addButton( FindMediaButton, 255, 135, 55, 21, m_pJt->tr("locate") );
    connect( FindMediaButton,  SIGNAL(clicked()), this, SLOT( locateMissingMedia() )  );
    

	////////////////////////////////////////////////////////
	// give this man a keyframer!
	addKeyframer(moduleOptionsUI[3]);

}

void GLEffect::createMenuItem( QPopupMenu * themenu )
{
	Q_CHECK_PTR(themenu);
    themenu->insertItem("&"+m_pJt->tr("CPU Effect"),this,SLOT( addCpuEffectFromTopMenu() ) );
    themenu->insertItem("&"+m_pJt->tr("Mesh Effect"),this,SLOT( addMeshEffectFromTopMenu() ) );
    themenu->insertItem("&"+m_pJt->tr("GPU Effect"),this,SLOT( addGpuEffectFromTopMenu() ) );
    themenu->insertSeparator();
    themenu->insertItem("&"+m_pJt->tr("Delete Effect"),this,SLOT( delLayer() ) );
    themenu->insertSeparator();
    themenu->insertItem("&"+m_pJt->tr("Move Effect Up"),this, SLOT( moveLayerUp() ) );
    themenu->insertItem("&"+m_pJt->tr("Move Effect Down"),this, SLOT( moveLayerDown() ) ); 
}























