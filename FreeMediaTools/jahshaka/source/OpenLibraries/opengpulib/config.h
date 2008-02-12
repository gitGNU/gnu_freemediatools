/*******************************************************************************
**
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/


#ifndef OPENGPULIB_CONFIG_INC_
#define OPENGPULIB_CONFIG_INC_

#ifdef WIN32
	#ifdef OPENGPULIB_EXPORTS
		#define OPENGPULIB_API __declspec( dllexport )
	#else
		#define OPENGPULIB_API __declspec( dllimport )
	#endif // OPENGPULIB_EXPORTS
#else
	#define OPENGPULIB_API
#endif // WIN32

#endif // OPENGPULIB_CONFIG_INC_
