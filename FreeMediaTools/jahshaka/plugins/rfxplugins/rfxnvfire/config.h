/*******************************************************************************
**
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU Limited General Public License
**
*******************************************************************************/


#ifndef RFXNVFIRE_CONFIG_INC_
#define RFXNVFIRE_CONFIG_INC_

#ifdef WIN32
	#ifdef RFXNVFIRE_EXPORTS
		#define RFXNVFIRE_API __declspec( dllexport )
	#else
		#define RFXNVFIRE_API __declspec( dllimport )
	#endif // RFXNVFIRE_EXPORTS
#else
	#define RFXNVFIRE_API
#endif // WIN32

#endif // RFXNVFIRE_CONFIG_INC_
