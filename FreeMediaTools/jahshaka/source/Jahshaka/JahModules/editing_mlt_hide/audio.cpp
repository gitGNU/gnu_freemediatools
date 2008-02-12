/*******************************************************************************
**
** The header file for the Jahshaka/MLT editing module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "audio.h"

#include <mlt++/Mlt.h>
using namespace Mlt;

#ifdef WIN32
#include <windows.h>
#endif // WIN32

#include <iostream>
#include <cstdlib>
#include <deque>
#include <string>

#include <openmedialib/ml/ml.hpp>
namespace ml = olib::openmedialib::ml;

//
// Library initialisation mechanism
//

namespace
{
	static ALCdevice *device = NULL;
	static ALCcontext *context = NULL;
	static int retry_count = 10;

	void reflib( int init )
	{
		static long refs = 0;

		if( init > 0 && ++refs == 1 )
		{
			// Initialise the openal libs
			if ( retry_count > 0 )
				device = alcOpenDevice( NULL );

			int err = alcGetError( device );

			if ( err == ALC_NO_ERROR )
			{
				context = alcCreateContext( device, NULL );
				if ( context != NULL )
					alcMakeContextCurrent( context );
			}
			else
			{
				// Nice one openal - crashes if i do this, leaks if i don't
				//alcCloseDevice( device );
				device = NULL;
				retry_count --;
			}
		}
		else if( init < 0 && --refs == 0 )
		{
			// Uninitialise
			if ( context != NULL )
			{
				alcDestroyContext( context );
				alcCloseDevice( device );
				context = NULL;
				device = NULL;
			}
		}
	}
}

namespace jahnle
{
	MltAudio::MltAudio( ) 
		: fps_( 25.0 )
		, channels_( 2 )
		, frequency_( 44100 )
	{
		timer_.reset( );

		// Initialise
		reflib( 1 );

		if ( context != NULL )
		{
			// Create an arbitrary number of buffers
			for ( size_t i = 0; i < BUFFER_COUNT; i ++ )
			{
				ALuint buffer;
   				alGenBuffers( 1, &buffer );
					buffers_.push_back( buffer );
			}

			// Initialise the source
   			alGenSources( 1, &source_ );
			alSourcei( source_, AL_LOOPING, AL_FALSE );
   			alSource3f( source_, AL_POSITION, 0.0, 0.0, 0.0);
   			alSource3f( source_, AL_VELOCITY, 0.0, 0.0, 0.0);
   			alSource3f( source_, AL_DIRECTION, 0.0, 0.0, 0.0);
   			alSourcef( source_, AL_ROLLOFF_FACTOR, 0.0 );
   			alSourcei( source_, AL_SOURCE_RELATIVE, AL_TRUE );
		}
	}

	MltAudio::~MltAudio( )
	{
		if ( context != NULL )
		{
			// Clean up all the buffers
			flush( );
			while( buffers_.size( ) )
			{
				alDeleteBuffers( 1, &( *buffers_.begin( ) ) );
				buffers_.pop_front( );
			}
			alDeleteSources( 1, &source_ );
		}

		// Uninitialise
		reflib( -1 );
	}

	void MltAudio::setRender( double fps, int channels, int frequency )
	{
		fps_ = fps;
		channels_ = channels;
		frequency_ = frequency;
	}

	void MltAudio::play( )
	{
		if ( !init( ) ) return;
   		ALenum state;
   		alGetSourcei( source_, AL_SOURCE_STATE, &state );
		if ( state != AL_PLAYING )
			alSourcePlay( source_ );
	}

	void MltAudio::sleep( int microseconds )
	{
		value_type time = { microseconds / 1000000, microseconds % 1000000 };
		timer_.sleep( time );
	}

	void MltAudio::recover( )
	{
		if ( !init( ) ) return;

		int processed;

		do
		{
			// Determine how many buffers are processed
   			alGetSourcei( source_, AL_BUFFERS_PROCESSED, &processed );

			// Special case - if we have no buffers processed and no more available 
			// for use, then we need to wait until we recover at least one
			if ( processed == 0 && buffers_.size( ) == 0 )
				sleep( 10000 );
		}
		while ( processed == 0 && buffers_.size( ) == 0 );

		// Unqueue the processed buffers and return them to our pool
   		while( processed -- )
   		{
			ALuint buffer;
			alSourceUnqueueBuffers( source_, 1, &buffer );
			buffers_.push_back( buffer );
   		}
	}

	bool MltAudio::init( )
	{
		return context != NULL;
	}

	bool MltAudio::push( Frame &frame )
	{
		if ( !init( ) ) return false;

		int16_t *audio = NULL;
		mlt_audio_format format = mlt_audio_pcm;
		int frequency = frequency_;
		int channels = channels_;
		int samples = mlt_sample_calculator( float( fps_ ), frequency, frame.get_int( "_position" ) );

		// Start playing automatically once we have no buffers left
		if ( buffers_.size( ) == 0 )
			play( );

		// Recover any played out buffers
		recover( );

		audio = frame.get_audio( format, frequency, channels, samples );

		if ( audio != NULL && samples != 0 )
		{
			// TODO: complete mapping from audio type (combination of af and channels)
   			if( channels == 1 )
				format_ = AL_FORMAT_MONO16;
			else
				format_ = AL_FORMAT_STEREO16;

			// Copy from the frame and queue the audio
 			alBufferData( *buffers_.begin( ), format_, audio, samples * channels * 2, frequency );
			alSourceQueueBuffers( source_, 1, &( *buffers_.begin( ) ) );
			buffers_.pop_front( );
		}

		return audio != NULL;
	}

	bool MltAudio::push( ml::frame_type_ptr frame )
	{
		if ( !init( ) ) return false;

		// Get the audio from the frame
		ml::audio_type_ptr aud = frame->get_audio( );

		// Recover any played out buffers
		recover( );

		// If we have audio in this frame then schedule for playout
   		if ( aud != 0 && buffers_.size( ) > 0 )
		{
			// TODO: complete mapping from audio type (combination of af and channels)
   			if( aud->channels( ) == 1 )
				format_ = AL_FORMAT_MONO16;
			else
				format_ = AL_FORMAT_STEREO16;

			// Quick hack for 5.1 support
			if ( aud->channels( ) == 6 )
			{
				typedef ml::audio< unsigned char, ml::pcm16 > pcm16_audio_type;
				int frequency = aud->frequency( );
				int samples = aud->samples( );
				short *buffer = ( short * )aud->data( );
				ml::audio_type_ptr stereo = ml::audio_type_ptr( new ml::audio_type( pcm16_audio_type( frequency, 2, samples ) ) );
				short *new_buffer = ( short * )stereo->data( );
				buffer += 2;
				while ( samples -- )
				{
					*new_buffer ++ = *buffer ++;
					*new_buffer ++ = *buffer ++;
					buffer += 4;
				}
				aud = stereo;
			}

			// Copy from the frame and queue the buffer
#		ifdef SWAB_AUDIO 
			short* buf = ( short* ) aud->data( );
			for( int i = 0; i < aud->size( ) / sizeof( short ); ++i )
				buf[ i ] = ( buf[ i ] >> 8 ) | ( buf[ i ] << 8 );
#		endif
 			alBufferData( *buffers_.begin( ), format_, aud->data( ), aud->size( ), aud->frequency( ) );
			alSourceQueueBuffers( source_, 1, &( *buffers_.begin( ) ) );
			buffers_.pop_front( );
		}

		// Make sure the source is playing
		if ( buffers_.size( ) == 0 )
			play( );

		return aud != 0;
	}

	void MltAudio::flush( )
	{
		if ( !init( ) ) return;
		alSourceStop( source_ );
		recover( );
	}

	void MltAudio::complete( )
	{
		if ( !init( ) ) return;
		// Recover all buffers
		recover( );
   		ALenum state;
   		alGetSourcei( source_, AL_SOURCE_STATE, &state );
		while ( state == AL_PLAYING && buffers_.size( ) < BUFFER_COUNT )
		{
			sleep( 10000 );
			recover( );
   			alGetSourcei( source_, AL_SOURCE_STATE, &state );
		}
		alSourceStop( source_ );
	}

	bool MltAudio::empty( )
	{
		if ( !init( ) ) return false;
   		ALenum state;
   		alGetSourcei( source_, AL_SOURCE_STATE, &state );
		return state == AL_PLAYING && buffers_.size( ) > 2;
	}
};
