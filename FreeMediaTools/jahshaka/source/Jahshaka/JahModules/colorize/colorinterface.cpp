/*******************************************************************************
**
** The source file for the Jahshaka color interface module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "color.h"
#include "colortriangle.h"
#include "qtcolortriangle.h"
#include <InputLCD.h>
#include <supergrangecontrol.h>
#include <jahformatter.h>
#include <valueFormatters.h>
#include <jahtranslate.h>
#include <qlistview.h>
#include <openmedialib.h>
#include <qframe.h>
#include <qhbox.h>
#include <qslider.h>
#include <qlabel.h>
#include <widget.h>

void GLColorize::buildInterfaceMenus( QHBox* , QHBox* rightcontroller) 
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

void GLColorize::buildInterface( QHBox* f ) 
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
    LoadButton->setText( m_pJt->tr("Load") );
    LayersFrameLayout->addWidget( LoadButton );
    connect  ( LoadButton,  SIGNAL(clicked()), this, SLOT( SceneLoad())  );

    SaveButton = new QPushButton( LayersFrame, "SaveButton" );
    SaveButton->setText( m_pJt->tr("Save") );
    LayersFrameLayout->addWidget( SaveButton );
    connect  ( SaveButton,  SIGNAL(clicked()), this, SLOT( sceneSave())  );

/* 
    ImportFxButton = new QPushButton( ControlFrameUI[0][0], "ImportFXButton" );
    JahFormatter::addButton( ImportFxButton, 110, 100, 86, 25 , m_pJt->tr("Import FX") );
    connect  ( ImportFxButton,  SIGNAL(clicked()), this, SLOT( importFx())  );

    ExportFxButton = new QPushButton( ControlFrameUI[0][0], "ImportFXButton" );
    JahFormatter::addButton( ExportFxButton, 110, 130, 86, 25 , m_pJt->tr("Export FX") );
    connect  ( ExportFxButton,  SIGNAL(clicked()), this, SLOT( saveEffects())  );
*/

    ResetButton = new QPushButton( LayersFrame, "ResetButton" );
    ResetButton->setText( m_pJt->tr("Reset") );
    LayersFrameLayout->addWidget( ResetButton );
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
    moduleOptionsUI[0] = new QHBox( EffectsFrame, "colorize" ); //keycontrols
    tabframe->insertTab( moduleOptionsUI[0], tr( "  "+m_pJt->tr("COLOR")+"  " ) );

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

    //makes sure to raise the first tab
    tabframe->setCurrentPage( 0 );

    /////////////////////////////////////////////////////////////////////
    // the layout boxes for different types of layers
    // the object control tab bar
    // this is the object control panel

    ///////////////////////////////////////////////////////////////
    // object layer options sub control set
    m_bHasButtons = false;

    ///////////////////////////////////////////////////////////////
    // create the color correction controls

    ControlFrameUI[0][0] = new QFrame( moduleOptionsUI[0], "ControlFrame2" ); //was ControlFrame2
    ControlFrameUI[0][0]->setGeometry( QRect( 0, 0, 660, 190 ) );

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

        QBoxLayout* buttonLayout = new QVBoxLayout();
        mainLayout->addLayout( buttonLayout );

        m_gpu_select_qcheckbox = new QCheckBox( placeholder, "GpuSelect" );
        m_gpu_select_qcheckbox->setText( m_pJt->tr("Use GPU") );
        buttonLayout->addWidget( m_gpu_select_qcheckbox );
        connect( m_gpu_select_qcheckbox,  SIGNAL(clicked()), this, SLOT( gpuSelect() )  );

        VideoRenderSelect = new QCheckBox( placeholder, "SourceRes" );
        VideoRenderSelect->setText( m_pJt->tr("render using source res") );
        buttonLayout->addWidget( VideoRenderSelect );
        connect( VideoRenderSelect,  SIGNAL(clicked()), this, SLOT(changeVideoRender())  );

        QGridLayout* g = new QGridLayout( 2, 2 );
        mainLayout->addLayout( g );

        QLabel* sframetext = new QLabel( ControlFrameUI[0][0], "sframetext" );
        g->addWidget( sframetext, 0, 0, Qt::AlignVCenter );
        sframetext->setText( tr( m_pJt->tr("STARTFRAME") ) );
        sframetext->setAlignment( int( QLabel::AlignRight ) );
        
        m_pStartFrameControl = new SupergRangeControl( ControlFrameUI[0][0], "startframe" );
        m_pStartFrameControl->setFormatter( TimecodeValueFormatter() );
        g->addWidget( m_pStartFrameControl, 0, 1 );
        JahFormatter::configure( m_pStartFrameControl, m_nAStartFrame, m_nAStartFrame );
        connect( m_pStartFrameControl, SIGNAL(valueChanged(int)),   SLOT(updatesliderStartframe(int)) );
        
        QLabel* eframetext = new QLabel( ControlFrameUI[0][0], "eframetext" );
        g->addWidget( eframetext, 1, 0, Qt::AlignVCenter );
        eframetext->setText( tr( m_pJt->tr("ENDFRAME")  ) );
        eframetext->setAlignment( int( QLabel::AlignRight ) );
        
        m_pEndFrameControl = new SupergRangeControl( ControlFrameUI[0][0], "endframe" );
        m_pEndFrameControl->setFormatter( TimecodeValueFormatter() );
        g->addWidget( m_pEndFrameControl, 1, 1 );
        JahFormatter::configure( m_pEndFrameControl, m_nAEndFrame, m_nAStartFrame );
        connect( m_pEndFrameControl, SIGNAL(valueChanged(int)),   SLOT(updatesliderEndframe(int)) );

        // add a spacer to shuffle up
        mainLayout->addStretch();
        top->addStretch();
    }

    ///////////////////////////////////////////////
    ControlFrameUI[3][0] = new QFrame( moduleOptionsUI[3], "ControlFrame2" ); //was ControlFrame2
    ControlFrameUI[3][0]->setGeometry( QRect( 0, 0, 660, 190 ) );

    createAxisPanel ( ControlFrameUI[3][0] );

    m_aJahSliders[9]->setMinValue(1);
    m_aJahSliders[9]->setMaxValue(500);
    m_aJahSliders[9]->show();
    m_aJahSliderLCDs[9]->setMinInt(1);
    m_aJahSliderLCDs[9]->setMaxInt(500);
    m_aJahSliderLCDs[9]->show();
    m_aJahSliderLabels[9]->setText("Gamma");
    m_aJahSliderLabels[9]->show();

