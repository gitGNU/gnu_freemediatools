/*******************************************************************************
**
** The source file for the Jahshaka animation module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "anime.h"
#include <qcheckbox.h>
#include <qlineedit.h>
#include <qslider.h>
#include <qlistview.h>
#include <InputLCD.h>
#include <coreobject.h>

/////////////////////////////////////////////////////
// we should only call updateSliders() when there has ben a click
// on a layer
// not as a part of the glcore...
// the only other time is when playing or scrubbing
// in which case we only update
//     updateSliderValues();

void GLAnime::updateSliders()
{   
    m_pJTrace->debug("in updateSliders");

    EffectLayer* effect_layer = NULL;

    //hide everything
    hideSliders(10);
    hideAllHeadings();

    if ( activeLayerIsAnEffect() )
    {
        effect_layer = getActiveEffectLayer();

        if(effect_layer != NULL && effect_layer->getEffectInfo()->getType() == EffectInfo::JAH_PLUGIN_TYPE)
        {
            //get the number of the plugin we are dealing with
            int plugFx = effect_layer->getPluginNumber();

            ///////////////////////////////////////////////////////////////
            //first we do the slider labels!
            bool us  = m_pJPlugIn->vfxPlugins->at(plugFx)->getUseSliders(); //

            if (us)
            {
                int  ns  = m_pJPlugIn->vfxPlugins->at(plugFx)->getNumberSliders(); //      3;

                for (int is=0; is<ns; is++)
                {
                    setSliderLabelValue(is, m_pJPlugIn->vfxPlugins->at(plugFx)->getSliderLabel(is));
                }

                showSliders(ns);
            }

            ///////////////////////////////////////////////////////////////
            //then we do the group labels!
            bool ul  = m_pJPlugIn->vfxPlugins->at(plugFx)->getUseLabels(); //

            if (ul)
            {
                int  nl  = m_pJPlugIn->vfxPlugins->at(plugFx)->getNumberLabels();

                for (int il=0; il<nl; il++)
                {
                    setHeadingValue(il, m_pJPlugIn->vfxPlugins->at(plugFx)->getLabel(il));
                }

                showHeadings(nl);
            }
        }
		else
		{
			//this means its a effect but its not a plugin
			//so we use the m_pCore set of sliders for now
			//we can pull up a color correction ui or keyer ui here
			//if we want to...
			applySliderNames(effect_layer);
            showSliders( (int)effect_layer->getSliderNameVector()->size() );
		}

	}
    else
	{
		//we need to differentiate between the world space 
		//and layer space now
		setDefaultSliderText();

		//if its the world layer we only show the first 6
		if (getActiveJahLayer() != m_pCamera )
		{
			showSliders(10);
			setDefaultHeadings();
			showAllHeadings();
		}
		else
		{        
			showSliders(6); //was 5
			setDefaultHeadings();
			showHeadings(2);
		}

        if ( getActiveJahLayer() && getActiveJahLayer()->objtype == ObjectCategory::JAHLIGHT )
        {
            showSlider(0, false);
            showSlider(1, false);
            showSlider(2, false);
            showSlider(6, false);
            showSlider(7, false);
            showSlider(8, false);
            showSlider(9, false);
        }
	}

    //update the rest of the slider/ui data
    updateSliderValues();
    slotUpdateListViewCheckBoxes();

    updateUiOptions();

    updateUiSettings();
}

//we override this here since the 'world' is now layer 1

void GLAnime::updateSliderValues()
{
    //m_pJTrace->debug("in updateSliderValues");

    if ( !multipleLayersSelectedInListView() && getActiveJahLayer() )
    {
        motionNode* motion_node = getActiveJahLayer()->layernodes->m_node;

       /////////////////////////////////////////////////////////////////////////////
        //basic values here
        if (getActiveJahLayer() != m_pCamera )
        {
            setSliderValue( 0, (int)motion_node->rx );
            setSliderValue( 1, (int)motion_node->ry );
            setSliderValue( 2, (int)motion_node->rz );

            setSliderValue( 3, (int)motion_node->tx );
            setSliderValue( 4, (int)motion_node->ty );
            setSliderValue( 5, (int)motion_node->tz );

            setSliderValue( 6, getScaleValue(motion_node->sx)  );
            setSliderValue( 7, getScaleValue(motion_node->sy)  );
            setSliderValue( 8, getScaleValue(motion_node->sz)  );

            setSliderValue( 9, (int)motion_node->Alpha  );

        }
        else //its the world space
        {
            setSliderValue( 0, (int)motion_node->rx );
            setSliderValue( 1, (int)motion_node->ry );
            setSliderValue( 2, (int)motion_node->rz );

            setSliderValue( 3, (int)motion_node->tx );
            setSliderValue( 4, (int)motion_node->ty );
            setSliderValue( 5, (int)motion_node->tz );
        }

        motionNode* interpolated_values;

        if ( getActiveJahLayer() && getActiveJahLayer()->objtype == ObjectCategory::EFFECT )
        {
            interpolated_values = getActiveJahLayer()->getParent()->layernodes->m_node;
        }
        else
        {
            interpolated_values = motion_node;
        }

#ifndef JAHPLAYER
        getLightingSliderPtr(0)->setValue( (int)( interpolated_values->ambient_light_color.x * 255.0f) );
        getLightingSliderPtr(1)->setValue( (int)( interpolated_values->ambient_light_color.y * 255.0f) );
        getLightingSliderPtr(2)->setValue( (int)( interpolated_values->ambient_light_color.z * 255.0f) );
        getLightingSliderPtr(3)->setValue( (int)( interpolated_values->diffuse_light_color.x * 255.0f) );
        getLightingSliderPtr(4)->setValue( (int)( interpolated_values->diffuse_light_color.y * 255.0f) );
        getLightingSliderPtr(5)->setValue( (int)( interpolated_values->diffuse_light_color.z * 255.0f) );
        getLightingSliderPtr(6)->setValue( (int)( interpolated_values->specular_light_color.x * 255.0f) );
        getLightingSliderPtr(7)->setValue( (int)( interpolated_values->specular_light_color.y * 255.0f) );
        getLightingSliderPtr(8)->setValue( (int)( interpolated_values->specular_light_color.z * 255.0f) );


#endif
    }
}

void GLAnime::updateUiOptions()
{
	
    ////////////////////////////////////////////////////////////////////////////////////
    // all other ui settings

    //first hide em all
    hideOptions(m_nNumUiOptions-1);

    EffectLayer* effect_layer = NULL;

    if ( activeLayerIsAnEffect() )
    {
        effect_layer = getActiveEffectLayer();

        if(effect_layer != NULL && effect_layer->getEffectInfo()->getType() == EffectInfo::JAH_PLUGIN_TYPE)
        {
	        int plugin_number = effect_layer->getPluginNumber();
            int  nb  = m_pJPlugIn->vfxPlugins->at(plugin_number)->getNumberButtons(); 

            for (int i=0; i<nb; i++)
            {
                m_aJahModuleOptions[i]->setText( m_pJPlugIn->vfxPlugins->at(plugin_number)->getButton(i) );
                m_aJahModuleOptions[i]->setChecked( m_pJPlugIn->vfxPlugins->at(plugin_number)->getOptionStatus(i) );
            //first we update the layer options
           }

            showOptions(nb-1);
        }
    } 
    else    //its not a effect so its a layer
    {
        //curently hardcoded to just 2 options
        //which would be the path and the light
        if ( getActiveJahLayer() && getActiveJahLayer() != m_pCamera ) //world has no options ?
        {
            m_pJTrace->debug("updating options");

            //setDefaultHeadings();
            showOptions(m_nNumUiOptions - 1);

            //get the layer in question
            JahLayer* active_layer = getActiveJahLayer();

            //first we update the layer options
            m_aJahModuleOptions[0]->setText("Lighting");
            m_aJahModuleOptions[1]->setText("Rotate First");
            m_aJahModuleOptions[2]->setText("Foreground");
            m_aJahModuleOptions[3]->setText("Use Depth");
            m_aJahModuleOptions[4]->setText("Reflection");
            m_aJahModuleOptions[5]->setText("Show Path");
            m_aJahModuleOptions[LAYER_SELECTED_VISIBLE_OPTION]->setText("Show Layer");

            m_aJahModuleOptions[0]->setChecked( active_layer->getLightingEnabled() ); //reflection ? getActiveJahLayer()->drawKeyframes
            m_aJahModuleOptions[1]->setChecked( active_layer->getTranslateFirst() );
            m_aJahModuleOptions[2]->setChecked(active_layer->foreground);
            m_aJahModuleOptions[3]->setChecked(active_layer->depth);
            m_aJahModuleOptions[4]->setChecked(active_layer->reflect);
            m_aJahModuleOptions[LAYER_SELECTED_VISIBLE_OPTION]->setChecked( active_layer->getIsSelectedVisible() );
        }
    }

}

void GLAnime::updateUiSettings()
{
    m_pJTrace->debug("in updateUiSettings");

    if ( !getActiveJahLayer() )
    {
        return;
    }

    //get the layer in question
    JahLayer* active_layer = getActiveJahLayer();

    m_pUseOpenGLKeyLayerCheckBox->setChecked( active_layer->getUseOpenGlKeyer() );
    m_pUseFastShaderKeyLayerCheckBox->setChecked( active_layer->getUseFastShaderKeyer() );


    //update the world values
    GridSelect->setChecked(m_bGridVal);
    TextSelect->setChecked(m_pTextSafe->layerStatus);

    //for the objects panel
    smooth1->setChecked(active_layer->smooth);

    //for the 3dobjects panel
    //shader1->setChecked(active_layer->pixelshader);
    //shader2->setChecked(active_layer->pixelshader);
            
    ///////////////////////////////////////////////////////////////////////
    //for the media panel

    //for the asset
    ClipSelect->setChecked(active_layer->drawtheLayer);

    if (active_layer->layerAssetStatus) 
    {
        m_pSlipFramesLCD->setValue( active_layer->getSlipFrameOffset() );
        m_pInFramesLCD->setValue ( active_layer->m_inframe );
        m_pOutFramesLCD->setValue( active_layer->m_outframe );
    }
    else
    {
        m_pSlipFramesLCD->setValue(0);
        m_pInFramesLCD->setValue(0);
        m_pOutFramesLCD->setValue(0);
    }

    //for the key
    KeySelect->setChecked(active_layer->drawtheKey);

    if (active_layer->keyAssetStatus)
    {
        m_pKeySlipFramesLCD->setValue( active_layer->getKeySlipFrameOffset() );
        m_pKeyInFramesLCD->setValue( active_layer->m_keyinframe );
        m_pKeyOutFramesLCD->setValue( active_layer->m_keyoutframe );
    }
    else
    {
        m_pKeySlipFramesLCD->setValue(0);
        m_pKeyInFramesLCD->setValue(0);
        m_pKeyOutFramesLCD->setValue(0);
    }

    //the media options here
    //currently replicated for the key for continuity
    extendHeadTail->setChecked ( active_layer->showEdges );
    toggleTheLoop->setChecked ( active_layer->loop );
    togglePing->setChecked ( active_layer->ping );

    //this is to update the locate media layerbox
    QString thedata = active_layer->asset.getFullFileName();
    FindMediaText->setText( thedata );

}



void GLAnime::setOption1(void)  //used for path global for layers
{
    //only plugins and regular layers have options
    //hard coded effects dont
    if ( activeLayerIsAnEffect() )
    {
        parsePluginOption(1);
        updateEffects();
        updateGL();
    }
    else
    {
        bool lighting_flag = m_aJahModuleOptions[LIGHTING_OPTION]->isChecked();
        getActiveJahLayer()->setLightingEnabled(lighting_flag);
        updateGL();
    }
}

void GLAnime::setOption2(void) 
{
    if ( activeLayerIsAnEffect() )
    {
        parsePluginOption(2);
        updateEffects();
        updateGL();
    }
    else
    {
        bool translate_first_flag = m_aJahModuleOptions[TRANSLATE_FIRST_OPTION]->isChecked();
        getActiveJahLayer()->setTranslateFirst(translate_first_flag);
        updateGL();
    }
}

void GLAnime::setOption3(void)  //used for foreground lock
{
    if ( activeLayerIsAnEffect() )
    {
        parsePluginOption(3);
        updateEffects();
        updateGL();
    }
    else
    {
        getActiveJahLayer()->foreground = m_aJahModuleOptions[FOREGROUND_OPTION]->isChecked();
        updateGL(); 
    }
}

void GLAnime::setOption4(void) //used for depth lock
{
    if ( activeLayerIsAnEffect() )
    {
        parsePluginOption(4);
        updateEffects();
        updateGL();
    }
    else
    {
        getActiveJahLayer()->depth = m_aJahModuleOptions[DEPTH_OPTION]->isChecked();
        updateGL();
    }
}

void GLAnime::setOption5(void) //used for reflection
{
    if ( activeLayerIsAnEffect() )
    {
        parsePluginOption(5);
        updateEffects();
        updateGL();
    }
    else
    {
        getActiveJahLayer()->reflect = m_aJahModuleOptions[REFLECTION_OPTION]->isChecked();
        updateGL();
    }
}

void GLAnime::setReflectionState(JahLayer* jah_layer, bool flag) 
{ 
    setActiveJahLayer(jah_layer);
    m_aJahModuleOptions[REFLECTION_OPTION]->setChecked(flag);
}

void
GLAnime::slotSetLayerSelectedVisible()
{
    bool flag = m_aJahModuleOptions[LAYER_SELECTED_VISIBLE_OPTION]->isChecked();

    JahLayer* jah_layer = getActiveJahLayer();

    if (jah_layer)
    {
        jah_layer->setIsSelectedVisible(flag);
    }

    updateGL();
}

