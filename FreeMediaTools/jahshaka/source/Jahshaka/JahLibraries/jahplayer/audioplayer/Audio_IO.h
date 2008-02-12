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
 *  $Id: Audio_IO.h,v 1.1 2005/06/15 11:24:20 jahshaka Exp $
 *
 **************************************************************************/

#ifndef __AUDIO_IO_H
#define __AUDIO_IO_H

//#include "config.h"
#include "types.h"
#include <string>

using std::string;

namespace IO {

/**
 *  @class AUDIO_IO.
 *  
 *  Interface for all audio in/out devices, files, modules e.t.c
 *
 *  It consist of:
 *
 *          - Main read/write functions
 *
 *          - Parameters (mode, realtime infrormation, name)
 */

class AUDIO_IO {
public:
	/**
	 *  Get supported modes (bitwise OR)
	 */
	virtual int Get_Supported_Modes() = 0;

	/**
	 *  Open audio object
	 *
	 *  @return
	 *         - 0 - success
	 *         - nonzero - error IO_Error
	 */
	virtual int Open() = 0;
	virtual void Close() = 0;
	/**
	 * Read data in audio buffer
	 * @return size of readed data
	 */
	virtual unsigned long Read(float* buf, unsigned long size);
	/**
	 * Write data in audio buffer
	 * @return size of written data
	 */		
	virtual unsigned long Write(float* buf, unsigned long size);

	virtual unsigned long Read_Raw(byte* buf, unsigned long size) = 0;
	virtual unsigned long Write_Raw(byte* buf, unsigned long size) = 0;
	
	/**
	 *  Set callback function.      
	 */
	virtual void Set_Callback(unsigned long (*callback_func) (float* buf, unsigned long size, void* arg), void* user_arg);
	/**
	 *  Attributes
	 */
	inline bool Is_Open() const { return is_open; }
	
	virtual int Mode() const { return mode; };
	/**
	 *  Set mode.
	 *
	 *  @return
	 *         - true - success,
	 *         - false - mode not supported by this device
	 */
	virtual bool Set_Mode(int val);

	virtual inline  string Name() const = 0;
	virtual inline  string Description() const = 0;
	
	virtual inline  bool Realtime() const { return is_realtime; };
	virtual inline  void Realtime(bool val) {is_realtime = val; };
	
	/**
	 *  Object controls process of reading/writing using callback function
	 */
	virtual inline  bool Master() const { return is_master; };
	virtual inline  void SetMaster(bool val) { is_master = val; };

	virtual inline  void SetSamplerate (int val) { samplerate = val;};
	virtual inline  int Samplerate () const { return samplerate;};

	virtual inline  void SetChannels (int val) { channels = val;};
	virtual inline  int Channels () const { return channels;};

	virtual inline  void SetBPS (int val) { bits_per_sample = val;};
	virtual inline  int BPS () { return bits_per_sample;};
	
	virtual inline  unsigned long Buffersize () { return buffer_size;}


	AUDIO_IO(int _mode = OCT_IO_MODE_READ);
	virtual ~AUDIO_IO();
	
protected:
	
	int mode;
	bool is_realtime;
	bool is_master;
	bool is_open;

	byte* buffer;
	unsigned long buffer_size;
	
	int samplerate;
	int channels;
	int bits_per_sample;
	
	unsigned long (*callback) (float* buf, unsigned long size, void* arg);
	void* _user_arg;
};

};//namespace IO

#endif /* __AUDIO_IO_H */
