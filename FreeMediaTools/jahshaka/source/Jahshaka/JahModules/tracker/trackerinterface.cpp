/*******************************************************************************
 **
 ** The source file for the Jahshaka animation interface module
 ** The Jahshaka Project
 ** Copyright (C) 2000-2006 VM Inc.
 ** Released under the GNU General Public License
 **
 *******************************************************************************/

#include "tracker.h"
#include "qslider.h"
#include "qpalette.h"
#include <jahformatter.h>
#include <valueFormatters.h>
#include <jahtranslate.h>
#include <InputLCD.h>
#include <supergrangecontrol.h>
#include <qframe.h>
#include <qhbox.h>
#include <qlistview.h>
#include <qcombobox.h>
#include <qtoolbutton.h>
#include <qlabel.h>
#include <widget.h>
#include <openmedialib.h>

void GLTracker::buildInterfaceMenus( QHBox* , QHBox* rightcontroller) 
{
   	// Container widget
	QWidget *container = new QWidget( rightcontroller, "container" );
	QVBoxLayout *container_layout = new QVBoxLayout( container, 0, 0, "container_layout");

	container_layout->addStretch( );
 
    //moving ui elements after zoom
    tooltranslate = new JahToolButton( container, "translate" );
    JahFormatter::addJahPlayerButton( tooltranslate, m_qsJahBasePath+"Pixmaps/desktop/transtool.png", m_qsJahBasePath+"Pixmaps/desktop/transtool.png" );
	container_layout->addWidget( tooltranslate );
    
    toolrotate =   new JahToolButton( container, "rotate" );
    JahFormatter::addJahPlayerButton( toolrotate, m_qsJahBasePath+"Pixmaps/desktop/rotatetool.png", m_qsJahBasePath+"Pixmaps/desktop/rotatetool.png" );
	container_layout->addWidget( toolrotate );
    
    toolscale =   new JahToolButton( container, "scale" );
    JahFormatter::addJahPlayerButton( toolscale,  m_qsJahBasePath+"Pixmaps/desktop/scaletool.png", m_qsJahBasePath+"Pixmaps/desktop/scaletool.png" );
	container_layout->addWidget( toolscale );

	container_layout->addSpacing( 10 );
    
    m_pScrubRender = new JahToolButton( container, "controllerrewindbutton" );
    JahFormatter::addJahPlayerButton( m_pScrubRender, m_qsJahBasePath+"Pixmaps/desktop/phototool.png", m_qsJahBasePath+"Pixmaps/desktop/phototool.png" );
    connect( m_pScrubRender,  SIGNAL(clicked()), SLOT(Render())  );
	container_layout->addWidget( m_pScrubRender );
    
    m_pScrubRenderAll = new JahToolButton( container, "controllerpreviousbutton" );
    JahFormatter::addJahPlayerButton( m_pScrubRenderAll, m_qsJahBasePath+"Pixmaps/desktop/rendertool.png", m_qsJahBasePath+"Pixmaps/desktop/rendertool.png" );
    connect( m_pScrubRenderAll,  SIGNAL(clicked()), SLOT(RenderScene())  );
	container_layout->addWidget( m_pScrubRenderAll );
    
    //connect tools to object and add object to display widget
    connect( tooltranslate,  SIGNAL(clicked()), this, SLOT(toolTranslate() )  );
    connect( toolscale,      SIGNAL(clicked()), this, SLOT(toolScale() )  );
    connect( toolrotate,     SIGNAL(clicked()), this, SLOT(toolRotate() )  );
    
	container_layout->addStretch( );
}

