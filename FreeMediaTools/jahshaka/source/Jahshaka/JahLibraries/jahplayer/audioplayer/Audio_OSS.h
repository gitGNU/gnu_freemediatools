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
 *  $Id: Audio_OSS.h,v 1.2 2005/12/05 20:02:58 glslang Exp $
 *
 **************************************************************************/

#ifndef __AUDIO_OSS_H
#define __AUDIO_OSS_H

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#if !defined ( __sgi )
#include <sys/soundcard.h>
#endif
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string>


//#include "config.h"
#include "Audio_Dev.h"
#include <pthread.h>

using std::string;

namespace IO {

class AUDIO_OSS : public AUDIO_DEV {
public:
	inline virtual string Name() const {return "OSS device"; };
	inline virtual string Description()const { return "OSS Audio driver"; };

	inline virtual int Get_Supported_Modes() { return OCT_IO_MODE_READ | OCT_IO_MODE_WRITE;};

	virtual void Start();
	virtual void Stop();
	
	virtual int Open();
	virtual void Close();

	virtual unsigned long Read_Raw(byte* buf, unsigned long size);
	virtual unsigned long Write_Raw(byte* buf, unsigned long size);

	virtual void Set_Master(bool val);
	virtual void Set_Realtime(bool val);
	virtual void Set_Samplerate (int val);
	virtual void Set_Channels (int val);
	virtual void Set_BPS (int val);
	virtual void Set_DeviceName (string val);
	inline virtual string DeviceName () const { return dev_fname; };

	inline unsigned long  FragmentSize() const { return fragment_size;};
	void  FragmentSize(unsigned long val);
	void  BufferSize (unsigned long val) { FragmentSize (val);}
	unsigned long  BufferSize () const { return FragmentSize ();}

	int NumBlocks () const { return num_blocks; }
	void NumBlocks (int val);

	void SoundProc ();
	static void _SoundProc (void* arg);
	
	AUDIO_OSS(const string& name = "/dev/dsp");
	virtual ~AUDIO_OSS();
protected:
		
	int fragment_size;
	int num_blocks;
	
	int device_handle;
	string dev_fname;

	pthread_t soundproc_thread;
};

};//namespace IO

#endif /* __AUDIO_OSS_H */
