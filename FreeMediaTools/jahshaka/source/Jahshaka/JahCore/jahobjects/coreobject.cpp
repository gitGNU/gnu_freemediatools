/*******************************************************************************
**
** The source file for the Jahshaka core object file
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "coreobject.h"
#include "openmedialib.h"
#include "openobjectlib.h"
#include "jahpluginlib.h"
#include "jahpreferences.h"
#include <qlistview.h>

#define STRINGDATA(X) ( X.data() ? X.data() : "" )  

JahLayer::JahLayer(void) :
  layernodes( NULL ),
  mediaExchangePresent(false),
 
  /*    CompositeType::TYPE m_composite_type;
    EffectInfo::EFFECT_CATEGORY m_previous_rendered_layer_effect_category;
    EffectLayer* m_effect_layer;
    GLenum m_dst_blend_factor;
    GLenum m_src_blend_factor;
    GLenum m_texture_format;
    GLfloat MeshXCoord [74][50];
    GLfloat MeshXNormal [74][50];
    GLfloat MeshYCoord [74][50];
    GLfloat MeshYNormal [74][50];
    GLfloat MeshZCoord [74][50];
    GLfloat MeshZNormal [74][50];
    GLfloat Red, Green, Blue;
    GLfloat hblur;
    GLfloat heightOffset;
    GLfloat light_ambient[4];
    GLfloat light_diffuse[4];
    GLfloat light_specular[4];
    GLfloat vblur;
    GLfloat widthOffset;
    GLuint m_default_texture_id;
  */
  //JahKeyframeObject *
  //JahLayer* 
  m_parent(0),
  //JahPluginLib* 
  jplugin(0),
  //LayerCategory::TYPE layertype;
  //LayerListEntry* 
  m_layer_list_entry(0),
  /*    ObjectCategory::TYPE objtype;
    ParticleCategory::TYPE ParticleStyle;
    ParticleDrawstyle::TYPE ParticleDraw;*/
//QCheckListItem*
m_check_list_item(0),
      //QImage m_composite_qimage;
      //QImage m_composite_qimage_flipped;
      //    QPtrList<EffectLayer>*
