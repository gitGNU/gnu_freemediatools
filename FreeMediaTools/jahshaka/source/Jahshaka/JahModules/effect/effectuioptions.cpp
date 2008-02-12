/*******************************************************************************
**
** The source file for the Jahshaka effect module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "effect.h"
#include <glcore.h>
#include <coreobject.h>
#include <qlistview.h>
#include <InputLCD.h>
#include <qlineedit.h>

void GLEffect::updateSliders()
{   
    m_pJTrace->debug("in updateSliders");

    //need to keep track of last layer m_bSelected
    //vs current layer m_bSelected and layer types
    //as if they havent changed we dont need to hide and reshow the sliders
    //we just need to update values...

    EffectLayer* effect_layer = NULL;

    //hide everything
    hideSliders(10);

    hideAllHeadings();

    if ( activeLayerIsAnEffect() )
    {
        effect_layer = getActiveEffectLayer();
  
        if(effect_layer != NULL && effect_layer->getEffectInfo()->getType() == EffectInfo::JAH_PLUGIN_TYPE)
	    {
            m_pJTrace->debug( "GLAnime","updating plugin slider display");
            //plugins currently have a lot of ui control

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
            //first we do the group labels!
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
			//setDefaultSliderText();

            showSliders( (int)effect_layer->getSliderNameVector()->size() );
			//setDefaultHeadings();
			//showAllHeadings();
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
			showSliders(5); //was 5
			setDefaultHeadings();
			showHeadings(2);
		}
	}

    //update the rest of the slider/ui data
    updateSliderValues();

    updateUiOptions();

    updateUiSettings();

}

//we override this here since the 'world' is now layer 1

void GLEffect::updateSliderValues()
{
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

    }
	
}

void GLEffect::updateUiOptions()
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
   

}

void GLEffect::updateUiSettings()
{
    VideoRenderSelect->setChecked(m_pCore->useimagesize);

    if ( !getActiveJahLayer() )
    {
        return;
    }

    //get the layer in question
    JahLayer* active_layer = getActiveJahLayer();

    m_pUseOpenGLKeyLayerCheckBox->setChecked( active_layer->getUseOpenGlKeyer() );


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


void GLEffect::setOption1(void)
{
    if ( activeLayerIsAnEffect() )
    {
        parsePluginOption(1);
        updateEffects();
        updateGL();
    }
}

void GLEffect::setOption2(void)
{
    if ( activeLayerIsAnEffect() )
    {
        parsePluginOption(2);
        updateEffects();
        updateGL();
    }
}

void GLEffect::setOption3(void)
{
    if ( activeLayerIsAnEffect() )
    {
        parsePluginOption(3);
        updateEffects();
        updateGL();
    }
}

void GLEffect::setOption4(void)
{
    if ( activeLayerIsAnEffect() )
    {
        parsePluginOption(4);
        updateEffects();
        updateGL();
    }
}

//we use this to disconnect ui form the m_pCore
void GLEffect::changeVideoRender()
{
	bool status = VideoRenderSelect->isChecked();

	SetVideoRender(status);
}

void
GLEffect::changeRenderOnlyToSelectedEffect()
{
    bool flag = m_render_only_to_selected_effect_qcheckbox->isChecked();

    setRenderOnlyToSelectedEffect(flag);

    if ( getActiveJahLayer()->getParent() )
    {
        getActiveJahLayer()->getParent()->setEffectsSlidersHaveChanged(true);
    }

    updateGL();
}


