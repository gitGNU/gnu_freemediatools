/*******************************************************************************
**
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU Limited General Public License
**
*******************************************************************************/


#ifndef VFXPOLARNOISE_CONFIG_INC_
#define VFXPOLARNOISE_CONFIG_INC_

#ifdef WIN32
	#ifdef VFXPOLARNOISE_EXPORTS
		#define VFXPOLARNOISE_API __declspec( dllexport )
	#else
		#define VFXPOLARNOISE_API __declspec( dllimport )
	#endif // VFXPOLARNOISE_EXPORTS
#else
	#define VFXPOLARNOISE_API
#endif // WIN32

#endif // VFXPOLARNOISE_CONFIG_INC_