m_effects_list(0),
//    QPtrList<JahLayer>*
m_lights_list(0),
      /*  QString JahMediaPath;
	  QString layername;
	  QString objextension;
	  QString objfilename;
	  StabilizingState*
      */
      m_stabilizing_state(0),
  /*
    TextureImage m_key_composite;
    TextureImage m_texture_composite;
    TextureImage textureData;
    TextureImage textureKey;
    assetData asset;
    assetData keyasset;

*/
//  assetExchange* 
  mediaExchange(0),
      //    bool
  MeshFirsttime(false),
  /*bool*/ ObjectDisplayMode(false),
    /*bool*/ ParticleColors(false),
    /*bool*/ ParticleFirsttime(false),
    /*bool*/ blend(false),
    /*bool*/ blur(false),
    /*bool*/ cull(false),
    /*bool*/ depth(false),
    /*bool*/ drawKeyframes(false),
    /*bool*/ drawtheKey(false),
    /*bool*/ drawtheLayer(false),
    /*bool*/ foreground(false),
    /*bool*/ invertKey(false),
    /*bool*/ jahobjectPresent(false),
    /*bool*/ keyAssetStatus(false),
    /*bool*/ layerAssetStatus(false),
    /*bool*/ layerEffect(false),
    /*bool*/ layerStatus(false),
    /*bool*/ layerfog(false),
    /*bool*/ lighting(false),
    /*bool*/ loadLight(false),
    /*bool*/ loadObject(false),
    /*bool*/ loadParticles(false),
    /*bool*/ loop(false),
    /*bool*/ m_composite_texture_updated_by_lighting(false),
    /*bool*/ m_contains_cpu_effects(false),
    /*bool*/ m_create_chroma_alpha_mask(false),
    /*bool*/ m_effects_have_key_frames(false),
    /*bool*/ m_effects_sliders_have_changed(false),
    /*bool*/ m_effects_updated(false),
    /*bool*/ m_first_effect_layer_being_rendered(false),
    /*bool*/ m_first_pass(false),
    /*bool*/ m_inframe_valid(false),
    /*bool*/ m_is_chroma_key_layer(false),
    /*bool*/ m_is_slected_visible(false),
    /*bool*/ m_lighting_enabled(false),
    /*bool*/ m_qimage_composite_dirty(false),
    /*bool*/ m_select_colors_individually(false),
    /*bool*/ m_show_lights(false),
    /*bool*/ m_texture_composite_dirty(false),
    /*bool*/ m_translate_first(false),
    /*bool*/ m_use_fast_shader_keyer(false),
    /*bool*/ m_use_open_gl_keyer(false),
    /*bool*/ m_use_pbuffer(false),
    /*bool*/ negative(false),
  swaprgb(false),
  mirror(false),
    /*bool*/ option1(false),
    /*bool*/ option2(false),
    /*bool*/ option3(false),
    /*bool*/ option4(false),
    /*bool*/ option5(false),
    /*bool*/ ping(false),
    /*bool*/ reflect(false),
    /*bool*/ selected(false),
    /*bool*/ showEdges(false),
    /*bool*/ smooth(false),
  /*bool*/ updateFont(false),
  /*bool*/ usedagpu(false),
  /*  color4 m_light0_ambient;
    color4 m_light0_diffuse;
    color4 m_light0_specular;
    float BlurRate;
    float m_light0_shininess;
    float m_previous_alpha;
    float m_previous_text_extrude;
    float4 m_light0_position;
    float4 m_mesh_normal_save[74][50];
    float4 m_mesh_save[74][50];
    int JAHIMAGE_SCALE;
    int JAHMESH_X_DIMENSION;
    int JAHMESH_X_DIMENSION_DIV_2;
    int JAHMESH_Y_DIMENSION;
    int JAHMESH_Y_DIMENSION_DIV_2;
    int SteerShape;
    int X_RESOLUTION;
    int Y_RESOLUTION;
    int demoNum, numSteps, prim;
    int lightNumber;
    int listID, SpotTexID;
    int m_currentframe;
    int m_inframe;
    int m_key_slip_frame_offset;
    int m_keycurrentframe;
    int m_keyinframe;
    int m_keyoutframe;
    int m_outframe;
    int m_plugin_number;
    int m_slip_frame_offset;
    int maxParticles;*/

  //objectData* 
  jahobject(0),
  /*
    std::string m_asset_name_string;
    std::string m_layer_name_string;
  */
  //textObj *
  text(0),
  //JahLayer* 
  m_camera_layer(0)
{
  layername="THISISMYTEST";
    jtrace = JahTrace::getInstance();	//set up tracer

	jplugin = JahPluginLib::getInstance();	//set up tracer

    //////////////////////////////////////////////////////////////
    //flag to let you know if a mediaobject was created
    //we only create mediaobject when a media file is
    //assigned to the layer to keep layers light
    mediaExchangePresent  = false;

    //initialize JahBasePath and JahMediaPath
    JahPrefs& jprefs = JahPrefs::getInstance();
    JahBasePath  = jprefs.getBasePath().data();
    JahMediaPath = jprefs.getMediaPath().data();

    ///////////////////////////////////////////////////////////////
    //this will be moved out into the world and sent that way
    //for now we start up using default resolution
    int jahres = jprefs.getJahResolution();

    updateResolution(jahres);

    ////////////////////////////////////////////////////////////////
    //set up default variables 
    setupDefaultVariables();

	m_effects_list = new QPtrList<EffectLayer>;
	m_effects_list->setAutoDelete(true);

	m_check_list_item = NULL;
    m_effect_layer = NULL;

    m_lights_list = new QPtrList<JahLayer>;
    //m_lights_list->setAutoDelete(true);

    m_is_chroma_key_layer = false;

    m_texture_composite_dirty = false;
    m_select_colors_individually = false;
    m_contains_cpu_effects = false;
    m_effects_sliders_have_changed = false;
    m_effects_have_key_frames = false;
    m_create_chroma_alpha_mask = false;
    setUsePbuffer(true);
    m_stabilizing_state = NULL;


    text = NULL ;
    jahobject = NULL ;

    ////////////////////////////////////////////////////////////////
    //set up default variables 
    setupKeyframeVariables();

    getLight0AmbientPtr()->x = 1.0;
    getLight0AmbientPtr()->y = 1.0;
    getLight0AmbientPtr()->z = 1.0;
    getLight0AmbientPtr()->w = 1.0;
    getLight0DiffusePtr()->x = 1.0;
    getLight0DiffusePtr()->y = 1.0;
    getLight0DiffusePtr()->z = 1.0;
    getLight0DiffusePtr()->w = 1.0;
    getLight0SpecularPtr()->x = 1.0;
    getLight0SpecularPtr()->y = 1.0;
    getLight0SpecularPtr()->z = 1.0;
    getLight0SpecularPtr()->w = 1.0;
    getLight0PositionPtr()->x = 0.0;
    getLight0PositionPtr()->y = 0.0;
    getLight0PositionPtr()->z = 100.0;
    getLight0PositionPtr()->w = 0.0;

    setTranslateFirst(true);
    m_camera_layer = NULL;
    setCompositeType( CompositeType::STANDARD_COMPOSITE_TYPE );
    setParent(NULL);
    setLightingEnabled(false);
    setShowLights(true);  
	objtype = ObjectCategory::JAHLAYER;
    createEmpty2DTexture(getDefaultTextureIdPtr(), GL_RGBA, 2, 2);
    unsigned int temp_buffer[4] = { 1, 1, 1, 1 };
    glBindTexture( GL_TEXTURE_2D, getDefaultTextureId() );
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 2, 2, GL_RGBA, GL_UNSIGNED_BYTE, &temp_buffer);
    m_slip_frame_offset = 0;
    m_key_slip_frame_offset = 0;
    setIsSelectedVisible(true);
    setPreviousAlpha(1000.0f);
    setCompositeTextureUpdatedByLighting(false);
    setEffectsUpdated(false);
}

