
// DAE - A Collada plugin to sg.

// Copyright (C) 2005-2006 VM Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef DAE_CONFIG_INC_
#define DAE_CONFIG_INC_

#ifdef WIN32
#	ifdef DAE_EXPORTS
#		define DAE_DECLSPEC __declspec( dllexport )
#	else
#		define DAE_DECLSPEC __declspec( dllimport )
#	endif
#else
#	define DAE_DECLSPEC
#endif

#endif
