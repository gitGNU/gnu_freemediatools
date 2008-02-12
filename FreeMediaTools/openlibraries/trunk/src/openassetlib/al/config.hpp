// config.hpp
//
// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef AL_CONFIG_INC_
#define AL_CONFIG_INC_

#ifdef WIN32
#	ifdef AL_EXPORTS
#		define AL_DECLSPEC __declspec( dllexport )
#	else
#		define AL_DECLSPEC __declspec( dllimport )
#	endif
#else
#	ifdef AL_EXPORTS
#		define AL_DECLSPEC __attribute__( ( visibility( "default" ) ) )
#	else
#		define AL_DECLSPEC __attribute__( ( visibility( "default" ) ) )
#	endif
#endif

#ifndef OPENASSETLIB_LIBNAME
#	define OPENASSETLIB_LIBNAME "openassetlib_al"
#endif

#ifndef OPENASSETLIB_TOOLSET
#	if defined( _MSC_VER ) && ( _MSC_VER == 1310 ) && !defined( __ICL )
#		define OPENASSETLIB_TOOLSET "vc71"
#	elif defined( _MSC_VER ) && ( _MSC_VER >= 1400 )
#		define OPENASSETLIB_TOOLSET "vc80"
#	elif defined( __ICL ) && ( __INTEL_COMPILER >= 900 )
#		define OPENASSETLIB_TOOLSET "iw90"
#	endif
#endif

#ifndef OPENASSETLIB_DEBUG_SUFFIX
#	ifndef NDEBUG
#		define OPENASSETLIB_DEBUG_SUFFIX "d"
#	else
#		define OPENASSETLIB_DEBUG_SUFFIX "r"		
#	endif
#endif

#ifndef OPENASSETLIB_VERSION
#	define OPENASSETLIB_VERSION "0_5_0"
#endif

// library search record (auto link).
#if defined( WIN32 ) && !defined( OPENASSETLIB_BUILD )
#	if defined( OPENASSETLIB_LIBNAME ) && defined( OPENASSETLIB_TOOLSET ) && defined( OPENASSETLIB_DEBUG_SUFFIX ) && defined( OPENASSETLIB_VERSION )
#		pragma comment( lib, OPENASSETLIB_LIBNAME "-" OPENASSETLIB_TOOLSET "-" OPENASSETLIB_DEBUG_SUFFIX "-" OPENASSETLIB_VERSION ".lib" )
#	endif
#endif

#endif

