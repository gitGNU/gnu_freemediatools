/*******************************************************************************
**
** The header file for the Jahshaka/MLT editing module base class
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef EDIT_BASE_H
#define EDIT_BASE_H

namespace Mlt
{
	class Frame;
}

class GLCore;
class QString;

class EditBase
{
	public:
		virtual ~EditBase( ) { }
		virtual void updateUI( ) = 0;
		virtual void updateSlider( int ) = 0;
		virtual void updatePosition( int ) = 0;
		virtual void checkPoint( ) = 0;
		virtual bool isControlled( ) = 0;
		virtual void consumerFrameShow( Mlt::Frame & ) = 0;
		virtual GLCore *getCore( ) = 0;
		virtual void glworldUpdate( ) = 0;
		virtual const QString &getMediaPath( ) = 0;
};

#endif
