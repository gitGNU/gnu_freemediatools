/*******************************************************************************
**
** The header file for the Jahshaka jahpreferences object 
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "jahkeyframeobject.h"

////////////////////////////////////////////////////////////

JahKeyframeObject::JahKeyframeObject() 
{
    jtrace = JahTrace::getInstance();	//set up tracer

    //set up the nodes for keyframes
    m_node            = new motionNode;    // node is always equal to the current node
    
    ///////////////////////////////////////////////
    // the linked list is here
    m_nodelist = new QPtrList<motionNode>;
    m_nodelist->setAutoDelete( TRUE );     // delete items when they are removed!!!
    
    m_node->clear();
    
}

JahKeyframeObject::~JahKeyframeObject() 
{
  delete m_node;
  delete m_nodelist;

}


//routine to reset and clear out the keyframes for a layer
void JahKeyframeObject::reset(void) 
{
    
    m_nodelist->clear();
    delete m_nodelist;
    
    m_nodelist = new QPtrList<motionNode>;
    m_nodelist->setAutoDelete( TRUE );     // delete items when they are removed!!!
    
   
    delete m_node; 
    m_node = new motionNode;   // node is always equal to the current node
    
    m_node->clear();
    
}

//routine to clear the setting for the current default node
void JahKeyframeObject::clear(void)
{
    
    m_node->clear();
}

//////////////////////////////////////////////////////////////////
//routine to add a keyframe 
void JahKeyframeObject::addNodes(int frame_number, bool layer_enabled) 
{
  jtrace->debug( "JahKeyframes::addNodes","adding a new node");

    if (frame_number < 1)
    {
        // Hack - frame number should never be less than 1, but is.
        // Sometime I should set a break point here and find out why this is happening
        frame_number = 1;
    }
    
    motionNode* node = getKeyframe(frame_number);
  
    if ( node == NULL )
    {
        motionNode* keyframe = m_nodelist->first();
        int index = 0;

        if (keyframe)
        {
            for ( ; keyframe; keyframe = m_nodelist->next() )
            {
                if (keyframe->m_frame_number > frame_number)
                {
                    break;
                }

                index++;
            }
        }
        
        node = new motionNode;
        node->copy(m_node);
        node->m_frame_number = frame_number;
        node->layer_enabled = layer_enabled;
        m_nodelist->insert(index, node);   
    }
    else
    {
        node->copy(m_node);
    }

}


void JahKeyframeObject::removeNodes(int theFrame) 
{
    motionNode* keyframe = m_nodelist->first();

    for ( ; keyframe; keyframe = m_nodelist->next() )
    {
        if ( (keyframe->m_frame_number == theFrame) )
        {
            m_nodelist->remove(keyframe);
            return;
        }
    }
}

int JahKeyframeObject::getNextKeyframe(int current_frame_number) 
{
    if ( !m_nodelist->count() )
    {
        return -1;
    }

    motionNode* keyframe = m_nodelist->first();

    for ( ; keyframe; keyframe = m_nodelist->next() )
    {
        if ( (keyframe->m_frame_number > current_frame_number) )
        {
            return keyframe->m_frame_number;
        }
    }

    return m_nodelist->first()->m_frame_number;
}

int JahKeyframeObject::getPrevKeyframe(int current_frame_number) 
{
    if ( !m_nodelist->count() )
    {
        return -1;
    }

    motionNode* keyframe = m_nodelist->last();

    for ( ; keyframe; keyframe = m_nodelist->prev() )
    {
        if ( (keyframe->m_frame_number < current_frame_number) )
        {
            return keyframe->m_frame_number;
        }
    }

    return m_nodelist->last()->m_frame_number;
}

motionNode* 
JahKeyframeObject::getKeyframe(int frame_number) 
{
    motionNode* keyframe = m_nodelist->first();

    for ( ; keyframe; keyframe = m_nodelist->next() )
    {
        if (keyframe->m_frame_number == frame_number)
        {
            return keyframe;
        }
    }

    return NULL;
}


//updates the current node with the interpolation
void JahKeyframeObject::updateNodes(int currentFrame) 
{
    //updates node to value at current keyframe
    //getNode( currentFrame, node);
    getNode( currentFrame);
}


void JahKeyframeObject::getFirstLastFrame( int *firstFrame, int *lastFrame) 
{   
    //find frame before and frame after
    //set new frame in between

    if ( m_nodelist->count() )
    {
        *firstFrame = m_nodelist->at(0)->m_frame_number;  //nodenum is the keyframe that node was set at
        *lastFrame  = m_nodelist->at(m_nodelist->count()-1)->m_frame_number;
    }
    else
    {
        *firstFrame = 0;
        *lastFrame = 0;
    }
}

///////////////////////////////////////////////////////////////////
//this is a linear interpreter for the keyframe values
//we need to add a bezier as well and clean this up
//to make it faster
//int JahKeyframeObject::getNode(int frameNumber, motionNode *mn) 
int JahKeyframeObject::getNode(int current_frame_number) 
{
    if ( !m_nodelist->count() )
    {
        return -1;
    }
    
    motionNode* keyframe = m_nodelist->first();
    motionNode* previous_keyframe = keyframe;
    motionNode* next_keyframe = keyframe;

    bool previous_keyframe_found = false;
    bool next_keyframe_found = false;

    for ( ; keyframe; keyframe = m_nodelist->next() )
    {
        int keyframe_frame_number = keyframe->m_frame_number;

        if (keyframe_frame_number == current_frame_number)
        {
            m_node->copy(keyframe);
            return 0;
        }

        if (keyframe_frame_number < current_frame_number)
        {
            previous_keyframe = keyframe;
            previous_keyframe_found = true;
        }

        if (keyframe_frame_number > current_frame_number)
        {
            next_keyframe = keyframe;
            next_keyframe_found = true;
            break;
        }
    }

    if (!previous_keyframe_found)
    {
        previous_keyframe = m_nodelist->getFirst();
        next_keyframe = previous_keyframe;
    }
    else if (!next_keyframe_found)
    {
        previous_keyframe = m_nodelist->getLast();
        next_keyframe = previous_keyframe;
    }
    
    getLinearInterpolation(m_node, previous_keyframe, next_keyframe, current_frame_number);
    
    return 0;
}

void JahKeyframeObject::getLinearInterpolation(motionNode *mn, 
                                               motionNode* previous_keyframe, 
                                               motionNode* next_keyframe, 
                                               int frameNumber) 
{
    if (next_keyframe== NULL || previous_keyframe == NULL)
    {
        return;
    }

    mn->layer_enabled = previous_keyframe->layer_enabled;

    float diff, mult, dist;
    
    if (next_keyframe->m_frame_number == previous_keyframe->m_frame_number)
    {
        diff = 1.0f;
    }
    else
    {
        diff = next_keyframe->m_frame_number - previous_keyframe->m_frame_number;
    }

    mult = (float)(frameNumber - previous_keyframe->m_frame_number) / diff;
    
    dist      = next_keyframe->sx    - previous_keyframe->sx;    
    mn->sx    = previous_keyframe->sx + dist * mult;
    
    dist      = next_keyframe->sy    - previous_keyframe->sy;    
    mn->sy    = previous_keyframe->sy + dist * mult;
    
    dist      = next_keyframe->sz    - previous_keyframe->sz;    
    mn->sz    = previous_keyframe->sz + dist * mult;
    
    dist      = next_keyframe->rx    - previous_keyframe->rx;    
    mn->rx    = previous_keyframe->rx + dist * mult;
    
    dist      = next_keyframe->ry    - previous_keyframe->ry;    
    mn->ry    = previous_keyframe->ry + dist * mult;
    
    dist      = next_keyframe->rz    - previous_keyframe->rz;    
    mn->rz    = previous_keyframe->rz + dist * mult;
    
    dist      = next_keyframe->tx    - previous_keyframe->tx;    
    mn->tx    = previous_keyframe->tx + dist * mult;
    
    dist      = next_keyframe->ty    - previous_keyframe->ty;    
    mn->ty    = previous_keyframe->ty + dist * mult;
    
    dist      = next_keyframe->tz    - previous_keyframe->tz;    
    mn->tz    = previous_keyframe->tz + dist * mult;
    
    dist      = next_keyframe->Alpha - previous_keyframe->Alpha; 
    mn->Alpha = previous_keyframe->Alpha + dist * mult;
    
    dist        = next_keyframe->extrude - previous_keyframe->extrude; 
    mn->extrude = previous_keyframe->extrude + dist * mult;
    
    dist      = next_keyframe->bx    - previous_keyframe->bx;    
    mn->bx    = previous_keyframe->bx + dist * mult;
    
    dist      = next_keyframe->by - previous_keyframe->by; 
    mn->by    = previous_keyframe->by + dist * mult;
    
    float4 next_ambient_light_color = next_keyframe->ambient_light_color;
    float4 previous_ambient_light_color = previous_keyframe->ambient_light_color;
    float4 dist4  =  next_ambient_light_color - previous_ambient_light_color;
    dist4 *= mult;
    mn->ambient_light_color = previous_keyframe->ambient_light_color + dist4;

    float4 next_diffuse_light_color = next_keyframe->diffuse_light_color;
    float4 previous_diffuse_light_color = previous_keyframe->diffuse_light_color;
    dist4  =  next_diffuse_light_color - previous_diffuse_light_color; 
    dist4 *= mult;
    mn->diffuse_light_color = previous_keyframe->diffuse_light_color + dist4;

    float4 next_specular_light_color = next_keyframe->specular_light_color;
    float4 previous_specular_light_color = previous_keyframe->specular_light_color;
    dist4  =  next_specular_light_color - previous_specular_light_color; 
    dist4 *= mult;
    mn->specular_light_color = previous_keyframe->specular_light_color + dist4;

    mn->m_frame_number = frameNumber;
}





