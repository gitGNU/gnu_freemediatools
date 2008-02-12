/*******************************************************************************
**
** Source file for the Jahshaka editing module - track item widget extension
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef JAH_EDIT_MLT_ITEM
#define JAH_EDIT_MLT_ITEM

#include <mlt++/Mlt.h>
#include <multitrack.h>
#include "project.h"

// The multitrack item used here
// Takes track and clip info to allow quick identification
class NleMultiTrackItem : public MultiTrackItem
{
	public:
		NleMultiTrackItem( const QString &text, MultiTrackTrack *t, int t_idx, Mlt::ClipInfo *info )
			: MultiTrackItem( text, t )
			, m_t_idx( t_idx )
			, m_info( info ) 
		{ 
			setTime( clipStart( ) );
			setLength( clipDuration( ) );
		}

		virtual ~NleMultiTrackItem( )
		{
			delete_clip_info( m_info );
		}

		int trackIndex( ) const { return m_t_idx; }
		int clipIndex( ) const { return m_info->clip; }
		int clipStart( ) const { return m_info->start; }
		int clipDuration( ) const { return m_info->frame_count; }
		int clipEnd( ) const { return m_info->start + m_info->frame_count; }

	private:
		int m_t_idx;
		Mlt::ClipInfo *m_info;
};

#endif
