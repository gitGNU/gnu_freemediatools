/*******************************************************************************
**
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU Limited General Public License
**
*******************************************************************************/


#ifndef RFXNVCHARCOAL_CONFIG_INC_
#define RFXNVCHARCOAL_CONFIG_INC_

#ifdef WIN32
	#ifdef RFXNVCHARCOAL_EXPORTS
		#define RFXNVCHARCOAL_API __declspec( dllexport )
	#else
		#define RFXNVCHARCOAL_API __declspec( dllimport )
	#endif // RFXNVCHARCOAL_EXPORTS
#else
	#define RFXNVCHARCOAL_API
#endif // WIN32

#endif // RFXNVCHARCOAL_CONFIG_INC_
