/*******************************************************************************
**
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU Limited General Public License
**
*******************************************************************************/


#ifndef VFXMOSAIC_CONFIG_INC_
#define VFXMOSAIC_CONFIG_INC_

#ifdef WIN32
	#ifdef VFXMOSAIC_EXPORTS
		#define VFXMOSAIC_API __declspec( dllexport )
	#else
		#define VFXMOSAIC_API __declspec( dllimport )
	#endif // VFXMOSAIC_EXPORTS
#else
	#define VFXMOSAIC_API
#endif // WIN32

#endif // VFXMOSAIC_CONFIG_INC_
