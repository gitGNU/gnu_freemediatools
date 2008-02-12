/*******************************************************************************
**
** The source file for the Jahshaka glworldcontrols.cpp file
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "glworld.h"
#include "glworldlists.h"
#include <timelineSlider.h>
#include <qlistview.h>
#include <InputLCD.h>

////////////////////////////////////////////
// layer controls

void
GLWorld::resetAllSliders()
{
    motionNode* sliders = getActiveJahLayer()->layernodes->m_node;
    sliders->rx = 0.0f;
    sliders->ry = 0.0f;
    sliders->rz = 0.0f;
    sliders->sx = 1.0f;
    sliders->sy = 1.0f;
    sliders->sz = 1.0f;
    sliders->tx = 0.0f;
    sliders->ty = 0.0f;
    sliders->tz = 0.0f;
    sliders->Alpha = 100.0f;

    updateSliders();
}

//active layer controls here
void GLWorld::setXRotation( int degrees )
{ 
    static int lastdegree;

    if ( !getActiveJahLayer() )
    {
        return;
    }

    if (getActiveJahLayer() == m_pCamera)
    {
        m_pCamera->layernodes->m_node->rx = (GLfloat)(degrees);
        getActiveJahLayer()->layernodes->m_node->rx = (GLfloat)(degrees);
    }
    else
    {
        if ( multipleLayersSelectedInListView() )
        {
            LayerListEntry* layer_list_entry = getLayerList()->first();

            for(; layer_list_entry; layer_list_entry = getLayerList()->next() )
            {
                JahLayer* jah_layer = layer_list_entry->getJahLayer();

                if ( jah_layer->getCheckListItem() && jah_layer->getCheckListItem()->isSelected() )
                {
                    if (degrees > lastdegree)
                    {
                        jah_layer->layernodes->m_node->rx += 5.0f;
                    }
                    else if (degrees < lastdegree) 
                    {
                        jah_layer->layernodes->m_node->rx -= 5.0f;
                    }
                }
            }
        }
        else
        {
            getActiveJahLayer()->layernodes->m_node->rx = (GLfloat)(degrees );
        }

        lastdegree = degrees;
    }

    if ( getUseAutoKey() )
    {
        motionNode* keyframe = getActiveJahLayer()->findKeyframeAtTime( getCurrentFrame() );

        if (keyframe)
        {
            keyframe->rx = getActiveJahLayer()->layernodes->m_node->rx;
            updateKeyframeDisplay();
        }
    }

    if ( getActiveJahLayer()->drawtheKey && !getActiveJahLayer()->getUsePbuffer() )
    {
        motionNode* slider = getActiveJahLayer()->layernodes->m_node;
        slider->rx = jahstd::Min<float>(slider->rx, 90.0f);
        slider->rx = jahstd::Max<float>(slider->rx, -90.0f);
    }

    updateEffects();

    setDontTrack(true);
    updateGL();
    setDontTrack(false);
}

void GLWorld::setYRotation( int degrees )
{
    static int lastdegree;

    if ( !getActiveJahLayer() )
    {
        return;
    }

    if (getActiveJahLayer() == m_pCamera)
    {
        m_pCamera->layernodes->m_node->ry = (GLfloat)(degrees);
        getActiveJahLayer()->layernodes->m_node->ry = (GLfloat)(degrees);
    }
    else
    {
        if ( multipleLayersSelectedInListView() )
        {
            LayerListEntry* layer_list_entry = getLayerList()->first();

            for(; layer_list_entry; layer_list_entry = getLayerList()->next() )
            {
                JahLayer* jah_layer = layer_list_entry->getJahLayer();

                if ( jah_layer->getCheckListItem() && jah_layer->getCheckListItem()->isSelected() )
                {
                    if (degrees > lastdegree)
                    {
                        jah_layer->layernodes->m_node->ry += 5.0f;
                    }
                    else if (degrees < lastdegree) 
                    {
                        jah_layer->layernodes->m_node->ry -= 5.0f;
                    }
                }
            }
        }
        else
        {
			getActiveJahLayer()->layernodes->m_node->ry = (GLfloat)(degrees );
        }

        lastdegree = degrees;
    }

    if ( getUseAutoKey() )
    {
        motionNode* keyframe = getActiveJahLayer()->findKeyframeAtTime( getCurrentFrame() );

        if (keyframe)
        {
            keyframe->ry = getActiveJahLayer()->layernodes->m_node->ry;
            updateKeyframeDisplay();
        }
    }

    if ( getActiveJahLayer()->drawtheKey && !getActiveJahLayer()->getUsePbuffer() )
    {
        motionNode* slider = getActiveJahLayer()->layernodes->m_node;
        slider->ry = jahstd::Min<float>(slider->ry, 90.0f);
        slider->ry = jahstd::Max<float>(slider->ry, -90.0f);
    }

    updateEffects();

    setDontTrack(true);
    updateGL();
    setDontTrack(false);
}

void GLWorld::setZRotation( int degrees )
{
    static int lastdegree;

    if ( !getActiveJahLayer() )
    {
        return;
    }

    if (getActiveJahLayer() == m_pCamera)
    {
        m_pCamera->layernodes->m_node->rz = (GLfloat)(degrees);
        getActiveJahLayer()->layernodes->m_node->rz = (GLfloat)(degrees);
    }
    else
    {
        if ( multipleLayersSelectedInListView() )
        {
            LayerListEntry* layer_list_entry = getLayerList()->first();

            for(; layer_list_entry; layer_list_entry = getLayerList()->next() )
            {
                JahLayer* jah_layer = layer_list_entry->getJahLayer();

                if ( jah_layer->getCheckListItem() && jah_layer->getCheckListItem()->isSelected() )
                {
                    if (degrees > lastdegree)
                    {
                        jah_layer->layernodes->m_node->rz += 5.0f;
                    }
                    else if (degrees < lastdegree) 
                    {
                        jah_layer->layernodes->m_node->rz -= 5.0f;
                    }
                }
            }
        }
        else
        {
			getActiveJahLayer()->layernodes->m_node->rz = (GLfloat)(degrees );
        }

        lastdegree = degrees;
    }

    if ( getUseAutoKey() )
    {
        motionNode* keyframe = getActiveJahLayer()->findKeyframeAtTime( getCurrentFrame() );

        if (keyframe)
        {
            keyframe->rz = getActiveJahLayer()->layernodes->m_node->rz;
            updateKeyframeDisplay();
        }
    }

    updateEffects();

    setDontTrack(true);
    updateGL();
    setDontTrack(false);
}



void GLWorld::setXTrans   ( int degrees )
{
    static int lastdegree;

    if ( !getActiveJahLayer() )
    {
        return;
    }

    if (getActiveJahLayer() == m_pCamera)
    {
        m_pCamera->layernodes->m_node->tx = (GLfloat)(degrees);
        getActiveJahLayer()->layernodes->m_node->tx = (GLfloat)(degrees);
    }
    else
    {

        if ( multipleLayersSelectedInListView() )
        {
            LayerListEntry* layer_list_entry = getLayerList()->first();

            for(; layer_list_entry; layer_list_entry = getLayerList()->next() )
            {
                JahLayer* jah_layer = layer_list_entry->getJahLayer();

                if ( jah_layer->getCheckListItem() && jah_layer->getCheckListItem()->isSelected() )
                {
                    if (degrees > lastdegree)
                    {
                        jah_layer->layernodes->m_node->tx += 5.0f;
                    }
                    else if (degrees < lastdegree) 
                    {
                        jah_layer->layernodes->m_node->tx -= 5.0f;
                    }
                }
            }
        }
        else
        {
        getActiveJahLayer()->layernodes->m_node->tx = (GLfloat)(degrees );
        }

        lastdegree = degrees;
    }

    if ( getUseAutoKey() )
    {
        motionNode* keyframe = getActiveJahLayer()->findKeyframeAtTime( getCurrentFrame() );

        if (keyframe)
        {
            keyframe->tx = getActiveJahLayer()->layernodes->m_node->tx;
            updateKeyframeDisplay();
        }
    }

    updateEffects();

    setDontTrack(true);
    updateGL();
    setDontTrack(false);

}
void GLWorld::setYTrans   ( int degrees )
{
    static int lastdegree;

    if ( !getActiveJahLayer() )
    {
        return;
    }

    motionNode* keyframe = getActiveJahLayer()->findKeyframeAtTime( getCurrentFrame() );

    if (keyframe)
    {
        keyframe->ty = (GLfloat)degrees;
        updateKeyframeDisplay();
    }

    if (getActiveJahLayer() == m_pCamera)
    {
        m_pCamera->layernodes->m_node->ty = (GLfloat)(degrees);
        getActiveJahLayer()->layernodes->m_node->ty = (GLfloat)(degrees);
    }
    else
    {

        if ( multipleLayersSelectedInListView() )
        {
            LayerListEntry* layer_list_entry = getLayerList()->first();

            for(; layer_list_entry; layer_list_entry = getLayerList()->next() )
            {
                JahLayer* jah_layer = layer_list_entry->getJahLayer();

                if ( jah_layer->getCheckListItem() && jah_layer->getCheckListItem()->isSelected() )
                {
                    if (degrees > lastdegree)
                    {
                        jah_layer->layernodes->m_node->ty += 5.0f;
                    }
                    else if (degrees < lastdegree) 
                    {
                        jah_layer->layernodes->m_node->ty -= 5.0f;
                    }
                }
            }
        }
        else
        {
			getActiveJahLayer()->layernodes->m_node->ty = (GLfloat)(degrees );
        }

        lastdegree = degrees;
    }

    if ( getUseAutoKey() )
    {
        motionNode* keyframe = getActiveJahLayer()->findKeyframeAtTime( getCurrentFrame() );

        if (keyframe)
        {
            keyframe->ty = getActiveJahLayer()->layernodes->m_node->ty;
            updateKeyframeDisplay();
        }
    }

    updateEffects();

    setDontTrack(true);
    updateGL();
    setDontTrack(false);
}
void GLWorld::setZTrans   ( int degrees )
{
    static int lastdegree;

    if ( !getActiveJahLayer() )
    {
        return;
    }

    if (getActiveJahLayer() == m_pCamera)
    {
        m_pCamera->layernodes->m_node->tz = (GLfloat)(degrees);
        getActiveJahLayer()->layernodes->m_node->tz = (GLfloat)(degrees);
    }
    else
    {

        if ( multipleLayersSelectedInListView() )
        {
            LayerListEntry* layer_list_entry = getLayerList()->first();

            for(; layer_list_entry; layer_list_entry = getLayerList()->next() )
            {
                JahLayer* jah_layer = layer_list_entry->getJahLayer();

                if ( jah_layer->getCheckListItem() && jah_layer->getCheckListItem()->isSelected() )
                {
                    if (degrees > lastdegree)
                    {
                        jah_layer->layernodes->m_node->tz += 5.0f;
                    }
                    else if (degrees < lastdegree) 
                    {
                        jah_layer->layernodes->m_node->tz -= 5.0f;
                    }
                }
            }
        }
        else
        {
			getActiveJahLayer()->layernodes->m_node->tz = (GLfloat)(degrees );
        }

        lastdegree = degrees;
    }

    if ( getUseAutoKey() )
    {
        motionNode* keyframe = getActiveJahLayer()->findKeyframeAtTime( getCurrentFrame() );

        if (keyframe)
        {
            keyframe->tz = getActiveJahLayer()->layernodes->m_node->tz;
            updateKeyframeDisplay();
        }
    }

    updateEffects();

    setDontTrack(true);
    updateGL();
    setDontTrack(false);
}

//default scale is 1 so things are different here
//as a value of 0 is no scaling...
float GLWorld::setScaleValue   ( int degree )
{
    float degrees = degree;

    GLfloat thescale;

    if (degree==0)
    {
        thescale=1;
    }
    else
    {
        thescale = 1.0 + (GLfloat)(degrees/100);
    }

    return thescale;

}

int GLWorld::getScaleValue   ( float degree )
{
    int integer_value;
    float float_value;

    if (degree >= 1.0f)
    {
        float_value = (degree - 0.999f) * 100.0f;
    }
    else
    {
        float_value = (degree - 1.001f) * 100.0f;
    }

    integer_value = int(float_value);
    return integer_value;
}

void GLWorld::setXScale   ( int degrees )
{

    static int lastdegree;

    if ( !getActiveJahLayer() )
    {
        return;
    }

    if (getActiveJahLayer() == m_pCamera)
    {
        //m_pCamera->m_node->tz = (GLfloat)(degrees);
    }
    else
    {
        if ( multipleLayersSelectedInListView() )
        {
            LayerListEntry* layer_list_entry = getLayerList()->first();

            for(; layer_list_entry; layer_list_entry = getLayerList()->next() )
            {
                JahLayer* jah_layer = layer_list_entry->getJahLayer();

                if ( jah_layer->getCheckListItem() && jah_layer->getCheckListItem()->isSelected() )
                {
                    if (degrees > lastdegree)
                    {
                        jah_layer->layernodes->m_node->sx += 0.1f;
                    }
                    else if (degrees < lastdegree) 
                    {
                        jah_layer->layernodes->m_node->sx += 0.1f;
                    }
                }
            }
        }
        else
        {
            getActiveJahLayer()->layernodes->m_node->sx = setScaleValue(degrees);
        }

        lastdegree = degrees;
    }

    if ( getUseAutoKey() )
    {
        motionNode* keyframe = getActiveJahLayer()->findKeyframeAtTime( getCurrentFrame() );

        if (keyframe)
        {
            keyframe->sx = getActiveJahLayer()->layernodes->m_node->sx;
            updateKeyframeDisplay();
        }
    }

    updateEffects();

    setDontTrack(true);
    updateGL();
    setDontTrack(false);
}

void GLWorld::setYScale   ( int degrees )
{

    static int lastdegree;

    if ( !getActiveJahLayer() )
    {
        return;
    }

    if (getActiveJahLayer() == m_pCamera)
    {
        //m_pCamera->m_node->tz = (GLfloat)(degrees);
    }
    else
    {

        if ( multipleLayersSelectedInListView() )
        {
            LayerListEntry* layer_list_entry = getLayerList()->first();

            for(; layer_list_entry; layer_list_entry = getLayerList()->next() )
            {
                JahLayer* jah_layer = layer_list_entry->getJahLayer();

                if ( jah_layer->getCheckListItem() && jah_layer->getCheckListItem()->isSelected() )
                {
                    if (degrees > lastdegree)
                    {
                        jah_layer->layernodes->m_node->sy += 0.1f;
                    }
                    else if (degrees < lastdegree) 
                    {
                        jah_layer->layernodes->m_node->sy += 0.1f;
                    }
                }
            }
        }
        else
        {
            getActiveJahLayer()->layernodes->m_node->sy = setScaleValue(degrees);
        }

        lastdegree = degrees;
    }

    if ( getUseAutoKey() )
    {
        motionNode* keyframe = getActiveJahLayer()->findKeyframeAtTime( getCurrentFrame() );

        if (keyframe)
        {
            keyframe->sy = getActiveJahLayer()->layernodes->m_node->sy;
            updateKeyframeDisplay();
        }
    }

    updateEffects();

    setDontTrack(true);
    updateGL();
    setDontTrack(false);
}

void GLWorld::setZScale   ( int degrees )
{
    static int lastdegree;

    if ( !getActiveJahLayer() )
    {
        return;
    }

    if (getActiveJahLayer() == m_pCamera)
    {
        //m_pCamera->m_node->tz = (GLfloat)(degrees);
    }
    else
    {
        if ( multipleLayersSelectedInListView() )
        {
            LayerListEntry* layer_list_entry = getLayerList()->first();

            for(; layer_list_entry; layer_list_entry = getLayerList()->next() )
            {
                JahLayer* jah_layer = layer_list_entry->getJahLayer();

                if ( jah_layer->getCheckListItem() && jah_layer->getCheckListItem()->isSelected() )
                {
                    if (degrees > lastdegree)
                    {
                        jah_layer->layernodes->m_node->sz += 0.1f;
                    }
                    else if (degrees < lastdegree) 
                    {
                        jah_layer->layernodes->m_node->sz += 0.1f;
                    }
                }
            }
        }
        else
        {
            getActiveJahLayer()->layernodes->m_node->sz = setScaleValue(degrees);
        }

        lastdegree = degrees;
    }

    if ( getUseAutoKey() )
    {
        motionNode* keyframe = getActiveJahLayer()->findKeyframeAtTime( getCurrentFrame() );

        if (keyframe)
        {
            keyframe->sz = getActiveJahLayer()->layernodes->m_node->sz;
            updateKeyframeDisplay();
        }
    }

    updateEffects();

    setDontTrack(true);
    updateGL();
    setDontTrack(false);


}

void GLWorld::setAlpha    ( int degrees )  
{ 
	JahLayer* current_jah_layer = getActiveJahLayer();

    if (!current_jah_layer)
    {
        return;
    }
 
    motionNode* keyframe = getActiveJahLayer()->findKeyframeAtTime( getCurrentFrame() );

    if (keyframe)
    {
        keyframe->Alpha = (GLfloat)degrees;
        updateKeyframeDisplay();
    }

    current_jah_layer->layernodes->m_node->Alpha = (GLfloat)(degrees);

    updateEffects();

    setDontTrack(true);
    updateGL();
    setDontTrack(false);
}

void GLWorld::setZoom     ( int degrees )  
{ 
    m_pCamera->layernodes->m_node->sc = (GLfloat)(degrees); updateGL();   
}


void GLWorld::setExtrude  ( int depth )  
{

    if (depth < 0)
    {
        depth = 0;
    }

    getActiveJahLayer()->layernodes->m_node->extrude = float(depth);
    getActiveJahLayer()->updateFont = true;

    updateGL(); 
}

void GLWorld::setKeyframeDrawStatus()
{

    if (getActiveJahLayer() != m_pCamera) 
    {
        getActiveJahLayer()->drawKeyframes = !getActiveJahLayer()->drawKeyframes;
    }

    updateGL(); 
}

void
GLWorld::slotSetLayerSelectedVisible()
{
}

void GLWorld::setColor() 
{
    QColor newcolor;

    GLfloat redcolor, greencolor, bluecolor;

    QColor c = QColorDialog::getColor( newcolor, 0, 0 );

    QRgb thecolor=c.rgb();

    int r=qRed(thecolor);   
    if (r==0)  redcolor = 0.0;    else {  redcolor   = (r/255.0); }

    int g=qGreen(thecolor); 
    if (g==0)  greencolor = 0.0;  else {  greencolor = (g/255.0);  }

    int b=qBlue(thecolor);  
    if (b==0)  bluecolor  = 0.0;  else {  bluecolor  = (b/255.0);  }

    getActiveJahLayer()->Red = redcolor;
    getActiveJahLayer()->Blue = bluecolor;
    getActiveJahLayer()->Green = greencolor;

}


////////////////////////////////////////////
// layer video controls

//we need to add inframe and outframe or startslipframe and endslipframe

//if forceplay we update visuals here as well

void
GLWorld::slotSetSlipFrames(int offset)
{
    JahLayer* jah_layer = getActiveJahLayer();

    if (jah_layer)
    {
        jah_layer->setSlipFrameOffset(offset);
        updateAnimation( m_pControllerSlider->value(), true ); 
        updateGL(); 
    }

    jah_layer->setInframeValid(true);

    if ( getLockKeyAndClip() )
    {
        slotSetKeySlipFrames(offset);
    }
}

void
GLWorld::slotSetKeySlipFrames(int offset)
{
    JahLayer* jah_layer = getActiveJahLayer();

    if (jah_layer)
    {
        jah_layer->setKeySlipFrameOffset(offset);
        updateAnimation( m_pControllerSlider->value(), true ); 
        updateGL(); 
    }

    jah_layer->setInframeValid(true);
    m_pKeySlipFramesLCD->setValue(offset);
}

void GLWorld::setinFrames(int frame)   
{
    JahLayer* jah_layer = getActiveJahLayer();

    if (!jah_layer)
    {
        return;
    }

    if (frame <= jah_layer->m_outframe && frame >= 1)
    {
        jah_layer->m_inframe = frame;
    }
    else if (frame < 1)
    {
        jah_layer->m_inframe = 1;
        m_pInFramesLCD->setValue(1);
    }
    else
    {
        // Restore the previous value of the lcd
        m_pInFramesLCD->setValue( jah_layer->m_inframe );
    }

    //is it possible to only update the layer and not the entire scene? no
    if (m_bForcePlay) 
    { 
        updateAnimation( m_pControllerSlider->value(), true ); 
        updateGL();  
    } 
    else 
    { 
        updateGL();  
    }

    //updateSliders();
    updateSliderValues();
    slotUpdateListViewCheckBoxes();
    jah_layer->setInframeValid(true);

    if ( getLockKeyAndClip() )
    {
        setinKeyFrames(frame); 
        m_pInFramesLCD->setValue(jah_layer->m_inframe);
    }
}

void GLWorld::setoutFrames(int frame)   
{
    JahLayer* jah_layer = getActiveJahLayer();

    if (!jah_layer)
    {
        return;
    }

    if ( frame >= jah_layer->m_inframe && frame < jah_layer->asset.getNumFrames() )
    {
        jah_layer->m_outframe = frame;
    }
    else if ( frame > jah_layer->asset.getNumFrames() )
    {
        jah_layer->m_outframe = jah_layer->asset.getNumFrames();
        m_pOutFramesLCD->setValue(jah_layer->m_outframe);
    }
    else
    {
        // Restore the previous value of the lcd
        m_pOutFramesLCD->setValue( jah_layer->m_outframe );
    }

    if (m_bForcePlay) 
    { 
        updateAnimation( m_pControllerSlider->value(), true); 
        updateGL();
    } 
    else 
    { 
        updateGL(); 
    }

    //updateSliders();
    updateSliderValues();
    slotUpdateListViewCheckBoxes();
    jah_layer->setInframeValid(true);

    if ( getLockKeyAndClip() )
    {
        setoutKeyFrames(frame);
    }
}


///////////////////////////////////////
//change to key in and key out as well

void GLWorld::setinKeyFrames( int frame )   
{
    JahLayer* jah_layer = getActiveJahLayer();

    if (!jah_layer)
    {
        return;
    }

    if (frame <= jah_layer->m_keyoutframe && frame >= 1)
    {
        jah_layer->m_keyinframe = frame;
    }
    else if (frame < 1)
    {
        jah_layer->m_keyinframe = 1;
        m_pKeyInFramesLCD->setValue(1);
    }
    else
    {
        // Restore the previous value of the lcd
        m_pKeyInFramesLCD->setValue( jah_layer->m_keyinframe );
    }

    if (m_bForcePlay) 
    {
        updateAnimation( m_pControllerSlider->value(), true ); 
        updateGL();  
    }
    else 
    {
        updateGL();
    }

    //updateSliders();
    updateSliderValues();
    slotUpdateListViewCheckBoxes();
    jah_layer->setInframeValid(true);
    m_pKeyInFramesLCD->setValue(jah_layer->m_keyinframe);
}

void GLWorld::setoutKeyFrames      ( int frame )   
{
    JahLayer* jah_layer = getActiveJahLayer();

    if (!jah_layer)
    {
        return;
    }

    if ( frame >= jah_layer->m_keyinframe && frame < jah_layer->keyasset.getNumFrames() )
    {
        jah_layer->m_keyoutframe = frame;
    }
    else if ( frame > jah_layer->keyasset.getNumFrames() )
    {
        jah_layer->m_keyoutframe = jah_layer->keyasset.getNumFrames();
        m_pKeyOutFramesLCD->setValue(jah_layer->m_keyoutframe);
    }
    else
    {
        // Restore the previous value of the lcd
        m_pKeyOutFramesLCD->setValue( jah_layer->m_keyoutframe );
    }

    if (m_bForcePlay) 
    { 
        updateAnimation( m_pControllerSlider->value(), true); 
    } 
    else 
    { 
        updateGL(); 
    }

    //updateSliders();
    updateSliderValues();
    slotUpdateListViewCheckBoxes();
    jah_layer->setInframeValid(true);
    m_pKeyOutFramesLCD->setValue(jah_layer->m_keyoutframe);
}



///////////////////////////////////////////////////////
// update lights

void GLWorld::updateLights( void) 
{
    JahLayer* thelayer = getActiveJahLayer();

    switch ( thelayer->lightNumber ) 
	{ 
        case 1     :  {  
                        #ifdef GL_LIGHT1
                        glLightfv(GL_LIGHT1, GL_DIFFUSE, thelayer->light_diffuse);
                        glLightfv(GL_LIGHT1, GL_AMBIENT, thelayer->light_ambient);
                        glLightfv(GL_LIGHT1, GL_SPECULAR, thelayer->light_specular);
                        #endif
                        break; 
                      }
        case 2     :  {  
                        #ifdef GL_LIGHT2
                        glLightfv(GL_LIGHT2, GL_DIFFUSE, thelayer->light_diffuse);
                        glLightfv(GL_LIGHT2, GL_AMBIENT, thelayer->light_ambient);
                        glLightfv(GL_LIGHT2, GL_SPECULAR, thelayer->light_specular);
                        #endif
                        break; 
                      }
        case 3     :  {  
                        #ifdef GL_LIGHT3
                        glLightfv(GL_LIGHT3, GL_DIFFUSE, thelayer->light_diffuse);
                        glLightfv(GL_LIGHT3, GL_AMBIENT, thelayer->light_ambient);
                        glLightfv(GL_LIGHT3, GL_SPECULAR, thelayer->light_specular);
                        #endif
                        break; 
                      }
        case 4     :  {  
                        #ifdef GL_LIGHT4
                        glLightfv(GL_LIGHT4, GL_DIFFUSE, thelayer->light_diffuse);
                        glLightfv(GL_LIGHT4, GL_AMBIENT, thelayer->light_ambient);
                        glLightfv(GL_LIGHT4, GL_SPECULAR, thelayer->light_specular);
                        #endif
                        break; 
                      }
        case 5     :  {  
                        #ifdef GL_LIGHT5
                        glLightfv(GL_LIGHT5, GL_DIFFUSE, thelayer->light_diffuse);
                        glLightfv(GL_LIGHT5, GL_AMBIENT, thelayer->light_ambient);
                        glLightfv(GL_LIGHT5, GL_SPECULAR, thelayer->light_specular);
                        #endif
                        break; 
                      }
        case 6     :  { 
                        #ifdef GL_LIGHT6
                        glLightfv(GL_LIGHT6, GL_DIFFUSE, thelayer->light_diffuse);
                        glLightfv(GL_LIGHT6, GL_AMBIENT, thelayer->light_ambient);
                        glLightfv(GL_LIGHT6, GL_SPECULAR, thelayer->light_specular);
                        #endif
                        break; 
                      }
        case 7     :  {  
                        #ifdef GL_LIGHT7
                        glLightfv(GL_LIGHT7, GL_DIFFUSE, thelayer->light_diffuse);
                        glLightfv(GL_LIGHT7, GL_AMBIENT, thelayer->light_ambient);
                        glLightfv(GL_LIGHT7, GL_SPECULAR, thelayer->light_specular);
                        #endif
                        break; 
                      }
        default    :  {  
                        break; 
                      }

    }


}






