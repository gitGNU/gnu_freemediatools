/*******************************************************************************
**
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU Limited General Public License
**
*******************************************************************************/


#ifndef VFXFISHEYE_CONFIG_INC_
#define VFXFISHEYE_CONFIG_INC_

#ifdef WIN32
	#ifdef VFXFISHEYE_EXPORTS
		#define VFXFISHEYE_API __declspec( dllexport )
	#else
		#define VFXFISHEYE_API __declspec( dllimport )
	#endif // VFXFISHEYE_EXPORTS
#else
	#define VFXFISHEYE_API
#endif // WIN32

#endif // VFXFISHEYE_CONFIG_INC_
