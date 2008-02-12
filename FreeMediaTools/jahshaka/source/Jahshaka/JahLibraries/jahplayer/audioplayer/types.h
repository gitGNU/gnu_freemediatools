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
 *  $Id: types.h,v 1.1 2005/06/15 11:24:21 jahshaka Exp $
 *
 **************************************************************************/

#ifndef __TYPES_H
#define __TYPES_H

namespace IO {
	
/**
 *  read/write capabilities.
 *  They can be combined via bitwice-OR.
 */
enum IO_Mode {
	OCT_IO_MODE_READ = 1, // Only for reading
	OCT_IO_MODE_WRITE = 2, // Only for writing
	OCT_IO_MODE_READWRITE = 4 // aka duplex mode
};

/**
 *  Open error index values
 */
enum Open_Error {
	OCT_OPEN_ERR_SAMPLE_FORMAT = 1,
	OCT_OPEN_ERR_SAMPLE_RATE,
	OCT_OPEN_ERR_CHANNELS,
	OCT_OPEN_ERR_MODE,
	OCT_OPEN_ERR_REALTIME,
	OCT_OPEN_ERR_OPEN,
	OCT_OPEN_ERR_UNKNOWN
};

enum FormatType {
	OCT_FMT_S16 = 0,
	OCT_FMT_U8,
	OCT_FMT_FLOAT
};

typedef unsigned char byte;

};//namespace IO

#endif /* __TYPES_H */
