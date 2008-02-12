/*******************************************************************************
**
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU Limited General Public License
**
*******************************************************************************/


#ifndef JITMEDIANFILTER_CONFIG_INC_
#define JITMEDIANFILTER_CONFIG_INC_

#ifdef WIN32
	#ifdef JITMEDIANFILTER_EXPORTS
		#define JITMEDIANFILTER_API __declspec( dllexport )
	#else
		#define JITMEDIANFILTER_API __declspec( dllimport )
	#endif // JITMEDIANFILTER_EXPORTS
#else
	#define JITMEDIANFILTER_API
#endif // WIN32

#endif // JITMEDIANFILTER_CONFIG_INC_