/////////////////////////////////////////////////////
// its doing this for EVERY layer on construction
//but we only need to do it for background layers

void JahLayer::updateResolution(int res)
{
    projectData thedata;

    X_RESOLUTION = thedata.getXres(res);
    Y_RESOLUTION = thedata.getYres(res);

    widthOffset  = (GLfloat)X_RESOLUTION/2;
    heightOffset = (GLfloat)Y_RESOLUTION/2;
}

JahLayer::~JahLayer(void)
{
	if (layernodes)
	{
		delete layernodes;
	}

    if (mediaExchangePresent)
    {
        delete mediaExchange;
    }

	if ( textureData.texID )
    	glDeleteTextures(1, &textureData.texID); 
	if ( textureKey.texID )
    	glDeleteTextures(1, &textureKey.texID); 
	if ( getTextureComposite().texID )
    	glDeleteTextures(1, &(getTextureComposite().texID) );

    if ( getCategory() == ObjectCategory::LARGE_LAYER )
    {
        glDeleteTextures(4, textureData.getTileTextureIdPtr(0) );
    }

    if ( getCategory() == ObjectCategory::OBJECT && jahobject )
    {
        delete jahobject;
    }

	if (m_effects_list)
	{
		delete m_effects_list;
	}
}

void JahLayer::setupKeyframeVariables()
{
	//array of nodes for keyframes
  if (layernodes)
    {
	delete layernodes;
	layernodes=0;
    }
    layernodes  = new JahKeyframeObject;
    layernodes->clear();
}

//////////////////////////////////////////////////////////////
// routines necessary to initialize external objects

