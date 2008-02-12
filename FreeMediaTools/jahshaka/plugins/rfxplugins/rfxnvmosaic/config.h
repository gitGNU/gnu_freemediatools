/*******************************************************************************
**
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU Limited General Public License
**
*******************************************************************************/


#ifndef RFXNVMOSAIC_CONFIG_INC_
#define RFXNVMOSAIC_CONFIG_INC_

#ifdef WIN32
	#ifdef RFXNVMOSAIC_EXPORTS
		#define RFXNVMOSAIC_API __declspec( dllexport )
	#else
		#define RFXNVMOSAIC_API __declspec( dllimport )
	#endif // RFXNVMOSAIC_EXPORTS
#else
	#define RFXNVMOSAIC_API
#endif // WIN32

#endif // RFXNVMOSAIC_CONFIG_INC_
