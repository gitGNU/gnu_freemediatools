/*******************************************************************************
**
** The header file for the Jahshaka openobjectpluginlist module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/
#ifndef OPENOBJECTPLUGINS_H
#define OPENOBJECTPLUGINS_H

// System Includes
#include <string> 
//#include <qstring.h> 
#include <qlibrary.h> 

// Other Includes
#include "plugins/openobjectplugintemplate.h"


///////////////////////////////////////////
// linked list to store effects
class OpenObjectPluginList
{
public:

    OpenObjectPluginList( );
    ~OpenObjectPluginList( );

    //main routine that load the plugin
    bool    loadPlugin( std::string const & loadname );
	bool	unloadPlugin(void);

    //utility routine to test if plugin is loaded
    bool    initPluginStatus();
    bool    isPluginLoaded();

    //initializing plugins
    bool    initializeEffectsPlugin();


    //class data about the plugin
	std::string         getJahPluginVersion();

    int             getPluginNumber();
    OOL_PLUGIN_CLASS getPluginClass();
    std::string         getPluginFamily();
    std::string         getPluginGroup();
    std::string         getPluginName();

    std::string         getPluginExtensions();

    //void            setBasePath(QString path);

    //this is the pointer to the plugin
    OpenObjectLibPlugin*      thePlugin;

    //this is the dynamic library should be private
    QLibrary *      OpenObjectPluginLib;

private:

    std::string         pluginFamily;
    std::string         pluginGroup;
    std::string         pluginName;

    int             pluginNumber;
    OOL_PLUGIN_CLASS  thepluginClass;
    std::string         pluginExtension;

    bool            pluginLoaded;

    bool            plDebug;
};

#endif // EFFECTPLUGINS_H

