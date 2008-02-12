
// DAE - An DAE plugin to sg.

// Copyright (C) 2005-2006 VM Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifdef HAVE_CONFIG_H
#include <openlibraries_global_config.hpp>
#endif

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#define _WIN32_DCOM
#include <windows.h>
#include <objbase.h>
#endif

#include <boost/thread/recursive_mutex.hpp>

#include <openobjectlib/plugins/Collada/dae_plugin.hpp>

namespace opl = olib::openpluginlib;
namespace DAE = olib::openobjectlib::plugins::dae;

namespace
{
	void reflib( int init )
	{
		static long refs = 0;
	
		assert( refs >= 0 && L"DAE_plugin::refinit: refs is negative." );
		
		if( init > 0 && ++refs == 1 )
		{
#ifdef WIN32
			CoInitializeEx( NULL, COINIT_SPEED_OVER_MEMORY | COINIT_APARTMENTTHREADED );
#endif
		}
		else if( init < 0 && --refs == 0 )
		{
#ifdef WIN32
			CoUninitialize( );
#endif
		}
	}
	
	boost::recursive_mutex mutex;
}

extern "C"
{
	DAE_DECLSPEC bool openplugin_init( void )
	{
		boost::recursive_mutex::scoped_lock lock( mutex );
		
		reflib( 1 );
		
		return true;
	}

	DAE_DECLSPEC bool openplugin_uninit( void )
	{
		boost::recursive_mutex::scoped_lock lock( mutex );

		reflib( -1 );
		
		return true;
	}

	DAE_DECLSPEC bool openplugin_create_plugin( const char*, opl::openplugin** plug )
	{
		*plug = new DAE::DAE_plugin;
		
		return true;
	}
	
	DAE_DECLSPEC void openplugin_destroy_plugin( opl::openplugin* plug )
	{ delete static_cast<DAE::DAE_plugin*>( plug ); }
}
