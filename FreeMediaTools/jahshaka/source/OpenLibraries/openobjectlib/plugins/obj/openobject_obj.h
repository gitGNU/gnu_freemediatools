/*******************************************************************************
**
** The header file for the Jahshaka OpenObject Glm plugin 
** The Jahshaka Project
** Copyright (C) 2000-2004 The Jahshaka Project
** Released under the GNU General Public License
**
*******************************************************************************/
#ifndef OPENOBJECTOBJ_H
#define OPENOBJECTOBJ_H

// System Includes
#include <string>

// Other Includes
#include "../openobjectplugintemplate.h"
#include "glm.h"

// Forward Declarations

// dll setup
#ifdef Q_WS_WIN
# define MY_EXPORT __declspec(dllexport)
#else
# define MY_EXPORT
#endif
 
extern "C" { MY_EXPORT OpenObjectLibPlugin * getPluginClass ( ) ; }



class pluginObjectManager : public OpenObjectLibPluginManager
{
  public :

	pluginObjectManager ( int const & aObjectID ) : OpenObjectLibPluginManager ( aObjectID ) { } ;

	//these are specific to the plugin
	GLuint          model_list;                // display list for object 
    GLMmodel*       model;                     // glm model data structure 
    GLfloat         scale;                     // original scale factor 
    char*           model_file;                // name of the obect file 

	//these are settings unique to each model
	float			smoothing_angle;  
	int				material_mode;   
	bool			facet_normal;
};



class OpenObjectPlugin : public OpenObjectLibPlugin
{

  public :

	OpenObjectPlugin ( ) ;

    ~ OpenObjectPlugin ( )
    {
		//need to iterate list and erase all allocated
		//objects and the list itself
		//should be done in header?
    } ;

	int findObjectLayer ( int aObjectId ) ;
	
    void   initializePlugin ( ) ;

	bool    openObject ( int objnum , std::string const & mediafile ) ;
	bool    deleteObject(int objnum);
	void    drawObject(int objnum);

	//update object settings
	void	setSmoothingAngle(int objnum,float angle);
	void	setMaterialMode  (int objnum,int mode);
	void	setFacetNormal   (int objnum,bool normal);
};


#endif //OPENOBJECTOBJ_H




