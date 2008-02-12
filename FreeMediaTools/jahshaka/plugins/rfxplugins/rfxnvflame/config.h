/*******************************************************************************
**
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU Limited General Public License
**
*******************************************************************************/


#ifndef RFXNVFLAME_CONFIG_INC_
#define RFXNVFLAME_CONFIG_INC_

#ifdef WIN32
	#ifdef RFXNVFLAME_EXPORTS
		#define RFXNVFLAME_API __declspec( dllexport )
	#else
		#define RFXNVFLAME_API __declspec( dllimport )
	#endif // RFXNVFLAME_EXPORTS
#else
	#define RFXNVFLAME_API
#endif // WIN32

#endif // RFXNVFLAME_CONFIG_INC_
