/*******************************************************************************
**
** The header file for the Jahshaka jahpreferences object 
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

// System Includes


//#include <openpluginlib/pl/property.hpp>

//#include "core.hpp"

namespace olib{
  namespace openpluginlib 
  {};
};

namespace openpluginlib=olib::openpluginlib;

//typename openpluginlib::value_property<node_ptr> v_node_ptr;
//typedef openpluginlib::multi_value_property<node_ptr> m_node_ptr;


#include <qdir.h>

#include <openpluginlib/pl/discovery_traits.hpp>
#include <openpluginlib/pl/openpluginlib.hpp>

// Own Include
#include "openobjectlib.h"

#include <openpluginlib/pl/bind_info.hpp>

// Other Includes
#include <openpluginlib/pl/property.hpp>

//olib::openpluginlib::property_typename test;
#pragma warning "mike after the property"
#include <openobjectlib/sg/core.hpp>
#include <openobjectlib/sg/shaders.hpp>
#include "openobjectplugins.h"



namespace opl = olib::openpluginlib;

//typedef opl::value_property<shader_ptr>			v_shader_ptr;


#include <openpluginlib/pl/openpluginlib.hpp>
#include <openpluginlib/pl/utf8_utils.hpp>

namespace opl = olib::openpluginlib;


struct myquery_traits
{
  opl::wstring libname( ) const
  { return L"openobjectlib"; }
  
  opl::wstring to_match( ) const
  { return L""; }
  
  opl::wstring type( ) const
  { return L""; }
  
  int merit( ) const
  { return 0; }
};


//#include "jahpreferences.h"

//#include "charutilities.h"

class OpenObjectLib::OpenObjectLibDestroyer
{
private:
	OpenObjectLib* _pInstance;
public:
	OpenObjectLibDestroyer()					{	_pInstance = NULL;		};
	~OpenObjectLibDestroyer()					{	delete _pInstance;		};
	void SetInstance(OpenObjectLib* pInstance)	{	_pInstance = pInstance;	};
};

/////////////////////////////////////////////////////////////////////
//we should make this routine save the prefs and load the prefs in 
//to make things a bit cleane rmon

OpenObjectLib* OpenObjectLib::_instance=0 ;
OpenObjectLib::OpenObjectLibDestroyer OpenObjectLib::_destroyer;

OpenObjectLib::OpenObjectLib( ) :
	oolibPluginsCounter( 0 )
{   
	//set counter to zero
	objcounter = 0;
    jtrace = OL_Trace::getInstance();	//set up tracer
}

OpenObjectLib::~OpenObjectLib() 
{
}

OpenObjectLib* OpenObjectLib::getInstance()
{

    ////////////////////////////////////////////
    // for multithreded apps

    //this is safe to use with threads however you have to manually
    //destroy the object thats created with the new command
    //currently we dont do that


    if (_instance == 0)  // is it the first call?
    {  
    _instance = new OpenObjectLib; // create sole instance
    _destroyer.SetInstance(_instance);
    }
    return _instance; // address of sole instance


    ////////////////////////////////////////////
    // for singlethreded apps

    //static objects are automatically destroyed
    //but this is not multithread safe
    /*
    static JahPluginLib inst;
    return &inst;
    */
}


/////////////////////////////////////////////////////////////////
//routines to set data start here

void OpenObjectLib::initializePlugins(void)
{
}

///////////////////////////////////////////////////////////////
// currently we search by name and class
// we should add family to this as well later on

int OpenObjectLib::getPluginId(std::string /*theext*/, int /*theclass*/ )
{
	return 0;
}

//////////////////////////////////////////
//passing objnum is redundant here unless the object is already open
//thats why its being kept around

bool OpenObjectLib::createObject ( int pluginId, int objnum, std::string filename  )
{
	bool status;

	status = oolibPlugins ->at(pluginId)->thePlugin->openObject(objnum, filename);

	return status;

}

void OpenObjectLib::drawObject ( int pluginId, int objnum  ) 
{
	
	oolibPlugins->at(pluginId)->thePlugin->drawObject(objnum);
}

bool OpenObjectLib::deleteObject ( int pluginId, int objnum  ) 
{
	bool status;

	status = oolibPlugins->at(pluginId)->thePlugin->deleteObject(objnum);

	return status;
}

///////////////////////////////////////////
// utility routines here

std::string OpenObjectLib::getFormats()
{
	return std::string( "*.X3D *.x3d *.OBJ *.obj *.3DS *.3ds *.DAE *.dae" );
}

bool OpenObjectLib::getFileFormat(std::string /*filename*/, std::string & /*theExtension*/)
{	
	return false;
}

class mydiscovery
// :public opl::discovery<myquery_traits>
{
public:
  mydiscovery(const myquery_traits &)
  {

  };

  size_t size ()
  {
    return 0;
  }
};


void OpenObjectLib::listPlugins()
{
	mydiscovery plugins( ( myquery_traits( ) ) );

	QString str_num;
	str_num.setNum( plugins.size( ) );
	
	jtrace->info("Total OOlib Plugins loaded:" + str_num);
}

int OpenObjectLib::incrementObjcounter ( void ) 
{ 
	objcounter ++; 
	return objcounter; 
}

int OpenObjectLib::getObjcounter ( void ) 
{ 
	return objcounter; 
}

std :: string OpenObjectLib::getFilename ( int pluginId, int objnum )
{
	return oolibPlugins->at(pluginId)->thePlugin->getFileName(objnum);
}

//////////////////////////////////////////////////
//set flags in objects
void OpenObjectLib::setSmoothingAngle(int pluginId, int objnum, float smoothing_angle)
{
	oolibPlugins->at(pluginId)->thePlugin->setSmoothingAngle(objnum,smoothing_angle);
}

void OpenObjectLib::setMaterialMode	(int pluginId, int objnum, int material_mode)
{
	oolibPlugins->at(pluginId)->thePlugin->setMaterialMode(objnum,material_mode);
}

void OpenObjectLib::setFacetNormal		(int pluginId, int objnum, bool facet_normal)
{
	oolibPlugins->at(pluginId)->thePlugin->setFacetNormal(objnum,facet_normal);
}

////////////////////////////////////////////////////////
//from charutilities

char * OpenObjectLib::toLowercaseInPlace(char * const & ai_string)
{
	if ( ai_string )

	{
		char * pname = ai_string ;
		for ( pname = ai_string ; pname < ai_string + strlen ( ai_string ) ; pname ++ ) 

		if ( isupper ( * pname ) ) 
		{
			* pname = ( char ) tolower ( * pname ) ;
		}
	}
	return ai_string;
}

bool OpenObjectLib::checkExtension ( 
    char const * const & /*a_filename*/,
    char const * const & /*a_extension*/)
{
	return true;
}




