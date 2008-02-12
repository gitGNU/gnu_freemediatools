/*******************************************************************************
**
** The header file for the Jahshaka glworld module 
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef GLWORLD_H
#define GLWORLD_H

//////////////////////////////////////////////
//qt library includes here

#include <qwidget.h>
#include <qapplication.h>
#include <jahheadable.h>
class QFrame;
class QCheckBox;
class QComboBox;
class QButton;
class QPushButton;
class QTable;
class QSlider;
class QHBox;
class QVBox;
class QWidgetStack;
class QTimer;
class QListView;
class QCheckListItem;
class QSlider;
class QLabel;
class QLCDNumber;
class QMultiLineEdit;
class QLineEdit;
class QPopupMenu;
class QHBoxLayout;
class QToolButton;
class QMenuBar;
class QSplitter;
class QTextView;
class QHBoxLayout;
class QSpacerItem;

//custom qt widgets
class InputLCD;
class JahDesktopSideTable;
class SlickSlider;
class TimelineSlider;
class SliderButton;
class FancyPopup;
class JahTimer;
class JahToolButton;
class JahThemes;
class SupergRangeControl;

//////////////////////////////////////////////
//for OpenMediaLib asset support
#include "assetdata.h"

//forward references
namespace bxmlnode
{
    struct basicxmlnode ;
}
 
//////////////////////////////////////////////
//important core includes here

class JahLayer;

//for the Render class
#include "corestructures.h"
#include "coreeffects.h"
class JahRender;

//the different world lists
class ActiveLayerList;
class LayerListEntry;

//////////////////////////////////////////////
//forward definitions for trace and translation
class JahTrace;
class JahTranslate;

//////////////////////////////////////////////
//display widgets for jahshaka
//we nullify them when building jahplayer
//... ugh - horrible

#ifndef JAHPLAYER
class JahKeyframer;
class JahNodes;
#else
typedef struct { int null; } JahKeyframer; 
typedef struct { int null; } JahNodes; 
#endif

class GLCore;
class JahPluginLib;
class JahControl;

static const unsigned int JAH_XML_TAB_DEPTH = 4;
static const unsigned int MODULE_AUTO_SAVE_FREQUENCY = 60;  // Seconds
static const unsigned int NUMBER_OF_AXIS_SLIDERS = 13;
static const unsigned int NUMBER_OF_LIGHTING_SLIDERS = 12;

static const int MAX_TRACKING_POINTS = 4;
static const int MAX_TRACKING_HISTORY = 4;
static const float MAX_TRACKER_CIRCLE_RADIUS = 31;
static const unsigned int TRACKER_POINT_TEXTURE_WIDTH = static_cast<int>( (MAX_TRACKER_CIRCLE_RADIUS + 1.0f) * 2 );
static const float MAX_TRACKER_TRACKING_RADIUS = 200;

///////////////////////////////////////////////
//old array to sort layer rendering on the fly

struct worldSort
{
    int       laynum;
    GLfloat    depth;
};

/////////////////////////////////////////////
// main class

template < typename T > QCheckListItem* factory( T*, QListViewItem*, const QString& );

class GLWorld : public QWidget, public JahHeadable {
    Q_OBJECT

public:

    GLWorld( GLCore* jahcore, const char* name, QHBox* controller=0, int* globalclipnumber=0 );
    virtual ~GLWorld();

	// This method should be called before any attempt to raise a module
    void activate( );

    enum FRAMES_PER_SECOND
    {
        FRAMES_PER_SECOND_15,
        FRAMES_PER_SECOND_24,
        FRAMES_PER_SECOND_25,
        FRAMES_PER_SECOND_30,
        FRAMES_PER_SECOND_60,
        FRAMES_PER_SECOND_90
    };

protected:
    static std::vector<std::string> m_frames_per_second_name_vector;

public:
    virtual void headRender( int width = 0, int height = 0 );
    int getCurrentWidth( ) const;
    int getCurrentHeight( ) const;
    
    std::vector<std::string>* getFramesPerSecondNameVector();
    std::string getFramesPerSecondName(FRAMES_PER_SECOND index);
    
    // Should be called in the start by any module that needs plugins
    void initializePlugins( );
    
    //////////////////////////////////////////
    //gl commands to be sorted out later? when is later?
    virtual void makeCurrent(void);
    virtual void updateGL   (void);

    //build the modules interface
    virtual void buildController( QHBox* controller);

    void initializeWorld();
    void buildEffectsOptions();

    virtual void initializeObjects();

    void raiseCore();

    //initialize a mesh layer
    void        initializeMeshLayer(int layer);
    void        initializeMeshLayer(JahLayer* layer);

 protected:
    ////////////////////////////////////////
    //pointer to tracer singleton
    JahTrace            * m_pJtrace;

    ////////////////////////////////////////
    //pointer to translater singleton
    JahTranslate        * m_pJt;

 public:
    template < typename T > QCheckListItem* 
      addCheckListItem(T* parent,
		       bool status = TRUE,
		       bool headtail = FALSE,
		       QCheckListItem* (*fn)
		       (T*, QListViewItem*, 
			const QString& ) = &factory );

    //for gpu power status
    bool        m_bGPUSupport, // referece copy from prefs 
      m_bNV_GPUSupport;

 public:
    //main create layer function
    bool buildLayer (JahLayer* layer, QString layerName, LayerCategory::TYPE layertype, ObjectCategory::TYPE objtype );
    
    //added for easy layer configuration in the modules
    JahLayer* configureLayer(QString cname=0, 
                                bool cstatus=0, bool cblend=0, bool cdepth=0,
                                bool cassetstatus=0, QString cfilename=0, QString cextension=0 );


    //virtual functions are meant to be overridden in calling modules
    virtual void updateVideoTexture(JahLayer*, int);
    QImage  screenShot(int Frame,int xloffset,int xroffset,int ytoffset,int yboffset,
                        int xres, int yres, int number_of_base_layers);

    virtual void    mouseMoveCamera( int x,int y );
    virtual void    mouseMoveLayer ( int thelayer, int x, int y );
    virtual void    mouseMoveLayer ( JahLayer* jah_layer, int x, int y );
    virtual void    processSelection(GLuint *pSelectBuff);
    
    virtual void contextMenuEvent(QContextMenuEvent * e);

    virtual void keyPressEvent( QKeyEvent *k );
    virtual void keyReleaseEvent( QKeyEvent * );

    //////////////////////////////////////////////////////////////
    //ui code for menu items
    virtual void createMenuItem(QPopupMenu * );

    //////////////////////////////////////////////////////////////
    //for on screen stats feedback
    void drawOverlayStats(double fps);

 protected:
    bool m_bStatsShowStats;

 public:
    //////////////////////////////////////////////////////////////
    //for load and save of scenes

    //used for file loading layer creation
    bool loadLayer ( JahLayer* jah_layer, std::string layer_name, LayerCategory::TYPE layertype, ObjectCategory::TYPE objtype, 
                     bool isasset, assetData asset, bool haskey, assetData keyasset);

    bool loadLayer ( int nl, QString layerName, LayerCategory::TYPE layertype, ObjectCategory::TYPE objtype, 
                     bool isasset, assetData asset, bool haskey, assetData keyasset);

    //PackageSave() is a slot
    void  SaveAllAssets(QString destination); //used by PackageSave

    //test asset and locate
    void locateLoadAsset(assetData& asset_data,std::string const& file_name_string);

    //for xml scene save 
    void SaveAll( std::string const & a_save_filename, bool const a_status );
    void SaveAllData( std::fstream & ai_fstream );
    void saveSceneFile( std::string const & a_save_filename);
    void saveAllLayers( std::fstream & ai_strstream );
    void saveHeaders( std::fstream& fstream, std::string const& save_filename, std::string module_name);
    void saveEffectLayers( std::fstream & ai_fstream , JahLayer* jah_layer );
    void saveLightLayers( std::fstream & ai_fstream , JahLayer* jah_layer );
    void SaveNodes( std::fstream & ai_fstream , JahLayer* jah_layer );
    void SaveProperties( std::fstream & ai_fstream , JahLayer* jah_layer );
    void SaveCameraNodes( std::fstream & ai_fstream );
    void SaveColor( std::fstream & ai_fstream , unsigned int const & a_i ); 
    void SaveText( std::fstream & ai_fstream , unsigned int const & a_i ); 
    void SaveAsset( std::fstream & ai_fstream , JahLayer* jah_layer );
    void SaveKeyAsset( std::fstream & ai_fstream , unsigned int const & a_i );
    void SaveEffect( std::fstream & ai_fstream , unsigned int const & a_i );
    void SaveObjectData( std::fstream & ai_fstream , unsigned int const & a_i );
    void SaveParticleData( std::fstream & ai_fstream , unsigned int const & a_i );
    void SaveTrackerData( std::fstream & ai_fstream );

    const char*     sceneGetIndent();
    void            sceneIncreaseIndent() ;
    void            sceneDecreaseIndent() ;
    int             getSceneSaveIndentationDepth() ;
    void            setSceneSaveIndentationDepth(int depth) ;

    //for xml scene load
    bool LoadAll( std::string const& scene_file_string, bool clear_first, bool apply_effects, 
                  std::string const& file_name_string);
    void LoadAllData( bxmlnode::basicxmlnode const* const & l_xml_root );
    void loadSceneFile(std::string file_name );
    int  sceneFindNextTag(std::string& scene_string, const char* keyword);
    assetData LoadAsset( bxmlnode::basicxmlnode const* const & l_xml_root , int const & i ); 
    void LoadCameraNodes( bxmlnode::basicxmlnode const* const & a_layerstyle_node );
    void LoadLayerNodes( bxmlnode::basicxmlnode const* const & a_layerstyle_node , int const & i );
    void LoadEffect ( bxmlnode::basicxmlnode const* const& a_layerstyle_node , int const & i );
    assetData LoadKeyAsset ( bxmlnode::basicxmlnode const* const & a_keyasset_node , int const & i );
    void LoadParticleData ( bxmlnode::basicxmlnode const* const & a_layerstyle_node , int const & i );
    void LoadObjectData ( bxmlnode::basicxmlnode const* const & a_objects_node , int const & i ) ;
    void LoadColor ( bxmlnode::basicxmlnode const* const & a_text_node , int const & i ) ;
    void LoadText ( bxmlnode::basicxmlnode const* const & a_text_node , int const & i );
    bool getRenderOnlyToSelectedEffect();
    void setRenderOnlyToSelectedEffect(bool flag) ;
    std::string& getJahGlBlendModeStringVector(int index) ;
    GLuint getJahGlBlendModeValueVector(int index) ;
    bool getSceneSaveExport();
    void setSceneSaveExport(bool flag);

    void setXRotationSlider(JahLayer* jah_layer, int value);
    void setYRotationSlider(JahLayer* jah_layer, int value);
    void setZRotationSlider(JahLayer* jah_layer, int value);
    void setXTranslationSlider(JahLayer* jah_layer, int value);
    void setYTranslationSlider(JahLayer* jah_layer, int value);
    void setZTranslationSlider(JahLayer* jah_layer, int value);
    void setXScaleSlider(JahLayer* jah_layer, int value);
    void setYScaleSlider(JahLayer* jah_layer, int value);
    void setZScaleSlider(JahLayer* jah_layer, int value);
    void setTransparencySlider(JahLayer* jah_layer, int value);

    enum JAH_GL_BLEND_MODE
    {
        JAH_GL_ZERO,
        JAH_GL_ONE,
        JAH_GL_SRC_COLOR,
        JAH_GL_ONE_MINUS_SRC_COLOR,
        JAH_GL_SRC_ALPHA,
        JAH_GL_ONE_MINUS_SRC_ALPHA,
        JAH_GL_DST_ALPHA,
        JAH_GL_ONE_MINUS_DST_ALPHA,
        JAH_GL_DST_COLOR,
        JAH_GL_ONE_MINUS_DST_COLOR,
        JAH_GL_NO_BLEND_MODE
    };

    enum RENDER_QUALITY
    {
        RENDER_QUALITY_JPEG,
        RENDER_QUALITY_PNG,
        RENDER_QUALITY_BMP,
        RENDER_QUALITY_MLT_SELECT,
        RENDER_QUALITY_MLT_FLASH_2000_KBS_640X480,
        RENDER_QUALITY_MLT_FLASH_2000_KBS_320X240,
        RENDER_QUALITY_MLT_FLASH_VCD_1000_KBS,
        RENDER_QUALITY_MLT_FLASH_VCD_500_KBS,
        RENDER_QUALITY_MLT_FLASH_VCD_200_KBS,
        RENDER_QUALITY_MLT_FLASH_VCD_100_KBS,
        RENDER_QUALITY_MLT_FLASH_100_KBS_160X120,
        RENDER_QUALITY_MLT_FLASH_50_KBS_160X120
    };

    enum LAYER_TYPE
    {
        LAYER_3D_OBJECT,
        LAYER_PARTICLE,
        LAYER_LIGHT,
        LAYER_3D_TEXT,
        LAYER_IMAGE_LARGE,
        LAYER_IMAGE
    };


    std::vector<std::string> m_vJahGLBlendModeStringVector;
    std::vector<GLuint> m_vJahGLBlendModeValueVector;

protected:
	// Module defined ui start method
    virtual void start( );

    int   m_nSceneSaveIndentationDepth;

    bool  m_bDontTrack;
    bool  m_bRenderOnlyToSelectedEffect;
    bool  m_bSceneSaveExport;
    bool  m_bStopRequested;

    JahLayer*       m_pBaseLayer;

    JahLayer*	    m_aPointLayers[MAX_TRACKING_POINTS];

    bool            m_bShowLights;
    bool            m_bAnimationUpdated;
    bool            m_bDontPaint;

public:
    bool            getAnimationUpdated() ;
    void            setAnimationUpdated(bool flag) ;
    virtual void    createTrackerPointLayers();
    JahLayer*       getBaseLayer() ;
    bool            getStopRequested();
    void            setStopRequested(bool flag);
    bool            getShowLights();
    void            setShowLights(bool flag);
    bool            getUseAutoKey();
    void            setUseAutoKey(bool flag);

    void            startSingleShotTimer();
    bool            getDontPaint() ;
    void            setDontPaint(bool flag) ;

    virtual JahLayer*  createALayer(LAYER_TYPE type, JahLayer* parent);


    //////////////////////////////////////////////////////////////
    //custom ui objects
    void createAxisPanel ( QFrame* theuiframe);

    bool            getDontTrack() ;
    void            setDontTrack(bool flag);

    virtual QCheckBox* getKeySelect();

    void  loopThroughLayers(std::string message);

    ///////////////////////////////////////////////////////////////////
    //used for lights   
    void updateLights( void); 

    ///////////////////////////////////////////////////////////////////
    //used to set up custom widgets
    void                initializeNodes(QHBox* parent2);

 protected: // protect them!
    ///////////////////////////////////////////////////////////////////
    // systemwide variables
    ///////////////////////////////////////////////////////////////////
    
 /////////////////////////////////////////
    //the opengl core
    GLCore          *m_pCore;
    
    int             m_nX_RESOLUTION; // from the project
    int             m_nY_RESOLUTION; // from the project
    double          m_fPIXEL_RATIO;  // from the project

 protected:
    //////////////////////////////////////////
    //for the linked list for dynamic object layers
    QPtrList<LayerListEntry>*       m_pLayerListEntryList;
    //    QPtrList<JahLayer>*             m_pJahLayerList;

 public:
    QPtrList<LayerListEntry>*   getLayerList() ;
    GLCore*                     getCore() ;

    int             getNumberOfLayers();
    JahLayer*       getJahLayer(int layer_number);

    int             getJahLayerNumber(JahLayer* jah_layer);
    LayerListEntry* findLayerListEntry(QListViewItem* item);
    LayerListEntry* findLayerListEntry(JahLayer* jah_layer);

 protected:
    //new linked list for layer sorting
    // haha NWO new world order!
    QPtrList<worldSort> * m_pNewworldorder;

 public:
    //new shortcut routine for modules
    /// \warning bad overload as an int can be silently converted to a pointer.
    void initializeListview(int layer_number, bool layer_status);
    virtual void initializeListview(JahLayer* jah_layer, bool layer_status);

    //////////////////////////////////////////
    public :

      // new access methods
    JahLayer  *getCamera();
    int getX_RESOLUTION();
    int getY_RESOLUTION();

 protected:
    //default module variables
    QString    m_qsModuleName; // Paint|"Effects"|"Animation"
	QHBox* m_pController;
    JahLayer  *m_pGrid;
    JahLayer  *m_pTitleSafe;
    JahLayer  *m_pTextSafe;
    JahLayer  *m_pCamera;

    QCheckBox*              m_pShowLightsCheckBox;
    QCheckBox*              m_pUseAutoKeyCheckBox;
    QCheckBox*              m_pUseOpenGLKeyLayerCheckBox;
    QCheckBox*              m_pUseFastShaderKeyLayerCheckBox;

    ActiveLayerList  *m_pActiveLayers;

    bool m_bSlidersHaveChanged;
    //bool forceupdatefxbuffer;

    int m_nNumberOfLights;


 public:
    QCheckBox*              getShowLightsCheckBox() ;
    QCheckBox*              getUseAutoKeyCheckBox() ;
    bool                    useOpenGLKey();
    bool                    useFastShaderKey();
    bool                    getLockKeyAndClip();

    QTimer*     getAutoSaveTimer() ;
    bool        getAutoSaveInProgress();
    void        setAutoSaveInProgress(bool flag);
    static const char*      getTrackerFragmentShader();

    bool        getModuleUsesLighting() ;
    void        setModuleUsesLighting(bool flag);

    QSlider*    getLightingSliderPtr(int slider_number) ;
    InputLCD*   getLightingLcdPtr(int slider_number) ;
    QLabel*     getLightingSliderLabelPtr(int slider_number);
    bool        getUseLighting();

    void            hideAllLightingSliders();
    void            showAllLightingSliders();
    bool            getSlidersHaveChanged() ;
    void            setSlidersHaveChanged(bool flag);
    JahLayer*       getImageLayer() ;
    void            setImageLayer(JahLayer* layer) ;
    virtual void    configureModuleEngine(void);
    int             getFramesPerSecond() ;
    void            setFramesPerSecond(int value);
    QCheckBox*      getUseLightingCheckBox() ;
    QCheckBox*      getTranslateFirstCheckBox() ;
    JahLayer*       getFirstJahLayer();
    bool            imageLayerNeedsEffectsRedraw(JahLayer* image_layer);

protected:
    QTimer*                 m_pAutoSaveTimer;
    bool                    m_bAutoSaveInProgress;
    static const char*      m_sTrackerFragmentShader;
    QCheckBox*              m_pUseLightingCheckBox;
    QCheckBox*              m_pTranslateFirstCheckBox;
    JahLayer*               m_pImageLayer;
    int                     m_nFramesPerSecond;


 protected: // was public, why have member variables public?

    //controls the rendering of objects in a scene
    JahRender* m_pRenderSpace;

    //the timer controls screen redraw
    QTimer* m_pTimer;
    double m_fJahFPS;
    
    //The core timer object for FPS
    JahTimer    *m_pCoreTimer;
   
    //set up defalt variables here
    QString m_qsRenderPath;
    QString m_qsScenePath;

    //base path and media path
    QString m_qsJahBasePath;
    QString m_qsJahMediaPath;

    bool m_bGridVal; 

    bool m_bPolySmooth, 
         m_bSelect;

    int             * m_pClipNumber;            //used to track global clipnumber

    JahLayer*       m_pActiveLayer;
    int             m_nCurrentFrame; 
    int             m_nLastLayer;

 public:
    int                 getCurrentFrame() ;
    void                setCurrentFrame(int frame_number);
    void                incrementCurrentFrame(int step);
    JahLayer*           getActiveJahLayer();
    void                setActiveJahLayer(JahLayer* layer);
    LayerListEntry*     getActiveLayer();
    int                 getActiveLayerNumber();
    EffectLayer*        getActiveEffectLayer();
    bool                activeLayerIsAnEffect();
    void                drawAllLayers(int number_of_base_layers, 
			  bool draw_only_to_selected_effect = false);

    void                buildEffects(JahLayer* image_layer,
			 bool draw_only_to_selected_effect = false);

    bool                canAcceptEffects(ObjectCategory::TYPE object_category);
    bool                isQimageBlack(QImage& image, char* message);
    bool                multipleLayersSelectedInListView();
    void                addMultiSelectKeyNodes();
    void                removeMultiSelectKeyNodes();
    void                saveEffects(std::string& file_name,
				    JahLayer* jah_layer);
    bool                getAnimating() ;
    void                setAnimating(bool flag);
    std::string&        getRenderOutputFileNameString();
    void                setRenderOutputFileNameString(std::string name);
    bool                getStopScript() ;
    void                setStopScript(bool flag) ;
    void                set (bool flag) ;
    int                 getRedrawDelay() ;
    void                setRedrawDelay(int delay) ;

 protected:

    std::string         m_sRenderOutputFilename;

    bool                m_bStopScript;

    int             m_nMinFrame,
                    m_nMaxFrame;

    bool            m_bAnimation;
    bool            m_bAnimSliderSelected;
    int             m_nRedrawDelay;

    int             m_nLastX,
                    m_nLastY;

    QString         m_qsRenderExt;
    QString         m_qsRenderFormat;
    int             m_nRenderQuality;
    int             m_nJahRenderer;
    int             m_nJahResolutionValue;


#ifndef HIDE_GUI
    QLabel          *m_pJahResLabel;

    // TODO : pair of (widget and bool)
    QComboBox       *m_pJahResCombo; //controlled by  m_bHasResMenu
    bool            m_bHasResMenu; // alway false, read once. never updated

#endif

    MouseMode       m_MMode;                  //mouse selection mode

    bool            m_bHWFOG;                  //for hardware fog support,
    //from the pref
    bool            m_bHWALIASING;             //for hardware antialiasing
    //support. from the prefs
    
    int             m_nNumUiOptions;           //for ui options
    bool            m_bForcePlay;              //for realtime videlayer playback
    bool            m_bHWAliasing;             //for hardware anti aliasing

    bool            m_bUpdateEffect;           //for effects
    bool            m_bOldJahLoadfile;         //for old file loading support
    bool            m_bMousePressed;           //for mouse tracking

    
    bool            m_bHasKeyFramer;           //interface widgets
    JahKeyframer   * m_pKeyFramer;          //the keyframer
    bool            m_bHasNodeFramer;
    bool            m_bHasWireUp;

#ifdef NEWWIREUPWIDGET
    WireWidget      *m_pWireUp;         //the v3 test nodeframer
#else
    // what I am compiling: MIKE 
    JahNodes        *m_pNodeFramer;         //the nodeframer
#endif

    JahDesktopSideTable *m_pMediatable;
    
    //used to load the plugins in
    static JahPluginLib*   m_pJPlugIn;

    //for global fog
    GLfloat m_fHFog, m_fVFog;

    ///////////////////////////////////////////////////////////////////
    // ui variables and such

#ifndef HIDE_GUI    
    QWidget         *m_pPage;

    //
    QCheckBox       *m_pForcePlayCheckBox;

    // god, why do we not put this in its own class?
    QPushButton     *m_pControllerPreviousButton;
    QPushButton     *m_pControllerStopButton;
    QPushButton     *m_pControllerPlayButton;
    QPushButton     *m_pControllerFFWardButton;
    QPushButton     *m_pControllerNextButton;
    QPushButton     *m_pControllerRewindButton;

#endif
    /// the main slider/timeline controller
    /*
      is informed about the FPS
       in set resolution and in lazy
       not really slider but the main controller slider

       see updateTimeSlider is called from outside!

void GLWorld::sliderValueChanged(int)

    */
