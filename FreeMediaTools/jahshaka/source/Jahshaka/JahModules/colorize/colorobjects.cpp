/*******************************************************************************
**
** The source file for the Jahshaka color objects module
** The Jahshaka Project
** Copyright (C) 2000-2004 The Jahshaka Project
** Released under the GNU General Public License
**
*******************************************************************************/


#include "color.h"
#include "pbuffer.h"

//this may not be necessary mon
/*
#if 0

void 
GLColorize::callEffect(EffectInfo::EFFECT_TYPE effect_type, JahLayer* effect_layer, JahLayer* base_layer, QImage& image) 
{
    if (effect_type == EffectInfo::JAH_PLUGIN_TYPE)
    {
        int plugin_number = effect_layer->getPluginNumber();
        doPlugin(base_layer, plugin_number, image);
    }
    else if (effect_type > EffectInfo::CPU_TYPE_START &&
             effect_type < EffectInfo::MESH_TYPE_START)
    {
        ( *this.*getCpuEffectFunction(effect_type) )(base_layer, image);
    }
    else if (effect_type > EffectInfo::MESH_TYPE_START &&
             effect_type < EffectInfo::GPU_TYPE_START)
    {
        ( *this.*getMeshEffectFunction(effect_type) )(base_layer);
    }
    else if (effect_type > EffectInfo::GPU_TYPE_START && //gpuactive &&
             effect_type < EffectInfo::EFFECT_TYPE_END)
    {
        if (gpusupport)
        {
            ( *this.*getGpuEffectFunction(effect_type) )(base_layer);
        }
    }
}



void
GLColorize::executeEffectsList(JahLayer* image_layer, bool use_pbuffer)
{  
    initializeMeshLayer(image_layer);

    QPtrList<EffectLayer>* effects_list = image_layer->getEffectsList();

    EffectLayer* last_effect_layer = effects_list->last();

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

    if ( use_pbuffer)
    {
        core->getPbuffer()->Bind();
    }

    previous_enabled_effect_count = enabled_effect_count;

    EffectLayer* effect_layer = effects_list->first();

    for (; effect_layer; effect_layer = effects_list->next())
    {
        bool last_effect = effect_layer == last_effect_layer;

        JahLayer* effect_jah_layer = effect_layer->getJahLayer();
        m_current_effect_layer_number = findLayer(effect_jah_layer);
        
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
    }
    
    if (use_pbuffer)
    {
        core->getPbuffer()->Unbind();
    }
    
    //if (not) scrubbing or playing or rendering update the buffer?
    if (!m_animation && !m_animsliderselected && ! getGpuActive()  && !renderStatus && useBuffering)
    {
    }
    
    forceupdatefxbuffer = false;
}

//we need to update the add-effect menud

void GLColorize::callEffect( EffectInfo::EFFECT_TYPE,  int, QImage& ) 
{
//break menu into rt and processor base
    //set up tracer
    //JahTrace& jtrace = JahTrace::getInstance();
    jtrace.debug( "GLColorize::","building effect#",i);

    effectobj = i;

    switch ( effect_type ) {

     //plugin image processing
      case EffectInfo::JAH_PLUGIN_TYPE      : {  doPlugin(newimage,i); break; }

      case EffectInfo::GREY_TYPE            : {  doGrey(newimage);                      break; }
      case EffectInfo::SELECT_COLORS_TYPE   : {  doColorSelect(newimage);               break; }
      case EffectInfo::CONTRAST_TYPE        : {  doContrast(newimage);                  break; }
      case EffectInfo::BRIGHTNESS_TYPE      : {  doBrightness(newimage);                break; }
      case EffectInfo::SWAP_RGB_TYPE        : {  doSwapRGB(newimage);                   break; }
      case EffectInfo::NEGATIVE_TYPE        : {  doNegative(newimage);                  break; }
      case EffectInfo::COLORIZE_TYPE        : {  doCPUColorize(newimage);               break; }

      //opengl based realtimefx
      case COLORMATRIX                : {  doColorMatrixRt();                     break; }      
	  case RTCOLORLUM                 : {  doColorMatrix();                      break; }

	  //for the gpu based effects
      case CHARCOAL_NV30_GPU          : {  doNV30CharcoalGPU();                   break; }


      default                         : {  break; }

    }
}


#endif
*/

