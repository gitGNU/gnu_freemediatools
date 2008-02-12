/*******************************************************************************
**
** The source file for the Jahshaka OpenObject Glm plugin 
** The Jahshaka Project
** Copyright (C) 2000-2004 The Jahshaka Project
** Released under the GNU General Public License
**
*******************************************************************************/

// System Includes
#include <qgl.h>

// Own Include
#include "openobject_dxf.h"

// Other Includes
#include <openobject_plugincore.h> // Do not remove ! see comment in the file.

#ifdef WHERE_THE_HELL_DOES_THIS_COME_FROM

#include "surface3d.h"
#include "dxf.h"
#include "biglist.h"


void    OpenObjectPlugin::initializePlugin(void)
{
    pluginName		= "dxflib";			//name of plugin
    pluginFamily	= "object";         //family of plugin
    pluginGroup		= "3d";				//group of plugin
    pluginClass		= OOL_PLUGIN_3D;    //class of plugin
    pluginExtension	= "dxf";			//supported extension
}


//itw confusing since this routine does both a open and a add
//we should break these into 2 separate routines
bool OpenObjectPlugin::openObject(int objnum, std::string const & a_filename) //called in OpenObjectLib::createObject
{
 
	///////////////////////////////////////////////////
	//create a new entry in the list
	objectManager->append( new pluginObjectManager(objnum) );
	numberObjects++;

	//also if status is false at the end of the add
	//we need to erase the item from the list
	//and let the objectlib know it didnt work
  
	objectopen = readDxf(
		(char *) a_filename.data(),
		m_the_surface3d.mVertices,			//must be allocated by: new type[0]
		m_the_surface3d.mTriangles);		//must be allocated by: new type[0]

	if ( objectopen )
	{
		objectopen = charutilities::loadData ( m_the_surface3d );
	}

	return objectopen;

};


bool OpenObjectPlugin::deleteObject(int objnum)
{
	objectopen = false;

    return true;
};

void OpenObjectPlugin::drawObject(int objnum)
{
    //glDisable(GL_LIGHTING);   
	charutilities::bigList(m_the_surface3d);

};


#endif