void JahLayer::setupDefaultVariables() 
{

    //layer flags and default settings
    layerAssetStatus     = false;       
    m_inframe = 0;
    m_outframe = 0;
    setInframeValid(false);

    keyAssetStatus       = false;
    m_keyinframe = 0;
    m_keyoutframe = 0;
    
    
    foreground           = 0;

    selected        = FALSE;
    drawKeyframes   = FALSE;

    showEdges       = true;
    loop			= false;
    ping			= false;

    layerfog        = FALSE;
    drawtheKey      = FALSE;

    //add these to the save/load routines
    blend           = 1;
    blendmode1      = GL_DST_COLOR;
    blendmode2      = GL_ZERO;
    depth           = FALSE;
    reflect			= FALSE;   
    smooth			= TRUE;

    cull            = 0;        
    lighting        = 0;

    //for the built in layer effects
    layerEffect     = FALSE;
    blur            = FALSE;
    hblur           = 0.0;
    vblur           = 0.0;

    negative        = FALSE;
    swaprgb         = FALSE;
    mirror          = FALSE;

    //Set default color
    Red = Green = Blue = 1.0f;

    //true means its textured, false is wireframe
    ObjectDisplayMode = FALSE;

    //layer mesh for effects
    MeshFirsttime   = FALSE;

    setFirstPass(true);

	JAHIMAGE_SCALE   =                  1;
	JAHMESH_X_DIMENSION =               72;
	JAHMESH_Y_DIMENSION =               48;
	JAHMESH_X_DIMENSION_DIV_2 =         (JAHMESH_X_DIMENSION / 2);
	JAHMESH_Y_DIMENSION_DIV_2 =         (JAHMESH_Y_DIMENSION / 2);

    foreground      = FALSE;   
    keyAssetStatus  = FALSE;
    invertKey       = FALSE;
    blend           = FALSE;   
    depth           = FALSE;

    //3d obj object data defaults



    //set up particle defaults
    ParticleDraw    = ParticleDrawstyle::PARLINE;
    ParticleColors  = FALSE;

    //options defaults
    option1    = false;
    option2    = false;
    option3    = false;
    option4    = false;
    option5    = false;

	loadObject = false;
	loadParticles = false;
	loadLight = false;

	jahobjectPresent = false;
    m_parent = NULL;

    //for lights
    lightNumber = -1; //means no light here mon

	//fliping color data uses the cpu and is slower

    //notes for irix...
    /*
    GLenum texture_format;
    #ifdef JAHIRIX
    texture_format = GL_RGB;
    #else
    texture_format = GL_RGBA;
    #endif
    */ 

    m_texture_format = GL_BGRA;
    setSrcBlendFactor(GL_SRC_ALPHA);
    setDstBlendFactor(GL_ONE_MINUS_SRC_ALPHA);
    setUseOpenGlKeyer(false);
    setUseFastShaderKeyer(true);
}

//we have to initialize the textobject first before using it
void JahLayer::addTextObj() 
{

	updateFont		= true;

    text            = new textObj;
    text->fontshere = false;
    text->fontstyle = FontDrawstyle::FPOLYGON;

}

//we have to initialize the mediaobject first before using it
void JahLayer::addMediaObj() 
{
    if (!mediaExchangePresent)
    {
        mediaExchange = new assetExchange;
		mediaExchange->setCorrectAspectRatio( false );
        mediaExchangePresent=true;
    }
}

//we have to initialize the mediaobject first before using it
void JahLayer::addObjectObj() 
{
    if (!jahobjectPresent)
    {
        jahobject = new objectData;
        jahobjectPresent=true;
    }
}


//////////////////////////////////////////////////////////////
// miscellaneous routines


//routine to print out all info on object
void JahLayer::printInfo(void) 
{
    QString data;

    debug("Printing out the layer data");
    debug("---------------------------");
    debug( "LayerName   :" + layername );
    debug( "LayerType   :" + data.setNum(layertype   ) );
    debug( "ObjectType  :" + data.setNum(objtype     ) );
    debug( "Status      :" + data.setNum(layerStatus ) );
    debug( "Selected    :" + data.setNum(selected    ) );
    debug( " " );
    debug( "Key         :" + data.setNum(keyAssetStatus   ) );
    debug( "DrawKey     :" + data.setNum(drawtheKey  ) );
    debug( "InvertKey   :" + data.setNum(invertKey   ) );
    //debug( "VideoTexKey :" + data.setNum(videotexkey ) );
    debug( " " );
    debug( "Blending    :" + data.setNum(blend       ) );
    debug( "Depth       :" + data.setNum(depth       ) );
    debug( "Lighting    :" + data.setNum(lighting    ) );
    debug( "Reflection  :" + data.setNum(reflect     ) );
    debug("---------------------------");
    //asset.printinfo();
    debug("---------------------------");

}

bool 
JahLayer::addEffect(EffectInfo::EFFECT_TYPE category, bool status, int pluginid)
{
	EffectLayer* new_effect = new EffectLayer;
    m_effects_list->append(new_effect);
    new_effect->objtype = category;
    new_effect->layerStatus = status;
    new_effect->setPluginNumber(pluginid);

    return true;
}


