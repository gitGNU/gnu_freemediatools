/*******************************************************************************
 **
 ** The source file for the Jahshaka animation interface module
 ** The Jahshaka Project
 ** Copyright (C) 2000-2006 VM Inc.
 ** Released under the GNU General Public License
 **
 *******************************************************************************/

#include "character.h"
#include "anime.h"
#include <qframe.h>
#include <qmultilineedit.h>
#include <qpopupmenu.h>
#include <qhbox.h>
#include <qlayout.h>
#include <qwidgetstack.h>
#include <qslider.h>
#include <qcheckbox.h>
#include <qstylefactory.h>
#include <InputLCD.h>
#include <supergrangecontrol.h>
#include <jahformatter.h>
#include <valueFormatters.h>
#include <dialogs.h>
#include <qlistview.h>
#include <openmedialib.h>
#include <jahtranslate.h>
#include <widget.h>

static const unsigned int LAYERS_TAB = 1;

void 
GLChar::buildInterface( QHBox* f ) 
{
    JahPrefs& jprefs = JahPrefs::getInstance();
    
    QWidget* placeholder = new QWidget( f );
    QHBoxLayout* mainLayout = new QHBoxLayout( placeholder );
    JahFormatter::setMarginAndSpacingSmall( mainLayout );
    
    ///////////////////////////////////////////////////////////
    //The layers interface
    LayersFrame = new QFrame( placeholder, "EffectsFrame" );
    mainLayout->addWidget( LayersFrame );
    
    QBoxLayout* LayersFrameLayout = new QVBoxLayout( LayersFrame );
    
    ///////////////////////////////////////////////////////////
    //The layer options
    //these are defined in the world object
    
    m_pAddButton = new QPushButton( LayersFrame, "m_pAddButton" );
    m_pAddButton->setText( m_pJt->tr("AddLayer") );
    LayersFrameLayout->addWidget( m_pAddButton );
    connect  ( m_pAddButton,  SIGNAL(clicked()), this, SLOT( addLayer())  );
    
    m_pNameButton = new QPushButton( LayersFrame, "m_pNameButton" );
    m_pNameButton->setText( m_pJt->tr("Name") );
    LayersFrameLayout->addWidget( m_pNameButton );
    connect  ( m_pNameButton,  SIGNAL(clicked()), this, SLOT( nameLayer())  );
    
    m_pNamePopUp = new FancyPopup( this, m_pJt->tr("Enter Layer Name"),250,150 );  //send size and position as well
    connect ( m_pNamePopUp, SIGNAL( returnText(QString)), SLOT( setlayerName(QString)) );
    
    m_pDelButton = new QPushButton( LayersFrame, "Delete Button" );
    m_pDelButton->setText( m_pJt->tr("Del") );
    LayersFrameLayout->addWidget( m_pDelButton );
    connect  ( m_pDelButton,  SIGNAL(clicked()), this, SLOT( delLayer())  );
    
    m_pMoveUpButton = new QPushButton( LayersFrame, "Move Layer Up Button" );
    m_pMoveUpButton->setText( m_pJt->tr("Up") );
    LayersFrameLayout->addWidget( m_pMoveUpButton );
    connect  ( m_pMoveUpButton,  SIGNAL(clicked()), this, SLOT( moveLayerUp())  );
    
    m_pMoveDownButton = new QPushButton( LayersFrame, "Move Layer Down Button" );
    m_pMoveDownButton->setText( m_pJt->tr("Down") );
    LayersFrameLayout->addWidget( m_pMoveDownButton );
    connect  ( m_pMoveDownButton,  SIGNAL(clicked()), this, SLOT( moveLayerDown())  );

    LayersFrameLayout->addStretch();
    
    //////////////////////////////////////////////////////////////
    // set up the scene options, or prefernces
    
    EffectsFrame = new QFrame( f );
    mainLayout->addWidget( EffectsFrame );
    QBoxLayout* EffectsFrameLayout = new QHBoxLayout( EffectsFrame );
    JahFormatter::setSpacing( EffectsFrameLayout );
    
    JahFormatter::addSpacingSmall( EffectsFrameLayout );

    /////////////////////////////////////////////////////////
    //this is the list-view
    m_pLayerListView = new QListView( EffectsFrame );
    m_pLayerListView->setRootIsDecorated(true);
    m_pLayerListView->addColumn( (m_pJt->tr("LAYERS")), -1 );
    m_pLayerListView->setSorting( -1,1 );   // disables the autosort
    //allow for multiple selections
    m_pLayerListView->setSelectionMode (QListView::Extended );
    
    connect( m_pLayerListView, SIGNAL( clicked( QListViewItem* ) ), this, SLOT( layerClicked( QListViewItem* ) ) );
    
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
    tabframe->insertTab( moduleOptionsUI[0], tr("  "+m_pJt->tr("SCENE")+"  ") );
    
    moduleOptionsUI[1] = new QHBox( EffectsFrame, "objectcontrols" ); //objectcontrols
    tabframe->insertTab( moduleOptionsUI[1], tr( "  "+m_pJt->tr("LAYERS")+"  " ) );
    
    moduleOptionsUI[2] = new QHBox( EffectsFrame, "axiscontrols" ); //axiscontrols
    tabframe->insertTab( moduleOptionsUI[2], tr( "  "+m_pJt->tr("CONTROLS")+"  " ) );
    
    moduleOptionsUI[3] = new QHBox( EffectsFrame, "texturecontrols" ); //texturecontrols
    tabframe->insertTab( moduleOptionsUI[3], tr( "    "+m_pJt->tr("MEDIA")+"    " ) );
    
    //this has become the settings panel need to update variable names
    moduleOptionsUI[4] = new QHBox( EffectsFrame, "keysettings" ); //keycontrols
    tabframe->insertTab( moduleOptionsUI[4], tr( "  "+m_pJt->tr("KEYFRAMES")+"  " ) );
    
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
    
    ///////////////////////////////////////////////////////////////////
    // the scene control tab bar
    
    {
        //mover this into the layers...
        ControlFrameUI[0][0] = new QFrame( moduleOptionsUI[0], "ControlFrame3" ); //scenecontrols
        QBoxLayout* mainLayout = new QHBoxLayout( ControlFrameUI[0][0] );
        
        QGridLayout* pushButtonsLayout = new QGridLayout( 7, 3 );
        JahFormatter::setMarginAndSpacing( pushButtonsLayout );
        pushButtonsLayout->setResizeMode( QLayout::Fixed );
        for ( int i=0; i<6; ++i ) pushButtonsLayout->setRowStretch( i, 0 );
        pushButtonsLayout->setRowStretch( 6, 1 );
        pushButtonsLayout->setColStretch( 0, 0 );
        pushButtonsLayout->setColStretch( 1, 0 );
        pushButtonsLayout->setColStretch( 2, 1 );
        mainLayout->addLayout( pushButtonsLayout );
        
        LoadButton = new QPushButton( ControlFrameUI[0][0], "LoadButton" );
        pushButtonsLayout->addWidget( LoadButton, 0, 0 );
        LoadButton->setText( m_pJt->tr("Load") );
        connect  ( LoadButton,  SIGNAL(clicked()), this, SLOT( SceneLoad())  );
        
        AppendButton = new QPushButton( ControlFrameUI[0][0], "AppendButton" );
        pushButtonsLayout->addWidget( AppendButton, 1, 0 );
        AppendButton->setText( m_pJt->tr("Append") );
        connect  ( AppendButton,  SIGNAL(clicked()), this, SLOT( SceneAppend())  );
        
        SaveButton = new QPushButton( ControlFrameUI[0][0], "SaveButton" );
        pushButtonsLayout->addWidget( SaveButton, 2, 0 );
        SaveButton->setText( m_pJt->tr("Save") );
        connect  ( SaveButton,  SIGNAL(clicked()), this, SLOT( sceneSave())  );
        
        ResetButton = new QPushButton( ControlFrameUI[0][0], "ResetButton" );
        pushButtonsLayout->addWidget( ResetButton, 3, 0 );
        ResetButton->setText( m_pJt->tr("Reset All") );
        connect  ( ResetButton,  SIGNAL(clicked()), this, SLOT( ResetAll())  );
        
        ClearButton = new QPushButton( ControlFrameUI[0][0], "ClearButton" );
        pushButtonsLayout->addWidget( ClearButton, 4, 0 );
        ClearButton->setText( m_pJt->tr("Clear Scene") );
        connect( ClearButton,  SIGNAL(clicked()), this, SLOT( ClearAll())  );
        
        PackageButton = new QPushButton( ControlFrameUI[0][0], "ExportSceneButton" );
        pushButtonsLayout->addWidget( PackageButton, 0, 1 );
        PackageButton->setText( m_pJt->tr("Export Scene") );
        connect( PackageButton,  SIGNAL(clicked()), this, SLOT( PackageSave())  );
        
        m_pRunScriptButton = new QPushButton( ControlFrameUI[0][0], "RunScript" );
        pushButtonsLayout->addWidget( m_pRunScriptButton, 1, 1 );
        m_pRunScriptButton->setText( m_pJt->tr("Run Script") );
        connect( m_pRunScriptButton,  SIGNAL(clicked()), this, SLOT( getScript() )  );
        
        //m_save_as_export_button = new QPushButton( ControlFrameUI[0][0], "SaveAsExport" );
        //JahFormatter::addButton( m_save_as_export_button, 120, 70, 105, 25, m_pJt->tr("Save as Export") );
        //connect( m_save_as_export_button,  SIGNAL(clicked()), this, SLOT( saveAsExport() )  );
        
        ImportFxButton = new QPushButton( ControlFrameUI[0][0], "ImportFXButton" );
        pushButtonsLayout->addWidget( ImportFxButton, 3, 1 );
        ImportFxButton->setText( m_pJt->tr("Import FX") );
        connect( ImportFxButton,  SIGNAL(clicked()), this, SLOT( importFx())  );
        
        ExportFxButton = new QPushButton( ControlFrameUI[0][0], "ExportFXButton" );
        pushButtonsLayout->addWidget( ExportFxButton, 4, 1 );
        ExportFxButton->setText( m_pJt->tr("Export FX") );
        connect( ExportFxButton,  SIGNAL(clicked()), this, SLOT( saveEffects())  );
        
        //need to move this into the module
        //this is mandatory for all modules...
        
        // second column
        QBoxLayout* l = new QVBoxLayout();
        JahFormatter::setMarginAndSpacing( l );
        mainLayout->addLayout( l );
        
        // group stats and lcd together
        QBoxLayout* statsAndLcdLayout = new QHBoxLayout();
        l->addLayout( statsAndLcdLayout );
        
        //add the stats toggle buttons
        QBoxLayout* statsLayout = new QVBoxLayout();
        statsAndLcdLayout->addLayout( statsLayout );
        
        ShowStats = new QCheckBox( ControlFrameUI[0][0], "ShowStats" );
        statsLayout->addWidget( ShowStats );
        ShowStats->setText( m_pJt->tr("show stats") );
        connect( ShowStats,  SIGNAL(clicked()), this, SLOT(toggleStatsDisplay())  );
        
        m_pForcedPlay = new QCheckBox( ControlFrameUI[0][0], "forceplaycheckbox" );
        statsLayout->addWidget( m_pForcedPlay );
        m_pForcedPlay->setText( m_pJt->tr("ForcePlay") );
        connect( m_pForcedPlay,  SIGNAL(clicked()), this, SLOT(toggleForcePlay())  );
        
        m_pShowLightsCheckBox = new QCheckBox( ControlFrameUI[0][0], "showlights" );
        statsLayout->addWidget( m_pShowLightsCheckBox );
        m_pShowLightsCheckBox->setText( "Show Lights" );
        m_pShowLightsCheckBox->setChecked(true);
        connect( m_pShowLightsCheckBox,  SIGNAL(clicked()), this, SLOT( slotSetShowLights() )  );
        
        m_pUseAutoKeyCheckBox = 
	  new QCheckBox( ControlFrameUI[0][0], "m_pUseAutoKeyCheckBox;" );

        statsLayout->addWidget( m_pUseAutoKeyCheckBox );
        m_pUseAutoKeyCheckBox->setText( "Auto Key" );
        m_pUseAutoKeyCheckBox->setChecked(false);

        // lcds
        QGridLayout* lcdLayout = new QGridLayout( 2, 2 );
        statsAndLcdLayout->addLayout( lcdLayout );
        
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
        
        l->addStretch();
        
        // other buttons
        QGridLayout* otherLayout = new QGridLayout( 2, 2 );
        otherLayout->setResizeMode( QLayout::Fixed );
        l->addLayout( otherLayout );
        
        m_pSelectFPSLabel = new QLabel( ControlFrameUI[0][0], "FPSPushButtonLabel" );
        otherLayout->addWidget( m_pSelectFPSLabel, 0, 0 );
        m_pSelectFPSLabel->setAlignment( int( QLabel::AlignCenter ) );
        m_pSelectFPSLabel->setText( m_pJt->tr( "Playback Speed" ) );
        
        m_pSelectFPSPushbutton = new QPushButton( ControlFrameUI[0][0], "FPSPushButton" );
        otherLayout->addWidget( m_pSelectFPSPushbutton, 1, 0 );
        QString text_string = getFramesPerSecondName(FRAMES_PER_SECOND_30).data();
        m_pSelectFPSPushbutton->setText(text_string);
        connect(m_pSelectFPSPushbutton, SIGNAL( pressed() ), SLOT( slotChooseFramesPerSecond() ) );
        
        m_pJahResLabel = new QLabel( ControlFrameUI[0][0], "languagelabel" );
        otherLayout->addWidget( m_pJahResLabel, 0, 1 );
        m_pJahResLabel->setAlignment( int( QLabel::AlignCenter ) );
        m_pJahResLabel->setText( m_pJt->tr( "Resolution" ) );
        
        m_pJahResCombo = new QComboBox( FALSE, ControlFrameUI[0][0], "m_pJahResCombo" );
        otherLayout->addWidget( m_pJahResCombo, 1, 1 );
        m_bHasResMenu = true;
        
        projectData thedata;
        thedata.buildComboBox(m_pJahResCombo);
        int defaultRes = JahPrefs::getInstance().getJahResolution();
        m_pJahResCombo->setCurrentItem(defaultRes); //should be set to appdefault
        connect( m_pJahResCombo,  SIGNAL(activated(int)), this, SLOT(setResolution(int))  );
        
        l->addStretch();
    }
    
    Globals::setShowLights(true);
    
    ///////////////////////////////////////////////////////////////////////
    //set up world sub control set
    //these are all in ControlFrameUI[1]
    //since fxobject_world is in ControlFrameUI[1]
    
    ControlFrameUI[1][0] = new QFrame( m_aObjectControls[2], "ControlFrame3" ); //was ControlFrame3
    ControlFrameUI[1][0]->setGeometry( QRect( 0, 0, 660, 190 ) );
    
    TextSelect = new QCheckBox( ControlFrameUI[1][0], "TitleSafe" );
    JahFormatter::addCheckButton( TextSelect, 10, 80, 110, 21, m_pJt->tr("title safe") );
    connect( TextSelect,  SIGNAL(clicked()), this, SLOT(setTextSafe())  );
    
    GridSelect = new QCheckBox( ControlFrameUI[1][0], "Grid" );
    JahFormatter::addCheckButton( GridSelect, 10, 40, 110, 21, m_pJt->tr("world grid") );
    connect( GridSelect,  SIGNAL(clicked()), this, SLOT(setGrid())  );
    
    SmoothSelect = new QCheckBox( ControlFrameUI[1][0], "SmoothSelect" );
    JahFormatter::addCheckButton( SmoothSelect, 10, 10, 110, 21, m_pJt->tr("use aliasing") );
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
        JahFormatter::addCheckButton( FogSelect, 200, 10, 110, 21, m_pJt->tr("hardware fog") );
        connect( FogSelect,  SIGNAL(clicked()), this, SLOT(setFog())  );
        
        //QSlider       *HFogSlider, *VFogSlider;
        HFogSlider = new QSlider( ControlFrameUI[1][0] );    HFogSlider_lcd = new InputLCD( ControlFrameUI[1][0] );
        JahFormatter::addSliderLcdLabel( ControlFrameUI[1][0], HFogSlider, 200, 30, 10, 21,  100, HFogSlider_lcd,  "H", 9);
        connect( HFogSlider,     SIGNAL(sliderMoved(int)), this, SLOT(setHFog(int)) );
        connect( HFogSlider,     SIGNAL(sliderMoved(int)), HFogSlider_lcd, SLOT(setValue(int)) );
        connect( HFogSlider_lcd, SIGNAL(valueChanged(int)), this, SLOT(setHFog(int)) );
        
        VFogSlider = new QSlider( ControlFrameUI[1][0] );    VFogSlider_lcd = new InputLCD( ControlFrameUI[1][0] );
        JahFormatter::addSliderLcdLabel( ControlFrameUI[1][0], VFogSlider, 200, 60, 10, 21,  100, VFogSlider_lcd,  "V", 9);
        connect( VFogSlider,     SIGNAL(sliderMoved(int)), this, SLOT(setVFog(int)) );
        connect( VFogSlider,     SIGNAL(sliderMoved(int)), VFogSlider_lcd, SLOT(setValue(int)) );
        connect( VFogSlider_lcd, SIGNAL(valueChanged(int)), this, SLOT(setVFog(int)) );
    }
    
    
    ///////////////////////////////////////////////////////////////
    // object layer options sub control set
    //these are all in ControlFrameUI[1]
    //since fxobject_clip is in ControlFrameUI[1]
    
    ControlFrameUI[1][1] = new QFrame( m_aObjectControls[0], "ControlFrame5" ); //ControlFrame5
    ControlFrameUI[1][1]->setGeometry( QRect( 0, 0, 660, 190 ) );
    
    //the different object types
    //need a label here to explain
    QLabel * layerlabel = new QLabel(ControlFrameUI[1][1]);
    layerlabel->setGeometry( QRect( 65, 10, 90, 25 ) );
    layerlabel->setText( "Layer Type" );
    QFont label_font(  layerlabel->font() );
    label_font.setPointSize( 10 );
    layerlabel->setFont( label_font );
    
    
    ObjectLayer = new QPushButton( ControlFrameUI[1][1], "Layer" );
    JahFormatter::addButton( ObjectLayer, 10, 35, 86, 25 , m_pJt->tr("Layer") );
    connect  ( ObjectLayer,  SIGNAL(clicked()), this, SLOT( ChangeObjectLayer())  );
    
    ObjectCube = new QPushButton( ControlFrameUI[1][1], "Cube" );
    JahFormatter::addButton( ObjectCube, 10, 70, 86, 25, m_pJt->tr("Cube") );
    connect  ( ObjectCube,  SIGNAL(clicked()), this, SLOT( ChangeObjectCube())  );
    
    ObjectCylinder = new QPushButton( ControlFrameUI[1][1], "Cylinder" );
    JahFormatter::addButton( ObjectCylinder, 10, 105, 86, 25, m_pJt->tr("Cylinder") );
    connect  ( ObjectCylinder,  SIGNAL(clicked()), this, SLOT( ChangeObjectCylinder())  );
    
    ObjectSphere = new QPushButton( ControlFrameUI[1][1], "Sphere" );
    JahFormatter::addButton( ObjectSphere, 110, 35, 86, 25, m_pJt->tr("Sphere") );
    connect( ObjectSphere,  SIGNAL(clicked()), this, SLOT( ChangeObjectSphere())  );
    
    ObjectBezier = new QPushButton( ControlFrameUI[1][1], "Bezier" );
    JahFormatter::addButton( ObjectBezier, 110, 70, 86, 25, m_pJt->tr("Bezier") );
    connect( ObjectBezier,  SIGNAL(clicked()), this, SLOT( ChangeObjectBezier())  );
    
    ObjectMesh = new QPushButton( ControlFrameUI[1][1], "Mesh" );
    JahFormatter::addButton( ObjectMesh, 110, 105, 86, 25, m_pJt->tr("Mesh") );
    connect( ObjectMesh,  SIGNAL(clicked()), this, SLOT( ChangeObjectMesh())  );
    
    //for adding effects
    int x_offset = 220;
    
    QLabel * effectlabel = new QLabel(ControlFrameUI[1][1]);
    effectlabel->setGeometry( QRect( x_offset, 10, 86, 25 ) );
    effectlabel->setAlignment(QLabel::AlignCenter);
    effectlabel->setText( "Add Effect" );
    QFont effectlabel_font(  effectlabel->font() );
    effectlabel_font.setPointSize( 10 );
    effectlabel->setFont( effectlabel_font );
    
    
    AddCpuEffectButton = new QPushButton( ControlFrameUI[1][1], "CpuEffectButton" );
    JahFormatter::addButton( AddCpuEffectButton, x_offset, 35, 86, 25, m_pJt->tr("CPU Effect") );
    connect( AddCpuEffectButton,  SIGNAL(clicked()), this, SLOT( addCpuEffect())  );
    
    AddMeshEffectButton = new QPushButton( ControlFrameUI[1][1], "MeshEffectButton" );
    JahFormatter::addButton( AddMeshEffectButton, x_offset, 70, 86, 25, m_pJt->tr("Mesh Effect") );
    connect( AddMeshEffectButton,  SIGNAL(clicked()), this, SLOT( addMeshEffect())  );
    
    AddGpuEffectButton = new QPushButton( ControlFrameUI[1][1], "GpuEffectButton" );
    JahFormatter::addButton( AddGpuEffectButton, x_offset, 105, 86, 25, m_pJt->tr("GPU Effect") );
    connect( AddGpuEffectButton,  SIGNAL(clicked()), this, SLOT( addGpuEffect())  );
    
    x_offset = 330;
    
    m_pCompositingModeSelectButton = new QPushButton( ControlFrameUI[1][1], "CompositingModeButton" );
    JahFormatter::addButton( m_pCompositingModeSelectButton, x_offset, 35, 150, 25, m_pJt->tr("Standard") );
    connect( m_pCompositingModeSelectButton,  SIGNAL(clicked()), this, SLOT( slotSetCompositingMode() )  );
    
    
    m_pCompositingModeLabel = new QLabel(ControlFrameUI[1][1]);
    m_pCompositingModeLabel->setGeometry( QRect( x_offset, 10, 150, 25 ) );
    m_pCompositingModeLabel->setAlignment(QLabel::AlignCenter);
    m_pCompositingModeLabel->setText( "Compositing Mode" );
    QFont compositing_mode_label_font(  m_pCompositingModeLabel->font() );
    compositing_mode_label_font.setPointSize( 10 );
    m_pCompositingModeLabel->setFont( compositing_mode_label_font );
    
    x_offset = 500;
    //options
    m_pBufferSelectCheckBox = new QCheckBox( ControlFrameUI[1][1], "pbufferbox" );
    m_pBufferSelectCheckBox->setGeometry( QRect( x_offset, 55, 115, 21 ) );
    m_pBufferSelectCheckBox->setText( m_pJt->tr("use pbuffer") );
    connect( m_pBufferSelectCheckBox,  SIGNAL(clicked()), this, SLOT( setUsePbuffer())  );
    
    mesh1 = new QCheckBox( ControlFrameUI[1][1], "meshbox" );
    mesh1->setGeometry( QRect( x_offset, 80, 115, 21 ) );
    mesh1->setText( m_pJt->tr("toggle mesh") );
    connect( mesh1,  SIGNAL(clicked()), this, SLOT(toggleMesh())  );
    
    smooth1 = new QCheckBox( ControlFrameUI[1][1], "reflect1" );
    smooth1->setGeometry( QRect( x_offset, 105, 105, 21 ) );
    smooth1->setText( "smoothshading" );
    connect( smooth1,  SIGNAL(clicked()), this, SLOT(toggleSmooth())  );
    
    
    ResetLayerButton = new QPushButton( ControlFrameUI[1][1], "ResetLayerButton" );
    JahFormatter::addButton( ResetLayerButton, x_offset, 10, 86, 25, m_pJt->tr("Reset") );
    connect  ( ResetLayerButton,  SIGNAL(clicked()), this, SLOT( ResetLayer())  );
    
    /////////////////font options
    //these are all in ControlFrameUI[1]
    
    ControlFrameUI[1][2] = new QFrame( m_aObjectControls[1], "ControlFrame6" ); //was ControlFrame6
    ControlFrameUI[1][2]->setGeometry( QRect( 0, 0, 660, 190 ) );
    
    //ScaleText_5 = new QLabel( ControlFrameUI[1][2], "ScaleText_4" );
    //JahFormatter::addLabel( ScaleText_5, 0, 10, 110, 21, "EXTRUDE", 9);
    
    ScaleX_5 = new QSlider( ControlFrameUI[1][2] ); ScaleXlcd_5 = new InputLCD( ControlFrameUI[1][2] );
    //JahFormatter::addSliderLcdLabel( ControlFrameUI[1][2], ScaleX_5,   300, 30, 10, 21,  0, 200,  int( DEFAULT_TEXT_EXTRUDE_DEPTH ),  ScaleXlcd_5,  "E", 9);
    
    connect( ScaleX_5,    SIGNAL(sliderMoved(int)), ScaleXlcd_5, SLOT(setValue(int)) );
    connect( ScaleX_5,    SIGNAL(sliderMoved(int)), this, SLOT(setExtrude(int)) );
    connect( ScaleX_5, SIGNAL(valueChanged(int)), this, SLOT(setExtrude(int)) );
    ScaleX_5->hide();
    ScaleXlcd_5->hide();
    
    m_pTextExtrudeLabel = 
      new QLabel( 
		 ControlFrameUI[1][2], 
		 "m_pTextExtrudeSlider" );

    JahFormatter::addLabel( m_pTextExtrudeLabel
			    , 0, 10, 125, 21, 
			    "Extrude Depth", 9);
    m_pTextExtrudeLabel->setAlignment( int( QLabel::AlignCenter ) );
    
    m_pTextExtrudeSlider = new QSlider( ControlFrameUI[1][2] ); 
    m_pTextExtrudeLCD = new InputLCD( ControlFrameUI[1][2] );
    JahFormatter::addSliderLcdLabel( ControlFrameUI[1][2], m_pTextExtrudeSlider,   0, 30, 10, 21,  0, 200,  
                                     int( DEFAULT_TEXT_EXTRUDE_DEPTH ),  m_pTextExtrudeLCD,  "", 9);
    m_pTextExtrudeSlider->setMinValue(-500);
    m_pTextExtrudeSlider->setMaxValue(500);
    m_pTextExtrudeLCD->setMinInt(-500);
    m_pTextExtrudeLCD->setMaxInt(500);
    
    connect( m_pTextExtrudeSlider, SIGNAL(valueChanged(int)), m_pTextExtrudeLCD, SLOT(setValue(int)) );
    connect( m_pTextExtrudeLCD, SIGNAL(valueChanged(int)), m_pTextExtrudeSlider, SLOT(setValue(int)) );
    connect( m_pTextExtrudeSlider, SIGNAL(valueChanged(int)), this, SLOT( setZScale(int) ) );
    
    textDraw = new QPushButton( ControlFrameUI[1][2], "draw" );
    JahFormatter::addButton( textDraw, 120, 140, 55, 21, m_pJt->tr("Style") );
    connect( textDraw,  SIGNAL(clicked()), this, SLOT(changeFontDraw())  );
    
    m_pTextEd	= new QMultiLineEdit( ControlFrameUI[1][2], "text" );
    m_pTextEd->setGeometry( QRect( 0, 60, 300, 80 ) );
    m_pTextEd->setText( "jahshaka" );
    connect( m_pTextEd, SIGNAL(textChanged()), this, SLOT(m_pTextEdChanged()) );
    
    textFont = new QPushButton( ControlFrameUI[1][2], "font" );
    JahFormatter::addButton( textFont, 0, 140, 55, 21, m_pJt->tr("Font") );
    connect( textFont,  SIGNAL(clicked()), this, SLOT(loadFont())  );
    
    textColor = new QPushButton( ControlFrameUI[1][2], "color" );
    JahFormatter::addButton( textColor, 60, 140, 55, 21, m_pJt->tr("Color") );
    connect( textColor,  SIGNAL(clicked()), this, SLOT(setFontColor())  );
    
    ///////////////////////////////////////////////////////
    //particle options
    
    ControlFrameUI[1][3] = new QFrame( m_aObjectControls[3], "ControlFrame7" ); //was ControlFrame7
    ControlFrameUI[1][3]->setGeometry( QRect( 0, 0, 660, 190 ) );
    
    loadimage_3 = new QPushButton( ControlFrameUI[1][3], "emitter" );
    JahFormatter::addButton( loadimage_3, 0, 20, 111, 20,  m_pJt->tr("Emitter") );
    connect( loadimage_3,  SIGNAL(clicked()), this, SLOT(changeParticle())  );
    
    loadimage_4 = new QPushButton( ControlFrameUI[1][3], "style" );
    JahFormatter::addButton( loadimage_4, 0, 60, 111, 20,  m_pJt->tr("Style") );
    connect( loadimage_4,  SIGNAL(clicked()), this, SLOT(changeParticleDraw())  );
    
    particlestyle = new QPushButton( ControlFrameUI[1][3], "tex" );
    JahFormatter::addButton( particlestyle, 115, 20, 35, 20, m_pJt->tr("Color") );
    connect( particlestyle,  SIGNAL(clicked()), this, SLOT(changeParticleColors())  );
    
    ////////////////////////////////////////////////////////
    //object options 5
    
    ControlFrameUI[1][4] = new QFrame( m_aObjectControls[4], "ControlFrame9" ); //ControlFrame9
    ControlFrameUI[1][4]->setGeometry( QRect( 0, 0, 660, 190 ) );
    
    loadobject_1 = new QPushButton( ControlFrameUI[1][4], "load" );
    JahFormatter::addButton( loadobject_1, 0, 20, 111, 30, m_pJt->tr("Load Object") );
    connect( loadobject_1,  SIGNAL(clicked()), this, SLOT(loadObjObject())  );
    
    objectstyle = new QPushButton( ControlFrameUI[1][4], "draw" );
    JahFormatter::addButton( objectstyle, 115, 20, 35, 30, m_pJt->tr("fill") );
    connect( objectstyle,  SIGNAL(clicked()), this, SLOT(changeObjectDraw())  );
    
    ResetObjectButton1 = new QPushButton( ControlFrameUI[1][4], "ResetLayerButton" );
    JahFormatter::addButton( ResetObjectButton1, 410, 10, 86, 25, m_pJt->tr("Reset") );
    connect  ( ResetObjectButton1,  SIGNAL(clicked()), this, SLOT( ResetLayer())  );
    
    //these should only be on objects
    shader1 = new QCheckBox( ControlFrameUI[1][4], "shader1" );
    shader1->setGeometry( QRect( 400, 100, 105, 21 ) );
    shader1->setText( "pixelshader1" );
    connect( shader1,  SIGNAL(clicked()), this, SLOT(toggleShader1())  );
    
    shader2 = new QCheckBox( ControlFrameUI[1][4], "shader1" );
    shader2->setGeometry( QRect( 525, 100, 105, 21 ) );
    shader2->setText( "pixelshader2" );
    connect( shader2,  SIGNAL(clicked()), this, SLOT(toggleShader2())  );
    
    ///////////////////////////////////////////////////////////////////////
    //lighting controls
    ControlFrameUI[LAYERS_TAB][5] = new QFrame( m_aObjectControls[5], "ControlFrame9" ); //ControlFrame9
    ControlFrameUI[LAYERS_TAB][5]->setGeometry( QRect( 0, 0, 660, 190 ) );
    
    
    int x_position = 10;
    int y_position = 20;
    int slider_number = 0;
    
    m_aLightingSliders[slider_number] 
      = new QSlider( ControlFrameUI[LAYERS_TAB][5] );
    m_aLightingLCDs[slider_number] 
      = new InputLCD( ControlFrameUI[LAYERS_TAB][5] );
    m_aLightingSliderLabels[slider_number] 
      = new QLabel( ControlFrameUI[LAYERS_TAB][5] );
    
    JahFormatter::addJahSliderGroup(  ControlFrameUI[LAYERS_TAB][5], 
				      getLightingSliderPtr(slider_number),
                                      x_position, y_position, -10, 21,  255, 
				      getLightingLcdPtr(slider_number),
                                      getLightingSliderLabelPtr(slider_number), 
				      "Ambient Red", 9);
    
    getLightingSliderPtr(slider_number)->setMinValue(0);
    getLightingSliderPtr(slider_number)->setValue(255);
    getLightingLcdPtr(slider_number)->setValue(255);
    getLightingLcdPtr(slider_number)->setMinInt(0);
    getLightingLcdPtr(slider_number)->setMaxInt(255);
    connect( getLightingSliderPtr(slider_number), SIGNAL(valueChanged(int)), getLightingLcdPtr(slider_number), SLOT( setValue(int) ) );
    connect( getLightingLcdPtr(slider_number), SIGNAL(valueChanged(int)), getLightingSliderPtr(slider_number), SLOT( setValue(int) ) );
    connect( getLightingSliderPtr(slider_number), SIGNAL(valueChanged(int)), this, SLOT( slotLightColorAmbientRed() ) );
    
    y_position += 40;
    slider_number++;
    
    m_aLightingSliders[slider_number] = new QSlider( ControlFrameUI[LAYERS_TAB][5] );
    m_aLightingLCDs[slider_number] = new InputLCD( ControlFrameUI[LAYERS_TAB][5] );
    m_aLightingSliderLabels[slider_number] = new QLabel( ControlFrameUI[LAYERS_TAB][5] );
    
    JahFormatter::addJahSliderGroup(  ControlFrameUI[LAYERS_TAB][5], getLightingSliderPtr(slider_number), 
                                      x_position, y_position, -10, 21,  255, getLightingLcdPtr(slider_number),
                                      getLightingSliderLabelPtr(slider_number), "Ambient Green", 9);
    
    getLightingSliderPtr(slider_number)->setMinValue(0);
    getLightingSliderPtr(slider_number)->setValue(255);
    getLightingLcdPtr(slider_number)->setValue(255);
    getLightingLcdPtr(slider_number)->setMinInt(0);
    getLightingLcdPtr(slider_number)->setMaxInt(255);
    connect( getLightingSliderPtr(slider_number), SIGNAL(valueChanged(int)), getLightingLcdPtr(slider_number), SLOT( setValue(int) ) );
    connect( getLightingLcdPtr(slider_number), SIGNAL(valueChanged(int)), getLightingSliderPtr(slider_number), SLOT( setValue(int) ) );
    connect( getLightingSliderPtr(slider_number), SIGNAL(valueChanged(int)), this, SLOT( slotLightColorAmbientGreen() ) );
    
    y_position += 40;
    slider_number++;
    
    m_aLightingSliders[slider_number] = new QSlider( ControlFrameUI[LAYERS_TAB][5] );
    m_aLightingLCDs[slider_number] = new InputLCD( ControlFrameUI[LAYERS_TAB][5] );
    m_aLightingSliderLabels[slider_number] = new QLabel( ControlFrameUI[LAYERS_TAB][5] );
    
    JahFormatter::addJahSliderGroup(  ControlFrameUI[LAYERS_TAB][5], getLightingSliderPtr(slider_number), 
                                      x_position, y_position, -10, 21,  255, getLightingLcdPtr(slider_number),
                                      getLightingSliderLabelPtr(slider_number), "Ambient Blue", 9);
    
    getLightingSliderPtr(slider_number)->setMinValue(0);
    getLightingSliderPtr(slider_number)->setValue(255);
    getLightingLcdPtr(slider_number)->setValue(255);
    getLightingLcdPtr(slider_number)->setMinInt(0);
    getLightingLcdPtr(slider_number)->setMaxInt(255);
    connect( getLightingSliderPtr(slider_number), SIGNAL(valueChanged(int)), getLightingLcdPtr(slider_number), SLOT( setValue(int) ) );
    connect( getLightingLcdPtr(slider_number), SIGNAL(valueChanged(int)), getLightingSliderPtr(slider_number), SLOT( setValue(int) ) );
    connect( getLightingSliderPtr(slider_number), SIGNAL(valueChanged(int)), this, SLOT( slotLightColorAmbientBlue() ) );
    
    y_position += 40;
    m_ambient_color_button	= new QPushButton(ControlFrameUI[LAYERS_TAB][5],"ambientcolorbox");
    JahFormatter::addIconButton(m_ambient_color_button, x_position + 65, y_position, 40, 22 );
    m_ambient_color_button->setPaletteBackgroundColor( QColor(255, 255, 255) );
    connect( m_ambient_color_button,  SIGNAL(clicked()), this, SLOT(slotChooseLightColorAmbient())  );

    x_position += 155;
    y_position = 20;
    slider_number++;
    
    m_aLightingSliders[slider_number] = new QSlider( ControlFrameUI[LAYERS_TAB][5] );
    m_aLightingLCDs[slider_number] = new InputLCD( ControlFrameUI[LAYERS_TAB][5] );
    m_aLightingSliderLabels[slider_number] = new QLabel( ControlFrameUI[LAYERS_TAB][5] );
    
    JahFormatter::addJahSliderGroup(  ControlFrameUI[LAYERS_TAB][5], getLightingSliderPtr(slider_number), 
                                      x_position, y_position, -10, 21,  255, getLightingLcdPtr(slider_number),
                                      getLightingSliderLabelPtr(slider_number), "Diffuse Red", 9);
    
    getLightingSliderPtr(slider_number)->setMinValue(0);
    getLightingSliderPtr(slider_number)->setValue(255);
    getLightingLcdPtr(slider_number)->setValue(255);
    getLightingLcdPtr(slider_number)->setMinInt(0);
    getLightingLcdPtr(slider_number)->setMaxInt(255);
    connect( getLightingSliderPtr(slider_number), SIGNAL(valueChanged(int)), getLightingLcdPtr(slider_number), SLOT( setValue(int) ) );
    connect( getLightingLcdPtr(slider_number), SIGNAL(valueChanged(int)), getLightingSliderPtr(slider_number), SLOT( setValue(int) ) );
    connect( getLightingSliderPtr(slider_number), SIGNAL(valueChanged(int)), this, SLOT( slotLightColorDiffuseRed() ) );
    
    y_position += 40;
    slider_number++;
    
    m_aLightingSliders[slider_number] = new QSlider( ControlFrameUI[LAYERS_TAB][5] );
    m_aLightingLCDs[slider_number] = new InputLCD( ControlFrameUI[LAYERS_TAB][5] );
    m_aLightingSliderLabels[slider_number] = new QLabel( ControlFrameUI[LAYERS_TAB][5] );
    
    JahFormatter::addJahSliderGroup(  ControlFrameUI[LAYERS_TAB][5], getLightingSliderPtr(slider_number), 
                                      x_position, y_position, -10, 21,  255, getLightingLcdPtr(slider_number),
                                      getLightingSliderLabelPtr(slider_number), "Diffuse Green", 9);
    
    getLightingSliderPtr(slider_number)->setMinValue(0);
    getLightingSliderPtr(slider_number)->setValue(255);
    getLightingLcdPtr(slider_number)->setValue(255);
    getLightingLcdPtr(slider_number)->setMinInt(0);
    getLightingLcdPtr(slider_number)->setMaxInt(255);
    connect( getLightingSliderPtr(slider_number), SIGNAL(valueChanged(int)), getLightingLcdPtr(slider_number), SLOT( setValue(int) ) );
    connect( getLightingLcdPtr(slider_number), SIGNAL(valueChanged(int)), getLightingSliderPtr(slider_number), SLOT( setValue(int) ) );
    connect( getLightingSliderPtr(slider_number), SIGNAL(valueChanged(int)), this, SLOT( slotLightColorDiffuseGreen() ) );
    
    y_position += 40;
    slider_number++;
    
    m_aLightingSliders[slider_number] = new QSlider( ControlFrameUI[LAYERS_TAB][5] );
    m_aLightingLCDs[slider_number] = new InputLCD( ControlFrameUI[LAYERS_TAB][5] );
    m_aLightingSliderLabels[slider_number] = new QLabel( ControlFrameUI[LAYERS_TAB][5] );
    
    JahFormatter::addJahSliderGroup(  ControlFrameUI[LAYERS_TAB][5], getLightingSliderPtr(slider_number), 
                                      x_position, y_position, -10, 21,  255, getLightingLcdPtr(slider_number),
                                      getLightingSliderLabelPtr(slider_number), "Diffuse Blue", 9);
    
    getLightingSliderPtr(slider_number)->setMinValue(0);
    getLightingSliderPtr(slider_number)->setValue(255);
    getLightingLcdPtr(slider_number)->setValue(255);
    getLightingLcdPtr(slider_number)->setMinInt(0);
    getLightingLcdPtr(slider_number)->setMaxInt(255);
    connect( getLightingSliderPtr(slider_number), SIGNAL(valueChanged(int)), getLightingLcdPtr(slider_number), SLOT( setValue(int) ) );
    connect( getLightingLcdPtr(slider_number), SIGNAL(valueChanged(int)), getLightingSliderPtr(slider_number), SLOT( setValue(int) ) );
    connect( getLightingSliderPtr(slider_number), SIGNAL(valueChanged(int)), this, SLOT( slotLightColorDiffuseBlue() ) );
    
    y_position += 40;
    
    m_diffuse_color_button	= new QPushButton(ControlFrameUI[LAYERS_TAB][5],"backgroundcolor");
    JahFormatter::addIconButton(m_diffuse_color_button, x_position + 65, y_position, 40, 22 );
    m_diffuse_color_button->setPaletteBackgroundColor( QColor(255, 255, 255) );
    connect( m_diffuse_color_button,  SIGNAL(clicked()), this, SLOT(slotChooseLightColorDiffuse())  );

    x_position += 155;
    y_position = 20;
    slider_number++;
    
    m_aLightingSliders[slider_number] = new QSlider( ControlFrameUI[LAYERS_TAB][5] );
    m_aLightingLCDs[slider_number] = new InputLCD( ControlFrameUI[LAYERS_TAB][5] );
    m_aLightingSliderLabels[slider_number] = new QLabel( ControlFrameUI[LAYERS_TAB][5] );
    
    JahFormatter::addJahSliderGroup(  ControlFrameUI[LAYERS_TAB][5], getLightingSliderPtr(slider_number), 
                                      x_position, y_position, -10, 21,  255, getLightingLcdPtr(slider_number),
                                      getLightingSliderLabelPtr(slider_number), "Specular Red", 9);
    
    getLightingSliderPtr(slider_number)->setMinValue(0);
    getLightingSliderPtr(slider_number)->setValue(255);
    getLightingLcdPtr(slider_number)->setValue(255);
    getLightingLcdPtr(slider_number)->setMinInt(0);
    getLightingLcdPtr(slider_number)->setMaxInt(255);
    connect( getLightingSliderPtr(slider_number), SIGNAL(valueChanged(int)), getLightingLcdPtr(slider_number), SLOT( setValue(int) ) );
    connect( getLightingLcdPtr(slider_number), SIGNAL(valueChanged(int)), getLightingSliderPtr(slider_number), SLOT( setValue(int) ) );
    connect( getLightingSliderPtr(slider_number), SIGNAL(valueChanged(int)), this, SLOT( slotLightColorSpecularRed() ) );
    
    y_position += 40;
    slider_number++;
    
    m_aLightingSliders[slider_number] = new QSlider( ControlFrameUI[LAYERS_TAB][5] );
    m_aLightingLCDs[slider_number] = new InputLCD( ControlFrameUI[LAYERS_TAB][5] );
    m_aLightingSliderLabels[slider_number] = new QLabel( ControlFrameUI[LAYERS_TAB][5] );
    
    JahFormatter::addJahSliderGroup(  ControlFrameUI[LAYERS_TAB][5], getLightingSliderPtr(slider_number), 
                                      x_position, y_position, -10, 21,  255, getLightingLcdPtr(slider_number),
                                      getLightingSliderLabelPtr(slider_number), "Specular Green", 9);
    
    getLightingSliderPtr(slider_number)->setMinValue(0);
    getLightingSliderPtr(slider_number)->setValue(255);
    getLightingLcdPtr(slider_number)->setValue(255);
    getLightingLcdPtr(slider_number)->setMinInt(0);
    getLightingLcdPtr(slider_number)->setMaxInt(255);
    connect( getLightingSliderPtr(slider_number), SIGNAL(valueChanged(int)), getLightingLcdPtr(slider_number), SLOT( setValue(int) ) );
    connect( getLightingLcdPtr(slider_number), SIGNAL(valueChanged(int)), getLightingSliderPtr(slider_number), SLOT( setValue(int) ) );
    connect( getLightingSliderPtr(slider_number), SIGNAL(valueChanged(int)), this, SLOT( slotLightColorSpecularGreen() ) );
    
    y_position += 40;
    slider_number++;
    
    m_aLightingSliders[slider_number] = new QSlider( ControlFrameUI[LAYERS_TAB][5] );
    m_aLightingLCDs[slider_number] = new InputLCD( ControlFrameUI[LAYERS_TAB][5] );
    m_aLightingSliderLabels[slider_number] = new QLabel( ControlFrameUI[LAYERS_TAB][5] );
    
    JahFormatter::addJahSliderGroup(  ControlFrameUI[LAYERS_TAB][5], getLightingSliderPtr(slider_number), 
                                      x_position, y_position, -10, 21,  255, getLightingLcdPtr(slider_number),
                                      getLightingSliderLabelPtr(slider_number), "Specular Blue", 9);
    
    getLightingSliderPtr(slider_number)->setMinValue(0);
    getLightingSliderPtr(slider_number)->setValue(255);
    getLightingLcdPtr(slider_number)->setValue(255);
    getLightingLcdPtr(slider_number)->setMinInt(0);
    getLightingLcdPtr(slider_number)->setMaxInt(255);
    connect( getLightingSliderPtr(slider_number), SIGNAL(valueChanged(int)), getLightingLcdPtr(slider_number), SLOT( setValue(int) ) );
    connect( getLightingLcdPtr(slider_number), SIGNAL(valueChanged(int)), getLightingSliderPtr(slider_number), SLOT( setValue(int) ) );
    connect( getLightingSliderPtr(slider_number), SIGNAL(valueChanged(int)), this, SLOT( slotLightColorSpecularBlue() ) );
    
    y_position += 40;
    
    m_specular_color_button	= new QPushButton(ControlFrameUI[LAYERS_TAB][5],"backgroundcolor");
    JahFormatter::addIconButton(m_specular_color_button, x_position + 65, y_position, 40, 22 );
    m_specular_color_button->setPaletteBackgroundColor( QColor(255, 255, 255) );
    connect( m_specular_color_button,  SIGNAL(clicked()), this, SLOT(slotChooseLightColorSpecular())  );
    ///////////////////////////////////////////////////////////////////////
    //set up object control set and sliders
    //set up world sub control set
    //these are all in ControlFrameUI[2]
    //since fxaxis_clipone is in ControlFrameUI[2]
    
    ControlFrameUI[2][0] = new QFrame( moduleOptionsUI[2], "ControlFrame1" ); //ControlFrame1
    ControlFrameUI[2][0]->setGeometry( QRect( 0, 0, 660, 190 ) );    // was 760
    
    createAxisPanel ( ControlFrameUI[2][0] );
    
    x_position += 155;
    y_position = 20;
    slider_number++;
    
    m_aLightingSliders[slider_number] = new QSlider( ControlFrameUI[LAYERS_TAB][5] );
    m_aLightingLCDs[slider_number] = new InputLCD( ControlFrameUI[LAYERS_TAB][5] );
    m_aLightingSliderLabels[slider_number] = new QLabel( ControlFrameUI[LAYERS_TAB][5] );
    
    JahFormatter::addJahSliderGroup(  ControlFrameUI[LAYERS_TAB][5], getLightingSliderPtr(slider_number), 
                                      x_position, y_position, -10, 21, 500, getLightingLcdPtr(slider_number),
                                      getLightingSliderLabelPtr(slider_number), "X Position", 9);
    
    connect( getLightingSliderPtr(slider_number), SIGNAL(valueChanged(int)), getLightingLcdPtr(slider_number), SLOT( setValue(int) ) );
    connect( getLightingLcdPtr(slider_number), SIGNAL(valueChanged(int)), getLightingSliderPtr(slider_number), SLOT( setValue(int) ) );
    connect( getLightingSliderPtr(slider_number), SIGNAL(valueChanged(int)), m_aJahSliders[3], SLOT( setValue(int) ) );
    connect( m_aJahSliders[3], SIGNAL(valueChanged(int)), getLightingSliderPtr(slider_number),  SLOT( setValue(int) ) );
    
    y_position += 40;
    slider_number++;
    
    m_aLightingSliders[slider_number] = new QSlider( ControlFrameUI[LAYERS_TAB][5] );
    m_aLightingLCDs[slider_number] = new InputLCD( ControlFrameUI[LAYERS_TAB][5] );
    m_aLightingSliderLabels[slider_number] = new QLabel( ControlFrameUI[LAYERS_TAB][5] );
    
    JahFormatter::addJahSliderGroup(  ControlFrameUI[LAYERS_TAB][5], getLightingSliderPtr(slider_number), 
                                      x_position, y_position, -10, 21, 500, getLightingLcdPtr(slider_number),
                                      getLightingSliderLabelPtr(slider_number), "Y Position", 9);
    
    connect( getLightingSliderPtr(slider_number), SIGNAL(valueChanged(int)), getLightingLcdPtr(slider_number), SLOT( setValue(int) ) );
    connect( getLightingLcdPtr(slider_number), SIGNAL(valueChanged(int)), getLightingSliderPtr(slider_number), SLOT( setValue(int) ) );
    connect( getLightingSliderPtr(slider_number), SIGNAL(valueChanged(int)), m_aJahSliders[4], SLOT( setValue(int) ) );
    connect( m_aJahSliders[4], SIGNAL(valueChanged(int)), getLightingSliderPtr(slider_number),  SLOT( setValue(int) ) );
    
    y_position += 40;
    slider_number++;
    
    m_aLightingSliders[slider_number] = new QSlider( ControlFrameUI[LAYERS_TAB][5] );
    m_aLightingLCDs[slider_number] = new InputLCD( ControlFrameUI[LAYERS_TAB][5] );
    m_aLightingSliderLabels[slider_number] = new QLabel( ControlFrameUI[LAYERS_TAB][5] );
    
    JahFormatter::addJahSliderGroup(  ControlFrameUI[LAYERS_TAB][5], getLightingSliderPtr(slider_number), 
                                      x_position, y_position, -10, 21, 500, getLightingLcdPtr(slider_number),
                                      getLightingSliderLabelPtr(slider_number), "Z Position", 9);
    
    connect( getLightingSliderPtr(slider_number), SIGNAL(valueChanged(int)), getLightingLcdPtr(slider_number), SLOT( setValue(int) ) );
    connect( getLightingLcdPtr(slider_number), SIGNAL(valueChanged(int)), getLightingSliderPtr(slider_number), SLOT( setValue(int) ) );
    connect( getLightingSliderPtr(slider_number), SIGNAL(valueChanged(int)), m_aJahSliders[5], SLOT( setValue(int) ) );
    connect( m_aJahSliders[5], SIGNAL(valueChanged(int)), getLightingSliderPtr(slider_number),  SLOT( setValue(int) ) );
    
    //add the placeholders for plugins
    m_aJahModuleOptions[LIGHTING_OPTION] = new QCheckBox( ControlFrameUI[2][0], "Placeholder" );
    JahFormatter::addCheckButton( m_aJahModuleOptions[LIGHTING_OPTION], 520, 10, 145, 25, "Lighting" );
    connect( m_aJahModuleOptions[LIGHTING_OPTION],  SIGNAL(clicked()), this, SLOT(setOption1())  );
    m_pUseLightingCheckBox = m_aJahModuleOptions[LIGHTING_OPTION];
    
    //increment the ui uption counter...
    m_nNumUiOptions +=1;
    
    //add the placeholders for plugins
    m_aJahModuleOptions[TRANSLATE_FIRST_OPTION] = new QCheckBox( ControlFrameUI[2][0], "Placeholder" );
    JahFormatter::addCheckButton( m_aJahModuleOptions[TRANSLATE_FIRST_OPTION], 520, 35, 145, 25, "Rotate First" );
    connect( m_aJahModuleOptions[TRANSLATE_FIRST_OPTION],  SIGNAL(clicked()), this, SLOT(setOption2())  );
    m_pTranslateFirstCheckBox = m_aJahModuleOptions[TRANSLATE_FIRST_OPTION];
    
    //increment the ui uption counter...
    m_nNumUiOptions +=1;
    
    //add the placeholders for plugins
    m_aJahModuleOptions[FOREGROUND_OPTION] = new QCheckBox( ControlFrameUI[2][0], "Placeholder" );
    JahFormatter::addCheckButton( m_aJahModuleOptions[FOREGROUND_OPTION], 520, 60, 145, 25, "Foreground" );
    connect( m_aJahModuleOptions[FOREGROUND_OPTION],  SIGNAL(clicked()), this, SLOT(setOption3())  ); //for clip
    
    //increment the ui uption counter...
    m_nNumUiOptions +=1;
    
    //add the placeholders for plugins
    m_aJahModuleOptions[DEPTH_OPTION] = new QCheckBox( ControlFrameUI[2][0], "Placeholder" );
    JahFormatter::addCheckButton( m_aJahModuleOptions[DEPTH_OPTION], 520, 85, 145, 25, "Use Depth" );
    connect( m_aJahModuleOptions[DEPTH_OPTION],  SIGNAL(clicked()), this, SLOT(setOption4())  ); //for clip
    
    //increment the ui uption counter...
    m_nNumUiOptions +=1;
    
    ////add the placeholders for plugins
    m_aJahModuleOptions[REFLECTION_OPTION] = new QCheckBox( ControlFrameUI[2][0], "Placeholder" );
    JahFormatter::addCheckButton( m_aJahModuleOptions[REFLECTION_OPTION], 520, 110, 145, 25, "Reflection" );
    connect( m_aJahModuleOptions[REFLECTION_OPTION],  SIGNAL(clicked()), this, SLOT(setOption5())  ); 
    
    m_nNumUiOptions +=1;
    
    //add the checkbox
    m_aJahModuleOptions[SHOW_PATH_OPTION] = new QCheckBox( ControlFrameUI[2][0], "PathSelect" );
    JahFormatter::addCheckButton( m_aJahModuleOptions[SHOW_PATH_OPTION], 520, 135, 145, 25, m_pJt->tr("Show Path") );
    connect( m_aJahModuleOptions[SHOW_PATH_OPTION],  SIGNAL(clicked()), this, SLOT(setKeyframeDrawStatus())  ); //for clip
    
    ////increment the ui uption counter...
    m_nNumUiOptions +=1;
    
    //add the checkbox
    m_aJahModuleOptions[LAYER_SELECTED_VISIBLE_OPTION] = new QCheckBox( ControlFrameUI[2][0], "ShowLayer" );
    JahFormatter::addCheckButton( m_aJahModuleOptions[LAYER_SELECTED_VISIBLE_OPTION], 520, 160, 145, 25, m_pJt->tr("Show Layer") );
    connect( m_aJahModuleOptions[LAYER_SELECTED_VISIBLE_OPTION],  SIGNAL(clicked()), this, SLOT( slotSetLayerSelectedVisible() )  ); //for clip
    
    ////increment the ui uption counter...
    m_nNumUiOptions +=1;
    
    m_bHasButtons = true;
    
    
    ///////////////////////////////////////////////////////////////
    // texture layer options
    // need to add video-texture options here
    
    //image options
    
    ControlFrameUI[3][0] = new QFrame( moduleOptionsUI[3], "ControlFrame2" ); //was ControlFrame2
    ControlFrameUI[3][0]->setGeometry( QRect( 0, 0, 660, 190 ) );
    
    ClipSelect = new QCheckBox( ControlFrameUI[3][0], "ClipSelect" );
    JahFormatter::addCheckButton( ClipSelect, 0, 30, 45, 25, m_pJt->tr("Clip") );
    connect( ClipSelect,  SIGNAL(clicked()), this, SLOT( setClipStatus() )  ); //for clip
    
    grabdesktopClip = new QPushButton( ControlFrameUI[3][0], "grabdesktop" );
    JahFormatter::addButton( grabdesktopClip, 50, 30, 110, 25, m_pJt->tr("Grab Desktop") );
    connect( grabdesktopClip,  SIGNAL(clicked()), this, SLOT( grabDesktop() )  );
    
    keyimage_2 = new QPushButton( ControlFrameUI[3][0], "clipmapping" );
    JahFormatter::addButton( keyimage_2, 165, 30, 35, 25, "IM" );
    connect( keyimage_2,  SIGNAL(clicked()), this, SLOT(keyCliplayer())  );

    m_pUseOpenGLKeyLayerCheckBox = new QCheckBox( ControlFrameUI[3][0], "KeySelect" );
    JahFormatter::addCheckButton( m_pUseOpenGLKeyLayerCheckBox, 220, 30, 120, 25, m_pJt->tr("OpenGL Key") );
    connect( m_pUseOpenGLKeyLayerCheckBox,  SIGNAL(clicked()), this, SLOT( slotSetUseOpenGlKeyer() )  );
    
    m_pUseFastShaderKeyLayerCheckBox = new QCheckBox( ControlFrameUI[3][0], "m_pUseFastShaderKeyLayerCheckBox" );
    JahFormatter::addCheckButton( m_pUseFastShaderKeyLayerCheckBox, 220, 52, 120, 25, m_pJt->tr("Fast Shader Key") );
    connect( m_pUseFastShaderKeyLayerCheckBox,  SIGNAL(clicked()), this, SLOT( slotSetUseFastShaderKeyer() )  );
    
    KeySelect = new QCheckBox( ControlFrameUI[3][0], "KeySelect" );
    JahFormatter::addCheckButton( KeySelect, 0, 75, 45, 25, m_pJt->tr("Key") );
    connect( KeySelect,  SIGNAL(clicked()), this, SLOT( setKeyStatus() )  );
    
    grabdesktopKey = new QPushButton( ControlFrameUI[3][0], "grabdesktop" );
    JahFormatter::addButton( grabdesktopKey, 50, 75, 110, 25, m_pJt->tr("Grab Desktop") );
    connect( grabdesktopKey,  SIGNAL(clicked()), this, SLOT( grabDesktopKey() )  ); //for key...
    
    keyimage_3 = new QPushButton( ControlFrameUI[3][0], "keyOne_3" );
    JahFormatter::addButton( keyimage_3, 165, 75, 35, 25, "KM" );
    connect( keyimage_3,  SIGNAL(clicked()), this, SLOT(keyClipkey())  );
    
    keyStatusBox = new QPushButton( ControlFrameUI[3][0], "Select1" );
    JahFormatter::addButton( keyStatusBox, 220, 75, 35, 25, "Invert" );
    connect( keyStatusBox,  SIGNAL(clicked()), this, SLOT( InvertKeyData() )  );
    
    // video options
    // need to hook this up now...
    
    VideoLabel = new QLabel( ControlFrameUI[3][0], "TranslateText_1" );
    JahFormatter::addLabel( VideoLabel, 380, 10, 120, 21, "VIDEO OPTIONS", 9);
    
    
    m_pSlipFramesLCD = new InputLCD( ControlFrameUI[3][0] );
    JahFormatter::addLabelLcd( ControlFrameUI[3][0], m_pSlipFramesLCD, 380, 30, 60, 21,  0,  "Slip", 8);
    connect(m_pSlipFramesLCD, SIGNAL(valueChanged(int)), this, SLOT( slotSetSlipFrames(int) ) );
    // These are necessary because of some Qt problem
    m_pSlipFramesLCD->setValue(1);
    m_pSlipFramesLCD->setValue(0);
    
    m_pInFramesLCD = new InputLCD( ControlFrameUI[3][0] );
    JahFormatter::addLcdLabel( ControlFrameUI[3][0], m_pInFramesLCD,     460, 30, 30, 21,  0,  "In", 8);
    m_pInFramesLCD->setMinInt(1);
    connect(m_pInFramesLCD, SIGNAL(valueChanged(int)), this, SLOT( setinFrames(int) ) );
    
    m_pOutFramesLCD = new InputLCD( ControlFrameUI[3][0] );
    JahFormatter::addLcdLabel( ControlFrameUI[3][0], m_pOutFramesLCD,    530, 30, 30, 21,  0,  "Out", 8);
    m_pOutFramesLCD->setMinInt(1);
    connect(m_pOutFramesLCD, SIGNAL(valueChanged(int)), this, SLOT( setoutFrames(int) ) );
    
    m_pKeySlipFramesLCD = new InputLCD( ControlFrameUI[3][0] );
    JahFormatter::addLabelLcd( ControlFrameUI[3][0], m_pKeySlipFramesLCD,    380, 75, 60, 21,  0,  "Slip", 8);
    connect(m_pKeySlipFramesLCD, SIGNAL(valueChanged(int)), this, SLOT( slotSetKeySlipFrames(int) ) );
    // These are necessary because of some Qt problem
    m_pKeySlipFramesLCD->setValue(1);
    m_pKeySlipFramesLCD->setValue(0);
    
    m_pKeyInFramesLCD = new InputLCD( ControlFrameUI[3][0] );
    JahFormatter::addLcdLabel( ControlFrameUI[3][0], m_pKeyInFramesLCD,  460, 75, 30, 21,  0,  "In", 8);
    m_pKeyInFramesLCD->setMinInt(1);
    connect(m_pKeyInFramesLCD, SIGNAL(valueChanged(int)), this, SLOT( setinKeyFrames(int) ) );
    
    m_pKeyOutFramesLCD = new InputLCD( ControlFrameUI[3][0] );
    JahFormatter::addLcdLabel( ControlFrameUI[3][0], m_pKeyOutFramesLCD, 530, 75, 30, 21,  0,  "Out", 8);
    m_pKeyOutFramesLCD->setMinInt(1);
    connect(m_pKeyOutFramesLCD, SIGNAL(valueChanged(int)), this, SLOT( setoutKeyFrames(int) ) );
    
    extendHeadTail = new QCheckBox( ControlFrameUI[3][0], "extendHeadTail" );
    JahFormatter::addCheckButton( extendHeadTail, 380, 95, 150, 25, "Extend Head-Tail" );
    connect( extendHeadTail,  SIGNAL(clicked()), this, SLOT(toggleExtendHeadTail())  );

    m_pLockKeyAndClipCheckBox = new QCheckBox( ControlFrameUI[3][0], "m_pLockKeyAndClipCheckBox" );
    JahFormatter::addCheckButton( m_pLockKeyAndClipCheckBox, 380, 130, 100, 25, "Lock Key" );
    connect( m_pLockKeyAndClipCheckBox,  SIGNAL(clicked()), this, SLOT( slotUpdateAnimation() )  );
    
    toggleTheLoop = new QCheckBox( ControlFrameUI[3][0], "toggleTheLoop" );
    JahFormatter::addCheckButton( toggleTheLoop, 510, 95, 100, 25, "Loop" );
    connect( toggleTheLoop,  SIGNAL(clicked()), this, SLOT( toggleLoop() )  );
    
    togglePing = new QCheckBox( ControlFrameUI[3][0], "togglePing" );
    JahFormatter::addCheckButton( togglePing, 510, 130, 100, 25, "Ping Pong" );
    connect( togglePing,  SIGNAL(clicked()), this, SLOT( togglePingPong() )  );
    
    //what is this used for?
    //need to make the icon a lock... 3x current height as well for links
    //lockicon	= new QToolButton(ControlFrameUI[3][0],"lockicon");
    //lockicon->setGeometry( QRect( 600, 55, 18, 26 ) );
    //lockicon->setFixedSize(18, 26);  	//lighticon->setUsesBigPixmap(true);
    //lockicon->setAutoRaise(true);
    
    FindMediaLabel = new QLabel( ControlFrameUI[3][0], "FindMediaLabel" );
    JahFormatter::addLabel( FindMediaLabel, 0, 110, 110, 21, "Media Location", 9);
    
    FindMediaText	= new QLineEdit( ControlFrameUI[3][0], "FindMediaText" );
    FindMediaText->setGeometry( QRect( 2, 135, 250, 21 ) );
    //FindMediaText->setText( "jahshaka" );
    
    FindMediaButton = new QPushButton( ControlFrameUI[3][0], "find" );
    JahFormatter::addButton( FindMediaButton, 255, 135, 55, 21, m_pJt->tr("locate") );
    connect( FindMediaButton,  SIGNAL(clicked()), this, SLOT( locateMissingMedia() )  );
    
    
    ////////////////////////////////////////////////////////
    // give this man a keyframer!
