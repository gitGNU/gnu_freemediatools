/*******************************************************************************
**
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU Limited General Public License
**
*******************************************************************************/


#ifndef RFXNVDIFFUSION_CONFIG_INC_
#define RFXNVDIFFUSION_CONFIG_INC_

#ifdef WIN32
	#ifdef RFXNVDIFFUSION_EXPORTS
		#define RFXNVDIFFUSION_API __declspec( dllexport )
	#else
		#define RFXNVDIFFUSION_API __declspec( dllimport )
	#endif // RFXNVDIFFUSION_EXPORTS
#else
	#define RFXNVDIFFUSION_API
#endif // WIN32

#endif // RFXNVDIFFUSION_CONFIG_INC_
