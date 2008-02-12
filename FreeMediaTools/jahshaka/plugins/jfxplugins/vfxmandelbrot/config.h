/*******************************************************************************
**
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU Limited General Public License
**
*******************************************************************************/


#ifndef VFXMANDELBROT_CONFIG_INC_
#define VFXMANDELBROT_CONFIG_INC_

#ifdef WIN32
	#ifdef VFXMANDELBROT_EXPORTS
		#define VFXMANDELBROT_API __declspec( dllexport )
	#else
		#define VFXMANDELBROT_API __declspec( dllimport )
	#endif // VFXMANDELBROT_EXPORTS
#else
	#define VFXMANDELBROT_API
#endif // WIN32

#endif // VFXMANDELBROT_CONFIG_INC_
