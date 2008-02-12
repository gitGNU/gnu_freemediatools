/*******************************************************************************
**
** The header file for the Jahshaka jahpluginlib module 
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/
#ifndef JAHPLUGINLIB_H
#define JAHPLUGINLIB_H

#include    <qstring.h>
#include    <qsettings.h>
#include    <qdir.h>
#include    <qptrlist.h>

#include    "jahplugins.h"
#include    "jahpreferences.h"
#include    "jahtracer.h"

class JahPluginLib
{

    public:

        //for some reason we have to do this public on windows vc6++
        #ifdef WIN32
        ~JahPluginLib() throw() ;
        #endif

    private:

        static JahPluginLib* _instance ;

        // These are needed to ensure that the singletons destructor is called on application shutdown.
        class JahPluginLibDestroyer;
        static JahPluginLibDestroyer _destroyer;

        // Hide these 'cause this is a singleton.       
        JahPluginLib() throw() ; 

        #ifndef WIN32
        ~JahPluginLib() throw() ;    
        #endif

        JahPluginLib( const JahPluginLib&);

        JahPluginLib& operator = ( const JahPluginLib&) ;

        //JahLanguage theLanguage;

    public:
        //gets a instance of this object
        static JahPluginLib* getInstance() throw();

        void initializePlugins ( void ) throw();
        int getPluginId(QString name, int theclass) throw();
        int getPluginId(QString& guid);
        int getPluginId(std::string& guid);

        QPtrList<JahPluginListElement> *vfxPlugins;
        int vfxPluginsCounter;

		////////////////////////////////////////
		//pointer to tracer
		JahTrace			* jtrace;

};

#endif

