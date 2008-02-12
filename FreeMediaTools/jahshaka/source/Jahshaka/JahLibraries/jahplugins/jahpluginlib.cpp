/*******************************************************************************
**
** The header file for the Jahshaka jahpreferences object 
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "qdir.h"
#include "jahpreferences.h"
#include "jahplugins.h"
#include "jahpluginlib.h"

class JahPluginLib::JahPluginLibDestroyer
{
private:
	JahPluginLib* _pInstance;
public:
	JahPluginLibDestroyer()						throw()	{	_pInstance = NULL;		};
	~JahPluginLibDestroyer()					throw()	{	delete _pInstance;		};
	void SetInstance(JahPluginLib* pInstance)	throw()	{	_pInstance = pInstance;	};
};

/////////////////////////////////////////////////////////////////////
//we should make this routine save the prefs and load the prefs in 
//to make things a bit cleane rmon

JahPluginLib* JahPluginLib::_instance=0 ;
JahPluginLib::JahPluginLibDestroyer JahPluginLib::_destroyer;

JahPluginLib::JahPluginLib()
    throw()
	:	vfxPlugins(NULL)
{   
	//should we get rid of this?
	//not sure how a singleton using a singleton works...
	//maybe just initialize it in the calling routines along?
    jtrace = JahTrace::getInstance();	//set up tracer
}

JahPluginLib::~JahPluginLib() 
    throw()
{
	delete vfxPlugins;
}

JahPluginLib* JahPluginLib::getInstance()
    throw()
{
    if (_instance == 0)  // is it the first call?
    {  
    _instance = new JahPluginLib; // create sole instance
    _destroyer.SetInstance(_instance);
    }
    return _instance; // address of sole instance
}


/////////////////////////////////////////////////////////////////
//routines to set data start here

void JahPluginLib::initializePlugins(void)
    throw()
{
    //jtrace->info( ">in initializePlugins");

    QString JahBasePath;
    //initalize the global vars from the prefs
    JahPrefs& jprefs = JahPrefs::getInstance();
    JahBasePath  = jprefs.getBasePath().data();

    ///////////////////////////////////////////////////////////////////////////
    //this can be done in a loop through all plugins in the plugin directory

    // 1. search plugin directory for plugins
    // 2. load all plugins
    // 3. query plugins for name using string functionality
    // 4. add plugins to plugin list

    // to be added later on
    // 5. query plugins for ui settings
    // 6. update ui settings when plugins are selected...

    //////////////////////////////////////////////////////////////////////////
    //initialize linked list and counter
    vfxPlugins = new QPtrList<JahPluginListElement>;
    vfxPlugins->setAutoDelete( TRUE );     // delete items when they are removed

    vfxPluginsCounter = 0;

    //////////////////////////////////////////////////////////////////////////
    //this is to be changed into a loop after scanning plugin directory 
    QString jahpluginpath = JahBasePath+"plugins/";

    //the .so is for linux only...
    //windows plugins are .dll
    QString plExt;

    #ifdef WIN32
    plExt = ".dll";
    #else
    plExt = ".so";
    #endif

    #ifdef __APPLE__
    plExt = ".dylib";
    #endif

    ///////////////////////////////////////////////////////////////////
    //we iterate through all the folders in the plugins directory
    //and scan each folder for plugins
    //this can be cleaned up now that it works
    QString pluginfilename,loadfile;
    QString converter;

    QDir dd(jahpluginpath);
    if (!dd.exists())
        return;

    const QFileInfoList *dlist = dd.entryInfoList();
    QFileInfoListIterator dit( *dlist );
    QFileInfo *dfi;

    while ( (dfi = dit.current()) != 0 )
    {
        if (dfi->isDir() && (dfi->fileName() != ".."))
        {
			//enter dir
            QString thedir = jahpluginpath+dfi->fileName();
            QDir pldir(thedir);

            const QFileInfoList *list = pldir.entryInfoList();
            QFileInfoListIterator it( *list );
            QFileInfo *fi;

            //scan dir for plugins
            while ( (fi = it.current()) != 0 )
            {
                pluginfilename = fi->fileName();

                //if (pluginfilename.endsWith(".so"))
                if (pluginfilename.endsWith(plExt))
                {
                    //got a plugin, lets load it...
                    converter = converter.setNum(  fi->size()  );

                    loadfile = thedir + "/" + pluginfilename;

                    //first we create the plugin object in the list
                    //if its not already there...
                    if ( !vfxPlugins->at(vfxPluginsCounter) )
                    {
                        vfxPlugins->append( new JahPluginListElement() );
                    }

                    //then we load the plugin into the plugin object in the list
                    //debug("loading:"+loadfile);

                    if (vfxPlugins->at(vfxPluginsCounter)->loadPlugin(loadfile))
                    {
                        QString full_path = thedir + "/";
                        
                        vfxPlugins->at(vfxPluginsCounter)->setBasePath(thedir+"/");
                        
                        //if the plugin loaded we increment so we can add more
                        if ( vfxPlugins->at(vfxPluginsCounter)->initPluginStatus() )
                        {
                            //we let the plugin know where it is so it can look for necessary files
                            //currently we pass over the base path only (main root)
                            //we can also pass over the plugin directory itself if we like
                            vfxPlugins->at(vfxPluginsCounter)->setBasePath(thedir+"/");

                            vfxPluginsCounter +=1;
                        }
                        else
                        {
                            //it loaded but it wont run so get rid of it
                            //unload plugin here...
                            vfxPlugins->at(vfxPluginsCounter)->unloadPlugin();
                            
                            vfxPlugins->remove(vfxPluginsCounter);
                            jtrace->debug("Plugin chose not to load");

                        }
                    }
                    else
                    {
                        //didnt load remove the object
                        vfxPlugins->remove(vfxPluginsCounter);
                        jtrace->debug("Plugin was unable to load");
                    }
                }
                ++it;
            }
        }
        ++dit;
    }
    //jtrace->info( ">finished initializePlugins");

}

///////////////////////////////////////////////////////////////
// currently we search by name and class
// we should add family to this as well later on

int JahPluginLib::getPluginId(QString name, int theclass)
    throw()
{
    int theid = -1;

    QString rere;
    //debug("Looking for:"+name+" Class:"+rere.setNum(theclass));

    for (int x=0; x<vfxPluginsCounter; x++)
    {
        QString plname  = vfxPlugins->at(x)->getPluginName();
        int plclass     = vfxPlugins->at(x)->getPluginClass();

        if ((plname == name) && (plclass = theclass))
        {
            //debug("Found:"+name+" Class:"+rere.setNum(theclass)+" id:"+rere.setNum(x));
            theid = x;
			break;
        }
    }
    return theid;
}

int
JahPluginLib::getPluginId(std::string& guid)
{
    QString tmp;
    return ( getPluginId( tmp = QString( guid.data() ) ) );
}

int 
JahPluginLib::getPluginId(QString& guid)
{
    for (int i = 0; i < vfxPluginsCounter; i++)
    {
        if ( guid == vfxPlugins->at(i)->getGuid() )
        {
            return i;
        }
    }

    return -1;
}
