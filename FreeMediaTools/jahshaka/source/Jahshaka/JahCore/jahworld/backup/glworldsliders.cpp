/*******************************************************************************
**
** The source code for the Jahshaka glworldsliders.cpp file
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include <qlabel.h>
#include <qslider.h>
#include <qcheckbox.h>
#include "glworld.h"
#include "coreobject.h"
#include <qlistview.h>

#include <InputLCD.h>

//////////////////////////////////////////////////////
//first the headings group headings
void				
GLWorld::setDefaultAxisSliderLabels()
{
    m_aJahSliderLabels[0]->setText("X Rotation");
    m_aJahSliderLabels[1]->setText("Y Rotation");
    m_aJahSliderLabels[2]->setText("Z Rotation");

    m_aJahSliderLabels[3]->setText("X Translation");
    m_aJahSliderLabels[4]->setText("Y Translation");
    m_aJahSliderLabels[5]->setText("Z Translation");

    m_aJahSliderLabels[6]->setText("X Scale");
    m_aJahSliderLabels[7]->setText("Y Scale");
    m_aJahSliderLabels[8]->setText("Z Scale");

    m_aJahSliderLabels[9]->setText("Transparency");
}

void 
GLWorld::hideAllHeadings( )
{
    //for (int i=0; i<=2; i++)
    //{
    //    JahModuleLabel[i]->hide();
    //}

}

void GLWorld::showAllHeadings( )
{
    //for (int i=0; i<=2; i++)
    //{
    //    JahModuleLabel[i]->show();
    //}
}

void GLWorld::showHeadings(int /* ns */ )
{
    //for (int i=0; i<ns; i++)
    //{
    //    JahModuleLabel[i]->show();
    //}
}


void GLWorld::setHeadingValue(int /* heading */, QString /* value */ )
{
    //JahModuleLabel[heading]->setText( tr( value  ) );
/*
    if (hasHeadings)
    {
        JahModuleLabel[heading]->setValue(value);
    }
*/
}

void GLWorld::setDefaultHeadings(void)
{
    //JahModuleLabel[0]->setText( tr( JahModuleLabelText[0]  ) );
    //JahModuleLabel[1]->setText( tr( JahModuleLabelText[1]  ) );
    //JahModuleLabel[2]->setText( tr( JahModuleLabelText[2]  ) );

/*
    if (hasHeadings)
    {
        JahModuleLabel[heading]->setValue(value);
    }
*/
}


//////////////////////////////////////////////////////
//then the sliders and their labels

void GLWorld::hideAllSliders( )
{
    for (int i=0; i<=9; i++)
    {
        JahSliders[i]->hide();
        JahSlidersLCD[i]->hide();
        m_aJahSliderLabels[i]->hide();
    }

}

void GLWorld::hideSliders( int ns )
{
	//makes for a logical call when considering number of sliders
	int hs = ns -1;

    for (int i = 0; i <= hs; i++)
    {
        showSlider(i, false);
    }

}

void GLWorld::showAllSliders( )
{
    for (int i = 0; i <= 9; i++)
    {
        showSlider(i, true);
    }
}

void 
GLWorld::showSlider(int slider_number, bool flag)
{
    if (flag)
    {
        JahSliders[slider_number]->show();
		#ifndef NEW_SLIDERS
        JahSlidersLCD[slider_number]->show();
		#endif
		m_aJahSliderLabels[slider_number]->show();
    }
    else
    {
        JahSliders[slider_number]->hide();
		#ifndef NEW_SLIDERS
        JahSlidersLCD[slider_number]->hide();
		#endif
		m_aJahSliderLabels[slider_number]->hide();
    }

}

void GLWorld::showSliders(int ns )
{
	int hs = ns -1;

    for (int i=0; i <= hs; i++)
    {
        showSlider(i, true);
    }
}

