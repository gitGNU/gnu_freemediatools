/*******************************************************************************
**
** The header file for the Jahshaka jahpreferences module 
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/
#ifndef JAHKEYFRAMES_H
#define JAHKEYFRAMES_H

/////////////////////////////////////////////////////////
//core keyframe structure

#include <string>
#include <fstream>
//#include <qgl.h>

#include "gpumathlib.h"

//these can all be floats not GLfloats

class motionNode 
{
public:

  motionNode ();

  ~motionNode ();

  int         m_frame_number;   // this is the keyframe
  GLfloat     tx, ty, tz;
  GLfloat     rx, ry, rz;
  GLfloat     sx, sy, sz, sc;
  GLfloat     extrude;
  GLfloat     Alpha;
  
  //for the blur :)
  GLfloat     bx, by;
  
  float4      ambient_light_color;
  float4      diffuse_light_color;
  float4      specular_light_color;
  
  bool        layer_enabled;
  
  
  void copy(motionNode*);
  void clear(void);

  void saveXML( std::fstream & ai_fstream )
	{
		ai_fstream << "<MOTIONNODE\n" ;
		ai_fstream << " m_frame_number = '" << m_frame_number << "'\n" ;  
  		ai_fstream << " tx = '" << tx << "'\n" ;  
		ai_fstream << " ty = '" << ty << "'\n" ;  
		ai_fstream << " tz = '" << tz << "'\n" ;  
		ai_fstream << " rx = '" << rx << "'\n" ;  
		ai_fstream << " ry = '" << ry << "'\n" ;  
		ai_fstream << " rz = '" << rz << "'\n" ;  
		ai_fstream << " sx = '" << sx << "'\n" ;  
		ai_fstream << " sy = '" << sy << "'\n" ;  
		ai_fstream << " sz = '" << sz << "'\n" ;  
		ai_fstream << " sc = '" << sc << "'\n" ;  
		ai_fstream << " Alpha = '" << Alpha << "'\n" ;  
		ai_fstream << " bx = '" << bx << "'\n" ;  
        ai_fstream << " extrude = '" << extrude << "'\n" ;
		ai_fstream << " by = '" << by << "'\n" ;  
		ai_fstream << " />" << "\n" ;     
	}
};

#endif

