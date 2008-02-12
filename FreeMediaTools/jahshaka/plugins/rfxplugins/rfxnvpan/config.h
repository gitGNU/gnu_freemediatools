/*******************************************************************************
**
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU Limited General Public License
**
*******************************************************************************/


#ifndef RFXNVPAN_CONFIG_INC_
#define RFXNVPAN_CONFIG_INC_

#ifdef WIN32
	#ifdef RFXNVPAN_EXPORTS
		#define RFXNVPAN_API __declspec( dllexport )
	#else
		#define RFXNVPAN_API __declspec( dllimport )
	#endif // RFXNVPAN_EXPORTS
#else
	#define RFXNVPAN_API
#endif // WIN32

#endif // RFXNVPAN_CONFIG_INC_
