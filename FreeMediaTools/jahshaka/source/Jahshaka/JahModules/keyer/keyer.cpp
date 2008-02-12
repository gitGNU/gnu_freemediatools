/*******************************************************************************
**
** The source file for the Jahshaka animation module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "keyer.h"
#include <glworldlists.h>
#include <glcore.h>
#include <qcolor.h>
#include <qslider.h>
#include <qlabel.h>
#include "keyercolorwidget.h"
#include "colordropper.h"
#include "gpumathlib.h"
#include <InputLCD.h>
#include <supergrangecontrol.h>
#include "qtcolortriangle.h"
#include <qlistview.h>
#include <jahpreferences.h>



void GLKeyer::start( )
{
	buildInterface(m_controls);
	buildInterfaceMenus(m_leftcontroller,m_rightcontroller);
	initializeObjects();

	// Update sliders to reflected selected fps
	setResolution( JahPrefs::getInstance().getJahResolution() );
}

void GLKeyer::initializeObjects()
{    
    m_pJTrace->info( "Initializing GLKeyer");

    //make the current opengl context current so the textures can be set up properly
    makeCurrent();    
    
    ///////////////////////////////////////////////
    //this creates the default world layer using the linked list
    JahLayer* jah_layer = configureLayer ( "World", false, true, false, true );
    buildLayer(jah_layer, "World", LayerCategory::LAYER, ObjectCategory::BACKDROP );
    m_chroma_world_layer = getLayerList()->last()->getJahLayer();
    QCheckListItem* new_q_check_list_item = new QCheckListItem(m_pLayerListView, "World");
    m_chroma_world_layer->setCheckListItem(new_q_check_list_item);
    m_chroma_world_layer->getCheckListItem()->setVisible(false);

	jah_layer = configureLayer ( "Base Layer", true, true, true, true );
	buildLayer(jah_layer, "Base Layer", LayerCategory::LAYER, ObjectCategory::MESHLAYER );  
    m_pChromaBaseLayer = getLayerList()->getLast()->getJahLayer();

    initializeListview(m_pChromaBaseLayer, true);

	jah_layer = configureLayer ( "Key Layer", true, true, true, true );
    buildLayer(jah_layer, "Key Layer", LayerCategory::LAYER, ObjectCategory::MESHLAYER );  
    m_pChromaKeyLayer = getLayerList()->getLast()->getJahLayer();

    initializeListview(m_pChromaKeyLayer, true);

	//call objectmenu to update ui and then update sliders
	changeSelection(0);
    updateSliders();

    /////////////////////////////////////////////////////
    //initialize the buffer used for effect outputimage buffering
    //configureBuffer(1); 

    //turn forceplay on here
    setForcePlay(true);

    //////////////////////////////////////////////////////
    // added for custom variables
    initializeVariables();

    //////////////////////////////////////////////////////
    //sets up the m_pCore functionality
    configureModuleEngine();

    //////////////////////////////////////////////////////
    //set the ui tool
    noTool();

	////////////////////////////////////////////
    //use source size renders
    VideoRenderSelect->setChecked(true);
	m_bRenderAtImageResolution = true;


	for (int x=7; x<=8; x++)
	{
        m_aJahSliderLabels[x]->hide();
#ifndef NEW_SLIDERS
        m_aJahSliderLCDs[x]->hide();
#endif
        m_aJahSliders[x]->hide();
	}

	/*
    m_aJahSliderLabels[7]->hide();
	m_aJahSliderLCDs[7]->hide();
	m_aJahSliders[7]->hide();

    m_aJahSliderLabels[8]->hide();
	m_aJahSliderLCDs[8]->hide();
	m_aJahSliders[8]->hide();
	*/

    setSliderValue(9, 100);
    m_fSoftenValue = 0.0f;
}


void GLKeyer::initializeVariables(void)
{
    static bool initVariables = true;

    if (initVariables)
    {
        #define PI                  3.14159265358979323846
        #define VIEW_ANGLE			45.0
        #define VIEW_ANGLE_RADIANS 	((VIEW_ANGLE / 180.0) * PI)

        m_fCameraDistance = ((float)m_nY_RESOLUTION / 2.0) / tan(VIEW_ANGLE_RADIANS / 2.0);

        m_bSelectColorsIndividually = false;

    }
    initVariables = false;

    //customizations
    m_bTexturesInitialized = false;
    m_bUseTexturesForReadback = false;
    m_pGPUSelect = false;
}

