/*******************************************************************************
 **
 ** The source file for the Jahshaka/MLT editing module project page
 ** The Jahshaka Project
 ** Copyright (C) 2000-2006 VM Inc.
 ** Released under the GNU General Public License
 **
 *******************************************************************************/

#include <iostream>

#include "pageproject.h"
using namespace jahnle;

#include "edit.h"
using namespace Mlt;

#include "project.h"
#include "preview.h"
#include "track.h"
#include "item.h"

#include <qpopupmenu.h>
#include <qbutton.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlayout.h>

#include <glcore.h>
#include <supergrangecontrol.h>
#include <valueFormatters.h>
#include <jahformatter.h>
#include <jahpreferences.h>

PageProject::PageProject( QWidget *parent, GLEdit *edit ) 
    : Page( parent, edit ),
      m_paste( NULL )
{
    // Palettes for the sliders
    QPalette active_palette( QColor( 0, 0, 0 ) );
    active_palette.setColor( QPalette::Active, QColorGroup::Text, Qt::green );
    QPalette inactive_palette( QColor( 0, 0, 0 ) );
    inactive_palette.setColor( QPalette::Active, QColorGroup::Text, Qt::red );

    // setup layout
    QVBoxLayout* mainLayout = new QVBoxLayout( this );
    JahFormatter::setMarginAndSpacing( mainLayout );
    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->setResizeMode( QLayout::Fixed );
    mainLayout->addLayout( topLayout );
    
    // Track manipulations
    m_tracks = new QPushButton( this, "tracks" );
    topLayout->addWidget( m_tracks );
    m_tracks->setFocusPolicy( QWidget::NoFocus );
    m_tracks->setText( "T&racks..." );
    connect( m_tracks, SIGNAL( clicked( ) ), this, SLOT( trackManipulations( ) ) );
    
    // Add mode selection
    QLabel *label = new QLabel( "Mode:", this );
    topLayout->addWidget( label );
    label->setAlignment( Qt::AlignRight | Qt::AlignVCenter );

    m_add_mode_box = new QComboBox( FALSE, this, "add_mode_box" );
    topLayout->addWidget( m_add_mode_box );
    m_add_mode_box->setFocusPolicy( QWidget::NoFocus );
    project( )->populateAddMode( m_add_mode_box );
    connect( m_add_mode_box, SIGNAL( activated( int ) ), this, SLOT( setAddMode( int ) ) );
    
    TimecodeValueFormatter fmt( preview( )->isPAL() ? 25 : 30 );
    
    // In point 'slider'
    label = new QLabel( "In:", this );
    topLayout->addWidget( label );
    label->setAlignment( Qt::AlignRight | Qt::AlignVCenter );

    m_in_input = new SupergRangeControl( this, "in_input" );
    topLayout->addWidget( m_in_input );
    m_in_input->setValue( 0 );
    m_in_input->setFormatter( fmt );
    m_in_input->setDefault( SupergRangeControl::Min );
    m_in_input->setPalette( active_palette, true );

    connect( m_in_input, SIGNAL( valueChanged( int ) ), SLOT( inPositionChanged( int ) ) );
    connect( m_in_input, SIGNAL( inputPressEvent( QMouseEvent * ) ), SLOT( positionPushed( QMouseEvent * ) ) );
    connect( m_in_input, SIGNAL( inputReleaseEvent( ) ), SLOT( positionReleased( ) ) );
    
    // Out point 'slider'
    label = new QLabel( "Out:", this );
    topLayout->addWidget( label );
    label->setAlignment( Qt::AlignRight | Qt::AlignVCenter );

    m_out_input = new SupergRangeControl( this, "out_input" );
    topLayout->addWidget( m_out_input );
    m_out_input->setValue( 0 );
    m_out_input->setFormatter( fmt );
    m_out_input->setDefault( SupergRangeControl::Max );
    m_out_input->setPalette( active_palette, true );

    connect( m_out_input, SIGNAL( valueChanged( int ) ), SLOT( outPositionChanged( int ) ) );
    connect( m_out_input, SIGNAL( inputPressEvent( QMouseEvent * ) ), SLOT( positionPushed( QMouseEvent * ) ) );
    connect( m_out_input, SIGNAL( inputReleaseEvent( ) ), SLOT( positionReleased( ) ) );
    
    // add a spacer
    topLayout->addStretch();

    // Duration is for output only
    label = new QLabel( "Length:", this );
    topLayout->addWidget( label );
    label->setAlignment( Qt::AlignRight | Qt::AlignVCenter );

    m_duration_output = new SupergRangeControl( this, "duration_input" );
    topLayout->addWidget( m_duration_output );
    m_duration_output->setFormatter( fmt );
    m_duration_output->setDefault( SupergRangeControl::None );
    m_duration_output->setPalette( inactive_palette, true );
    
    // Zoom
    const QString PIXMAP_PATH = QString( JahPrefs::getInstance().getBasePath().data() ) + "Pixmaps/widgets/";

    QPushButton* reset_zoom = new QPushButton( this, "reset_zoom" );
    topLayout->addWidget( reset_zoom );
    reset_zoom->setFocusPolicy( QWidget::NoFocus );
    reset_zoom->setPixmap( PIXMAP_PATH + "reset-magnification.png" );
    reset_zoom->setFlat( true );
    reset_zoom->setFixedSize( QSize( 32, 32 ) );
    reset_zoom->setAccel( QKeySequence( "Alt+-" ) );
    connect( reset_zoom, SIGNAL( clicked( ) ), SLOT( resetZoom( ) ) );

    QPushButton* zoom_in = new QPushButton( this, "zoom_in" );
    topLayout->addWidget( zoom_in );
    zoom_in->setFocusPolicy( QWidget::NoFocus );
    zoom_in->setPixmap( PIXMAP_PATH + "zoom-in.png" );
    zoom_in->setFlat( true );
    zoom_in->setFixedSize( QSize( 32, 32 ) );
    zoom_in->setAccel( QKeySequence( "Alt+=" ) );
    connect( zoom_in, SIGNAL( clicked( ) ), SLOT( zoomIn( ) ) );

    // Multitrack widget
    m_multitrack = new MultiTrack( this );
    mainLayout->addWidget( m_multitrack );
    m_multitrack->setFocusPolicy( QWidget::ClickFocus );
    m_multitrack->setFPS( preview( )->isPAL() ? 25 : 30 );
    connect( m_multitrack, SIGNAL( positionChanged(int) ), editor( ), SLOT( sliderValueChanged( int ) ) );
    connect( m_multitrack, 
             SIGNAL( itemsMoved(Qt::ButtonState,const QValueList<MultiTrackItem*>&, int) ), 
             SLOT( multitrackItemsMoved(Qt::ButtonState,const QValueList<MultiTrackItem*>&, int) ) );
    connect( m_multitrack, 
             SIGNAL( trackSelected(Qt::ButtonState,MultiTrackTrack *) ), 
             SLOT( multitrackTrackSelected(Qt::ButtonState,MultiTrackTrack *) ) );
    connect( m_multitrack, SIGNAL( trackSelectEvent( QMouseEvent * ) ), SLOT( trackSelectEvent( QMouseEvent * ) ) );
    connect( m_multitrack, SIGNAL( itemSelectEvent( QMouseEvent * ) ), SLOT( itemSelectEvent( QMouseEvent * ) ) );
	connect( m_multitrack, SIGNAL( multitrackDragEnterEvent( QDragEnterEvent * ) ), SLOT( multitrackDragEnterEvent( QDragEnterEvent * ) ) );
	connect( m_multitrack, SIGNAL( multitrackDropEvent( QDropEvent * ) ), SLOT( multitrackDropEvent( QDropEvent * ) ) );
	connect( m_multitrack, SIGNAL( enableClicked( MultiTrackTrack * ) ), SLOT( multitrackEnableClicked( MultiTrackTrack * ) ) );
	connect( m_multitrack, SIGNAL( silenceClicked( MultiTrackTrack * ) ), SLOT( multitrackSilenceClicked( MultiTrackTrack * ) ) );
	connect( m_multitrack, SIGNAL( lockedClicked( MultiTrackTrack * ) ), SLOT( multitrackLockedClicked( MultiTrackTrack * ) ) );
}

