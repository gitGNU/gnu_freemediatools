
// OBJ - An OBJ plugin to sg.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef OBJ_CONFIG_INC_
#define OBJ_CONFIG_INC_

#ifdef WIN32
#	ifdef OBJ_EXPORTS
#		define OBJ_DECLSPEC __declspec( dllexport )
#	else
#		define OBJ_DECLSPEC __declspec( dllimport )
#	endif
#else
#	ifdef OBJ_EXPORTS
#		define OBJ_DECLSPEC __attribute__( ( visibility( "default" ) ) )
#	else
#		define OBJ_DECLSPEC __attribute__( ( visibility( "default" ) ) )
#	endif
#endif

#endif