//this routine is used to configure the modules engine and m_pCore functionality
void GLKeyer::configureModuleEngine(void)    
{ 
	//////////////////////////////////////////////////////
    //configure the color correction mode
    
    EffectLayer* new_effect_layer;

	//if (!( m_bGPUSupport && nv_m_bGPUSupport) )
	if ( jahstd::glslSupport() )
	{
	  m_bGPUSelect = true;
	  QString title = "GPU Keyer";
	  std::string guid = "";
	  new_effect_layer = addEffectLayer(m_pChromaKeyLayer->getLayerListEntry(), title, EffectInfo::CHROMA_KEYER_GLSL_SEPARATE_ALPHA_TYPE, -1, guid);
	  
	  if ( m_use_gpu_qcheckbox->isChecked() )
	    {
	    }
	  else
		{
			m_use_gpu_qcheckbox->toggle();
		}
    }
	else
	{
        m_pGPUSelect = false;
		QString title = "CPU Keyer";
        std::string guid = "";
        new_effect_layer = addEffectLayer(getLayerList()->at(1), title, EffectInfo::CHROMA_KEYER_CPU_TYPE, -1, guid);

		if ( m_use_gpu_qcheckbox->isChecked() )
		{
			m_use_gpu_qcheckbox->toggle();
		}
		else
		{
		}
	}

    m_pChromaSettingsLayer = new_effect_layer->getJahLayer();
    setCurrentEffectLayer(m_pChromaSettingsLayer);
    m_pChromaSettingsLayer->setEffectsSlidersHaveChanged(true);
    m_bSlidersHaveChanged = true;
    
}

void GLKeyer::objectMenu(int menu)
{

    changeSelection(menu);
}

//////////////////////////////////////////
// need to fix missing command below makeTexture
//////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////

void GLKeyer::addClip(assetData clip)
{
    if (getActiveJahLayer() == m_pChromaKeyLayer || getActiveJahLayer() == m_pChromaSettingsLayer)
    {
        m_pChromaKeyLayer->assignAsset(clip, VideoCategory::IMAGE);
        m_pChromaKeyLayer->LoadLayerAsset(VideoCategory::IMAGE);
        m_pChromaKeyLayer->makeTexture();

        //set renderspace imagesize here...
        m_pCore->imagewidth  = m_pChromaKeyLayer->getImageWidth();
        m_pCore->imageheight = m_pChromaKeyLayer->getImageHeight();
    }
    else if (getActiveJahLayer() == m_pChromaBaseLayer)
    {
        m_pChromaBaseLayer->assignAsset(clip, VideoCategory::IMAGE);
        m_pChromaBaseLayer->LoadLayerAsset(VideoCategory::IMAGE);
        m_pChromaBaseLayer->makeTexture();
    }
    else
    {
        return;
    }

    if (clip.endframe > m_nMaxFrame)
    {
        m_nMaxFrame = clip.endframe;
        updatesliderEndframe(m_nMaxFrame);
        m_pEndFrameControl->setValue(m_nMaxFrame);
    }

    updateSliders();

    setSlidersHaveChanged(true);;
    
    //update start and end points to the start and end points of the source clip
    updatesliderStartframe (  clip.startframe);
    updatesliderEndframe(  clip.endframe);
    updateGL();
}

void GLKeyer::addBaseClip(assetData clip)
{
    m_pChromaBaseLayer->assignAsset(clip, VideoCategory::IMAGE);
    m_pChromaBaseLayer->LoadLayerAsset(VideoCategory::IMAGE);
    m_pChromaBaseLayer->makeTexture();

    //forceupdatefxbuffer = true;

    //initialize the buffer
    //if (useBuffering)
   //     initBuffer(&getLayerList()->at(0)->thelayer->textureData.objectImage);

    updateGL();
    updateSliders();

    setSlidersHaveChanged(true);;
    
    //update start and end points to the start and end points of the source clip
    updatesliderStartframe (  clip.startframe);
    updatesliderEndframe(  clip.endframe);

    //update the ui settings
    m_pStartFrameControl->setRange( clip.startframe, clip.endframe-1 );
    m_pStartFrameControl->setValue(clip.startframe);

    m_pEndFrameControl->setRange( clip.startframe+1, clip.endframe );
    m_pEndFrameControl->setValue(clip.endframe);
}

void 
GLKeyer::setBackgroundStatus(void)
{

	bool backStatus = backgroundSelect->isChecked();

	JahLayer* jah_layer = getLayerList()->at(0)->thelayer;

	jah_layer->layerStatus = backStatus;
}


