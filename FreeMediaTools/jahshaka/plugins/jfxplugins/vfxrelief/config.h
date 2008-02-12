/*******************************************************************************
**
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU Limited General Public License
**
*******************************************************************************/


#ifndef VFXRELIEF_CONFIG_INC_
#define VFXRELIEF_CONFIG_INC_

#ifdef WIN32
	#ifdef VFXRELIEF_EXPORTS
		#define VFXRELIEF_API __declspec( dllexport )
	#else
		#define VFXRELIEF_API __declspec( dllimport )
	#endif // VFXRELIEF_EXPORTS
#else
	#define VFXRELIEF_API
#endif // WIN32

#endif // VFXRELIEF_CONFIG_INC_
