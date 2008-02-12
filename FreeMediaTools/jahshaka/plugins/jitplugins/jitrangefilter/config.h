/*******************************************************************************
**
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU Limited General Public License
**
*******************************************************************************/


#ifndef JITRANGEFILTER_CONFIG_INC_
#define JITRANGEFILTER_CONFIG_INC_

#ifdef WIN32
	#ifdef JITRANGEFILTER_EXPORTS
		#define JITRANGEFILTER_API __declspec( dllexport )
	#else
		#define JITRANGEFILTER_API __declspec( dllimport )
	#endif // JITRANGEFILTER_EXPORTS
#else
	#define JITRANGEFILTER_API
#endif // WIN32

#endif // JITRANGEFILTER_CONFIG_INC_
