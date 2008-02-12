/*******************************************************************************
**
** The header file for the Jahshaka/MLT editing module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef JAHNLE_MLTAUDIO_H
#define JAHNLE_MLTAUDIO_H

#include <openpluginlib/pl/timer.hpp>
#include <openmedialib/ml/ml.hpp>

#include <iostream>
#include <cstdlib>
#include <deque>
#include <string>

#if defined ( WIN32 ) || defined ( OPENALFRAMEWORK )
#include <al.h>
#include <alc.h>
#else
//Mike: we dont have AL here. use an ifdef
//#include <AL/al.h>
//#include <AL/alc.h>
#endif

#if !defined( GCC_VERSION ) && defined( __GNUC__ )
#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#endif

namespace Mlt
{
	class Frame;
}

namespace jahnle
{
	class MltAudio
	{
		public:
			MltAudio( );
			~MltAudio( );
			void setRender( double fps, int channels, int frequency );
			void play( );
			void sleep( int microseconds );
			void recover( );
			bool init( );
			bool push( Mlt::Frame &frame );
			bool push( olib::openmedialib::ml::frame_type_ptr );
			void flush( );
			void complete( );
			bool empty( );

		private:
			static const size_t BUFFER_COUNT = 8;
			double fps_;
			int channels_;
			int frequency_;
			std::deque < ALuint > buffers_;
        	ALuint source_;
        	ALenum format_;
#if defined WIN32 || ( GCC_VERSION >= 40000 && !defined __APPLE__ ) // GC - shouldn't this Apple thing be PPC?
		typedef olib::openpluginlib::rdtsc_default_timer default_timer;
#else
		typedef olib::openpluginlib::gettimeofday_default_timer default_timer;
#endif
		typedef default_timer::value_type value_type;
		default_timer timer_;
	};
}

#endif
