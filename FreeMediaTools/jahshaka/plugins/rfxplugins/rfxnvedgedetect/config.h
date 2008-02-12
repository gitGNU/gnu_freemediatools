/*******************************************************************************
**
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU Limited General Public License
**
*******************************************************************************/


#ifndef RFXNVEDGEDETECT_CONFIG_INC_
#define RFXNVEDGEDETECT_CONFIG_INC_

#ifdef WIN32
	#ifdef RFXNVEDGEDETECT_EXPORTS
		#define RFXNVEDGEDETECT_API __declspec( dllexport )
	#else
		#define RFXNVEDGEDETECT_API __declspec( dllimport )
	#endif // RFXNVEDGEDETECT_EXPORTS
#else
	#define RFXNVEDGEDETECT_API
#endif // WIN32

#endif // RFXNVEDGEDETECT_CONFIG_INC_
