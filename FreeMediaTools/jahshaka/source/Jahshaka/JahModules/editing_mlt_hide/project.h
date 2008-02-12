/*******************************************************************************
**
** The source file for the Jahshaka editing module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef JAHNLE_PROJECT_H
#define JAHNLE_PROJECT_H

#include <mlt++/Mlt.h>

class QComboBox;
class GLEdit;
class assetData;
class NleMultiTrackTrack;

namespace jahnle
{
	// Modes for adding new assets
	typedef enum
	{
		mode_append_track = 0,
		mode_append = 1,
		mode_append_project = 2,
		mode_insert = 3,
		mode_overwrite = 4,
		mode_overwrite_start = 5,
		mode_split_clip = 6
	}
	add_mode;

	// Modes for keyboard modifiers
	typedef enum
	{
		keyboard_none,
		keyboard_shift,
		keyboard_alt
	}
	keyboard_modifier;

	// The mlt project
	class Project : public Mlt::Tractor
	{
		public:
			// Constructors for the project (with or without and xml doc)
			Project( GLEdit *, Project * = NULL );
			Project( GLEdit *, const char *, Project * = NULL );
			Project( GLEdit *, assetData &, Project * = NULL );

			// Copy over old settings if available
			void inherit( Project * );

			// Start and end markers to ensure that mods are carried out correctly
			void start( );
			void end( );

			// Accessors for the multitrack gui object (multitrack may be NULL)
			void setActiveTrack( int index, NleMultiTrackTrack *track = NULL );
			NleMultiTrackTrack *getMultiTrack( ) { return m_multi_track; }
			int getActiveTrack( ) { return m_active_track; }

			// Add/insert/move and remove Tracks
			// NB: the MLT tractor object lacks insert/remove for tracks (for some odd reason :-/)
			// hence the new project returned should be switched into use
			void attachDefaultFilters( Mlt::Playlist & );
			void addTrack( );
			Project *insertTrack( );
			Project *removeTrack( );
			Project *moveTrackUp( );
			Project *moveTrackDown( );

			// Split the cut on the selected track at the current position or position specified
			bool split( int pos = -1 );

			// Join the cuts
			void join( );

			// Delete the cut on the selected track at the current position
			void remove( );

			// Replace the current cut with blanks
			void blank( );

			// Move the selected item on the selected track to the position specified
			void move( int clip_index, int delta );

			// Change in and out points of selected track at the current position
			void changeInPoint( int in = -1 );
			void changeOutPoint( int out = -1 );

			// Populate the add mode selection widget
			void populateAddMode( QComboBox * );

			// Add an asset
			void setAddMode( add_mode );
			bool addAsset( assetData );
			bool addClone( Mlt::Producer & );
			bool addClip( Mlt::Producer & );

			// Mix current and next cut
			void mix( int );

			// Move to next and previous cuts on the selected track
			void movePrev( );
			void moveNext( );

			// Allow debug mode to be turned on and off
			void toggleDebug( ) { m_debug = !m_debug; }

			// Indicate whether this project has a background or not
			bool hasBackground( ) { return m_has_background; }

			// Force a refresh of the project settings
			void refresh( ) { start( ); end( ); }

			// Set keyboard modifier (affects behaviour of some operations like changing in and out)
			void setKeyboardModifier( keyboard_modifier modifier ) { m_modifier = modifier; }

			// Get the current cut for copy/cut operations
			Producer *getCurrentCut( );

			// Move selected filter up or down
			void moveFilter( int direction );

		private:
			GLEdit *m_edit;
			int m_active_track;
			add_mode m_add_mode;
			NleMultiTrackTrack *m_multi_track;
			int m_starts;
			bool m_debug;
			bool m_has_background;
			keyboard_modifier m_modifier;

			// Normalise the project
			void normalise( );
	};
}

// Temporary clean up implementation
#ifdef WIN32
#	define delete_clip_info( info )		Mlt::Playlist::delete_clip_info( info )
#else
#	define delete_clip_info( info )		delete info
#endif

#ifdef WIN32
#	define delete_event( event )		Mlt::Properties::delete_event( event )
#else
#	define delete_event( event )		delete event
#endif

#endif
