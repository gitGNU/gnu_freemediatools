/*******************************************************************************
**
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU Limited General Public License
**
*******************************************************************************/


#ifndef RFXNVTURBULENCE_CONFIG_INC_
#define RFXNVTURBULENCE_CONFIG_INC_

#ifdef WIN32
	#ifdef RFXNVTURBULENCE_EXPORTS
		#define RFXNVTURBULENCE_API __declspec( dllexport )
	#else
		#define RFXNVTURBULENCE_API __declspec( dllimport )
	#endif // RFXNVTURBULENCE_EXPORTS
#else
	#define RFXNVTURBULENCE_API
#endif // WIN32

#endif // RFXNVTURBULENCE_CONFIG_INC_
