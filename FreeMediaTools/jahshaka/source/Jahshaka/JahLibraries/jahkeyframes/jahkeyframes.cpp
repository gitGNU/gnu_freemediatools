/*******************************************************************************
**
** The header file for the Jahshaka jahpreferences object 
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "jahkeyframes.h"

void motionNode::clear(void) 
{
    m_frame_number = 0;

    tx=0.0f;    ty=0.0f;    tz=0.0f;
    rx=0.0f;    ry=0.0f;    rz=0.0f;
    sx=1.0f;    sy=1.0f;    sz=1.0f;  sc=1.0f;  

    Alpha = 100.0f;     // as in 100%  

    extrude = DEFAULT_TEXT_EXTRUDE_DEPTH;
	
	bx=0.0f;		by=0.0f;

    ambient_light_color.x = 1.0f;
    ambient_light_color.y = 1.0f;
    ambient_light_color.z = 1.0f;
    ambient_light_color.w = 1.0f;
    diffuse_light_color.x = 1.0f;
    diffuse_light_color.y = 1.0f;
    diffuse_light_color.z = 1.0f;
    diffuse_light_color.w = 1.0f;
    specular_light_color.x = 1.0f;
    specular_light_color.y = 1.0f;
    specular_light_color.z = 1.0f;
    specular_light_color.w = 1.0f;
    layer_enabled = true;
}

void motionNode::copy(motionNode* existing_node) 
{
    //m_frame_number = existing_node->m_frame_number;

    tx = existing_node->tx;    
    ty = existing_node->ty;    
    tz = existing_node->tz;
    rx = existing_node->rx;    
    ry = existing_node->ry;    
    rz = existing_node->rz;
    sx = existing_node->sx;    
    sy = existing_node->sy;    
    sz = existing_node->sz;    
    sc = existing_node->sc;

    Alpha = existing_node->Alpha;

    extrude = existing_node->extrude;

	bx = existing_node->bx;
	by = existing_node->by;

    ambient_light_color = existing_node->ambient_light_color;
    diffuse_light_color = existing_node->diffuse_light_color;
    specular_light_color = existing_node->specular_light_color;
    layer_enabled = existing_node->layer_enabled;
}
