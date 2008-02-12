
// sg - A scene graph representation.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef CONFIG_INC_
#define CONFIG_INC_

#ifdef WIN32
#	ifdef SG_EXPORTS
#		define SG_DECLSPEC __declspec( dllexport )
#	else
#		define SG_DECLSPEC __declspec( dllimport )
#	endif
#else
#	ifdef SG_EXPORTS
#		define SG_DECLSPEC __attribute__( ( visibility( "default" ) ) )
#	else
#		define SG_DECLSPEC __attribute__( ( visibility( "default" ) ) )
#	endif
#endif

#ifndef OPENOBJECTLIB_SGLIBNAME
#	define OPENOBJECTLIB_SGLIBNAME "openobjectlib_sg"
#endif

#ifndef OPENOBJECTLIB_TOOLSET
#	if defined( _MSC_VER ) && ( _MSC_VER == 1310 ) && !defined( __ICL )
#		define OPENOBJECTLIB_TOOLSET "vc71"

#	elif defined( _MSC_VER ) && ( _MSC_VER >= 1400 )
#		define OPENOBJECTLIB_TOOLSET "vc80"

#	elif defined( __ICL ) && ( __INTEL_COMPILER >= 900 )
#		define OPENOBJECTLIB_TOOLSET "iw90"
#	endif
#endif

#ifndef OPENOBJECTLIB_DEBUG_SUFFIX
#	ifndef NDEBUG
#		define OPENOBJECTLIB_DEBUG_SUFFIX "d"
#	else
#		define OPENOBJECTLIB_DEBUG_SUFFIX "r"		
#	endif
#endif

#ifndef OPENOBJECTLIB_VERSION
#	define OPENOBJECTLIB_VERSION "0_5_0"
#endif

// library search record (auto link).
#if defined( WIN32 ) && !defined( OPENOBJECTLIB_BUILD ) && !defined( OPENOBJECTLIB_PLUGIN )
#	if defined( OPENOBJECTLIB_SGLIBNAME ) && defined( OPENOBJECTLIB_TOOLSET ) && defined( OPENOBJECTLIB_DEBUG_SUFFIX ) && defined( OPENOBJECTLIB_VERSION )
#		pragma comment( lib, OPENOBJECTLIB_SGLIBNAME "-" OPENOBJECTLIB_TOOLSET "-" OPENOBJECTLIB_DEBUG_SUFFIX "-" OPENOBJECTLIB_VERSION ".lib" )
#	endif
#endif

#endif
