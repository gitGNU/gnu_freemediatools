/*******************************************************************************
**
** The header file for the Jahshaka OpenObjectLib module 
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/
#ifndef OPENOBJECTLIB_H
#define OPENOBJECTLIB_H

/////////////////////////////////////////
//this is the base openobjectlib class
//it should allow us to initialize the oolib engine
//load all the plugins
//and give the app access to the library and
//subclasses by including all the headers here


// System Includes
#include <string>
#include <qptrlist.h>
#include "opencore.h"

// Forward Declarations
class OpenObjectPluginList ;
//class JahTrace ;

///////////////////////////////////////
//this class initalizes the library
//and loads all the plugins in

class OpenObjectLib
{

    public:

        //for some reason we have to do this public on windows vc6++
        #ifdef WIN32
        ~OpenObjectLib();
        #endif

    private:

		//////////////////////////////////////////
		//initalizing routines for singleton
        static OpenObjectLib* _instance ;

        // These are needed to ensure that the singletons destructor is called on application shutdown.
        class OpenObjectLibDestroyer;
        static OpenObjectLibDestroyer _destroyer;

        // Hide these 'cause this is a singleton.       
        OpenObjectLib(); 

        #ifndef WIN32
        ~OpenObjectLib();    
        #endif

        OpenObjectLib( const OpenObjectLib&);
        OpenObjectLib& operator = ( const OpenObjectLib&) ;
        
		//////////////////////////////////////////
		//the list of plugins
		QPtrList<OpenObjectPluginList> *oolibPlugins;

		//total number of plugins
		//should this be private?
        int oolibPluginsCounter;

		//each object has a unique number
		//should be private as well?
		int objcounter;

    public:
        
		//////////////////////////////////////////
		//gets a instance of this object
        static OpenObjectLib* getInstance();
	
		////////////////////////////////////////
		//pointer to tracer singleton
		OL_Trace			* jtrace;

		//////////////////////////////////////////
		//class routines start here

        void initializePlugins	( void );

        int getPluginId			(std::string name, int theclass);

		//////////////////////////////////////////

		bool createObject		( int pluginId, int objnum, std::string filename  );
		void drawObject			( int pluginId, int objnum  );
		bool deleteObject		( int pluginId, int objnum  );

        std::string getFormats		( void );

		bool getFileFormat		(std::string filename, std::string & theExtension);
		void listPlugins		( void );
		int getObjcounter		( void );
		int incrementObjcounter ( void );
		std :: string getFilename		( int pluginId, int objnum );

		//set flags in the plugin
		void setSmoothingAngle	(int pluginId, int objnum, float smoothing_angle);
		void setMaterialMode	(int pluginId, int objnum, int material_mode);
		void setFacetNormal		(int pluginId, int objnum, bool facet_normal);

		//utility
		char * toLowercaseInPlace(char * const & ai_string);
		
		bool checkExtension ( 
			char const * const & a_filename,
			char const * const & a_extension);

};


//for application access
#include "openobject.h"

#endif

/*
////////////////////////////////////////
// DOCUMENTATION

	////////////////////////////////////////////
	//routines used to initialize library and 
	//load the plugins in
	////////////////////////////////////////////

		JahTrace& jtrace = JahTrace::getInstance();
		jtrace.info( ">Initializing OpenObjectLib Plugins");

		//lib loader here
		OpenObjectLib* oolib = OpenObjectLib::getInstance();
		oolib->initializePlugins();

		jtrace.info( ">Initialized OpenObjectLib Plugins");


	////////////////////////////////////////////
	//routines used to destroy the library
	//and unload the plugins
	////////////////////////////////////////////

		JahTrace& jtrace = JahTrace::getInstance();
		jtrace.info( ">Unloading OpenObjectLib");

		//lib unloader here
		OpenObjectLib* oolib = OpenObjectLib::getInstance();
		oolib->destroy();

		jtrace.info( ">Unloaded OpenObjectLib");


*/
