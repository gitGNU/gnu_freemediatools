
// ml - A media library representation.

// Copyright (C) 2005-2006 VM Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef OML_DIRECTSHOW_PLUGIN_INC_
#define OML_DIRECTSHOW_PLUGIN_INC_

#include <map>
#include <AtlBase.h>
#include <dshow.h>
#include <initguid.h>
#include <Qedit.h>
#include <WinGDI.h>
#include <Dvdmedia.h>
#include <streams.h> // required for AutoLock
#include <Aviriff.h> // required for FCC macro. see http://msdn2.microsoft.com/en-us/library/ms783788.aspx

#include <openpluginlib/pl/openpluginlib.hpp>
#include <openpluginlib/pl/utf8_utils.hpp>
#include <openimagelib/il/il.hpp>
#include <openimagelib/il/utility.hpp>
#include <openmedialib/ml/openmedialib_plugin.hpp>
#include <openmedialib/ml/input.hpp>
#include <openmedialib/ml/frame.hpp>

namespace opl = olib::openpluginlib;
namespace oil = olib::openimagelib;
namespace oml = olib::openmedialib;

//#define BUFFER_MEDIA_SAMPLES

namespace olib { namespace openmedialib { namespace ml { 

#if defined(BUFFER_MEDIA_SAMPLES)
	typedef boost::shared_ptr<struct media_sample> media_sample_ptr;

	struct media_sample
	{
		double SampleTime;
		BYTE *pBuffer;
		long BufferLen;

		media_sample()
			: SampleTime(0)
			, pBuffer(NULL)
			, BufferLen(0)
		{
		}

		~media_sample()
		{
			//delete [] pBuffer;
		}
	};
#endif

	class ML_PLUGIN_DECLSPEC directshow_input
		: public input_type
	{
	public:
		directshow_input( opl::wstring resource, const opl::wstring mime_type = L"" );
		virtual ~directshow_input( );

		// input_type overrides
		bool							is_valid()				const;
		bool							is_seekable()			const;
		const openpluginlib::wstring	get_uri()				const;
		const openpluginlib::wstring	get_mime_type()			const;
		int								get_frames()			const;
		int								get_video_streams()		const;
		int								get_audio_streams()		const;

		void							seek(const int position, const bool relative = false);
		frame_type_ptr					fetch();

	private:
		HRESULT build_graph();

		opl::wstring	uri_;
		opl::wstring	mime_type_;
		bool			valid_;
		long long		total_frames_;

		AM_MEDIA_TYPE video_mediatype_;
		AM_MEDIA_TYPE audio_mediatype_;

		CComPtr<IGraphBuilder>		pGraph_;
		CComPtr<IMediaSeeking>		pMediaSeeking_;
		CComPtr<IMediaControl>		pMediaControl_;
		CComPtr<IVideoFrameStep>	pIVideoFrameStep_;
		CComPtr<ISampleGrabber>		pISampleGrabberVideo_;
		CComPtr<ISampleGrabber>		pISampleGrabberAudio_;
		CComPtr<IBaseFilter>		pSourceFilter_;
		CComPtr<IBaseFilter>		pVideoSampleGrabber_;
		CComPtr<IBaseFilter>		pAudioSampleGrabber_;
		CComPtr<IBaseFilter>		pVideoNullRenderer_;
		CComPtr<IBaseFilter>		pAudioNullRenderer_;

		HANDLE						hVideoReceivedEvent_;
		HANDLE						hAudioReceivedEvent_;

		CCritSec					video_container_cs_;
		CCritSec					audio_container_cs_;
#if defined(BUFFER_MEDIA_SAMPLES)
		typedef std::map< double, media_sample_ptr > MediaSampleContainer;
#else
		typedef std::map< double, CComPtr< IMediaSample > > MediaSampleContainer;
#endif
		MediaSampleContainer video_container_;
		MediaSampleContainer audio_container_;

		friend class VideoCallbackDelegate;
		VideoCallbackDelegate* video_callback_delegate_;

		friend class AudioCallbackDelegate;
		AudioCallbackDelegate* audio_callback_delegate_;
	};

} } }

#endif