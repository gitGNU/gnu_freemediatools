/*******************************************************************************
**
** The source file for the Jahshaka effect module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "color.h"
#include <qslider.h>
#include <qlistview.h>
#include "coreobject.h"

void GLColorize::updateSliders()
{   
    //update the rest of the slider/ui data
    updateSliderValues();

    updateUiOptions();

    updateUiSettings();
}

//we override this here since the 'world' is now layer 1

void GLColorize::updateSliderValues()
{
    motionNode* motion_node = getActiveJahLayer()->layernodes->m_node;

    m_color_corrector_widget->redSlider->setValue( int( motion_node->rx ) );
    m_color_corrector_widget->greenSlider->setValue( int( motion_node->ry ) );
    m_color_corrector_widget->blueSlider->setValue( int( motion_node->rz ) );
    m_color_corrector_widget->tintSlider->setValue( int( motion_node->tx ) );
    m_color_corrector_widget->brightnessSlider->setValue( int( motion_node->ty ) );
    m_color_corrector_widget->strengthSlider->setValue( int( motion_node->tz ) );
    m_color_corrector_widget->hSlider->setValue( getScaleValue(motion_node->sx) );
    m_color_corrector_widget->sSlider->setValue( getScaleValue(motion_node->sy) );
    m_color_corrector_widget->vSlider->setValue( getScaleValue(motion_node->sz) );
    m_color_corrector_widget->gammaSlider->setValue( int( motion_node->Alpha ) );
}

void GLColorize::updateUiOptions()
{
}

void GLColorize::updateUiSettings()
{
}

//we use this to disconnect ui form the core
void GLColorize::changeVideoRender()
{
	bool status = VideoRenderSelect->isChecked();
	SetVideoRender(status);
}

