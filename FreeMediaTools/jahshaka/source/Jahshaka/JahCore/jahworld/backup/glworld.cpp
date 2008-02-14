/* source file for the Jahshaka glworld.cpp module
 ** The Jahshaka Project
 ** Copyright (C) 2000-2006 VM Inc.
 ** Released under the GNU General Public License
 **
 *******************************************************************************/

#include "glworld.h"

#include <qslider.h>
#include <InputLCD.h>
#include "supergrangecontrol.h"
#include <qhbox.h>
#include <qfiledialog.h>
#include <qtimer.h>
#include <qpopupmenu.h>
#include <qlistview.h>
#include <qcheckbox.h>
#include <qlayout.h>

#include "pbuffer.h"
#include <sstream>

#include <widget.h>
#include <dialogs.h>
#include <jahformatter.h>
#include <valueFormatters.h>
#include <jahdataio.h>
#include <jahtimer.h>
#include <timelineSlider.h>

#include "glworldlists.h"
#include "glcore.h"
#include "jahrender.h"
#include "jahtranslate.h"
#include "jahpluginlib.h"
#include <openobjectlib.h>

#ifndef JAHPLAYER
#include "jahnodes.h"
#include "keyframer.h"
#endif

#include <openmedialib.h>

GLWorld::GLWorld( GLCore* jahcore, const char* name, QHBox* controller, int* globalclipnumber )
    : m_pCore( jahcore ),
      m_pLayerListEntryList( NULL ),
      m_qsModuleName( name ),
      m_pController( controller ),
      m_pGrid( NULL ),
      m_pTitleSafe( NULL ),
      m_pTextSafe( NULL ),
      m_pCamera( NULL ),
      m_pActiveLayers( NULL ),
      m_pAutoSaveTimer( NULL ),
      m_pRenderSpace( NULL ),
      m_pTimer( NULL ),
      m_pCoreTimer( NULL ),
      m_pClipNumber( globalclipnumber ),
      m_pActiveLayer( NULL ),
      m_pControllerStartFrameControl( NULL ), // GUI
      m_pControllerEndFrameControl( NULL ), // GUI
      m_pAnimFrameLCD( NULL ),     // GUI
      m_pStartFrameControl( NULL ),// GUI
      m_pEndFrameControl( NULL ),  // GUI
      m_pCompositingModeSelectButton(NULL), // GUI

      m_aCPUEffectsDispatchTable(EffectInfo::NOT_A_TYPE),
      m_aMeshEffectsDispatchTable(EffectInfo::NOT_A_TYPE),
      m_aGPUEffectsDispatchTable(EffectInfo::NOT_A_TYPE),

      m_aCPUEffectsSelectionVector(EffectInfo::NOT_A_TYPE),
      m_aMeshEffectsSelectionVector(EffectInfo::NOT_A_TYPE),
      m_aGPUEffectsSelectionVector(EffectInfo::NOT_A_TYPE),

      m_bStarted( false ),

      //TODO: make sure all the data is initialited
      m_nAStartFrame(0),
      m_nAEndFrame(1),
      m_nAAnimFrame(1)

{  
    m_pJtrace = JahTrace::getInstance();	//set up tracer
    m_pJt = JahTranslate::getInstance();	//set up translator
    
    m_pJtrace->debug("Initializing Module",m_qsModuleName);
    
    m_frames_per_second_name_vector[FRAMES_PER_SECOND_15] = "15 Frames/Sec";
    m_frames_per_second_name_vector[FRAMES_PER_SECOND_24] = "24 Frames/Sec";
    m_frames_per_second_name_vector[FRAMES_PER_SECOND_25] = "25 Frames/Sec";
    m_frames_per_second_name_vector[FRAMES_PER_SECOND_30] = "30 Frames/Sec";
    m_frames_per_second_name_vector[FRAMES_PER_SECOND_60] = "60 Frames/Sec";
    m_frames_per_second_name_vector[FRAMES_PER_SECOND_90] = "90 Frames/Sec";

    m_pUseAutoKeyCheckBox = NULL;
    
    m_pTextEd = NULL; // GUI - not in constructuor
    Globals::setAutoSaveEnable(true);
    
    m_nCurrWidth = jahcore->width( );
    m_nCurrHeight = jahcore->height( );

    for (int i = 0; i < 11; i++)
    {
      m_aLightingSliders[i] = NULL; // GUI
    }
}

//pointer to jahpluginlib
JahPluginLib* GLWorld::m_pJPlugIn=0;

GLWorld::~GLWorld(void) 
{
    if ( m_pTimer ) m_pTimer->stop( );
    if ( m_pAutoSaveTimer ) m_pAutoSaveTimer->stop( );
    
    delete m_pCoreTimer;m_pCoreTimer=0;
    delete m_pRenderSpace;m_pRenderSpace=0;
    delete m_pActiveLayers;m_pActiveLayers=0;
    delete m_pLayerListEntryList;m_pLayerListEntryList=0;
    //    delete m_pJahLayerList;m_pJahLayerList=0;
    delete m_pGrid;m_pGrid=0;
    delete m_pTitleSafe;m_pTitleSafe=0;
    delete m_pTextSafe;m_pTextSafe=0;
    
    std::vector<EffectInfo*>::iterator it = m_aCPUEffectsSelectionVector.begin( );
    while ( it != m_aCPUEffectsSelectionVector.end( ) )
        delete *( it ++ );
    
    it = m_aMeshEffectsSelectionVector.begin( );
    while ( it != m_aMeshEffectsSelectionVector.end( ) )
        delete *( it ++ );
    
    it = m_aGPUEffectsSelectionVector.begin( );
    while ( it != m_aGPUEffectsSelectionVector.end( ) )
        delete *( it ++ );
};

