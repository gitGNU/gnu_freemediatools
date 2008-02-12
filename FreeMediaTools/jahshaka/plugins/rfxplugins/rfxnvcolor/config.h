/*******************************************************************************
**
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU Limited General Public License
**
*******************************************************************************/


#ifndef RFXNVCOLOR_CONFIG_INC_
#define RFXNVCOLOR_CONFIG_INC_

#ifdef WIN32
	#ifdef RFXNVCOLOR_EXPORTS
		#define RFXNVCOLOR_API __declspec( dllexport )
	#else
		#define RFXNVCOLOR_API __declspec( dllimport )
	#endif // RFXNVCOLOR_EXPORTS
#else
	#define RFXNVCOLOR_API
#endif // WIN32

#endif // RFXNVCOLOR_CONFIG_INC_
