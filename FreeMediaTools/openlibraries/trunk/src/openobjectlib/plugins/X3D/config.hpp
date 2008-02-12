
// X3D - An X3D plugin to sg.

// Copyright (C) 2005 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef X3D_CONFIG_INC_
#define X3D_CONFIG_INC_

#ifdef WIN32
#	ifdef X3D_EXPORTS
#		define X3D_DECLSPEC __declspec( dllexport )
#	else
#		define X3D_DECLSPEC __declspec( dllimport )
#	endif
#else
#	ifdef X3D_EXPORTS
#		define X3D_DECLSPEC __attribute__( ( visibility( "default" ) ) )
#	else
#		define X3D_DECLSPEC __attribute__( ( visibility( "default" ) ) )
#	endif
#endif

#endif