void GLWorld::lazy( )
{

    m_pJtrace->info("GLWorld::lazy");

    //////////////////////////////////////////////////////////
    //set up ui effects options
    //should be done in the user interface code
    buildEffectsOptions();
    
    //////////////////////////////////////////////////////////
    //set up pointers to singletons for full class access
    
    //initalize the global vars from the prefs
    JahPrefs& jprefs = JahPrefs::getInstance();
    
    m_qsJahBasePath  = jprefs.getBasePath().data();
    m_qsJahMediaPath = jprefs.getMediaPath().data();
    m_nJahRenderer   = jprefs.getRenderer();
    
    //start up using default resolution
    int jahres = jprefs.getJahResolution();
    m_bHasResMenu = false; // GUI
    
    m_nJahResolutionValue = jahres;
    
    projectData thedata;
    m_nX_RESOLUTION = thedata.getXres(jahres);
    m_nY_RESOLUTION = thedata.getYres(jahres);
    m_fPIXEL_RATIO  = thedata.getPixelRatio(jahres);
    
    m_bHWFOG        = jprefs.getHwFog();
    m_bHWALIASING   = jprefs.getHwAliasing();
    
    //initialize all global variables
    m_nMinFrame=1; 
    m_nMaxFrame=100;
    m_bGridVal = false;
    
    m_qsRenderPath = m_qsJahMediaPath+"/media/renders/";
    
    //need to check this here as we dont have a JahScenesPAth pref yet
    QDir d( m_qsJahMediaPath+"/scenes/" );                        // "./example"
    
    if ( d.exists() )
    {
        m_qsScenePath  = m_qsJahMediaPath+"/scenes/";
    }
    else
    {
        m_qsScenePath = m_qsJahBasePath+"/scenes/";
    }
    
    
    m_MMode = MOFF; // mouse off
    m_bForcePlay = false;
    m_bUpdateEffect = false;

    //for gpu effects
    //m_bGPLSupport = false;
    //m_bNV_GPUSupport = false;

    m_bGPUActive = false;
    
    m_bGPUSupport = jprefs.getGpusupport();
    m_bNV_GPUSupport = jprefs.getNVGpusupport();
    
    //for hardware anti aliasing support
    m_bHWAliasing = false;
    
    // This list will supersede m_layerlist ASAP FIXME
    //m_pJahLayerList = new QPtrList<JahLayer>;
    //    m_pJahLayerList->setAutoDelete(true);
    
    ///////////////////////////////////////////////
    // the linked list is here
    m_pLayerListEntryList = new QPtrList<LayerListEntry>;
    m_pLayerListEntryList->setAutoDelete( TRUE );     // delete items when they are removed
    
    m_pCamera = NULL;
    
    //note that these dont have  to be layers
    //and should be display lists
    m_pGrid     = new JahLayer;
    m_pTitleSafe   = new JahLayer;
    m_pTextSafe    = new JahLayer;
    
    //the renderspace
    m_pRenderSpace = new JahRender(this);
    
    //the active layers list
    m_pActiveLayers = new ActiveLayerList;
    
    //for ui options
    m_nNumUiOptions = 0;
    
    //for autosave
    m_bAutoSaveInProgress = false;
    
    //builds the controller
    buildController(m_pController);
    
    //initalize world objects
    initializeWorld();
    
    //set default render quality to uncompressed BMP
    m_nRenderQuality = RENDER_QUALITY_BMP;
    
    //fog settings for the world view
    m_fHFog = 0.0; 
    m_fVFog = 0.0;
    
    //for the ui
    m_bHasLabels = false; // GUI
    m_bHasSliders = false; // GUI
    m_bHasButtons = false; // GUI
    
    //for various interface widgets
    m_bHasKeyFramer = false;
    m_bHasNodeFramer = false;
    
    /////////////////////////////////////////////////////////////////////
    //need to track and monitor this as well as control it for playback
    //controls the fps for scene playback
    //redrawWait=0;

    m_fJahFPS = 30.0; // doble

    setRedrawDelay( (int)((1.0 / m_fJahFPS) * 1000.0) );
    
    m_pTimer = new QTimer( this );

    connect( m_pTimer, 
	     SIGNAL(timeout()), 
	     this, 
	     SLOT(scheduleUpdate()) );

    m_pTimer->start( getRedrawDelay(), TRUE );
    
    m_pAutoSaveTimer = new QTimer(this);
    connect( m_pAutoSaveTimer, 
	     SIGNAL(timeout()),
	     this, 
	     SLOT( autoSaveThisModule() ) );

    m_pAutoSaveTimer->start(MODULE_AUTO_SAVE_FREQUENCY * 1000, true);
    
    /////////////////////////////////////////////
    //hook up core timer for fps feedback
    m_pCoreTimer = new JahTimer;
    
    //for statistical feedback
    m_bStatsShowStats = false;
    
    m_pVertexShader = NULL;
    m_pFragmentShader = NULL;
    m_pShaderProgram = NULL;
    
    m_nSceneSaveIndentationDepth = 0;
    
    Globals::setRendering(false);
    
    
    //for lights
    m_nNumberOfLights = 0;
    
    //for the render status
    m_bRenderAtImageResolution = false;
    
    //finished initializing
    m_pJtrace->debug("Initialized Module",m_qsModuleName);
    
    // Set the fps correctly
    TimecodeValueFormatter fmt;
    fmt.setFPS( thedata.getFPS( jahres ), thedata.getDropFrame( jahres ) );
    
    /*
      TODO : duplicate code from setresolution!
    */
    if ( m_pControllerSlider )  // GUI
      m_pControllerSlider->setFPS( 
				  thedata.getFPS( jahres ),
				  thedata.getDropFrame( jahres ) );

    if ( m_pStartFrameControl ) // GUI
      m_pStartFrameControl->setFormatter( fmt );

    if ( m_pControllerEndFrameControl )  // GUI
      m_pControllerEndFrameControl->setFormatter( fmt );

    if ( m_pControllerStartFrameControl )  // GUI
      m_pControllerStartFrameControl->setFormatter( fmt );

    if ( m_pEndFrameControl )  // GUI
      m_pEndFrameControl->setFormatter( fmt );

    if ( m_pAnimFrameLCD )  // GUI
      m_pAnimFrameLCD->setFormatter( fmt );
    
    for (int slider_number = 0; 
	 slider_number < (int)NUMBER_OF_AXIS_SLIDERS; 
	 slider_number++)
    {
        m_aJahSliders[slider_number] = NULL; // GUI
        m_aJahSliderLabels[slider_number] = NULL; // GUI
    }
    
    setRenderOnlyToSelectedEffect(false);
    setModuleUsesLighting(false);
    m_pRunScriptButton = NULL; // GUI
    setStopScript(false);
    
    
    m_vJahGLBlendModeStringVector.push_back("GL_ZERO");
    m_vJahGLBlendModeStringVector.push_back("GL_ONE");
    m_vJahGLBlendModeStringVector.push_back("GL_SRC_COLOR");
    m_vJahGLBlendModeStringVector.push_back("GL_ONE_MINUS_SRC_COLOR");
    m_vJahGLBlendModeStringVector.push_back("GL_SRC_ALPHA");
    m_vJahGLBlendModeStringVector.push_back("GL_ONE_MINUS_SRC_ALPHA");
    m_vJahGLBlendModeStringVector.push_back("GL_DST_ALPHA");
    m_vJahGLBlendModeStringVector.push_back("GL_ONE_MINUS_DST_ALPHA");
    m_vJahGLBlendModeStringVector.push_back("GL_DST_COLOR");
    m_vJahGLBlendModeStringVector.push_back("GL_ONE_MINUS_DST_COLOR");
    
    m_vJahGLBlendModeValueVector.push_back(GL_ZERO);
    m_vJahGLBlendModeValueVector.push_back(GL_ONE);
    m_vJahGLBlendModeValueVector.push_back(GL_SRC_COLOR);
    m_vJahGLBlendModeValueVector.push_back(GL_ONE_MINUS_SRC_COLOR);
    m_vJahGLBlendModeValueVector.push_back(GL_SRC_ALPHA);
    m_vJahGLBlendModeValueVector.push_back(GL_ONE_MINUS_SRC_ALPHA);
    m_vJahGLBlendModeValueVector.push_back(GL_DST_ALPHA);
    m_vJahGLBlendModeValueVector.push_back(GL_ONE_MINUS_DST_ALPHA);
    m_vJahGLBlendModeValueVector.push_back(GL_DST_COLOR);
    m_vJahGLBlendModeValueVector.push_back(GL_ONE_MINUS_DST_COLOR);
    
    setStopRequested(false);
};

void GLWorld::headRender( int width, int height ) 
{ 
    m_pJtrace->info("GLWorld::headRender");

    glViewport( 0, 0, width, height );
 
    //  set the values, 
    m_nCurrWidth  = width;
    m_nCurrHeight = height;
    
    m_pCore->paintGL( ); 
    
    // reset the current dimensions based on the rendering
    m_nCurrWidth  = m_pCore->getRenderWidth();
    m_nCurrHeight = m_pCore->getRenderHeight();
}

void GLWorld::scheduleUpdate( )
{
    m_pJtrace->info("GLWorld::scheduleUpdate");

    updatePosition( );
    updateGL( );
    schedulePosition( );
}

void GLWorld::initializePlugins( )
{
    static bool initialized = false;
    
    if ( initialized ) return;
    initialized = true;
    
    /////////////////////////////////////////////////////////////////
    //ok lets load the plugins in now...
    m_pJtrace->info( ">Initializing Plugins");
    
    JahPluginLib* jplugin = JahPluginLib::getInstance();
    jplugin->initializePlugins();
    
    m_pJtrace->info( ">Initialized Plugins");
    

}

