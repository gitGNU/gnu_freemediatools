/*******************************************************************************
**
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU Limited General Public License
**
*******************************************************************************/


#ifndef JITNEGEXNOISE_CONFIG_INC_
#define JITNEGEXNOISE_CONFIG_INC_

#ifdef WIN32
	#ifdef JITNEGEXNOISE_EXPORTS
		#define JITNEGEXNOISE_API __declspec( dllexport )
	#else
		#define JITNEGEXNOISE_API __declspec( dllimport )
	#endif // JITNEGEXNOISE_EXPORTS
#else
	#define JITNEGEXNOISE_API
#endif // WIN32

#endif // JITNEGEXNOISE_CONFIG_INC_
