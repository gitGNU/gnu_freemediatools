/*******************************************************************************
**
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU Limited General Public License
**
*******************************************************************************/


#ifndef RFXNVFISHEYE_CONFIG_INC_
#define RFXNVFISHEYE_CONFIG_INC_

#ifdef WIN32
	#ifdef RFXNVFISHEYE_EXPORTS
		#define RFXNVFISHEYE_API __declspec( dllexport )
	#else
		#define RFXNVFISHEYE_API __declspec( dllimport )
	#endif // RFXNVFISHEYE_EXPORTS
#else
	#define RFXNVFISHEYE_API
#endif // WIN32

#endif // RFXNVFISHEYE_CONFIG_INC_