EffectLayer* 
JahLayer::findFirstGpuEffectLayer()
{
    EffectLayer* current = getEffectsList()->first();

    for ( ; current; current = getEffectsList()->next() )
    {
        JahLayer* jah_layer = current->getJahLayer();

        if ( current->isAGpuEffect() && jah_layer->layerIsEnabled() )
        {
            return current;
        }

        if (current->getType() == EffectInfo::JAH_PLUGIN_TYPE)
        {
            int plugin_number = current->getPluginNumber();
            JahPluginListElement* current_plugin = jplugin->vfxPlugins->at(plugin_number);
            
            if (current_plugin->thePlugin->pluginClass == JAH_GPU_PLUGIN)
            {
                return current;
            }
        }
    }

    return NULL;
}

EffectInfo::EFFECT_CATEGORY
JahLayer::getPluginEffectCategory()
{
    EffectLayer* effect_layer = getEffectLayer();
    int plugin_number = effect_layer->getPluginNumber();
    JahPluginListElement* current_plugin = jplugin->vfxPlugins->at(plugin_number);

    if (current_plugin->thePlugin->pluginClass == JAH_GPU_PLUGIN)
    {
        return EffectInfo::GPU_CATEGORY;
    }

    if (current_plugin->thePlugin->pluginClass == JAH_CPU_PLUGIN)
    {
        return EffectInfo::CPU_CATEGORY;
    }

    return EffectInfo::MESH_CATEGORY;
}

EffectInfo::EFFECT_CATEGORY
JahLayer::getEffectCategory()
{
    EffectLayer* effect_layer = getEffectLayer();
    EffectInfo::EFFECT_TYPE effect_type = effect_layer->getType();

    if ( effect_type == EffectInfo::JAH_PLUGIN_TYPE )
    {
        return getPluginEffectCategory();
    }
    else
    {
        return effect_layer->getCategory();
    }
}

void   
JahLayer::bindBaseOrCompositeTexture()
{
    if ( getTextureCompositeDirty() )
    {
        glBindTexture(GL_TEXTURE_2D, getTextureComposite().texID);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, textureData.texID);
    }
}

GLuint
JahLayer::getBaseOrCompositeTextureId()
{
    if ( getTextureCompositeDirty() )
    {
        return getTextureComposite().texID;
    }
    else
    {
        return textureData.texID;
    }
}


void                        
JahLayer::setIsChromaKeyLayer(bool flag) 
{ 
    m_is_chroma_key_layer = flag; 

    if (flag)
    {
        setCategory(ObjectCategory::JAHLAYER);
    }
    else
    {
        setCategory(ObjectCategory::MESHLAYER);
    }
}

EffectInfo::EFFECT_CATEGORY 
JahLayer::getPreviousRenderedLayerEffectCategory()
{
    return m_previous_rendered_layer_effect_category;
}

void 
JahLayer::setPreviousRenderedLayerEffectCategory(EffectInfo::EFFECT_CATEGORY category)
{
    m_previous_rendered_layer_effect_category = category;
}


int                        
JahLayer::getEnabledEffectCount()
{
    int count = 0;

    QPtrList<EffectLayer>* effects_list = getEffectsList();
    EffectLayer* effect = effects_list->first();

    for ( ; effect; effect = effects_list->next() )
    {
        if ( effect->getJahLayer()->layerIsEnabled() )
        {
            count++;
        }
    }
    return count;
}

bool                        
JahLayer::reorderListViewItems()
{
    removeAllListViewItems();
    buildListViewItems();
    return true;
}

bool                        
JahLayer::removeAllListViewItems()
{
    // Take all the listviewitem children out of the tree, but do not delete them
    QPtrList<EffectLayer>* effects_list = getEffectsList();

    if (effects_list == NULL)
    {
        return false;
    }

    QListViewItem* parent_list_view_item = getCheckListItem();
    EffectLayer* effect_layer = effects_list->first();

    for ( ; effect_layer; effect_layer = effects_list->next() )
    {
        JahLayer* effect_jah_layer = effect_layer->getJahLayer();
        QListViewItem* effect_list_view_item = effect_jah_layer->getCheckListItem();
        parent_list_view_item->takeItem(effect_list_view_item);
    }

    return true;
}

