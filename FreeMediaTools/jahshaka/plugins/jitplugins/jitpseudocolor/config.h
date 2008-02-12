/*******************************************************************************
**
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU Limited General Public License
**
*******************************************************************************/


#ifndef JITPSEUDOCOLOR_CONFIG_INC_
#define JITPSEUDOCOLOR_CONFIG_INC_

#ifdef WIN32
	#ifdef JITPSEUDOCOLOR_EXPORTS
		#define JITPSEUDOCOLOR_API __declspec( dllexport )
	#else
		#define JITPSEUDOCOLOR_API __declspec( dllimport )
	#endif // JITPSEUDOCOLOR_EXPORTS
#else
	#define JITPSEUDOCOLOR_API
#endif // WIN32

#endif // JITPSEUDOCOLOR_CONFIG_INC_
