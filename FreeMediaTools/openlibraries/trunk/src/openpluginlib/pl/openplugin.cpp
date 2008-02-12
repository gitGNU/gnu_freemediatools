
// openpluginlib - A plugin interface to openlibraries.

// Copyright (C) 2005-2007 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#include <algorithm>
#include <functional>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <dlfcn.h>
#endif // WIN32

#include <boost/filesystem/path.hpp>

#include <openpluginlib/pl/openplugin.hpp>
#include <openpluginlib/pl/utf8_utils.hpp>

namespace fs = boost::filesystem;

namespace olib { namespace openpluginlib { 

namespace detail {

namespace
{
#ifdef WIN32
	HMODULE dlopen_( const boost::filesystem::path& path )
#else
	void* dlopen_( const boost::filesystem::path& path )
#endif
	{
#ifdef WIN32
		return LoadLibrary( to_wstring( path.native_file_string( ).c_str( ) ).c_str( ) );
#else
		// CY: It is essential that RTLD_GLOBAL is used here for dynamic_cast to be
		// functional. All g++ apps should also link with -Wl,-E.
		return dlopen( path.native_file_string( ).c_str( ), RTLD_GLOBAL | RTLD_NOW );
#endif
	}
	
#ifdef WIN32
	void* dlsym_( HMODULE shared, const char* entry_point )
#else
	void* dlsym_( void* shared, const char* entry_point )
#endif
	{
#ifdef WIN32
		return GetProcAddress( shared, entry_point );
#else
		return dlsym( shared, entry_point );
#endif
	}

#ifdef WIN32
	bool dlclose_( HMODULE shared )
#else
	bool dlclose_( void* shared )
#endif
	{
#ifdef WIN32
		return FreeLibrary( shared ) == TRUE;
#else
		return dlclose( shared ) == 0;
#endif
	}

	bool resolve_plugin_symbols( plugin_resolver& resolver )
	{
		resolver.init			= ( openplugin_init )			dlsym_( resolver.dl_handle, "openplugin_init" );
		resolver.uninit			= ( openplugin_uninit )			dlsym_( resolver.dl_handle, "openplugin_uninit" );
		resolver.create_plugin	= ( openplugin_create_plugin )	dlsym_( resolver.dl_handle, "openplugin_create_plugin" );
		resolver.destroy_plugin = ( openplugin_destroy_plugin ) dlsym_( resolver.dl_handle, "openplugin_destroy_plugin" );

		if( !resolver.init || !resolver.uninit || !resolver.create_plugin || !resolver.destroy_plugin )
			return false;
			
		return true;
	}
}

plugin_resolver::plugin_resolver( )
	: init( 0 )
	, uninit( 0 )
	, create_plugin( 0 )
	, destroy_plugin( 0 )
	, dl_handle( NULL )
	, dlopened( false )
{ }

plugin_resolver::~plugin_resolver( )
{
}

bool acquire_shared_symbols( plugin_resolver& resolver, const std::vector<wstring>& shared_name )
{
	typedef std::vector<wstring>::const_iterator const_iterator;
	
	for( const_iterator I = shared_name.begin( ); I != shared_name.end( ); ++I )
	{
		resolver.dl_handle = dlopen_( fs::path( to_string( *I ).c_str( ), fs::native ) );
		if( resolver.dl_handle )
		{
			if( !resolve_plugin_symbols( resolver ) ) continue;

			resolver.dlopened = true;
			break;
		}
	}

	return resolver.dlopened;
}

void release_shared_symbols( plugin_resolver& resolver )
{
	if( resolver.dlopened )
	{
		if( dlclose_( resolver.dl_handle ) )
			resolver.dlopened = false;
	}
}

}

openplugin::~openplugin( )
{ }




} }


