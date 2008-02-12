/*******************************************************************************
**
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU Limited General Public License
**
*******************************************************************************/


#ifndef RFXNVSPHERE_CONFIG_INC_
#define RFXNVSPHERE_CONFIG_INC_

#ifdef WIN32
	#ifdef RFXNVSPHERE_EXPORTS
		#define RFXNVSPHERE_API __declspec( dllexport )
	#else
		#define RFXNVSPHERE_API __declspec( dllimport )
	#endif // RFXNVSPHERE_EXPORTS
#else
	#define RFXNVSPHERE_API
#endif // WIN32

#endif // RFXNVSPHERE_CONFIG_INC_
