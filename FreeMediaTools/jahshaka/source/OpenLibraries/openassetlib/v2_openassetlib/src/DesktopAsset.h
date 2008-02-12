	
// il - A asset library representation.

// Copyright (C) 2000-2006 VM Inc.
// Released under the LGPL.
// For more information, see http://www.openassetlib.org

#pragma once
#include "Asset.h"

namespace olib
{
	namespace openassetlib
	{
		class DesktopAsset : public Asset
		{
		public:
			DesktopAsset(	const std::string& path, 
							const std::string& filename, 
							unsigned long startFrame, 
							unsigned long endFrame, 
							const std::string& name, 
							int xPos, 
							int yPos, 
							int xSize, 
							int ySize, 
							int currentFrameOffset, 
							bool filmstripMode);
			~DesktopAsset(void);

			const std::string&	name()			const { return _name;			};
			int					xPosition()		const { return _xPos;			};
			int					yPosition()		const { return _yPos;			};
			int					xSize()			const { return _xSize;			};
			int					ySize()			const { return _ySize;			};
			int					frameOffset()	const { return _frameOffset;	};
			bool				filmstripMode()	const { return _filmstripMode;	};

		private:
			std::string	_name;
			int			_xPos;
			int			_yPos;
			int			_xSize;
			int			_ySize;
			int			_frameOffset;
			bool		_filmstripMode;
		};
	}
}

