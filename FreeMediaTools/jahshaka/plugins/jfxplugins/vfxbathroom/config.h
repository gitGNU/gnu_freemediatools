/*******************************************************************************
**
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU Limited General Public License
**
*******************************************************************************/

#ifndef VFXBATHROOM_CONFIG_INC_
#define VFXBATHROOM_CONFIG_INC_

#ifdef WIN32
	#ifdef VFXBATHROOM_EXPORTS
		#define VFXBATHROOM_API __declspec( dllexport )
	#else
		#define VFXBATHROOM_API __declspec( dllimport )
	#endif // VFXBATHROOM_EXPORTS
#else
	#define VFXBATHROOM_API
#endif // WIN32

#endif // VFXBATHROOM_CONFIG_INC_