void GLTracker::buildInterface( QHBox* f ) 
{
    JahPrefs& jprefs = JahPrefs::getInstance();
    
    JahFormatter::setMarginSmall( f->layout() );
    JahFormatter::setSpacing( f->layout() );

    /////////////////////////////////////////////////////////
    //this is the list-view
    m_pLayerListView = new QListView( f );

    m_pLayerListView->setRootIsDecorated(true);
    m_pLayerListView->addColumn( (m_pJt->tr("EFFECTS")), -1 );
    m_pLayerListView->setSorting( -1,1 );   // disables the autosort    
    //effects currently only works in single m_bSelection mode
    m_pLayerListView->setSelectionMode (QListView::Single );
    
    JahFormatter::setListViewAsSingleColumn( m_pLayerListView );

    connect( m_pLayerListView, SIGNAL( clicked( QListViewItem* ) ), this, SLOT( layerClicked( QListViewItem* ) ) );
    
    /////////////////////////////////////////////////////////
    //Set up tabbed interface
    
    tabframe = new QTabWidget( f, "axiscontrols" );  
    
    tabframe->setTabShape( QTabWidget::Triangular );
    tabframe->setAutoMask( FALSE );
    
    //this has become the settings panel need to update variable names
    moduleOptionsUI[0] = new QHBox( f, "encoderUI0" ); //scenecontrols
    tabframe->insertTab( moduleOptionsUI[0], tr( "  "+m_pJt->tr("SCENE")+"  " ) );
    
    moduleOptionsUI[1] = new QHBox( f, "axiscontrols" ); //axiscontrols
    tabframe->insertTab( moduleOptionsUI[1], tr( "  "+m_pJt->tr("CONTROLS")+"  " ) );
    
    moduleOptionsUI[2] = new QHBox( f, "texturecontrols" ); //texturecontrols
    tabframe->insertTab( moduleOptionsUI[2], tr( "    "+m_pJt->tr("CLIP")+"    " ) );
    
    //this has become the settings panel need to update variable names
    moduleOptionsUI[3] = new QHBox( f, "keysettings" ); //keycontrols
    tabframe->insertTab( moduleOptionsUI[3], tr( "  "+m_pJt->tr("KEYFRAMES")+"  " ) );
    
    /////////////////////////////////////////////////////////////////////
    // the layout boxes for different types of layers
    // the object control tab bar
    // this is the object control panel
    
    ///////////////////////////////////////////////////////////////////
    // the scene control tab bar
    
    //mover this into the layers...
    {
        ControlFrameUI[0][0] = new QFrame( moduleOptionsUI[0], "ControlFrame3" ); //scenecontrols
        QBoxLayout* outer = new QVBoxLayout( ControlFrameUI[0][0] );
        QBoxLayout* top = new QHBoxLayout();
        JahFormatter::setMarginAndSpacing( top );
        outer->addLayout( top );

        QBoxLayout* buttons = new QVBoxLayout();
        top->addLayout( buttons );

        LoadButton = new QPushButton( ControlFrameUI[0][0], "LoadButton" );
        buttons->addWidget( LoadButton );
        LoadButton->setText( m_pJt->tr("Load") );
        connect( LoadButton,  SIGNAL(clicked()), this, SLOT( SceneLoad())  );
    
        SaveButton = new QPushButton( ControlFrameUI[0][0], "SaveButton" );
        buttons->addWidget( SaveButton );
        SaveButton->setText( m_pJt->tr("Save") );
        connect( SaveButton,  SIGNAL(clicked()), this, SLOT( sceneSave())  );
    
        ExportButton = new QPushButton( ControlFrameUI[0][0], "ExportButton" );
        buttons->addWidget( ExportButton );
        ExportButton->setText( m_pJt->tr("Export") );
        connect( ExportButton,  SIGNAL(clicked()), this, SLOT( PackageSave() )  );
    
        ResetButton = new QPushButton( ControlFrameUI[0][0], "ResetButton" );
        buttons->addWidget( ResetButton );
        ResetButton->setText( m_pJt->tr("Reset All") );
        connect( ResetButton,  SIGNAL(clicked()), this, SLOT( ResetAll())  );
    
        buttons->addStretch();

        QBoxLayout* framesAndRes = new QVBoxLayout();
        top->addLayout( framesAndRes );
        JahFormatter::setMarginAndSpacing( framesAndRes );

        //need to move this into the module
        //this is mandatory for all modules...
        QGridLayout* frames = new QGridLayout( 2, 2 );
        framesAndRes->addLayout( frames );
        
        QLabel* sframetext = new QLabel( ControlFrameUI[0][0], "sframetext" );
        frames->addWidget( sframetext, 0, 0 );
        sframetext->setText( tr( m_pJt->tr("STARTFRAME") ) );
        sframetext->setAlignment( int( QLabel::AlignRight ) );
    
        m_pStartFrameControl = new SupergRangeControl( ControlFrameUI[0][0], "startframe" );
        frames->addWidget( m_pStartFrameControl, 0, 1 );
        m_pStartFrameControl->setFormatter( TimecodeValueFormatter() );
        JahFormatter::configure( m_pStartFrameControl, m_nAStartFrame, m_nAStartFrame );
        connect( m_pStartFrameControl, SIGNAL(valueChanged(int)),   SLOT(updatesliderStartframe(int)) );
    
        QLabel* eframetext = new QLabel( ControlFrameUI[0][0], "eframetext" );
        frames->addWidget( eframetext, 1, 0 );
        eframetext->setText( tr( m_pJt->tr("ENDFRAME")  ) );
        eframetext->setAlignment( int( QLabel::AlignRight ) );
    
        m_pEndFrameControl = new SupergRangeControl( ControlFrameUI[0][0], "endframe" );
        frames->addWidget( m_pEndFrameControl, 1, 1 );
        m_pEndFrameControl->setFormatter( TimecodeValueFormatter() );
        JahFormatter::configure( m_pEndFrameControl, m_nAEndFrame, m_nAStartFrame );
        connect( m_pEndFrameControl, SIGNAL(valueChanged(int)),   SLOT(updatesliderEndframe(int)) );
    
        QBoxLayout* resolution = new QVBoxLayout();
        framesAndRes->addLayout( resolution );
        
        m_pJahResLabel = new QLabel( ControlFrameUI[0][0], "languagelabel" );
        resolution->addWidget( m_pJahResLabel );
        m_pJahResLabel->setAlignment( int( QLabel::AlignCenter ) );
        m_pJahResLabel->setText( m_pJt->tr( "Resolution" ) );
    
        m_pJahResCombo = new QComboBox( FALSE, ControlFrameUI[0][0], "m_pJahResCombo" );
        resolution->addWidget( m_pJahResCombo );
        projectData thedata;
        thedata.buildComboBox(m_pJahResCombo);
        int defaultRes = jprefs.getJahResolution();
        m_pJahResCombo->setCurrentItem(defaultRes); //should be set to appdefault
        connect( m_pJahResCombo,  SIGNAL(activated(int)), this, SLOT(setResolution(int))  );
        m_bHasResMenu = true;

        framesAndRes->addStretch();
        top->addStretch();
        outer->addStretch();
    }
    
    
    ///////////////////////////////////////////////////////////////
    // object layer options sub control set
    
    ControlFrameUI[1][0] = new QFrame( moduleOptionsUI[1], "ControlFrame1" ); //ControlFrame1
    
    //createAxisPanel ( ControlFrameUI[1][0] );
    
    int x_position = 10;
    int y_position = 20;
    
    m_x_position_slider = new QSlider( ControlFrameUI[1][0] );
    m_x_position_lcd = new InputLCD( ControlFrameUI[1][0] );
    m_x_position_label = new QLabel( ControlFrameUI[1][0] );
    
    JahFormatter::addJahSliderGroup(  ControlFrameUI[1][0], m_x_position_slider, 
                                      x_position, y_position, 80, 21,  500, m_x_position_lcd,
                                      m_x_position_label, "X Position", 9);
    
    m_x_position_lcd->setMinInt(-500);
    m_x_position_lcd->setMaxInt(500);
    m_x_position_lcd->setValue(0);
    
    connect( m_x_position_slider, SIGNAL(valueChanged(int)), this, SLOT( setXRotation(int) ) );
    connect( m_x_position_lcd, SIGNAL(valueChanged(int)), m_x_position_slider, SLOT( setValue(int) ) );
    connect( m_x_position_slider, SIGNAL(valueChanged(int)), m_x_position_lcd, SLOT( setValue(int) ) );
    connect( m_x_position_slider, SIGNAL(valueChanged(int)), this, SLOT(captureTrackerPoint(int)) );
    
    
    y_position += 40;
    
    m_y_position_slider = new QSlider( ControlFrameUI[1][0] );
    m_y_position_lcd = new InputLCD( ControlFrameUI[1][0] );
    m_y_position_label = new QLabel( ControlFrameUI[1][0] );
    
    JahFormatter::addJahSliderGroup(  ControlFrameUI[1][0], m_y_position_slider, 
                                      x_position, y_position, 80, 21,  500, m_y_position_lcd,
                                      m_y_position_label, "Y Position", 9);
    
    m_y_position_lcd->setMinInt(-500);
    m_y_position_lcd->setMaxInt(500);
    m_y_position_lcd->setValue(0);
    
    connect( m_y_position_slider, SIGNAL(valueChanged(int)), this, SLOT( setYRotation(int) ) );
    connect( m_y_position_lcd, SIGNAL(valueChanged(int)), m_y_position_slider, SLOT( setValue(int) ) );
    connect( m_y_position_slider, SIGNAL(valueChanged(int)), m_y_position_lcd, SLOT( setValue(int) ) );
    connect( m_y_position_slider, SIGNAL(valueChanged(int)), this, SLOT(captureTrackerPoint(int)) );
    
    
    y_position += 40;
    
    m_point_radius_slider = new QSlider( ControlFrameUI[1][0] );
    m_point_radius_lcd = new InputLCD( ControlFrameUI[1][0] );
    m_point_radius_label = new QLabel( ControlFrameUI[1][0] );
    
    JahFormatter::addJahSliderGroup(  ControlFrameUI[1][0], m_point_radius_slider, 
                                      x_position, y_position, 80, 21,  500, m_point_radius_lcd,
                                      m_point_radius_label, "Point Radius", 9);
    
    m_point_radius_slider->setMinValue(0);
    m_point_radius_lcd->setMinInt(0);
    m_point_radius_lcd->setMaxInt(500);
    m_point_radius_lcd->setValue(0);
    
    connect( m_point_radius_slider, SIGNAL(valueChanged(int)), this, SLOT( setZRotation(int) ) );
    connect( m_point_radius_lcd, SIGNAL(valueChanged(int)), m_point_radius_slider, SLOT( setValue(int) ) );
    connect( m_point_radius_slider, SIGNAL(valueChanged(int)), m_point_radius_lcd, SLOT( setValue(int) ) );
    connect( m_point_radius_slider, SIGNAL(valueChanged(int)), this, SLOT(captureTrackerPoint(int)) );
    
    x_position += 230;
    y_position = 20;
    
    m_point_search_radius_slider = new QSlider( ControlFrameUI[1][0] );
    m_point_search_radius_lcd = new InputLCD( ControlFrameUI[1][0] );
    m_point_search_radius_label = new QLabel( ControlFrameUI[1][0] );
    
    JahFormatter::addJahSliderGroup(  ControlFrameUI[1][0], m_point_search_radius_slider, 
                                      x_position, y_position, 80, 21,  500, m_point_search_radius_lcd,
                                      m_point_search_radius_label, "Search Radius", 9);
    
    m_point_search_radius_slider->setMinValue(0);
    m_point_search_radius_lcd->setMinInt(0);
    m_point_search_radius_lcd->setMaxInt(500);
    m_point_search_radius_lcd->setValue(0);
    
    connect( m_point_search_radius_slider, SIGNAL(valueChanged(int)), this, SLOT( setXTrans(int) ) );
    connect( m_point_search_radius_lcd, SIGNAL(valueChanged(int)), m_point_search_radius_slider, SLOT( setValue(int) ) );
    connect( m_point_search_radius_slider, SIGNAL(valueChanged(int)), m_point_search_radius_lcd, SLOT( setValue(int) ) );
    connect( m_point_search_radius_slider, SIGNAL(valueChanged(int)), this, SLOT(captureTrackerPoint(int)) );
    
    y_position += 40;
    
    m_point_sensitivity_slider = new QSlider( ControlFrameUI[1][0] );
    m_point_sensitivity_lcd = new InputLCD( ControlFrameUI[1][0] );
    m_point_sensitivity_label = new QLabel( ControlFrameUI[1][0] );
    
    JahFormatter::addJahSliderGroup(  ControlFrameUI[1][0], m_point_sensitivity_slider, 
                                      x_position, y_position, 80, 21,  500, m_point_sensitivity_lcd,
                                      m_point_sensitivity_label, "Sensitivity", 9);
    
    m_point_sensitivity_lcd->setMinInt(-500);
    m_point_sensitivity_lcd->setMaxInt(500);
    m_point_sensitivity_lcd->setValue(0);
    
    connect( m_point_sensitivity_slider, SIGNAL(valueChanged(int)), this, SLOT( setYTrans(int) ) );
    connect( m_point_sensitivity_lcd, SIGNAL(valueChanged(int)), m_point_sensitivity_slider, SLOT( setValue(int) ) );
    connect( m_point_sensitivity_slider, SIGNAL(valueChanged(int)), m_point_sensitivity_lcd, SLOT( setValue(int) ) );
    connect( m_point_sensitivity_slider, SIGNAL(valueChanged(int)), this, SLOT(captureTrackerPoint(int)) );
    
    y_position += 40;
    
    m_point_checksum_tolerance_slider = new QSlider( ControlFrameUI[1][0] );
    m_point_checksum_tolerance_lcd = new InputLCD( ControlFrameUI[1][0] );
    m_point_checksum_tolerance_label = new QLabel( ControlFrameUI[1][0] );
    
    JahFormatter::addJahSliderGroup(  ControlFrameUI[1][0], m_point_checksum_tolerance_slider, 
                                      x_position, y_position, 80, 21,  500, m_point_checksum_tolerance_lcd,
                                      m_point_checksum_tolerance_label, "Checksum Tolerance", 9);
    
    m_point_checksum_tolerance_lcd->setMinInt(-500);
    m_point_checksum_tolerance_lcd->setMaxInt(500);
    m_point_checksum_tolerance_lcd->setValue(0);
    
    connect( m_point_checksum_tolerance_slider, SIGNAL(valueChanged(int)), this, SLOT( setZTrans(int) ) );
    connect( m_point_checksum_tolerance_lcd, SIGNAL(valueChanged(int)), m_point_checksum_tolerance_slider, SLOT( setValue(int) ) );
    connect( m_point_checksum_tolerance_slider, SIGNAL(valueChanged(int)), m_point_checksum_tolerance_lcd, SLOT( setValue(int) ) );
    connect( m_point_checksum_tolerance_slider, SIGNAL(valueChanged(int)), this, SLOT(captureTrackerPoint(int)) );
    
    
    m_stabilize_qcheckbox = new QCheckBox( ControlFrameUI[1][0], "Stabilize" );
    m_stabilize_qcheckbox->setGeometry( QRect( 500, 20, 210, 21 ) );
    m_stabilize_qcheckbox->setText( "Stabilize" );
    connect( m_stabilize_qcheckbox,  SIGNAL(clicked()), this, SLOT(setStabilize())  );
    
    m_use_gpu_qcheckbox = new QCheckBox( ControlFrameUI[1][0], "GPUSelect" );
    m_use_gpu_qcheckbox->setGeometry( QRect( 500, 40, 210, 21 ) );
    m_use_gpu_qcheckbox->setText( "Use GPU" );
    m_use_gpu_qcheckbox->toggle();
    connect( m_use_gpu_qcheckbox,  SIGNAL(clicked()), this, SLOT(setGpuSelect())  );
        
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
    
    ClipSelect = new QCheckBox( ControlFrameUI[2][0], "ClipSelect" );
    JahFormatter::addCheckButton( ClipSelect, 0, 30, 45, 25, m_pJt->tr("Clip") );
    connect( ClipSelect,  SIGNAL(clicked()), this, SLOT(setClipStatus())  ); //for clip
    
    grabdesktopClip = new QPushButton( ControlFrameUI[2][0], "grabdesktop" );
    JahFormatter::addButton( grabdesktopClip, 45, 30, 110, 25, m_pJt->tr("Grab Desktop") );
    //connect( grabdesktopClip,  SIGNAL(clicked()), this, SLOT(grabDesktop())  );
    
    keyimage_2 = new QPushButton( ControlFrameUI[2][0], "clipmapping" );
    JahFormatter::addButton( keyimage_2, 165, 30, 35, 25, "IM" );
    //connect( keyimage_2,  SIGNAL(clicked()), this, SLOT(keyCliplayer())  );
    
    
    KeySelect = new QCheckBox( ControlFrameUI[2][0], "KeySelect" );
    JahFormatter::addCheckButton( KeySelect, 0, 75, 45, 25, m_pJt->tr("Key") );
    connect( KeySelect,  SIGNAL(clicked()), this, SLOT(setKeyStatus())  );
    
    grabdesktopKey = new QPushButton( ControlFrameUI[2][0], "grabdesktop" );
    JahFormatter::addButton( grabdesktopKey, 45, 75, 110, 25, m_pJt->tr("Grab Desktop") );
    //connect( grabdesktopKey,  SIGNAL(clicked()), this, SLOT(grabDesktopKey())  ); //for key...
    
    keyimage_3 = new QPushButton( ControlFrameUI[2][0], "keyOne_3" );
    JahFormatter::addButton( keyimage_3, 165, 75, 35, 25, "KM" );
    //connect( keyimage_3,  SIGNAL(clicked()), this, SLOT(keyClipkey())  );
    
    keyStatusBox = new QPushButton( ControlFrameUI[2][0], "Select1" );
    JahFormatter::addButton( keyStatusBox, 220, 75, 35, 25, "Invert" );
    connect( keyStatusBox,  SIGNAL(clicked()), this, SLOT(InvertKeyData())  );
    
    // video options
    // need to hook this up now...
    
    VideoLabel = new QLabel( ControlFrameUI[2][0], "TranslateText_1" );
    JahFormatter::addLabel( VideoLabel, 380, 10, 110, 21, "VIDEO OPTIONS", 9);
    
    
    m_pSlipFramesLCD = new InputLCD( ControlFrameUI[2][0] );
    JahFormatter::addLabelLcd( ControlFrameUI[2][0], m_pSlipFramesLCD, 380, 30, 60, 21,  0,  "Slip", 8);
    m_pSlipFramesLCD->setActive    (false);
    
    m_pInFramesLCD = new InputLCD( ControlFrameUI[2][0] );
    JahFormatter::addLcdLabel( ControlFrameUI[2][0], m_pInFramesLCD,     460, 30, 30, 21,  0,  "In", 8);
    connect(m_pInFramesLCD, SIGNAL(valueChanged(int)), this, SLOT(setinFrames(int)) );
    
    m_pOutFramesLCD = new InputLCD( ControlFrameUI[2][0] );
    JahFormatter::addLcdLabel( ControlFrameUI[2][0], m_pOutFramesLCD,    530, 30, 30, 21,  0,  "Out", 8);
    connect(m_pOutFramesLCD, SIGNAL(valueChanged(int)), this, SLOT(setoutFrames(int)) );
    
    m_pKeySlipFramesLCD = new InputLCD( ControlFrameUI[2][0] );
    JahFormatter::addLabelLcd( ControlFrameUI[2][0], m_pKeySlipFramesLCD,    380, 75, 60, 21,  0,  "Slip", 8);
    
    m_pKeyInFramesLCD = new InputLCD( ControlFrameUI[2][0] );
    JahFormatter::addLcdLabel( ControlFrameUI[2][0], m_pKeyInFramesLCD,  460, 75, 30, 21,  0,  "In", 8);
    connect(m_pKeyInFramesLCD, SIGNAL(valueChanged(int)), this, SLOT(setinKeyFrames(int)) );
    
    m_pKeyOutFramesLCD = new InputLCD( ControlFrameUI[2][0] );
    JahFormatter::addLcdLabel( ControlFrameUI[2][0], m_pKeyOutFramesLCD, 530, 75, 30, 21,  0,  "Out", 8);
    connect(m_pKeyOutFramesLCD, SIGNAL(valueChanged(int)), this, SLOT(setoutKeyFrames(int)) );
    
    extendHeadTail = new QCheckBox( ControlFrameUI[2][0], "extendHeadTail" );
    JahFormatter::addCheckButton( extendHeadTail, 380, 95, 100, 25, "Extend Head-Tail" );
    connect( extendHeadTail,  SIGNAL(clicked()), this, SLOT(toggleExtendHeadTail())  );
    
    toggleTheLoop = new QCheckBox( ControlFrameUI[2][0], "toggleTheLoop" );
    JahFormatter::addCheckButton( toggleTheLoop, 480, 95, 100, 25, "Loop" );
    connect( toggleTheLoop,  SIGNAL(clicked()), this, SLOT(toggleLoop())  );
    
    togglePing = new QCheckBox( ControlFrameUI[2][0], "togglePing" );
    JahFormatter::addCheckButton( togglePing, 480, 130, 100, 25, "Ping Pong" );
    connect( togglePing,  SIGNAL(clicked()), this, SLOT(togglePingPong())  );
    
    //what is this used for?
    //need to make the icon a lock... 3x current height as well for links
    lockicon    = new QToolButton(ControlFrameUI[2][0],"lockicon");
    lockicon->setGeometry( QRect( 600, 55, 18, 26 ) );
    lockicon->setFixedSize(18, 26);     //lighticon->setUsesBigPixmap(true);
    lockicon->setAutoRaise(true);
    
    
}

void GLTracker::createMenuItem( QPopupMenu * themenu )
{
    Q_CHECK_PTR(themenu);
    themenu->insertItem("&"+m_pJt->tr("Export Scene"),this,SLOT( PackageSave() ) );
    //themenu->insertItem("&"+m_pJt->tr("Delete Object"),this,SLOT(delLayer()) );
    themenu->insertSeparator();
    //make this a popup 'Move'
    //themenu->insertItem("&"+m_pJt->tr("Move Up"),this, SLOT(moveLayerUp()) );
    //themenu->insertItem("&"+m_pJt->tr("Move Down"),this, SLOT(moveLayerDown()) ); 
}
