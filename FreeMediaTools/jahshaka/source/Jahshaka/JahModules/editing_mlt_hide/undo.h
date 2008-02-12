/*******************************************************************************
**
** Source file for the Jahshaka editing module - undo functionality
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef JAH_EDIT_MLT_UNDO
#define JAH_EDIT_MLT_UNDO

#include <qstring.h>

// Undo state holder
class UndoState
{
	public:
		UndoState( const char *doc, int position ) :
			m_doc( doc ),
			m_position( position )
		{
		}

		QString &getDoc( ) { return m_doc; }
		int getPosition( ) { return m_position; }
		void setPosition( int position ) { m_position = position; }

	private:
		QString m_doc;
		int m_position;
};

#endif

