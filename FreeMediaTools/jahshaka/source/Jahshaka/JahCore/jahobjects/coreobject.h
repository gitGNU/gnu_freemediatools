/*******************************************************************************
**
** The header file for the Jahshaka core object file
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef COREOBJECT_H_INCLUDED
#define COREOBJECT_H_INCLUDED

#include "coreheaders.h"

#include "corestructures.h"


#include "coreeffects.h"

//#include "qlistview.h"

//for plugins
//#include "jahplugins.h" 
//#include "jahpluginlib.h"

class assetExchange;
class QListViewItem;
class QCheckListItem;
class LayerListEntry;
class JahPluginLib;

class JahLayer {

public:
    EffectLayer*		        getEffectLayer() { return m_effect_layer; }
    void                        setEffectLayer(EffectLayer* layer) { m_effect_layer = layer; }
    QPtrList<EffectLayer>*		getEffectsList() { return m_effects_list; }
    QCheckListItem*				getCheckListItem() { return m_check_list_item; }

    void						setCheckListItem(QCheckListItem* item) { m_check_list_item = item; }
    int                         getPluginNumber() { return m_plugin_number; }
    void                        setPluginNumber(int number) { m_plugin_number = number; }

    JahLayer*                   getParent() { return m_parent; }

    void                        setParent(JahLayer* parent) { m_parent = parent; }
    bool                        isChromaKeyLayer() { return m_is_chroma_key_layer; }
    void                        setCategory(ObjectCategory::TYPE category) { objtype = category; }
    bool                        isCameraLayer();

    ObjectCategory::TYPE              getCategory() { return objtype; }
    EffectInfo::EFFECT_CATEGORY getEffectCategory();
    EffectInfo::EFFECT_CATEGORY getPluginEffectCategory();

    void    setIsChromaKeyLayer(bool flag); 
    int     getEnabledEffectCount();
    bool    removeAllListViewItems();
    bool    buildListViewItems();
    bool    reorderListViewItems();
    bool    containsCpuEffects() { return m_contains_cpu_effects; }
    void    setContainsCpuEffects(bool flag) { m_contains_cpu_effects = flag; }
    void    setEffectsSlidersHaveChanged(bool flag) { m_effects_sliders_have_changed = flag; }
    bool    getEffectsSlidersHaveChanged() { return m_effects_sliders_have_changed; }
    void    setEffectsHaveKeyFrames(bool flag) { m_effects_have_key_frames = flag; }
    bool    getEffectsHaveKeyFrames() { return m_effects_have_key_frames; }
    bool    canAcceptLighting();

    LayerListEntry*     getLayerListEntry() { return m_layer_list_entry; }
    void                setLayerListEntry(LayerListEntry* entry) { m_layer_list_entry = entry; }
    float               getPreviousTextExtrude() { return m_previous_text_extrude; }
    void                setPreviousTextExtrude(float value) { m_previous_text_extrude = value; }
    bool                textExtrudeHasChanged();
    bool                getUsePbuffer() { return m_use_pbuffer; }
    void                setUsePbuffer(bool flag) { m_use_pbuffer = flag; }
    StabilizingState*   getStabilizingState() { return m_stabilizing_state; }
    void                setStabilizingState(StabilizingState* state) { m_stabilizing_state = state; }
    void                drawTexture(bool position_object = true);
    color4*             getLight0AmbientPtr() { return &m_light0_ambient; }
    color4*             getLight0DiffusePtr() { return &m_light0_diffuse; }
    color4*             getLight0SpecularPtr() { return &m_light0_specular; }
    float4*             getLight0PositionPtr() { return &m_light0_position; }
    float*              getLight0ShininessPtr() { return &m_light0_shininess; }
    void                setTranslateFirst(bool flag) { m_translate_first = flag; }
    bool                getTranslateFirst() { return m_translate_first; }
    JahLayer*           getCameraLayer() { return m_camera_layer; }
    void                setCameraLayer(JahLayer* layer) { m_camera_layer = layer; }
    QPtrList<JahLayer>* getLightsList() { return m_lights_list; }
    void                setLightsList( QPtrList<JahLayer>* list ) { m_lights_list = list; }

    motionNode*         findKeyframeAtTime(int time);

    float               getPreviousAlpha() { return m_previous_alpha; }
    void                setPreviousAlpha(float value) { m_previous_alpha = value; }

    bool                getCompositeTextureUpdatedByLighting() { return m_composite_texture_updated_by_lighting; }
    void                setCompositeTextureUpdatedByLighting(bool flag) { m_composite_texture_updated_by_lighting = flag; }

    bool                getCompositeTextureNeedsUpdatingByLighting();

    bool                getEffectsUpdated() { return m_effects_updated; }
    void                setEffectsUpdated(bool flag) { m_effects_updated = flag; }
    bool                getSupportsTransparency();
    bool                getLightingNeedsToUpdateCompositeTexture();


protected:
    QPtrList<EffectLayer>*      m_effects_list;
    QPtrList<JahLayer>*         m_lights_list;
	JahLayer*                   m_camera_layer;

    //why are we using a QListView item as a data type?
    //when its really a ui widget?
    QCheckListItem*				m_check_list_item;
    LayerListEntry*             m_layer_list_entry;

    // This is needed in case the layer is an effect
    EffectLayer*                m_effect_layer;
    int                         m_plugin_number;

    //shouldnt have a layer in here
    //as this is the layer type in the first place?
    JahLayer*                   m_parent;
    bool                        m_is_chroma_key_layer;
    bool                        m_contains_cpu_effects;
    bool                        m_effects_sliders_have_changed;
    bool                        m_effects_have_key_frames;
    float                       m_previous_text_extrude;
    bool                        m_use_pbuffer;
    StabilizingState*           m_stabilizing_state;
    color4                      m_light0_ambient;
    color4                      m_light0_diffuse;
    color4                      m_light0_specular;
    float4                      m_light0_position;
    float                       m_light0_shininess;
    bool                        m_translate_first;
    float                       m_previous_alpha;
    bool                        m_composite_texture_updated_by_lighting;
    bool                        m_effects_updated;
    


    ////////////////////////////////////////
    //pointer to plugin singleton
    JahPluginLib*   jplugin;

public:
    JahLayer(void);
    ~JahLayer(void);

    //////////////////////////////////////////////
    //declare generic functions first
    //////////////////////////////////////////////

    //initializes all the variables
    void setupDefaultVariables(void);

    //prints out all layer information
    void printInfo(void);

    // for gpu effects
    bool usedagpu;


    // core layer information
    ObjectCategory::TYPE        objtype;
    LayerCategory::TYPE         layertype;
    QString                     layername;
    std::string                 m_layer_name_string;

    bool                layerIsEnabled();

    bool                layerStatus;    //means layer is on or off
    bool                selected;       //means layer is selected
    bool                drawtheLayer;   //used to hide layers even if on and selected
    bool                drawtheKey;     //used to toggle the keyasset
    bool                invertKey;      //used to invert the key in keyasset

    //////////////////////////////////////////////////////
    //resolution data for world layer
    /////////////////////////////////////////////////////

    void updateResolution(int res);

    //////////////////////////////////////////////////////
    //assets used to describe media connected to the layer
    /////////////////////////////////////////////////////

    //these could be made into pointers to be more efficient
    assetData           asset;
    assetData           keyasset;
    std::string         m_asset_name_string;

    bool                layerAssetStatus;
    bool                keyAssetStatus;

    //assign a asset to a layer
    void assignAsset(assetData clip, VideoCategory::TYPE cliptype, bool update_video_settings = false);
    bool LoadLayerAsset( VideoCategory::TYPE imagetype = VideoCategory::CLIP );

    //assetexchange is replacing mediadata
    assetExchange*      mediaExchange;
    bool                mediaExchangePresent;
    bool                getInframeValid() { return m_inframe_valid; }
    void                setInframeValid(bool flag) { m_inframe_valid = flag; }
    int                 getSlipFrameOffset() { return m_slip_frame_offset; }
    void                setSlipFrameOffset(int value) { m_slip_frame_offset = value; }
    int                 getKeySlipFrameOffset() { return m_key_slip_frame_offset; }
    void                setKeySlipFrameOffset(int value) { m_key_slip_frame_offset = value; }

    void addMediaObj();

    //for video layers
    bool                m_inframe_valid;
    int                 m_currentframe;
    int                 m_inframe;
    int                 m_outframe;
    int                 m_slip_frame_offset;

    //for keys
    int                 m_keycurrentframe;
    int                 m_keyinframe;
    int                 m_keyoutframe;
    int                 m_key_slip_frame_offset;
    GLuint              m_default_texture_id;

    //////////////////////////////////////////////
    //keyframe routines
    //////////////////////////////////////////////

    //motionNode        *node;
    JahKeyframeObject    *layernodes;

    //routines to setup and reset keyframe objects
    void setupKeyframeVariables(void);

    //routine to draw keyframes for a layer
    bool drawKeyframes;

    ////////////////////////////////////////////////////
    // layer options
    bool option1;
    bool option2;
    bool option3;
    bool option4;
    bool option5;

    ////////////////////////////////////////////////////
    //jahtexture texture calls here
    ////////////////////////////////////////////////////

    GLuint getDefaultTextureId() { return m_default_texture_id; }
    GLuint* getDefaultTextureIdPtr() { return &m_default_texture_id; }
    TextureImage& getTextureComposite() { return m_texture_composite; }
    TextureImage& getKeyComposite() { return m_key_composite; }
    TextureImage& getTextureData() { return textureData; }

    //layers can have keys
    TextureImage textureData;
    TextureImage textureKey;

    QImage*             getCompositeQimagePtr() { return &m_composite_qimage; }
    QImage*             getCompositeQimageFlippedPtr() { return &m_composite_qimage_flipped; }
    bool                getSelectColorsIndividually() { return m_select_colors_individually; }
    void                setSelectColorsIndividually(bool flag) { m_select_colors_individually = flag; }
    bool                getFirstPass() { return m_first_pass; }
    void                setFirstPass(bool flag) { m_first_pass = flag; }
    bool                getCreateChromaAlphaMask() { return m_create_chroma_alpha_mask; }
    void                setCreateChromaAlphaMask(bool flag) { m_create_chroma_alpha_mask = flag; }

    CompositeType::TYPE         getCompositeType() { return m_composite_type; }
    void                        setCompositeType(CompositeType::TYPE type);

    bool                getLightingEnabled() { return m_lighting_enabled; }
    void                setLightingEnabled(bool flag) { m_lighting_enabled = flag; }
    bool                getShowLights() { return m_show_lights; }
    void                setShowLights(bool flag) { m_show_lights = flag; }

protected:
    bool                            m_first_effect_layer_being_rendered;
    QImage                          m_composite_qimage;
    QImage                          m_composite_qimage_flipped;
    TextureImage                    m_texture_composite;
    TextureImage                    m_key_composite;
    EffectInfo::EFFECT_CATEGORY     m_previous_rendered_layer_effect_category;
    bool                            m_texture_composite_dirty;
    bool                            m_qimage_composite_dirty;
    bool                            m_select_colors_individually;
    bool                            m_first_pass;
    bool                            m_create_chroma_alpha_mask;
    GLenum                          m_src_blend_factor;
    GLenum                          m_dst_blend_factor;
    CompositeType::TYPE             m_composite_type;
    bool                            m_lighting_enabled;
    bool                            m_show_lights;
    bool                            m_use_open_gl_keyer;
    bool                            m_use_fast_shader_keyer;

public:
    GLenum          getSrcBlendFactor() { return m_src_blend_factor; }
    void            setSrcBlendFactor(GLenum factor) { m_src_blend_factor = factor; }
    GLenum          getDstBlendFactor() { return m_dst_blend_factor; }
    void            setDstBlendFactor(GLenum factor) { m_dst_blend_factor = factor; }

    bool            isFirstEffectLayerBeingRendered() { return m_first_effect_layer_being_rendered; }
    void            setFirstEffectLayerBeingRendered(bool flag) { m_first_effect_layer_being_rendered = flag; }
    EffectLayer*    findFirstGpuEffectLayer();

    EffectInfo::EFFECT_CATEGORY     getPreviousRenderedLayerEffectCategory();
    void            setPreviousRenderedLayerEffectCategory(EffectInfo::EFFECT_CATEGORY category);
    QImage*         getSourceQimageForCurrentEffect();
    void            setTextureCompositeDirty(bool flag) { m_texture_composite_dirty = flag; }
    void            setQimageCompositeDirty(bool flag) { m_qimage_composite_dirty = flag; }
    bool            getTextureCompositeDirty() { return m_texture_composite_dirty; }
    bool            getQimageCompositeDirty() { return m_qimage_composite_dirty; }
    bool            getUseOpenGlKeyer() { return m_use_open_gl_keyer; }
    void            setUseOpenGlKeyer(bool flag) { m_use_open_gl_keyer = flag; }
    bool            getUseFastShaderKeyer() { return m_use_fast_shader_keyer; }
    void            setUseFastShaderKeyer(bool flag) { m_use_fast_shader_keyer = flag; }
    


    void   bindBaseOrCompositeTexture();
    GLuint getBaseOrCompositeTextureId();

    void makeTexture( void );
    void updateTexture( void );
    void updateKeyTexture( void );
    void makeLargeTexture( void );
    void makeLayerTexture( void );
    void makeKeyTexture( void );
    void changeKeyStatus(void );

    void updateVidTex( int theframe);
    bool updateVidTexAsset(int frame);
    bool updateVidTexKeyasset(int frame);

    void setLayerTexture(void);
    void bindKeyTexture(void);

    void buildGlTexture (TextureImage& texture_image, QImage & buf  );
    void updateGlTexture ( QImage & buf );

    void InitializeTexture(TextureImage & textured);

    //return some layer data... used for new effects module
    //dont now if this is repetitious...
    int getImageWidth()  { return textureData.objectImage.width(); }
    int getImageHeight() { return textureData.objectImage.height(); }

    ///////////////////////////////////////////////
    //jahdraw calls here
    //////////////////////////////////////////////

    void            positionObject();
    void            drawLayer(void);

    ///////////////////////////////////////////////
    //generic jahobject object creation here
    ///////////////////////////////////////////////

    //////////////////////////////////////////////////
    //used for the light
    ////////////////////////////////////////////////////////

    bool            loadLight;
    int             lightNumber;

    GLfloat         light_diffuse[4];
    GLfloat         light_ambient[4];
    GLfloat         light_specular[4];

    //////////////////////////////////////////////////
    //used for layer fog
    ////////////////////////////////////////////////////////
    //void            makeFog(void);
    bool            layerfog;

    //////////////////////////////////////////////////
    //used for the mesh layer
    ////////////////////////////////////////////////////////

    //void            initializeMeshLayer( void);
    void            setMeshCoord( int i, int j, GLfloat x, GLfloat y);
    void            setMeshCoord( int i, int j, GLfloat x, GLfloat y, GLfloat z);
    void            getMeshCoord( int i, int j, GLfloat& x, GLfloat& y, GLfloat& z);
    void            getMeshCoord( int i, int j, float4& vector);
    void            setMeshNormal( int i, int j, GLfloat x, GLfloat y, GLfloat z);
    void            setMeshNormal( int i, int j, float4& vector);
    void            getMeshNormal( int i, int j, GLfloat& x, GLfloat& y, GLfloat& z);
    void            getMeshNormal( int i, int j, float4& vector);
    void            saveMeshCoords();
    void            restoreMeshCoords();
    void            saveMeshNormals();
    void            restoreMeshNormals();
    void            generateMeshNormals();
    //void            makeMeshLayer( void );

    // can we use pointers here to free up memory?
    GLfloat         MeshXCoord [74][50]; // should be 72,48 for 720 486
    GLfloat         MeshYCoord [74][50];
    GLfloat         MeshZCoord [74][50];
    float4          m_mesh_save[74][50];
    GLfloat         MeshXNormal [74][50]; // should be 72,48 for 720 486
    GLfloat         MeshYNormal [74][50];
    GLfloat         MeshZNormal [74][50];
    float4          m_mesh_normal_save[74][50];
    bool            MeshFirsttime;

    int             JAHIMAGE_SCALE;
    int             JAHMESH_X_DIMENSION;
    int             JAHMESH_Y_DIMENSION;
    int             JAHMESH_X_DIMENSION_DIV_2;
    int             JAHMESH_Y_DIMENSION_DIV_2;

    ////////////////////////////////////////////////////////
    //for the text object
    ////////////////////////////////////////////////////////

    textObj         *text;
    void            addTextObj();
    bool            updateFont;

    ////////////////////////////////////////////////////
    //jah 3d model loading and drawing routines
    //////////////////////////////////////////////////

    QString         objfilename;
    QString         objextension;

    //////////////////////////////////////////////////
    //for 3d object support
    objectData*     jahobject;
    bool            jahobjectPresent;

    void            addObjectObj();

    //flag used to reload objects?
    bool            loadObject;

    ///////////////////////////////////////////////////
    //particle creation here
    //////////////////////////////////////////////////

    bool            loadParticles;

    //texturing routines used in particle engine
    inline float    Gaussian2(float x, float y, float sigma);
    void            MakeGaussianSpotTexture();  //used for particles

    //particle variables
    ParticleCategory::TYPE    ParticleStyle;
    ParticleDrawstyle::TYPE   ParticleDraw;
    bool                ParticleFirsttime;
    bool                ParticleColors;

    int             particle_handle, action_handle;
    int             maxParticles;
    int             demoNum,         numSteps,      prim;
    int             listID, SpotTexID;
    float           BlurRate;
    int             SteerShape;

    //////////////////////////////////////////////////
    // screen resolution/ratio based variables for rendering
    //////////////////////////////////////////////////

    // should be in a subroutine later to be called whenever this data changes
    GLfloat         widthOffset;
    GLfloat         heightOffset;


    ////////////////////////////////////////////////////
    //for effects layer support
    //////////////////////////////////////////////////

    bool            layerEffect;

    //these should be in a dynamic array that can be used
    //to add multiple effects to a layer
    //also should be node based so we can keyframe them

    bool            negative,swaprgb,mirror;
    bool            blur;
    GLfloat         hblur;
    GLfloat         vblur;

    //////////////////////////////////////////////////
    // global variables
    //////////////////////////////////////////////////

    //generic display settings
    GLenum      blendmode1, blendmode2;
    GLfloat     Red, Green, Blue;

    //generic layer flags and settings
    bool        blend;
    bool        depth;
    bool        cull;
    bool        lighting;
    bool        reflect;
    bool        smooth;
    bool        foreground;

protected:
    bool        m_is_slected_visible;

public:
    bool        getIsSelectedVisible() { return m_is_slected_visible; }
    void        setIsSelectedVisible(bool flag) { m_is_slected_visible = flag; }

    bool        ObjectDisplayMode;  //draw object as wireframe or as textured?
    bool        showEdges;          //used to hide or show head/tail for clips with slip
    bool        loop;
    bool        ping;


private:
    ////////////////////////////////////////
    //pointer to tracer singleton
    JahTrace            * jtrace;

    QString JahBasePath;
    QString JahMediaPath;

    int X_RESOLUTION;
    int Y_RESOLUTION;

    //new variables for color conversion mode control
public:
    GLenum      m_texture_format;

public:
    GLenum      getTextureFormat() { return m_texture_format; }
    int getXResolution() { return X_RESOLUTION; }
    int getYResolution() { return Y_RESOLUTION; }

    bool addEffect(EffectInfo::EFFECT_TYPE category, bool status = true, int pluginid = 0);


    //void saveXML( std::fstream& ai_fstream ) ; 
};


#endif // COREOBJECT_H_INCLUDED
