/***************************************************************************
 *  Copyright (C) 2003 Roman Kaljakin
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  $Id: Audio_OSS.cpp,v 1.3 2005/12/05 20:02:58 glslang Exp $
 *
 **************************************************************************/

#include "Audio_OSS.h"


#include "Audio_IO.h"
#include "Audio_Dev.h"
#include "Audio_OSS.h"
#include <math.h>
#include <iostream>

using namespace std;
using namespace IO;

AUDIO_OSS::AUDIO_OSS(const string& name)
{
	dev_fname = name;
	device_handle = 0;
	bits_per_sample = 16;
	samplerate = 44100;
	channels = 2;
	mode = OCT_IO_MODE_WRITE;
	fragment_size = 512;
	num_blocks = 8;
}

AUDIO_OSS::~AUDIO_OSS()
{
	Stop();
	Close();
}

int AUDIO_OSS::Open()
{
#if !defined ( __sgi )
	if (Is_Open()) return 0;	
	
	switch (Mode ()) {
	case OCT_IO_MODE_READ:
		if ((device_handle = ::open (dev_fname.c_str(), O_RDONLY, 0)) == -1)
			return OCT_OPEN_ERR_OPEN;
		break;
	case OCT_IO_MODE_WRITE:
		if ((device_handle = ::open (dev_fname.c_str(), O_WRONLY, 0)) == -1)
			return OCT_OPEN_ERR_OPEN;
		break;
	default:
		return OCT_OPEN_ERR_MODE; // Duplex mode not supported
		
	}

	int result = 0;
	
//    	result = ioctl(device_handle,SNDCTL_DSP_RESET,NULL);
//    	if (result == -1) return OCT_OPEN_ERR_UNKNOWN;

//   	int val = AFMT_S16_NE;
//   	result = ioctl(device_handle,SNDCTL_DSP_SETFMT,&val);
//   	if (result == -1) return OCT_OPEN_ERR_UNKNOWN;

	
 	int stereo = channels == 2?1:0;
 	result = ioctl(device_handle, SNDCTL_DSP_STEREO, &stereo);
 	if (result == -1) return OCT_OPEN_ERR_CHANNELS;

	//cout<<" -- "<<result<<endl;
	
 	result = ioctl(device_handle, SNDCTL_DSP_SPEED, &samplerate);
 	if (result == -1) return OCT_OPEN_ERR_SAMPLE_RATE;

	
 	result = ioctl(device_handle, SNDCTL_DSP_SAMPLESIZE, &bits_per_sample);
 	if (result == -1) return OCT_OPEN_ERR_SAMPLE_FORMAT;
	
	//cout<<bits_per_sample<<" - sample format"<<endl;
	
	int value;
	if (fragment_size == 0) {
		if (num_blocks == 0)
			value = 0x7fff0009;
		else {			
			value = (num_blocks << 16) | 0x0009;
		}
	} else {
		unsigned long tmp = fragment_size;
		int cnt = 0;
		while (tmp != 1) {
			cnt++;
			tmp /= 2;
		}

		if (num_blocks == 0)
			value = (0x7fff<<16) | (int) cnt;
		else {
			value = (num_blocks << 16) | (int) cnt;			
		}
	}

#ifdef _DEBUG
	printf ("SNDCTL_DSP_SETFRAGMENT : %#x\n", value);
#endif //_DEBUG

 	result = ioctl(device_handle, SNDCTL_DSP_SETFRAGMENT, &value);
 	if (result == -1) return OCT_OPEN_ERR_UNKNOWN;
	
	is_open = true;
	
	return 0;	
#else
	return OCT_OPEN_ERR_UNKNOWN;
#endif
}

void AUDIO_OSS::Close()
{
	if (Is_Open ()) {
		is_open = false;
		close(device_handle);
	}	
}
/**
 *  Start read/write process via callback function.
 */
void AUDIO_OSS::Start()
{
	if (!Master () || callback == 0)
		return;

	is_started = true;

	//FIXME:  pthread_create return nonzero (error) but
	//thread started
	if (pthread_create(&soundproc_thread, 0, (void* (*) (void*)) _SoundProc, (void*)this)){
//#ifdef _DEBUG
		cout<<"audio start failed.. \n";
//#endif //_DEBUG
		is_started = false;
		return;
	}		
}

void AUDIO_OSS::Stop()
{
	if (Is_Started())
		is_started = false;
}

unsigned long AUDIO_OSS::Read_Raw(byte* buf, unsigned long size)
{
	size_t sz = (unsigned long) size;
	size_t rd = read(device_handle, buf, sz);
	return (unsigned long) rd;
}

unsigned long AUDIO_OSS::Write_Raw(byte* buf, unsigned long size)
{
	size_t sz = (unsigned long) size;
	size_t wr = write(device_handle, buf, sz);
	return (unsigned long) wr;
}


void AUDIO_OSS::Set_Master(bool val)
{
	if (!Is_Started ())
		if (!(Mode() & OCT_IO_MODE_READWRITE))
			is_master = val;
}

void AUDIO_OSS::Set_Realtime(bool)
{
	// not supporteed yet
}

