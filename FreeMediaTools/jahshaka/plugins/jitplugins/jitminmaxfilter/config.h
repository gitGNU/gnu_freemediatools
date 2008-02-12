/*******************************************************************************
**
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU Limited General Public License
**
*******************************************************************************/


#ifndef JITMINMAXFILTER_CONFIG_INC_
#define JITMINMAXFILTER_CONFIG_INC_

#ifdef WIN32
	#ifdef JITMINMAXFILTER_EXPORTS
		#define JITMINMAXFILTER_API __declspec( dllexport )
	#else
		#define JITMINMAXFILTER_API __declspec( dllimport )
	#endif // JITMINMAXFILTER_EXPORTS
#else
	#define JITMINMAXFILTER_API
#endif // WIN32

#endif // JITMINMAXFILTER_CONFIG_INC_
