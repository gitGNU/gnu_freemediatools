/*******************************************************************************
**
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU Limited General Public License
**
*******************************************************************************/


#ifndef JITGEOMMEANFILTER_CONFIG_INC_
#define JITGEOMMEANFILTER_CONFIG_INC_

#ifdef WIN32
	#ifdef JITGEOMMEANFILTER_EXPORTS
		#define JITGEOMMEANFILTER_API __declspec( dllexport )
	#else
		#define JITGEOMMEANFILTER_API __declspec( dllimport )
	#endif // JITGEOMMEANFILTER_EXPORTS
#else
	#define JITGEOMMEANFILTER_API
#endif // WIN32

#endif // JITGEOMMEANFILTER_CONFIG_INC_

