/*******************************************************************************
**
** The source file for the Jahshaka animation interface module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "keyer.h"
#include "keyercolorwidget.h"
#include "qtcolortriangle.h"
#include <InputLCD.h>
#include <supergrangecontrol.h>
#include <jahformatter.h>
#include <jahtranslate.h>
#include <valueFormatters.h>
#include <qlistview.h>
#include <qframe.h>
#include <qhbox.h>
#include <qlabel.h>
#include <qslider.h>
#include <widget.h>

void GLKeyer::buildInterfaceMenus( QHBox*, QHBox* rightcontroller) 
{
    //rightmenu here
    glworldRight =  new QFrame (rightcontroller, "gl right" );

    toolzoomin =   new JahToolButton( glworldRight, "zoomin" );

    JahFormatter::addJahPlayerButton( toolzoomin,   0, 15, 30, 27 ,    24, 28, 
                                      m_qsJahBasePath+"Pixmaps/desktop/zoomin.png", m_qsJahBasePath+"Pixmaps/desktop/zoomin.png" );

    zoom = new QSlider( glworldRight, "scrubslider" );
    zoom->setGeometry( QRect( 0, 50, 30, 150 ) );
    zoom->setOrientation( QSlider::Vertical );
    zoom->setMinValue( -10 );    zoom->setMaxValue( 10 );    zoom->setValue   ( 0 );
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

void GLKeyer::buildInterface( QHBox* f ) 
{
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
    LoadButton = new QPushButton( LayersFrame, "LoadButton" );
    LayersFrameLayout->addWidget( LoadButton );
    LoadButton->setText( m_pJt->tr("Load") );
    connect  ( LoadButton,  SIGNAL(clicked()), this, SLOT( SceneLoad())  );

    SaveButton = new QPushButton( LayersFrame, "SaveButton" );
    LayersFrameLayout->addWidget( SaveButton );
    SaveButton->setText( m_pJt->tr("Save") );
    connect  ( SaveButton,  SIGNAL(clicked()), this, SLOT( sceneSave())  );

    ResetButton = new QPushButton( LayersFrame, "ResetButton" );
    LayersFrameLayout->addWidget( ResetButton );
    ResetButton->setText( m_pJt->tr("Reset") );
    connect  ( ResetButton,  SIGNAL(clicked()), this, SLOT( ResetAll())  );
    
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
    m_pLayerListView->addColumn( (m_pJt->tr("EFFECTS")), -1 );
    m_pLayerListView->setSorting( -1,1 );   // disables the autosort
    //effects currently only works in single m_bSelection mode
    m_pLayerListView->setSelectionMode (QListView::Single );

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
    moduleOptionsUI[0] = new QHBox( EffectsFrame, "keyer" ); //keycontrols
    tabframe->insertTab( moduleOptionsUI[0], tr( "  "+m_pJt->tr("KEYER")+"  " ) );

    //this has become the settings panel need to update variable names
    moduleOptionsUI[1] = new QHBox( EffectsFrame, "keysettings" ); //keycontrols
    tabframe->insertTab( moduleOptionsUI[1], tr( "  "+m_pJt->tr("KEYFRAMES")+"  " ) );

    //this has become the settings panel need to update variable names
    moduleOptionsUI[2] = new QHBox( EffectsFrame, "options" ); //scenecontrols
    tabframe->insertTab( moduleOptionsUI[2], tr( "  "+m_pJt->tr("OPTIONS")+"  " ) );

    //this is only here temporarily
    //this has become the settings panel need to update variable names
    moduleOptionsUI[3] = new QHBox( EffectsFrame, "options" ); //scenecontrols
    tabframe->insertTab( moduleOptionsUI[3], tr( "  "+m_pJt->tr("AXIS")+"  " ) );

    /////////////////////////////////////////////////////////////////////
    // the layout boxes for different types of layers
    // the object control tab bar
    // this is the object control panel

    // object layer options sub control set
    m_bHasButtons = false;

     ///////////////////////////////////////////////////////////////
    // create the color correction controls

    ControlFrameUI[0][0] = new QFrame( moduleOptionsUI[0], "ControlFrame2" ); //was ControlFrame2
    ControlFrameUI[0][0]->setGeometry( QRect( 0, 0, 660, 190 ) );

    createKeyerPanel ( ControlFrameUI[0][0] );

    ////////////////////////////////////////////////////////////////
    //initialize the keyframe editor
    addKeyframer(moduleOptionsUI[1]);

    ////////////////////////////////////
    // create control frame

    {
        QWidget* placeholder = new QWidget( moduleOptionsUI[2] );
        QBoxLayout* top = new QVBoxLayout( placeholder );
        QBoxLayout* mainLayout = new QHBoxLayout();
        JahFormatter::setMarginAndSpacing( mainLayout );
        top->addLayout( mainLayout );
        
#if 0
        backgroundSelect = new QCheckBox( moduleOptionsUI[2], "ClipSelect" );
        JahFormatter::addCheckButton( backgroundSelect, 20, 60, 210, 25, m_pJt->tr("Background") );
        connect( backgroundSelect,  SIGNAL(clicked()), this, SLOT(setBackgroundStatus())  ); //for clip
#endif
        {
            QGridLayout* g = new QGridLayout( 2, 2 );
            mainLayout->addLayout( g );
            
            VideoRenderSelect = new QCheckBox( placeholder, "RenderUsingSourceResolution" );
            VideoRenderSelect->setText( m_pJt->tr("Render Using Source Res") );
            g->addWidget( VideoRenderSelect, 0, 0 );
            connect( VideoRenderSelect,  SIGNAL(clicked()), this, SLOT(changeVideoRender())  );

            m_use_gpu_qcheckbox = new QCheckBox( placeholder, "UseGPU" );
            m_use_gpu_qcheckbox->setText( m_pJt->tr("Use GPU") );
            g->addWidget( m_use_gpu_qcheckbox, 1, 0 );
            connect( m_use_gpu_qcheckbox,  SIGNAL(clicked()), this, SLOT( setGpuSelect() ) );

            m_pSelectFPSLabel = new QLabel( placeholder, "FPSPushButtonLabel" );
            m_pSelectFPSLabel->setAlignment( int( QLabel::AlignCenter ) );
            m_pSelectFPSLabel->setText( m_pJt->tr( "Playback Speed" ) );
            g->addWidget( m_pSelectFPSLabel, 0, 1 );

            m_pSelectFPSPushbutton = new QPushButton( placeholder, "FPSPushButton" );
            m_pSelectFPSPushbutton->setText( QString( getFramesPerSecondName(FRAMES_PER_SECOND_30).data() ) );
            connect(m_pSelectFPSPushbutton, SIGNAL( pressed() ), SLOT( slotChooseFramesPerSecond() ) );
            g->addWidget( m_pSelectFPSPushbutton, 1, 1 );
        }

        mainLayout->addStretch();

        {
            QGridLayout* g = new QGridLayout( 2, 2 );
            mainLayout->addLayout( g );
            
            QLabel* sframetext = new QLabel( placeholder, "sframetext" );
            g->addWidget( sframetext, 0, 0, Qt::AlignVCenter );
            sframetext->setText( tr( m_pJt->tr("STARTFRAME") ) );
            sframetext->setAlignment( int( QLabel::AlignRight ) );
            
            m_pStartFrameControl = new SupergRangeControl( placeholder, "startframe" );
            m_pStartFrameControl->setFormatter( TimecodeValueFormatter() );
            g->addWidget( m_pStartFrameControl, 0, 1 );
            JahFormatter::configure( m_pStartFrameControl, m_nAStartFrame, m_nAStartFrame );
            connect( m_pStartFrameControl, SIGNAL(valueChanged(int)),   SLOT(updatesliderStartframe(int)) );
            
            QLabel* eframetext = new QLabel( placeholder, "eframetext" );
            g->addWidget( eframetext, 1, 0, Qt::AlignVCenter );
            eframetext->setText( tr( m_pJt->tr("ENDFRAME")  ) );
            eframetext->setAlignment( int( QLabel::AlignRight ) );
            
            m_pEndFrameControl = new SupergRangeControl( placeholder, "endframe" );
            m_pEndFrameControl->setFormatter( TimecodeValueFormatter() );
            g->addWidget( m_pEndFrameControl, 1, 1 );
            JahFormatter::configure( m_pEndFrameControl, m_nAEndFrame, m_nAStartFrame );
            connect( m_pEndFrameControl, SIGNAL(valueChanged(int)),   SLOT(updatesliderEndframe(int)) );
        }

        // add a spacer to shuffle up
        mainLayout->addStretch();
        top->addStretch();
    }

    ///////////////////////////////////////////////
    ControlFrameUI[3][0] = new QFrame( moduleOptionsUI[3], "ControlFrame2" ); //was ControlFrame2
    ControlFrameUI[3][0]->setGeometry( QRect( 0, 0, 660, 190 ) );

    createAxisPanel ( ControlFrameUI[3][0] );
}


///////////////////////////////////////////////
//custom keyer panel

void GLKeyer::createKeyerPanel ( QFrame* theuiframe)
{
    KeyerColorWidget* color_widget = new KeyerColorWidget( theuiframe );
    setKeyerColorWidget(color_widget);
    getKeyerColorWidget()->show();

    connect( getKeyerColorWidget()->colorDropper, SIGNAL( clicked() ), this, SLOT( showColorDropper() ) );
	
    connect ( getKeyerColorWidget()->sliderH, SIGNAL(valueChanged(int)), this, SLOT( setHueRange(int) ) );
    connect ( getKeyerColorWidget()->sliderS, SIGNAL(valueChanged(int)), this, SLOT( setSaturationRange(int) ) );
    connect ( getKeyerColorWidget()->sliderV, SIGNAL(valueChanged(int)), this, SLOT( setValueRange(int) ) );
    connect ( getKeyerColorWidget()->softenSlider, SIGNAL(valueChanged(int)), this, SLOT( setSoftenValue(int) ) );
    connect ( getKeyerColorWidget()->softenSlider, SIGNAL(valueChanged(int)), this, SLOT( setXScale(int) ) );

    // Previously we used 3 connections to spin boxes, but actually better to use 1 connection to triangle insted.
    //  connect ( getKeyerColorWidget()->spinBoxH, SIGNAL(valueChanged(int)), this, SLOT( setHueBase(int) ) );
    //  connect ( getKeyerColorWidget()->spinBoxS, SIGNAL(valueChanged(int)), this, SLOT( setSaturationBase(int) ) );
    //  connect ( getKeyerColorWidget()->spinBoxV, SIGNAL(valueChanged(int)), this, SLOT( setValueBase(int) ) );

    QtColorTriangle *qtColorTriangle = getKeyerColorWidget()->qtColorTriangle;
    connect( (QObject*)(qtColorTriangle->triangle), SIGNAL( colorChangedExternal(const QColor&)), 
        this, SLOT( setColorBase( const QColor& )));

    connect ( getKeyerColorWidget()->showAlphaCheckBox, SIGNAL( clicked() ), this, SLOT( createAlphaMask() )  );
    connect ( getKeyerColorWidget()->selectColorsIndividuallyCheckBox, SIGNAL( clicked() ), this, SLOT( setSelectColorsIndividually() )  );
}

void GLKeyer::createMenuItem( QPopupMenu * themenu )
{
	Q_CHECK_PTR(themenu);
	//themenu->insertItem("&"+m_pJt->tr("Add Object"),this, SLOT(addLayer()) );
 //   themenu->insertItem("&"+m_pJt->tr("Name Object"),this,SLOT(nameLayer()) );
 //   themenu->insertItem("&"+m_pJt->tr("Delete Object"),this,SLOT(delLayer()) );
    themenu->insertSeparator();
	//make this a popup 'Move'
    //themenu->insertItem("&"+m_pJt->tr("Move Up"),this, SLOT(moveLayerUp()) );
    //themenu->insertItem("&"+m_pJt->tr("Move Down"),this, SLOT(moveLayerDown()) ); 
}


