#ifndef HIDE_GUI
    TimelineSlider  *m_pControllerSlider;

    SupergRangeControl* m_pControllerStartFrameControl;
    SupergRangeControl* m_pControllerEndFrameControl;

    QPushButton     *m_pKeyFrameButton;
    QLCDNumber      *m_pTimeCodeDisplay;
    QFrame          *m_pImagingFrame;
    QPushButton     *m_pImagingPhotoButton;
    QPushButton     *m_pImagingRenderButton;

    //protected:
    QHBoxLayout     *m_pPageLayout;

    //these have been moved here
    QPushButton*   m_pGetButton;

    //////////////////////////////////////
    //for the new dynamic label groups
    bool m_bHasLabels;
    QLabel          * m_aJahModuleLabels[5];
    QString         m_aJahModuleLabelTexts[5];

    bool m_bHasSliders; // never used, alsway falsee

    SliderButton    *m_aJahSliders[NUMBER_OF_AXIS_SLIDERS];
    SliderButton*   m_text_extrude_slider;

    QLabel*         m_pText_extrude_label;
    InputLCD*       m_pText_extrude_lcd;

    InputLCD        * m_aJahSliderLCDs[NUMBER_OF_AXIS_SLIDERS];
    QLabel          * m_aJahSliderLabels[NUMBER_OF_AXIS_SLIDERS];

    QString         m_aJahSliderLabelTexts[NUMBER_OF_AXIS_SLIDERS];

    bool m_bHasButtons; // always false
    QCheckBox       *m_aJahModuleOptions[10];
    QMultiLineEdit     * m_pTextEd;

    QCheckBox*      m_pSelectColorsIndividuallyQCheckBox;

    QCheckBox*      m_pBufferSelectCheckBox;

    // are these ever used?
    InputLCD          *m_pSlipFramesLCD;
    InputLCD          *m_pInFramesLCD;
    InputLCD          *m_pOutFramesLCD;
    InputLCD          *m_pKeySlipFramesLCD;
    InputLCD          *m_pKeyInFramesLCD;
    InputLCD          *m_pKeyOutFramesLCD;

    QCheckBox*        m_pLockKeyAndClipCheckBox;
