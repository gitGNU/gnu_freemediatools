/*******************************************************************************
**
** The source file for the Jahshaka openobject library
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

// System Includes
#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

#include <qapplication.h>
#include <qmessagebox.h>

// Own Include
#include "openobject.h"

#ifdef NEWOPENOBJECTLIB

#include <boost/filesystem/path.hpp>

#ifdef HAVE_CONFIG_H
#include <openlibraries_global_config.hpp>
#endif

#include <openpluginlib/pl/openpluginlib.hpp>
#include <openpluginlib/pl/utf8_utils.hpp>

#include <openobjectlib/sg/openobjectlib_plugin.hpp>

namespace opl = olib::openpluginlib;
namespace fs  = boost::filesystem;
namespace sg  = olib::openobjectlib::sg;

#ifndef WIN32
const opl::string oil_plugin_path( OPENIMAGELIB_PLUGINS );
const opl::string oel_plugin_path( OPENEFFECTSLIB_PLUGINS );
const opl::string oml_plugin_path( OPENMEDIALIB_PLUGINS );
const opl::string ool_plugin_path( OPENOBJECTLIB_PLUGINS );
#endif

#else
// Other Includes
#include "openobjectlib.h"
#endif

namespace
{
#ifdef NEWOPENOBJECTLIB
	struct query_traits
	{
		query_traits( const opl::wstring& filename )
			: filename_( filename )
		{ }
		
		opl::wstring libname( ) const
		{ return L"openobjectlib"; }
		
		opl::wstring to_match( ) const
		{ return filename_; }

		opl::wstring type( ) const
		{ return L""; }
		
		int merit( ) const
		{ return 0; }
		
		opl::wstring filename_;
	};


class mydiscovery
// :public opl::discovery<myquery_traits>
{
public:
  mydiscovery(const query_traits &)
  {

  };

  bool  empty ()
  {
    return true;
  }
  size_t size ()
  {
    return 0;
  }
};


	sg::scene_ptr load_scene( const fs::path& path )
	{
	
	  //mydiscovery ool_discovery;
	
		mydiscovery  plugins( query_traits( opl::to_wstring( path.native_file_string( ).c_str( ) ) ) );
		if( plugins.empty( ) )
			return sg::scene_ptr( );
	

		#ifdef USE_DISCO

		ool_discovery::const_iterator i = plugins.begin( );
		boost::shared_ptr<sg::openobjectlib_plugin> plug = boost::shared_static_cast<sg::openobjectlib_plugin>( i->create_plugin( "" ) );
		if( !plug )
			return sg::scene_ptr( );
	
		return plug->load( path );
#else

		return sg::scene_ptr( );
#endif 

	}
#endif
}

////////////////////////////////////////
// constructors and destructors

objectData::objectData(void)
    : r_( sg::create_hw_GL_renderer( ) )
{
    objectopen		= false;

#ifdef NEWOPENOBJECTLIB
#else	
	//////////////////////////////
	//object settings
    smoothing_angle = 90.0;	             // smoothing angle
    material_mode   = 0;
    facet_normal    = false;
#endif

	//move most of the above into initalizing routine
	//that polls plugins for object type support
	//and tracks the data for reuse
	//initializeEngine();
}

objectData::~objectData(void)
{
}

////////////////////////////////////////
// main code only uses AVI for now

void  objectData::initializeEngine(void)
{
#ifdef NEWOPENOBJECTLIB
#else
	pluginId		= -1;

	oolib = OpenObjectLib::getInstance();

	//see whats available
	//oolib->listPlugins();
#endif
}

//make thePlugin private
//so we cant do this directly but only via openobjectlib

void  objectData::updateSettings(void)
{
#ifdef NEWOPENOBJECTLIB
#else
	oolib->setSmoothingAngle(pluginId,objNumber,smoothing_angle);
	oolib->setMaterialMode	(pluginId,objNumber,material_mode);
	oolib->setFacetNormal	(pluginId,objNumber,facet_normal);
#endif
}

///////////////////////////////////////////
//successfully opening a object is really 
//adding a object and should be rename to addObject

bool objectData::openObject(std::string const & filename) //called by JahRender::loadOBJ( void )
{
	QApplication::setOverrideCursor( Qt::waitCursor );
	scene_ = load_scene( fs::path( filename, fs::native ) );
	QApplication::restoreOverrideCursor( );
	
	if( !scene_ )
		QMessageBox::information( 0, "OpenObjectLib 3D scene importer",
					"The scene you're trying to load could not be parsed correctly.\n" 
					"Either is in a format that is not supported or may contain illegal data for the particular format.\n"
					"If you feel it is a bug please report it at bugs.jahshaka.org (include the model if you can)." );

	return scene_ && scene_->get_root( ) ? true : false;
}

//////////////////////////////////////////
// only draw if  aobject is open
void  objectData::drawObject()
{
	if( scene_ )
	{
		sg::spatial_ptr root = scene_->get_root( );
		if( root )	
			r_->render( root );
	}
}

bool objectData::closeObject(void)
{
	return false;
}

//////////////////////////////////////////////////////
//dont think we need this here
//as the oolib should manage this on its own

bool  objectData::isObjectOpen(void)
{
	return objectopen;
}

///////////////////////////////////////////////
// get the extension here, this needs a proper cleaning up
// as it should be checking against supported extensions string

bool  objectData::getExtension(std::string const &, std::string &)
{
	return false;
}

///////////////////////////////////////////////
// utility routines
bool  objectData::validateFile(std::string const &)
{
	return false;
}

std::string objectData::getFileName()
{
	return "";
}

///////////////////////////////////////////////
//useful for debugging but not hooked up
void  objectData::printinfo(void)
{
    if (objectopen)
    {
		//oolib->oolibPlugins->at(pluginId)->thePlugin->printinfo();
	}
}
