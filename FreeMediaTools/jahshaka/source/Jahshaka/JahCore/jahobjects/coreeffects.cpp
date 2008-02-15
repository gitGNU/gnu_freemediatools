/*******************************************************************************
**
** The header file for the Jahshaka core effects module 
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "coreeffects.h"
#include <string>


std::vector<std::string> EffectInfo::m_name_table(EffectInfo::NOT_A_TYPE);


EffectInfo::EffectInfo(EFFECT_TYPE type)
{
	m_type = type;

	m_name_table[GREY_CPU_TYPE] = "Grey";
	m_name_table[EDGE_DETECT_CPU_TYPE] = "Edge Detection";
	m_name_table[SELECT_COLORS_CPU_TYPE] = "Select Colors";
	m_name_table[CONTRAST_CPU_TYPE] = "Contrast";
	m_name_table[BRIGHTNESS_CPU_TYPE] = "Brightness";
	m_name_table[MATRIX_SHARPEN_CPU_TYPE] = "Sharpen";
	m_name_table[MATRIX_BLUR_CPU_TYPE] = "Matrix Blur";
	m_name_table[MATRIX_EMBOSS_CPU_TYPE] = "Emboss";
	m_name_table[GAUSSIAN_BLUR_CPU_TYPE] = "Gaussian Blur";
	m_name_table[MIRROR_CPU_TYPE] = "Mirror";
	m_name_table[SWAP_RGB_CPU_TYPE] = "Swap RGB";
	m_name_table[NEGATIVE_CPU_TYPE] = "Negative";
	m_name_table[COLORIZE_CPU_TYPE] = "Colorize";
	m_name_table[CHROMA_KEYER_CPU_TYPE] = "Chroma Key CPU";

	m_name_table[FLAG_MESH_TYPE] = "Flag RT";
	m_name_table[FISHEYE_MESH_TYPE] = "FishEye RT";
	m_name_table[RIPPLE_MESH_TYPE] = "Ripple RT";
	m_name_table[SWIRL_MESH_TYPE] = "Swirl RT";

	m_name_table[FIRE_3D_GPU_TYPE] = "3D Fire GPU";
	m_name_table[STRETCH_CROP_GPU_TYPE] = "Stretch/Crop GPU";
	m_name_table[COLORIZE_GPU_TYPE] = "Colorize GPU";
	m_name_table[EARTHQUAKE_GPU_TYPE] = "Earthquake GPU";
	m_name_table[LIGHTING_GPU_TYPE] = "Lighting GPU";
	m_name_table[CHARCOAL_GPU_TYPE] = "Charcoal GPU";
	m_name_table[CHROMA_KEYER_GLSL_SEPARATE_ALPHA_TYPE] = "Chroma Key GLSL Separate Alpha";
	m_name_table[CHROMA_KEYER_GLSL_TYPE] = "Chroma Key GLSL";
}

//////////////////////////////////////////////////////////////
// need to fix these babys up a bit
//added the .data() bit to the == comparisons

EffectInfo::EFFECT_CATEGORY        
EffectInfo::getCategory(std::string& thename)
{
    for (int i = 0; i < NOT_A_CATEGORY; i++)
    {
        EFFECT_CATEGORY category = EFFECT_CATEGORY(i);

		if (m_name_table[category] == thename )
        {
            return category;
        }
    }

    return NOT_A_CATEGORY;
}

EffectInfo::EFFECT_TYPE         
EffectInfo::getType(std::string& name)
{
    for (int i = 0; i < NOT_A_TYPE; i++)
    {
        EFFECT_TYPE type = EFFECT_TYPE(i);

        if (m_name_table[type] == name)
        {
            return type;
        }
    }

    return NOT_A_TYPE;
}




EffectInfo::EFFECT_CATEGORY		
EffectInfo::getCategory(EFFECT_TYPE type)
{
    if (type > CPU_TYPE_START && type < MESH_TYPE_START)
    {
        return CPU_CATEGORY;
    }
    else if (type > MESH_TYPE_START && type < GPU_TYPE_START)
    {
        return MESH_CATEGORY;
    }
    else if (type > GPU_TYPE_START && type < EFFECT_TYPE_END)
    {
        return GPU_CATEGORY;
    }

    return NOT_A_CATEGORY;
}

EffectLayer::EffectLayer(void)
{
  objtype = EffectInfo::NOT_A_TYPE;
};

EffectLayer::~EffectLayer(void)
{

};


EffectInfo::EffectInfo()
{
  m_type = NOT_A_TYPE;
}

std::string*		
EffectInfo::getName(EFFECT_TYPE type)
{
  return &m_name_table[type]; 
}

std::string	    
EffectInfo::getStringName(EFFECT_TYPE type)
{ return std::string(m_name_table[type]); }

EffectInfo::EFFECT_TYPE	            EffectInfo::getType() 
{
  return m_type; 
}
void				    
EffectInfo::setType(EffectInfo::EFFECT_TYPE type) 
{
  m_type = type; 
}

JahLayer*			    
EffectInfo::getJahLayer()
{ 
  return m_jah_layer; 
}

void                    
EffectInfo::setJahLayer(JahLayer* layer)
{ 
  m_jah_layer = layer; 
}