void 
GLKeyer::layerClicked( QListViewItem* item )
{
    if (!item) 
    {
        return;
    }

    LayerListEntry* layer_list_entry = findLayerListEntry(item);

    if (!layer_list_entry)
    {
        return;
    }

	JahLayer* jah_layer = layer_list_entry->thelayer;
    setActiveJahLayer(jah_layer);
    QCheckListItem* check_list_item = (QCheckListItem*)jah_layer->getCheckListItem();
    jah_layer->layernodes->m_node->layer_enabled = check_list_item->isOn();

    if (jah_layer == m_pChromaKeyLayer)
    {
        showSliders(9);
        setDefaultSliderText();
    }
    else if (jah_layer == m_pChromaSettingsLayer)
    {
        hideSliders(9);
        showSliders(7);
        setColorLimitSliderLabels();
    }
    else if (jah_layer == m_pChromaBaseLayer)
    {
        showSliders(9);
        setDefaultSliderText();
    }

    if ( jah_layer && ((QCheckListItem*)item)->isOn() )
    {
        jah_layer->layerStatus = true;
    }
    else
    {
        jah_layer->layerStatus = false;
    }

    updateKeyframeDisplay();
    updateSliderValues();
    updateSliders();
    updateGL();
}

void GLKeyer::setColorBase( const QColor& color )
{
    //qDebug("setColorBase"); 
    int h, s, v; 
    color.getHsv( h, s, v );
     m_fHSVBase.x = float(h) / 359.0f;
     m_fHSVBase.y = float(s) / 255.0f;
     m_fHSVBase.z = float(v) / 255.0f;
     setRgbaValues();
}

void 
GLKeyer::setHueBase(int value)
{
    m_fHSVBase.x = float(value) / 359.0f;
    setRgbaValues();
}

void 
GLKeyer::setSaturationBase(int value)
{
    m_fHSVBase.y = float(value) / 255.0f;
    setRgbaValues();
}

void 
GLKeyer::setValueBase(int value)
{
    m_fHSVBase.z = float(value) / 255.0f;
    setRgbaValues();
}

void 
GLKeyer::setHueRange(int value)
{
    m_fHSBRange.x = float(value);
    setRgbaValues();
}

void 
GLKeyer::setSaturationRange(int value)
{
    m_fHSBRange.y = float(value);
    setRgbaValues();
}

void 
GLKeyer::setSoftenValue(int value)
{
    m_fSoftenValue = float(value) / 255.0f;
    m_aJahSliderLCDs[6]->setValue(value);
    updateGL();
}

void 
GLKeyer::setValueRange(int value)
{
    m_fHSBRange.z = float(value);
    setRgbaValues();
}

void
GLKeyer::setRgbaValues()
{
    QColor min_color = m_keyer_color_widget->minColor();
    QColor max_color = m_keyer_color_widget->maxColor();

    setActiveJahLayer( getChromaSettingsLayer() );

    float4 hsv_low;
    float4 hsv_high;
    float4 rgb_low;
    float4 rgb_high;

    int red_low;
    int green_low;
    int blue_low;

    int red_high;
    int green_high;
    int blue_high;

    int hue_low;
    int saturation_low;
    int value_low;
    int hue_high;
    int saturation_high;
    int value_high;

    min_color.getHsv(&hue_low, &saturation_low, &value_low);
    hsv_low.x = float(hue_low) / 360.0f;
    hsv_low.y = float(saturation_low) / 255.0f;
    hsv_low.z = float(value_low) / 255.0f;

    max_color.getHsv(&hue_high, &saturation_high, &value_high);
    hsv_high.x = float(hue_high) / 360.0f;
    hsv_high.y = float(saturation_high) / 255.0f;
    hsv_high.z = float(value_high) / 255.0f;

    find_rgb_range_from_hsv(hsv_low, hsv_high, rgb_low, rgb_high);

    red_low = int(rgb_low.x * 255.0f);
    green_low = int(rgb_low.y * 255.0f);
    blue_low = int(rgb_low.z * 255.0f);

    red_high = int(rgb_high.x * 255.0f);
    green_high = int(rgb_high.y * 255.0f);
    blue_high = int(rgb_high.z * 255.0f);

    setXRotation( red_low );
    setYRotation( green_low );
    setZRotation( blue_low );

    setXTrans( red_high );
    setYTrans( green_high );
    setZTrans( blue_high );

    updateSliders();
}

void 
GLKeyer::createAlphaMask(void)
{
    if ( getKeyerColorWidget()->showAlphaCheckBox->isChecked() )
    {
        getCurrentEffectLayer()->setCreateChromaAlphaMask(true);
    }
    else
    {
        getCurrentEffectLayer()->setCreateChromaAlphaMask(false);
    }

    updateGL();
}

