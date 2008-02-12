/*******************************************************************************
**
** The Jahshaka Plugin API
** The Jahshaka Project
** Copyright (C) 2000-2004 The Jahshaka Project
** This is Open Source Software Released under the BSD License
**
*******************************************************************************/

#ifndef OPENOBJECTPLUGINTEMPLATE_H
#define OPENOBJECTPLUGINTEMPLATE_H

/////////////////////////////////////////////////////////////
//currently we need Qt to compile all jahshaka plugins

// System Includes
#include <string>
#include <qptrlist.h>

// Forward Declarations
class pluginObjectManager ;

/////////////////////////////////////////////////////////////
// The main Plugin Class

enum OOL_PLUGIN_CLASS
{
    OOL_PLUGIN_UNINITIALIZED,
	OOL_PLUGIN_3D,
    OOL_PLUGIN_2D 
};


class OpenObjectLibPluginManager
{
  public :

    int mObjectID ;

    OpenObjectLibPluginManager ( int const & aObjectID ) { mObjectID = aObjectID ; } ;
};


class OpenObjectLibPlugin
{

public:

    /////////////////////////////////////////////
    // init the plugin
    OpenObjectLibPlugin ( )
    {
        //debug(">> initializing oolib plugin");
        JahPluginVersion = "2.0";

        pluginName		= "NULL";
        pluginFamily	= "NULL";
        pluginGroup		= "NULL";
        pluginClass		= OOL_PLUGIN_UNINITIALIZED;
    
		objectopen		= false;
        pluginExtension   = "NULL";

		//first object is zero
		numberObjects = -1;

    }

    virtual ~OpenObjectLibPlugin( ) {}

	/////////////////////////////////////////////////////////
	//main plugin information
	virtual std::string getName(void)              { return pluginName;   }
	virtual std::string getFamily(void)            { return pluginFamily; }
	virtual std::string getGroup(void)             { return pluginGroup;  }
	virtual std::string getExtensions(void)        { return pluginExtension;  }

	virtual OOL_PLUGIN_CLASS getClass(void)    { return pluginClass;  }

	virtual void   initializePlugin(){}

	/////////////////////////////////////////////////////////////
	// plugin specific

	virtual bool    openObject( int, std::string const & ) { return true ; }

	virtual bool    deleteObject(int){ return true; } //was closeObject

	virtual bool    isObjectOpen(){ return objectopen; }

	virtual void    drawObject(int){}

	//not hooked up yet
	virtual std::string getFileName(int){ return NULL;}

	//not hookeed up yet
	virtual void    printinfo(void){}

	/////////////////////////////
	virtual void	setSmoothingAngle(int,float){}
	virtual void	setMaterialMode  (int,int){}
	virtual void	setFacetNormal   (int,bool){}

	//////////////////////////////////////////////////////////////////
	// variables down here

	//important one here
	QPtrList<pluginObjectManager> *objectManager;

	//important globals here - name of the plugin
	std::string			JahPluginVersion;          //version of plugin
	std::string			pluginFamily;              //familyof plugin
	std::string			pluginGroup;               //group of plugin
	std::string			pluginName;                //name of plugin
	OOL_PLUGIN_CLASS	pluginClass;			   //class of plugin ie 2D,3D

	//plugin specific
	bool				objectopen;
	std::string			pluginExtension;                //name of plugin

	int					numberObjects;
	bool				sceneExists;
};



#endif 