//need to move into the ui generator code
void GLWorld::buildEffectsOptions(void)
{
    m_aCPUEffectsDispatchTable[EffectInfo::GREY_CPU_TYPE] = &GLWorld::doGrey;
    m_aCPUEffectsDispatchTable[EffectInfo::EDGE_DETECT_CPU_TYPE] = &GLWorld::doEdge;
    m_aCPUEffectsDispatchTable[EffectInfo::SELECT_COLORS_CPU_TYPE] = &GLWorld::doColorSelect;
    m_aCPUEffectsDispatchTable[EffectInfo::CONTRAST_CPU_TYPE] = &GLWorld::doContrast;
    m_aCPUEffectsDispatchTable[EffectInfo::BRIGHTNESS_CPU_TYPE] = &GLWorld::doBrightness;
    m_aCPUEffectsDispatchTable[EffectInfo::MATRIX_SHARPEN_CPU_TYPE] = &GLWorld::doMatrixSharpen;
    m_aCPUEffectsDispatchTable[EffectInfo::MATRIX_BLUR_CPU_TYPE] = &GLWorld::doMatrixBlur;
    m_aCPUEffectsDispatchTable[EffectInfo::MATRIX_EMBOSS_CPU_TYPE] = &GLWorld::doMatrixEmboss;
    m_aCPUEffectsDispatchTable[EffectInfo::GAUSSIAN_BLUR_CPU_TYPE] = &GLWorld::doGaussianBlur;
    m_aCPUEffectsDispatchTable[EffectInfo::MIRROR_CPU_TYPE] = &GLWorld::doMirror;
    m_aCPUEffectsDispatchTable[EffectInfo::SWAP_RGB_CPU_TYPE] = &GLWorld::doSwapRGB;
    m_aCPUEffectsDispatchTable[EffectInfo::NEGATIVE_CPU_TYPE] = &GLWorld::doNegative;
    m_aCPUEffectsDispatchTable[EffectInfo::COLORIZE_CPU_TYPE] = &GLWorld::doCPUColorize;
    m_aCPUEffectsDispatchTable[EffectInfo::CHROMA_KEYER_CPU_TYPE] = &GLWorld::doChromaKeyerCPU;
    
    m_aMeshEffectsDispatchTable[EffectInfo::FLAG_MESH_TYPE] = &GLWorld::doFlagRt;
    m_aMeshEffectsDispatchTable[EffectInfo::FISHEYE_MESH_TYPE] = &GLWorld::doFisheyeRt;
    m_aMeshEffectsDispatchTable[EffectInfo::RIPPLE_MESH_TYPE] = &GLWorld::doRippleRt;
    m_aMeshEffectsDispatchTable[EffectInfo::SWIRL_MESH_TYPE] = &GLWorld::doSwirlRt;
    
    m_aGPUEffectsDispatchTable[EffectInfo::STRETCH_CROP_GPU_TYPE] = &GLWorld::doStretchCrop;
    m_aGPUEffectsDispatchTable[EffectInfo::COLORIZE_GPU_TYPE] = &GLWorld::doColorizeGPU;
    m_aGPUEffectsDispatchTable[EffectInfo::EARTHQUAKE_GPU_TYPE] = &GLWorld::doNV30EarthquakeGPU;
    m_aGPUEffectsDispatchTable[EffectInfo::LIGHTING_GPU_TYPE] = &GLWorld::doNV30LightingGPU;
    m_aGPUEffectsDispatchTable[EffectInfo::CHARCOAL_GPU_TYPE] = &GLWorld::doNV30CharcoalGPU;
    m_aGPUEffectsDispatchTable[EffectInfo::CHROMA_KEYER_GLSL_SEPARATE_ALPHA_TYPE] = &GLWorld::doChromaKeyerGLSLSeparateAlpha;
    m_aGPUEffectsDispatchTable[EffectInfo::CHROMA_KEYER_GLSL_TYPE] = &GLWorld::doChromaKeyerGLSL;
    
    EffectInfo* new_menu_item;
    
    int i;
    
    for (i = EffectInfo::CPU_TYPE_START + 1; i < EffectInfo::MESH_TYPE_START; i++)
    {
        new_menu_item = new EffectInfo( EffectInfo::EFFECT_TYPE(i) );
        m_aCPUEffectsSelectionVector[i] = new_menu_item;
    }
    
    for (i = EffectInfo::MESH_TYPE_START + 1; i < EffectInfo::GPU_TYPE_START; i++)
    {
        new_menu_item = new EffectInfo( EffectInfo::EFFECT_TYPE(i) );
        m_aMeshEffectsSelectionVector[i] = new_menu_item;
    }
    
    for (i = EffectInfo::GPU_TYPE_START + 1; i < EffectInfo::EFFECT_TYPE_END; i++)
    {
        new_menu_item = new EffectInfo( EffectInfo::EFFECT_TYPE(i) );
        m_aGPUEffectsSelectionVector[i] = new_menu_item;
    }
    
    
}

void GLWorld::setResolution(int val)
{
    
    ///////////////////////////////////
    // projectData object
    projectData thedata;
    
    ///////////////////////////////////
    // error checking
    int numSettings;
    
    numSettings = thedata.getNumformats();
    if (val>numSettings)
    {
        val = thedata.getDefaultformat();
    }
    
    ////////////////////////////////////
    //update system and ui
    m_nJahResolutionValue = val;
    
    m_nX_RESOLUTION = thedata.getXres(m_nJahResolutionValue);
    m_nY_RESOLUTION = thedata.getYres(m_nJahResolutionValue);
    m_fPIXEL_RATIO  = thedata.getPixelRatio(m_nJahResolutionValue);

    TimecodeValueFormatter fmt;
    fmt.setFPS( thedata.getFPS( m_nJahResolutionValue ), thedata.getDropFrame( m_nJahResolutionValue ) );

    if ( m_pControllerSlider )  // GUI
      m_pControllerSlider->setFPS( thedata.getFPS( m_nJahResolutionValue),
				   thedata.getDropFrame( m_nJahResolutionValue)
				   );

    // TODO : for each x in subobjects ($x->setFormatter( fmt );)
    // BEGIN BLOCK
    if ( m_pStartFrameControl )  // GUI
      m_pStartFrameControl->setFormatter( fmt );

    if ( m_pEndFrameControl )  // GUI
      m_pEndFrameControl->setFormatter( fmt );

    if ( m_pAnimFrameLCD )  // GUI
      m_pAnimFrameLCD->setFormatter( fmt );

    if ( m_pControllerEndFrameControl ) // GUI
      m_pControllerEndFrameControl->setFormatter( fmt );

    if ( m_pControllerStartFrameControl )  // GUI
      m_pControllerStartFrameControl->setFormatter( fmt );

    // END BLOCK


    
    //a hack for the paint module here since it doesnt use a layer yet
    if (m_qsModuleName=="Paint")
    {
        //if (m_pCore->glmode==1)
        m_pCore->setFixedSize( QSize( m_nX_RESOLUTION, m_nY_RESOLUTION ) );
    }
    else
    {
      // todo : distribute the new resolution to all subobjects
      // 
        getLayerList()->at(0)->thelayer->updateResolution(m_nJahResolutionValue);
        m_pTitleSafe->updateResolution(m_nJahResolutionValue); //textsafe
        m_pTextSafe->updateResolution(m_nJahResolutionValue); //textsafe
    }
    
    setDontPaint(false);
    updateGL();
    
    
}

void GLWorld::updateResMenu(int val)
{
  if(m_bHasResMenu) // GUI
    {
        m_pJahResCombo->setCurrentItem(val); // GUI
    }
}

void GLWorld::changeZoom(int val)
{
    //val is -10 to 10 we need to compute this
    GLfloat value = GLfloat( val + 9 ) / 10.0f;

	if ( value <= 0.0f ) 
    {
        value = 0.1f;
    }

    m_pCore->setZoom( setZoom( value ) );
    updateGL();
}

void GLWorld::resetZoom(void)
{
    m_pCore->setZoom( 1.0f );
    updateGL();
}

//////////////////////////////////////////////
// Slider related

//not really slider but the main controller slider
void GLWorld::updateTimeSlider(int value)
{
  m_pJtrace->debug( "GLWorld::updateTimeSlider");

    if ( value != m_pControllerSlider->value() )
        m_pControllerSlider->setValue(value); // GUI
    m_nCurrentFrame = value;
}

//main controller slider selected
void GLWorld::sliderselected()
{ 
    m_bAnimSliderSelected = TRUE;
}

void GLWorld::slotTimeSliderReleased()
{
    m_bAnimSliderSelected = FALSE;
}

void GLWorld::sliderValueChanged(int)
{
//and not rendering
    if ( !getAnimating() && !Globals::getRendering() )
    {
      m_pJtrace->debug( "GLWorld::world slider value changed");

      int theframe;
      theframe = m_pControllerSlider->value();
      updateAnimation(theframe);
      updateGL();
    }
}


////////////////////////////////////////////////
// Player related

void GLWorld::setForcePlay(bool forceit)  
{ 
    m_bForcePlay = forceit; 
    //forcedplay->setChecked(forceplay);
}

void GLWorld::toggleForcePlay()  
{ 
    m_bForcePlay = !m_bForcePlay;
    setForcePlay(m_bForcePlay); //make forcedplay call its not a endless loop
}


void GLWorld::firstframeanimation()  
{
    m_nCurrentFrame = m_nMinFrame;
    
    updateAnimation(m_nCurrentFrame);
    updateGL();
    
    if ( getAnimating() ) 
    { 
        toggleAnimation(FALSE); 
    }
}

