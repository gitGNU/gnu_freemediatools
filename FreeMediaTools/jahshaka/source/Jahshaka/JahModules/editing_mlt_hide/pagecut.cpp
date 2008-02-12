/*******************************************************************************
**
** The source file for the Jahshaka/MLT editing module cut page
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "pagecut.h"
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

PageCut::PageCut( QWidget *parent, GLEdit *edit ) 
    : Page( parent, edit ),
      m_cut( NULL ),
      m_properties( NULL ),
      m_fetch_colour( false ),
      m_fetch_geometry( false ),
      m_geometry( NULL )
{
    QHBoxLayout* mainLayout = new QHBoxLayout( this );
    JahFormatter::setMarginAndSpacing( mainLayout );

    m_multitrack = new MultiTrack( this );
    mainLayout->addWidget( m_multitrack );
    m_multitrack->setFocusPolicy( QWidget::ClickFocus );
    connect( m_multitrack, SIGNAL( positionChanged(int) ), SLOT( sliderValueChanged( int ) ) );
    connect( m_multitrack, SIGNAL( trackSelected(Qt::ButtonState,MultiTrackTrack *) ), SLOT( trackSelected(Qt::ButtonState,MultiTrackTrack *) ) );
	connect( m_multitrack, SIGNAL( enableClicked( MultiTrackTrack * ) ), SLOT( enableClicked( MultiTrackTrack * ) ) );

    QVBoxLayout* tableAndButtonsLayout = new QVBoxLayout();
    mainLayout->addLayout( tableAndButtonsLayout );

    m_table = new Table( this, "cut_table" );
    tableAndButtonsLayout->addWidget( m_table );
    connect( m_table, SIGNAL( valueChanged( int, int ) ), SLOT( valueChanged( int, int ) ) );
    
    m_indicate = new Filter( "invert" );
    m_indicate->set( "alpha", 128 );
    
    m_geometry = new Geometry( );
}

PageCut::~PageCut( )
{
    delete m_properties;
    delete m_cut;
    delete m_geometry;
    delete m_indicate;
}

void PageCut::restart( )
{
    m_multitrack->setEnabled( false );
    m_multitrack->deselectAll( );
    m_multitrack->clear( );
    m_table->setEnabled( false );
}

void PageCut::refresh( )
{
    Producer *producer = project( )->track( project( )->getActiveTrack( ) );
    Playlist track( *producer );
    int cut_idx = track.get_clip_index_at( editor( )->position( ) );
    if ( cut_idx < track.count( ) )
    {
        ClipInfo *info = track.clip_info( cut_idx );
        updateSettings( info );
        delete_clip_info( info );
    }
    else
    {
        restart( );
    }
    delete producer;
}

void PageCut::setPosition( int value )
{
	// The cut slider
	m_multitrack->blockSignals( true );
	m_multitrack->setPosition( value - m_multitrack->getOffset( ) );
	m_multitrack->blockSignals( false );
}

void PageCut::sliderValueChanged( int value )
{
	value = GLEdit::rangeCheck( value + m_multitrack->getOffset( ), 0, editor( )->length( ) - 1 ); 
	editor( )->seek( value );
}

void PageCut::trackSelected( Qt::ButtonState, MultiTrackTrack *mtrack )
{
	// Quick and dirty hack - revisit
	NleMultiTrackTrack *nle_track = ( NleMultiTrackTrack * )mtrack;
	Producer *producer = project( )->track( project( )->getActiveTrack( ) );
	Playlist track( *producer );
	int cut_idx = track.get_clip_index_at( editor( )->position( ) );
	if ( cut_idx < track.count( ) )
	{
		ClipInfo *info = track.clip_info( cut_idx );
		info->cut->set( "selected", nle_track->trackIndex( ) );
		updateSettings( info );
		delete_clip_info( info );
	}
	delete producer;
	editor( )->checkPoint( );
	refresh( );
}

void PageCut::updateSettings( ClipInfo *info )
{
	int i = 0;
	QString title;

	// Set the length and offset for this multitrack
	m_multitrack->deselectAll( );
	m_multitrack->clear( );
	m_multitrack->setLength( info->frame_count - 1 );
	m_multitrack->setOffset( info->start );

	// The first item is always the cut itself
	// TODO: Fix fx cut case
	NleMultiTrackTrack *cut_fx = new NleMultiTrackTrack( "Cut", m_multitrack, 0 );
	cut_fx->setControls( 0 );
	MultiTrackItem *item = new MultiTrackItem( info->producer->parent( ).get( "jah_title" ), cut_fx );
	item->setTime( 0 );
	item->setLength( info->frame_count - 1 );

	// Temporary - dump properties into the table
	delete m_cut;
	m_cut = new Producer( info->cut );

	// Handle the selected case
	int selected = info->cut->get_int( "selected" );
	Properties *props = new Producer( m_cut->parent( ) );
	NleMultiTrackTrack *selected_track = cut_fx;
	MultiTrackItem *selected_item = item;
	int selected_index = 0;

	// Iterate through the attached filters
	int fx_count = 0;
	Filter *filter = info->cut->filter( i ++ );
	while ( filter != NULL && filter->is_valid( ) )
	{
		// If we have filter and it's not a normaliser
		if ( filter->is_valid( ) && filter->get_int( "_fezzik" ) == 0 )
		{
			// Create a track and a single item for this filter
			title.sprintf( "FX%d", ++ fx_count );
			cut_fx = new NleMultiTrackTrack( title, m_multitrack, fx_count );
			cut_fx->setControls( show_enabled );
			cut_fx->setEnabled( filter->get_int( "disable" ) == 0 );
			item = new MultiTrackItem( getTitle( filter ), cut_fx );
			item->setTime( 0 );

			int in = filter->get_int( "in" );
			int out = filter->get_int( "out" );
			if ( out == 0 ) out = m_multitrack->length( );
			item->setTime( in );
			item->setLength( out - in );

			// If this is the selected item, this is one that will be shown
			if ( selected >= fx_count ) 
			{
				delete props;
				props = new Filter( *filter );
				selected_track = cut_fx;
				selected_item = item;
				selected_index = fx_count;
			}
		}

		delete filter;

		// Fetch the next filter
		filter = info->cut->filter( i ++ );
	}

	// Set the selection status of the track and the item
	m_multitrack->setSelected( selected_track, true );
	m_multitrack->setSelected( selected_item, true );

	m_cut->set( "selected", selected_index );
	showProperties( props );
}

void PageCut::showProperties( Properties *properties )
{
	delete m_properties;
	m_properties = properties;

	if ( properties != NULL && ( properties->get( "mlt_service" ) == NULL || strcmp( properties->get( "mlt_service" ), "blank" ) ) )
	{
		bool is_filter = !strcmp( properties->get( "mlt_type" ), "filter" );

		int rows = 0;
		m_table->setRow( rows ++, "jah_title", getTitle( properties ) );

		for ( int i = 0; i < properties->count( ); i ++ )
		{
			const char *name = properties->get_name( i );
			const char *value = properties->get( name );

			if ( name[ 0 ] != '_' && value != NULL && strcmp( name, "in" ) && strcmp( name, "out" ) && 
				 strncmp( name, "mlt_", 4 ) && strcmp( name, "track" ) && strncmp( name, "jah_", 4 ) &&
				 strcmp( name, "disable" ) && strcmp( name, "title" ) && strcmp( name, "fps" ) && 
				 strncmp( name, "frame_rate", 10 ) && strcmp( name, "eof" ) && strcmp( name, "length" ) && 
				 strcmp( name, "id" ) && strcmp( name, "seekable" ) && strcmp( name, "video_index" ) && 
			  	 strcmp( name, "audio_index" ) && strcmp( name, "av_bypass" ) )
			{
				QString type = QString( "jah_p_" ) + name;
				if ( properties->get( type ) != NULL )
					m_table->setRow( rows ++, name, value, properties->get( type ) );
				else
					m_table->setRow( rows ++, name, value );
			}
		}

		if ( is_filter )
		{
			int out = properties->get_int( "out" );
			if ( out == 0 ) out = m_multitrack->length( ) - 1;
			m_table->setRow( rows ++, "in", properties->get( "in" ) );
			m_table->setRow( rows ++, "out", QString::number( out ) );
		}

		m_table->setNumRows( rows );
		m_table->setEnabled( true );

		int disable = properties->get_int( "disable" );
		m_table->setEnabled( !disable );
		m_multitrack->setEnabled( true );
	}
	else
	{
		m_multitrack->clear( );
		m_table->setNumRows( 0 );
		m_multitrack->setEnabled( false );
		m_table->setEnabled( false );
	}
}

const char *PageCut::getTitle( Mlt::Properties *properties )
{
	const char *title = properties->get( "jah_title" );
	if ( title == NULL ) title = properties->get( "title" );
	if ( title == NULL ) title = properties->get( "mlt_service" );
	if ( title == NULL ) title = "Untitled";
	return title;
}

void PageCut::valueChanged( int row, int )
{
	if ( m_properties != NULL )
	{
		QString name = m_table->getName( row );
		QString value = m_table->getValue( row );
		project( )->start( );
		Service service( *( Service * )m_properties );
		m_properties->set( name, value );
		project( )->end( );
		preview( )->refresh( );
	}
}

void PageCut::enableClicked( MultiTrackTrack *object )
{
	NleMultiTrackTrack *track = dynamic_cast< NleMultiTrackTrack * >( object );
	if ( m_properties != NULL && m_multitrack->isSelected( track ) )
	{
		project( )->block( );
		Service service( *( Service * )m_properties );
		m_properties->set( "disable", !m_properties->get_int( "disable" ) );
		delete m_properties;
		m_properties = NULL;
		project( )->unblock( );
	}
}

void PageCut::removeEffect( )
{
	if ( m_cut != NULL && m_properties != NULL )
	{
		Filter filter( ( Filter & )*m_properties );
		if ( filter.is_valid( ) )
		{
			project( )->start( );
			m_cut->detach( filter );
			delete m_properties;
			m_properties = NULL;
			project( )->end( );
			preview( )->refresh( );
		}
	}
}

static void changeSelected( Project *project, Producer *cut, int direction )
{
	if ( cut && !cut->is_blank( ) )
	{
		project->start( );
		cut->set( "selected", cut->get_int( "selected" ) + direction );
		project->end( );
	}
}

// Keyboard bindings
void PageCut::processKeyEvent( QKeyEvent *k )
{
	k->accept( );

	if ( k->state( ) == 0 )
	{
		switch ( toupper( k->key( ) ) )
		{
			case 'I': setInPoint( ); break;
			case 'O': setOutPoint( ); break;
			case '-': project( )->moveFilter( -1 ); break;
			case '=': case '+': project( )->moveFilter( 1 ); break;
			case Key_Delete: removeEffect( ); break;
			case Key_Up: changeSelected( project( ), m_cut, -1 ); break;
			case Key_Down: changeSelected( project( ), m_cut, 1 ); break;
			default: k->ignore( ); break;
		}
	}
	else
	{
		k->ignore( );
	}
}

bool PageCut::dropAsset( assetData &asset )
{
	bool accepted = false;

	if ( m_cut != NULL && m_properties != NULL )
	{
		Filter filter( ( Filter & )*m_properties );
		if ( filter.is_valid( ) && filter.get( "resource" ) != NULL && asset.extension != ".jfx" )
		{
			QString name = asset.getFullFileName( );
			project( )->start( );
			m_properties->set( "resource", name.utf8( ) );
			delete m_properties;
			m_properties = NULL;
			project( )->end( );
			preview( )->refresh( );
			accepted = true;
		}
	}

	return accepted;
}

void PageCut::setInPoint( )
{
	if ( m_cut != NULL && m_properties != NULL )
	{
		Filter filter( ( Filter & )*m_properties );
		if ( filter.is_valid( ) )
		{
			project( )->start( );
			m_properties->set( "in", m_multitrack->position( ) );
			delete m_properties;
			m_properties = NULL;
			project( )->end( );
			preview( )->refresh( );
		}
	}
}

void PageCut::setOutPoint( )
{
	if ( m_cut != NULL && m_properties != NULL )
	{
		Filter filter( ( Filter & )*m_properties );
		if ( filter.is_valid( ) )
		{
			project( )->start( );
			m_properties->set( "out", m_multitrack->position( ) );
			delete m_properties;
			m_properties = NULL;
			project( )->end( );
			preview( )->refresh( );
		}
	}
}

bool PageCut::acceptsSelection( ) 
{
	m_fetch_colour = m_fetch_geometry = false;
	if ( m_properties != NULL && m_properties->get( "key" ) )
	{
		m_fetch_colour = true;
	}
	else if ( m_properties != NULL && m_properties->get( "composite.geometry" ) )
	{
		m_fetch_geometry = true;
		m_name_selection = "composite.geometry";
	}
	else if ( m_properties != NULL && m_properties->get( "geometry" ) )
	{
		m_fetch_geometry = true;
		m_name_selection = "geometry";
	}
	return m_fetch_colour | m_fetch_geometry;
}

void PageCut::startSelection( double x, double y ) 
{
	if ( m_fetch_colour )
		colourStartSelection( x, y );
	else if ( m_fetch_geometry )
		geometryStartSelection( x, y );
}

void PageCut::moveSelection( double x, double y ) 
{
	if ( m_fetch_colour )
		colourMoveSelection( x, y );
	else if ( m_fetch_geometry )
		geometryMoveSelection( x, y );
}

void PageCut::setSelection( double x, double y ) 
{
	if ( m_fetch_colour )
		colourSetSelection( x, y );
	else if ( m_fetch_geometry )
		geometrySetSelection( x, y );
}

void PageCut::consumerFrameShow( Mlt::Frame &frame ) 
{
	if ( m_fetch_colour )
		colourConsumerFrameShow( frame );
}

void PageCut::colourStartSelection( double x, double y ) 
{
	editor( )->ignoreChanges( true );
	editor( )->setSpeed( 0 );
	m_sx = x;
	m_sy = y;
	m_dx = 0;
	m_dy = 0;
	m_final_colour = false;
	m_fetch_colour = true;
	m_properties->set( "disable", 1 );
	preview( )->refresh( );
}

void PageCut::colourMoveSelection( double x, double y ) 
{
	m_dx = x;
	m_dy = y;
	m_fetch_colour = true;
	preview( )->refresh( );
}

void PageCut::colourSetSelection( double x, double y ) 
{
	m_dx = x;
	m_dy = y;
	m_final_colour = true;
	m_fetch_colour = true;
	editor( )->ignoreChanges( false );
	project( )->start( );
	m_properties->set( "disable", 0 );
	project( )->end( );
	preview( )->refresh( );
}

void PageCut::colourConsumerFrameShow( Mlt::Frame &frame ) 
{
	if ( m_fetch_colour )
	{
		// Get the width and height and format of the image.
		int w = frame.get_int( "width" );
		int h = frame.get_int( "height" );
		mlt_image_format format = ( mlt_image_format )frame.get_int( "format" );
		uint8_t *image = frame.get_image( format, w, h );
		int x = int( w * ( m_sx + m_dx ) ) / 100;
		int y = int( h * ( m_sy + m_dy ) ) / 100;
		char key[ 256 ] = "";
		int r, g, b, v;

		switch( format )
		{
			case mlt_image_rgb24:
				image += y * w * 3 + x * 3;
				sprintf( key, "0x%02x%02x%02x", *image, *( image + 1 ), *( image + 2 ) );
				break;

			case mlt_image_rgb24a:
				image += y * w * 4 + x * 4;
				sprintf( key, "0x%02x%02x%02x", *image, *( image + 1 ), *( image + 2 ) );
				break;

			case mlt_image_opengl:
				image += ( h - y - 1 ) * w * 4 + x * 4;
				sprintf( key, "0x%02x%02x%02x", *image, *( image + 1 ), *( image + 2 ) );
				break;

			case mlt_image_yuv422:
				image += y * w * 2 + ( ( x >> 1 ) << 1 ) * 2;
				v = ( int( *image ) + int( *( image + 2 ) ) ) >> 1;
				YUV2RGB( v, *( image + 1 ), *( image + 3 ), r, g, b );
				sprintf( key, "0x%02x%02x%02x", r, g, b );
				break;

			default:
				break;
		}

		if ( strcmp( key, "" ) )
		{
			project( )->start( );
			if ( m_final_colour )
				m_properties->set( "disable", 0 );
			m_properties->set( "key", key );
			project( )->end( );
		}

		refresh( );

		m_fetch_colour = false;
	}
}

void PageCut::showSelection( bool active )
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

QString PageCut::getGeometry( double ox, double oy )
{
	GeometryItem item;
	m_geometry->fetch( item, m_multitrack->position( ) );

	GeometryItem backup;
	m_geometry->fetch( backup, m_multitrack->position( ) );

	switch( editor( )->getToolMode( ) )
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

	m_geometry->insert( item );
	QString geometry = m_geometry->serialise( );

	m_geometry->insert( backup );

	return geometry;
}

void PageCut::geometryStartSelection( double, double ) 
{
	if ( m_properties != NULL && m_name_selection != "" )
	{
		editor( )->ignoreChanges( true );
		m_geometry->parse( m_properties->get( m_name_selection ), editor( )->length( ), 720, preview( )->isPAL( ) ? 576 : 480 );
		editor( )->setSpeed( 0 );
		project( )->start( );
		showSelection( true );
		project( )->end( );
		preview( )->refresh( );
	}
}

void PageCut::geometryMoveSelection( double x, double y ) 
{
	if ( m_properties != NULL && m_name_selection != "" )
	{
		project( )->start( );
		Service service( *( Service * )m_properties );
		m_properties->set( m_name_selection, getGeometry( x, y ).ascii( ) );
		project( )->end( );
		preview( )->refresh( );
	}
}

void PageCut::geometrySetSelection( double x, double y ) 
{
	if ( m_properties != NULL && m_name_selection != "" )
	{
		project( )->start( );
		Service service( *( Service * )m_properties );
		m_properties->set( m_name_selection, getGeometry( x, y ).ascii( ) );
		delete m_properties;
		m_properties = NULL;
		showSelection( false );
		editor( )->ignoreChanges( false );
		project( )->end( );
		preview( )->refresh( );
	}
}

