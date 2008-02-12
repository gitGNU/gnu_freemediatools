
// openpluginlib - A plugin interface to openlibraries.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifdef WIN32
#define STRICT
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <openpluginlib/py/python.hpp>
#include <openpluginlib/py/py.hpp>

namespace opl = olib::openpluginlib;

BOOST_PYTHON_MODULE( openpluginlib )
{
	opl::detail::py_geometry( );
	opl::detail::py_string( );
	opl::detail::py_openpluginlib( );
	opl::detail::py_pcos_any( );
	opl::detail::py_pcos_key( );
	opl::detail::py_pcos_observer( );
	opl::detail::py_pcos_subject( );
	opl::detail::py_pcos_property_container( );
	opl::detail::py_pcos_property( );
	opl::detail::py_pcos_visitor( );
}

#ifdef WIN32
extern "C" BOOL WINAPI DllMain( HINSTANCE hInstDLL, DWORD fdwReason, LPVOID )
{
	if( fdwReason == DLL_PROCESS_ATTACH )
		DisableThreadLibraryCalls( hInstDLL );
	
	return TRUE;
}
#endif