PageProject::~PageProject( )
{
    delete m_paste;
}

void PageProject::multitrackDragEnterEvent( QDragEnterEvent *e )
{
	editor( )->getCore( )->customDragEnterEvent( e );
}

void PageProject::multitrackDropEvent( QDropEvent *e )
{
	editor( )->getCore( )->customDropEvent( e );
}

void PageProject::restart( )
{
    // Default add mode
    m_add_mode_box->setCurrentItem( 0 );
    
    // Set FPS
    TimecodeValueFormatter fmt( preview( )->isPAL() ? 25 : 30 );
    m_in_input->setFormatter( fmt );
    m_out_input->setFormatter( fmt );
    m_duration_output->setFormatter( TimecodeValueFormatter( preview( )->isPAL() ? 25 : 30, false ) );
}

void PageProject::refresh( )
{
    // Reset 'sliders' when the project is empty
    if ( editor( )->length( ) == 0 )
    {
        m_in_input->setRange( 0, 0 );
        m_out_input->setRange( 0, 0 );
        m_duration_output->setRange( 0, 0 );
        
        m_in_input->blockSignals( true );
        m_in_input->setValue( 0 );
        m_in_input->blockSignals( false );
        
        m_out_input->blockSignals( true );
        m_out_input->setValue( 0 );
        m_out_input->blockSignals( false );
        
        m_duration_output->setValue( 0 );
    }
    
    // Fetch the project to a local var
    Project *project = this->project( );
    int first_track = project->hasBackground( ) ? 1 : 0;
    
    // Ensure the selected track is valid
    if ( project->getActiveTrack( ) < first_track )
        project->setActiveTrack( first_track, NULL );
    else if ( project->getActiveTrack( ) >= project->count( ) )
        project->setActiveTrack( project->count( ) - 1, NULL );
    else
        project->setActiveTrack( project->getActiveTrack( ), NULL );
    
    // Clear the multitrack
    m_multitrack->deselectAll( );
    m_multitrack->clear( );
    
    // Set the width of the multitrack in frames
    if ( m_multitrack )
        m_multitrack->setLength( editor( )->length( ) );
    
    // Iterate through the tracks
    for ( int track_idx = first_track; track_idx < project->count( ); track_idx ++ )
    {
        // Attempt to get the first track as a producer
        Producer *producer = project->track( track_idx );
        
        // Convert to a track
        Playlist track( *producer );
        
        // Generate the title
        QString title;
        title.sprintf( "T%d", track_idx );
        
        // Construct the widget
        NleMultiTrackTrack *track_ui = new NleMultiTrackTrack( title, m_multitrack, track_idx );

		// Determine track level controls
		track_ui->setEnabled( ( producer->get_int( "hide" ) & 1 ) == 0 );
		track_ui->setSilence( ( producer->get_int( "hide" ) & 2 ) == 0 );
		track_ui->setLocked( producer->get_int( "jah_locked" ) != 0 );
        
        // Convenience
        bool track_selected = track_idx == project->getActiveTrack( );
        
        // Update the project
        if ( track_selected )
        {
            m_multitrack->setSelected( track_ui, true );
            project->setActiveTrack( track_idx, track_ui );
        }
        
        // Iterate through the cuts
        for ( int cut_idx = 0; cut_idx < track.count( ); cut_idx ++ )
        {
            // Get the info related to the cut
            ClipInfo *info = track.clip_info( cut_idx );
            
            // Create the item
            NleMultiTrackItem *item = new NleMultiTrackItem( info->producer->parent( ).get( "jah_title" ), track_ui, track_idx, info );
            
            // Set as selected
            bool clip_selected = editor( )->position( ) >= item->clipStart( ) && editor( )->position( ) < item->clipEnd( );
            if ( !info->producer->is_blank( ) )
                m_multitrack->setSelected( item, track_selected && clip_selected );
            
            // Update the sliders accordingly
            if ( track_selected && clip_selected )
            {
                int length = info->cut->get_int( "_max_length" ) != 0 ? info->cut->get_int( "_max_length" ) : info->cut->get_int( "length" );
                
                m_in_input->blockSignals( true );
                m_in_input->setRange( 0, info->frame_out );
                m_in_input->setValue( info->frame_in );
                m_in_input->blockSignals( false );
                
                m_out_input->blockSignals( true );
                m_out_input->setRange( info->frame_in, length - 1 );
                m_out_input->setValue( info->frame_out );
                m_out_input->blockSignals( false );
                
                m_duration_output->setRange( info->frame_count, info->frame_count );
                m_duration_output->setValue( info->frame_count );

                m_in_input->setEnabled( track.get_int( "jah_locked" ) == 0 );
                m_out_input->setEnabled( track.get_int( "jah_locked" ) == 0 );
            }
        }

        // Delete the reference to the producer
        delete producer;
    }
    
    // Force a redraw
    m_multitrack->update( );
}

