/*******************************************************************************
**
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU Limited General Public License
**
*******************************************************************************/


#ifndef RFXNVCHANNELBLUR_CONFIG_INC_
#define RFXNVCHANNELBLUR_CONFIG_INC_

#ifdef WIN32
	#ifdef RFXNVCHANNELBLUR_EXPORTS
		#define RFXNVCHANNELBLUR_API __declspec( dllexport )
	#else
		#define RFXNVCHANNELBLUR_API __declspec( dllimport )
	#endif // RFXNVCHANNELBLUR_EXPORTS
#else
	#define RFXNVCHANNELBLUR_API
#endif // WIN32

#endif // RFXNVCHANNELBLUR_CONFIG_INC_
