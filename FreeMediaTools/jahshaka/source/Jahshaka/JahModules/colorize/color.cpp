/*******************************************************************************
**
** The source file for the Jahshaka color module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "color.h"
#include <glworldlists.h>
#include <glcore.h>
#include <InputLCD.h>
#include <supergrangecontrol.h>
#include <qlabel.h>
#include <jahpreferences.h>

void GLColorize::start( )
{
	buildInterface(m_controls);
	buildInterfaceMenus(m_leftcontroller,m_rightcontroller);
	initializeObjects();

	// Update sliders to reflected m_bSelected fps
	setResolution( JahPrefs::getInstance().getJahResolution() );
}

void GLColorize::initializeObjects()
{    
    m_pJTrace->info( "Initializing GLColor Class");

    //make the current opengl context current so the textures can be set up properly
    makeCurrent();
	
    ///////////////////////////////////////////////////////////////////
	// going to replace this code with a routine to do it for us

	JahLayer* jah_layer = configureLayer ( "World", false, true, false, true );
    buildLayer(jah_layer, "World", LayerCategory::LAYER, ObjectCategory::BACKDROP );

	jah_layer = configureLayer ( "Image Layer", true, true, true, true );
	buildLayer(jah_layer, "Image Layer", LayerCategory::LAYER, ObjectCategory::MESHLAYER );  

    setImageLayer( getLayerList()->last()->getJahLayer() );

	//setup/customize listviewitem for layer 1
	initializeListview(1,TRUE);

	//call objectmenu to update ui and then update sliders
    objectMenu(getNumberOfLayers() - 1);
	updateSliders();

    /////////////////////////////////////////////////////
    //initialize the buffer used for effect outputimage buffering
    //configureBuffer(1); 

    //turn forceplay on here
    setForcePlay(true);

    //////////////////////////////////////////////////////
    //added for custom variables that need to be set up 
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

    m_aJahSliderLabels[0]->setText("Red");
    m_aJahSliderLabels[1]->setText("Green");
    m_aJahSliderLabels[2]->setText("Blue");

    m_aJahSliderLabels[3]->setText("Tint");
    m_aJahSliderLabels[4]->setText("Brightness");
    m_aJahSliderLabels[5]->setText("Strength");

    m_aJahSliderLabels[6]->setText("Hue");
    m_aJahSliderLabels[7]->setText("Saturation");
    m_aJahSliderLabels[8]->setText("Value");

    m_aJahSliderLabels[9]->setText("Gamma");

    setSliderValue(9, 100);
}

void GLColorize::initializeVariables(void)
{
    //this is a flag to make sure this routine isnt run
    //more than one time... not sure if this is really
    //needed anyore

    static bool initVariables = true;

    //this is used for the gpu effects
    // The m_pCamera distance calculation should be passed in from the GL resize
    // routine.  This is a HACK which should be fixed FIXME

    if (initVariables)
    {
        #define PI                  3.14159265358979323846
        #define VIEW_ANGLE			45.0
        #define VIEW_ANGLE_RADIANS 	((VIEW_ANGLE / 180.0) * PI)

        m_fCameraDistance = ((float)m_nY_RESOLUTION / 2.0) / tan(VIEW_ANGLE_RADIANS / 2.0);

	m_bContrastTextureInitialized = FALSE;
    }

    initVariables = false;
}

//this routine is used to configure the modules engine and m_pCore functionality
void GLColorize::configureModuleEngine(void)    
{ 
	//////////////////////////////////////////////////////
    //configure the color correction mode

    std::string guid = "";

	if ( m_bNV_GPUSupport ) // || m_bGPUSupport ) //nvidia support for the color corrector
    {
        std::string title = "Colorize GPU";
        addEffectLayer( getImageLayer(), title, EffectInfo::COLORIZE_GPU_TYPE, -1, guid); 

	  m_gpu_select_qcheckbox->toggle();
    }
    else 
	{
        std::string title = "Colorize CPU";
        addEffectLayer(getImageLayer(), title, EffectInfo::COLORIZE_CPU_TYPE, -1, guid);
    }

    getImageLayer()->setEffectsSlidersHaveChanged(true);
    m_bSlidersHaveChanged = true;
}


//////////////////////////////////////////////////
// adds a new layer to the listview and to the system
void GLColorize::objectMenu(int menu)
{
    changeSelection(menu);
}

//////////////////////////////////////////
// need to fix missing command below makeTexture
//////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////

void GLColorize::addClip(assetData clip)
{
    if (clip.endframe > m_nMaxFrame)
    {
        m_nMaxFrame = clip.endframe;
        updatesliderEndframe(m_nMaxFrame);
        m_pEndFrameControl->setValue(m_nMaxFrame);
    }

    getImageLayer()->assignAsset(clip, VideoCategory::IMAGE);
    getImageLayer()->LoadLayerAsset(VideoCategory::IMAGE);
    getImageLayer()->makeTexture();

    //set renderspace imagesize here...
    m_pCore->imagewidth  = getImageLayer()->getImageWidth();
    m_pCore->imageheight = getImageLayer()->getImageHeight();

    //forceupdatefxbuffer = true;

    //initialize the buffer
   // if (useBuffering)
   //     initBuffer(&getImageLayer()->textureData.objectImage);

    updateGL();
    updateSliders();

    m_bSlidersHaveChanged = true;
    
    //update start and end points to the start and end points of the source clip
    updatesliderStartframe (  clip.startframe);

    if (   getImageLayer()->asset.theType == VideoType::MOVIE 
        || getImageLayer()->asset.theType == VideoType::SEQUENCE
       ) 
    {
        updatesliderEndframe(  clip.endframe);
    }

    //update the ui settings
    //m_pStartFrameControl->setMinMaxInt( clip.startframe, clip.endframe-1 );
    m_pStartFrameControl->setValue(clip.startframe);

    //m_pEndFrameControl->setMinMaxInt( clip.startframe+1, clip.endframe );
    m_pEndFrameControl->setValue(clip.endframe);

}

void
GLColorize::gpuSelect()
{
    bool m_bSelected = getGpuSelectCheckbox()->isOn();
    EffectLayer* effect_layer = getImageLayer()->getEffectsList()->first();

    if (m_bSelected)
    {
        effect_layer->setType(EffectInfo::COLORIZE_GPU_TYPE);
        getImageLayer()->layername = "Colorize GPU";
    }
    else
    {
        effect_layer->setType(EffectInfo::COLORIZE_CPU_TYPE);
        getImageLayer()->layername = "Colorize CPU";
    }

    updateGL();
}

QCheckBox*
GLColorize::getGpuSelectCheckbox()
{
    return m_gpu_select_qcheckbox;
}

    