void GLWorld::previousframeanimation()  
{
    m_nCurrentFrame -= 1;
    
    if (m_nCurrentFrame < m_nMinFrame)
    {
        m_nCurrentFrame = m_nMaxFrame;
    }
    
    updateAnimation(m_nCurrentFrame);
    updateGL();
    
    if ( getAnimating() ) 
    { 
        toggleAnimation(FALSE); 
    }
}

void GLWorld::nextframeanimation()  
{
    m_nCurrentFrame++;	
    
    if (m_nCurrentFrame > m_nMaxFrame)
    {
        m_nCurrentFrame = m_nMinFrame;
    }
    
    updateAnimation(m_nCurrentFrame);
    updateGL();
    
    if ( getAnimating() ) 
    { 
        toggleAnimation(FALSE); 
    }
}

void GLWorld::lastframeanimation()
{
    
    m_nCurrentFrame = m_nMaxFrame;
    updateAnimation(m_nCurrentFrame);
    updateGL();
    
    if ( getAnimating() ) 
    { 
        toggleAnimation(FALSE); 
    }
}

////////////////////////////////////////
// Font control routines
void 
GLWorld::setTextLayerText(JahLayer* text_layer, const char* text_string)
{
  m_pJtrace->info( "setTextLayerText");

    QString text_qstring = text_string;
    text_layer->text->text = text_qstring;
    text_layer->getCheckListItem()->setText(0, text_string); 
    
    updateGL();  
}

void GLWorld::setText(const char* text_string) 
{ 

  m_pJtrace->info( "setText");

    QString text_qstring = text_string;
    getActiveJahLayer()->text->text = text_qstring;
    
    updateGL();  
}  

void GLWorld::setText(const QString& text_string) 
{ 
  m_pJtrace->info( "setText");

    getActiveJahLayer()->text->text = text_string;
    std::string first_twenty_characters = text_string.data();
    first_twenty_characters = first_twenty_characters.substr(0, 20);
    
    // Don't use new lines - they will make the listview look lousy
    size_t first_new_line = first_twenty_characters.find_first_of("\n");
    
    if (first_new_line != std::string::npos)
    {
        first_twenty_characters = first_twenty_characters.substr(0, first_new_line);
    }
    
    QString first_twenty_characters_qstring = first_twenty_characters.data();
    getActiveJahLayer()->getCheckListItem()->setText(0, first_twenty_characters_qstring); 
    
    updateGL();  
}  

void GLWorld::setFontColor() 
{ 
    setColor();
    
    updateGL(); 
} 

void GLWorld::loadFont() 
{
    //need to use a loader here... give the user a choice
    QString format = "ttf";
    
    QString filename = QFileDialog::getOpenFileName( 
        m_qsJahBasePath+"fonts",  QString( "*.%1" ).arg( format.lower() ), this );
    
    if ( !filename.isEmpty() ) 
    {
        getActiveJahLayer()->updateFont = true;
        getActiveJahLayer()->text->font = filename;
    }
    
    //refresh screen
    updateGL(); 
}

/////////////////////////////////////////
// 3D model control here

void GLWorld::changeParticleColors() 
{
    
    getActiveJahLayer()->ParticleColors = 
        !getActiveJahLayer()->ParticleColors;
    
}

//////////////////////////////////////////
// Clip and Key control

void GLWorld::setKeyStatus()
{
  m_pJtrace->debug( "GLWorld::","called setKeyStatus()");
    if ( getActiveJahLayer() != m_pCamera ) 
    {
        if ( getKeySelect() )
        {
            getActiveJahLayer()->drawtheKey = getKeySelect()->isOn();
        }
        else
        {
            getActiveJahLayer()->drawtheKey = !getActiveJahLayer()->drawtheKey;
        }
    }
    
    getActiveJahLayer()->setEffectsSlidersHaveChanged(true);
    updateGL();
}

void GLWorld::setClipStatus()         
{
  m_pJtrace->info( "GLWorld::","called setClipStatus()");
    
    if (getActiveJahLayer() != m_pCamera)
    {
        getActiveJahLayer()->drawtheLayer =  
            !getActiveJahLayer()->drawtheLayer;
    }
    
    updateGL();
}

void GLWorld::InvertKeyData()  
{ 
    getActiveJahLayer()->changeKeyStatus(); 
    updateGL();
}   

void GLWorld::addClip(assetData clip) 
{
    JahLayer* jah_layer = getActiveJahLayer();
    addClip(jah_layer, clip);
}

///////////////////////////////////////////////////////////
// why not just pass videodata to the layer?
// would be much cleaner

void GLWorld::addClip(JahLayer* jah_layer, assetData clip) 
{
    int endframe = clip.endframe - 1;
    
    if (endframe > m_nMaxFrame)
    {
        m_nMaxFrame = endframe;
        updatesliderEndframe(m_nMaxFrame);
        m_pEndFrameControl->setValue(m_nMaxFrame);
    }
    
    if (clip.startframe <= m_nMinFrame)
    {
        m_nMinFrame = clip.startframe;
	if ( m_nMinFrame < 1 ) 
	  m_nMinFrame = 1;
	updatesliderStartframe(m_nMinFrame);
        m_pStartFrameControl->setValue(m_nMinFrame);
    }
    
    if (jah_layer->getCategory() == ObjectCategory::EFFECT)
    {
        jah_layer = jah_layer->getParent();
    }
    
    jah_layer->assignAsset(clip, VideoCategory::IMAGE, true);
    
    jah_layer->LoadLayerAsset(VideoCategory::IMAGE);
    jah_layer->makeTexture();
    jah_layer->setFirstPass(true);

    updateGL();
    updateSliders();
    
}

void GLWorld::addClipKey(assetData clip)
{
    getActiveJahLayer()->assignAsset(clip, VideoCategory::KEY, true);
    
    getActiveJahLayer()->LoadLayerAsset(VideoCategory::KEY);
    getActiveJahLayer()->makeTexture();
    
    updateGL();
    updateSliders();
    
}


/////////////////////////////////////////////////
// Reset and clear layers

void GLWorld::ResetLayer() 
{
  m_pJtrace->info( "GLWorld::","called ResetLayer()");
    
    if (getActiveJahLayer() == m_pCamera) 
    { 
        resetkeys(m_pCamera);
    } 
    else 
    { 
        resetkeys(getActiveJahLayer()); 
    }
    
    m_bAnimation = FALSE;
    updateGL();
    
}

void GLWorld::ResetAll() 
{
  m_pJtrace->info( "GLWorld::","called ResetAll()");
    
    if ( m_pCamera )
    	resetkeys(m_pCamera);
    
    for ( uint i = 0; i <= getLayerList()->count(); ++i )
    {
        if ( getLayerList()->at(i) ) 
        {
            resetkeys(getLayerList()->at(i)->thelayer); 
        }   
    }
    
    m_bAnimation = FALSE;
    updateGL();
}

void GLWorld::ClearAll() 
{
    m_pLayerListView->clear();

    // TODO : refactor to own method
    getLayerList()->clear();
    delete m_pLayerListEntryList;
    m_pLayerListEntryList=0;
    
    if (m_qsModuleName=="Effects")
    {
#ifndef JAHPLAYER
#ifndef NEWWIREUPWIDGET
        m_pNodeFramer->clear();
#endif
#endif
    }
    
    //recreate the variables
    getLayerList(); // this will create a new list automaticallay
    
    //initialize them now
    initializeObjects();
    initializeWorld();
    m_pActiveLayer = getFirstJahLayer();
    
    setResolution(m_nJahResolutionValue);
}

JahLayer*       
GLWorld::getFirstJahLayer()
{
    return( getLayerList()->first()->getJahLayer() );
}

void GLWorld::loadClearAll()
{
    m_pJtrace->info( "GLWorld::","called LoadClearAll()");
    m_pLayerListView->clear();
    getLayerList()->clear();
    delete m_pLayerListEntryList;
    m_pLayerListEntryList=0;

    //recreate the variables
    getLayerList();
    
    m_pActiveLayer = NULL;
    
    m_pChromaSettingsLayer = NULL;
    m_pChromaKeyLayer = NULL;
    m_pChromaBaseLayer = NULL;
}

