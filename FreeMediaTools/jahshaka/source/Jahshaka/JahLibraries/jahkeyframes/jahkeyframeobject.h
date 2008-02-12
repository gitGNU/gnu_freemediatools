/*******************************************************************************
**
** The header file for the Jahshaka jahpreferences module 
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/
#ifndef JAHKEYFRAMEOBJECT_H
#define JAHKEYFRAMEOBJECT_H

/////////////////////////////////////////////////////////
//core keyframe structure

#include "jahkeyframes.h"
#include "jahtracer.h"

#include <qptrlist.h>

////////////////////////////////////////////////////////////
// the main layer object class

class JahKeyframeObject 
{

public:
    JahKeyframeObject(void);

    ~JahKeyframeObject(void);

    //////////////////////////////////////////////
    //keyframe routines
    void printInfo(void);

    //void addNodes(int currentFrame, motionNode * newNode);
    void addNodes(int currentFrame, bool layer_enabled);

    void removeNodes(int currentFrame);
	int  getPrevKeyframe(int currentFrame);
    int  getNextKeyframe(int currentFrame); 
    motionNode*  getKeyframe(int theFrame);
	
    void clear(void);

    void updateNodes(int currentFrame);  //gets interpolation

    void getFirstLastFrame( int *firstFrame, int *lastFrame);

    //int  getNode(int frameNumber, motionNode *mn);
    int  getNode(int frameNumber);

    void getLinearInterpolation(motionNode *mn,
                                motionNode* previous_keyframe, motionNode* next_keyframe, int frameNumber); 

    void reset(void);
    
public:
	////////////////////////////////////////
	//pointer to tracer
	JahTrace			* jtrace;

    //make these private and add routines to access them...

    //the current node
    motionNode* m_node;
    
    //for the linked list for dynamic keyframes
    QPtrList<motionNode>* m_nodelist;

	void saveXML( std::fstream & ai_fstream ) 
	{ 
		m_node->saveXML(ai_fstream); 
	};
};



#endif

