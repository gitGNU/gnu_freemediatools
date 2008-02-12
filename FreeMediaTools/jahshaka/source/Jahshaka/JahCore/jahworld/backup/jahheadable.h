/*******************************************************************************
**
** The header file for the Jahshaka video headable interface
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef JAH_HEADABLE_
#define JAH_HEADABLE_

class JahHeadable
{
	public:
		virtual ~JahHeadable( ) { }
		virtual void headRender( int width = 0, int height = 0 ) = 0;
};

#endif