#endif

 public:
    ///////////////////////////////////////////////
    //for the onscreen buffer
    virtual void  updateEffects();


    JahToolButton* getStopButton() ;
    

 protected:
    //////////////////////////////////////////////////////////////////////
    //controller variables



    int    m_nAstartframe,
      m_nAEndFrame, 
      m_nAAnimFrame;


#ifndef HIDE_GUI

    QListView*      m_pLayerListView;
    QHBox           *m_pMiddleFrame;  
    QSlider *     m_pSlider2;
    SupergRangeControl*    m_pAnimFrameLCD;   

    // display the minium frame
    SupergRangeControl*    m_pStartFrameControl;

    // the last frame
    SupergRangeControl*    m_pEndFrameControl;

    QCheckBox       *m_pForcedplay;

    JahToolButton   *m_pScrubfirst, 
      * m_pScrubprevious, 
      * m_pScrubstop, 
      * m_pScrubplay, 
      * m_pScrubnext, 
      * m_pScrublast;
    
    JahToolButton   *m_pKeyback, 
      *m_pKeyadd, 
      *m_pKeyremove, 
      *m_pKeynext;

    JahToolButton*  m_pStopButton;

    QPushButton     *m_pScrubKey, 
      * m_pScrubRenderSlides;
    JahToolButton   * m_pScrubRender, 
      * m_pScrubRenderAll;

    QPushButton*    m_pDeleteKeyFrameButton;
    QPushButton*    m_pAddKeyFrameButton;
    QPushButton*    m_pPrevKeyFrameButton;
    QPushButton*    m_pNnextKeyFrameButton;

    //buttons that control the listbox
    QPushButton     *m_pAddButton, 
      *m_pAddFxButton,
      *m_pDelButton, 
      *m_pSceneButton;

    QPushButton*    m_pRefresh_button;
    QPushButton*    m_pUndo_button;
    QPushButton*    m_pRedo_button;
    QPushButton*    m_pDelete_button;
    QPushButton*    m_pCopy_one_button;
    QPushButton*    m_pCopy_all_button;
    QPushButton*    m_pPaste_button;
    QPushButton     *m_pNameButton;
    QPushButton     *m_pMoveupButton, 
      *m_pMovedownButton;
    FancyPopup      *m_pNamePopUp;

    //scene control variables
    QWidgetStack    *m_pControlpanel;
    QFrame          *m_pScene_controls;
    QFrame          *m_pObject_controls;

    //interface stacks
    //need to switch to an array
    QWidgetStack    *m_pObjectControlStack;
    QHBox           *m_aObjectControls[10];

    QCheckBox*      m_pGPUSelect;
    QPushButton*    m_pSelectFPSPushbutton;
    QLabel*         m_pSelectFPSLabel;

    QToolButton*    m_pSrcBlendModeSelectButton;
    QToolButton*    m_pDstBlendModeSelectButton;
    QToolButton*    m_pReset_blendModeButton;

    QLabel*         m_pSrc_BlendModeSelectLabel;
    QLabel*         m_pDst_BlendModeSelectLabel;
    QLabel*         m_pReset_blendModeLabel;

    QPushButton*    m_pCompositingModeSelectButton;
    //    QLabel*         m_pCompositingModeSelectLabel; // not used!
    QLabel*         m_pCompositingModeLabel;

    bool            m_bModuleUsesLighting;

    QSlider*        m_aLightingSliders[12];
    InputLCD*       m_aLightingLCDs[12];
    QLabel*         m_aLightingSliderLabels[12];

    QPushButton*    m_pRunScriptButton;
    QPushButton*    m_pSaveAsExportButton;

