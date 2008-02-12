
// OBJ - An OBJ plugin to sg.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#include <openobjectlib/plugins/wavefront/obj_plugin.hpp>

namespace opl = olib::openpluginlib;
namespace OBJ = olib::openobjectlib::plugins::OBJ;

extern "C"
{
	OBJ_DECLSPEC bool openplugin_init( void )
	{
		return true;
	}

	OBJ_DECLSPEC bool openplugin_uninit( void )
	{
		return true;
	}

	OBJ_DECLSPEC bool openplugin_create_plugin( const char*, opl::openplugin** plug )
	{
		*plug = new OBJ::OBJ_plugin;
		return true;
	}
	
	OBJ_DECLSPEC void openplugin_destroy_plugin( opl::openplugin* plug )
	{ delete static_cast<OBJ::OBJ_plugin*>( plug ); }
}