void GLWorld::setSliderValue(int slider, int value)
{
    if (hasSliders)
    {
        int min_value = JahSliders[slider]->minValue();
        int max_value = JahSliders[slider]->maxValue();

        // The lighting sliders 9-11 duplicate sliders 3-5, so their min/max values
        // feed back and need to be updated
        int lighting_slider_number = slider + 6;

        // This is a hack to keep the slider from being forced back into it's min/max range
        if (value < min_value)
        {
            JahSliders[slider]->setMinValue(value);

            if ( (slider > 2) && (slider < 6) && getLightingSliderPtr(lighting_slider_number) )
            {
                getLightingSliderPtr(lighting_slider_number)->setMinValue(value);
            }
        }
        else if (value > max_value)
        {
            JahSliders[slider]->setMaxValue(value);

            if ( (slider > 2) && (slider < 6) && getLightingSliderPtr(lighting_slider_number) )
            {
                getLightingSliderPtr(lighting_slider_number)->setMaxValue(value);
            }
        }
        else if (slider < 9 && value <= 500 && value >= -500)
        {
            JahSliders[slider]->setMinValue(-500);
            JahSliders[slider]->setMaxValue(500);

             if ( (slider > 2) && (slider < 6) && getLightingSliderPtr(slider) )
            {
                getLightingSliderPtr(lighting_slider_number)->setMinValue(-500);
                getLightingSliderPtr(lighting_slider_number)->setMaxValue(500);
            }
       }
        else if (slider == 9 && value <= 100 && value >= 0)
        {
            JahSliders[slider]->setMinValue(0);
        }

        JahSliders[slider]->setValue(value);

		#ifndef NEW_SLIDERS
        JahSlidersLCD[slider]->setValue(value);
		#endif
	}
}

void GLWorld::setSliderLabelValue(int slider, QString value)
{
   
    if (hasSliders)
    {
        //debug("setting slider label value");
        //debug(value);

        m_aJahSliderLabels[slider]->setText( tr( value) );
    }
}

void GLWorld::setDefaultSliderText(void)
{
    //we can make this a loop

    m_aJahSliderLabels[0]->setText( tr( m_aJahSliderLabelsText[0]  ) );
    m_aJahSliderLabels[1]->setText( tr( m_aJahSliderLabelsText[1]  ) );
    m_aJahSliderLabels[2]->setText( tr( m_aJahSliderLabelsText[2]  ) );
    m_aJahSliderLabels[3]->setText( tr( m_aJahSliderLabelsText[3]  ) );
    m_aJahSliderLabels[4]->setText( tr( m_aJahSliderLabelsText[4]  ) );
    m_aJahSliderLabels[5]->setText( tr( m_aJahSliderLabelsText[5]  ) );
    m_aJahSliderLabels[6]->setText( tr( m_aJahSliderLabelsText[6]  ) );
    m_aJahSliderLabels[7]->setText( tr( m_aJahSliderLabelsText[7]  ) );
    m_aJahSliderLabels[8]->setText( tr( m_aJahSliderLabelsText[8]  ) );

}



//////////////////////////////////////////////////////
//now the slider checkbox options

// IMPORTANT
//we need to loop thorugh 'num options'
//since all options in the array may not be defined

void GLWorld::showOptions(int ns )
{
    for (int i=0; i<=ns; i++)
    {
        JahModuleOption[i]->show();
    }
}

void GLWorld::hideOptions(int ns )
{
    //debug("hiding");
    for (int i=0; i<=ns; i++)
    {
        JahModuleOption[i]->hide();
    }
}


void GLWorld::setOptionValue(int option, bool value)
{
    JahModuleOption[option]->setChecked(value);

}

//////////////////////////////////////////////
// Slider related

void GLWorld::updateUiOptions()       { /* this is overridden */   }

void GLWorld::updateUiSettings()       { /* this is overridden */   }


void GLWorld::updateSliders()
{
  jtrace->debug( "GLWorld","updating slider display");

    if ( !multipleLayersSelectedInListView() )
    {
        hideSliders(9);
        hideAllHeadings();

        /////////////////////////////////////////////////////////////////////////////
        //basic values here
        if (getActiveJahLayer() != camera)
        {
            showSliders(9);
            showAllHeadings();
        }
        else
        {
            //its the world
            showSliders(5);
            showHeadings(2);
        }

        ///////////////////////////////////////////
        // update the rest of the ui
        updateSliderValues();
        slotUpdateListviewCheckboxes();

        updateUiOptions();

        updateUiSettings();

    }
}