#endif 

public:
    QPushButton*                getRunScriptButton();
    virtual void                chooseNewCPUEffect(LayerListEntry* parent, QWidget* thebutton, int x_offset = 0, int y_offset = 0);
    virtual void                addCPUEffectLayer(int effect_id, JahLayer* parent_jah_layer, const char* effect_name);
    virtual void                chooseNewMeshEffect(LayerListEntry* parent, QWidget* thebutton, int x_offset = 0, int y_offset = 0);
    virtual void                addMeshEffectLayer(int effect_id, JahLayer* parent_jah_layer, const char* effect_name);
    virtual void                chooseNewGPUEffect(LayerListEntry* parent, QWidget* thebutton, int x_offset = 0, int y_offset = 0);
    virtual void                addGPUEffectLayer(int effect_id, JahLayer* parent_jah_layer, const char* effect_name);
    
    EffectLayer*        addEffectLayer(LayerListEntry* parent, QString& title, 
                                       EffectInfo::EFFECT_TYPE effect_type, int plugin_id, std::string& guid);
    EffectLayer*        addEffectLayer(JahLayer* parent, std::string& title, 
                                       EffectInfo::EFFECT_TYPE effect_type, int plugin_id, std::string& guid);
    int                 addPluginsToMenu(int usingclass, int existingitems, QPopupMenu *menu);
    void                setPluginAxisSliderLabels(EffectLayer* effect_layer);
    virtual void        setDefaultAxisSliderLabels();
    virtual void        setSliderNames(EffectLayer* effect_layer);
    virtual void        applySliderNames(EffectLayer* effect_layer);

    virtual void        setLayerName(JahLayer* jah_layer, const char* layer_name);


    void parsePluginOption(int option);  //used for path global for layers


    void                doNegative(JahLayer* base_layer, QImage& theimage);
    void                doSwapRGB(JahLayer* base_layer, QImage& theimage);
    void                doMirror (JahLayer* base_layer, QImage& theimage);

    //math based functions
    void                doGrey(JahLayer* base_layer, QImage& pix);
    void                doContrast(JahLayer* base_layer, QImage& pix );
    void                doBrightness(JahLayer* base_layer, QImage& pix );

    void                doMatrixSharpen(JahLayer* base_layer, QImage& pix);
    void                doMatrixBlur(JahLayer* base_layer, QImage& pix);
    void                doMatrixEmboss(JahLayer* base_layer, QImage& pix);

    void                doEdge(JahLayer* base_layer, QImage& pix);
    void                doColorSelect(JahLayer* base_layer, QImage& pix);
    //void doBlur(QImage& pix);
    void                doGaussianBlur(JahLayer* base_layer, QImage& pix);

    void                doCPUColorize(JahLayer* base_layer, QImage& pix);
    void                doChromaKeyerCPU(JahLayer* key_layer, QImage& pix);
    void                CPUChromaKeyer(JahLayer* key_layer, int buffer_width, int buffer_height, GLuint key_texture_id);

    //opengl accelerated
    void                doColorMatrixRt(JahLayer* base_layer);
    void                doBlurRt(JahLayer* base_layer);
    void                doSharpenRt(JahLayer* base_layer);
    void                doContrastRt(JahLayer* base_layer);

    //opengl mesh accelerated functions
    void                doFlagRt(JahLayer* base_layer);
    void                doRippleRt(JahLayer* base_layer);
    //void doBaseLayer(void);

    //opengl geometry based mesh effects
    void                doSwirlRt(JahLayer* base_layer);
    void                doFisheyeRt(JahLayer* base_layer);

    //////////////////////////////////////////////////////////////
    //gpu based effects
    void                doNV30CharcoalGPU(JahLayer* base_layer);
    void                doNV30LightingGPU(JahLayer* base_layer);
    void                doNV30EarthquakeGPU(JahLayer* base_layer);

    void                doNV30Fire3DGPU(JahLayer* base_layer);

    void                doColorizeGPU(JahLayer* base_layer);
    void                doNV30ColorizeGPU(JahLayer* base_layer);
    void                doArbColorizeGPU(JahLayer* base_layer);

    void                doChromaKeyerGLSLSeparateAlpha(JahLayer* key_layer);
    void                doChromaKeyerGLSL(JahLayer* key_layer);

    void                doColorCorrectorGLSL(JahLayer* image_layer);
    void                doStabilizeGLSL(JahLayer* image_layer);

    void                doStretchCrop(JahLayer* image_layer);

    


    // GPGPU effects
    void                doNV30FisheyeGPGPU(QImage& pix);
    void                doNV30SwirlGPGPU(QImage& pix);

    void                getMeshNormal(JahLayer* base_layer, float4& normal, int mesh_x_coordinate, 
                                      int mesh_y_coordinate, int mesh_x_dimension, 
                                      int mesh_y_dimension);

    void                doPlugin(JahLayer* image_layer, int plugin_number, QImage& pix);
    void                udpatePluginSettings(int pl);
    static bool         updateQimageFromTexture(QImage* q_image, TextureImage& texture_image);
    static bool         updateTextureFromQImage(TextureImage& texture_image, QImage* q_image, JahLayer* jah_layer);
    static bool         updateQimageFromRgbaBuffer(QImage* q_image, unsigned int* image_buffer, int image_width, int image_height);

 protected:

    bool        m_bNoiseMatrixInitialized; // was noiseMatrixInitialized
    int*        m_pPermutationTable;

    float4*     m_pGradientTable;

    bool        m_bFireTextureInitialized;
    bool        randomTextureInitialized;
    bool        contrastTextureInitialized;

    GLuint      m_nGradientTexture;
    GLuint      permutation_texture;
    GLuint      turbulence_texture;
    GLuint      random_texture;
    GLuint      contrast_texture;
    float       m_fZOffset;
 public:

    typedef void (GLWorld::*CPUEffectFunction)(JahLayer* base_layer, QImage&);
    typedef void (GLWorld::*MeshEffectFunction)(JahLayer* base_layer);
    typedef void (GLWorld::*GPUEffectFunction)(JahLayer* base_layer);

    static int getNumberEmbeddedCPUEffects() ;
    static int getNumberEmbeddedMeshEffects() ;
    static int getNumberEmbeddedGPUEffects() ;

    std::vector<EffectInfo*>*   getCPUEffectsSelectionVector() ;
    std::vector<EffectInfo*>*   getMeshEffectsSelectionVector() ;
    std::vector<EffectInfo*>*   getGPUEffectsSelectionVector() ;

    CPUEffectFunction   getCPUEffectFunction(int i) ;
    MeshEffectFunction  getMeshEffectFunction(int i) ;
    GPUEffectFunction   getGPUEffectFunction(int i) ;

    void        callEffect(EffectInfo::EFFECT_TYPE effect_type, JahLayer* effect_layer, 
                           JahLayer* base_layer, QImage& newimage);

    JahLayer*   getCurrentEffectLayer() ;
    void        setCurrentEffectLayer(JahLayer* layer);
    float       getCameraDistance();

    GlslShader*                 getFragmentShader() ;
    GlslShader*                 getVertexShader() ;
    GlslProgram*                getShaderProgram() ;
    std::vector<GlslShader*>    getShaderVector() ;
    GLhandleARB                 getShaderProgramHandle() ;
    bool                        getGPUActive() ;
    void                        setGPUActive(bool flag) ;
    int                         findLayer(JahLayer* layer);
    int                         findEffectParent(JahLayer* layer);
    void                        executeEffectsList(JahLayer* image_layer, bool use_pbuffer, bool draw_only_to_selected_effect = false);


    GLuint*     getCompositeTextureIdPtr() ;
    GLuint      getCompositeTextureId() ;
    void        setCompositeTextureId(GLuint id) ;
    float4*     getCompositeSizeRatioPtr() ;
    void        glslChromaKeyer(JahLayer* key_layer, GLuint base_texture_id, GLuint key_texture_id);
    void        glslChromaKeyer2(JahLayer* key_layer, GLuint key_texture_id);
    void        saveEffectRenderingInBaseTexture(JahLayer* base_layer);
    bool        setTextureImageParameters(TextureImage* texture_image, int texture_width, int texture_height, 
                         int buffer_width, int buffer_height, GLuint texture_id);

