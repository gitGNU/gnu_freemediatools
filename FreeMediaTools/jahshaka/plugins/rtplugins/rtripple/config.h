/*******************************************************************************
**
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU Limited General Public License
**
*******************************************************************************/

#ifndef RTRIPPLE_CONFIG_INC_
#define RTRIPPLE_CONFIG_INC_

#ifdef WIN32
	#ifdef RTRIPPLE_EXPORTS
		#define RTRIPPLE_API __declspec( dllexport )
	#else
		#define RTRIPPLE_API __declspec( dllimport )
	#endif // RTRIPPLE_EXPORTS
#else
	#define RTRIPPLE_API
#endif // WIN32

#endif // RTRIPPLE_CONFIG_INC_
