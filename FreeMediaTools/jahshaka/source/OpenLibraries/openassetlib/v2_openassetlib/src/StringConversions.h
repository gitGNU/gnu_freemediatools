	
// il - A asset library representation.

// Copyright (C) 2000-2006 VM Inc.
// Released under the LGPL.
// For more information, see http://www.openassetlib.org

#pragma once
#include <stdio.h>

inline char* itoa(int value, char* buffer, int radix)
{
	radix = 10;
	sprintf(buffer, "%d", value);
	return buffer;
}