void PageProject::setPosition( int value )
{
    // The multitrack slider
    m_multitrack->blockSignals( true );
    m_multitrack->setPosition( value );
    m_multitrack->blockSignals( false );
}

// Allow the user to add, insert, remove or move tracks
void PageProject::trackManipulations( )
{
    QPopupMenu *menu = new QPopupMenu( 0 );
    
    menu->insertItem( "Append", 1 );
    menu->insertItem( "Insert", 2 );
    menu->insertItem( "Remove", 3 );
    menu->insertItem( "Move Up", 4 );
    menu->insertItem( "Move Down", 5 );
    
    int id = menu->exec( m_tracks->mapToGlobal( QPoint( 0, m_tracks->height( ) + 1 ) ) );
    
    switch( id )
    {
    case 1:	addTrack( ); break;
    case 2:	insertTrack( ); break;
    case 3:	removeTrack( ); break;
    case 4:	moveTrackUp( ); break;
    case 5:	moveTrackDown( ); break;
    }
    
    delete menu;
}

// Add/Remove tracks
void PageProject::addTrack( )
{
    project( )->addTrack( );
}

void PageProject::insertTrack( )
{
    Project *new_project = project( )->insertTrack( );
    if ( new_project != NULL && new_project->is_valid( ) )
        editor( )->switchProject( new_project, editor( )->position( ) );
    else
        delete new_project;
    editor( )->checkPoint( );
}

