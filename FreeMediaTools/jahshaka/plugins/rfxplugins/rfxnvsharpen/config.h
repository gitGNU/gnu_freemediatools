/*******************************************************************************
**
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU Limited General Public License
**
*******************************************************************************/


#ifndef RFXNVSHARPEN_CONFIG_INC_
#define RFXNVSHARPEN_CONFIG_INC_

#ifdef WIN32
	#ifdef RFXNVSHARPEN_EXPORTS
		#define RFXNVSHARPEN_API __declspec( dllexport )
	#else
		#define RFXNVSHARPEN_API __declspec( dllimport )
	#endif // RFXNVSHARPEN_EXPORTS
#else
	#define RFXNVSHARPEN_API
#endif // WIN32

#endif // RFXNVSHARPEN_CONFIG_INC_
