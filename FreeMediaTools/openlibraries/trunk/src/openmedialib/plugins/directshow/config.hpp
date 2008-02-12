// config.hpp for filesystem storage plugin
//
// Copyright (C) 2005-2006 VM Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef FILESYSTEM_STORAGE_PLUGIN_CONFIG_INC_
#define FILESYSTEM_STORAGE_PLUGIN_CONFIG_INC_

#ifdef WIN32
#	ifdef FILESYSTEM_STORAGE_PLUGIN_EXPORTS
#		define AL_FILESYSTEM_DECLSPEC __declspec( dllexport )
#	else
#		define AL_FILESYSTEM_DECLSPEC __declspec( dllimport )
#	endif
#else
#	ifdef FILESYSTEM_STORAGE_PLUGIN_EXPORTS
#		define AL_FILESYSTEM_DECLSPEC extern
#	else
#		define AL_FILESYSTEM_DECLSPEC
#	endif
#endif

#endif
