/*******************************************************************************
**
** The source file for the Jahshaka effect module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "keyer.h"


void GLKeyer::updateSliders()
{   
    //update the rest of the slider/ui data
    updateSliderValues();
    slotUpdateListViewCheckBoxes();

    updateUiOptions();

    updateUiSettings();

}

void GLKeyer::updateUiOptions()
{
}

void GLKeyer::updateUiSettings()
{
}

//we use this to disconnect ui form the core
void GLKeyer::changeVideoRender()
{
	bool status = VideoRenderSelect->isChecked();
	SetVideoRender(status);
}
