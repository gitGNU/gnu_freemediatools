
// ml - A media library representation.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef ML_CONFIG_INC_
#define ML_CONFIG_INC_

#ifdef WIN32
#	ifdef ML_PLUGIN_EXPORTS
#		define ML_PLUGIN_DECLSPEC __declspec( dllexport )
#	else
#		define ML_PLUGIN_DECLSPEC __declspec( dllimport )
#	endif
#else
#	ifdef ML_PLUGIN_EXPORTS
#		define ML_PLUGIN_DECLSPEC __attribute__( ( visibility( "default" ) ) )
#	else
#		define ML_PLUGIN_DECLSPEC __attribute__( ( visibility( "default" ) ) )
#	endif 
#endif

#ifdef WIN32
#	ifdef ML_EXPORTS
#		define ML_DECLSPEC __declspec( dllexport )
#	else
#		define ML_DECLSPEC __declspec( dllimport )
#	endif
#else
#	ifdef ML_EXPORTS
#		define ML_DECLSPEC __attribute__( ( visibility( "default" ) ) )
#	else
#		define ML_DECLSPEC __attribute__( ( visibility( "default" ) ) )
#	endif
#endif

#ifndef OPENMEDIALIB_LIBNAME
#	define OPENMEDIALIB_LIBNAME "openmedialib_ml"
#endif

#ifndef OPENMEDIALIB_TOOLSET
#	if defined( _MSC_VER ) && ( _MSC_VER == 1310 ) && !defined( __ICL )
#		define OPENMEDIALIB_TOOLSET "vc71"

#	elif defined( _MSC_VER ) && ( _MSC_VER >= 1400 )
#		define OPENMEDIALIB_TOOLSET "vc80"

#	elif defined( __ICL ) && ( __INTEL_COMPILER >= 900 )
#		define OPENMEDIALIBLIB_TOOLSET "iw90"
#	endif
#endif

#ifndef OPENMEDIALIB_DEBUG_SUFFIX
#	ifndef NDEBUG
#		define OPENMEDIALIB_DEBUG_SUFFIX "d"
#	else
#		define OPENMEDIALIB_DEBUG_SUFFIX "r"		
#	endif
#endif

#ifndef OPENMEDIALIB_VERSION
#	define OPENMEDIALIB_VERSION "0_5_0"
#endif

// library search record (auto link).
#if defined( WIN32 ) && !defined( OPENMEDIALIB_BUILD )
#	if defined( OPENMEDIALIB_LIBNAME ) && defined( OPENMEDIALIB_TOOLSET ) && defined( OPENMEDIALIB_DEBUG_SUFFIX ) && defined( OPENMEDIALIB_VERSION )
#		pragma comment( lib, OPENMEDIALIB_LIBNAME "-" OPENMEDIALIB_TOOLSET "-" OPENMEDIALIB_DEBUG_SUFFIX "-" OPENMEDIALIB_VERSION ".lib" )
#	endif
#endif

#endif
