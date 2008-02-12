/*******************************************************************************
**
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU Limited General Public License
**
*******************************************************************************/


#ifndef VFXSWIRL_CONFIG_INC_
#define VFXSWIRL_CONFIG_INC_

#ifdef WIN32
	#ifdef VFXSWIRL_EXPORTS
		#define VFXSWIRL_API __declspec( dllexport )
	#else
		#define VFXSWIRL_API __declspec( dllimport )
	#endif // VFXSWIRL_EXPORTS
#else
	#define VFXSWIRL_API
#endif // WIN32

#endif // VFXSWIRL_CONFIG_INC_