#if 0
    connect( m_aJahSliders[9], SIGNAL(valueChanged(int)), m_aJahSliderLCDs[9], SLOT( setValue(int) ) );
    connect( m_aJahSliderLCDs[9], SIGNAL(valueChanged(int)), m_aJahSliders[9], SLOT( setValue(int) ) );
#endif

    for (int i = 0; i < 10; i++)
    {
        m_aJahSliders[i]->hide();
        m_aJahSliderLCDs[i]->hide();
        m_aJahSliderLabels[i]->hide();
    }

    m_color_corrector_widget = createColorPanel ( ControlFrameUI[0][0] );
}


///////////////////////////////////////////////
//custom color panel

ColorCorrectorWidget*
GLColorize::createColorPanel ( QFrame* theuiframe)
{
	ccw = new ColorCorrectorWidget( theuiframe );
	ccw->show();

	
    connect ( ccw->redSlider, SIGNAL(valueChanged(int)), this, SLOT(setRed(int)) );
    connect ( ccw->greenSlider, SIGNAL(valueChanged(int)), this, SLOT(setGreen(int)) );
    connect ( ccw->blueSlider, SIGNAL(valueChanged(int)), this, SLOT(setBlue(int)) );
    connect ( ccw->gammaSlider, SIGNAL(valueChanged(int)), this, SLOT( setAlpha(int) ) );
    connect ( ccw->gammaSlider, SIGNAL(valueChanged(int)), m_aJahSliders[9], SLOT( setValue(int) ) );

    connect ( ccw->hSlider, SIGNAL(valueChanged(int)), this, SLOT(setHue(int)) );
    connect ( ccw->sSlider, SIGNAL(valueChanged(int)), this, SLOT(setSaturation(int)) );
    connect ( ccw->vSlider, SIGNAL(valueChanged(int)), this, SLOT(setValue(int)) );

    connect ( ccw->resetButton, SIGNAL( clicked() ), this, SLOT( resetAllSliders() ) );

    // Brightness is not represented on the color wheel, just via a slider.
    connect ( ccw->brightnessSlider, SIGNAL(valueChanged(int)), this, SLOT(setBrightness(int)) );


    // === Connections below here are for color wheel / 'triangle' 

  	ColorTriangle * coltri = ccw->qtColorTriangle->triangle; // bit ugly, but signals come from 'colortriangle' itself.

    // Make the outer donut respond to saturation slider change, via the helper slot. 
    connect(  ccw->sSlider, SIGNAL( valueChanged( int )), this, SLOT ( saturationHelperSlot( int )));  

    // Tell the outer donut to rotate if we change the hue shift slider, via a helper slot.
    connect ( ccw->hSlider, SIGNAL( valueChanged( int )), this, SLOT( hueShiftSliderHelperSlot(int)) );
    // ... and tell the slider to update when the colortri tells us (via helper for unit conversion).
    connect ( (QObject*)coltri, SIGNAL(correctorHueShiftChanged(int)), this, SLOT( hueShiftHelperSlot(int)) );
    
    // tint /offset. Tell the triangle to move the m_bSelector vector when sliders change, via a helper slot (for unit conversion).
    connect(  ccw->tintSlider, SIGNAL( valueChanged( int )), this, SLOT ( hueTintHelperSlot( int )));
    connect(  ccw->strengthSlider, SIGNAL( valueChanged( int )), this, SLOT ( hueStrengthHelperSlot( int )));
    // Tell the tint and strengh sliders to respond to changes in the color wheel ( via helper slot).
    connect ( (QObject*)coltri, SIGNAL(correctorWheelChanged(int, int)), this, SLOT( hueWheelChangeHelperSlot( int, int ) ));

    return ccw;
}

