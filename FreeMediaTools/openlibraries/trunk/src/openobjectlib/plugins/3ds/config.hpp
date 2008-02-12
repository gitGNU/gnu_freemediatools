
// 3DS - An 3DS plugin to sg.

// Copyright (C) 2005-2006 VM Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef DS_CONFIG_INC_
#define DS_CONFIG_INC_

#ifdef WIN32
#	ifdef DS_EXPORTS
#		define DS_DECLSPEC __declspec( dllexport )
#	else
#		define DS_DECLSPEC __declspec( dllimport )
#	endif
#else
#	define DS_DECLSPEC
#endif

#endif
