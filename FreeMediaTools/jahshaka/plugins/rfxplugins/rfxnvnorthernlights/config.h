/*******************************************************************************
**
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU Limited General Public License
**
*******************************************************************************/


#ifndef RFXNVNORTHERNLIGHTS_CONFIG_INC_
#define RFXNVNORTHERNLIGHTS_CONFIG_INC_

#ifdef WIN32
	#ifdef RFXNVNORTHERNLIGHTS_EXPORTS
		#define RFXNVNORTHERNLIGHTS_API __declspec( dllexport )
	#else
		#define RFXNVNORTHERNLIGHTS_API __declspec( dllimport )
	#endif // RFXNVNORTHERNLIGHTS_EXPORTS
#else
	#define RFXNVNORTHERNLIGHTS_API
#endif // WIN32

#endif // RFXNVNORTHERNLIGHTS_CONFIG_INC_