void AUDIO_OSS::Set_Samplerate(int val)
{
	/*
	// We can't change samplerate of input signal in OSS.
	// Maybe anywhere is a sound devices that can do resampling
	// of input signal.
	if (!(Mode () & OCT_IO_MODE_WRITE))
		return;	

	if (Is_Open()) {
		if (ioctl(device_handle,SNDCTL_DSP_SPEED,&val) != -1) {
			samplerate = val;
#ifdef _DEBUG
			printf ("SNDCTL_DSP_SPEED: %i\n", val);
#endif //_DEBUG
		} else {
#ifdef _DEBUG
			printf ("SNDCTL_DSP_SPEED failed\n");
#endif //_DEBUG		
		}
		
	} else
		samplerate = val;
	*/

	// XXX Doesn't OSS supports change of samplerate on-the-fly
	// (when device is opened) or I am doing something wrong?
	samplerate = val;
	Close ();
	Open ();	
}

void AUDIO_OSS::Set_Channels(int val)
{
#if !defined ( __sgi )
        // We can't change number of channels of input signal in OSS.
	// Maybe anywhere is a sound devices that can do converting
	// stereo to mono of input signal.
	if (!(Mode () & OCT_IO_MODE_WRITE))
			return;
	
	if (Is_Open ()) {		
		int stereo = val == 2?1:0;
		if (ioctl(device_handle,SNDCTL_DSP_STEREO,&stereo) != -1)
			channels = val;
	} else
		channels = val;
#endif
}

void AUDIO_OSS::Set_BPS(int val)
{
#if !defined ( __sgi )
	// We also can't change sample format of input signal in OSS.
	if (!(Mode () & OCT_IO_MODE_WRITE))
		return;

	if (Is_Open ()) {
		if(ioctl(device_handle,SNDCTL_DSP_SAMPLESIZE,&val) != -1)
			bits_per_sample = val;
	} else
		bits_per_sample = val;
#endif
}

void AUDIO_OSS::Set_DeviceName (string val)
{
	if (!Is_Open ())
		dev_fname = val;
}

void AUDIO_OSS::SoundProc ()
{
	float* buf;
	unsigned long size = 0;

	if (BPS() <= 8)
		size = fragment_size;
	else if (BPS() <= 16)
		size = fragment_size/2; 
	else if (BPS() <= 32)
		size = fragment_size/4;

	//cout<<" size is :"<<size<<endl;

	buf= new float [size];
	
	unsigned int readed;
	
	if (Mode() &  OCT_IO_MODE_READ) {
		while (Is_Started()) {
			if ((readed = Read(buf, size)) == 0)
				break;
			if (!callback(buf, readed, _user_arg))
				break;
		}
	}
	if (Mode() & OCT_IO_MODE_WRITE) {
		while (Is_Started()) {
			if ((readed = callback(buf, size, _user_arg)) == 0)
				break;
			if (Write (buf, readed) == 0)
				break;
		}
	}
	
	Stop();
	
}
void AUDIO_OSS::_SoundProc (void* arg)
{
	static_cast<AUDIO_OSS*> (arg)->SoundProc();
}

void  AUDIO_OSS::FragmentSize(unsigned long val)
{
/*
	unsigned long tmp = val;
	int cnt = 0;
	while (tmp != 1) {
		cnt++;
		tmp /= 2;
	}	
	
	int value;
	if (num_blocks == 0)
		value = (0x7fff << 16) | (int) cnt;
	else
		value = (((int) num_blocks) << 16) | (int) cnt;
	
   	if(ioctl(device_handle, SNDCTL_DSP_SETFRAGMENT, &value) != -1) {
  		fragment_size = val;
#ifdef _DEBUG
		printf ("SNDCTL_DSP_SETFRAGMENT : %#x\n", value);
#endif //_DEBUG
	} else {
#ifdef _DEBUG
		printf ("SNDCTL_DSP_SETFRAGMENT failed\n");
#endif //_DEBUG
	}
*/

	// XXX Doesn't OSS supports fragments parameters changing on-the-fly
	// (when device is opened) or I am doing something wrong?
	fragment_size = val;
	Close ();
	Open ();
}

void AUDIO_OSS::NumBlocks (int val)
{
/*
	unsigned long tmp = fragment_size;
	int cnt = 0;
	while (tmp != 1) {
		cnt++;
		tmp /= 2;
	}

	int value = (((int) val) << 16) | (int) cnt;
	
	if(ioctl(device_handle, SNDCTL_DSP_SETFRAGMENT, &value) != -1) {
		num_blocks = val;
#ifdef _DEBUG
		printf ("SNDCTL_DSP_SETFRAGMENT : %#x\n", value);
#endif //_DEBUG

	} else {
#ifdef _DEBUG
		printf ("SNDCTL_DSP_SETFRAGMENT failed\n");
#endif //_DEBUG
		
	}
*/
	
	// XXX Doesn't OSS supports fragments parameters changing on-the-fly
	// (when device is opened) or I am doing something wrong?
	num_blocks = val;
	Close ();
	Open ();
}
