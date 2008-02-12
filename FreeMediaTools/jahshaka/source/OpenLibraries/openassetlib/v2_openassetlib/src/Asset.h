	
// OAL - A asset library representation.

// Copyright (C) 2000-2006 VM Inc.
// Released under the LGPL.
// For more information, see http://www.openassetlib.org

#pragma once
#include <string>
#include <ctime>
#include "DataSource.h"

#if !defined(_WIN32)
#define __int64 long long
#endif

namespace olib
{
	namespace openassetlib
	{
		class Asset
		{
		public:
			enum MediaType
			{
				//Undefined,
				AUDIO,
				STILL_IMAGE,
				IMAGE_SEQUENCE,
				VIDEO,
				SCENE_3D,
			};

			enum AudioFormat
			{
				//Undefined,
				WAV,
				AIFF,
				MP3,
				OGG,
			};

			enum ImageFormat
			{
				BMP,
				JPEG,
				PNG,
				GIF,
				TGA,
				RGB,
				PPM,
				EXR,
			};

			enum VideoFormat
			{
				//Undefined,
				MPEG,
				AVI,
				DV,
				MOV,
				DIF,
				WESTLEY,
				JFX,
				JEF,
				MPL,
			};
			
			enum VideoLineStandard
			{
				PAL,
				NTSC_DF,
				NTSC_NDF,
				SECAM,
			};

			Asset(const std::string& path, const std::string& filename, unsigned long startFrame, unsigned long endFrame);
			virtual ~Asset(void);

			// Accessor functions
			std::string			path()				const;
			std::string			filename()			const;
			unsigned long		startFrame()		const;
			unsigned long		endFrame()			const;

			//Asset&	operator=(const Asset& rhs)		const;
			//bool	operator==(const Asset& rhs)	const;
			//bool	operator!=(const Asset& rhs)	const;

			std::string			sequenceExtension()	const;
			DataSource::Type	dataSourceType()	const;
			std::string			dataSourceURI()		const;

		private:
			std::string			_path;			// url
			std::string			_filename;		// including extension
			unsigned __int64	_fileSize;		// in bytes
			time_t				_timeCreated;
			time_t				_timeModified;
			unsigned long		_startFrame;
			unsigned long		_endFrame;

			std::string			_title;
			std::string			_description;
			std::string			_notes;
			std::string			_keywords;

			MediaType			_mediaType;
			unsigned short		_mediaFormat;		// Based on MediaType, cast to Imageformat/VideoFormat/AudioFormat
			unsigned short		_xUnscaledSize;
			unsigned short		_yUnscaledSize;

/*
			??MediaType			_mediaType;		// AUDIO / IMAGE / VIDEO
			bool/ImageType			_imageType;		// STILL / SEQUENCE
//			unsigned long		_format;		// cast to Imageformat / VideoFormat / AudioFormat depending on MediaType! or as below...
			ImageFormat			_imageFormat;
			VideoFormat			_videoFormat;
			AudioFormat			_audioFormat;

			unsigned char		_colourDepth;

			unsigned short		_nativeFrameWidth;
			unsigned short		_nativeFrameHeight;

			unsigned __int64	_ownersUserId;	// owner
			std::string			_title;			// 
			std::string			_description;	// 
			std::string			_notes;			// 
			std::string			_keywords;		// tags

			time_t				_timeCreated;
			time_t				_timeModified;

			unsigned char		_frameRate;		// native frames per second
			VideoLineStandard	_videoLineStandard;
			unsigned short		_audioBitRate;
			unsigned short		_videoBitRate;
			unsigned short		_compressionRate;
			bool				_compressed;
			CompressionMode		_compressionMode;

			std::string			_sequenceExtension;
			DataSource::Type	_dataSourceType;
			std::string			_dataSourceURI;
*/
		};
	}
}