void PageProject::removeTrack( )
{
    Project *new_project = project( )->removeTrack( );
    if ( new_project != NULL && new_project->is_valid( ) )
        editor( )->switchProject( new_project, editor( )->position( ) );
    else
        delete new_project;
    editor( )->checkPoint( );
}

void PageProject::moveTrackUp( )
{
    Project *new_project = project( )->moveTrackUp( );
    if ( new_project != NULL && new_project->is_valid( ) )
        editor( )->switchProject( new_project, editor( )->position( ) );
    else
        delete new_project;
    editor( )->checkPoint( );
}

void PageProject::moveTrackDown( )
{
    Project *new_project = project( )->moveTrackDown( );
    if ( new_project != NULL && new_project->is_valid( ) )
        editor( )->switchProject( new_project, editor( )->position( ) );
    else
        delete new_project;
    editor( )->checkPoint( );
}

void PageProject::setAddMode( int option )
{
    project( )->setAddMode( ( add_mode )option );
}

// Position modifier pushed and released
void PageProject::positionPushed( QMouseEvent *msev )
{
    switch ( msev->state( ) )
    {
    case ShiftButton:
        project( )->setKeyboardModifier( keyboard_shift );
        break;
    case ControlButton:
    case AltButton:
        project( )->setKeyboardModifier( keyboard_alt );
        break;
    default:
        project( )->setKeyboardModifier( keyboard_none );
        break;
    }
    
    editor( )->setSpeed( 0 );
    editor( )->sliderselected( );
    editor( )->ignoreChanges( true );
}

void PageProject::positionReleased( )
{
    project( )->setKeyboardModifier( keyboard_none );
    editor( )->ignoreChanges( false );
    editor( )->updateAnimation( );
}

// In/Out position changed
void PageProject::inPositionChanged( int val )
{
    project( )->changeInPoint( val );
}

void PageProject::outPositionChanged( int val )
{
    project( )->changeOutPoint( val );
}

// Zoom In/Out
void PageProject::zoomIn( )
{
    m_multitrack->setScaling( m_multitrack->scaling( ) * 2.0 );
    refresh( );
}

void PageProject::zoomOut( )
{
    m_multitrack->setScaling( m_multitrack->scaling( ) / 2.0 );
    refresh( );
}

void PageProject::resetZoom( )
{
    m_multitrack->setScaling( 1.0 );
    refresh( );
}

void PageProject::multitrackTrackSelected( Qt::ButtonState, MultiTrackTrack *object )
{
    NleMultiTrackTrack *track = ( NleMultiTrackTrack * )object;
    project( )->setActiveTrack( track->trackIndex( ), track );
	editor( )->checkPoint( );
	refresh( );
}

// Multitrack item position changed
void PageProject::multitrackItemsMoved( Qt::ButtonState, const QValueList<MultiTrackItem*>&items, int delta )
{
    if ( items.count( ) > 0 )
    {
        // Convert the item to the real type
        NleMultiTrackItem *nle_item = ( NleMultiTrackItem * )( items.first( ) );
        
        // TODO: Check button state and carry out different types of moves
        project( )->move( nle_item->clipIndex( ), delta );
    }
}

