/*******************************************************************************
**
** The source file for the Jahshaka effect module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "tracker.h"
#include <qslider.h>

void 
GLTracker::updateSliders(motionNode* keyframe)
{   
    m_x_position_slider->setValue(int( keyframe->rx ));
    m_y_position_slider->setValue(int( keyframe->ry ));
    m_point_radius_slider->setValue(int( keyframe->rz ));
    m_point_search_radius_slider->setValue(int( keyframe->tx ));
    m_point_sensitivity_slider->setValue(int( keyframe->ty ));
    m_point_checksum_tolerance_slider->setValue(int( keyframe->tz ));

    updateUiOptions();
    updateUiSettings();
}

void GLTracker::updateUiOptions()
{
    hideOptions(m_nNumUiOptions-1);
}

void GLTracker::updateUiSettings()
{
    m_aJahModuleOptions[0]->setChecked(getActiveJahLayer()->option1);
    m_aJahModuleOptions[1]->setChecked(getActiveJahLayer()->option2);
    m_aJahModuleOptions[2]->setChecked(getActiveJahLayer()->option3);
    m_aJahModuleOptions[3]->setChecked(getActiveJahLayer()->option4);
}


void GLTracker::setOption1(void)
{
    getActiveJahLayer()->option1 = m_aJahModuleOptions[0]->isChecked();
    updateEffects();
}

void GLTracker::setOption2(void)
{
    getActiveJahLayer()->option2 = m_aJahModuleOptions[1]->isChecked();
    updateEffects();
}

void GLTracker::setOption3(void)
{
    getActiveJahLayer()->option3 = m_aJahModuleOptions[2]->isChecked();
    updateEffects();
}

void GLTracker::setOption4(void)
{
    getActiveJahLayer()->option4 = m_aJahModuleOptions[3]->isChecked();
    updateEffects();
}


