/*******************************************************************************
**
** The source file for the Jahshaka/MLT editing module settings page
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "pagesettings.h"
using namespace jahnle;

#include "edit.h"
#include "project.h"
#include "preview.h"
#include <jahformatter.h>

#include <qlabel.h>
#include <qbutton.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qvbox.h>

PageSettings::PageSettings( QWidget *parent, GLEdit *edit ) 
    : Page( parent, edit )
{
    QVBoxLayout* top = new QVBoxLayout( this );
    JahFormatter::setMarginAndSpacingLarge( top );

    QHBoxLayout* mainLayout = new QHBoxLayout();
    top->addLayout( mainLayout );
    top->addStretch();

    // col1
    QVBox* column1 = new QVBox( this );
    mainLayout->addWidget( column1 );

    // New project
    m_new_project = new QPushButton( column1, "new_project" );
    m_new_project->setFocusPolicy( QWidget::NoFocus );
    m_new_project->setText( "&Clear Project" );
    connect( m_new_project, SIGNAL( clicked( ) ), editor( ), SLOT( newProject( ) ) );
    
    // Load project from file
    m_open_project = new QPushButton( column1, "open_project" );
    m_open_project->setFocusPolicy( QWidget::NoFocus );
    m_open_project->setText( "&Open Project" );
    connect( m_open_project, SIGNAL( clicked( ) ), editor( ), SLOT( openProject( ) ) );
    
    // Save project to file
    m_save_project = new QPushButton( column1, "save_project" );
    m_save_project->setFocusPolicy( QWidget::NoFocus );
    m_save_project->setText( "&Save Project" );
    connect( m_save_project, SIGNAL( clicked( ) ), editor( ), SLOT( saveProject( ) ) );
    
    // Save project to desktop
    m_save_desktop = new QPushButton( column1, "save_desktop" );
    m_save_desktop->setFocusPolicy( QWidget::NoFocus );
    m_save_desktop->setText( "Sto&re on Desktop" );
    connect( m_save_desktop, SIGNAL( clicked( ) ), editor( ), SLOT( saveDesktop( ) ) );

    mainLayout->addWidget( JahFormatter::createSeparator( this ) );

    // col2
    QVBox* column2 = new QVBox( this );
    mainLayout->addWidget( column2 );

    // Allow runtime switching of PAL/NTSC
    QLabel *label = new QLabel( "Normalisation", column2 );
    label->setAlignment( int( QLabel::AlignCenter ) );
    m_normalisation_box = new QComboBox( FALSE, column2, "normalisation_box" );
    m_normalisation_box->setFocusPolicy( QWidget::NoFocus );
    preview( )->populateNormalisation( m_normalisation_box );
    connect( m_normalisation_box, SIGNAL( activated( int ) ), SLOT( setNormalisation( int ) ) );

    // Progressive or interlaced output
    label = new QLabel( "Deinterlace", column2 );
    label->setAlignment( int( QLabel::AlignCenter ) );
    m_progressive_box = new QComboBox( FALSE, column2, "progessive_box" );
    m_progressive_box->setFocusPolicy( QWidget::NoFocus );
    preview( )->populateProgressive( m_progressive_box );
    connect( m_progressive_box, SIGNAL( activated( int ) ), SLOT( setProgressive( int ) ) );
    
    mainLayout->addWidget( JahFormatter::createSeparator( this ) );

    // col3
    QVBox* column3 = new QVBox( this );
    mainLayout->addWidget( column3 );

    // Rescaling options (quality, performance)
    label = new QLabel( "Rescaling", column3 );
    label->setAlignment( int( QLabel::AlignCenter ) );
    m_rescale_box = new QComboBox( FALSE, column3, "rescale_box" );
    m_rescale_box->setFocusPolicy( QWidget::NoFocus );
    preview( )->populateRescale( m_rescale_box );
    connect( m_rescale_box, SIGNAL( activated( int ) ), SLOT( setRescale( int ) ) );
    
    // Software scaling
    label = new QLabel( "Resolution", column3 );
    label->setAlignment( int( QLabel::AlignCenter ) );
    m_resolution_box = new QComboBox( FALSE, column3, "resolution_box" );
    m_resolution_box->setFocusPolicy( QWidget::NoFocus );
    preview( )->populateResolution( m_resolution_box );
    connect( m_resolution_box, SIGNAL( activated( int ) ), SLOT( setResolution( int ) ) );
    
    mainLayout->addWidget( JahFormatter::createSeparator( this ) );

    // col4
    QVBox* column4 = new QVBox( this );
    mainLayout->addWidget( column4 );

    // Aspect ratio
    label = new QLabel( "Aspect Ratio", column4 );
    label->setAlignment( int( QLabel::AlignCenter ) );
    m_aspect_box = new QComboBox( FALSE, column4, "aspect_box" );
    m_aspect_box->setFocusPolicy( QWidget::NoFocus );
    preview( )->populateAspect( m_aspect_box );
    connect( m_aspect_box, SIGNAL( activated( int ) ), SLOT( setAspect( int ) ) );
    
    // Audio options
    label = new QLabel( "Audio Resampling", column4 );
    label->setAlignment( int( QLabel::AlignCenter ) );
    m_audio_box = new QComboBox( FALSE, column4, "audio_box" );
    m_audio_box->setFocusPolicy( QWidget::NoFocus );
    preview( )->populateAudio( m_audio_box );
    connect( m_audio_box, SIGNAL( activated( int ) ), SLOT( setAudio( int ) ) );

    mainLayout->addWidget( JahFormatter::createSeparator( this ) );

    // col5
    QVBox* column5 = new QVBox( this );
    mainLayout->addWidget( column5 );

    // Audio options
    label = new QLabel( "Audio Driver", column5 );
    label->setAlignment( int( QLabel::AlignCenter ) );
    m_audio_driver_box = new QComboBox( FALSE, column5, "audio_driver_box" );
    m_audio_driver_box->setFocusPolicy( QWidget::NoFocus );
    preview( )->populateAudioDriver( m_audio_driver_box );
    connect( m_audio_driver_box, SIGNAL( activated( int ) ), SLOT( setAudioDriver( int ) ) );
    
    // Playback mode options
    label = new QLabel( "Preview Audio", column5 );
    label->setAlignment( int( QLabel::AlignCenter ) );
    m_mode_box = new QComboBox( FALSE, column5, "mode_box" );
    m_mode_box->setFocusPolicy( QWidget::NoFocus );
    preview( )->populateMode( m_mode_box );
    connect( m_mode_box, SIGNAL( activated( int ) ), SLOT( setMode( int ) ) );
}

void PageSettings::restart( )
{
    preview( )->populateNormalisation( m_normalisation_box );
    preview( )->populateProgressive( m_progressive_box );
    preview( )->populateRescale( m_rescale_box );
    preview( )->populateResolution( m_resolution_box );
    preview( )->populateAspect( m_aspect_box );
    preview( )->populateAudio( m_audio_box );
    preview( )->populateAudioDriver( m_audio_driver_box );
    preview( )->populateMode( m_mode_box );
}

void PageSettings::refresh( )
{
	// Disable or enable normalisation switch
	m_normalisation_box->setEnabled( editor( )->length( ) == 0 );

	// Disable Save desktop when project is empty
	m_save_desktop->setEnabled( editor( )->length( ) != 0 );
}

// Settings related slots
void PageSettings::setNormalisation( int pal )
{
	// Stop the preview
	preview( )->stop( );

	// Set the pal/ntsc option requested
	preview( )->setPAL( pal );

	// Create a new project (destroying old in the process)
	editor( )->createProject( );
	
	// Start the preview
	preview( )->start( );
}

void PageSettings::setProgressive( int option )
{
	preview( )->stop( );
	preview( )->setProgressive( option );
	preview( )->start( );
	project( )->refresh( );
}

void PageSettings::setRescale( int option )
{
	preview( )->stop( );
	preview( )->setRescale( option );
	preview( )->start( );
	project( )->refresh( );
}

void PageSettings::setAspect( int option )
{
	preview( )->stop( );
	preview( )->setAspect( option );
	preview( )->start( );
	project( )->refresh( );
}

void PageSettings::setResolution( int option )
{
	preview( )->stop( );
	preview( )->setSize( option );
	preview( )->start( );
	project( )->refresh( );
}

void PageSettings::setAudio( int option )
{
	preview( )->stop( );
	preview( )->setAudio( option );
	preview( )->start( );
	project( )->refresh( );
}

void PageSettings::setAudioDriver( int option )
{
	preview( )->stop( );
	preview( )->setAudioDriver( option );
	preview( )->start( );
	project( )->refresh( );
}

void PageSettings::setMode( int option )
{
	preview( )->stop( );
	preview( )->setMode( option );
	preview( )->start( );
	project( )->refresh( );
}

