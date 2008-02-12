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
 *  $Id: Audio_Dev.cpp,v 1.1 2005/06/15 11:24:20 jahshaka Exp $
 *
 **************************************************************************/

#include "Audio_IO.h"
#include "Audio_Dev.h"

using namespace std;
using namespace IO;

AUDIO_DEV::AUDIO_DEV():
	AUDIO_IO(OCT_IO_MODE_READWRITE | OCT_IO_MODE_READ | OCT_IO_MODE_WRITE),
	is_started(false),
	let_xruns(true)	
{
	
}

AUDIO_DEV::~AUDIO_DEV()
{
	
}

int AUDIO_DEV::Get_Supported_Modes()
{
	return OCT_IO_MODE_READWRITE | OCT_IO_MODE_READ | OCT_IO_MODE_WRITE;
}
