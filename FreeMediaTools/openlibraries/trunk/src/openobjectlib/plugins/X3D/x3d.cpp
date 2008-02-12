
// X3D - An X3D plugin to sg.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#define _WIN32_DCOM
#include <windows.h>
#include <objbase.h>
#endif

#include <boost/thread/recursive_mutex.hpp>

#include <openobjectlib/plugins/X3D/x3d_plugin.hpp>

namespace opl = olib::openpluginlib;
namespace X3D = olib::openobjectlib::plugins::X3D;

namespace
{
	void reflib( int init )
	{
		static long refs = 0;
	
		assert( refs >= 0 && L"X3D_plugin::refinit: refs is negative." );
		
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
	X3D_DECLSPEC bool openplugin_init( void )
	{
		boost::recursive_mutex::scoped_lock lock( mutex );
		
		reflib( 1 );
		
		return true;
	}

	X3D_DECLSPEC bool openplugin_uninit( void )
	{
		boost::recursive_mutex::scoped_lock lock( mutex );

		reflib( -1 );
		
		return true;
	}

	X3D_DECLSPEC bool openplugin_create_plugin( const char*, opl::openplugin** plug )
	{
		*plug = new X3D::X3D_plugin;
		
		return true;
	}
	
	X3D_DECLSPEC void openplugin_destroy_plugin( opl::openplugin* plug )
	{ delete static_cast<X3D::X3D_plugin*>( plug ); }
}