int
GLWorld::getActiveLayerNumber()
{
    LayerListEntry* layer_list_entry = getActiveJahLayer()->getLayerListEntry();
    int layer_number = getLayerList()->find(layer_list_entry);
    return layer_number;
}


/*
  the controller slider updates the dependant key
framer 

*/
void GLWorld::connectKeyframer( void ) 
{
#ifndef JAHPLAYER
    connect ( m_pKeyFramer, SIGNAL(AddKeyframe(void)),    this, SLOT(addKeyframe(void)) );
    connect ( m_pKeyFramer, SIGNAL(RemoveKeyframe(void)), this, SLOT(removeKeyframe(void)) );
    connect ( m_pKeyFramer, SIGNAL(PrevKeyframe(void)),   this, SLOT(prevKeyframe(void)) );
    connect ( m_pKeyFramer, SIGNAL(NextKeyframe(void)),   this, SLOT(nextKeyframe(void)) );
    connect ( m_pKeyFramer, SIGNAL(signalKeyframeChanged(void)), this, SLOT( slotKeyframeChanged(void) ) );
    
    // tell keyframer to update its timeline
    connect( m_pControllerSlider,   
	     SIGNAL(valueChanged(int)),  
	     m_pKeyFramer,  
	     SLOT(movePlayLine(int)) );
#endif
}

void GLWorld::addKeyframer( QWidget *parentwidget ) 
{
#ifndef JAHPLAYER
    m_pKeyFramer = new JahKeyframer( this, parentwidget, "keyframer" );
    
    m_bHasKeyFramer = true; //let the world know its there
    
    connectKeyframer();
#endif
}

////////////////////////////////////////////////////
// creates a instance of the nodeframer
// for modules that want to use the widget
void GLWorld::initializeNodes( QHBox* parent2)
{
#ifndef JAHPLAYER
//#ifndef WIN32
    
    
    
#ifdef NEWWIREUPWIDGET
    thewireup = new WireWidget( parent2, "keyframer" );
    thewireup->setFixedSize( QSize( 980, 600 ) );    //was 955,486
    
    haswireup = true;
    m_bHasNodeFramer = false;
#else
    
    m_pNodeFramer = new JahNodes( parent2, "keyframer" );
    m_pNodeFramer->setFixedSize( QSize( 0, 0 ) );    //was 955,486
    
    m_bHasNodeFramer = true;
    m_bHasWireUp = false;
    
    // pick up signals from user clicking on the effect node canvas view.
    connect ( m_pNodeFramer->nodeeditor, 
	      SIGNAL( nodeClicked ( int ) ) ,
	      this, 
	      SLOT ( effectNodeClicked ( int )));
    
    
#endif
    
#endif 
}

int
GLWorld::findEffectParent(JahLayer* layer)
{
    if (layer->objtype == ObjectCategory::EFFECT)
    {
        JahLayer* parent = layer->getParent();
        return( findLayer(parent) );
    }
    
    return findLayer(layer);
}

void GLWorld::SetGPUSelect(void)
{
    if (m_bGPUSupport || m_bNV_GPUSupport)
    {
      m_bGPUActive = m_pGPUSelect->isChecked();
    }

    if ( getLayerList()->at(1) && getGPUActive() )
    {
        getLayerList()->at(1)->
	  thelayer->usedagpu = true;
    }
    else
    {
        getLayerList()->at(1)->thelayer->usedagpu = false;
    }
    
    updateGL();
}

bool            
GLWorld::canAcceptEffects(ObjectCategory::TYPE object_category)
{
    if (   object_category == ObjectCategory::JAHLAYER
        || object_category == ObjectCategory::MESHLAYER
        || object_category == ObjectCategory::CUBE
        || object_category == ObjectCategory::SPHERE
        || object_category == ObjectCategory::CYLINDER
        )
    {
        return true;
    }
    
    return false;
}

int 
GLWorld::getJahLayerNumber(JahLayer* jah_layer)
{
    LayerListEntry* layerlist_layer = getLayerList()->first();
    
    for (; layerlist_layer; layerlist_layer = getLayerList()->next())
    {
        if (layerlist_layer->thelayer == jah_layer)
        {
            return getLayerList()->at();
        }
    }
    
    return(-1);
}


LayerListEntry* 
GLWorld::findLayerListEntry(QListViewItem* item)
{
    for (LayerListEntry* layer_list_entry = getLayerList()->first(); 
	 layer_list_entry;
	 layer_list_entry = getLayerList()->next() )
    {
        if (layer_list_entry->thelayer->getCheckListItem() == item)
        {
            return layer_list_entry;
        }
    }
    
    return NULL;
}

LayerListEntry* 
GLWorld::findLayerListEntry(JahLayer* jah_layer)
{
    for (LayerListEntry* layer_list_entry = getLayerList()->first(); 
	 layer_list_entry; 
	 layer_list_entry = getLayerList()->next() )
    {
        if (layer_list_entry->getJahLayer() == jah_layer)
        {
            return layer_list_entry;
        }
    }
    
    return NULL;
}


void
GLWorld::setSelectColorsIndividually(void)
{

    JahLayer* active_layer = getActiveJahLayer();

    m_bSelectColorsIndividually =
      m_pSelectColorsIndividuallyQCheckBox->isChecked();

    // pass this on to the active layer
    active_layer->
      setSelectColorsIndividually(
				  m_bSelectColorsIndividually
				  );
    // update the screen
    updateGL();
}

QCheckBox* 
GLWorld::getKeySelect()
{
    return NULL;
}


void
GLWorld::autoSaveThisModule()
{
    if (m_qsModuleName == "Animation" || m_qsModuleName == "Effects")
    {
        if ( !Globals::getRendering() && Globals::getAutoSaveEnable() )
        {
            setAutoSaveInProgress(true);
            std::ostringstream save_filename;
            save_filename.str("");
            save_filename << m_qsScenePath.data()
			  << m_qsModuleName.data() << "_backup.jsf";
            std::ostringstream save_filename2;
            save_filename2.str("");
            save_filename2 << m_qsScenePath.data() 
			   << m_qsModuleName.data() << "_backup2.jsf";
            char system_command[1000];
            char copy_command[50];
#ifdef WIN32
            sprintf(copy_command, "copy");
#else
            sprintf(copy_command, "cp");
#endif
            sprintf( system_command, "%s %s %s", copy_command, save_filename.str().c_str(), save_filename2.str().c_str() );
            //system(system_command);
            setSceneSaveExport(false);
            saveSceneFile( save_filename.str() );
            setAutoSaveInProgress(false);
        }
        
        getAutoSaveTimer()->start(MODULE_AUTO_SAVE_FREQUENCY * 1000, true);
    }
}

void 
GLWorld::setUsePbuffer(void)
{
    bool use_pbuffer = m_pBufferSelectCheckBox->isChecked();
    
    JahLayer* active_layer = getActiveJahLayer();
    JahLayer* parent_layer = active_layer->getParent();
    JahLayer* image_layer = parent_layer ? parent_layer : active_layer;
    image_layer->setUsePbuffer(use_pbuffer);
    image_layer->setEffectsSlidersHaveChanged(true);
    updateGL();
}

void 
GLWorld::slotSetSrcBlendFactor()
{
    QPopupMenu *menu = new QPopupMenu( 0 );
    
    menu->insertItem( "GL_ZERO", JAH_GL_ZERO);
    menu->insertItem( "GL_ONE", JAH_GL_ONE);
    menu->insertItem( "GL_SRC_COLOR", JAH_GL_SRC_COLOR);
    menu->insertItem( "GL_ONE_MINUS_SRC_COLOR", JAH_GL_ONE_MINUS_SRC_COLOR);
    menu->insertItem( "GL_SRC_ALPHA", JAH_GL_SRC_ALPHA);
    menu->insertItem( "GL_ONE_MINUS_SRC_ALPHA", JAH_GL_ONE_MINUS_SRC_ALPHA);
    menu->insertItem( "GL_DST_ALPHA", JAH_GL_DST_ALPHA);
    menu->insertItem( "GL_ONE_MINUS_DST_ALPHA", JAH_GL_ONE_MINUS_DST_ALPHA);
    menu->insertItem( "GL_DST_COLOR", JAH_GL_DST_COLOR);
    menu->insertItem( "GL_ONE_MINUS_DST_COLOR", JAH_GL_ONE_MINUS_DST_COLOR);
    
    
    menu->setMouseTracking( TRUE );
    JAH_GL_BLEND_MODE factor = 
        (JAH_GL_BLEND_MODE)
      menu->exec(
		 m_pSrcBlendModeSelectButton
		 ->mapToGlobal( 
			       QPoint( 0, 
				       m_pSrcBlendModeSelectButton
				       ->height() + 1 ) ) );
    
    if ( factor >= JAH_GL_ZERO ) 
    {
        setSrcBlendFactor(factor);
    }
    
    delete menu;
    updateGL();  //redraw scene
}

