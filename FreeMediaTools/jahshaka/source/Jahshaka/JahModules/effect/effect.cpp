/*******************************************************************************
**
** The source file for the Jahshaka animation module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "effect.h"
#include "jahcreate.h"
#include <glworldlists.h>
#include <glcore.h>
#include <vector>
#include <qcheckbox.h>
#include <qlistview.h>
#include <qimage.h>
#include <qslider.h>
#include <InputLCD.h>
#include <supergrangecontrol.h>
#include <assetexchange.h>

void GLEffect::start( )
{
	// We need to ensure plugins are loaded here when in quick start mode
	if ( JahPrefs::getInstance().getQuickStart( ) )
		initializePlugins( );

    //build the user interface into the framework hooks handed down
    buildInterface(m_controls);
    buildInterfaceMenus(m_leftcontroller,m_rightcontroller);

    //set up a nodeframe for this module
	//nodeparent = parent1;

    initializeNodes(m_parent1);

    //initalize the module
    initializeObjects();

	// Update sliders to reflected m_bSelected fps
	setResolution( JahPrefs::getInstance().getJahResolution() );
}

void GLEffect::initializeObjects()
{    
    m_pJTrace->info( "Initializing GLEffect Class");

    //make the current opengl context current
    makeCurrent();
	
    ///////////////////////////////////////////////
    //this creates the default layers

	JahLayer* jah_layer = configureLayer ( "World", false, true, false, true );
    buildLayer(jah_layer, "World", LayerCategory::LAYER, ObjectCategory::BACKDROP );

	jah_layer = configureLayer ( "FxLayer", true, true, true, true );
	buildLayer(jah_layer, "FxLayer", LayerCategory::LAYER, ObjectCategory::MESHLAYER ); 

    setActiveJahLayer( getLayerList()->at(1)->getJahLayer() );

	//setup/customize listviewitem for layer 1
	initializeListview(1,TRUE);

    //hook up menu and sliders and default tool
    objectMenu(getNumberOfLayers() - 1);
    updateSliders();

	/////////////////////////////////////////////////////
    //initialize the buffer used for effect outputimage buffering
    //configureBuffer(1); 

    //turn forceplay on here
    setForcePlay(true);

    //////////////////////////////////////////////////////
    //added for custom variables that need to be set up 
    //only the first time around
    static bool initVariables = true;
    
	if (initVariables)
        initializeVariables();

    initVariables = false;

	//////////////////////////////////////////////////////
    //set the ui tool
    noTool();

	////////////////////////////////////////////
    //use source size renders
    VideoRenderSelect->setChecked(true);
	m_bRenderAtImageResolution = true;

    //this is where we get the global instance of the plugin class
    m_pJPlugIn = JahPluginLib::getInstance();

	//now add first layer to nodeframe
#ifdef NEWWIREUPWIDGET
	if (haswireup)
		thewireup->addNodes(getNumberOfLayers(),"Empty Layer", 
		m_qsJahBasePath + "Pixmaps/jahlayer.png");
#else
    //now add first layer to nodeframe. TODO: do image stuff in nodes code.
    m_pNodeFramer->setAssetName( "Empty Layer" );
    QImage theimage = QImage( m_qsJahBasePath + "Pixmaps/jahlayer.png"); 
    m_pNodeFramer->setMainImage( theimage );
    m_pNodeFramer->addNodes(getNumberOfLayers(),"Image");
#endif

}


void GLEffect::initializeVariables(void)
{
    //this is used for the gpu effects
    // The m_pCamera distance calculation should be passed in from the GL resize
    // routine.  This is a HACK which should be fixed FIXME

    #define PI                  3.14159265358979323846
    #define VIEW_ANGLE			45.0
    #define VIEW_ANGLE_RADIANS 	((VIEW_ANGLE / 180.0) * PI)

    m_fCameraDistance = ((float)m_nY_RESOLUTION / 2.0) / tan(VIEW_ANGLE_RADIANS / 2.0);

    m_bNoiseMatrixInitialized     = FALSE;
    m_bFireTextureInitialized     = FALSE;
    m_bRandomTextureInitialized   = FALSE;
    m_bContrastTextureInitialized = FALSE;
}

//////////////////////////////////////////////////
// adds a new layer to the listview and to the system


void GLEffect::objectMenu(int menu)
{

    changeSelection(menu);
}

//////////////////////////////////////////
// need to fix missing command below makeTexture
//////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////

void GLEffect::addClip(assetData clip)
{
    if (clip.endframe > m_nMaxFrame)
    {
        m_nMaxFrame = clip.endframe;
        updatesliderEndframe(m_nMaxFrame);
        m_pEndFrameControl->setValue(m_nMaxFrame);
    }

    JahLayer* jah_layer = getLayerList()->at(1)->thelayer;

    jah_layer->assignAsset(clip, VideoCategory::IMAGE);

    jah_layer->LoadLayerAsset(VideoCategory::IMAGE);

    jah_layer->makeTexture();
    jah_layer->setEffectsSlidersHaveChanged(true);

    if ( jah_layer->containsCpuEffects() )
    {
        *jah_layer->getCompositeQimagePtr() = jah_layer->textureData.objectImage.copy();
        *jah_layer->getCompositeQimageFlippedPtr() = 
            jah_layer->getCompositeQimagePtr()->mirror(false,true);
    }


    //set renderspace imagesize here...
    m_pCore->imagewidth  = jah_layer->getImageWidth();
    m_pCore->imageheight = jah_layer->getImageHeight();

    updateGL();
    updateSliders();

    m_bSlidersHaveChanged = true;

    //update start and end points to the start and end points of the source clip
    updatesliderStartframe (  clip.startframe);

    if (   jah_layer->asset.theType == VideoType::MOVIE 
        || jah_layer->asset.theType == VideoType::SEQUENCE
       ) 
    {
        updatesliderEndframe(  clip.endframe);
    }

    //update the ui settings
    m_pStartFrameControl->setValue(clip.startframe);
    m_pEndFrameControl->setValue(clip.endframe);

    // Update the nodeframer view
    //  TODO: do image stuff in nodes code

#ifdef NEWWIREUPWIDGET
	if (haswireup)
		thewireup->updateNodes(0,clip);
#else
    if ( m_bHasNodeFramer )
    {
    assetExchange exchange;
    QImage theimage;
    exchange.getStreamImagePtr( clip, theimage, 0 );
    QImage imgx = theimage.smoothScale( 100, 65, QImage::ScaleMin );
    m_pNodeFramer->setMainImage( imgx );
    m_pNodeFramer->setAssetName( clip.filename );
    m_pNodeFramer->rebuildNodes();
    }
#endif
}


void GLEffect::addCpuEffect()
{
    chooseNewCPUEffect( getLayerList()->at(1), m_pAddButton );


    EffectLayer* effect_layer = NULL;

    if ( activeLayerIsAnEffect() )
    {
        effect_layer = getActiveEffectLayer();
 
#ifdef NEWWIREUPWIDGET
		if (haswireup)
			thewireup->addNodes(getNumberOfLayers(),effect_layer->effectname);  
#endif
    }
}



void GLEffect::addMeshEffect()
{
    chooseNewMeshEffect( getLayerList()->at(1),AddRTButton );

    EffectLayer* effect_layer = NULL;

    if ( activeLayerIsAnEffect() )
    {
        effect_layer = getActiveEffectLayer();
		
#ifdef NEWWIREUPWIDGET
		if (haswireup)
			thewireup->addNodes(getNumberOfLayers(),effect_layer->effectname);  
#endif
    }
}


void GLEffect::addGpuEffect()
{
    chooseNewGPUEffect( getLayerList()->at(1),AddGpuButton );

    EffectLayer* effect_layer = NULL;

    if ( activeLayerIsAnEffect() )
    {
        effect_layer = getActiveEffectLayer();
		
#ifdef NEWWIREUPWIDGET
		if (haswireup)
			thewireup->addNodes(getNumberOfLayers(),effect_layer->effectname);  
#endif
    }
}

void
GLEffect::addCpuEffectFromTopMenu()
{
    QPopupMenu* jah_control_menu_options = Globals::getJahControl()->menuOptions[EFFECTS];
    chooseNewCPUEffect( getLayerList()->at(1), jah_control_menu_options, 1, 1 );

    EffectLayer* effect_layer = NULL;

    if ( activeLayerIsAnEffect() )
    {
        effect_layer = getActiveEffectLayer();
 
#ifdef NEWWIREUPWIDGET
		if (haswireup)
			thewireup->addNodes(getNumberOfLayers(),effect_layer->effectname);  
#endif
    }
}

void 
GLEffect::addGpuEffectFromTopMenu()
{
    QPopupMenu* jah_control_menu_options = Globals::getJahControl()->menuOptions[EFFECTS];
    chooseNewGPUEffect( getLayerList()->at(1), jah_control_menu_options, 1, 1 );

    EffectLayer* effect_layer = NULL;

    if ( activeLayerIsAnEffect() )
    {
        effect_layer = getActiveEffectLayer();
		
#ifdef NEWWIREUPWIDGET
		if (haswireup)
			thewireup->addNodes(getNumberOfLayers(),effect_layer->effectname);  
#endif
    }
}


void 
GLEffect::addMeshEffectFromTopMenu()
{
    QPopupMenu* jah_control_menu_options = Globals::getJahControl()->menuOptions[EFFECTS];
    chooseNewMeshEffect( getLayerList()->at(1), jah_control_menu_options, 1, 1 );

    EffectLayer* effect_layer = NULL;

    if ( activeLayerIsAnEffect() )
    {
        effect_layer = getActiveEffectLayer();
		
#ifdef NEWWIREUPWIDGET
		if (haswireup)
			thewireup->addNodes(getNumberOfLayers(),effect_layer->effectname);  
#endif
    }
}

