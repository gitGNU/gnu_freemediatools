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
 *  $Id: Audio_IO.cpp,v 1.1 2005/06/15 11:24:20 jahshaka Exp $
 *
 **************************************************************************/

#include "Audio_IO.h"



#include "Audio_IO.h"

#include <stdio.h>
#include <limits.h>

using namespace std;
using namespace IO;


AUDIO_IO::AUDIO_IO(int _mode)
{
	mode = _mode;
	is_realtime = false;
	is_master = false;
	is_open = false;
	samplerate = 44100;
	channels = 2;
	callback = 0;
	buffer = 0;
	buffer_size = 0;
}

AUDIO_IO::~AUDIO_IO()
{
	if (buffer) {
		delete [] buffer;
	}
}

void AUDIO_IO::Set_Callback(unsigned long (*callback_func) (float* buf, unsigned long size, void* arg), void* user_arg)
{
	callback = callback_func;
	_user_arg = user_arg;
}

bool AUDIO_IO::Set_Mode(int val)	
{
	// We can't set mode when object is open
	if (Is_Open())
		return false;
	if (Get_Supported_Modes () & val) { // mode is supported
		mode = val;
		return true;
	} else
		return false;	
}

unsigned long AUDIO_IO::Read(float* buf, unsigned long size)
{
	unsigned long tmp_size, readed;
	
	byte* src;
	float* dst = buf;
	unsigned long nsamples;

		
	if (BPS() == 8) {
		tmp_size = size;
		
		if (buffer_size != tmp_size) {
			delete buffer;
			buffer = new byte[tmp_size];
			buffer_size = tmp_size;
		}
		
		src = buffer;

		readed = Read_Raw(buffer, buffer_size);
		nsamples = readed;
		
		while (nsamples--) {
			*dst = *src/255.0f;
			dst++;
			src++;
		}
		
		return readed;
		
	} else if (BPS() == 16) {
		tmp_size = size*2;
		
		if (buffer_size != tmp_size) {
			delete buffer;
			buffer = new byte[tmp_size];
			buffer_size = tmp_size;
		}
		
		src = buffer;
		
		readed = Read_Raw(buffer, buffer_size);
		nsamples = readed/2;

		while (nsamples--) {

			*dst = (*((short *) src))/32767.0f;
			dst++;
			src += 2;			
		}

		
		return readed/2;
		
	} else if (BPS() == 24) {
		tmp_size = size*4;
		
		if (buffer_size != tmp_size) {
			delete buffer;
			buffer = new byte[tmp_size];
			buffer_size = tmp_size;
		}
		
		src = buffer;


		readed = Read_Raw(buffer, buffer_size);
		nsamples = readed/4;

		while (nsamples--) {
			*dst = (*((int*)src) >> 8)/8388607.0f;
			dst++;
			src += 4;
		}

		
		return readed/4;
	}
	return 0;
}

unsigned long AUDIO_IO::Write(float* buf, unsigned long size)
{
	unsigned long tmp_size, written;
	
	float* src = buf;
	byte* dst;
	unsigned long nsamples = size;
	
	if (BPS() == 8) {
		tmp_size = size;

		if (buffer_size != tmp_size) {
			delete buffer;
			buffer = new byte[tmp_size];
			buffer_size = tmp_size;
		}
		dst = buffer;

		while (nsamples--) {
			*dst = (byte) ((*src) * 255.0f);
			dst++;
			src++;
		}
		
		written = Write_Raw(buffer, buffer_size);

		
		return written;
		
	} else if (BPS () == 16) {
		tmp_size = size*2;
		
		if (buffer_size != tmp_size) {
			delete buffer;
			buffer = new byte[tmp_size];
			buffer_size = tmp_size;
		}
		dst = buffer;

		float val;		

		while (nsamples--) {
			val = *src;
			if (val > 1.0f)
				*((short *)dst) = SHRT_MAX;
			else if (val < -1.0f)
				*((short *)dst) = SHRT_MIN;
			else
				*((short *)dst) = (short) (val * 32767.0f);
			dst += 2;
			src++;
		}

		
		written = Write_Raw(buffer, buffer_size);
		
		return written/2;
		
	} else if (BPS () == 24) {
		tmp_size = size*4;
		
		if (buffer_size != tmp_size) {
			delete buffer;
			buffer = new byte[tmp_size];
			buffer_size = tmp_size;
		}
				
		dst = buffer;

		int val;
		
		while (nsamples--) {
			val = (int) ((*src) * 8388607.0f) << 8;
			if (val > INT_MAX)
				*((int*)dst) = INT_MAX;
			else if (val < INT_MIN)
				*((int*)dst) = INT_MIN;
			else
				*((int*)dst) = val;
			dst += 4;
			src++;
		}
		
		written = Write_Raw(buffer,buffer_size);
		
		
		return written/4;
	}
	return 0;
}