void 
GLWorld::slotResetBlendFactor()
{
    setSrcBlendFactor(JAH_GL_SRC_ALPHA);
    setDstBlendFactor(JAH_GL_ONE_MINUS_SRC_ALPHA);
}

void 
GLWorld::slotSetDstBlendFactor()
{
    QPopupMenu *menu = new QPopupMenu( 0 );
    
    menu->insertItem( "GL_ZERO", JAH_GL_ZERO);
    menu->insertItem( "GL_ONE", JAH_GL_ONE);
    menu->insertItem( "GL_SRC_COLOR", JAH_GL_SRC_COLOR);
    menu->insertItem( "GL_ONE_MINUS_SRC_COLOR", JAH_GL_ONE_MINUS_SRC_COLOR);
    menu->insertItem( "GL_SRC_ALPHA", JAH_GL_SRC_ALPHA);
    menu->insertItem( "GL_ONE_MINUS_SRC_ALPHA", JAH_GL_ONE_MINUS_SRC_ALPHA);
    menu->insertItem( "GL_DST_ALPHA", JAH_GL_DST_ALPHA);
    menu->insertItem( "GL_ONE_MINUS_DST_ALPHA", JAH_GL_ONE_MINUS_DST_ALPHA);
    menu->insertItem( "GL_DST_COLOR", JAH_GL_DST_COLOR);
    menu->insertItem( "GL_ONE_MINUS_DST_COLOR", JAH_GL_ONE_MINUS_DST_COLOR);
    
    
    menu->setMouseTracking( TRUE );
    JAH_GL_BLEND_MODE factor = 
        (JAH_GL_BLEND_MODE)menu->exec( 
				      m_pDstBlendModeSelectButton
				      ->mapToGlobal( QPoint( 0, m_pDstBlendModeSelectButton->height() + 1 ) ) );
    
    if ( factor >= JAH_GL_ZERO ) 
    {
        setDstBlendFactor(factor);
    }
    
    delete menu;
    updateGL();  //redraw scene
}

void 
GLWorld::configureModuleEngine()
{
}

void            
GLWorld::updateCompositeButton(CompositeType::TYPE type)
{
    QString name = CompositeType::getName(type).data();

    if(m_pCompositingModeSelectButton)
    {
        m_pCompositingModeSelectButton->setText(name);
    }
}

void            
GLWorld::setCompositeType(CompositeType::TYPE type)
{
    getActiveJahLayer()->setCompositeType(type);
    updateGL();
}

void
GLWorld::slotSetCompositingMode()
{
    QPopupMenu *menu = new QPopupMenu( 0 );
    
    menu->insertItem( "Reset", CompositeType::STANDARD_COMPOSITE_TYPE);
    menu->insertItem( "A", CompositeType::A_COMPOSITE_TYPE);
    menu->insertItem( "B", CompositeType::B_COMPOSITE_TYPE);
    menu->insertItem( "A over B", CompositeType::A_OVER_B_COMPOSITE_TYPE);
    menu->insertItem( "B over A", CompositeType::B_OVER_A_COMPOSITE_TYPE);
    menu->insertItem( "A in B", CompositeType::A_IN_B_COMPOSITE_TYPE);
    menu->insertItem( "B in A", CompositeType::B_IN_A_COMPOSITE_TYPE);
    menu->insertItem( "A out B", CompositeType::A_OUT_B_COMPOSITE_TYPE);
    menu->insertItem( "B out A", CompositeType::B_OUT_A_COMPOSITE_TYPE);
    menu->insertItem( "A atop B", CompositeType::A_ATOP_B_COMPOSITE_TYPE);
    menu->insertItem( "B atop A", CompositeType::B_ATOP_A_COMPOSITE_TYPE);
    menu->insertItem( "A xor B", CompositeType::A_XOR_B_COMPOSITE_TYPE);
    
    
    menu->setMouseTracking( TRUE );
    
    CompositeType::TYPE type = 
        (CompositeType::TYPE)menu->exec( m_pCompositingModeSelectButton->mapToGlobal( QPoint( 0, m_pCompositingModeSelectButton->height() + 1 ) ) );
    
    if ( type >= CompositeType::A_COMPOSITE_TYPE ) 
    {
        setCompositeType(type);
        updateCompositeButton(type);
    }
}

void
GLWorld::setSrcBlendFactor(JAH_GL_BLEND_MODE factor)
{
    m_nSrcBlendFactor =
      getJahGlBlendModeValueVector(factor);

    m_pSrcBlendModeSelectButton->
      setText(
	      getJahGlBlendModeStringVector(factor).data() );
    getActiveJahLayer()->
      setSrcBlendFactor(m_nSrcBlendFactor);
}

void
GLWorld::setDstBlendFactor(JAH_GL_BLEND_MODE factor)
{
    m_nDstBlendFactor = getJahGlBlendModeValueVector(factor);
    m_pDstBlendModeSelectButton->setText( getJahGlBlendModeStringVector(factor).data() );
    getActiveJahLayer()->setDstBlendFactor(m_nDstBlendFactor);
}


bool        
GLWorld::getUseLighting()
{
    return m_pUseLightingCheckBox->isChecked();
}

bool        
GLWorld::getShowLights()
{
    return getShowLightsCheckBox()->isChecked();
}

bool        
GLWorld::getUseAutoKey()
{
    if ( getUseAutoKeyCheckBox() )
    {
        return getUseAutoKeyCheckBox()->isChecked();
    }

    return false;
}

void        
GLWorld::setUseAutoKey(bool flag)
{
    getUseAutoKeyCheckBox()->setChecked(flag);
}

void        
GLWorld::hideAllLightingSliders()
{
    for (int i = 0; i < (int)NUMBER_OF_LIGHTING_SLIDERS; i++)
    {
        getLightingSliderPtr(i)->hide();
        getLightingLcdPtr(i)->hide();
        getLightingSliderLabelPtr(i)->hide();
    }
}

void        
GLWorld::showAllLightingSliders()
{
    for (int i = 0; i < (int)NUMBER_OF_LIGHTING_SLIDERS; i++)
    {
        getLightingSliderPtr(i)->show();
        getLightingLcdPtr(i)->show();
        getLightingSliderLabelPtr(i)->show();
    }
}

void
GLWorld::createTrackerPointLayers()
{
}

void GLWorld::makeCurrent( void ) 
{ 
    m_pCore->makeCurrent( );
}

void GLWorld::updateGL( void ) 
{
    if ( usesVideoHead( ) )
        emit showInHead( this );

    m_pCore->updateGL( );
    
    m_nCurrWidth  = m_pCore->getRenderWidth();
	m_nCurrHeight = m_pCore->getRenderHeight();
}

JahLayer *GLWorld::getJahLayer( int layer_number ) 
{ 
    return getLayerList()->at(layer_number)->thelayer; 
}

LayerListEntry *GLWorld::getActiveLayer( )
{
    return m_pActiveLayer->getLayerListEntry();
}

int             
GLWorld::getNumberOfLayers() 
{ 
    return getLayerList()->count(); 
}