#ifndef JAHPLAYER
    addKeyframer(moduleOptionsUI[4]);
#endif
    
    
    ////////////////////////////////////////////////////////
    // raise the control bar and raise backgound as default
    
    m_pObjectControlStack->raiseWidget(0);
}

void 
GLChar::createMenuItem( QPopupMenu * themenu )
{

    Q_CHECK_PTR(themenu);

    QPopupMenu *submenu0 = new QPopupMenu( this );
    Q_CHECK_PTR( submenu0 );

	connect(submenu0, SIGNAL(activated(int)), this, 
	SLOT(addLayerFromTopMenu(int)));

    submenu0->insertItem("&"+m_pJt->tr("Outline"), 4 );
    submenu0->insertItem("&"+m_pJt->tr("Polugon"), 3 );
    submenu0->insertItem("&"+m_pJt->tr("3D Extrude"), 2 );
    submenu0->insertItem("&"+m_pJt->tr("Textured"), 1 );
    themenu->insertItem( "&Add Text", submenu0 );

    themenu->insertItem("&"+m_pJt->tr("Add Light"),this,SLOT( addLightFromTopMenu() ) );
    themenu->insertItem("&"+m_pJt->tr("Name Layer"),this,SLOT( nameLayerFromTopMenu() ) );
    themenu->insertItem("&"+m_pJt->tr("Delete Layer"),this,SLOT( delLayer() ) );
    themenu->insertSeparator();

    //themenu->insertItem("&"+m_pJt->tr("Load Scene"),this,SLOT( SceneLoad() ) );
    themenu->insertItem("&"+m_pJt->tr("Append Scene"),this,SLOT( SceneAppend() ) );
    //themenu->insertItem("&"+m_pJt->tr("Save Scene"),this,SLOT( sceneSave() ) );
    themenu->insertItem("&"+m_pJt->tr("Export Scene"),this,SLOT( PackageSave() ) );
}

