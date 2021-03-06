
// openpluginlib - A plugin interface to openlibraries.

// Copyright (C) 2005-2007 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef OPENPLUGIN_INC_
#define OPENPLUGIN_INC_

#include <string>
#include <vector>

#ifdef WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif // WIN32

#include <boost/shared_ptr.hpp>

#include <openpluginlib/pl/config.hpp>
#include <openpluginlib/pl/string.hpp>

namespace olib { namespace openpluginlib {

struct openplugin;

extern "C"
{
	typedef bool ( *openplugin_init )( );
	typedef bool ( *openplugin_uninit )( );
	typedef bool ( *openplugin_create_plugin )( const char*, openplugin** );
	typedef void ( *openplugin_destroy_plugin )( openplugin* );
}

namespace detail
{
	struct OPENPLUGINLIB_DECLSPEC plugin_resolver
	{
		explicit plugin_resolver( );
		~plugin_resolver( );

		openplugin_init				init;
		openplugin_uninit			uninit;
		openplugin_create_plugin	create_plugin;
		openplugin_destroy_plugin	destroy_plugin;
		
#	ifdef WIN32
		HMODULE	dl_handle;
#	else
		void*	dl_handle;
#	endif
		bool dlopened;		
	};

	bool acquire_shared_symbols( plugin_resolver& resolver, const std::vector<wstring>& shared_name );
	void release_shared_symbols( plugin_resolver& resolver );

	struct plugin_item
	{
		explicit plugin_item( )
			: merit( 0 )
			, context( 0 )
		{ }

		std::vector<wstring> extension, filename;
		wstring name, type, mime, category, libname, in_filter, out_filter;
		int merit;

		// 3rd party plugin API support.
		void* context;

		plugin_resolver resolver;
	};
}

struct OPENPLUGINLIB_DECLSPEC openplugin
{
	virtual ~openplugin( );
};

typedef boost::shared_ptr<openplugin> opl_ptr;

} }

#endif
