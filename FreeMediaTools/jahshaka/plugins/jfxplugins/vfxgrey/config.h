/*******************************************************************************
**
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU Limited General Public License
**
*******************************************************************************/


#ifndef VFXGREY_CONFIG_INC_
#define VFXGREY_CONFIG_INC_

#ifdef WIN32
	#ifdef VFXGREY_EXPORTS
		#define VFXGREY_API __declspec( dllexport )
	#else
		#define VFXGREY_API __declspec( dllimport )
	#endif // VFXGREY_EXPORTS
#else
	#define VFXGREY_API
#endif // WIN32

#endif // VFXGREY_CONFIG_INC_
