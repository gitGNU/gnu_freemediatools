	
// il - A asset library representation.

// Copyright (C) 2000-2006 VM Inc.
// Released under the LGPL.
// For more information, see http://www.openassetlib.org

#include "Asset.h"

namespace olib
{
	namespace openassetlib
	{
		Asset::Asset(const std::string& path, const std::string& filename, unsigned long startFrame, unsigned long endFrame)
			:	_path(path),
				_filename(filename),
				_startFrame(startFrame),
				_endFrame(endFrame)
		{
		}

		Asset::~Asset(void)
		{
		}

		std::string Asset::path() const
		{
			return _path;
		}

		std::string Asset::filename() const
		{
			return _filename;
		}
/*
		std::string Asset::sequenceExtension() const
		{
			return _sequenceExtension;
		}
*/
		unsigned long Asset::startFrame() const
		{
			return _startFrame;
		}

		unsigned long Asset::endFrame() const
		{
			return _endFrame;
		}
/*
		DataSource::Type Asset::dataSourceType() const
		{
			return _dataSourceType;
		}

		std::string Asset::dataSourceURI() const
		{
			return _dataSourceURI;
		}
*/
	}
}