bool                        
JahLayer::buildListViewItems()
{
    QPtrList<EffectLayer>* effects_list = getEffectsList();

    if (effects_list == NULL)
    {
        return false;
    }

    // Insert the list view items from the end of the list to the beginning,
    // so that they appear in order on the screen
    QListViewItem* parent_list_view_item = getCheckListItem();

#ifdef ADD_EFFECTS_TO_TOP_OF_STACK
    EffectLayer* effect_layer = effects_list->first();
#else
    EffectLayer* effect_layer = effects_list->last();
#endif 

#ifdef ADD_EFFECTS_TO_TOP_OF_STACK
    for ( ; effect_layer; effect_layer = effects_list->next() )
#else
    for ( ; effect_layer; effect_layer = effects_list->prev() )
#endif
    {
        JahLayer* effect_jah_layer = effect_layer->getJahLayer();
        QListViewItem* effect_list_view_item = effect_jah_layer->getCheckListItem();
        parent_list_view_item->insertItem(effect_list_view_item);
    }

    return true;
}


bool
JahLayer::textExtrudeHasChanged()
{
    if ( layernodes->m_node->extrude != getPreviousTextExtrude() )
    {
        setPreviousTextExtrude(layernodes->m_node->extrude);
        return true;
    }

    return false;
}

void
JahLayer::drawTexture(bool position_object)
{
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();

        motionNode* node = layernodes->m_node;

        if (position_object)
        {
            glTranslatef( node->tx, node->ty, node->tz );
            glRotatef( node->rx, 1.0, 0.0, 0.0 );
            glRotatef( node->ry, 0.0, 1.0, 0.0 );
            glRotatef( node->rz, 0.0, 0.0, 1.0 );

            glScalef ( node->sx, node->sy, node->sz );
        }

        glColor4f( this->Red,  this->Green,  this->Blue, layernodes->m_node->Alpha / 100 );

        float image_width_div_2 = float( textureData.getImageWidth() ) / 2.0f;
        float image_height_div_2 = float( textureData.getImageHeight() ) / 2.0f;

		int key_texture_width;
		int key_texture_height;

		getPowerOfTwoTextureSize(key_texture_width, key_texture_height, textureData.getImageWidth(), textureData.getImageHeight());

		float key_texture_width_ratio = float(textureData.getImageWidth()) / float(key_texture_width);
		float key_texture_height_ratio = float(textureData.getImageHeight()) / float(key_texture_height);

        glBindTexture(GL_TEXTURE_2D, textureData.getTexId());
        glEnable(GL_TEXTURE_2D);

        glBegin(GL_QUADS);
        {
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(-image_width_div_2, -image_height_div_2, 0.0f);

            glTexCoord2f(key_texture_width_ratio, 0.0f);
            glVertex3f(image_width_div_2, -image_height_div_2, 0.0f);

            glTexCoord2f(key_texture_width_ratio, key_texture_height_ratio);
            glVertex3f(image_width_div_2, image_height_div_2, 0.0f);

            glTexCoord2f(0.0f, key_texture_height_ratio);
            glVertex3f(-image_width_div_2, image_height_div_2, 0.0f);
        }
        glEnd();

        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
}

