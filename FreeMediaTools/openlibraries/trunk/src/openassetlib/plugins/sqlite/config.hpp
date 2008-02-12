// config.hpp for sqlite3 metadata plugin

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef SQLITE3_METADATA_PLUGIN_CONFIG_INC_
#define SQLITE3_METADATA_PLUGIN_CONFIG_INC_

#ifdef WIN32
#	ifdef SQLITE_EXPORTS
#		define AL_SQLITE3_DECLSPEC __declspec( dllexport )
#	else
#		define AL_SQLITE3_DECLSPEC __declspec( dllimport )
#	endif
#else
#	ifdef SQLITE_EXPORTS
#		define AL_SQLITE3_DECLSPEC __attribute__( ( visibility( "default" ) ) )
#	else
#		define AL_SQLITE3_DECLSPEC __attribute__( ( visibility( "default" ) ) )
#	endif
#endif

#endif
