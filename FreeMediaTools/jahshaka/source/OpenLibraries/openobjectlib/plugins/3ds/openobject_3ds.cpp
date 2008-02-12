/*******************************************************************************
**
** The source file for the Jahshaka Openobject max plugin
** The Jahshaka Project
** Copyright (C) 2000-2004 The Jahshaka Project
** Released under the GNU General Public License
**
*******************************************************************************/
#ifdef _MSC_VER
#pragma warning (disable : 4786)
#endif

// System Includes
#include <qgl.h>

// Own Include
#include "openobject_3ds.h"

// Other Includes
#include "../openobject_plugincore.h" // Do not move ! see comment in the file.


void OpenObjectPlugin::initializePlugin(void)
{
    pluginName		= "3dsmaxlib";			//name of plugin
    pluginFamily	= "object";				//family of plugin
    pluginGroup		= "3d";					//group of plugin
    pluginClass		= OOL_PLUGIN_3D;		//class of plugin 
    pluginExtension	= "3DS";				//supported extension
}

bool OpenObjectPlugin::openObject(int objnum, std::string const & a_filename )
{
	///////////////////////////////////////////////////
	//create a new entry in the list
	objectManager->append( new pluginObjectManager(objnum) );
	numberObjects++;

	///////////////////////////////////////////////////
	//create new instance of the object
	objectManager->at(numberObjects)->scene = new L3DS;

	//also if status is false at the end of the add
	//we need to erase the item from the list
	//and let the objectlib know it didnt work
	bool status		= false;


	//and here we load the object into the
	//maxobject
	if (!objectManager->at(numberObjects)->scene->LoadFile( a_filename.data()))
	{
		status = false;
	}
	else
	{
		status = true;
		objectopen = true;
	}

	return status;

};


bool OpenObjectPlugin::deleteObject(int objnum)
{
    bool status = false;

	int thaobjlayer = findObjectLayer(objnum);

	//if (scene)  //sceneexists
		delete objectManager->at(thaobjlayer)->scene;

	status = true;

	//objectopen = false;

    return status;

};


void OpenObjectPlugin::drawObject(int objnum)
{
	int theobject = findObjectLayer(objnum);
    
	if (objectManager->at(theobject)->scene)
    {

		glPushMatrix(); //first we correct the orientation
		glScalef ( 40, 40, 40 );
		glNormal3f( 0.0, 0.0, 0.0 );

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		int a = objectManager->at(theobject)->scene->GetMeshCount();

		//printf("There are %d meshes in the 3ds file\n", a);

		for (int i= 0; i< a; i++)
		{
			LMesh &mesh = objectManager->at(theobject)->scene->GetMesh(i);

			glVertexPointer(4, GL_FLOAT, 0, &mesh.GetVertex(0));
			glTexCoordPointer(4, GL_FLOAT, 0, &mesh.GetVertex(0));
			glNormalPointer(GL_FLOAT, 0, &mesh.GetNormal(0));
			glColorPointer(3, GL_FLOAT, 0, &mesh.GetBinormal(0));
			glDrawElements(GL_TRIANGLES, mesh.GetTriangleCount()*3,
			GL_UNSIGNED_SHORT, &mesh.GetTriangle(0));
		}

		glPopMatrix(); //first we correct the orientation

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);

    //glDisable(GL_NORMALIZE);
    //glDisable(GL_LIGHTING);
    //if textureing is on... need a flag for this
    //glDisable(GL_TEXTURE_2D);
    }
};

