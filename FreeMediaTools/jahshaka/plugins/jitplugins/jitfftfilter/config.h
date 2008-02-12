/*******************************************************************************
**
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU Limited General Public License
**
*******************************************************************************/


#ifndef JITFFTFILTER_CONFIG_INC_
#define JITFFTFILTER_CONFIG_INC_

#ifdef WIN32
	#ifdef JITFFTFILTER_EXPORTS
		#define JITFFTFILTER_API __declspec( dllexport )
	#else
		#define JITFFTFILTER_API __declspec( dllimport )
	#endif // JITFFTFILTER_EXPORTS
#else
	#define JITFFTFILTER_API
#endif // WIN32

#endif // JITFFTFILTER_CONFIG_INC_

