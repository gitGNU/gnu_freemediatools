/*******************************************************************************
**
** The source file for the Jahshaka glworld.cpp module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "glworld.h"
#include "glworldlists.h"
#include <qslider.h>
#include <timelineSlider.h>
#include <qtimer.h>
#include <qlistview.h>
#include <iostream.h>

#ifndef JAHPLAYER
#include "keyframer.h"
#endif


////////////////////////////////////////////////
// Animation related

void GLWorld::updateVideoTexture(JahLayer* jah_layer, int theframe) 
{
    //if theres a effect on the layer then update effects as necessary
    if (jah_layer->layerEffect) 
    {
        m_bUpdateEffect = true;
    }

    jah_layer->updateVidTex(theframe);
}

void GLWorld::toggleAnimation(bool value) 
{
    m_bAnimation = value;

    if ( getAnimating() ) 
    { 
        scheduleUpdate(); 
    }
    else
    {
        m_pTimer->stop(); 
    }

}

void GLWorld::startanimation() 
{    
    if ( getAnimating() )
    {
        toggleAnimation(FALSE);
    }
    else
    {
        toggleAnimation(TRUE);
    }
}

void GLWorld::stopanimation()
{    
    toggleAnimation(FALSE);

    if ( Globals::getRendering() ) 
    {
        Globals::setRendering(false);
    }
    
    updateTimeSlider(m_nCurrentFrame);
    updateSliderValues();

    setStopRequested(true);
}

///////////////////////////////////////////////////////////
//this is a core routine that draws a frame in the scene

void GLWorld::updateAnimation(int frame_number, bool force)
{
    static int previous_frame_number = 0;

    setAnimationUpdated(true);

    if (frame_number < 1)
    {
        frame_number = 1;
    }

    if (frame_number == previous_frame_number && !force)
    {
        // This is a hack to mask a bigger problem where updateAnimation() gets called from all
        // over the place due to cyclical dependencies.  It needs to be fixed properly FIXME 
        previous_frame_number = frame_number;
        return;
    }

    previous_frame_number = frame_number;

    LayerListEntry* layer_list_entry = getLayerList()->first();

    JahLayer* jah_layer;

    for ( ; layer_list_entry; layer_list_entry = getLayerList()->next() ) 
    {
        jah_layer = layer_list_entry->getJahLayer();

        if  ( jah_layer->layernodes->m_nodelist->count() ) 
        { 
            updatekeyNodes(jah_layer, frame_number); 
            jah_layer->layerStatus = jah_layer->layernodes->m_node->layer_enabled;
        }  

        if (jah_layer->layertype == LayerCategory::TEXT)
        {
            if ( jah_layer->textExtrudeHasChanged() )
            {
                jah_layer->updateFont = true;
            }
        }

        if (   ( (jah_layer->asset.videotex == TRUE) && jah_layer->layerStatus )
            || ( (jah_layer->keyasset.videotex == TRUE) && jah_layer->layerStatus )
            ) 
        {
            jah_layer->updateVidTex(frame_number); 
        } 
    }

    updateTimeSlider(frame_number);

    if ( !getAnimating() && !Globals::getRendering() )
    {
        updateSliderValues();
        slotUpdateListViewCheckBoxes();
    }
}

void                
GLWorld::addMultiSelectKeyNodes()
{
    LayerListEntry* layer_list_entry = getLayerList()->first();

    for(; layer_list_entry; layer_list_entry = getLayerList()->next() )
    {
        JahLayer* jah_layer = layer_list_entry->getJahLayer();

        if ( jah_layer->getCheckListItem()->isSelected() )
        {
            jah_layer->layernodes->addNodes(m_nCurrentFrame, jah_layer->layerStatus);
        }
    }
}
void                
GLWorld::removeMultiSelectKeyNodes()
{
    LayerListEntry* layer_list_entry = getLayerList()->first();

    for(; layer_list_entry; layer_list_entry = getLayerList()->next() )
    {
        if ( layer_list_entry->getJahLayer()->getCheckListItem()->isSelected() )
        {
            removekeyNodes(layer_list_entry->thelayer, m_nCurrentFrame); 
        }
    }
}

void GLWorld::addKeyframe() 
{ 
    //m_nCurrentFrame = controllerslider->value();

    if ( multipleLayersSelectedInListView() )
    {
        addMultiSelectKeyNodes();
    }
    else
    {
        getActiveJahLayer()->layernodes->addNodes(m_nCurrentFrame, getActiveJahLayer()->layerStatus);
    }

    if ( getActiveJahLayer()->getCategory() == ObjectCategory::EFFECT)
    {
        getActiveJahLayer()->getParent()->setEffectsHaveKeyFrames(true);
    }

    updateKeyframeDisplay();
}

void GLWorld::removeKeyframe() 
{ 
    //m_nCurrentFrame = controllerslider->value();

    if ( multipleLayersSelectedInListView() )
    {
        removeMultiSelectKeyNodes();
    }
    else
    {
        removekeyNodes(getActiveJahLayer(), m_nCurrentFrame);
    }

    updateKeyframeDisplay();
}

void GLWorld::prevKeyframe() 
{ 
    // theres a workaround here for the camera keyframes which are in layer 0
    int newFrame;

    //m_nCurrentFrame = controllerslider->value();
    newFrame = getActiveJahLayer()->layernodes->getPrevKeyframe(m_nCurrentFrame);

    if (newFrame > 0) 
    {
        m_nCurrentFrame = newFrame;
        updateAnimation(m_nCurrentFrame);
        updateGL(); 
        updateKeyframeDisplay();
        slotUpdateListViewCheckBoxes();
        updateTimeSlider(m_nCurrentFrame);
    }
}

void
GLWorld::slotKeyframeChanged()
{
    if ( getActiveJahLayer() )
    {
        updateKeyNodes();
        updateSliderValues();
        updateGL();
    }
}

void GLWorld::nextKeyframe() 
{ 
    // theres a workaround here for the camera keyframes which are in layer 0
    int newFrame;

    //m_nCurrentFrame = controllerslider->value();
    newFrame = getActiveJahLayer()->layernodes->getNextKeyframe(m_nCurrentFrame);

    if (newFrame > 0) 
    {
        m_nCurrentFrame = newFrame;
        updateAnimation(m_nCurrentFrame);
        updateGL(); 
        updateKeyframeDisplay();
        slotUpdateListViewCheckBoxes();
        updateTimeSlider(m_nCurrentFrame);
    }
}


void GLWorld::updateKeyframeDisplay() 
{ 
    #ifndef JAHPLAYER
  //jtrace->debug( "GLWorld::","updateKeyframeDisplay() updating keyframes");

    if (m_bHasKeyFramer==true)
    {

        if ( getActiveJahLayer() )
        {
            if (getActiveJahLayer()->layernodes->m_nodelist->count() > 0)
            {

                m_pKeyFramer->updateKeyframes(m_nMaxFrame, m_nCurrentFrame, 
                                              getActiveJahLayer()->layernodes->m_nodelist);
                
            }
            else
            {
                m_pKeyFramer->updateKeyframes( );
            }
        }
    }
    #endif
}


///////////////////////////////////////////////////////////////
//routine to check if layers are visible and should be drawn

bool GLWorld::isLayerVisible(JahLayer* jah_layer) 
{
    int theFrame = m_pControllerSlider->value();
    bool returnval = false;

    if (!jah_layer)
      {
	m_pJTrace->debug( "TODO :GLWorld::isLayerVisible null layer");
	return false;
      }

    //is it visible?
    if ( jah_layer->layerStatus && jah_layer->getIsSelectedVisible() ) 
    {
        //its visible
        returnval = true;

        //now check if bleeding is on...
        if (!jah_layer->showEdges) 
        {
            int in_frame = jah_layer->m_inframe;

            //if its not check that edges are in bounds
            if (theFrame < in_frame) 
            { 
                returnval = false; 
            } //startframe inframe

            int out_frame = jah_layer->m_outframe;

            if (theFrame > out_frame) 
            { 
                returnval = false; 
            } //endframe outframe
        }
    }

    return returnval;                            
}


void
GLWorld::slotLightColorAmbientRed()
{
    JahLayer* image_layer = getActiveJahLayer();

    if (image_layer->objtype == ObjectCategory::EFFECT)
    {
        image_layer = image_layer->getParent();
    }

    image_layer->layernodes->m_node->ambient_light_color.x = (float)getLightingSliderPtr(0)->value() / 255.0f;
    image_layer->setEffectsSlidersHaveChanged(true);
    updateGL();

    QColor qcolor(
                    getLightingSliderPtr(0)->value(),
                    getLightingSliderPtr(1)->value(),
                    getLightingSliderPtr(2)->value()
                  );

    slotLightColorAmbient(qcolor);
}

void
GLWorld::slotLightColorAmbientGreen()
{
    JahLayer* image_layer = getActiveJahLayer();

    if (image_layer->objtype == ObjectCategory::EFFECT)
    {
        image_layer = image_layer->getParent();
    }

    image_layer->layernodes->m_node->ambient_light_color.y = (float)getLightingSliderPtr(1)->value() / 255.0f;
    image_layer->setEffectsSlidersHaveChanged(true);
    updateGL();

    QColor qcolor(
                    getLightingSliderPtr(0)->value(),
                    getLightingSliderPtr(1)->value(),
                    getLightingSliderPtr(2)->value()
                  );

    slotLightColorAmbient(qcolor);
}

void
GLWorld::slotLightColorAmbientBlue()
{
    JahLayer* image_layer = getActiveJahLayer();

    if (image_layer->objtype == ObjectCategory::EFFECT)
    {
        image_layer = image_layer->getParent();
    }

    image_layer->layernodes->m_node->ambient_light_color.z = (float)getLightingSliderPtr(2)->value() / 255.0f;
    image_layer->setEffectsSlidersHaveChanged(true);
    updateGL();

    QColor qcolor(
                    getLightingSliderPtr(0)->value(),
                    getLightingSliderPtr(1)->value(),
                    getLightingSliderPtr(2)->value()
                  );

    slotLightColorAmbient(qcolor);
}

void
GLWorld::slotLightColorDiffuseRed()
{
    JahLayer* image_layer = getActiveJahLayer();

    if (image_layer->objtype == ObjectCategory::EFFECT)
    {
        image_layer = image_layer->getParent();
    }

    image_layer->layernodes->m_node->diffuse_light_color.x = (float)getLightingSliderPtr(3)->value() / 255.0f;
    image_layer->setEffectsSlidersHaveChanged(true);
    updateGL();

    QColor qcolor(
                    getLightingSliderPtr(3)->value(),
                    getLightingSliderPtr(4)->value(),
                    getLightingSliderPtr(5)->value()
                  );

    slotLightColorDiffuse(qcolor);
}

void
GLWorld::slotLightColorDiffuseGreen()
{
    JahLayer* image_layer = getActiveJahLayer();

    if (image_layer->objtype == ObjectCategory::EFFECT)
    {
        image_layer = image_layer->getParent();
    }

    image_layer->layernodes->m_node->diffuse_light_color.y = (float)getLightingSliderPtr(4)->value() / 255.0f;
    image_layer->setEffectsSlidersHaveChanged(true);
    updateGL();

    QColor qcolor(
                    getLightingSliderPtr(3)->value(),
                    getLightingSliderPtr(4)->value(),
                    getLightingSliderPtr(5)->value()
                  );

    slotLightColorDiffuse(qcolor);
}

void
GLWorld::slotLightColorDiffuseBlue()
{
    JahLayer* image_layer = getActiveJahLayer();

    if (image_layer->objtype == ObjectCategory::EFFECT)
    {
        image_layer = image_layer->getParent();
    }

    image_layer->layernodes->m_node->diffuse_light_color.z = (float)getLightingSliderPtr(5)->value() / 255.0f;
    image_layer->setEffectsSlidersHaveChanged(true);
    updateGL();

    QColor qcolor(
                    getLightingSliderPtr(3)->value(),
                    getLightingSliderPtr(4)->value(),
                    getLightingSliderPtr(5)->value()
                  );

    slotLightColorDiffuse(qcolor);
}

void
GLWorld::slotLightColorSpecularRed()
{
    JahLayer* image_layer = getActiveJahLayer();

    if (image_layer->objtype == ObjectCategory::EFFECT)
    {
        image_layer = image_layer->getParent();
    }

    image_layer->layernodes->m_node->specular_light_color.x = (float)getLightingSliderPtr(6)->value() / 255.0f;
    image_layer->setEffectsSlidersHaveChanged(true);
    updateGL();

    QColor qcolor(
                    getLightingSliderPtr(6)->value(),
                    getLightingSliderPtr(7)->value(),
                    getLightingSliderPtr(8)->value()
                  );

    slotLightColorSpecular(qcolor);
}

void
GLWorld::slotLightColorSpecularGreen()
{
    JahLayer* image_layer = getActiveJahLayer();

    if (image_layer->objtype == ObjectCategory::EFFECT)
    {
        image_layer = image_layer->getParent();
    }

    image_layer->layernodes->m_node->specular_light_color.y = (float)getLightingSliderPtr(7)->value() / 255.0f;
    image_layer->setEffectsSlidersHaveChanged(true);
    updateGL();

    QColor qcolor(
                    getLightingSliderPtr(6)->value(),
                    getLightingSliderPtr(7)->value(),
                    getLightingSliderPtr(8)->value()
                  );

    slotLightColorSpecular(qcolor);
}

void
GLWorld::slotLightColorSpecularBlue()
{
    JahLayer* image_layer = getActiveJahLayer();

    if (image_layer->objtype == ObjectCategory::EFFECT)
    {
        image_layer = image_layer->getParent();
    }

    image_layer->layernodes->m_node->specular_light_color.z = (float)getLightingSliderPtr(8)->value() / 255.0f;
    image_layer->setEffectsSlidersHaveChanged(true);
    updateGL();

    QColor qcolor(
                    getLightingSliderPtr(6)->value(),
                    getLightingSliderPtr(7)->value(),
                    getLightingSliderPtr(8)->value()
                  );

    slotLightColorSpecular(qcolor);
}

void
GLWorld::slotLightPosition()
{
}













