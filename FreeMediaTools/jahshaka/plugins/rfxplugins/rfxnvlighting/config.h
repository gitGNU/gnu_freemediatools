/*******************************************************************************
**
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU Limited General Public License
**
*******************************************************************************/


#ifndef RFXNVLIGHTING_CONFIG_INC_
#define RFXNVLIGHTING_CONFIG_INC_

#ifdef WIN32
	#ifdef RFXNVLIGHTING_EXPORTS
		#define RFXNVLIGHTING_API __declspec( dllexport )
	#else
		#define RFXNVLIGHTING_API __declspec( dllimport )
	#endif // RFXNVLIGHTING_EXPORTS
#else
	#define RFXNVLIGHTING_API
#endif // WIN32

#endif // RFXNVLIGHTING_CONFIG_INC_
