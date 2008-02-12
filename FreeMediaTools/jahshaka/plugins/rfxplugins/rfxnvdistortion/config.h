/*******************************************************************************
**
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU Limited General Public License
**
*******************************************************************************/


#ifndef RFXNVDISTORTION_CONFIG_INC_
#define RFXNVDISTORTION_CONFIG_INC_

#ifdef WIN32
	#ifdef RFXNVDISTORTION_EXPORTS
		#define RFXNVDISTORTION_API __declspec( dllexport )
	#else
		#define RFXNVDISTORTION_API __declspec( dllimport )
	#endif // RFXNVDISTORTION_EXPORTS
#else
	#define RFXNVDISTORTION_API
#endif // WIN32

#endif // RFXNVDISTORTION_CONFIG_INC_
