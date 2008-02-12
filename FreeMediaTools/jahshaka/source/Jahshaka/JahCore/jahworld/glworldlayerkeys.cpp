/*******************************************************************************
**
** The source file for the Jahshaka glworldlayerkeys.cpp module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "glworld.h"
#include "coreobject.h"

//////////////////////////////////////////////////////////////////
//routine to add a keyframe 
void GLWorld::addkeyNodes(JahLayer * layer, int theframe) 
{ 
	//need to check if theres a node at the current position
	//and remove it before adding the new one
	//a cheat at writing over the old node 
	//until we get that in the keyframer
    
    layer->layernodes->addNodes(theframe, layer->layerStatus);
}

//////////////////////////////////////////////////////////////////
//routine to remove a keyframe 
void GLWorld::removekeyNodes(JahLayer * layer, int theframe) 
{
    layer->layernodes->removeNodes(theframe);
}

//////////////////////////////////////////////////////////////////
//updates the current node with the interpolation
void GLWorld::updatekeyNodes(JahLayer * layer,int currentFrame) 
{
    layer->layernodes->getNode( currentFrame);
}

void
GLWorld::updateKeyNodes()
{
    JahLayer* jah_layer = getActiveJahLayer();
    updatekeyNodes(jah_layer, m_nCurrentFrame);
}

//routine to reset and clear out the keyframes for a layer
void GLWorld::resetkeys(JahLayer * layer) 
{
	layer->layernodes->clear();

	clearkeys(layer);
}

//need a routine to clear all keyframes for a layer...
//


//routine to clear the setting for the current default node
void GLWorld::clearkeys(JahLayer * layer) 
{
    //all above replace with...
    layer->layernodes->clear();

}

