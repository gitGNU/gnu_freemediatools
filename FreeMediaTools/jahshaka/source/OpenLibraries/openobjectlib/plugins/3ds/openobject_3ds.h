/*******************************************************************************
**
** The header file for the Jahshaka Openobject max plugin
** The Jahshaka Project
** Copyright (C) 2000-2004 The Jahshaka Project
** Released under the GNU General Public License
**
*******************************************************************************/
#ifndef OPENOBJECT3DS_H
#define OPENOBJECT3DS_H

// System Includes

// Other Includes
#include "../openobjectplugintemplate.h"
#include "l3ds.h"

// Forward Declarations

// dll setup
#ifdef Q_WS_WIN
# define MY_EXPORT __declspec(dllexport)
#else
# define MY_EXPORT
#endif
 
extern "C" { MY_EXPORT OpenObjectLibPlugin * getPluginClass(); }


class pluginObjectManager : public OpenObjectLibPluginManager
{
  public :

	pluginObjectManager(int const & aObjectID) : OpenObjectLibPluginManager(aObjectID) {};

	//these are specific to the plugin
	L3DS*           scene;

	//these are settings unique to each model
};

class OpenObjectPlugin : public OpenObjectLibPlugin
{

  public :

	OpenObjectPlugin();

    ~OpenObjectPlugin()
    {
		//need to iterate list and erase all allocated
		//objects and the list itself
		//should be done in header?
    } ;

	int findObjectLayer(int aObjectId);

    void   initializePlugin();

	bool    openObject(int objnum, std::string const & a_mediafile ) ;
	bool    deleteObject(int objnum);
	void    drawObject(int objnum);
};

#endif // OPENOBJECT3DS_H



