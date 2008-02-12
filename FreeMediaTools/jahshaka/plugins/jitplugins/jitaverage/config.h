/*******************************************************************************
**
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU Limited General Public License
**
*******************************************************************************/


#ifndef JITAVERAGE_CONFIG_INC_
#define JITAVERAGE_CONFIG_INC_

#ifdef WIN32
	#ifdef JITAVERAGE_EXPORTS
		#define JITAVERAGE_API __declspec( dllexport )
	#else
		#define JITAVERAGE_API __declspec( dllimport )
	#endif // JITAVERAGE_EXPORTS
#else
	#define JITAVERAGE_API
#endif // WIN32

#endif // JITAVERAGE_CONFIG_INC_

