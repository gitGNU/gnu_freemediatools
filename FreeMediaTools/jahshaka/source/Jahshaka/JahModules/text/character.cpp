/*******************************************************************************
**
** The source file for the Jahshaka animation module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

# include "character.h"

GLChar::GLChar( GLCore* core, 
		const char* name,
		QHBox* controls, 
		QHBox* controller, 
		QHBox* leftcontroller, 
		QHBox* rightcontroller,
		int* globalclipnumber, 
		const QGLWidget*   )
  :  GLAnime(core, name, controls, controller, leftcontroller, rightcontroller, globalclipnumber)
  , m_controls( controls )
  , m_leftcontroller( leftcontroller )
  , m_rightcontroller( rightcontroller )
{
};

GLChar::~GLChar()
{
}