void PageProject::trackSelectEvent( QMouseEvent *e )
{
    if ( e->type( ) == QEvent::MouseButtonDblClick )
        editor( )->setPage( 2 );
}

void PageProject::itemSelectEvent( QMouseEvent *e )
{
    if ( e->type( ) == QEvent::MouseButtonDblClick )
        editor( )->setPage( 1 );
}

void PageProject::multitrackSilenceClicked( MultiTrackTrack *object )
{
	NleMultiTrackTrack *track = dynamic_cast< NleMultiTrackTrack * >( object );
	if ( track )
	{
		project( )->block( );
		Producer *producer = project( )->track( track->trackIndex( ) );
		int hide = producer->get_int( "hide" );
		producer->set( "hide", hide & 2 ? hide ^ 2 : hide | 2 );
		delete producer;
		project( )->unblock( );
	}
}

void PageProject::multitrackEnableClicked( MultiTrackTrack *object )
{
	NleMultiTrackTrack *track = dynamic_cast< NleMultiTrackTrack * >( object );
	if ( track )
	{
		project( )->block( );
		Producer *producer = project( )->track( track->trackIndex( ) );
		int hide = producer->get_int( "hide" );
		producer->set( "hide", hide & 1 ? hide ^ 1 : hide | 1 );
		delete producer;
		project( )->unblock( );
	}
}

void PageProject::multitrackLockedClicked( MultiTrackTrack *object )
{
	NleMultiTrackTrack *track = dynamic_cast< NleMultiTrackTrack * >( object );
	if ( track )
	{
		project( )->block( );
		Producer *producer = project( )->track( track->trackIndex( ) );
		producer->set( "jah_locked", !producer->get_int( "jah_locked" ) );
		delete producer;
		project( )->unblock( );
	}
}

void PageProject::copy( )
{
    delete m_paste;
    m_paste = project( )->getCurrentCut( );
    if ( m_paste == NULL || !m_paste->is_valid( ) || m_paste->is_blank( ) )
    {
        delete m_paste;
        m_paste = NULL;
    }
}

void PageProject::cut( )
{
    copy( );
    if ( m_paste )
        project( )->remove( );
}

void PageProject::paste( )
{
    if ( m_paste )
        project( )->addClone( *m_paste );
}

// Keyboard bindings
void PageProject::processKeyEvent( QKeyEvent *k )
{
    k->accept( );
    
    if ( k->state( ) == 0 )
    {
        switch ( toupper( k->key( ) ) )
        {
        case 'I': project( )->changeInPoint( ); editor( )->updateUiSettings( ); break;
        case 'O': project( )->changeOutPoint( ); editor( )->updateUiSettings( ); break;
        case 'S': project( )->split( ); break;
        case 'C': copy( ); break;
        case 'V': paste( ); break;
        case 'X': cut( ); break;
        case Key_Delete: project( )->remove( ); break;
        case Key_Up: project( )->setActiveTrack( project( )->getActiveTrack( ) - 1 ); editor( )->updateUiSettings( ); break;
        case Key_Down: project( )->setActiveTrack( project( )->getActiveTrack( ) + 1 ); editor( )->updateUiSettings( ); break;
        default: k->ignore( ); break;
        }
    }
    else if ( k->state( ) == ( ControlButton | AltButton ) )
    {
        switch ( toupper( k->key( ) ) )
        {
        case 'P': project( )->toggleDebug( ); break;
        default: k->ignore( ); break;
        }
    }
    else if ( k->state( ) == ControlButton )
    {
        switch ( toupper( k->key( ) ) )
        {
        case 'A': project( )->join( ); break;
        case 'M': project( )->mix( preview( )->isPAL( ) ? 25 : 30 ); break;
        case 'N': addTrack( ); break;
        case 'C': copy( ); break;
        case 'V': paste( ); break;
        case 'X': cut( ); break;
        case Key_Delete: project( )->blank( ); break;
        default: k->ignore( ); break;
        }
    }
    else if ( k->state( ) == AltButton )
    {
        switch ( toupper( k->key( ) ) )
        {
        case Key_Up: moveTrackUp( ); break;
        case Key_Down: moveTrackDown( ); break;
        default: k->ignore( ); break;
        }
    }
    else
    {
        k->ignore( );
    }
}

