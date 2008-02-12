/*******************************************************************************
**
** The header file for the Jahshaka Core headers file
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

// define all qt libraries used in interface creation

#ifndef COREHEADERS_H
#define COREHEADERS_H

//Windows definitions
#ifdef _WIN32
#include <windows.h>
#endif

//////////////////////////////////////////
// opengl and glut includes
#ifdef NEWOPENOBJECTLIB
	#ifndef __APPLE__
		#include <GL/glew.h>
		#include <GL/gl.h>
		#include <qgl.h>
	#else
		#include <GL/glew.h>
		#include <OpenGL/gl.h>
		#include <qgl.h>
	#endif
#else
	#ifndef __APPLE__
		#include "glew.h"
		#include <GL/gl.h>
		#include <qgl.h>
	#else
		#include "glew.h"
		#include <OpenGL/gl.h>
		#include <qgl.h>
	#endif
#endif // NEWOPENOBJECTLIB

/////////////////////////////////////////
// includes for openobjectlib
class objectData;

/////////////////////////////////////////
// includes for other object types

#include "papi.h"
#include "p_vector.h"
 
//qt specific
#include <qcolordialog.h>
#include <qimage.h>
#include <qglobal.h>
#include <qstring.h>

//for media support on layers
#include "assetdata.h"

//for the nodes for keyframes
#include "jahkeyframeobject.h"

#endif





