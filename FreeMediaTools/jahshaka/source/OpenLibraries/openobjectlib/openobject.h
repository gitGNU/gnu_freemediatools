/*******************************************************************************
**
** The header file for the Jahshaka OpenObject library
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/
#ifndef OPENOBJECT_H
#define OPENOBJECT_H

// System Includes
#include <string>
//#include <fstream>

#ifdef NEWOPENOBJECTLIB
#include <openobjectlib/sg/hw_GL_renderer.hpp>
#include <openobjectlib/sg/scene.hpp>
#else
// Forward Declarations
class OpenObjectLib;
#endif

////////////////////////////////////////////////////////////
// platform specific object support routines
//these are classes to abstract object types from the rest of the application
//allowing cross platform implementations for all object files to be
//created and maintained but allowing us to use our own calls
//defined in this file

////////////////////////////////////////////////////////////
//this is a separate class from openobjectlib.h
//since every layer has its own objectdata object
//but we use the main library as a conduit to the plugins

////////////////////////////////////////////////////////////
//The core controlling objectData class

class objectData {
    
public:

	objectData(void);
	~objectData(void);

	void    initializeEngine();

	//used to open/add and close object
	bool    openObject(std::string const & mediafile);
	bool    closeObject(void);

	//used to draw object
	void    drawObject();

	//utility routines
	bool    isObjectOpen();
	bool    validateFile(std::string const & filename);
	bool    getExtension(std::string const & filename, std::string & theext);
	std::string getFileName();
	void    printinfo(void);

	//access to data flags
	void    updateSettings(void);

#ifndef NEWOPENOBJECTLIB
	void	setSmoothingAngle(float angle) { smoothing_angle = angle;  updateSettings(); }
	void	setMaterialMode  (int mode)    { material_mode   = mode;   updateSettings(); }
	void	setFacetNormal   (bool normal) { facet_normal    = normal; updateSettings(); }
#endif 

#ifdef NEWOPENOBJECTLIB
	olib::openobjectlib::sg::scene_ptr getScene( ) { return scene_; }
#endif

private:
#ifdef NEWOPENOBJECTLIB
	olib::openobjectlib::sg::scene_ptr		scene_;
	boost::shared_ptr<olib::openobjectlib::sg::hw_GL_renderer> r_;
#else
	/////////////////////////////////////////////////////////
	//for access to plugins via instance of openobjectlib
	OpenObjectLib		* oolib;


	//the unique i of the current object
	int					objNumber;
	int					pluginId;

	//QString JahBasePath;
	
	/////////////////////////////////////////////////////////
	//data flags for objects, this list should grow
    float				smoothing_angle;           // smoothing angle 	  
	int					material_mode;	
    bool				facet_normal;
#endif
	//so we know if a object is open
	bool				objectopen;
	
};

#endif