void GLColorize::hueTintHelperSlot( int value )
{
    // Slider goes from -500 to + 500, whilst hue offset expects 0-3600 (in units of 1/10th deg).
    ColorTriangle * coltri = ccw->qtColorTriangle->triangle; 
    coltri->setCorrectorHueOffset( int (  (value / 5 * 18.0) + 1800.0 ) );
    setXTrans( int (  (value / 5 * 18.0) + 1800.0 ) ); 
    //updateSliders();
}

void GLColorize::hueShiftHelperSlot( int value )
{
    // hue shift slider ('x scale slider') runs from -500 to +500, which maps to  either 1 (if value is 0) or 
    // more usually 1 + value/100. Then, in the colorize function itself, hue is taken to be (value-1)/10. Hence,
    // the final limits of hue shift are -0.5 to +0.5, which on the wheel we interpret as -180 degrees and + 180 degrees, 
    // but in reverse sense.
    // NOTE! old hSlider was additive, with clamp,  NOT a true hue rotation. This is now changed, see in color_enhance2.
    
     ccw->hSlider->setValue( int ( -500* ( float( value ) ) / 180 ) ); // converts to -500/+500 range for h slider. 
}

void GLColorize::hueStrengthHelperSlot( int value )
{    
    ColorTriangle * coltri = ccw->qtColorTriangle->triangle; 
     // For now we'll keep slider centred at 0.0 (makes 'reset' easier etc), tho strength only valid for >0.
    int str = int( (value / 5) ); // -100 to +100.
    if ( str < 0 ) str *=-1; // flip it

    coltri->setCorrectorHueStrength( str );
    setZTrans( str ); 
    //updateSliders();
}

