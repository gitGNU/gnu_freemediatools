
// openpluginlib - A plugin interface to openlibraries.

// Copyright (c) 2007 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef SHLIB_INC_
#define SHLIB_INC_

namespace olib { namespace openpluginlib {

#ifdef WIN32
typedef HMODULE module_t;
#elif defined __APPLE__
typedef CFBundleRef module_t;
#else
typedef void* module_t;
#endif

module_t dlopen_( const char* path );
void* dlsym_( module_t shared, const char* entry_point );
void dlclose_( module_t module );

} }

#endif
