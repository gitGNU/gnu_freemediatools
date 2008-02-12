/*******************************************************************************
**
** The source file for the Jahshaka/MLT editing module track page
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "pagetrack.h"
using namespace jahnle;

#include "edit.h"
using namespace Mlt;

#include "project.h"
#include "preview.h"
#include "track.h"
#include "table.h"
#include <jahformatter.h>

#include <qcheckbox.h>
#include <qhbox.h>
#include <qframe.h>
#include <qpushbutton.h>
#include <qlayout.h>

PageTrack::PageTrack( QWidget *parent, GLEdit *edit ) 
    : Page( parent, edit ),
      m_edit( edit ),
      m_properties( NULL ),
      m_name_selection( "" ),
      m_accepts_selection( false )
{
    QHBoxLayout* mainLayout = new QHBoxLayout( this );
    JahFormatter::setMarginAndSpacing( mainLayout );

    m_multitrack = new MultiTrack( this );
    mainLayout->addWidget( m_multitrack );
    m_multitrack->setFocusPolicy( QWidget::ClickFocus );
    connect( m_multitrack, SIGNAL( positionChanged(int) ), editor( ), SLOT( sliderValueChanged( int ) ) );
    connect( m_multitrack, SIGNAL( trackSelected(Qt::ButtonState,MultiTrackTrack *) ), SLOT( trackSelected(Qt::ButtonState,MultiTrackTrack *) ) );
	connect( m_multitrack, SIGNAL( enableClicked( MultiTrackTrack * ) ), SLOT( enableClicked( MultiTrackTrack * ) ) );
    
    QVBoxLayout* tableAndButtonsLayout = new QVBoxLayout();
    mainLayout->addLayout( tableAndButtonsLayout );

    m_table = new Table( this, "track_table" );
    tableAndButtonsLayout->addWidget( m_table );
    connect( m_table, SIGNAL( valueChanged( int, int ) ), SLOT( valueChanged( int, int ) ) );

    m_indicate = new Filter( "invert" );
    m_indicate->set( "alpha", 128 );
    m_start = new Geometry( );
}

PageTrack::~PageTrack( )
{
	delete m_properties;
	delete m_indicate;
	delete m_start;
}

void PageTrack::restart( )
{
	m_multitrack->hide( );
	m_multitrack->deselectAll( );
	m_multitrack->clear( );
	m_table->hide( );
}

void PageTrack::refresh( )
{
	Producer *producer = project( )->track( project( )->getActiveTrack( ) );
	Playlist track( *producer );
	updateSettings( track );
	delete producer;
}

void PageTrack::setPosition( int value )
{
	m_multitrack->blockSignals( true );
	m_multitrack->setPosition( value );
	m_multitrack->blockSignals( false );
}

void PageTrack::trackSelected( Qt::ButtonState, MultiTrackTrack *mtrack )
{
	// Quick and dirty hack - revisit
	NleMultiTrackTrack *nle_track = ( NleMultiTrackTrack * )mtrack;
	Producer *producer = project( )->track( project( )->getActiveTrack( ) );
	Playlist track( *producer );
	producer->set( "selected", nle_track->trackIndex( ) );
	updateSettings( track );
	delete producer;
	editor( )->checkPoint( );
	refresh( );
}

void PageTrack::updateSettings( Playlist &track )
{
	int i = 0;
	QString title;

	// Set the length and offset for this multitrack
	m_multitrack->deselectAll( );
	m_multitrack->clear( );
	m_multitrack->setLength( editor( )->length( ) );

	// Handle the selected case
	int selected = track.get_int( "selected" );
	Properties *props = NULL;
	NleMultiTrackTrack *selected_track = NULL;
	MultiTrackItem *selected_item = NULL;

	// Iterate through the attached filters
	Filter *filter = track.filter( i );

	while ( filter != NULL )
	{
		if ( filter->is_valid( ) )
		{
			// Create the track for this filter
			title.sprintf( "FX%d", i + 1 );
			NleMultiTrackTrack *track_fx = new NleMultiTrackTrack( title, m_multitrack, i );
			track_fx->setControls( show_enabled );
			track_fx->setEnabled( filter->get_int( "disable" ) == 0 );
			MultiTrackItem *item = new MultiTrackItem( getTitle( filter ), track_fx );
			item->setTime( 0 );
			item->setLength( editor( )->length( ) );

			// If this is the selected item, this is one that will be shown
			if ( selected >= i ) 
			{
				delete props;
				props = new Filter( *filter );
				selected_track = track_fx;
				selected_item = item;
			}
		}

		delete filter;

		// Fetch the next filter
		filter = track.filter( ++ i );
	}

	if ( props != NULL )
	{
		// Set the selection status of the track and the item
		m_multitrack->setSelected( selected_track, true );
		m_multitrack->setSelected( selected_item, true );
		m_multitrack->show( );
	}

	// Temporary - dump properties into the table
	if ( m_properties == NULL || props == NULL || props->get( "mlt_service" ) != m_properties->get( "mlt_service" ) )
		showProperties( props );
	else
		delete props;
}

void PageTrack::showProperties( Properties *properties )
{
	delete m_properties;
	m_properties = properties;

	if ( properties != NULL )
	{
		int rows = 1;

		m_accepts_selection = false;
		m_name_selection = "";

		m_table->setRow( 0, "jah_title", getTitle( properties ) );

		for ( int i = 0; i < properties->count( ); i ++ )
		{
			const char *name = properties->get_name( i );
			const char *value = properties->get( name );
			if ( name[ 0 ] != '_' && value != NULL && strcmp( name, "in" ) && strcmp( name, "out" ) && 
				 strncmp( name, "mlt_", 4 ) && strcmp( name, "track" ) && strncmp( name, "jah_p_", 6 ) &&
				 strcmp( name, "disable" ) && strcmp( name, "jah_title" ) && strcmp( name, "title" ) )
			{
				m_table->setRow( rows ++, name, value );
				if ( strstr( name, "geometry" ) )
				{
					m_accepts_selection = true;
					m_name_selection = name;
				}
			}
		}

		m_table->setNumRows( rows );
		m_table->show( );

		int disable = properties->get_int( "disable" );
		m_table->setEnabled( !disable );
	}
	else
	{
		m_table->hide( );
	}
}

const char *PageTrack::getTitle( Mlt::Properties *properties )
{
	const char *title = properties->get( "jah_title" );
	if ( title == NULL ) title = properties->get( "title" );
	if ( title == NULL ) title = properties->get( "mlt_service" );
	if ( title != NULL && !strcmp( title, "transition" ) ) title = properties->get( "transition" );
	if ( title == NULL ) title = "Untitled";
	return title;
}

void PageTrack::valueChanged( int row, int )
{
	if ( m_properties != NULL )
	{
		QString name = m_table->getName( row );
		QString value = m_table->getValue( row );
		project( )->start( );
		m_properties->set( name, value );
		delete m_properties;
		m_properties = NULL;
		preview( )->refresh( );
		project( )->end( );
	}
}

void PageTrack::enableClicked( MultiTrackTrack *object )
{
	NleMultiTrackTrack *track = dynamic_cast< NleMultiTrackTrack * >( object );
	if ( m_properties != NULL && m_multitrack->isSelected( track ) )
	{
		project( )->block( );
		m_properties->set( "disable", !m_properties->get_int( "disable" ) );
		delete m_properties;
		m_properties = NULL;
		project( )->unblock( );
	}
}

QString PageTrack::getGeometry( double ox, double oy )
{
	GeometryItem item;
	m_start->fetch( item, 0 );
	switch( m_edit->getToolMode( ) )
	{
		case tool_null:
			break;
		case tool_translate:
			item.x( item.x( ) + ox * 7.2 );
			item.y( item.y( ) + oy * ( preview( )->isPAL( ) ? 5.76 : 4.80 ) );
			break;
		case tool_scale:
			item.w( item.w( ) + ox * 7.2 );
			item.h( item.h( ) + oy * ( preview( )->isPAL( ) ? 5.76 : 4.80 ) );
			break;
	}

	double x = item.x( );
	double y = item.y( );
	double w = item.w( );
	double h = item.h( );

	if ( w < 0 )
	{
		x = x + w;
		w = -w;
	}

	if ( h < 0 )
	{
		y = y + h;
		h = -h;
	}

	QString geometry;
	geometry.sprintf( "%d,%d:%dx%d", int( x ), int( y ), int( w ), int( h ) );

	return geometry;
}

void PageTrack::showSelection( bool active )
{
	Producer *producer = project( )->track( project( )->getActiveTrack( ) );
	Playlist track( *producer );
	int cut_idx = track.get_clip_index_at( editor( )->position( ) );
	if ( cut_idx < track.count( ) )
	{
		ClipInfo *info = track.clip_info( cut_idx );
		if ( active )
			info->cut->attach( *m_indicate );
		else
			info->cut->detach( *m_indicate );
		delete_clip_info( info );
	}
	delete producer;
}

void PageTrack::startSelection( double, double )
{
	if ( m_properties != NULL && m_name_selection != "" )
	{
		m_edit->ignoreChanges( true );
		m_start->parse( m_properties->get( m_name_selection ), m_edit->length( ), 720, preview( )->isPAL( ) ? 576 : 480 );
		m_edit->setSpeed( 0 );
		project( )->start( );
		showSelection( true );
		project( )->end( );
		preview( )->refresh( );
	}
}

void PageTrack::moveSelection( double ox, double oy )
{
	if ( m_properties != NULL && m_name_selection != "" )
	{
		project( )->start( );
		Service service( *( Service * )m_properties );
		m_properties->set( m_name_selection, getGeometry( ox, oy ).ascii( ) );
		project( )->end( );
		preview( )->refresh( );
	}
}

void PageTrack::setSelection( double ox, double oy )
{
	if ( m_properties != NULL && m_name_selection != "" )
	{
		project( )->start( );
		Service service( *( Service * )m_properties );
		m_properties->set( m_name_selection, getGeometry( ox, oy ).ascii( ) );
		delete m_properties;
		m_properties = NULL;
		showSelection( false );
		m_edit->ignoreChanges( false );
		project( )->end( );
		preview( )->refresh( );
	}
}