void
GLKeyer::setColorLimitSliderLabels()
{
    m_aJahSliderLabels[0]->setText("Red Low");
    m_aJahSliderLabels[1]->setText("Green Low");
    m_aJahSliderLabels[2]->setText("Blue Low");

    m_aJahSliderLabels[3]->setText("Red High");
    m_aJahSliderLabels[4]->setText("Green High");
    m_aJahSliderLabels[5]->setText("Blue High");
    m_aJahSliderLabels[6]->setText("Soften");
}

void 
GLKeyer::setSelectColorsIndividually(void)
{
    layerClicked( m_pChromaSettingsLayer->getCheckListItem() );
    JahLayer* active_layer = getActiveJahLayer();
    m_bSelectColorsIndividually = getKeyerColorWidget()->selectColorsIndividuallyCheckBox->isChecked();
    active_layer->setSelectColorsIndividually(m_bSelectColorsIndividually);
    
    updateGL();
}

void GLKeyer::showColorDropper()
{
    ColorDropper* dropper = new ColorDropper();
    dropper->setFixedSize( 128, 128 );
    getKeyerColorWidget()->qtColorTriangle->connect( dropper, SIGNAL( colorPicked( const QColor& ) ), 
                                                     SLOT( setColor( const QColor& ) ) );
    dropper->show();
}



float4&         GLKeyer::getRgbaLow() { return m_fRGBALow; }
    float4&         GLKeyer::getRgbaHigh() { return m_fRGBAHigh; }
    void            GLKeyer::setRgbaLow(float4& value) { m_fRGBALow = value; }
    void            GLKeyer::setRgbaHigh(float4& value) { m_fRGBAHigh = value; }
    float4&         GLKeyer::getHsvBase() { return m_fHSVBase; }
    void            GLKeyer::setHsvBase(float4& value) { m_fHSVBase = value; }
    float4&         GLKeyer::getHsvRange() { return m_fHSBRange; }
    void            GLKeyer::setHsvRange(float4& value) { m_fHSBRange = value; }
    float4&         GLKeyer::getRgbaRange() { return m_fRGBARange; }
    void            GLKeyer::setRgbaRange(float4& value) { m_fRGBARange = value; }
    void            GLKeyer::setRgbaValues();

    KeyerColorWidget*   GLKeyer::getKeyerColorWidget() { return m_keyer_color_widget; }
    void                GLKeyer::setKeyerColorWidget(KeyerColorWidget* widget) { m_keyer_color_widget = widget; }


	GLKeyer::GLKeyer( GLCore* m_pCore, const char* name, QHBox* controls,
            QHBox* controller,QHBox* leftcontroller, QHBox* rightcontroller,
            int* globalm_nClipnumber )
		: GLWorld(m_pCore, name, controller, globalm_nClipnumber )
		, m_controls( controls )
		, m_leftcontroller( leftcontroller )
		, m_rightcontroller( rightcontroller )
	{
	}

GLKeyer::~GLKeyer() 
{  

}

void GLKeyer::grabDesktop(void)      { emit GrabClip();      };

void GLKeyer::setDefaultAxisSliderLabels()
{

}
    void GLKeyer::grabDesktopBaselayer(void)   { emit GrabBaseClip();   };

    JahLayer*               GLKeyer::getChromaWorldLayer() { return m_chroma_world_layer; }
    JahLayer*               GLKeyer::getChromaBaseLayer() { return m_pChromaBaseLayer; }
    JahLayer*               GLKeyer::getChromaKeyLayer() { return m_pChromaKeyLayer; }
    JahLayer*               GLKeyer::getChromaSettingsLayer() { return m_pChromaSettingsLayer; }
    bool                    GLKeyer::getSelectColorsIndividually() { return m_bSelectColorsIndividually; }
    bool                    GLKeyer::getGpuSelect() { return m_pGPUSelect; }
    GLuint*                 GLKeyer::getBaseTextureIDPtr() { return &m_hBaseTextureID; }
    GLuint                  GLKeyer::getBaseTextureID() { return m_hBaseTextureID; }
    GLuint*                 GLKeyer::getKeyTextureIDPtr() { return &m_hKeyTextureID; }
    GLuint                  GLKeyer::getKeyTextureID() { return m_hKeyTextureID; }
    bool                    GLKeyer::getTexturesInitialized() { return m_bTexturesInitialized; }
