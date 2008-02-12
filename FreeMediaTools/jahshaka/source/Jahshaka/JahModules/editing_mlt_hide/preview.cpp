/*******************************************************************************
 **
 ** The source file for the Jahshaka/MLT editing module preview abstraction
 ** The Jahshaka Project
 ** Copyright (C) 2000-2006 VM Inc.
 ** Released under the GNU General Public License
 **
 *******************************************************************************/

#include "base.h"
#include "edit.h"
#include "preview.h"
#include "project.h"
using namespace Mlt;
using namespace jahnle;

#include <qdir.h>

Preview::Preview( const QString &base )
	: m_play_mode( 0 )
{
	// Create temp home dir
	QString directory = base + QString( "editor" );
	QDir d( directory );
	if ( !d.exists( ) )
		d.mkdir( directory, true );

	// Specify the preference file
	m_file_name = directory + QString( "/edit.properties" );

	// Set the default properties
	m_user_prefs.set( "normalisation", 1 );
	m_user_prefs.set( "progressive", 1 );
	m_user_prefs.set( "resize", 1 );
	m_user_prefs.set( "rescale_idx", 2 );
	m_user_prefs.set( "aspect_idx", 0 );
	m_user_prefs.set( "size_idx", 0 );
	m_user_prefs.set( "audio_driver_idx", 0 );
	m_user_prefs.set( "audio_idx", 1 );
	m_user_prefs.set( "mode_idx", 0 );
	m_user_prefs.set( "auto_zoom", 0 );

	// Load the user preferences
	m_user_prefs.load( m_file_name.ascii( ) );
}

Preview::~Preview( ) 
{ 
	save( );
}

// Populate widgets
void Preview::populateNormalisation( QComboBox *box )
{
	box->blockSignals( true );
	box->clear( );
	box->insertItem( "NTSC" );
	box->insertItem( "PAL" );
	box->setCurrentItem( m_user_prefs.get_int( "normalisation" ) );
	box->blockSignals( false );
}

void Preview::populateProgressive( QComboBox *box ) 
{
	box->blockSignals( true );
	box->clear( );
	box->insertItem( "Interlaced" );
	box->insertItem( "Linear Blend" );
	box->insertItem( "Bob" );
	box->insertItem( "Weave" );
	box->insertItem( "One Field" );
	box->setCurrentItem( m_user_prefs.get_int( "progressive" ) );
	box->blockSignals( false );
}

void Preview::populateRescale( QComboBox *box )
{
	box->blockSignals( true );
	box->clear( );
	box->insertItem( "No Scaling" );
	box->insertItem( "Nearest" );
	box->insertItem( "Bilinear" );
	box->insertItem( "Tiles" );
	box->insertItem( "Hyper" );
	box->setCurrentItem( m_user_prefs.get_int( "rescale_idx" ) );
	box->blockSignals( false );
}

void Preview::populateAspect( QComboBox *box )
{
	box->blockSignals( true );
	box->clear( );
	box->insertItem( "4:3" );
	box->insertItem( "16:9" );
	box->setCurrentItem( m_user_prefs.get_int( "aspect_idx" ) );
	box->blockSignals( false );
}

void Preview::populateResolution( QComboBox *box )
{
	box->blockSignals( true );
	box->clear( );
	if ( isPAL( ) )
	{
		box->insertItem( "720x576" );
		box->insertItem( "540x432" );
		box->insertItem( "360x288" );
	}
	else
	{
		box->insertItem( "720x480" );
		box->insertItem( "540x360" );
		box->insertItem( "360x240" );
	}
	box->setCurrentItem( m_user_prefs.get_int( "size_idx" ) );
	box->blockSignals( false );
}

void Preview::populateAudio( QComboBox *box )
{
	box->blockSignals( true );
	box->clear( );
	box->insertItem( "Stereo 48khz" );
	box->insertItem( "Stereo 44.1khz" );
	box->setCurrentItem( m_user_prefs.get_int( "audio_idx" ) );
	box->blockSignals( false );
}

void Preview::populateAudioDriver( QComboBox *box )
{
	box->blockSignals( true );
	box->clear( );
	box->insertItem( "Default" );
	box->insertItem( "ALSA" );
	box->insertItem( "DSP" );
	box->insertItem( "Arts" );
	box->insertItem( "ESD" );
	box->setCurrentItem( m_user_prefs.get_int( "audio_driver_idx" ) );
	box->blockSignals( false );
}

void Preview::populateMode( QComboBox *box )
{
	box->blockSignals( true );
	box->clear( );
	box->insertItem( "Normal" );
	box->insertItem( "On" );
	box->insertItem( "Off" );
	box->setCurrentItem( m_user_prefs.get_int( "mode_idx" ) );
	box->blockSignals( false );
}

void Preview::setNormalisation( int norm )
{
	m_user_prefs.set( "normalisation", norm );
	setNormalisation( norm == 0 ? "NTSC" : "PAL" );
}

int Preview::getNormalisation( )
{
	return m_user_prefs.get_int( "normalisation" );
}

void Preview::setProgressive( int option )
{
	switch( option )
	{
		case 0: setDeinterlace( "none" ); break;
		case 1: setDeinterlace( "linearblend" ); break;
		case 2: setDeinterlace( "bob" ); break;
		case 3: setDeinterlace( "weave" ); break;
		case 4: setDeinterlace( "onefield" ); break;
		case 5: setDeinterlace( "greedy" ); break;
	}
	m_user_prefs.set( "progressive", option );
}

