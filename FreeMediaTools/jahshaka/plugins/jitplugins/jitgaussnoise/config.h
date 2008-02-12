/*******************************************************************************
**
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU Limited General Public License
**
*******************************************************************************/


#ifndef JITGAUSSNOISE_CONFIG_INC_
#define JITGAUSSNOISE_CONFIG_INC_

#ifdef WIN32
	#ifdef JITGAUSSNOISE_EXPORTS
		#define JITGAUSSNOISE_API __declspec( dllexport )
	#else
		#define JITGAUSSNOISE_API __declspec( dllimport )
	#endif // JITGAUSSNOISE_EXPORTS
#else
	#define JITGAUSSNOISE_API
#endif // WIN32

#endif // JITGAUSSNOISE_CONFIG_INC_

