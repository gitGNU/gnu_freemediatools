/*******************************************************************************
**
** Source file for the Jahshaka editing module - track widget extension
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef JAH_EDIT_MLT_TRACK
#define JAH_EDIT_MLT_TRACK

#include <multitrack.h>

// The multitrack track used here
// Takes track index to allow quick identification
class NleMultiTrackTrack : public MultiTrackTrack
{
	public:
		NleMultiTrackTrack( const QString &text, MultiTrack *m, int t_idx ) :
			MultiTrackTrack( text, m ),
			m_t_idx( t_idx ) { }
		virtual ~NleMultiTrackTrack( ) { }
		int trackIndex( ) const { return m_t_idx; }

	private:
		int m_t_idx;
};

#endif