void                        
JahLayer::setCompositeType(CompositeType::TYPE type)
{
    switch (type)
    {
        case CompositeType::A_COMPOSITE_TYPE :
        {
            setSrcBlendFactor(GL_ONE);
            setDstBlendFactor(GL_ZERO);
            break;
        }
        case CompositeType::B_COMPOSITE_TYPE :
        {
            setSrcBlendFactor(GL_ZERO);
            setDstBlendFactor(GL_ONE);
            break;
        }
        case CompositeType::A_OVER_B_COMPOSITE_TYPE :
        {
            setSrcBlendFactor(GL_ONE);
            setDstBlendFactor(GL_ONE_MINUS_SRC_ALPHA);
            break;
        }
        case CompositeType::B_OVER_A_COMPOSITE_TYPE :
        {
            setSrcBlendFactor(GL_ONE_MINUS_DST_ALPHA);
            setDstBlendFactor(GL_ONE);
            break;
        }
        case CompositeType::A_IN_B_COMPOSITE_TYPE :
        {
            setSrcBlendFactor(GL_DST_ALPHA);
            setDstBlendFactor(GL_ZERO);
            break;
        }
        case CompositeType::B_IN_A_COMPOSITE_TYPE :
        {
            setSrcBlendFactor(GL_ZERO);
            setDstBlendFactor(GL_SRC_ALPHA);
            break;
        }
        case CompositeType::A_OUT_B_COMPOSITE_TYPE :
        {
            setSrcBlendFactor(GL_ONE_MINUS_DST_ALPHA);
            setDstBlendFactor(GL_ZERO);
            break;
        }
        case CompositeType::B_OUT_A_COMPOSITE_TYPE :
        {
            setSrcBlendFactor(GL_ZERO);
            setDstBlendFactor(GL_ONE_MINUS_SRC_ALPHA);
           break;
        }
        case CompositeType::A_ATOP_B_COMPOSITE_TYPE :
        {
            setSrcBlendFactor(GL_DST_ALPHA);
            setDstBlendFactor(GL_ONE_MINUS_SRC_ALPHA);
            break;
        }
        case CompositeType::B_ATOP_A_COMPOSITE_TYPE :
        {
            setSrcBlendFactor(GL_ONE_MINUS_DST_ALPHA);
            setDstBlendFactor(GL_SRC_ALPHA);
            break;
        }
        case CompositeType::A_XOR_B_COMPOSITE_TYPE :
        {
            setSrcBlendFactor(GL_ONE_MINUS_DST_ALPHA);
            setDstBlendFactor(GL_ONE_MINUS_SRC_ALPHA);
            break;
        }
        default :
        {
            setSrcBlendFactor(GL_SRC_ALPHA);
            setDstBlendFactor(GL_ONE_MINUS_SRC_ALPHA);
            break;
        }
    }

    m_composite_type = type;
}

bool    
JahLayer::canAcceptLighting()
{
    if (
              objtype == ObjectCategory::CUBE
           || objtype == ObjectCategory::BEZIERS
           || objtype == ObjectCategory::CYLINDER
           || objtype == ObjectCategory::JAHLAYER
           || objtype == ObjectCategory::JAHTEXT
           || objtype == ObjectCategory::MESHLAYER
           || objtype == ObjectCategory::OBJECT
           || objtype == ObjectCategory::SPHERE
           || objtype == ObjectCategory::BACKDROP
        )
    {
        return true;
    }

    return false;
}

//TONYBUG
bool    
JahLayer::layerIsEnabled()
{
    if (getCheckListItem())
    {
		return getCheckListItem()->isOn();
    }
    return false;
}

bool                        
JahLayer::isCameraLayer()
{
    return ( this == this->getCameraLayer() );
}

motionNode*         
JahLayer::findKeyframeAtTime(int frame_number)
{
    QPtrList<motionNode>* keyframe_list = layernodes->m_nodelist;
    motionNode* keyframe = keyframe_list->first();

    for ( ; keyframe; keyframe = keyframe_list->next() )
    {
        if (keyframe->m_frame_number == frame_number)
        {
            return keyframe;
        }
    }

    return NULL;
}


bool
JahLayer::getSupportsTransparency()
{
    return (   objtype == ObjectCategory::JAHLAYER
            || objtype == ObjectCategory::MESHLAYER
            || objtype == ObjectCategory::CUBE
            || objtype == ObjectCategory::SPHERE
            || objtype == ObjectCategory::CYLINDER
            || objtype == ObjectCategory::BEZIERS
           );
}

bool 
JahLayer::getLightingNeedsToUpdateCompositeTexture()
{
    float alpha = layernodes->m_node->Alpha;

    bool can_accept_lighting = canAcceptLighting();
    bool lighting_enabled = getLightingEnabled();
    bool supports_transparency = getSupportsTransparency();
    float previous_alpha = getPreviousAlpha();

    if ( blend && can_accept_lighting && lighting_enabled && supports_transparency )
    {
        if ( alpha != previous_alpha )
        {
            // Alpha value changed, need to do the extra rendering pass
            return true;
        }

        if ( getTextureCompositeDirty() && !getCompositeTextureUpdatedByLighting() )
        {
            // Composite was updated by effects, need to do the extra rendering pass
            return true;
        }

        if ( !getTextureCompositeDirty() && alpha < 99.9f )
        {
            // Lighting and transparency is on, need to do the extra rendering pass
            return true;
        }
    }

    return false;
}