void Preview::setRescale( int option )
{
	switch( option )
	{
		case 0: setRescale( "none" ); break;
		case 1: setRescale( "nearest" ); break;
		case 2: setRescale( "bilinear" ); break;
		case 3: setRescale( "tiles" ); break;
		case 4: setRescale( "hyper" ); break;
	}
	m_user_prefs.set( "rescale_idx", option );
	m_user_prefs.set( "resize", option != 0 );
}

void Preview::setAspect( int option )
{
	double aspect_ratio = 0.0;
	double display_ratio = 0.0;

	// 4:3 and 16:9 calcs
	if ( isPAL( ) )
	{
		switch ( option )
		{
			case 0: aspect_ratio = 59.0 / 54.0; display_ratio = 4.0 / 3.0; break;
			case 1: aspect_ratio = 118.0 / 81.0; display_ratio = 16.0 / 9.0; break;
		}
	}
	else
	{
		switch ( option )
		{
			case 0: aspect_ratio = 10.0 / 11.0; display_ratio = 4.0 / 3.0; break;
			case 1: aspect_ratio = 40.0 / 33.0; display_ratio = 16.0 / 9.0; break;
		}
	}

	if ( aspect_ratio != 0.0 )
	{
		// Propogate aspect ratio to consumer
		setAspectRatio( aspect_ratio, display_ratio );

		// Ensure that it gets stored
		m_user_prefs.set( "aspect_idx", option );
	}
}

void Preview::setSize( int option )
{
	if ( isPAL( ) )
	{
		switch( option )
		{
			case 0:	setSize( 720, 576 ); break;
			case 1:	setSize( 540, 432 ); break;
			case 2:	setSize( 360, 288 ); break;
		}
	}
	else
	{
		switch( option )
		{
			case 0:	setSize( 720, 480 ); break;
			case 1:	setSize( 540, 360 ); break;
			case 2:	setSize( 360, 240 ); break;
		}
	}
	m_user_prefs.set( "size_idx", option );
}

void Preview::setAudio( int option )
{
	switch( option )
	{
		case 0: setAudioFrequency( 48000 ); break;
		case 1: setAudioFrequency( 44100 ); break;
	}
	m_user_prefs.set( "audio_idx", option );
}

void Preview::setAudioDriver( int option )
{
	switch( option )
	{
		case 0: setAudioDriver( ( const char * )NULL ); break;
		case 1: setAudioDriver( "alsa" ); break;
		case 2: setAudioDriver( "dsp" ); break;
		case 3: setAudioDriver( "artsc" ); break;
		case 4: setAudioDriver( "esd" ); break;
	}
	m_user_prefs.set( "audio_driver_idx", option );
}

void Preview::setMode( int option )
{
	switch( option )
	{
		case 0: setAudioMode( "normal" ); break; 
		case 1: setAudioMode( "on" ); break;
		case 2: setAudioMode( "off" ); break;
	}

	m_user_prefs.set( "mode_idx", option );
}

void Preview::setOptions( )
{
	// Match the settings of the UI
	setAspect( m_user_prefs.get_int( "aspect_idx" ) );
	setRescale( m_user_prefs.get_int( "rescale_idx" ) );
	setSize( m_user_prefs.get_int( "size_idx" ) );
	setProgressive( m_user_prefs.get_int( "progressive" ) );
	setAudio( m_user_prefs.get_int( "audio_idx" ) );
	setAudioDriver( m_user_prefs.get_int( "audio_driver_idx" ) );
	setMode( m_user_prefs.get_int( "mode_idx" ) );
	setAutoZoom( m_user_prefs.get_int( "auto_zoom" ) != 0 );
}

// Save the user preferences
void Preview::save( )
{
#ifdef WIN32
	m_user_prefs.save( m_file_name.ascii( ) );
#else
	FILE *output = fopen( m_file_name.ascii( ), "w" );
	m_user_prefs.dump( output );
	fclose( output );
#endif
}

// Store the preview profile
void Preview::storeProfile( Project *project )
{
	if ( project != NULL )
	{
		project->block( );
		project->set( "jah_project", 1 );
		project->set( "jah_deinterlace_method", m_consumer.get( "deinterlace_method" ) );
		project->set( "jah_consumer_deinterlace", m_consumer.get( "progressive" ) );
		project->set( "jah_rescale.interp", m_consumer.get( "rescale" ) );
		project->set( "jah_consumer_aspect_ratio", m_consumer.get( "aspect_ratio" ) );
		project->set( "jah_display_ratio", m_consumer.get( "display_ratio" ) );
		project->set( "jah_display_width", m_consumer.get( "width" ) );
		project->set( "jah_display_height", m_consumer.get( "height" ) );
		project->set( "jah_audio_frequency", m_consumer.get( "frequency" ) );
		project->unblock( );
	}
}

void Preview::setAudioFrequency( int frequency )
{ 
	m_consumer.set( "frequency", frequency );
	//delete m_audio;
	//m_audio = new MltAudio( );
	//m_audio->setRender( isPAL( ) ? 25.0 : 30000.0 / 1001.0, 2, frequency ); 
}

