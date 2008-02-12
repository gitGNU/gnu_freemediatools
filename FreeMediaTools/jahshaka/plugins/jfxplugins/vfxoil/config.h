/*******************************************************************************
**
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU Limited General Public License
**
*******************************************************************************/


#ifndef VFXOIL_CONFIG_INC_
#define VFXOIL_CONFIG_INC_

#ifdef WIN32
	#ifdef VFXOIL_EXPORTS
		#define VFXOIL_API __declspec( dllexport )
	#else
		#define VFXOIL_API __declspec( dllimport )
	#endif // VFXOIL_EXPORTS
#else
	#define VFXOIL_API
#endif // WIN32

#endif // VFXOIL_CONFIG_INC_