void 
GLWorld::slotChooseFramesPerSecond()
{
    QPopupMenu *menu = new QPopupMenu( 0 );
    
    menu->insertItem( "15", FRAMES_PER_SECOND_15, 0 );
    menu->insertItem( "24", FRAMES_PER_SECOND_24, 0 );
    menu->insertItem( "25", FRAMES_PER_SECOND_25, 0 );
    menu->insertItem( "30", FRAMES_PER_SECOND_30, 0 );
    menu->insertItem( "60", FRAMES_PER_SECOND_60, 0 );
    menu->insertItem( "90", FRAMES_PER_SECOND_90, 0 );
    
    menu->setMouseTracking( TRUE );
    QPoint qpoint = QPoint( 0, m_pSelectFPSPushbutton->height() + 1 );// GUI
    int id = menu->exec( m_pSelectFPSPushbutton->mapToGlobal(qpoint) );
    
    int new_frames_per_second;
    // Values are just estmates.  Actually delay times are difficult to compute because of scene 
    // Complexity. 
    int redraw_delay = 28;
    
    if ( id != -1 ) 
    {
        switch(id) 
        {
        case FRAMES_PER_SECOND_15   : 
        { 
            new_frames_per_second = 15; 
            redraw_delay = 60;
            break; 
        }
        case FRAMES_PER_SECOND_24   : 
        { 
            new_frames_per_second = 24; 
            redraw_delay = 35;
            break; 
        }
        case FRAMES_PER_SECOND_25   : 
        { 
            new_frames_per_second = 25; 
            redraw_delay = 32;
            break; 
        }
        case FRAMES_PER_SECOND_30   : 
        { 
            new_frames_per_second = 30; 
            redraw_delay = 28;
            break; 
        }
        case FRAMES_PER_SECOND_60   : 
        { 
            new_frames_per_second = 60; 
            redraw_delay = 12;
            break; 
        }
        case FRAMES_PER_SECOND_90   : 
        { 
            new_frames_per_second = 90; 
            redraw_delay = 5;
            break; 
        }
        default  : 
        { 
            new_frames_per_second = 30; 
            redraw_delay = 28;
            break; 
        }
        }
    }
    else
    {
        new_frames_per_second = 30;
        id = FRAMES_PER_SECOND_30;
    }
    
    delete menu;
    
    QString text_string = getFramesPerSecondName( FRAMES_PER_SECOND(id) ).data();
    m_pSelectFPSPushbutton->setText(text_string);
    
    setFramesPerSecond(new_frames_per_second);
    m_pSelectFPSPushbutton->setDown(false);
    
    setRedrawDelay(redraw_delay);
}

std::vector<std::string> 
GLWorld::m_frames_per_second_name_vector(FRAMES_PER_SECOND_90 + 1);

void            
GLWorld::startSingleShotTimer()
{

  m_pJtrace->info( "startSingleShotTimer");

    // If video needs to be loaded, it will occupy most of the time delay
    // This needs to be scaled to compensate for an estimated delay based
    // on the scene characteristics
    int layer_count = getLayerList()->count();
    int delay = getRedrawDelay();
    
    // Take off 3ms delay for each layer
    delay -= (layer_count * 3);
    
    if (delay < 0)
    {
        delay = 0;
    }

    m_pTimer->start( delay, TRUE );
}

void GLWorld::slotUpdateListViewCheckBoxes()
{
    LayerListEntry* layer_list_entry = getLayerList()->first();
    
    for ( ; layer_list_entry; layer_list_entry = getLayerList()->next() )
    {
        JahLayer* jah_layer = layer_list_entry->getJahLayer();
        QCheckListItem* check_list_item = (QCheckListItem*)jah_layer->getCheckListItem();
        motionNode* motion_node = jah_layer->layernodes->m_node;
        
        if (check_list_item && motion_node)
        {
            check_list_item->setOn(motion_node->layer_enabled);
        }
    }
}

JahLayer* GLWorld::createALayer(LAYER_TYPE, JahLayer*)
{
  m_pJtrace->info( "TODO createALayer");

    return (JahLayer*)NULL;
}

void GLWorld::slotSetShowLights()
{
    Globals::setShowLights( getShowLights() );
    updateGL();
}

void GLWorld::slotLightColorAmbient(QColor )
{
}

void GLWorld::slotLightColorDiffuse(QColor )
{
}

void GLWorld::slotLightColorSpecular(QColor )
{
}

bool
GLWorld::useOpenGLKey()
{
  m_pJtrace->info( "useOpenGLKey");

    JahLayer* jah_layer = getActiveJahLayer();

    if (jah_layer)
    {
        return ( jah_layer->getUseOpenGlKeyer() );
    }

    return false;
}


bool
GLWorld::useFastShaderKey()
{

  m_pJtrace->info( "useFastShaderKey");

    JahLayer* jah_layer = getActiveJahLayer();

    if (jah_layer)
    {
        return ( jah_layer->getUseFastShaderKeyer() );
    }

    return false;
}



bool
GLWorld::getLockKeyAndClip()
{
    return m_pLockKeyAndClipCheckBox->isChecked();
}

void
GLWorld::slotUpdateAnimation()
{
    updateAnimation( getCurrentFrame() );
    updateGL();
}

void
GLWorld::slotSetUseOpenGlKeyer()
{
  m_pJtrace->info( "slotSetUseOpenGlKeyer");

    JahLayer* jah_layer = getActiveJahLayer();

    bool is_on = m_pUseOpenGLKeyLayerCheckBox->isOn();

    if (jah_layer)
    {
        jah_layer->setUseOpenGlKeyer(is_on);

        if (is_on)
        {
            m_pUseFastShaderKeyLayerCheckBox->setChecked(false);
            jah_layer->setUseFastShaderKeyer(false);
        }
    }

    slotUpdateAnimation();
}


void
GLWorld::slotSetUseFastShaderKeyer()
{
    JahLayer* jah_layer = getActiveJahLayer();

    bool is_on = m_pUseFastShaderKeyLayerCheckBox->isOn();

    if (jah_layer)
    {
        jah_layer->setUseFastShaderKeyer(is_on);

        if (is_on)
        {
            m_pUseOpenGLKeyLayerCheckBox->setChecked(false);
            jah_layer->setUseOpenGlKeyer(false);
        }
    }

    slotUpdateAnimation();
}


void GLWorld::activate( )
{
  if ( !m_bStarted )
    {
      lazy( );
      start( );
      m_bStarted = true;
    }
}


int GLWorld::getCurrentWidth( ) const
{
  return m_nCurrWidth;
}

int GLWorld::getCurrentHeight( ) const
{
  return m_nCurrHeight;
}

std::vector<std::string>* GLWorld::getFramesPerSecondNameVector()
{
  return &m_frames_per_second_name_vector;
}

std::string GLWorld::getFramesPerSecondName(FRAMES_PER_SECOND index)
{
  return getFramesPerSecondNameVector()->at(index);
}

void GLWorld::keyReleaseEvent( QKeyEvent * )
{ }

void GLWorld::createMenuItem(QPopupMenu * )

{ }

void    GLWorld::sceneIncreaseIndent()
{ 
  m_nSceneSaveIndentationDepth++;
}

void    GLWorld::sceneDecreaseIndent()
{ 
  m_nSceneSaveIndentationDepth--;
}

int     GLWorld::getSceneSaveIndentationDepth()
{ 
  return m_nSceneSaveIndentationDepth;
}

void    GLWorld::setSceneSaveIndentationDepth(int depth)
{ 
  m_nSceneSaveIndentationDepth = depth;
}

bool GLWorld::getRenderOnlyToSelectedEffect()
{ 
  return m_bRenderOnlyToSelectedEffect;
}

void GLWorld::setRenderOnlyToSelectedEffect(bool flag)
{
  m_bRenderOnlyToSelectedEffect = flag;
}

std::string& GLWorld::getJahGlBlendModeStringVector(int index)
{
  return m_vJahGLBlendModeStringVector[index];
}

GLuint GLWorld::getJahGlBlendModeValueVector(int index)
{ return m_vJahGLBlendModeValueVector[index];
}

bool GLWorld::getSceneSaveExport()
{ return m_bSceneSaveExport;
}

void GLWorld::setSceneSaveExport(bool flag)
{ 
  m_bSceneSaveExport = flag;
}


void GLWorld::start( )
{ 
}


bool GLWorld::getAnimationUpdated()
{ 
  return m_bAnimationUpdated;
}

void GLWorld::setAnimationUpdated(bool flag)
{ m_bAnimationUpdated = flag;
}

JahLayer*       GLWorld::getBaseLayer()
{
  m_pJtrace->info( "getBaseLayer");
  return m_pBaseLayer;
}

bool GLWorld::getStopRequested()
{ return m_bStopRequested;
}

void GLWorld::setStopRequested(bool flag)
{ m_bStopRequested = flag;
}

bool GLWorld::getDontPaint()
{ 
  return m_bDontPaint;
}

