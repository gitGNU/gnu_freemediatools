	
// il - A asset library representation.

// Copyright (C) 2000-2006 VM Inc.
// Released under the LGPL.
// For more information, see http://www.openassetlib.org

#include "DesktopAsset.h"

namespace olib
{
	namespace openassetlib
	{
		DesktopAsset::DesktopAsset(	const std::string& path, 
									const std::string& filename, 
									unsigned long startFrame, 
									unsigned long endFrame, 
									const std::string& name, 
									int xPos, 
									int yPos, 
									int xSize, 
									int ySize, 
									int frameOffset, 
									bool filmstripMode)
			:	Asset(path, filename, startFrame, endFrame),
				_name(name),
				_xPos(xPos),
				_yPos(yPos),
				_xSize(xSize),
				_ySize(ySize),
				_frameOffset(frameOffset),
				_filmstripMode(filmstripMode)
		{
		}

		DesktopAsset::~DesktopAsset(void)
		{
		}
	}
}

