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
 *  $Id: Audio_Dev.h,v 1.1 2005/06/15 11:24:20 jahshaka Exp $
 *
 **************************************************************************/

#ifndef __AUDIO_DEV_H
#define __AUDIO_DEV_H

//#include "config.h"
#include "Audio_IO.h"

namespace IO {
	
class AUDIO_DEV : public AUDIO_IO {
public:
	inline virtual string Name() const = 0;
	inline virtual string Description() const = 0;

	
	virtual void Start() = 0;
	virtual void Stop() = 0;

	virtual int Get_Supported_Modes();

	virtual int Open() = 0;

	virtual void Close() = 0;

	virtual unsigned long Read_Raw(byte* buf, unsigned long size) = 0;
	virtual unsigned long Write_Raw(byte* buf, unsigned long size) = 0;

	inline bool Is_Started() const { return is_started; };
	
	/**
	 *  Let x-runs:
	 *             - if reads from device - loose untaken internal buffer after timeout
	 *             - if writes in device - give to device empty buffer
	 */
	inline void Let_Xruns(bool val) { let_xruns = val; };
	inline bool Let_Xruns() const { return let_xruns; };

	AUDIO_DEV();
	virtual ~AUDIO_DEV();
protected:
	bool is_started;
	bool let_xruns;
};

};//namespace IO

#endif /* __AUDIO_DEV_H */
