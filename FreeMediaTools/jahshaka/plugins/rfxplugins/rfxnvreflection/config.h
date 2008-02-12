/*******************************************************************************
**
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU Limited General Public License
**
*******************************************************************************/


#ifndef RFXNVREFLECTION_CONFIG_INC_
#define RFXNVREFLECTION_CONFIG_INC_

#ifdef WIN32
	#ifdef RFXNVREFLECTION_EXPORTS
		#define RFXNVREFLECTION_API __declspec( dllexport )
	#else
		#define RFXNVREFLECTION_API __declspec( dllimport )
	#endif // RFXNVREFLECTION_EXPORTS
#else
	#define RFXNVREFLECTION_API
#endif // WIN32

#endif // RFXNVREFLECTION_CONFIG_INC_