void GLWorld::setDontPaint(bool flag)
{ 
  m_bDontPaint = flag;
}


bool GLWorld::getDontTrack()
{
  return m_bDontTrack;
}

void GLWorld::setDontTrack(bool flag)
{
  m_bDontTrack = flag;
}

QPtrList<LayerListEntry>*   GLWorld::getLayerList()
{
  m_pJtrace->info( "GLWorld::getLayerList");

  if (m_pLayerListEntryList)
    {
      return m_pLayerListEntryList;
    }
  else
    {
      // create on demand! should have been initialized from lazy
      m_pLayerListEntryList = new QPtrList<LayerListEntry>; // taken from GLWorld::ClearAll()
      m_pLayerListEntryList->setAutoDelete( TRUE );     // delete items when they are removed
      return m_pLayerListEntryList;
    }
}

GLCore*     GLWorld::getCore()
{
  return m_pCore;
}


QCheckBox*   GLWorld::getShowLightsCheckBox()
{
  return m_pShowLightsCheckBox;
}


QCheckBox*   GLWorld::getUseAutoKeyCheckBox()
{
  return m_pUseAutoKeyCheckBox;
}



QTimer*     GLWorld::getAutoSaveTimer()
{
  return m_pAutoSaveTimer;
}


bool GLWorld::getAutoSaveInProgress()
{
  return m_bAutoSaveInProgress;
}


void GLWorld::setAutoSaveInProgress(bool flag)
{ 
  m_bAutoSaveInProgress=flag; // TODO , was true, 
}


const char*      GLWorld::getTrackerFragmentShader()
{
  return m_sTrackerFragmentShader;
}


bool GLWorld::getModuleUsesLighting()
{
  return m_bModuleUsesLighting;
}

void GLWorld::setModuleUsesLighting(bool flag)
{ 
  m_bModuleUsesLighting = flag;
}


QSlider*    GLWorld::getLightingSliderPtr(int slider_number)
{ 
  return m_aLightingSliders[slider_number];
}

InputLCD*   GLWorld::getLightingLcdPtr(int slider_number)
{ 
  return m_aLightingLCDs[slider_number];
}

QLabel*     GLWorld::getLightingSliderLabelPtr(int slider_number)
{
  return m_aLightingSliderLabels[slider_number];
}

int      GLWorld::getCurrentFrame()
{
  return m_nCurrentFrame;
}

void GLWorld::    setCurrentFrame(int frame_number)
{ 
  m_nCurrentFrame = frame_number;
}

void GLWorld::    incrementCurrentFrame(int step)
{
  m_nCurrentFrame += step;
}

JahLayer* GLWorld::   getActiveJahLayer()
{
  m_pJtrace->info( "GLWorld::getActiveJahLayer");

  return m_pActiveLayer;
}

void GLWorld::    setActiveJahLayer(JahLayer* layer)
{ 
  m_pJtrace->info( "GLWorld::setActiveJahLayer");

  m_pActiveLayer = layer;
}

bool     GLWorld::getAnimating()
{
  return m_bAnimation;
}

void GLWorld::    setAnimating(bool flag)
{ 
  m_bAnimation = flag;
}

std::string& GLWorld::getRenderOutputFileNameString()
{
  return m_sRenderOutputFilename;
}

void GLWorld::    setRenderOutputFileNameString(std::string name)
{
  m_sRenderOutputFilename = name;
}

bool     GLWorld::getStopScript()
{
  return m_bStopScript;
}

void GLWorld::    setStopScript(bool flag)
{ 
  m_bStopScript = flag;
}

void GLWorld::    set (bool flag)
{ 
  m_bStopScript = flag;
}

int      GLWorld::getRedrawDelay()
{
  return m_nRedrawDelay;
}

void GLWorld::    setRedrawDelay(int delay)
{ 
  m_nRedrawDelay = delay;
}

QPushButton*     GLWorld::getRunScriptButton()
{
  return m_pRunScriptButton;
}

GLuint GLWorld::  getSrcBlendFactor()
{
  return m_nSrcBlendFactor;
}

GLuint GLWorld::  getDstBlendFactor()
{
  return m_nDstBlendFactor;
}

JahControl*     GLWorld::getJahControl()
{
  return m_pJahControl;
}

void GLWorld::setJahControl(JahControl* jah_control)
{
  m_pJahControl = jah_control;
}


bool GLWorld::usesVideoHead( )
{ return true;
}

void GLWorld::setPointLayer(int index, JahLayer* layer)
{
  m_pJtrace->info( "GLWorld::setPointLayer");

  m_aPointLayers[index] = layer;
}

JahLayer* GLWorld::getPointLayer(int index)
{
  return m_aPointLayers[index];
}

bool GLWorld::usesKeyFrames( )
{
  return true;
}

void GLWorld::grabDesktop(void)
{
  emit GrabDesktop();
};

GLfloat GLWorld::setZoom( GLfloat value )
{
  return value;
}



void GLWorld::qt_lock( )
{
  //qApp->lock( ); //TODO: this is not defined
}

void GLWorld::qt_unlock( )
{
  //qApp->unlock( );  //TODO: this is not defined
}

JahToolButton* GLWorld::getStopButton()
{
  return m_pStopButton;
}


int GLWorld::getNumberEmbeddedCPUEffects()
{
  return m_nNumberEmbeddedCPUEffects; // accesssor to the collection...
}

int GLWorld::getNumberEmbeddedMeshEffects()
{
  return m_nNumberEmbeddedMeshEffects;
}

int GLWorld::getNumberEmbeddedGPUEffects()
{
  return m_nNumberEmbeddedGPUEffects;
}


std::vector<EffectInfo*>*   GLWorld::getCPUEffectsSelectionVector()
{
  return &m_aCPUEffectsSelectionVector;
}

std::vector<EffectInfo*>*   GLWorld::getMeshEffectsSelectionVector()
{
  return &m_aMeshEffectsSelectionVector;
}

std::vector<EffectInfo*>*   GLWorld::getGPUEffectsSelectionVector()
{
  return &m_aGPUEffectsSelectionVector;
}


GLWorld::CPUEffectFunction   GLWorld::getCPUEffectFunction(int i)
{
  return m_aCPUEffectsDispatchTable[i];
}

GLWorld::MeshEffectFunction
GLWorld::getMeshEffectFunction(int i)
{
  return m_aMeshEffectsDispatchTable[i];
}

GLWorld::GPUEffectFunction   
GLWorld::getGPUEffectFunction(int i)
{
  return m_aGPUEffectsDispatchTable[i];
}

JahLayer*   GLWorld::getCurrentEffectLayer()
{
  return m_pCurrentEffectLayer;
}

void GLWorld::setCurrentEffectLayer(JahLayer* layer)
{ 
  m_pCurrentEffectLayer = layer;
}

float       GLWorld::getCameraDistance()
{
  return m_fCameraDistance;
}

GlslShader*      GLWorld::getFragmentShader()
{
  return m_pFragmentShader;
}

GlslShader*      GLWorld::getVertexShader()
{
  return m_pVertexShader;
}

GlslProgram*     GLWorld::getShaderProgram()
{
  return m_pShaderProgram;
}

std::vector<GlslShader*>    GLWorld::getShaderVector()
{
  return m_aShaderVector;
}

GLhandleARB      GLWorld::getShaderProgramHandle()
{
  return m_hShaderProgramHandle;
}

bool  GLWorld::getGPUActive()
{
  return m_bGPUActive;
}

void GLWorld:: setGPUActive(bool flag)
{
  m_bGPUActive = flag;
}

GLuint*     GLWorld::getCompositeTextureIdPtr()
{
  return &m_hCompositeTextureID;
}

GLuint      GLWorld::getCompositeTextureId()
{
  return m_hCompositeTextureID;
}

void GLWorld::setCompositeTextureId(GLuint id)
{
  m_hCompositeTextureID = id;
}

float4*     GLWorld::getCompositeSizeRatioPtr()
{
  return &m_fCompositeSizeRatio;
}



void GLWorld::setFramesPerSecond(int fps)
{
  m_nFramesPerSecond=fps;
}


JahLayer  *GLWorld::getCamera()
{
  return m_pCamera;
}

int GLWorld::getX_RESOLUTION()
{
  return m_nX_RESOLUTION;
}

int GLWorld::getY_RESOLUTION()
{
  return m_nY_RESOLUTION;
}

