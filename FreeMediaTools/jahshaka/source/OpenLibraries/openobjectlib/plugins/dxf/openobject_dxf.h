/*******************************************************************************
**
** The header file for the Jahshaka OpenObject Glm plugin 
** The Jahshaka Project
** Copyright (C) 2000-2004 The Jahshaka Project
** Released under the GNU General Public License
**
*******************************************************************************/
#ifndef OPENOBJECTDXF_H
#define OPENOBJECTDXF_H

// System Includes
#include <string>

// Other Includes
#include "../openobjectplugintemplate.h"

#ifdef WHERE_THE_HELL_DOES_THIS_COME_FROM
#include "surface3d.h"
#endif


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

	pluginObjectManager (int const & aObjectID) : OpenObjectLibPluginManager(aObjectID) {};

	//these are specific to the plugin
};



class OpenObjectPlugin : public OpenObjectLibPlugin
{
  public:

	OpenObjectPlugin();

    ~OpenObjectPlugin()
    {
		//need to iterate list and erase all allocated
		//objects and the list itself
		//should be done in header?
    } ;

	int findObjectLayer(int aObjectId);
	
    void   initializePlugin();

	bool    openObject(int objnum, std::string const & mediafile);
	bool    deleteObject(int objnum);
	void    drawObject(int objnum);

#ifdef WHERE_THE_HELL_DOES_THIS_COME_FROM
	//these are settings unique to each model
	charutilities::surface3d m_the_surface3d;
#endif
};


#endif // OPENOBJECTDXF_H




