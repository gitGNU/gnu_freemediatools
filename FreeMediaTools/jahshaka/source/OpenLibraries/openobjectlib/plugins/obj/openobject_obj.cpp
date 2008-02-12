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
#include "openobject_obj.h"

// Other Includes
#include "../openobject_plugincore.h" // Do not remove ! see comment in the file.


void    OpenObjectPlugin::initializePlugin(void)
{
    pluginName		= "glmlib";			//name of plugin
    pluginFamily	= "object";         //family of plugin
    pluginGroup		= "3d";				//group of plugin
    pluginClass		= OOL_PLUGIN_3D;    //class of plugin
    pluginExtension	= "OBJ";			//supported extension
}


//itw confusing since this routine does both a open and a add
//we should break these into 2 separate routines
bool OpenObjectPlugin::openObject(int objnum, std::string const & a_filename) //called in OpenObjectLib::createObject
{
 
	///////////////////////////////////////////////////
	//create a new entry in the list
	objectManager->append( new pluginObjectManager(objnum) );
	numberObjects++;

	///////////////////////////////////////////////////
	//create new instance of the object
	objectManager->at(numberObjects)->model = new GLMmodel;

	//also if status is false at the end of the add
	//we need to erase the item from the list
	//and let the objectlib know it didnt work
	bool status		= false;

	//#ifndef __sgi
	//if (model) glmDelete(model);
	//#endif

	///////////////////////////////////////////////
	//we can make model a pointer to the one in the objectManager
	objectManager->at(numberObjects)->model = glmReadOBJ( (char *) a_filename.data());   //was model_file

	//set up some defaults here
    objectManager->at(numberObjects)->smoothing_angle = 90.0;	             // smoothing angle
    objectManager->at(numberObjects)->material_mode   = 0;
    objectManager->at(numberObjects)->facet_normal    = false;

	//clean up model
	objectManager->at(numberObjects)->scale = 
		glmUnitize(objectManager->at(numberObjects)->model);

	glmFacetNormals(objectManager->at(numberObjects)->model);

	glmVertexNormals(objectManager->at(numberObjects)->model, 
		objectManager->at(numberObjects)->smoothing_angle);

	if (objectManager->at(numberObjects)->model->nummaterials > 0) 
		objectManager->at(numberObjects)->material_mode = 2;						   
	else 
		objectManager->at(numberObjects)->material_mode = 0;

	glmScale(objectManager->at(numberObjects)->model, 10);

	///////////////////////////////////////////////
	//if we created the model we return successfully
	if (objectManager->at(numberObjects)->model) 
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

	glmDelete(objectManager->at(thaobjlayer)->model);

	status = true;

	//objectopen = false;

    return status;

};

void OpenObjectPlugin::drawObject(int objnum)
{
	//first we need to find out which object in the qptrlist 
	//relates to objnum and use statics to speed it up if we dont need to check
	//for changes
	//static int theone;

	int theobject = findObjectLayer(objnum);

	//get rid of this once the above works
	//theobject = numberObjects;

    if (objectManager->at(theobject)->model)
    {

		//just got to figure out texturing properly!
		glShadeModel( GL_SMOOTH );

		glPushMatrix(); //first we correct the orientation
		glNormal3f( 0.0, 0.0, 0.0 );

		glScalef ( 40, 40, 40 );

		if (objectManager->at(theobject)->material_mode == 0)
		{
			if (objectManager->at(theobject)->facet_normal)
			{
				glmDraw(objectManager->at(theobject)->model, GLM_FLAT);
			}
			else
			{
				glmDraw(objectManager->at(theobject)->model, GLM_SMOOTH);
			}
		}
		else if (objectManager->at(theobject)->material_mode == 1)
		{
			if (objectManager->at(theobject)->facet_normal)
			{
				glmDraw(objectManager->at(theobject)->model, GLM_FLAT | GLM_COLOR);
			}
			else
			{
				glmDraw(objectManager->at(theobject)->model, GLM_SMOOTH | GLM_COLOR);
			}
		}
		else if (objectManager->at(theobject)->material_mode == 2)
		{
			if (objectManager->at(theobject)->facet_normal)
			{
				glmDraw(objectManager->at(theobject)->model, GLM_FLAT | GLM_MATERIAL);
			}
			else
			{
				glmDraw(objectManager->at(theobject)->model, GLM_SMOOTH | GLM_MATERIAL);
			}
		}
		else if (objectManager->at(theobject)->material_mode == 4)
		{
			if (objectManager->at(theobject)->facet_normal)
			{
				glmDraw(objectManager->at(theobject)->model, GLM_FLAT | GLM_TEXTURE);
			}
			else
			{
				glmDraw(objectManager->at(theobject)->model, GLM_SMOOTH | GLM_TEXTURE);
			}
		}
		else if (objectManager->at(theobject)->material_mode == 5)
		{
			if (objectManager->at(theobject)->facet_normal)
			{
				glmDraw(objectManager->at(theobject)->model, GLM_FLAT | GLM_NONE);
			}
			else
			{
				glmDraw(objectManager->at(theobject)->model, GLM_SMOOTH | GLM_NONE);
			}
		}

		glPopMatrix();
    }
};

//////////////////////////////////////////////////
//update object settings

void	OpenObjectPlugin::setSmoothingAngle(int objnum,float angle) 
{ 
	int theobject = findObjectLayer(objnum);
	objectManager->at(theobject)->smoothing_angle = angle;  
}

void	OpenObjectPlugin::setMaterialMode  (int objnum, int mode)    
{ 
	int theobject = findObjectLayer(objnum);
	objectManager->at(theobject)->material_mode   = mode;   
}

void	OpenObjectPlugin::setFacetNormal   (int objnum, bool normal) 
{ 
	int theobject = findObjectLayer(objnum);
	objectManager->at(theobject)->facet_normal    = normal; 
}
