/*******************************************************************************
**
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU Limited General Public License
**
*******************************************************************************/


#ifndef JITRGBVARY_CONFIG_INC_
#define JITRGBVARY_CONFIG_INC_

#ifdef WIN32
	#ifdef JITRGBVARY_EXPORTS
		#define JITRGBVARY_API __declspec( dllexport )
	#else
		#define JITRGBVARY_API __declspec( dllimport )
	#endif // JITRGBVARY_EXPORTS
#else
	#define JITRGBVARY_API
#endif // WIN32

#endif // JITRGBVARY_CONFIG_INC_
