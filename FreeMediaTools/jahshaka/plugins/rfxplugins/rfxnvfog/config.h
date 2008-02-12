/*******************************************************************************
**
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU Limited General Public License
**
*******************************************************************************/


#ifndef RFXNVFOG_CONFIG_INC_
#define RFXNVFOG_CONFIG_INC_

#ifdef WIN32
	#ifdef RFXNVFOG_EXPORTS
		#define RFXNVFOG_API __declspec( dllexport )
	#else
		#define RFXNVFOG_API __declspec( dllimport )
	#endif // RFXNVFOG_EXPORTS
#else
	#define RFXNVFOG_API
#endif // WIN32

#endif // RFXNVFOG_CONFIG_INC_
