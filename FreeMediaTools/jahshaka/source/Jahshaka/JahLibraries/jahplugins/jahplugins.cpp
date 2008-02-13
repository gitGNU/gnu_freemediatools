/*******************************************************************************
**
** The header file for the Jahshaka effects module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/


#include "jahplugins.h" 
#include "qlibrary.h"
#include "jahtracer.h"
#include "jahplugintemplate.h"
#include "jahpreferences.h"

typedef jahPlugin* (*myfunction)();


JahPluginListElement::JahPluginListElement( )
{
    jtrace = JahTrace::getInstance();	//set up tracer

    thePlugin     = 0;
    jahPluginLib  = 0;

    pluginLoaded  = FALSE;

    useLocalUI    = 0;
    numbersliders = 0;
    numberbuttons = 0;

    //used to debug plugin class
    plDebug = true;

};

JahPluginListElement::~JahPluginListElement( )
{
	//debug("called jahpluginlist destructor");
	jahPluginLib->unload();
    delete jahPluginLib;
}

bool  JahPluginListElement::loadPlugin(QString loadname)
{
  jtrace->debug( "JahPluginListElement","loadPlugin");

    JahPrefs& jprefs    = JahPrefs::getInstance();

    jahPluginLib = new QLibrary ( loadname);

    QString str = jahPluginLib->library(); //will set str to "mylib.dll" on Windows, and "libmylib.so" on Linux.

   //now load the plugin
    jahPluginLib->load();

    if ( !jahPluginLib->isLoaded() )
    {
        if (plDebug)
        {
            jtrace->debug(">>> error loading plugin:"+str);
        }
        return false;
    }

    //now we resolve the location and get a instance
    myfunction getThePlugin =  (myfunction) jahPluginLib->resolve("getPluginClass");

    if ( ! getThePlugin )
    {
        if (plDebug)
        {
            jtrace->debug(">>> error resolving plugin:"+str);
        }

        return false;
    }
    else
    {
        if (plDebug) 
        {
            jtrace->debug(">>> loaded plugin:"+str);
        }

        //first we grab the plugin here
        thePlugin = getThePlugin();

        //then we check to see if plugin is for the current version of jahshaka

        std::string JahVersion    = jprefs.getJahVersion().data();
        std::string PluginVersion = thePlugin->JahPluginVersion.data();

        if ( JahVersion.find( PluginVersion ) != std::string::npos )
        {
            //if it is we set up important globals are set here
            pluginName     = thePlugin->getName();
            pluginFamily   = thePlugin->getFamily();
            pluginGroup    = thePlugin->getGroup();
            thepluginClass = thePlugin->getClass();
			
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

bool JahPluginListElement::unloadPlugin()
{
    //debug("unloading plugin");

    //this is a manual unload
    if (pluginLoaded)
    {
            jahPluginLib->unload();
            pluginLoaded = false;
    }

	return true;

}


//we can get rid of this now and use the pluginLoaded bool variable...
bool JahPluginListElement::initPluginStatus()
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
bool JahPluginListElement::initializeEffectsPlugin()
{
	bool initOK = true;

    JahPrefs& jprefs    = JahPrefs::getInstance();

    numbersliders = thePlugin->getSliders();
    numberbuttons = thePlugin->getButtons();

	///////////////////////////////////////////////////////
	// custom routines for gpu plugins

	if (thepluginClass  == JAH_GPU_PLUGIN)
	{
		thePlugin->tempBasePath = jprefs.getBasePath().data();

		bool nvSupport		  = jprefs.getNVGpusupport();
	    bool arbSupport		  = jprefs.getGpusupport();

		thePlugin->nvsupport  = nvSupport;
		thePlugin->arbsupport = arbSupport;

		//if we have arb support lets send some data over
		if (arbSupport)
		{
			thePlugin->max_vp_instructions =  jprefs.getMaxVertexInstructionsARB();
			thePlugin->max_fp_instructions =  jprefs.getMaxFragmentInstructionsARB();
			
			//if the plugin supports arb lets test to see if it supports the hardware
			if (thePlugin->HASARB)
			{
				thePlugin->isComplexArbSupported();
			}
		}

		/////////////////////////////////////////////////////
		//this routine cheks to see if the plugin will be supported
		//under either arb or nv
		initOK = ( (thePlugin->HASNV && nvSupport ) || (thePlugin->HASARB && arbSupport  )) ? true : false;
        //fprintf(stderr, "thePlugin->HASARB = %d   arbSupport = %d\n", thePlugin->HASARB, arbSupport);

	}

	return initOK;

}

bool JahPluginListElement::isPluginLoaded()
{
    return pluginLoaded;
}

///////////////////////////////////////////////////////
// generic routines
QString& JahPluginListElement::getJahPluginVersion()    { return thePlugin->JahPluginVersion;   }

int JahPluginListElement::getPluginNumber()            { return pluginNumber;                  }
JAH_PLUGIN_CLASS JahPluginListElement::getPluginClass(){ return thepluginClass;                }

QString& JahPluginListElement::getPluginFamily()        { return thePlugin->pluginFamily;       }
QString& JahPluginListElement::getPluginGroup()         { return thePlugin->pluginGroup;        }
QString& JahPluginListElement::getPluginName()          { return thePlugin->pluginName;         }
QString& JahPluginListElement::getGuid()                { return thePlugin->getGuid();          }

int JahPluginListElement::getNumberSliders(void)       { return numbersliders;                 }
bool JahPluginListElement::getUseSliders(void)         { return thePlugin->usesliders;         }
QString& JahPluginListElement::getSliderLabel(int i)   { return thePlugin->sliderlabel[i];     }

int JahPluginListElement::getNumberButtons(void)       { return numberbuttons;                 }
QString& JahPluginListElement::getButton(int i)        { return thePlugin->button[i];          }
bool JahPluginListElement::getOptionStatus(int i)      { return thePlugin->getOption(i);       }

void JahPluginListElement::setOptionStatus(int i, bool val)      
{ 
    thePlugin->setOption(i,val);       
}

bool JahPluginListElement::getUseLabels(void)          { return thePlugin->uselabels;          }
int  JahPluginListElement::getNumberLabels(void)       { return thePlugin->numlabels;          }
QString& JahPluginListElement::getLabel(int i)         { return thePlugin->label[i];           }

void JahPluginListElement::setBasePath(QString path)   { thePlugin->setBasePath(path);         }