protected:
    GLuint          m_nSrcBlendFactor;
    GLuint          m_nDstBlendFactor;

public:
    GLuint          getSrcBlendFactor() ;
    GLuint          getDstBlendFactor() ;
    void            setSrcBlendFactor(JAH_GL_BLEND_MODE factor);
    void            setDstBlendFactor(JAH_GL_BLEND_MODE factor);
    JahControl*     getJahControl();
    void            setJahControl(JahControl* jah_control) ;
#ifndef JAHPLAYER
    void            runScript(std::string& script_file_name);
#endif
    void            setCompositeType(CompositeType::TYPE type);
    void            updateCompositeButton(CompositeType::TYPE type);

protected:
    static int          m_nNumberEmbeddedCPUEffects;
    static int          m_nNumberEmbeddedMeshEffects;
    static int          m_nNumberEmbeddedGPUEffects;

    static const char*  m_szColorCorrectionFragmentShader;

    std::vector<CPUEffectFunction>      m_aCPUEffectsDispatchTable;
    std::vector<MeshEffectFunction>     m_aMeshEffectsDispatchTable;
    std::vector<GPUEffectFunction>      m_aGPUEffectsDispatchTable;

    std::vector<EffectInfo*>        m_aCPUEffectsSelectionVector;
    std::vector<EffectInfo*>        m_aMeshEffectsSelectionVector;
    std::vector<EffectInfo*>        m_aGPUEffectsSelectionVector;

    static bool         sm_bNoiseMatrixInitialized;
    static bool         sm_bContrastTextureInitialized;
    static bool         sm_bRandomTextureInitialized;
    static int*         sm_pPermutationTable;
    static float4*      sm_pGradientTable;

    static GLuint       sm_nGradientTexture;
    static GLuint       sm_nPermutationTexture;
    static GLuint       sm_nTurbulenceTexture;
    static GLuint       sm_nTandomTexture;
    static GLuint       sm_nContrastTexture;

    JahLayer*           m_pCurrentEffectLayer;
    float               m_fCameraDistance;
    bool                m_bGPUActive;
    
    GLuint              m_hCompositeTextureID;
    float4              m_fCompositeSizeRatio;

    bool                    m_bSelectColorsIndividually;
    bool                    m_bGPUSelect;
    GLuint                  m_hBaseTextureID;
    GLuint                  m_hKeyTextureID;
    bool                    m_bTexturesInitialized;
    bool                    m_bUseTexturesForReadback;

    // struct
    GLint                   m_hRedLimitsHandle;
    GLint                   m_hGreenLimitsHandle;
    GLint                   m_hBlueLimitsHandle;

    GLint                   m_hSelectColorsIndividually_handle;
    GLint                   m_hCreateChromaAlphaMaskHandle;
    GLint                   m_hClearColorHandle;
    GLint                   m_hCsoftenValueHandle;

    // struct color red,green,blue,alpha TAGChromaKeyerRange 
    float                   m_fRedLow; // range
    float                   m_fRedHigh;

    float                   m_fGreenLow; // range
    float                   m_fGreenHigh;

    float                   m_fBlueLow; // range
    float                   m_fBlueHigh;

    float                   m_fAlphaLow; // range TAGChromaKeyerRange 
    float                   m_fAlphaHigh;

    float                   m_fSoftenValue; // parrt of TAGChromaKeyerRange 

    float4                  m_f4ClearColor;

    JahLayer*               m_pChromaBaseLayer;
    JahLayer*               m_pChromaKeyLayer;
    JahLayer*               m_pChromaSettingsLayer;
    JahControl*             m_pJahControl;

    // was unnamed. that is bad
    struct TAGChromaKeyerRange 
    {
        int         red_low;
        int         red_high;
        int         green_low;
        int         green_high;
        int         blue_low;
        int         blue_high;
        int         alpha_low;
        int         alpha_high;
        int         soften_value;
    } m_ChromaKeyerRange;



    std::vector<GlslShader*>    m_aShaderVector;
    GlslShader*                 m_pVertexShader;
    GlslShader*                 m_pFragmentShader;
    GlslProgram*                m_pShaderProgram;
    GLhandleARB                 m_hShaderProgramHandle;

	int m_nCurrWidth;
	int m_nCurrHeight;

    bool m_bRenderAtImageResolution;

