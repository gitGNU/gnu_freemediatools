/*******************************************************************************
**
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU Limited General Public License
**
*******************************************************************************/


#ifndef RFXNVCOMPOUNDEYE_CONFIG_INC_
#define RFXNVCOMPOUNDEYE_CONFIG_INC_

#ifdef WIN32
	#ifdef RFXNVCOMPOUNDEYE_EXPORTS
		#define RFXNVCOMPOUNDEYE_API __declspec( dllexport )
	#else
		#define RFXNVCOMPOUNDEYE_API __declspec( dllimport )
	#endif // RFXNVCOMPOUNDEYE_EXPORTS
#else
	#define RFXNVCOMPOUNDEYE_API
#endif // WIN32

#endif // RFXNVCOMPOUNDEYE_CONFIG_INC_