void GLColorize::saturationHelperSlot( int value )
{
    // the s slider goes from -500 to +500 mapped to -1.0 to +3.0, with saturation *mutliplicative* in color_enhance2. 
    // Default is 1.0 (center). The outer donut defaults to saturation 255*0.75, where 0.75 was arbitrary choice to 
    // look nice (cf Combustn). It wants it's saturation value from 0-255.
    // See the conversion in GLWorld::doCPUColorize since we must do the same here.
    double satscale = double ( value / 500.0 );
    satscale *= 2.0;
    satscale += 1.0; // now its -1.0 to + 3.0, hence centred around 1.0.
    
    ColorTriangle * coltri = ccw->qtColorTriangle->triangle; 
    coltri->setCorrectorSaturation( int( 255 * satscale * 0.75 ) ); // 0.75 was arbitrary choice in colortriangle::drawDonut() too.
}

void GLColorize::hueShiftSliderHelperSlot( int value )
{   // [Note: watch out for 'cyclic' signal/slot behaviour, e.g. due to float/int conversions or rounding, a
    // pair of recipricol signal/slots can keep setting eachother.]

    // as above in hueShiftHelperSlot, for the reverse operation,  the final limits of hue shift 
    // are -0.5 to +0.5, which on the wheel we interpret as -180 degrees and + 180 degrees, but in reverse sense.
    ColorTriangle * coltri = ccw->qtColorTriangle->triangle; 
    coltri->setCorrectorHueShift( int( - value * 180.0 / 500.0 ) );
}

void GLColorize::hueWheelOffsetHelperSlot( int value )
{   // Slider goes from -500 to + 500, whilst hue offset signal sends 0-3600 (in units of 1/10th deg).
    ccw->tintSlider->setValue (  int( float(5.0*(value - 1800.0)/18.0)  ));
}

void GLColorize::hueWheelStrengthHelperSlot( int value )
{   
    // Slider goes from -500 to + 500, whilst hue strength signal sends 0-100.
    // (NB when moving the slider, we flip strength if slider goes <0).
    ccw->strengthSlider->setValue( value * 5 );
}

void GLColorize::hueWheelChangeHelperSlot( int offsetvalue, int strengthvalue )
{
    // TODO: dont want 2 updates, disconnect sliders and reconnect after??
    // disconnect(  ccw->tintSlider, SIGNAL( valueChanged( int )), this, SLOT ( hueTintHelperSlot( int )));
    // disconnect(  ccw->strengthSlider, SIGNAL( valueChanged( int )), this, SLOT ( hueStrengthHelperSlot( int )));
   
    hueWheelOffsetHelperSlot( offsetvalue );
    hueWheelStrengthHelperSlot( strengthvalue );

    // [reconnect sliders here?]  
}

void GLColorize::setBaseRGB( const QColor &col )
{
    ccw->redSlider->setValue( col.red() * 4 - 500 );
    ccw->greenSlider->setValue( col.green() * 4 - 500 );
    ccw->blueSlider->setValue( col.blue() * 4 - 500 );
}

void GLColorize::createMenuItem( QPopupMenu * themenu )
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

void 
GLColorize::setRed(int value)
{
    setXRotation(value);
    //updateSliders();
}

void 
GLColorize::setGreen(int value)
{
    setYRotation(value);
    //updateSliders();
}

void 
GLColorize::setBlue(int value)
{
    setZRotation(value);
    //updateSliders();
}

void 
GLColorize::setBrightness(int value)
{
    setYTrans(value);
    //updateSliders();
}

void 
GLColorize::setHue(int value)
{
    setXScale(value);
    //updateSliders();
}

void 
GLColorize::setSaturation(int value)
{
    setYScale(value);
    //updateSliders();
}

void 
GLColorize::setValue(int value)
{
    setZScale(value);
    //updateSliders();
}

