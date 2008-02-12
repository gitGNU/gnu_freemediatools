/*******************************************************************************
**
** The header file for the Jahshaka effects plugin module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/
#ifndef EFFECTPLUGINS_H
#define EFFECTPLUGINS_H

#include "jahplugintemplate.h"
#include "jahpreferences.h"
#include "jahtracer.h"

#include "qlibrary.h" 

//for ze pluginz
class jahPlugin;
typedef jahPlugin* (*myfunction)();

///////////////////////////////////////////
// linked list to store effects
class JahPluginListElement
{
public:

    JahPluginListElement( );
    ~JahPluginListElement( );

    //main routine that load the plugin
    bool    loadPlugin(QString loadname);
	bool	unloadPlugin(void);

    //utility routine to test if plugin is loaded
    bool    initPluginStatus();
    bool    isPluginLoaded();

    //initializing plugins
    bool    initializeEffectsPlugin();


    //class data about the plugin
    QString &        getJahPluginVersion();

    int             getPluginNumber();
    JAH_PLUGIN_CLASS getPluginClass();
    QString&         getPluginFamily();
    QString&         getPluginGroup();
    QString&         getPluginName();
    QString&         getGuid();

    //ui data for local ui
    bool            getUseSliders();
    int             getNumberSliders();
    QString&        getSliderLabel(int l);

    bool            getUseLabels();
    int             getNumberLabels();
    QString&        getLabel(int l);

    int             getNumberButtons();
    QString&        getButton(int l);
    bool            getOptionStatus(int i);
    void            setOptionStatus(int i, bool val);      

    void            setBasePath(QString path);

    //this is the pointer to the plugin
    jahPlugin*      thePlugin;

    //this is the dynamic library should be private
    QLibrary *      jahPluginLib;

private:
	////////////////////////////////////////
	//pointer to tracer
	JahTrace			* jtrace;

    QString         pluginFamily;
    QString         pluginGroup;
    QString         pluginName;

    int             pluginNumber;
    JAH_PLUGIN_CLASS  thepluginClass;

    bool            useLocalUI;
    int             numbersliders;
    int             numberbuttons;

    bool            pluginLoaded;

    bool            plDebug;
};

#endif // EFFECTPLUGINS_H

