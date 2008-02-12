/*******************************************************************************
**
** The source file for the Jahshaka editing module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include <iostream>
#include <vector>
using namespace std;

#if defined ( __sgi )
#define __c99
#endif

#include "edit.h"
#include "project.h"
#include "preview.h"
using namespace Mlt;
using namespace jahnle;

Project::Project( GLEdit *edit, Project *old_project ) :
	Tractor( ),
	m_edit( edit ),
	m_active_track( 0 ),
	m_add_mode( mode_append_track ),
	m_multi_track( NULL ),
	m_starts( 0 ),
	m_debug( false ),
	m_has_background( false )
{
	inherit( old_project );
}

Project::Project( GLEdit *edit, const char *text, Project *old_project ) :
	Tractor( ( char * )"westley-xml", ( char * )text ),
	m_edit( edit ),
	m_active_track( 0 ),
	m_add_mode( mode_append_track ),
	m_multi_track( NULL ),
	m_starts( 0 ),
	m_debug( false ),
	m_has_background( false )
{
	inherit( old_project );
}

Project::Project( GLEdit *edit, assetData &asset, Project *old_project ) :
	Tractor( ( char * )"westley", ( char * )asset.getFullFileName( ).ascii( ) ),
	m_edit( edit ),
	m_active_track( 0 ),
	m_add_mode( mode_append_track ),
	m_multi_track( NULL ),
	m_starts( 0 ),
	m_debug( false ),
	m_has_background( false )
{
	inherit( old_project );
}

void Project::inherit( Project *old_project )
{
	// Start paused
	set_speed( 0 );
	set( "eof", "loop" );

	if ( old_project != NULL )
	{
		m_active_track = old_project->m_active_track;
		m_add_mode = old_project->m_add_mode;
		m_debug = old_project->m_debug;
	}

	// Critical functionality here - all projects now have a background track which is 
	// hidden from view - we should also be allowed to load old projects which don't 
	// have one. The background, when there, will not be shown in the UI and contains 
	// a single (transparent black) cut which stretches for the length of the project 
	// at all time - it can't be moved or selected.

	if ( count( ) == 0 )
	{
		Playlist track;
		track.set( "jah_background", 1 );
		track.set( "jah_volume", 0 );
		set_track( track, 0 );
		m_has_background = true;
	}
	else
	{
		Producer *producer = track( 0 );
		if ( producer != NULL )
			m_has_background = producer->get_int( "jah_background" );
		delete producer;
	}

	// Optimise on load
	optimise( );
}

void Project::start( )
{
	if ( m_starts ++ == 0 )
		block( );
}

void Project::end( )
{
	if ( -- m_starts == 0 )
	{
		normalise( );
		optimise( );
		unblock( );
		m_edit->checkPoint( );
		m_edit->updateUiSettings( );
	}
}

void Project::setActiveTrack( int index, NleMultiTrackTrack *track )
{
	int first_track = m_has_background ? 1 : 0;
	if ( index < first_track )
	{
		m_active_track = first_track;
		m_multi_track = NULL;
	}
	else if ( index >= count( ) )
	{
		m_active_track = count( ) - 1;
		m_multi_track = NULL;
	}
	else
	{
		m_active_track = index; 
		m_multi_track = track;
	}
}

// Normalise all tracks so that they're the same length.
void Project::normalise( )
{
	if ( count( ) > 0 )
	{
		int first_track = m_has_background ? 1 : 0;
		Playlist **tracks = new Playlist* [ count( ) ];
		int new_length = 0;
		int i = first_track;
		double total_volume = 0;

		// Find the longest (non background) track
		for ( i = 0; i < count( ); i ++ )
		{
			// Fetch track and trim all trailing blanks
			Producer *producer = track( i );
			tracks[ i ] = new Playlist( *producer );
			tracks[ i ]->consolidate_blanks( );

			// Store longest track
			if ( i >= first_track && tracks[ i ]->get_length( ) > new_length )
				new_length = tracks[ i ]->get_length( );
			else if ( i < first_track )
				tracks[ i ]->set( "length", 1 );

			// Obtain the total volume for all tracks
			if ( i >= first_track && tracks[ i ]->get( "jah_volume" ) )
				total_volume += 1.0;

			// Wipe the reference to the track producer
			delete producer;
		}

		// Remove the background (always first cut on first track)
		if ( first_track && tracks[ 0 ]->count( ) )
			tracks[ 0 ]->remove( 0 );

		// Normalise all tracks
		for ( i = first_track; i < count( ); i ++ )
		{
			if ( new_length > tracks[ i ]->get_length( ) )
				tracks[ i ]->blank( new_length - tracks[ i ]->get_length( ) - 1 );

#ifdef FIX_AUDIO_MIX
			if ( total_volume > 0 && tracks[ i ]->get( "jah_volume" ) )
				tracks[ i ]->set( "meta.volume", tracks[ i ]->get_double( "jah_volume" ) / total_volume );
#endif
		}

		// Iterate through each special case 'expandable' cut
		for ( i = first_track; i < count( ); i ++ )
		{
			Playlist track( *tracks[ i ] );

			// For each cut in track
			for ( int cut = 0; cut < track.count( ); cut ++ )
			{
				// Get the cut
				Producer *item = track.get_clip( cut );

				// Special cases for blanks and stills and fx
				if ( item->is_blank( ) )
				{
					int unused = track.blanks_from( cut, 1 );
					item->set( "_max_length", unused + new_length );
				}
				else if ( item->parent( ).get_int( "jah_image" ) )
				{
					int unused = track.blanks_from( cut + 1, 1 ) + item->get_playtime( );
					if ( i == 1 ) unused += 99;
					item->set( "length", unused );
					item->parent( ).set( "length", unused + 100 );
					item->parent( ).set( "out", unused + 99 );
					item->set( "_max_length", unused );
				}
				else if ( item->parent( ).get_int( "meta.fx_cut" ) )
				{
					int unused = track.blanks_from( cut + 1, 1 ) + item->get_playtime( );
					item->set( "length", unused );
					item->parent( ).set( "length", unused );
					item->set( "_max_length", unused );
				}

				delete item;
			}

			delete tracks[ i ];
		}

		// Now ensure that the background track is populated and has the correct length
		if ( first_track && new_length )
		{
			Preview *preview = m_edit->preview( );
			Producer background( "colour:0x00000000" );
			background.set( "length", new_length );
			background.set( "out", new_length - 1 );
			if ( preview != NULL )
			{
				preview->storeProfile( this );
				background.set( "aspect_ratio", preview->getAspectRatio( ) );
			}
			tracks[ 0 ]->append( background );
			delete tracks[ 0 ];
		}
		else if ( first_track )
		{
			delete tracks[ 0 ];
		}
		
		delete[ ] tracks;
	}
}

// Move to the start of the previous cut
void Project::movePrev( )
{
	int pos = 0;
	Producer *producer = track( getActiveTrack( ) );
	Playlist track( *producer );
	int target_index = track.get_clip_index_at( position( ) );
	if ( target_index >= track.count( ) ) target_index = track.count( ) - 1;
	if ( target_index > 0 && track.count( ) )
	{
		ClipInfo *info = track.clip_info( target_index - 1 );
		if ( position( ) > info->start + info->frame_count )
			pos = info->start + info->frame_count;
		else 
			pos = info->start;
		delete_clip_info( info );
	}
	m_edit->seek( pos );
	delete producer;
}

// Move to the start of the next cut
void Project::moveNext( )
{
	int pos = m_edit->length( ) - 1;
	Producer *producer = track( getActiveTrack( ) );
	Playlist track( *producer );
	int target_index = track.get_clip_index_at( position( ) );
	if ( target_index >= track.count( ) ) target_index = track.count( ) - 1;
	if ( target_index >= 0 && target_index < track.count( ) - 1 )
	{
		ClipInfo *info = track.clip_info( target_index );
		pos = info->start + info->frame_count;
		delete_clip_info( info );
	}
	m_edit->seek( pos );
	delete producer;
}

void Project::attachDefaultFilters( Playlist &track )
{
	Filter video_filter( "transition", "composite" );
	video_filter.set( "transition.aligned", 1 );
	video_filter.set( "transition.operator", "none" );
	video_filter.set( "transition.fill", 1 );
	video_filter.set( "transition.distort", 0 );
	video_filter.set( "transition.progressive", 1 );
	video_filter.set( "transition.valign", "centre" );
	video_filter.set( "transition.halign", "centre" );
	video_filter.set( "transition.geometry", "0,0:100%x100%" );
	track.attach( video_filter );
	Filter audio_filter( "transition", "mix" );
	audio_filter.set( "transition.combine", 1 );
	track.attach( audio_filter );
	track.set( "jah_volume", 1.0 );
}

// Add Track of a particular type
void Project::addTrack( )
{
	start( );
	Playlist track;
	attachDefaultFilters( track );
	set_track( track, count( ) );
	setActiveTrack( count( ) - 1, NULL );
	end( );
}

// Insert a track before the currently selected track
Project *Project::insertTrack( )
{
	int src = 0;
	int dest = 0;
	Playlist new_track;
	attachDefaultFilters( new_track );
	Project *new_project = new Project( m_edit, this );
	block( );
	for ( src = 0; src < count( ); src ++ )
	{
		if ( src == m_active_track )
			new_project->set_track( new_track, dest ++ );
		Producer *producer = track( src );
		new_project->set_track( *producer, dest ++ );
		delete producer;
	}
	new_project->normalise( );
	new_project->setActiveTrack( m_active_track, NULL );
	unblock( );
	return new_project;
}

// Remove the currently selected track
Project *Project::removeTrack( )
{
	Project *new_project = NULL;
	block( );
	int first_track = m_has_background ? 1 : 0;
	if ( count( ) > first_track + 1 )
	{
		int src = 0;
		int dest = 0;
		new_project = new Project( m_edit, this );
		for ( src = 0; src < count( ); src ++ )
		{
			if ( src != m_active_track )
			{
				Producer *producer = track( src );
				new_project->set_track( *producer, dest ++ );
				delete producer;
			}
		}
		new_project->normalise( );
		new_project->setActiveTrack( m_active_track - 1, NULL );
	}
	unblock( );
	return new_project;
}

Project *Project::moveTrackUp( )
{
	block( );
	Project *new_project = NULL;
	int first_track = m_has_background ? 1 : 0;
	if ( m_active_track > first_track )
	{
		int src = 0;
		int dest = 0;
		Producer *moving = track( m_active_track );
		new_project = new Project( m_edit, this );
		for ( src = 0; src < count( ); src ++ )
		{
			if ( dest == m_active_track - 1 )
				new_project->set_track( *moving, dest ++ );
			if ( src != m_active_track )
			{
				Producer *producer = track( src );
				new_project->set_track( *producer, dest ++ );
				delete producer;
			}
		}
		new_project->normalise( );
		new_project->setActiveTrack( m_active_track - 1, NULL );
		delete moving;
	}
	unblock( );
	return new_project;
}

Project *Project::moveTrackDown( )
{
	block( );
	Project *new_project = NULL;
	if ( m_active_track < count( ) - 1 )
	{
		int src = 0;
		int dest = 0;
		Producer *moving = track( m_active_track );
		new_project = new Project( m_edit, this );
		for ( src = 0; src < count( ); src ++ )
		{
			if ( src != m_active_track )
			{
				Producer *producer = track( src );
				new_project->set_track( *producer, dest ++ );
				delete producer;
			}
			if ( dest == m_active_track + 1 )
				new_project->set_track( *moving, dest ++ );
		}
		new_project->normalise( );
		new_project->setActiveTrack( m_active_track + 1, NULL );
		delete moving;
	}
	unblock( );
	return new_project;
}

// Split the cut on the selected track at the current position
bool Project::split( int pos )
{
	start( );

	// Fetch active track
	Producer *producer = track( getActiveTrack( ) );

	// Convert to a track
	Playlist track( *producer );

	// The split_at method only indicates an error, not if a split occurred
	// simple work around is to store the cuts before the split and compare after
	int count = track.count( );

	// Split at the current position
	if ( pos == -1 ) pos = position( );
	track.split_at( pos );

	// Delete the reference to the track producer
	delete producer;

	end( );

	// Indicate if a split occurred
	return track.count( ) != count;
}

void Project::join( )
{
	start( );

	// Fetch active track
	Producer *producer = track( getActiveTrack( ) );

	// Convert to a track
	Playlist track( *producer );

	// Locate and join with the clip following
	int idx = track.get_clip_index_at( position( ) );
	if ( idx < track.count( ) - 1 )
	{
		ClipInfo *ante = track.clip_info( idx );
		const char *title_ante = ante->producer->get( "jah_title" );
		ClipInfo *post = track.clip_info( idx + 1 );
		const char *title_post = post->producer->get( "jah_title" );
		track.join( idx );
		ClipInfo *real = track.clip_info( idx );
		QString title;
		if ( title_ante != NULL && title_post != NULL )
			title = QString( title_ante ) + QString( "+" ) + QString( title_post );
		else if ( title_ante != NULL )
			title = QString( title_ante );
		else if ( title_post != NULL )
			title = QString( title_post );
		else
			title = "Unknown";
		real->producer->set( "jah_title", title );
		delete_clip_info( real );
		delete_clip_info( post );
		delete_clip_info( ante );
	}

	// Delete the reference to the track producer
	delete producer;

	end( );
}

// Return a pointer to the current cut
Producer *Project::getCurrentCut( )
{
	Producer *result = NULL;

	Producer *producer = track( getActiveTrack( ) );
	Playlist track( *producer );
	int target_index = track.get_clip_index_at( position( ) );

	if ( target_index >= 0 && target_index < track.count( ) )
	{
		ClipInfo *clip = track.clip_info( target_index );
		result = clip->producer->cut( clip->frame_in, clip->frame_out );
		int i = 0;
		Filter *filter = NULL;
		do
		{
			filter = clip->cut->filter( i ++ );
			if ( filter != NULL )
			{
				if ( !filter->get_int( "_fezzik" ) )
				{
					Filter copy( filter->get( "mlt_service" ) );
					copy.inherit( *filter );
					result->attach( copy );
				}
				delete filter;
			}
		}
		while ( filter != NULL );
		delete_clip_info( clip );
	}

	delete producer;

	return result;
}

// Duplicate the clip and attached fx and add it
bool Project::addClone( Producer &clip )
{
	Producer *clone = clip.parent( ).cut( clip.get_in( ), clip.get_out( ) );
	int i = 0;
	Filter *filter = NULL;
	do
	{
		filter = clip.filter( i ++ );
		if ( filter != NULL )
		{
			if ( !filter->get_int( "_fezzik" ) )
			{
				Filter copy( filter->get( "mlt_service" ) );
				copy.inherit( *filter );
				clone->attach( copy );
			}
			delete filter;
		}
	}
	while ( filter != NULL );
	bool result = addClip( *clone );
	delete clone;
	return result;
}

static vector < Filter * > collectFilters( Producer &clip )
{
	vector < Filter * > result;
	int i = 0;
	Filter *filter = NULL;
	do
	{
		filter = clip.filter( i );
		if ( filter != NULL )
		{
			if ( !filter->get_int( "_fezzik" ) )
			{
				result.push_back( filter );
				clip.detach( *filter );
			}
			else 
			{
				delete filter;
				i ++;
			}
		}
	}
	while ( filter != NULL );
	return result;
}

static void attachFilters( Producer &clip, vector < Filter * > &filters )
{
	for ( vector< Filter * >::iterator filter = filters.begin( ); filter != filters.end( ); filter ++ )
	{
		clip.attach( **filter );
		delete *filter;
	}
}

void Project::moveFilter( int direction )
{
	start( );
	Producer *producer = track( getActiveTrack( ) );
	Playlist track( *producer );
	int target_index = track.get_clip_index_at( position( ) );
	if ( target_index >= 0 && target_index < track.count( ) )
	{
		ClipInfo *clip = track.clip_info( target_index );
		if ( !clip->cut->is_blank( ) && clip->cut->get_int( "selected" ) )
		{
			vector < Filter * > filters = collectFilters( *clip->cut );
			int selected = clip->cut->get_int( "selected" ) - 1;
			int new_position = selected + direction;
			if ( new_position >= 0 && new_position < ( int )filters.size( ) )
			{
				Filter *temp = filters[ selected ];
				filters[ selected ] = filters[ new_position ];
				filters[ new_position ] = temp;
				clip->cut->set( "selected", new_position + 1 );
			}
			attachFilters( *clip->cut, filters );
		}
		delete_clip_info( clip );
	}
	delete producer;
	end( );
}

// Delete the cut on the selected track at the current position
void Project::remove( )
{
	start( );

	// Fetch active track
	Producer *producer = track( getActiveTrack( ) );

	// Convert to a track
	Playlist track( *producer );

	// Identify the clip index at the position
	int target_index = track.get_clip_index_at( position( ) );

	// We only have to do something if they differ
	if ( target_index >= 0 && target_index < track.count( ) )
	{
		track.remove( target_index );
		if ( target_index < track.count( ) && !track.is_blank( target_index ) )
			m_edit->seek( track.clip_start( target_index ) );
	}

	// Delete the reference to the track producer
	delete producer;

	end( );
}

void Project::blank( )
{
	start( );

	// Fetch active track
	Producer *producer = track( getActiveTrack( ) );

	// Convert to a track
	Playlist track( *producer );

	// Identify the clip index at the position
	int target_index = track.get_clip_index_at( position( ) );

	// We only have to do something if they differ
	if ( target_index >= 0 && target_index < track.count( ) )
	{
		Producer *cut = track.replace_with_blank( target_index );
		delete cut;
	}

	// Delete the reference to the track producer
	delete producer;

	end( );
}

// Move the cut on the selected track to the position specified
void Project::move( int clip_index, int delta )
{
	start( );

	// Attempt to get the track as a producer
	Producer *producer = track( getActiveTrack( ) );

	// Convert to a track
	Playlist track( *producer );

	// Identify the clip index at the position
	int to = track.clip_start( clip_index ) - delta;
	if ( to < 0 ) to = 0;
	int target_index = track.get_clip_index_at( to );
	int offset = position( ) - track.clip_start( clip_index );

	// We only have to do something if they differ
	if ( target_index < track.count( ) )
	{
		ClipInfo *clip = track.clip_info( clip_index );
		ClipInfo *info = track.clip_info( target_index );
		ClipInfo *post = track.clip_info( target_index + 1 < track.count( ) ? target_index + 1 : target_index );

		int position = clip->start;

		if ( target_index == clip_index || info->producer->is_blank( ) )
		{
			Producer *producer = track.replace_with_blank( clip_index );
			delete producer;
			track.consolidate_blanks( true );
			track.split_at( to );
			target_index = track.get_clip_index_at( to );
			int blanks = track.clip_length( target_index );
			if ( target_index == track.count( ) - 1 )
			{
				track.split_at( to );
				track.remove( target_index );
				track.append( *clip->cut );
			}
			else if ( blanks < clip->frame_count )
			{
				track.consolidate_blanks( true );
				to = to + blanks - clip->frame_count;
				if ( to > 0 )
					track.split_at( to );
				else if ( to < 0 )
					to = 0;
				track.insert_at( to, clip->cut, true );
			}
			else
			{
				track.insert_at( to, clip->cut, true );
			}
		}
		else if ( target_index >= clip_index && to > position && post->producer->is_blank( ) )
		{
			Producer *producer = track.replace_with_blank( clip_index );
			delete producer;
			track.consolidate_blanks( true );
			track.insert_at( to, clip->cut, true );
			target_index = track.get_clip_index_at( to );
		}
		else if ( target_index != clip_index )
		{
			// TODO: This ignores blanks in the 'sandwich' case 
			// ie: [large-thing] [in][small][out] where small is blank
			if ( to > info->start + info->length / 2 ) target_index ++;
			track.move( clip_index, target_index );
		}

		// Take into account the offset of the move
		m_edit->seek( to + offset );

		delete_clip_info( info );
		delete_clip_info( post );
		delete_clip_info( clip );
	}
	else
	{
		Producer *producer = track.replace_with_blank( clip_index );
		track.append( *producer );
		delete producer;
	}

	// Delete the reference to the track producer
	delete producer;

	end( );
}

static void resize_blank_to_right( Playlist &track, int target, int diff )
{
	if ( diff < 0 )
	{
		track.insert_blank( target + 1, ( - diff ) - 1 );
	}
	else if ( diff > 0 )
	{
		while( diff && target < track.count( ) )
		{
			while( !track.is_blank( target ) && target < track.count( ) )
				target ++;
			if ( target < track.count( ) )
			{
				ClipInfo *info = track.clip_info( target );
				if ( info->frame_count > diff )
				{
					track.resize_clip( target, 0, info->frame_count - diff - 1 );
					diff = 0;
				}
				else
				{
					diff -= info->frame_count;
					track.remove( target );
				}
				delete_clip_info( info );
			}
		}
	}
}

// Change in and out points of selected track at the current position
void Project::changeInPoint( int in )
{
	start( );

	// Fetch active track
	Producer *producer = track( getActiveTrack( ) );

	// Convert to a track
	Playlist track( *producer );

	// Identify the clip index at the position
	int target_index = track.get_clip_index_at( position( ) );

	// We only have to do something if they differ
	if ( target_index >= 0 && target_index < track.count( ) )
	{
		ClipInfo *info = track.clip_info( target_index );
		if ( in == -1 && position( ) > info->start )
		{
			track.resize_clip( target_index, info->frame_in + position( ) - info->start, info->frame_out );
			m_edit->seek( info->start );
		}
		else if ( in != -1 )
		{
			if ( m_modifier == keyboard_alt )
			{
				int out = info->frame_out + in - info->frame_in;
				if ( out < info->length )
					track.resize_clip( target_index, in, out );
			}
			else
			{
				int old_length = info->frame_out - info->frame_in + 1;
				int new_length = info->frame_out - in + 1;
				if ( m_modifier == keyboard_shift )
					resize_blank_to_right( track, target_index, new_length - old_length );
				track.resize_clip( target_index, in, info->frame_out );
			}
			m_edit->seek( info->start );
		}
		delete_clip_info( info );
	}

	// Delete the reference to the track producer
	delete producer;

	end( );
}

void Project::changeOutPoint( int out )
{
	start( );

	// Fetch active track
	Producer *producer = track( getActiveTrack( ) );

	// Convert to a track
	Playlist track( *producer );

	// Identify the clip index at the position
	int target_index = track.get_clip_index_at( position( ) );

	// We only have to do something if they differ
	if ( target_index >= 0 && target_index < track.count( ) )
	{
		ClipInfo *info = track.clip_info( target_index );
		if ( out == -1 && position( ) < info->start + info->frame_count - 1 )
		{
			track.split_at( position( ), false );
			track.remove( target_index + 1 );
			m_edit->seek( position( ) );
		}
		else if ( out != -1 )
		{
			int in = info->frame_in;
			if ( m_modifier == keyboard_alt )
			{
				in = info->frame_in + out - info->frame_out;
				if ( in >= 0 )
					track.resize_clip( target_index, in, out );
			}
			else
			{
				int old_length = info->frame_out - info->frame_in + 1;
				int new_length = out - info->frame_in + 1;
				if ( m_modifier == keyboard_shift )
					resize_blank_to_right( track, target_index, new_length - old_length );
				track.resize_clip( target_index, info->frame_in, out );
			}
			m_edit->seek( info->start + out - in );
		}
		delete_clip_info( info );
	}

	// Delete the reference to the track producer
	delete producer;

	end( );
}

// Populate the add mode selection widget
void Project::populateAddMode( QComboBox *box )
{
	box->clear( );
	// NB: Order must match the add_mode enum
	box->insertItem( "Append to track" );
	box->insertItem( "Append after cut" );
	box->insertItem( "Append at end of project" );
	box->insertItem( "Insert before cut" );
	box->insertItem( "Overwrite cut" );
	box->insertItem( "Overwrite cut at start" );
	box->insertItem( "Split and insert" );
	box->setCurrentItem( m_add_mode );
}

// Set the add mode
void Project::setAddMode( add_mode mode )
{
	m_add_mode = mode;
}

static double mapDimensions( int w, int h )
{
	double ret = 1.0;

	// Hack to allow images which have been rendered from the jah modules
	// to be introduced with a (possibly) correct aspect ratio
	if ( w == 720 && h == 576 ) ret = 59.0 / 54.0;
	if ( w == 720 && h == 480 ) ret = 10.0 / 11.0;

	return ret;
}

// Add an asset to the project
bool Project::addAsset( assetData asset ) 
{
	// Get the full name
	QString name = asset.getFullFileName( );
	bool image = false;
	bool sequence = false;

	// Not sure if i need to check the category or not
	switch( asset.theType )
	{
		case VideoType::SEQUENCE: 
			if ( asset.startframe != asset.endframe )
			{
				name = asset.location + ".all" + asset.extension;
				sequence = true;
			}
			else
			{
				image = true;
			}
			break;

		case VideoType::STILL: 
			image = true;
			break;

		case VideoType::MOVIE: 
			break;

		case VideoType::AIFF:
		case VideoType::AVI:
		case VideoType::MPEG:
		case VideoType::MP3:
		case VideoType::NOTYPE: 
			// TODO: Check if these are ever used
			break;

	}

	// Try to load the file 
	Producer clip( ( char * )name.ascii( ) );

	// Set the title
	clip.set( "jah_title", asset.filename.ascii( ) );

	// Corrections for image and sequence handling
	if ( sequence )
	{
		clip.set( "cache", 1 );
		clip.set( "jah_image", 1 );
		clip.set( "ttl", 1 );
		clip.set( "length", asset.endframe - asset.startframe + 1 );
		clip.set( "out", asset.endframe - asset.startframe );
	}
	else if ( image )
	{
		clip.set( "jah_image", 1 );
		clip.set( "length", 5 * ( get_int( "fps" ) == 25 ? 25 : 30 ) );
		clip.set( "out", 5 * ( get_int( "fps" ) == 25 ? 25 : 30 ) - 1 );
	}
	else
	{
		clip.set( "meta.volume", 1.0 );
	}

	// Temporary hack to obtain a thumb nail 
	// arbitrary scaling - not shown at the moment - mainly used as a sanity check
	uint8_t *thumb = NULL;
	Filter convert( "avcolour_space" );
	if ( convert.is_valid( ) )
	{
		convert.set( "forced", mlt_image_yuv422 );
		clip.attach( convert );
		Frame *frame = clip.get_frame( );
		if ( sequence || image )
			clip.set( "aspect_ratio", mapDimensions( frame->get_int( "width" ), frame->get_int( "height" ) ) );
		frame->set( "rescale", "nearest" );
		thumb = frame->fetch_image( mlt_image_rgb24, 360, 288, 1 );
		clip.set( "thumb", thumb, 44 * 36 * 3, mlt_pool_release );
		frame->set( "image", thumb, 0, NULL, NULL );
		delete frame;
		clip.detach( convert );
	}

	return thumb != NULL && addClip( clip );
}

bool Project::addClip( Producer &clip )
{
	if ( clip.is_valid( ) )
	{
		// Extent of the clip to add
		int in = clip.get_in( );
		int out = clip.get_length( ) - 1;

		// Fetch active track
		Producer *producer = track( getActiveTrack( ) );

		// Convert to a track
		Playlist track( *producer );

		// Get the target index
		int target_index = track.get_clip_index_at( position( ) );

		// Get existing clip
		ClipInfo *info = target_index < track.count( ) ? track.clip_info( target_index ) : NULL;
		ClipInfo *info_after = NULL;

		// Correct add mode when the project is empty
		add_mode mode = info == NULL ? mode_append : m_add_mode;

		// Block this to allow multiple operations which aren't tracked by undo/redo
		start( );

		// Special case handling of fx on non-blank sections
		if ( clip.get_int( "meta.fx_cut" ) && info != NULL && !info->cut->is_blank( ) )
		{
			// We will treat fx cuts as filters that are applied to the cut
			int i = 0;
			Filter *filter = clip.filter( i ++ );
			while ( filter != NULL && filter->is_valid( ) )
			{
				if ( !filter->get_int( "_fezzik" ) )
					info->cut->attach( *filter );
				filter = clip.filter( i ++ );
			}
		}
		else if ( !clip.get_int( "meta.fx_cut" ) )
		{
			vector < Filter * > filters;

			// Allow modes of insert/overwrite/append etc
			switch ( mode )
			{
				case mode_append_track:
					track.consolidate_blanks( );
					track.append( clip );
					break;
	
				case mode_append:
					track.consolidate_blanks( );
					track.insert( clip, target_index + 1 );
					break;

				case mode_append_project:
					track.append( clip );
					break;
	
				case mode_insert:
					track.insert( clip, target_index );
					break;
	
				case mode_overwrite:
					filters = collectFilters( *info->cut );
					remove( );
					track.insert( clip, target_index );
					info_after = track.clip_info( target_index );
					attachFilters( *( info_after->cut ), filters );
					break;
	
				case mode_overwrite_start:
					filters = collectFilters( *info->cut );
					target_index = track.get_clip_index_at( info->start );
					if ( info->frame_count > clip.get_length( ) )
						track.split_at( info->start + clip.get_length( ) - 1 );
					else if ( info->frame_count < clip.get_length( ) )
						out = in + info->frame_count - 1;
					track.remove( target_index );
					track.insert( clip, target_index, in, out );
					info_after = track.clip_info( target_index );
					attachFilters( *( info_after->cut ), filters );
					break;
	
				case mode_split_clip:
					if ( split( ) || position( ) != info->start )
						target_index ++;
					track.insert( clip, target_index );
					break;
			}
		}

		// Unblock and checkpoint/update etc
		end( );

		// Delete the reference to the track producer
		delete producer;

		// Delete the clip info
		delete_clip_info( info );
		delete_clip_info( info_after );
	}

	return clip.is_valid( );
}

void Project::mix( int length )
{
	start( );

	// Fetch active track
	Producer *producer = track( getActiveTrack( ) );

	// Convert to a track
	Playlist track( *producer );

	// Get the target index
	int target_index = track.get_clip_index_at( position( ) );

	// Apply the transition
	Transition luma( "luma" );
	track.mix( target_index, length, &luma );
	Transition mix( "mix" );
	track.mix_add( target_index + 1, &mix );

	Producer *cut = track.get_clip( target_index + 1 );
	cut->parent( ).set( "jah_title", "mix" );
	delete cut;

	// Delete the producer
	delete producer;

	end( );
}
