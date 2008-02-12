/*******************************************************************************
**
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU Limited General Public License
**
*******************************************************************************/


#ifndef VFXPOLARWARP_CONFIG_INC_
#define VFXPOLARWARP_CONFIG_INC_

#ifdef WIN32
	#ifdef VFXPOLARWARP_EXPORTS
		#define VFXPOLARWARP_API __declspec( dllexport )
	#else
		#define VFXPOLARWARP_API __declspec( dllimport )
	#endif // VFXPOLARWARP_EXPORTS
#else
	#define VFXPOLARWARP_API
#endif // WIN32

#endif // VFXPOLARWARP_CONFIG_INC_