public:



	// Used by the implementation of scheduleUpdate
	virtual void updatePosition( );
	virtual bool usesVideoHead( );
	virtual void schedulePosition( );
    virtual void setObjectTab( JahLayer* jah_layer ); 
    virtual void setPointLayer(int index, JahLayer* layer);
    virtual JahLayer* getPointLayer(int index);


protected:
    virtual bool usesKeyFrames( );

/////////////////////////////////////////////////////
// need to figure out what is not a slot and make
// sure its not under 'slots'

public slots:
   	virtual void scheduleUpdate( );

    void slotUpdateAnimation();
    void slotSetUseOpenGlKeyer();
    void slotSetUseFastShaderKeyer();

	void slotSetSrcBlendFactor();
    void slotSetDstBlendFactor();
    void slotResetBlendFactor();
    void slotSetCompositingMode();

    void slotLightColorAmbientRed();
    void slotLightColorAmbientGreen();
    void slotLightColorAmbientBlue();

    void slotLightColorDiffuseRed();
    void slotLightColorDiffuseGreen();
    void slotLightColorDiffuseBlue();

    void slotLightColorSpecularRed();
    void slotLightColorSpecularGreen();
    void slotLightColorSpecularBlue();

    void slotLightPosition();

    virtual void slotLightColorAmbient(QColor color);
    virtual void slotLightColorDiffuse(QColor color);
    virtual void slotLightColorSpecular(QColor color);

    void slotChooseFramesPerSecond();
    void slotSetShowLights();


    //used to get assets from the desktop
    void grabDesktop(void)      ;

    //opengl routine
    virtual void paintGL();

    virtual void    layerClicked(QListViewItem* item);
    virtual void    effectNodeClicked( int layernumber );
    
    virtual void    mousePressEvent( QMouseEvent *e);
    virtual void    mouseReleaseEvent( QMouseEvent *e );
    virtual void    mouseMoveEvent( QMouseEvent  *e );
    virtual void    setSelectColorsIndividually(void);

    virtual void connectKeyframer( void ) ;
    virtual void addKeyframer( QWidget* parentwidget ) ;

    virtual void setResolution(int renval);
    virtual void updateResMenu(int renval);

    // layer controls
    virtual void setXRotation( int degrees );    
    virtual void setYRotation( int degrees );    
    virtual void setZRotation( int degrees );
    
    virtual float setScaleValue ( int   );
    virtual int   getScaleValue ( float );
    
    virtual void setXScale   ( int degrees );    
    virtual void setYScale   ( int degrees );    
    virtual void setZScale   ( int degrees );
    virtual void setXTrans   ( int degrees );    
    virtual void setYTrans   ( int degrees );    
    virtual void setZTrans   ( int degrees );
    virtual void setAlpha    ( int degrees );    
    virtual void setZoom     ( int degrees );    
    virtual void setExtrude  ( int degrees );

    virtual void setColor(void); 

    virtual void setKeyframeDrawStatus(void);
    virtual void slotSetLayerSelectedVisible();
    virtual void resetAllSliders(void);

    //for spaceball
    virtual void spaceEvent  ( double x, double y, double z, double a, double b, double c );

    //for 3d objects
    virtual void loadObjObject(void);

    // for video clips
    virtual void slotSetSlipFrames    ( int frame );
    virtual void setinFrames      ( int frame );
    virtual void setoutFrames     ( int frame );
    virtual void slotSetKeySlipFrames   ( int frame );
    virtual void setinKeyFrames   ( int frame );
    virtual void setoutKeyFrames  ( int frame );

    //for editing
    virtual void noTool( void );    
    virtual void toolTranslate( void );    
    virtual void toolScale( void );     
    virtual void toolRotate( void );
    virtual void changeZoom( int val );
    virtual void resetZoom( void );
    virtual GLfloat setZoom( GLfloat value );

    virtual void setHFog(int hf);
    virtual void setVFog(int vf);
    virtual void updateFog(void);

    virtual void SetGPUSelect(void);

    virtual void updateToolDisplay(void);

    virtual void setKeyStatus(void);
    virtual void setClipStatus(void);
    
    virtual void  toggleForegroundStatus();
    virtual void  toggleDepthStatus();

    void            setTextLayerText(JahLayer* text_layer, const char* text_string);
    void            setText(const char* text_string);

    virtual void  setText(const QString& q_string);    void  setFontColor( void );    void  loadFont( void );
    virtual void  setTextSafe( void );

    virtual void  setGrid( void );
    virtual void  setSmooth( void );
    virtual void  setBlur( void );
    virtual void  setFog(void);

    virtual void  InvertKeyData(void);

    ///////////////////////////////////////////////////////////////////
    //slider control
    virtual void  updateSliders();      //to be overridden...
    virtual void  slotUpdateListViewCheckBoxes(); 
    virtual void  updateSliderValues(); //to be overridden...
    virtual void  updateSliders(motionNode*); //to be overridden...
    virtual void  updateUiOptions();    //to be overridden...
    virtual void  updateUiSettings();   //to be overridden...

    virtual void hideAllHeadings( );
    virtual void showAllHeadings( );
    virtual void showHeadings(int ns );
    virtual void setHeadingValue(int heading, QString value);
    virtual void setDefaultHeadings(void);

    virtual void hideAllSliders( );
    virtual void hideSliders( int ns );
    virtual void showAllSliders( );
    virtual void showSliders(int ns );
    virtual void showSlider(int, bool);

    virtual void setSliderValue(int slider, int value);
    virtual void setSliderLabelValue(int slider, QString value);
    virtual void setDefaultSliderText(void);


    virtual void showOptions(int ns );
    virtual void hideOptions(int ns );

    void setOptionValue(int option, bool value);

    virtual void  sliderselected();
    virtual void  slotTimeSliderReleased();
    virtual void  updateTimeSlider(int value);
    virtual void  sliderValueChanged(int value);

    virtual void  updatesliderStartframe(int);
    virtual void  updatesliderEndframe(int);

    virtual void toggleForcePlay(void);
    virtual void setForcePlay(bool status);

    virtual void firstframeanimation(void);
    virtual void previousframeanimation(void);
    virtual void nextframeanimation(void);
    virtual void lastframeanimation(void);

    //for when you hit the play and stop buttons
    virtual void  startanimation();
    virtual void  stopanimation();
    virtual void  toggleAnimation(bool value);

    virtual void  updateAnimation(int Frame, bool force = false);

    //key frame routines
    virtual void  addKeyframe();
    virtual void  removeKeyframe();
    virtual void  prevKeyframe();
    virtual void  nextKeyframe();
    virtual void  slotKeyframeChanged();
    virtual void  updateKeyframeDisplay();

    virtual bool isLayerVisible(JahLayer* jah_layer);

    //used to render from module out to desktop
    virtual QImage  Photo(assetData);

    virtual QString buildFrameName(int frameval, assetData theclip );

    virtual QImage  RenderAll(assetData,int,int);
    
    virtual void  renderGL(void);
    virtual void  Render(void);
    virtual void  RenderScene(void);
    virtual void  setSceneToRender(bool renStatus);

    virtual QString checkandcleandir(QString destDir, int clipnum);

    virtual void  checkRenderQuality(void);
    virtual void  checkJahRenderer(void);
    virtual void  setRenderQuality(int quality);
    
    //for load and save buttons
    virtual void  SceneLoad(void);
    virtual void  SceneAppend(void);
    virtual void  importFx(void);
    virtual void  saveEffects(void);
    virtual bool  SceneLoadName(QString);
    virtual bool  ModelLoadName(QString);
    virtual void  sceneSave(void);
    virtual void  PackageSave(void);

    virtual void  getScript();
    virtual void  stopScript();

    virtual void  saveAsExport();


    //used for vga emulated video head
    //the slot thats called by a checkbox
    //shouldnt be here but in the modules ui code
    //virtual void    SetVideoRender(void);
    virtual void    SetVideoRender(bool);
    virtual void    getRenderResolution(int &, int&);

    virtual void    SoftPhoto(QImage&);

    //used to grap asset from desktop and add to layer
    virtual void    addClip(JahLayer* jah_layer, assetData clip);
    virtual void    addClip(assetData);
    virtual void    addClipKey(assetData);

    virtual void    nameLayer(void);
    virtual void    nameLayerFromTopMenu(void);

    virtual void    ResetAll(void);
    virtual void    ResetLayer(void);
    virtual void    ClearAll(void);
    virtual void    loadClearAll(void);

    virtual void  setlayerName(QString);
    virtual void  toggleLight(void);
    virtual void  toggleMesh(void);
    virtual void  setUsePbuffer();

    virtual void  toggleExtendHeadTail(void);
    virtual void  toggleLoop(void);
    virtual void  togglePingPong(void);
    virtual void  toggleShader1(void);
    virtual void  toggleShader2(void);

    virtual void  toggleReflect(void);
    virtual void  toggleSmooth(void);
    virtual void  slotShowLights();

    virtual void  changeParticleColors();

    virtual void toggleStatsDisplay(void);

    virtual bool  getSelectionTree(QListView* listv, int &layer, int &effect, bool &status);

    virtual void  addSelectionFx(bool status = TRUE, QString fxname = 0, bool headtail = FALSE, bool set_selected = true);
    virtual QListViewItem* addEffectToUserInterface(QListViewItem* parent, QString& name, 
                                            bool checked, bool selected);

    virtual void  clearSelection(QListView *thelv);
    virtual void  changeSelection(int layer);

    virtual void  setLayerVisible( int, bool  );  //this can be overridden if necessary

    //change layer type
    virtual void ChangeObjectLayer(void);
    virtual void ChangeObjectCube(void);
    virtual void ChangeObjectCylinder(void);
    virtual void ChangeObjectSphere(void);
    virtual void ChangeObjectBezier(void);
    virtual void ChangeObjectMesh(void);


    virtual void  delLayer(void);
    virtual void  deleteEffectsList(JahLayer* layer);
    virtual void  deleteLightsList(JahLayer* layer);
    virtual void  moveLayerUp(void);
    virtual void  moveLayerDown(void);

    //layer keframe control routines
    virtual void addkeyNodes(JahLayer * layer, int theframe);
    virtual void removekeyNodes(JahLayer * layer, int theframe);
    virtual void updatekeyNodes(JahLayer * layer,int currentFrame);
    virtual void updateKeyNodes();
    virtual void clearkeys(JahLayer * layer);
    virtual void resetkeys(JahLayer * layer);

    virtual void autoSaveThisModule();

signals:
    virtual void    updateDesktop( assetData );
    
    //this is for the network module and http feedback
    virtual void    updateBrowser( QString debugmessage );
    void hideUI( bool );
    void    GrabDesktop(void);       //linked to main jahcontrol to talk to desktop
    void showInHead( JahHeadable * );

private:
	void lazy( );

    //new variables for color conversion mode control
    bool        m_bFlipColorData;
    GLenum      m_hTexture_format;
    int         m_nTexture_mode;
    QString m_qsJahLoadfilename;

    // Indicator for module started
    bool m_bStarted;

public:

    // These are general method which don't seem to have a home in the current
    // jahshaka design - they allow threads to interact safely with the QT and X
    // components - provided here as static methods to allow use in C libraries
    // via pointer to function passing
    
    // The placement is wrong though - they're not world specific...

    static void qt_lock( ) ;
    
    static void qt_unlock( ) ;

};


#endif // GLWORLD_H