//void GLWorld::updateSliderValues()  { /* this is overridden */   }

void 
GLWorld::setXRotationSlider(JahLayer* jah_layer, int value)
{
    setActiveJahLayer(jah_layer);
    setSliderValue(0, value);
}

void 
GLWorld::setYRotationSlider(JahLayer* jah_layer, int value)
{
    setActiveJahLayer(jah_layer);
    setSliderValue(1, value);
}

void 
GLWorld::setZRotationSlider(JahLayer* jah_layer, int value)
{
    setActiveJahLayer(jah_layer);
    setSliderValue(2, value);
}

void 
GLWorld::setXTranslationSlider(JahLayer* jah_layer, int value)
{
    setActiveJahLayer(jah_layer);
    setSliderValue(3, value);
}

void 
GLWorld::setYTranslationSlider(JahLayer* jah_layer, int value)
{
    setActiveJahLayer(jah_layer);
    setSliderValue(4, value);
}

void 
GLWorld::setZTranslationSlider(JahLayer* jah_layer, int value)
{
    setActiveJahLayer(jah_layer);
    setSliderValue(5, value);
}

void 
GLWorld::setXScaleSlider(JahLayer* jah_layer, int value)
{
    setActiveJahLayer(jah_layer);
    setSliderValue(6, value);
}

void 
GLWorld::setYScaleSlider(JahLayer* jah_layer, int value)
{
    setActiveJahLayer(jah_layer);
    setSliderValue(7, value);
}

void 
GLWorld::setZScaleSlider(JahLayer* jah_layer, int value)
{
    setActiveJahLayer(jah_layer);
    setSliderValue(8, value);
}

void 
GLWorld::setTransparencySlider(JahLayer* jah_layer, int value)
{
    setActiveJahLayer(jah_layer);
    setSliderValue(9, value);
}

void GLWorld::updateSliders(motionNode*)
{
}

//updates all the sliders
void GLWorld::updateSliderValues()
{
  jtrace->debug( "GLWorld","updating slider values");

    if ( !multipleLayersSelectedInListView() )
    {
        motionNode* motion_node = getActiveJahLayer()->layernodes->m_node;


        /////////////////////////////////////////////////////////////////////////////
        //basic values here
        if (getActiveJahLayer() != camera)
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

        }
        else
        {
            motionNode* interpolated_values = camera->layernodes->m_node;
            //its the camera
            setSliderValue( 0, (int)interpolated_values->rx );
            setSliderValue( 1, (int)interpolated_values->ry );
            setSliderValue( 2, (int)interpolated_values->rz );

            setSliderValue( 3, (int)interpolated_values->tx );
            setSliderValue( 4, (int)interpolated_values->ty );
            setSliderValue( 5, (int)interpolated_values->tz );
        }

        motionNode* interpolated_values;

        if ( getActiveJahLayer()->objtype == ObjectCategory::EFFECT )
        {
            interpolated_values = getActiveJahLayer()->getParent()->layernodes->m_node;
        }
        else
        {
            interpolated_values = motion_node;
        }

        if ( getModuleUsesLighting() )
        {
            getLightingSliderPtr(0)->setValue( (int)( interpolated_values->ambient_light_color.x * 255.0f) );
            getLightingSliderPtr(1)->setValue( (int)( interpolated_values->ambient_light_color.y * 255.0f) );
            getLightingSliderPtr(2)->setValue( (int)( interpolated_values->ambient_light_color.z * 255.0f) );
            getLightingSliderPtr(3)->setValue( (int)( interpolated_values->diffuse_light_color.x * 255.0f) );
            getLightingSliderPtr(4)->setValue( (int)( interpolated_values->diffuse_light_color.y * 255.0f) );
            getLightingSliderPtr(5)->setValue( (int)( interpolated_values->diffuse_light_color.z * 255.0f) );
            getLightingSliderPtr(6)->setValue( (int)( interpolated_values->specular_light_color.x * 255.0f) );
            getLightingSliderPtr(7)->setValue( (int)( interpolated_values->specular_light_color.y * 255.0f) );
            getLightingSliderPtr(8)->setValue( (int)( interpolated_values->specular_light_color.z * 255.0f) );
        }
    }
}

