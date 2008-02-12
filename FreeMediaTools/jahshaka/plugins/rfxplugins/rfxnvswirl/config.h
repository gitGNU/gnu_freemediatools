/*******************************************************************************
**
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU Limited General Public License
**
*******************************************************************************/


#ifndef RFXNVSWIRL_CONFIG_INC_
#define RFXNVSWIRL_CONFIG_INC_

#ifdef WIN32
	#ifdef RFXNVSWIRL_EXPORTS
		#define RFXNVSWIRL_API __declspec( dllexport )
	#else
		#define RFXNVSWIRL_API __declspec( dllimport )
	#endif // RFXNVSWIRL_EXPORTS
#else
	#define RFXNVSWIRL_API
#endif // WIN32

#endif // RFXNVSWIRL_CONFIG_INC_
