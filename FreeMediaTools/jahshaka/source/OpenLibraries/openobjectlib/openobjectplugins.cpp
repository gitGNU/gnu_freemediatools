/*******************************************************************************
**
** The header file for the Jahshaka effects module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

// System Includes

// Own Include
#include "openobjectplugins.h" //jahplugin is temporarily in here

// Other Includes
//#include "jahpreferences.h"


//for ze pluginz
typedef OpenObjectLibPlugin* (*oofunction)();


OpenObjectPluginList::OpenObjectPluginList( )
{
    thePlugin     = 0;
    OpenObjectPluginLib  = 0;

    pluginLoaded  = FALSE;

    //used to debug plugin class
    plDebug = true;

};

OpenObjectPluginList::~OpenObjectPluginList( )
{
	OpenObjectPluginLib->unload();
    delete OpenObjectPluginLib;
}

bool  OpenObjectPluginList::loadPlugin(std::string const & loadname)
{
    //set up prefs used to validate plugin compatability
	//this can be hard coded in the app and the plugins
	//instead of using jahprefs
    //JahPrefs& jprefs    = JahPrefs::getInstance();

   //now load the plugin
   OpenObjectPluginLib = new QLibrary ( QString ( loadname.data() ) );

    std::string str = OpenObjectPluginLib->library().data(); //will set str to "mylib.dll" on Windows, and "libmylib.so" on Linux.

    OpenObjectPluginLib->load();

	//see if it all worked out
    if ( !OpenObjectPluginLib->isLoaded() )
    {
        if (plDebug)
        {
//            debug(">>> error loading plugin:"+str);
        }
        return false;
    }

    //now we resolve the location and get a instance
    oofunction getThePlugin =  (oofunction) OpenObjectPluginLib->resolve("getPluginClass");

    if ( ! getThePlugin )
    {
        if (plDebug)
        {
//            debug(">>> error resolving plugin:"+str);
        }

        return false;
    }
    else
    {
        if (plDebug) 
        {
//            debug(">>> loaded plugin:"+str);
        }

        //first we grab the plugin here
        thePlugin = getThePlugin();

        //then we check to see if plugin is for the current version of jahshaka

        //std::string JahVersion    = jprefs.getJahVersion().data();
        std::string JahVersion    = "2.0";
        std::string PluginVersion = thePlugin->JahPluginVersion;

        //if (JahVersion.contains(PluginVersion))
        if ( JahVersion.find( PluginVersion ) != std::string::npos )
        {
            //if it is we set up important globals are set here
            pluginName     = thePlugin->getName();
            pluginFamily   = thePlugin->getFamily();
            pluginGroup    = thePlugin->getGroup();
            thepluginClass = thePlugin->getClass();
            pluginExtension = thePlugin->getExtensions();
			
			pluginLoaded = true;
		}
        else
        {
            //if not dont load it - unload and erase plugin
            unloadPlugin();
            return false;
        }
	
    }

    return true;
}

bool OpenObjectPluginList::unloadPlugin()
{
    //debug("unloading plugin");

    //this is a manual unload
    if (pluginLoaded)
    {
            OpenObjectPluginLib->unload();
            pluginLoaded = false;
    }

	return true;

}


//we can get rid of this now and use the pluginLoaded bool variable...
bool OpenObjectPluginList::initPluginStatus()
{
    //checking isLoaded will crash if its not loaded
    //so we rely on the pluginLoaded value thats initialized above...

    //currently we only support effects plugins
    bool initializedOk = false;

    //this will be diffrent for ui plugins, etc
    if (pluginLoaded)
    {
        initializedOk = initializeEffectsPlugin();
    }

    return initializedOk;
    //return pluginLoaded;
}

//if it is an effects plugin then we run this...
//doesnt really initialize the plugin itself just what we need at startup time
bool OpenObjectPluginList::initializeEffectsPlugin()
{
	bool initOK = true;

    //JahPrefs& jprefs    = JahPrefs::getInstance();

	///////////////////////////////////////////////////////
	// custom routines for gpu plugins

	return initOK;

}

bool OpenObjectPluginList::isPluginLoaded()
{
    return pluginLoaded;
}

///////////////////////////////////////////////////////
// generic routines
std::string OpenObjectPluginList::getJahPluginVersion()    { return thePlugin->JahPluginVersion;   }

int OpenObjectPluginList::getPluginNumber()            { return pluginNumber;                  }
OOL_PLUGIN_CLASS OpenObjectPluginList::getPluginClass(){ return thepluginClass;                }

std::string OpenObjectPluginList::getPluginFamily()        { return thePlugin->pluginFamily;       }
std::string OpenObjectPluginList::getPluginGroup()         { return thePlugin->pluginGroup;        }
std::string OpenObjectPluginList::getPluginName()          { return thePlugin->pluginName;         }
std::string OpenObjectPluginList::getPluginExtensions()    { return thePlugin->pluginExtension;         }

//void OpenObjectPluginList::setBasePath(std::string path)   { thePlugin->setBasePath(path);         }


