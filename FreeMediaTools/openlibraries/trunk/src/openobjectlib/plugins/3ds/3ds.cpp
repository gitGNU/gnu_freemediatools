
// 3DS - An 3DS plugin to sg.

// Copyright (C) 2005-2006 VM Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#include <openobjectlib/plugins/3ds/3ds_plugin.hpp>

namespace opl = olib::openpluginlib;
namespace DS  = olib::openobjectlib::plugins::DS;

extern "C"
{
	DS_DECLSPEC bool openplugin_init( void )
	{
		return true;
	}

	DS_DECLSPEC bool openplugin_uninit( void )
	{
		return true;
	}

	DS_DECLSPEC bool openplugin_create_plugin( const char*, opl::openplugin** plug )
	{
		*plug = new DS::max3ds_plugin;
		
		return true;
	}
	
	DS_DECLSPEC void openplugin_destroy_plugin( opl::openplugin* plug )
	{ delete static_cast<DS::max3ds_plugin*>( plug ); }
}
