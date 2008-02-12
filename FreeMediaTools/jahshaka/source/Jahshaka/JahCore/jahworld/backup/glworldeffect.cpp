/*******************************************************************************
**
** The source code for the Jahshaka glworldeffect.cpp file
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/
#ifdef _MSC_VER
#pragma warning (disable : 4786) 
#endif

#include "glworld.h"
#include <qslider.h>
#include <qlabel.h>
#include <qpopupmenu.h>
#include <qlistview.h>
#include "glworldlists.h"
#include "glcore.h"
#include <InputLCD.h>
#include "blurlib.h"
#include "jahpluginlib.h"
#include "pbuffer.h"
#include <string>

#ifndef JAHPLAYER
#include "jahnodes.h"
#endif

#include <sliderbutton.h>

////////////////////////////////////////////////////////////
//world and layer fog controls
void GLWorld::setFog() {
    getActiveJahLayer()->layerfog = !getActiveJahLayer()->layerfog;

}

void GLWorld::setHFog(int hb)
{
    m_fHFog=  (GLfloat)hb;
    updateFog();
}

void GLWorld::setVFog(int vb)
{
    m_fVFog = (GLfloat)vb;
    updateFog();

}

void GLWorld::updateFog(void)
{
    //glFogi(GL_FOG_MODE, fogMode[fogfilter]);       // Fog Mode
    //glFogfv(GL_FOG_COLOR, fogColor);               // Set Fog Color
    //glFogf(GL_FOG_DENSITY, 0.45f);                 // How Dense Will The Fog Be
    //glHint(GL_FOG_HINT, GL_NICEST);                // Fog Hint Value

    glFogf(GL_FOG_START, m_fHFog);                    // Fog Start Depth : 1000.0f
    glFogf(GL_FOG_END, m_fVFog);                      // Fog End Depth   : 0.0 f

    updateGL();
}

int
GLWorld::m_nNumberEmbeddedCPUEffects = 
		EffectInfo::MESH_TYPE_START - EffectInfo::CPU_TYPE_START - 1;

int
GLWorld::m_nNumberEmbeddedMeshEffects = 
		EffectInfo::GPU_TYPE_START - EffectInfo::MESH_TYPE_START - 1;

int
GLWorld::m_nNumberEmbeddedGPUEffects = 
		EffectInfo::EFFECT_TYPE_END - EffectInfo::GPU_TYPE_START - 1;


/////////////////////////////////////////////////////////////////////////////////
// for the plugins
// this can be moved into the plugin library itself
int 
GLWorld::addPluginsToMenu(int usingclass, int existingitems, QPopupMenu *menu)
{
    //math based functions
    //int plugcount = 0;
    QPopupMenu *  submenu[10];
    QString     * submenutitle[10];
    int submenucount = 0;
    
    int menuitem = existingitems;
    
    for (int x=0; x < m_pJPlugIn->vfxPluginsCounter; x++)
    {
        // first check for family
        //  if fmaily exists we add plugin there
        //      then check for group
        //          if group exists we add plugin in family, in group
        
        QString name, family, group;
        
        name   = m_pJPlugIn->vfxPlugins->at(x)->getPluginName();
        family = m_pJPlugIn->vfxPlugins->at(x)->getPluginFamily();
        group  = m_pJPlugIn->vfxPlugins->at(x)->getPluginGroup();
        
        //////////////////////////////////////////////////////////////
        //need to get plugin type so we can put it in the right menu ie gpu etc
        //need to also get arb or nv support...
        JAH_PLUGIN_CLASS pluginclass;
        pluginclass = m_pJPlugIn->vfxPlugins->at(x)->getPluginClass();
        if (pluginclass!=usingclass) continue;
        
        
        ///////////////////////////////////////////////////////////////
        //plugin isnt part of a family we put it in the base menu
        if (family=="NULL")
        {
            menu->insertItem( name,menuitem,0 );
        }
        else
        {
            //////////////////////////////////////////////////////////////
            //its part of a family we put it in a submenu
            //need to check if the submenu already exists for reusability
            bool exists = false;;
            int submenuexists = 0;
            
            if (submenucount>0)
            {
                for (int i=0; i<submenucount; i++)
                {
                    if ( *submenutitle[i]==family )
                    {
                        exists = true;
                        submenuexists = i;
                    }
                    if (exists) break;
                }
            }
            
            if (exists)
            {
                submenu[submenuexists]->insertItem( name, menuitem, 0 );
            }
            else
            {
                submenutitle[submenucount] = new QString(family);
                submenu[submenucount] = new QPopupMenu(0);
                
                Q_CHECK_PTR( submenu[submenucount] );
                submenu[submenucount]->insertItem( name, menuitem, 0 );
                
                menu->insertItem( family, submenu[submenucount] );
                submenucount++;
            }
        }

        //means we added it so increment
        menuitem ++;
    }
    
    return menuitem;
}

EffectLayer*        
GLWorld::addEffectLayer(JahLayer* parent, std::string& title, EffectInfo::EFFECT_TYPE effect_type, int plugin_id, std::string& guid)
{
    QString qstring_title = QString( title.data() );

    if (effect_type == EffectInfo::JAH_PLUGIN_TYPE && plugin_id < 0)
    {
        return NULL;
    }

    EffectLayer* new_layer = addEffectLayer( parent->getLayerListEntry(), qstring_title, effect_type, plugin_id, guid);

    return new_layer;
}


EffectLayer* 
GLWorld::addEffectLayer(LayerListEntry* parent, QString& title, EffectInfo::EFFECT_TYPE effect_type, int plugin_id, std::string& guid)
{
    JahLayer* image_layer = parent->getJahLayer();

    // It doesn't make sense to add a new layer for an effect, but until the sliders
    // get sorted out it will have to do  FIXME
    LayerListEntry* new_layerlistentry = new LayerListEntry(title);
    JahLayer* new_layer = new_layerlistentry->thelayer;
    new_layer->layername = title;
    new_layer->m_layer_name_string = std::string( title.data() );
    new_layer->layertype = LayerCategory::LAYER;
    new_layer->objtype = ObjectCategory::EFFECT;
    new_layer->setParent(parent->thelayer);

    getLayerList()->append(new_layerlistentry);
    setActiveJahLayer(new_layer);
    EffectLayer* new_effect_layer = parent->addEffectLayer(effect_type, true, plugin_id);
    new_effect_layer->setJahLayer(new_layer); 
    new_effect_layer->effectname = title;
    new_layer->setEffectLayer(new_effect_layer);
    new_layer->setPluginNumber(plugin_id); 
    new_effect_layer->setGuid(guid);

    if (   new_effect_layer->getType() == EffectInfo::COLORIZE_CPU_TYPE 
        || new_effect_layer->getType() == EffectInfo::COLORIZE_GPU_TYPE
        )
    {
        m_aJahSliders[9]->setMaxValue(500);
        m_aJahSliderLCDs[9]->setMaxInt(500);
        m_aJahSliders[9]->setMinValue(1);
        m_aJahSliderLCDs[9]->setMinInt(1);
    }


    //need to move this code out of here and into the modules
	//not sure why we do this???
    if (effect_type == EffectInfo::JAH_PLUGIN_TYPE)
	{
#ifndef JAHPLAYER
		///////////////////////////////////////////////////////
		//these should not be in the core as they are UI code
		setPluginAxisSliderLabels(new_effect_layer);
#endif

	}
	else
	{
		if ( (m_qsModuleName != "Color") && (m_qsModuleName != "Keyer") && 
            (m_qsModuleName != "Tracker") )
		{
            std::string name_string = m_qsModuleName.data();
			///////////////////////////////////////////////////////
			//these should not be in the core as they are UI code
			#ifndef JAHPLAYER

            if (m_qsModuleName !="Paint" && m_qsModuleName != "Text")
            {
                setSliderNames(new_effect_layer);
                applySliderNames(new_effect_layer);
            }
            else
            {
                setDefaultAxisSliderLabels();
            }

			#endif
		}
	}

    
    QCheckListItem* new_item = 
        (QCheckListItem*)addEffectToUserInterface(parent->thelayer->getCheckListItem(), title, true, true);
    QListViewItem* current_item = m_pLayerListView->currentItem();

    if (current_item == NULL)
    {
        m_pLayerListView->setCurrentItem(new_item);
    }

    // Reorder the listview from top to bottom
    new_layer->setCheckListItem(new_item);
    new_layer->layerStatus = true;
    //forceupdatefxbuffer = true;

    image_layer->reorderListViewItems();
    // The reorder routine apparently changes the current item
    m_pLayerListView->setCurrentItem(current_item);

	///////////////////////////////////////////////////////
	//these should not be in the core as they are UI code
	#ifndef JAHPLAYER
    resetAllSliders();
    updateSliders();
    #ifndef NEWWIREUPWIDGET
        if (m_bHasNodeFramer)
            m_pNodeFramer->addNodes(getNumberOfLayers(),title);
    #endif
	#endif

	updateGL();
 
    return new_effect_layer;
}

bool 
GLWorld::isQimageBlack(QImage& image, char* message)
{
    for (int y = 0; y < image.height(); y++ )
    {
        for ( int x = 0; x < image.width(); x++ )
        {
            QRgb* pixel = (QRgb *)image.scanLine(y) + x; 

            if (*pixel != 0xff000000)
            {
                printf("isQimageBlack: %s Pixel x = %d y = %d is %08x\n", message, x, y, *pixel);
                fflush(stdout);
                return false;
            }
        }
    }

    return true;
}

void
GLWorld::executeEffectsList(JahLayer* image_layer, bool use_pbuffer, bool draw_only_to_selected_effect)
{  
    initializeMeshLayer(image_layer);

    QPtrList<EffectLayer>* effects_list = image_layer->getEffectsList();

    EffectLayer* last_effect_layer = effects_list->last();

    if (draw_only_to_selected_effect)
    {
        last_effect_layer = getActiveJahLayer()->getEffectLayer();
    }

    while (   ( last_effect_layer && !last_effect_layer->getJahLayer()->layerIsEnabled() )
           || ( last_effect_layer &&
                (last_effect_layer->getJahLayer()->getEffectCategory() == EffectInfo::MESH_CATEGORY)
              )
          )
    {
        last_effect_layer = effects_list->prev();
    }

    image_layer->setQimageCompositeDirty(false);
    image_layer->setTextureCompositeDirty(false);
    image_layer->setPreviousRenderedLayerEffectCategory( EffectInfo::NOT_A_CATEGORY);

    static int previous_enabled_effect_count = 0;
    int enabled_effect_count = image_layer->getEnabledEffectCount();

    if (enabled_effect_count == 0 && 
        enabled_effect_count != previous_enabled_effect_count)
    {
        updateTextureFromQImage(image_layer->getTextureComposite(), 
            &(image_layer->textureData.objectImage),
            image_layer);

        previous_enabled_effect_count = enabled_effect_count;
        return;
    }

    if (use_pbuffer)
    {
        m_pCore->bindPbuffer();
    }

    glClear(GL_COLOR_BUFFER_BIT);

    previous_enabled_effect_count = enabled_effect_count;

    EffectLayer* effect_layer = effects_list->first();

    for (; effect_layer; effect_layer = effects_list->next())
    {
        bool last_effect = effect_layer == last_effect_layer;

        JahLayer* effect_jah_layer = effect_layer->getJahLayer();
        m_pCurrentEffectLayer = effect_jah_layer;
        
        if ( effect_jah_layer->layerIsEnabled() )
        {
            EffectInfo::EFFECT_CATEGORY effect_category = effect_jah_layer->getEffectCategory();
            EffectInfo::EFFECT_CATEGORY previous_effect_category = 
                        image_layer->getPreviousRenderedLayerEffectCategory();

            if (effect_category == EffectInfo::CPU_CATEGORY)
            {
                if (previous_effect_category == EffectInfo::GPU_CATEGORY)
                {
                    // Copy the composite texture into the composite qimage 
                    updateQimageFromTexture(image_layer->getCompositeQimagePtr(), 
                                            image_layer->getTextureComposite() );

                    image_layer->setQimageCompositeDirty(true);
                }
                else if ( !image_layer->getQimageCompositeDirty() )
                {
                    // First CPU effect.  Copy the original QImage into the working buffer
                    *( image_layer->getCompositeQimagePtr() ) = 
                        image_layer->textureData.objectImage.copy();
                }

                image_layer->setQimageCompositeDirty(true);
            }
            else if ( 
                        (effect_category == EffectInfo::GPU_CATEGORY && 
                         previous_effect_category == EffectInfo::CPU_CATEGORY )
                    )
            {
                updateTextureFromQImage(image_layer->getTextureComposite(), 
                                        image_layer->getCompositeQimagePtr(),
                                        image_layer);
            }

            callEffect(effect_layer->getType(), effect_jah_layer, image_layer, 
                       *(image_layer->getCompositeQimagePtr()) );

            if (last_effect && effect_category == EffectInfo::CPU_CATEGORY)
            {
                updateTextureFromQImage(image_layer->getTextureComposite(), 
                                        image_layer->getCompositeQimagePtr(),
                                        image_layer);
            }

            if (effect_category == EffectInfo::MESH_CATEGORY &&
                previous_effect_category == EffectInfo::NOT_A_CATEGORY)
            {
            }
            else
            {
                image_layer->setTextureCompositeDirty(true);
            }

            if (effect_category != EffectInfo::MESH_CATEGORY)
            {
                // Mesh effects don't change the image, so don't record them here
                image_layer->setPreviousRenderedLayerEffectCategory(effect_category);
            }
        } 

        if (draw_only_to_selected_effect)
        {
            if ( effect_jah_layer == getActiveJahLayer() )
            {
                break;
            }
        }
    }
    
    if (use_pbuffer)
    {
        m_pCore->unbindPbuffer();
    }
}


void 
GLWorld::callEffect(EffectInfo::EFFECT_TYPE effect_type, JahLayer* effect_layer, JahLayer* base_layer, QImage& image) 
{
    if (effect_type == EffectInfo::JAH_PLUGIN_TYPE)
    {
        int plugin_number = effect_layer->getPluginNumber();
        doPlugin(base_layer, plugin_number, image);
    }
    else if (effect_type > EffectInfo::CPU_TYPE_START &&
             effect_type < EffectInfo::MESH_TYPE_START)
    {
        ( *this.*getCPUEffectFunction(effect_type) )(base_layer, image);
    }
    else if (effect_type > EffectInfo::MESH_TYPE_START &&
             effect_type < EffectInfo::GPU_TYPE_START &&
             !base_layer->drawtheKey)
    {
        ( *this.*getMeshEffectFunction(effect_type) )(base_layer);
    }
    else if (effect_type > EffectInfo::GPU_TYPE_START && //gpuactive &&
             effect_type < EffectInfo::EFFECT_TYPE_END)
    {
        if (m_bGPUSupport)
        {
            ( *this.*getGPUEffectFunction(effect_type) )(base_layer);
        }
    }
}


bool
GLWorld::setTextureImageParameters(TextureImage* texture_image, int texture_width, int texture_height, 
                          int buffer_width, int buffer_height, GLuint texture_id)
{
    texture_image->height = texture_height;
    texture_image->width = texture_width;
    texture_image->texwidthratio = float(buffer_width) / float(texture_width);
    texture_image->texheightratio = float(buffer_height) / float(texture_height);
    texture_image->mapwidth = buffer_width / 2;
    texture_image->mapheight = buffer_height / 2;
    texture_image->texID = texture_id;

    return true;
}


bool
GLWorld::updateQimageFromTexture(QImage* q_image, TextureImage& texture_image)
{
    int texture_width = ( int )texture_image.wresolution;
    int texture_height = ( int )texture_image.hresolution;
    int image_width = ( int )texture_image.width;
    int image_height = ( int )texture_image.height;
    uint texture_id = texture_image.texID;

    int image_buffer_size = texture_width * texture_height * 4;
    GLubyte* image_buffer = new GLubyte[image_buffer_size];

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE, image_buffer);

    if (   (image_width != texture_width)
        || (image_height != texture_height) )
    {
        getRectangleSubImage(image_buffer, image_buffer, 0, 0, image_width, image_height, texture_width, texture_height, texture_width);
    }

    updateQimageFromRgbaBuffer(q_image, (unsigned int*)image_buffer, image_width, image_height);
    delete [] image_buffer;

    return true;
}

bool
GLWorld::updateTextureFromQImage(TextureImage& texture_image, QImage* q_image, JahLayer* jah_layer)
{
    int image_width = ( int )texture_image.width;
    int image_height = ( int )texture_image.height;

    if (image_width == 0 || image_height == 0)
    {
        return false;
    }

    uint texture_id = texture_image.texID;
    unsigned int image_format;

    *( jah_layer->getCompositeQimageFlippedPtr() ) = q_image->mirror(false, true);

    if ( q_image->numColors() > 0 )
    {
        image_format = GL_RED;
    }
    else
    {
        image_format = GL_BGRA;
    }

    check_gl();
    glBindTexture(GL_TEXTURE_2D, texture_id);
    check_gl();
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
    check_gl();
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
    check_gl();
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image_width, image_height, image_format, GL_UNSIGNED_BYTE, 
                    jah_layer->getCompositeQimageFlippedPtr()->bits() ) ;
    check_gl();

    return true;
}

bool
GLWorld::updateQimageFromRgbaBuffer(QImage* q_image, unsigned int* image_buffer, int image_width, int image_height)
{
    int image_height_minus_one = image_height - 1;

    // This safety feature is here in case a QImage size is incorrect -
    if (q_image->width() != image_width || q_image->height() != image_height)
    {
        q_image->create(image_width, image_height, 32);
    }

    for ( int y = 0; y < image_height; y++ )
    {
        for ( int x = 0; x < image_width; x++ )
        {
            QRgb* pixel_address = (QRgb*)q_image->scanLine(y) + x;
            unsigned int* buffer_ptr = (unsigned int*)&image_buffer[0] + ((image_height_minus_one - y) * image_width) + x;
            char red = qRed(*buffer_ptr);
            char green = qGreen(*buffer_ptr);
            char blue = qBlue(*buffer_ptr);
            *pixel_address = qRgb(red, green, blue);
        }
    }

    return true;
}

EffectLayer*        
GLWorld::getActiveEffectLayer()
{
    JahLayer* active_jah_layer = getActiveJahLayer();

    if ( activeLayerIsAnEffect() )
    {
        return active_jah_layer->getEffectLayer();
    }

    return NULL;
}

bool                
GLWorld::activeLayerIsAnEffect()
{
    JahLayer* active_jah_layer = getActiveJahLayer();

    if ( active_jah_layer->getCategory() == ObjectCategory::EFFECT )
    {
        return true;
    }

    return false;
}

std::string& 
EffectInfo::getEffectCategoryName(EFFECT_CATEGORY category)
{
    static std::string  effect_category_name;

    switch (category)
    {
        case CPU_CATEGORY :
        {
            effect_category_name = "CPU_CATEGORY";
            break;
        }
        case MESH_CATEGORY :
        {
            effect_category_name = "MESH_CATEGORY";
            break;
        }
        case GPU_CATEGORY :
        {
            effect_category_name = "GPU_CATEGORY";
            break;
        }
        default:
        {
            effect_category_name = "NO_CATEGORY";
            break;
        }
    }

    return effect_category_name;
}

std::string& 
EffectInfo::getEffectTypeName(EFFECT_TYPE type)
{
    if (type <= NOT_A_TYPE)
    {
        return m_name_table[type];
    }

    return m_name_table[NOT_A_TYPE];
}

void
GLWorld::loopThroughLayers(std::string )
{
    LayerListEntry* layer = getLayerList()->first();

    for ( ; layer; layer = getLayerList()->next() )
    {
        JahLayer* jah_layer = layer->thelayer;
        std::string name = std::string( jah_layer->layername.data() );
    }
}

void        
GLWorld::setSliderNames(EffectLayer* effect_layer)
{
    switch ( effect_layer->getType() )
    {
        case EffectInfo::FLAG_MESH_TYPE :
        {
            effect_layer->setSliderName("Number Of Waves");
            effect_layer->setSliderName("Wave Position");
            effect_layer->setSliderName("Wave Height");
            break;
        }

        case EffectInfo::FISHEYE_MESH_TYPE :
        {
            effect_layer->setSliderName("X Position");
            effect_layer->setSliderName("Y Position");
            effect_layer->setSliderName("Radius");
            break;
        }

        case EffectInfo::RIPPLE_MESH_TYPE :
        {
            effect_layer->setSliderName("RippleRadius");
            effect_layer->setSliderName("Ripple Height");
            break;
        }

        case EffectInfo::SWIRL_MESH_TYPE :
        {
            effect_layer->setSliderName("X Center");
            effect_layer->setSliderName("Y Center");
            effect_layer->setSliderName("Radius");
            effect_layer->setSliderName("Twist");
            break;
        }

        case EffectInfo::EARTHQUAKE_GPU_TYPE :
        {
            effect_layer->setSliderName("Number of Layers");
            effect_layer->setSliderName("Speed");
            effect_layer->setSliderName("Clear Radius");
            effect_layer->setSliderName("X Displacement");
            effect_layer->setSliderName("Y Displacement");
            effect_layer->setSliderName("Z Displacement");
            break;
        }

        case EffectInfo::CHROMA_KEYER_GLSL_TYPE :
        case EffectInfo::CHROMA_KEYER_CPU_TYPE :
        case EffectInfo::CHROMA_KEYER_GLSL_SEPARATE_ALPHA_TYPE :
        {
            effect_layer->setSliderName("Red Low");
            effect_layer->setSliderName("Green Low");
            effect_layer->setSliderName("Blue Low");
            effect_layer->setSliderName("Red High");
            effect_layer->setSliderName("Green High");
            effect_layer->setSliderName("Blue High");
            effect_layer->setSliderName("Soften");
            break;
        }

        case EffectInfo::CHARCOAL_GPU_TYPE :
        {
            effect_layer->setSliderName("Light Position X");
            effect_layer->setSliderName("Light Position Y");
            effect_layer->setSliderName("Light Position Z");
            effect_layer->setSliderName("Transparency");
            effect_layer->setSliderName("Blend");
            effect_layer->setSliderName("Random Factor");
            effect_layer->setSliderName("Density");
            effect_layer->setSliderName("Exponent");
            effect_layer->setSliderName("Lightness");
            break;
        }

        case EffectInfo::LIGHTING_GPU_TYPE :
        {
            effect_layer->setSliderName("Light Position X");
            effect_layer->setSliderName("Light Position Y");
            effect_layer->setSliderName("Light Position Z");
            effect_layer->setSliderName("Red");
            effect_layer->setSliderName("Green");
            effect_layer->setSliderName("Blue");
            break;
        }

        case EffectInfo::COLORIZE_CPU_TYPE :
        case EffectInfo::COLORIZE_GPU_TYPE :
        {
            effect_layer->setSliderName("Red");
            effect_layer->setSliderName("Green");
            effect_layer->setSliderName("Blue");
            effect_layer->setSliderName("Tint");
            effect_layer->setSliderName("Brightness");
            effect_layer->setSliderName("Strength");
            effect_layer->setSliderName("Hue");
            effect_layer->setSliderName("Saturation");
            effect_layer->setSliderName("Value");
            effect_layer->setSliderName("Gamma");
            break;
        }

        case EffectInfo::STRETCH_CROP_GPU_TYPE :
        {
            effect_layer->setSliderName("Left");
            effect_layer->setSliderName("Right");
            effect_layer->setSliderName("Bottom");
            effect_layer->setSliderName("Top");
            effect_layer->setSliderName("Horizontal Offset");
            effect_layer->setSliderName("Vertical Offset");
            effect_layer->setSliderName("Stretch");
            effect_layer->setSliderName("Zoom");
            break;
        }

		case EffectInfo::EDGE_DETECT_CPU_TYPE :
        {
            effect_layer->setSliderName("Sensitivity");
            effect_layer->setSliderName("Edge Color");
            effect_layer->setSliderName("Back Color");
            break;
        }

        case EffectInfo::MATRIX_EMBOSS_CPU_TYPE :
        {
            effect_layer->setSliderName("Emboss Sum");
            break;
        }

        case EffectInfo::SELECT_COLORS_CPU_TYPE :
        {
            effect_layer->setSliderName("Red");
            effect_layer->setSliderName("Green");
            effect_layer->setSliderName("Blue");
            break;
        }

        case EffectInfo::GAUSSIAN_BLUR_CPU_TYPE :
        {
            effect_layer->setSliderName("Horizontal Blur");
            effect_layer->setSliderName("Vertical Blur");
            break;
        }

        default :
        {
            break;
        }
    }
}

void 
GLWorld::applySliderNames(EffectLayer* effect_layer)
{
    std::vector<std::string>* slider_name_vector = effect_layer->getSliderNameVector();
    int slider_count = int( slider_name_vector->size() );
    int slider_number;

    for (slider_number = 0; slider_number < slider_count; slider_number++)
    {
      m_aJahSliderLabels[slider_number]->setText( (*slider_name_vector)[slider_number].data() );
      m_aJahSliderLabels[slider_number]->show();
    }
}

