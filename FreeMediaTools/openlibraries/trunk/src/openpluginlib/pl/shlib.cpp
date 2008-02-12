
// openpluginlib - A plugin interface to openlibraries.

// Copyright (c) 2007 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifdef HAVE_CONFIG_H
#include <openlibraries_global_config.hpp>
#endif

#ifdef WIN32
#define STRICT
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#elif defined __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#else
#include <dlfcn.h>
#endif

#include <openpluginlib/pl/utf8_utils.hpp>
#include <openpluginlib/pl/shlib.hpp>

namespace olib { namespace openpluginlib {

// Code replication due to OFX. There are some
// differences in how OFX plugins are specified.
// Refactoring is left as an exercise to the reader.

module_t dlopen_( const char* path )
{
#ifdef WIN32
	return LoadLibrary( to_wstring( path ).c_str( ) );
#elif defined __APPLE__
	CFStringRef bundle_str = CFStringCreateWithCString( kCFAllocatorDefault, path, kCFStringEncodingASCII );
	CFURLRef url_ref = CFURLCreateWithFileSystemPath( kCFAllocatorDefault, bundle_str, kCFURLPOSIXPathStyle, true );
		
	CFBundleRef bundle = CFBundleCreate( kCFAllocatorDefault, url_ref );
		
	CFRelease( url_ref );
	CFRelease( bundle_str );
		
	return bundle;
#else
	return dlopen( path, RTLD_GLOBAL | RTLD_NOW );
#endif
}

void* dlsym_( module_t shared, const char* entry_point )
{
#ifdef WIN32
	return GetProcAddress( shared, entry_point );
#elif defined __APPLE__
	CFStringRef entry_str = CFStringCreateWithCString( kCFAllocatorDefault, entry_point, kCFStringEncodingASCII );

	void* entry = CFBundleGetFunctionPointerForName( shared, entry_str );
	CFRelease( entry_str );

	return entry;
#else
	return dlsym( shared, entry_point );
#endif
}

void dlclose_( module_t module )
{
#ifdef WIN32
	FreeLibrary( module );
#elif defined __APPLE__
	CFRelease( module );
#else
	dlclose( module );
#endif
}

} }
