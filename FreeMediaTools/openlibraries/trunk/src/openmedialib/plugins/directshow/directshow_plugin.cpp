// directshow_plugin.cpp
//
// Copyright (C) 2005-2006 VM Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#pragma warning ( push )
#pragma warning ( disable: 4251 )

#include <deque>
#include <boost/lexical_cast.hpp>

#ifdef WIN32

#include <openmedialib/plugins/directshow/directshow_plugin.hpp>

//--------------------------------------------------------------------------

namespace olib { namespace openmedialib { namespace ml { 

namespace
{
	void parse_BITMAPINFOHEADER(const BITMAPINFOHEADER& bmi)
	{
#if defined(_DEBUG)
		try
		{
			OutputDebugStringA((std::string("BITMAPINFOHEADER.biSize: ")          + boost::lexical_cast<std::string>(bmi.biSize)          + std::string("\n")).c_str());
			OutputDebugStringA((std::string("BITMAPINFOHEADER.biWidth: ")         + boost::lexical_cast<std::string>(bmi.biWidth)         + std::string("\n")).c_str());
			OutputDebugStringA((std::string("BITMAPINFOHEADER.biHeight: ")        + boost::lexical_cast<std::string>(bmi.biHeight)        + std::string("\n")).c_str());
			OutputDebugStringA((std::string("BITMAPINFOHEADER.biPlanes: ")        + boost::lexical_cast<std::string>(bmi.biPlanes)        + std::string("\n")).c_str());
			OutputDebugStringA((std::string("BITMAPINFOHEADER.biBitCount: ")      + boost::lexical_cast<std::string>(bmi.biBitCount)      + std::string("\n")).c_str());
			OutputDebugStringA((std::string("BITMAPINFOHEADER.biCompression: 0x") + boost::lexical_cast<std::string>(bmi.biCompression)   + std::string("\n")).c_str());
			OutputDebugStringA((std::string("BITMAPINFOHEADER.biSizeImage: ")     + boost::lexical_cast<std::string>(bmi.biSizeImage)     + std::string("\n")).c_str());
			OutputDebugStringA((std::string("BITMAPINFOHEADER.biXPelsPerMeter: ") + boost::lexical_cast<std::string>(bmi.biXPelsPerMeter) + std::string("\n")).c_str());
			OutputDebugStringA((std::string("BITMAPINFOHEADER.biYPelsPerMeter: ") + boost::lexical_cast<std::string>(bmi.biYPelsPerMeter) + std::string("\n")).c_str());
			OutputDebugStringA((std::string("BITMAPINFOHEADER.biClrUsed: ")       + boost::lexical_cast<std::string>(bmi.biClrUsed)       + std::string("\n")).c_str());
			OutputDebugStringA((std::string("BITMAPINFOHEADER.biClrImportant: ")  + boost::lexical_cast<std::string>(bmi.biClrImportant)  + std::string("\n")).c_str());
		}
		catch(const boost::bad_lexical_cast&)
		{
		}
#endif
	}

	void parse_VIDEOINFOHEADER(const VIDEOINFOHEADER& videoinfo)
	{
#if defined(_DEBUG)
		try
		{
			OutputDebugStringA((std::string("VIDEOINFOHEADER.rcSource: left=")   + boost::lexical_cast<std::string>(videoinfo.rcSource.left)   + std::string(" top=") + boost::lexical_cast<std::string>(videoinfo.rcSource.top) + std::string(" right=") + boost::lexical_cast<std::string>(videoinfo.rcSource.right) + std::string(" bottom=") + boost::lexical_cast<std::string>(videoinfo.rcSource.bottom) + std::string("\n")).c_str());
			OutputDebugStringA((std::string("VIDEOINFOHEADER.rcTarget: left=")   + boost::lexical_cast<std::string>(videoinfo.rcTarget.left)   + std::string(" top=") + boost::lexical_cast<std::string>(videoinfo.rcTarget.top) + std::string(" right=") + boost::lexical_cast<std::string>(videoinfo.rcTarget.right) + std::string(" bottom=") + boost::lexical_cast<std::string>(videoinfo.rcTarget.bottom) + std::string("\n")).c_str());
			OutputDebugStringA((std::string("VIDEOINFOHEADER.dwBitRate: ")       + boost::lexical_cast<std::string>(videoinfo.dwBitRate)       + std::string("\n")).c_str());
			OutputDebugStringA((std::string("VIDEOINFOHEADER.dwBitErrorRate: ")  + boost::lexical_cast<std::string>(videoinfo.dwBitErrorRate)  + std::string("\n")).c_str());
			OutputDebugStringA((std::string("VIDEOINFOHEADER.AvgTimePerFrame: ") + boost::lexical_cast<std::string>(videoinfo.AvgTimePerFrame) + std::string("\n")).c_str());
		}
		catch(const boost::bad_lexical_cast&)
		{
		}

		parse_BITMAPINFOHEADER(videoinfo.bmiHeader);
#endif
	}

	void parse_VIDEOINFOHEADER2(const VIDEOINFOHEADER2& videoinfo2)
	{
#if defined(_DEBUG)
		try
		{
			OutputDebugStringA((std::string("VIDEOINFOHEADER2.rcSource: left=")        + boost::lexical_cast<std::string>(videoinfo2.rcSource.left)      + std::string(" top=") + boost::lexical_cast<std::string>(videoinfo2.rcSource.top) + std::string(" right=") + boost::lexical_cast<std::string>(videoinfo2.rcSource.right) + std::string(" bottom=") + boost::lexical_cast<std::string>(videoinfo2.rcSource.bottom) + std::string("\n")).c_str());
			OutputDebugStringA((std::string("VIDEOINFOHEADER2.rcTarget: left=")        + boost::lexical_cast<std::string>(videoinfo2.rcTarget.left)      + std::string(" top=") + boost::lexical_cast<std::string>(videoinfo2.rcTarget.top) + std::string(" right=") + boost::lexical_cast<std::string>(videoinfo2.rcTarget.right) + std::string(" bottom=") + boost::lexical_cast<std::string>(videoinfo2.rcTarget.bottom) + std::string("\n")).c_str());
			OutputDebugStringA((std::string("VIDEOINFOHEADER2.dwBitRate: ")            + boost::lexical_cast<std::string>(videoinfo2.dwBitRate)          + std::string("\n")).c_str());
			OutputDebugStringA((std::string("VIDEOINFOHEADER2.dwBitErrorRate: ")       + boost::lexical_cast<std::string>(videoinfo2.dwBitErrorRate)     + std::string("\n")).c_str());
			OutputDebugStringA((std::string("VIDEOINFOHEADER2.AvgTimePerFrame: ")      + boost::lexical_cast<std::string>(videoinfo2.AvgTimePerFrame)    + std::string("\n")).c_str());
			OutputDebugStringA((std::string("VIDEOINFOHEADER2.dwInterlaceFlags: 0x")   + boost::lexical_cast<std::string>(videoinfo2.dwInterlaceFlags)   + std::string("\n")).c_str());
			OutputDebugStringA((std::string("VIDEOINFOHEADER2.dwCopyProtectFlags: 0x") + boost::lexical_cast<std::string>(videoinfo2.dwCopyProtectFlags) + std::string("\n")).c_str());
			OutputDebugStringA((std::string("VIDEOINFOHEADER2.dwPictAspectRatioX: ")   + boost::lexical_cast<std::string>(videoinfo2.dwPictAspectRatioX) + std::string("\n")).c_str());
			OutputDebugStringA((std::string("VIDEOINFOHEADER2.dwPictAspectRatioY: ")   + boost::lexical_cast<std::string>(videoinfo2.dwPictAspectRatioY) + std::string("\n")).c_str());
			OutputDebugStringA((std::string("VIDEOINFOHEADER2.dwControlFlags: 0x")     + boost::lexical_cast<std::string>(videoinfo2.dwControlFlags)     + std::string("\n")).c_str());
			OutputDebugStringA((std::string("VIDEOINFOHEADER2.dwReserved2: 0x")        + boost::lexical_cast<std::string>(videoinfo2.dwReserved2)        + std::string("\n")).c_str());
		}
		catch(const boost::bad_lexical_cast&)
		{
		}

		parse_BITMAPINFOHEADER(videoinfo2.bmiHeader);
#endif
	}

	void parse_WAVEFORMATEX(const WAVEFORMATEX& waveformat)
	{
#if defined(_DEBUG)
		try
		{
			OutputDebugStringA((std::string("WAVEFORMATEX.wFormatTag: ")      + boost::lexical_cast<std::string>(waveformat.wFormatTag)      + std::string("\n")).c_str()); 
			OutputDebugStringA((std::string("WAVEFORMATEX.nChannels: ")       + boost::lexical_cast<std::string>(waveformat.nChannels)       + std::string("\n")).c_str()); 
			OutputDebugStringA((std::string("WAVEFORMATEX.nSamplesPerSec: ")  + boost::lexical_cast<std::string>(waveformat.nSamplesPerSec)  + std::string("\n")).c_str()); 
			OutputDebugStringA((std::string("WAVEFORMATEX.nAvgBytesPerSec: ") + boost::lexical_cast<std::string>(waveformat.nAvgBytesPerSec) + std::string("\n")).c_str()); 
			OutputDebugStringA((std::string("WAVEFORMATEX.nBlockAlign: ")     + boost::lexical_cast<std::string>(waveformat.nBlockAlign)     + std::string("\n")).c_str()); 
			OutputDebugStringA((std::string("WAVEFORMATEX.wBitsPerSample: ")  + boost::lexical_cast<std::string>(waveformat.wBitsPerSample)  + std::string("\n")).c_str()); 
			OutputDebugStringA((std::string("WAVEFORMATEX.cbSize: ")          + boost::lexical_cast<std::string>(waveformat.cbSize)          + std::string("\n")).c_str()); 
		}
		catch(const boost::bad_lexical_cast&)
		{
		}
#endif
	}

	void parse_AM_MEDIA_TYPE(const AM_MEDIA_TYPE& mediatype)
	{
#if defined(_DEBUG)
		if(mediatype.majortype == MEDIATYPE_Video)
		{
			OutputDebugStringA("AM_MEDIA_TYPE.majortype: MEDIATYPE_Video\n");

			if(mediatype.formattype == FORMAT_VideoInfo)
			{
				OutputDebugStringA("AM_MEDIA_TYPE.formattype: FORMAT_VideoInfo\n");
				if(		(mediatype.pbFormat)
					&&	(mediatype.cbFormat >= sizeof(VIDEOINFOHEADER)) )
				{
					parse_VIDEOINFOHEADER(*reinterpret_cast<VIDEOINFOHEADER*>(mediatype.pbFormat));
				}
			}
			else if(mediatype.formattype == FORMAT_VideoInfo2)
			{
				OutputDebugStringA("AM_MEDIA_TYPE.formattype: FORMAT_VideoInfo2\n");
				if(		(mediatype.pbFormat)
					&&	(mediatype.cbFormat >= sizeof(VIDEOINFOHEADER2)) )
				{
					parse_VIDEOINFOHEADER2(*reinterpret_cast<VIDEOINFOHEADER2*>(mediatype.pbFormat));
				}
			}
			else if(mediatype.formattype == FORMAT_DvInfo)
			{
			}
			else if(mediatype.formattype == FORMAT_MPEG2Video)
			{
			}
			else if(mediatype.formattype == FORMAT_MPEGStreams)
			{
			}
			else if(mediatype.formattype == FORMAT_MPEGVideo)
			{
			}
		}
		else if(mediatype.majortype == MEDIATYPE_Audio)
		{
			OutputDebugStringA("AM_MEDIA_TYPE.majortype: MEDIATYPE_Audio\n");

			if(mediatype.formattype == FORMAT_WaveFormatEx)
			{
				OutputDebugStringA("AM_MEDIA_TYPE.formattype: FORMAT_WaveFormatEx\n");
				if(		(mediatype.pbFormat)
					&&	(mediatype.cbFormat >= sizeof(WAVEFORMATEX)) )
					parse_WAVEFORMATEX(*reinterpret_cast<WAVEFORMATEX*>(mediatype.pbFormat));
			}
		}

		try
		{
			OutputDebugStringA((std::string("AM_MEDIA_TYPE.subtype: 0x")            + boost::lexical_cast<std::string>((void*)*((long*)&mediatype.subtype)) + std::string("\n")).c_str());
			OutputDebugStringA((std::string("AM_MEDIA_TYPE.bFixedSizeSamples: ")    + boost::lexical_cast<std::string>(mediatype.bFixedSizeSamples)         + std::string("\n")).c_str());
			OutputDebugStringA((std::string("AM_MEDIA_TYPE.bTemporalCompression: ") + boost::lexical_cast<std::string>(mediatype.bTemporalCompression)      + std::string("\n")).c_str());
			OutputDebugStringA((std::string("AM_MEDIA_TYPE.lSampleSize: ")          + boost::lexical_cast<std::string>(mediatype.lSampleSize)               + std::string("\n")).c_str());
			OutputDebugStringA((std::string("AM_MEDIA_TYPE.cbFormat: ")             + boost::lexical_cast<std::string>(mediatype.cbFormat)                  + std::string("\n")).c_str());
		}
		catch(const boost::bad_lexical_cast&)
		{
		}
#endif
	}

	//---------------------------------------------------------------------------------------------

	opl::string fourcc_descriptor(unsigned long fourcc_format)
	{
		opl::string format;
		format += (char)(fourcc_format & 0x000000FF);
		format += (char)((fourcc_format & 0x0000FF00) >> 8);
		format += (char)((fourcc_format & 0x00FF0000) >> 16);
		format += (char)((fourcc_format & 0xFF000000) >> 24);
		return format;
	}
	
	// FOURCC to openimagelib format convertion
	// Format converter base class...
	typedef boost::shared_ptr<class format_converter> format_converter_ptr;
	
	class format_converter
	{
	public:
		explicit format_converter(unsigned long fourcc_format);
		virtual ~format_converter();

		opl::string get_source_format() const;
		virtual opl::wstring get_target_format() const = 0;
		virtual void convert(unsigned char* src_buffer, unsigned long src_buffer_size, image_type_ptr image) const = 0;

	private:
		unsigned long fourcc_format_;
	};

	format_converter::format_converter(unsigned long fourcc_format)
	: fourcc_format_(fourcc_format)
	{
	}

	format_converter::~format_converter()
	{
	}

	opl::string format_converter::get_source_format() const
	{
		return fourcc_descriptor(fourcc_format_);
	}
	
	// FOURCC Y41P converter...
	class Y41P_converter : public format_converter
	{
	public:
		explicit Y41P_converter();
		~Y41P_converter();

		opl::wstring get_target_format() const;
		void convert(unsigned char* src_buffer, unsigned long src_buffer_size, image_type_ptr image) const;
	};

	typedef boost::shared_ptr<Y41P_converter> Y41P_converter_ptr;
	Y41P_converter::Y41P_converter()
	: format_converter( FCC('Y41P') )
	{
	}

	Y41P_converter::~Y41P_converter()
	{
	}

	opl::wstring Y41P_converter::get_target_format() const
	{
		return opl::wstring(L"yuv411p");
	}

	void Y41P_converter::convert(unsigned char* src_buffer, unsigned long src_buffer_size, image_type_ptr image) const
	{
#if 1
		// Macropixel: u0 y0 v0 y1 u4 y2 v4 y3 y4 y5 y6 y7
		const unsigned long bytes_per_macropixel = 12;
		const unsigned long luma_per_macropixel = 8;
		unsigned long num_macropixels = src_buffer_size/bytes_per_macropixel;
		unsigned char* src  = src_buffer;
		unsigned char* dest_Y = (unsigned char*)image->data(0);
		unsigned char* dest_U = (unsigned char*)image->data(1);
		unsigned char* dest_V = (unsigned char*)image->data(2);

		for(unsigned long idx = 0; idx < num_macropixels; idx++)
		{
			*dest_U++ = *src++;
			*dest_Y++ = *src++;
			*dest_V++ = *src++;
			*dest_Y++ = *src++;
			*dest_U++ = *src++;
			*dest_Y++ = *src++;
			*dest_V++ = *src++;
			*dest_Y++ = *src++;
			*dest_Y++ = *src++;
			*dest_Y++ = *src++;
			*dest_Y++ = *src++;
			*dest_Y++ = *src++;
		}
#else
		memcpy(	(unsigned char*)(image->data(0)), 
				src_buffer,
				image->width()*image->height() );
		memset((unsigned char*)(image->data(1)), 128, image->width(1)*image->height(1));
		memset((unsigned char*)(image->data(2)), 128, image->width(2)*image->height(2));
#endif
	}
	
	// FOURCC YV12 converter...
	class YV12_converter : public format_converter
	{
	public:
		explicit YV12_converter();
		~YV12_converter();

		opl::wstring get_target_format() const;
		void convert(unsigned char* src_buffer, unsigned long src_buffer_size, image_type_ptr image) const;
	};

	typedef boost::shared_ptr<YV12_converter> YV12_converter_ptr;
	YV12_converter::YV12_converter()
	: format_converter( FCC('YV12') )
	{
	}

	YV12_converter::~YV12_converter()
	{
	}

	opl::wstring YV12_converter::get_target_format() const
	{
		return opl::wstring(L"yuv420p");
	}

	void YV12_converter::convert(unsigned char* src_buffer, unsigned long src_buffer_size, image_type_ptr image) const
	{
		// Note: YV12 plane order is Y, V then U.
		// Is this the case for a yuv420p olib image?
		memcpy(	(unsigned char*)(image->data(0)), 
				src_buffer,
				image->size() );
	}
	
	// RGB24 converter...
	class RGB24_converter : public format_converter
	{
	public:
		explicit RGB24_converter();
		~RGB24_converter();

		opl::wstring get_target_format() const;
		void convert(unsigned char* src_buffer, unsigned long src_buffer_size, image_type_ptr image) const;
	};

	typedef boost::shared_ptr<RGB24_converter> RGB24_converter_ptr;
	RGB24_converter::RGB24_converter()
	: format_converter( 0x00000000/*FCC('RGB ')*/ )
	{
	}

	RGB24_converter::~RGB24_converter()
	{
	}

	opl::wstring RGB24_converter::get_target_format() const
	{
		// http://msdn2.microsoft.com/en-us/library/ms787838.aspx
		return opl::wstring(L"b8g8r8");
	}

	void RGB24_converter::convert(unsigned char* src_buffer, unsigned long src_buffer_size, image_type_ptr image) const
	{
		memcpy(	(unsigned char*)(image->data(0)), 
				src_buffer,
				image->size() );
	}
	
	// FOURCC RGB32 converter...
	class RGBA32_converter : public format_converter
	{
	public:
		explicit RGBA32_converter();
		~RGBA32_converter();

		opl::wstring get_target_format() const;
		void convert(unsigned char* src_buffer, unsigned long src_buffer_size, image_type_ptr image) const;
	};

	typedef boost::shared_ptr<RGBA32_converter> RGBA32_converter_ptr;
	RGBA32_converter::RGBA32_converter()
	: format_converter( FCC('RGBA') )
	{
	}

	RGBA32_converter::~RGBA32_converter()
	{
	}

	opl::wstring RGBA32_converter::get_target_format() const
	{
		// http://msdn2.microsoft.com/en-us/library/ms787838.aspx
		return opl::wstring(L"b8g8r8a8");
	}

	void RGBA32_converter::convert(unsigned char* src_buffer, unsigned long src_buffer_size, image_type_ptr image) const
	{
		memcpy(	(unsigned char*)(image->data(0)), 
				src_buffer,
				image->size() );
	}
	
	// FOURCC to oil format converter factory function
	format_converter_ptr get_format_converter(unsigned long fourcc_format)
	{
		switch(fourcc_format)
		{
		case 0x00000000: // BI_RGB
		//	return format_converter_ptr(new RGB24_converter());
		//case FCC('RGBA'):
			return format_converter_ptr(new RGBA32_converter());
		case FCC('Y41P'):
			return format_converter_ptr(new Y41P_converter());
		case FCC('YV12'):
			return format_converter_ptr(new YV12_converter());
		default:
			return format_converter_ptr();
		}
	}
	
	//---------------------------------------------------------------------------------------------
	
	// Move these to anonymous namespace. No need to be part of class.
	HRESULT get_pin(IBaseFilter *pFilter, PIN_DIRECTION direction, IPin** ppPin)
	{
		if(!pFilter)
			return E_POINTER;
		if(!ppPin)
			return E_POINTER;

		CComPtr<IEnumPins> pEnumPins;
		HRESULT hr = pFilter->EnumPins(&pEnumPins);
		if(FAILED(hr))
			return hr;

		CComPtr<IPin> pPin;
		while(pEnumPins->Next(1, &pPin.p, 0) == S_OK)
		{
			PIN_DIRECTION dir;
			hr = pPin->QueryDirection(&dir);
			if(FAILED(hr))
				break;

			if(dir == direction)
			{
				*ppPin = pPin;
				break;
			}
		}

		return hr;
	}

	HRESULT get_pins(IBaseFilter *pFilter, PIN_DIRECTION direction, std::deque< CComPtr<IPin> >& pin_container)
	{
		if(!pFilter)
			return E_POINTER;

		pin_container.clear();

		CComPtr<IEnumPins> pEnumPins;
		HRESULT hr = pFilter->EnumPins(&pEnumPins);
		if(FAILED(hr))
			return hr;

		CComPtr<IPin> pPin;
		while(pEnumPins->Next(1, &pPin.p, 0) == S_OK)
		{
			PIN_DIRECTION dir;
			hr = pPin->QueryDirection(&dir);
			if(FAILED(hr))
				break;

			if(dir == direction)
				pin_container.push_back(pPin);
		}

		return hr;
	}
	
	//---------------------------------------------------------------------------------------------
	
#if defined(_DEBUG)
	void print_native_video_format(const opl::wstring& uri)
	{
		CComPtr<IMediaDet> pIMediaDet;
		HRESULT hr = pIMediaDet.CoCreateInstance(CLSID_MediaDet, NULL, CLSCTX_INPROC_SERVER);
		if(SUCCEEDED(hr))
			hr = pIMediaDet->put_Filename(BSTR(uri.c_str()));

		long num_output_streams = 0;
		if(SUCCEEDED(hr))
			hr = pIMediaDet->get_OutputStreams(&num_output_streams);

		if(SUCCEEDED(hr))
		{
			for(long idx = 0; idx < num_output_streams; idx++)
			{
				hr = pIMediaDet->put_CurrentStream(idx);
				if(FAILED(hr))
					continue;

				AM_MEDIA_TYPE mt;
				hr = pIMediaDet->get_StreamMediaType(&mt);
				if(FAILED(hr))
					continue;

				if(mt.formattype != FORMAT_VideoInfo)
					continue;

				OutputDebugStringA((std::string("Native FOURCC format: ") + fourcc_descriptor((*reinterpret_cast<VIDEOINFOHEADER*>(mt.pbFormat)).bmiHeader.biCompression).c_str() + std::string("\n")).c_str());
			}
		}
	}

	void print_num_filters_in_filter_graph(const CComPtr<IGraphBuilder>& pGraph)
	{
		CComPtr<IEnumFilters> pEnumFilters;
		HRESULT hr = pGraph->EnumFilters(&pEnumFilters);
		if(SUCCEEDED(hr))
		{
			const int FILTERS_TO_REQUEST = 100;
			CComPtr<IBaseFilter> pFiltersArray[FILTERS_TO_REQUEST];
			ULONG num_got = 0;
			hr = pEnumFilters->Next(FILTERS_TO_REQUEST, &pFiltersArray[0], &num_got);
			if(SUCCEEDED(hr))
			{
				try
				{
					OutputDebugStringA((std::string("Filters in filter graph: ") + boost::lexical_cast<std::string>(num_got) + std::string("\n")).c_str());
				}
				catch(const boost::bad_lexical_cast&)
				{
				}
			}
		}
	}

#endif
}

//#################################################################################################

class SampleGrabberCallbackDelegate : public ISampleGrabberCB
{
public:
	explicit SampleGrabberCallbackDelegate(directshow_input* input);
	~SampleGrabberCallbackDelegate();

	// IUnknown overrides
	HRESULT STDMETHODCALLTYPE QueryInterface( REFIID iid, void** ppvObject );
	ULONG STDMETHODCALLTYPE AddRef();
	ULONG STDMETHODCALLTYPE Release();

	// ISampleGrabberCB overrides
	virtual HRESULT STDMETHODCALLTYPE SampleCB(double SampleTime, IMediaSample *pSample) = 0;
	virtual HRESULT STDMETHODCALLTYPE BufferCB(double SampleTime, BYTE *pBuffer, long BufferLen) = 0;

protected:
	unsigned long		refcount_;
	directshow_input*	input_;
};

SampleGrabberCallbackDelegate::SampleGrabberCallbackDelegate(directshow_input* input)
	: input_(input)
	, refcount_(0)
{
}

SampleGrabberCallbackDelegate::~SampleGrabberCallbackDelegate()
{
}

HRESULT STDMETHODCALLTYPE SampleGrabberCallbackDelegate::QueryInterface( REFIID iid, void** ppv )
{
	if(!ppv)
		return E_POINTER;

	if(iid == IID_IUnknown)
		*ppv = dynamic_cast<IUnknown*>( this );
	else if(iid == IID_ISampleGrabberCB)
		*ppv = dynamic_cast<ISampleGrabberCB*>( this );
	else
	{
		// It didn't match an interface
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	
	// Increment refcount on the pointer we're about to return
	AddRef();
	// Return success
	return S_OK;
}

ULONG STDMETHODCALLTYPE SampleGrabberCallbackDelegate::AddRef()
{
	return(++refcount_);
}

ULONG STDMETHODCALLTYPE SampleGrabberCallbackDelegate::Release()
{
	if(--refcount_ <= 0)
		delete this;

	return refcount_;
}
	
//-------------------------------------------------------------------------------------------------

class VideoCallbackDelegate : public SampleGrabberCallbackDelegate
{
public:
	explicit VideoCallbackDelegate(directshow_input* input);
	~VideoCallbackDelegate();

	// ISampleGrabberCB overrides
	HRESULT STDMETHODCALLTYPE SampleCB(double SampleTime, IMediaSample *pSample);
	HRESULT STDMETHODCALLTYPE BufferCB(double SampleTime, BYTE *pBuffer, long BufferLen);
};

VideoCallbackDelegate::VideoCallbackDelegate(directshow_input* input)
	: SampleGrabberCallbackDelegate(input)
{
}

VideoCallbackDelegate::~VideoCallbackDelegate()
{
}

HRESULT STDMETHODCALLTYPE VideoCallbackDelegate::SampleCB(double SampleTime, IMediaSample *pSample)
{
#ifdef BUFFER_MEDIA_SAMPLES
	return E_FAIL;
#else
	if(!pSample)
		return E_INVALIDARG;
	if(pSample->IsPreroll() == S_OK)
		return S_FALSE;

	CAutoLock mutex(&input_->video_container_cs_);
#if defined(_DEBUG)
	try
	{
		std::string msg("Received video callback: SampleTime: ");
		msg += boost::lexical_cast<std::string>(SampleTime);
		msg += " (frame: ";
		msg += boost::lexical_cast<std::string>(static_cast<long long>((SampleTime * 10000000)/reinterpret_cast<VIDEOINFOHEADER*>(input_->video_mediatype_.pbFormat)->AvgTimePerFrame));
		msg += ") pSample: 0x";
		msg += boost::lexical_cast<std::string>(pSample);
		msg += "\n";
		OutputDebugStringA(msg.c_str());
	}
	catch(const boost::bad_lexical_cast&)
	{
		OutputDebugStringA("Received video callback\n");
	}
#endif
	input_->video_container_[SampleTime] = pSample;
	SetEvent(input_->hVideoReceivedEvent_);
	return S_OK;
#endif
}

HRESULT STDMETHODCALLTYPE VideoCallbackDelegate::BufferCB(double SampleTime, BYTE *pBuffer, long BufferLen)
{
#ifdef BUFFER_MEDIA_SAMPLES
	if(!pBuffer)
		return E_INVALIDARG;

	media_sample_ptr pMS = boost::shared_ptr<media_sample>(new media_sample);
	pMS->SampleTime	= SampleTime;
	pMS->pBuffer	= pBuffer;
	pMS->BufferLen	= BufferLen;

	CAutoLock mutex(&input_->video_container_cs_);
#if defined(_DEBUG)
	try
	{
		std::string msg("Received video callback: SampleTime: ");
		msg += boost::lexical_cast<std::string>(SampleTime);
		msg += " (frame: ";
		msg += boost::lexical_cast<std::string>(static_cast<long long>((SampleTime * 10000000)/reinterpret_cast<VIDEOINFOHEADER*>(input_->video_mediatype_.pbFormat)->AvgTimePerFrame));
		msg += ") pBuffer: 0x";
		msg += boost::lexical_cast<std::string>(pBuffer);
		msg += ", buffer size:";
		msg += boost::lexical_cast<std::string>(BufferLen);
		msg += "\n";
		OutputDebugStringA(msg.c_str());
	}
	catch(const boost::bad_lexical_cast&)
	{
		OutputDebugStringA("Received video frame\n");
	}
#endif
	input_->video_container_[SampleTime] = pMS;
	SetEvent(input_->hVideoReceivedEvent_);
	return S_OK;
#else
	return E_FAIL;
#endif
}
	
//-------------------------------------------------------------------------------------------------

class AudioCallbackDelegate : public SampleGrabberCallbackDelegate
{
public:
	explicit AudioCallbackDelegate(directshow_input* input);
	~AudioCallbackDelegate();

	// ISampleGrabberCB overrides
	HRESULT STDMETHODCALLTYPE SampleCB(double SampleTime, IMediaSample *pSample);
	HRESULT STDMETHODCALLTYPE BufferCB(double SampleTime, BYTE *pBuffer, long BufferLen);
};

AudioCallbackDelegate::AudioCallbackDelegate(directshow_input* input)
	: SampleGrabberCallbackDelegate(input)
{
}

AudioCallbackDelegate::~AudioCallbackDelegate()
{
}

HRESULT STDMETHODCALLTYPE AudioCallbackDelegate::SampleCB(double SampleTime, IMediaSample *pSample)
{
#ifdef BUFFER_MEDIA_SAMPLES
	return E_FAIL;
#else
	if(!pSample)
		return S_FALSE;
	if(pSample->IsPreroll() == S_OK)
		return S_FALSE;
	CAutoLock mutex(&input_->audio_container_cs_);
#if defined(_DEBUG)
	HRESULT hr = E_FAIL;
//#define DEBUG_GET_MEDIA_TIME
#ifdef  DEBUG_GET_MEDIA_TIME
	long long	startTime = 0,
				endTime = 0;
	hr = pSample->GetMediaTime(&startTime, &endTime);
	if(hr == VFW_E_MEDIA_TIME_NOT_SET)
	{
		startTime = -1;
		endTime = -1;
	}
#endif
	long long frame = static_cast<long long>((SampleTime * 10000000)/reinterpret_cast<VIDEOINFOHEADER*>(input_->video_mediatype_.pbFormat)->AvgTimePerFrame);
//#define DEBUG_CONVERT_TIME_FORMAT
#ifdef  DEBUG_CONVERT_TIME_FORMAT
	long long targetTime = 0;
	//hr = input_->pMediaSeeking_->ConvertTimeFormat(&targetTime, &TIME_FORMAT_MEDIA_TIME, frame, NULL/*&TIME_FORMAT_FRAME*/);
	//hr = input_->pMediaSeeking_->ConvertTimeFormat(&targetTime, &TIME_FORMAT_SAMPLE, long long(SampleTime*10000000), &TIME_FORMAT_MEDIA_TIME);
	//hr = input_->pMediaSeeking_->ConvertTimeFormat(&targetTime, &TIME_FORMAT_SAMPLE, long long(SampleTime*10000000), &TIME_FORMAT_MEDIA_TIME);
	hr = input_->pMediaSeeking_->ConvertTimeFormat(&targetTime, &TIME_FORMAT_SAMPLE, SampleTime, NULL/*&TIME_FORMAT_FRAME*/);
#endif
	try
	{
		std::string msg("Received audio callback: SampleTime: ");
		msg += boost::lexical_cast<std::string>(SampleTime);
		msg += " (frame: ";
		msg += boost::lexical_cast<std::string>(frame);
		msg += ") ";
#ifdef  DEBUG_GET_MEDIA_TIME
		msg += "startTime: ";
		msg += boost::lexical_cast<std::string>(startTime);
		msg += "; endTime: ";
		msg += boost::lexical_cast<std::string>(endTime);
		msg += "; ";
#endif
#ifdef  DEBUG_CONVERT_TIME_FORMAT
		msg += "targetTime: ";
//		msg += boost::lexical_cast<std::string>(double(targetTime)/10000000);
		msg += boost::lexical_cast<std::string>(targetTime);
		msg += "; ";
#endif
		msg += "pSample: 0x";
		msg += boost::lexical_cast<std::string>(pSample);
		msg += "\n";
		OutputDebugStringA(msg.c_str());
	}
	catch(const boost::bad_lexical_cast&)
	{
		OutputDebugStringA("Received audio callback\n");
	}
#endif
	input_->audio_container_[SampleTime] = pSample;
	SetEvent(input_->hAudioReceivedEvent_);
	return S_OK;
#endif
}

HRESULT STDMETHODCALLTYPE AudioCallbackDelegate::BufferCB(double SampleTime, BYTE *pBuffer, long BufferLen)
{
#ifdef BUFFER_MEDIA_SAMPLES
	if(!pBuffer)
		return S_FALSE;

	media_sample_ptr pMS = boost::shared_ptr<media_sample>(new media_sample);
	pMS->SampleTime	= SampleTime;
	pMS->pBuffer	= pBuffer;
	pMS->BufferLen	= BufferLen;

	CAutoLock mutex(&input_->audio_container_cs_);
#if defined(_DEBUG)
	try
	{
		std::string msg("Received audio callback: SampleTime: ");
		msg += boost::lexical_cast<std::string>(SampleTime);
		msg += " (frame: ";
		msg += boost::lexical_cast<std::string>(static_cast<long long>((SampleTime * 10000000)/reinterpret_cast<VIDEOINFOHEADER*>(input_->video_mediatype_.pbFormat)->AvgTimePerFrame));
		msg += ") pBuffer: 0x";
		msg += boost::lexical_cast<std::string>(pBuffer);
		msg += ", buffer size:";
		msg += boost::lexical_cast<std::string>(BufferLen);
		msg += "\n";
		OutputDebugStringA(msg.c_str());
	}
	catch(const boost::bad_lexical_cast&)
	{
		OutputDebugStringA("Received audio frame\n");
	}
#endif
	input_->audio_container_[SampleTime] = pMS;
	SetEvent(input_->hAudioReceivedEvent_);
	return S_OK;
#else
	return E_FAIL;
#endif
}
	
//#################################################################################################

directshow_input::directshow_input( opl::wstring resource, const opl::wstring mime_type )
	: input_type( ) 
	, uri_( resource )
	, mime_type_( mime_type )
	, valid_(false)
	, total_frames_(0)
	, hVideoReceivedEvent_(NULL)
	, hAudioReceivedEvent_(NULL)
	, video_callback_delegate_(NULL)
	, audio_callback_delegate_(NULL)
{
	// parse the resource path replacing forward slashes with back slashes.
	opl::string res(opl::to_string(resource));
	for(opl::string::iterator it = res.begin();
		it != res.end();
		it++)
	{
		if(*it == '/')
			*it = '\\';
	}
	uri_ = opl::to_wstring(res);

	// initialize video & audio mediatype cache
	memset(&video_mediatype_, 0, sizeof(AM_MEDIA_TYPE));
	memset(&audio_mediatype_, 0, sizeof(AM_MEDIA_TYPE));

	// create events for video & audio callbacks to indicate new data has been received
	hVideoReceivedEvent_ = CreateEvent(NULL, false, false, NULL);
	if(!hVideoReceivedEvent_)
		return;

	hAudioReceivedEvent_ = CreateEvent(NULL, false, false, NULL);
	if(!hAudioReceivedEvent_)
		return;

	// Create callback delegates for receiving media samples from the sample grabber filters
	video_callback_delegate_ = new VideoCallbackDelegate(this);
	if(!video_callback_delegate_)
		return;
	video_callback_delegate_->AddRef();

	audio_callback_delegate_ = new AudioCallbackDelegate(this);
	if(!audio_callback_delegate_)
		return;
	audio_callback_delegate_->AddRef();

#if defined(_DEBUG)
	print_native_video_format(uri_);
#endif

	HRESULT hr = build_graph();

#if defined(_DEBUG)
	if(SUCCEEDED(hr))
		print_num_filters_in_filter_graph(pGraph_);
#endif

	// Obtain and cache an IMediaSeeking i/f, set the time format to be frame based and read the media duration
	if(SUCCEEDED(hr))
	{
		hr = pGraph_->QueryInterface(IID_IMediaSeeking, (void**)&pMediaSeeking_);
		if(SUCCEEDED(hr))
		{
			hr = pMediaSeeking_->SetTimeFormat(&TIME_FORMAT_FRAME);
		}
#if defined(_DEBUG)
		if(SUCCEEDED(hr))
		{
			if(pMediaSeeking_->IsUsingTimeFormat(&TIME_FORMAT_NONE) == S_OK)
				OutputDebugStringA("TimeFormat: No format\n");
			else if(pMediaSeeking_->IsUsingTimeFormat(&TIME_FORMAT_FRAME) == S_OK)
				OutputDebugStringA("TimeFormat: Video frames\n");
			else if(pMediaSeeking_->IsUsingTimeFormat(&TIME_FORMAT_SAMPLE) == S_OK)
				OutputDebugStringA("TimeFormat: Samples in the stream\n");
			else if(pMediaSeeking_->IsUsingTimeFormat(&TIME_FORMAT_FIELD) == S_OK)
				OutputDebugStringA("TimeFormat: Interlaced video fields\n");
			else if(pMediaSeeking_->IsUsingTimeFormat(&TIME_FORMAT_BYTE) == S_OK)
				OutputDebugStringA("TimeFormat: Byte offset within the stream\n");
			else if(pMediaSeeking_->IsUsingTimeFormat(&TIME_FORMAT_MEDIA_TIME) == S_OK)
				OutputDebugStringA("TimeFormat: Reference time (100-nanosecond units)\n");
		}
#endif
		if(SUCCEEDED(hr))
		{
			LONGLONG duration;
			hr = pMediaSeeking_->GetDuration(&duration);
			if(SUCCEEDED(hr))
				total_frames_ = static_cast<long long>(duration);
		}
	}

	// Obtain and cache an IVideoFrameStep i/f.  See if the filter will support stepping.
	if(SUCCEEDED(hr))
	{
		HRESULT hr = pGraph_->QueryInterface(IID_IVideoFrameStep, (void**)&pIVideoFrameStep_);
		if(SUCCEEDED(hr))
			if(pIVideoFrameStep_->CanStep(0, NULL) != S_OK)
				pIVideoFrameStep_ = NULL;
	}

	// start the filter graph going either paused or running - not sure whats the best at this stage
	if(SUCCEEDED(hr))
	{
		hr = pGraph_->QueryInterface(IID_IMediaControl, (void**)&pMediaControl_);
		if(SUCCEEDED(hr))
		{
#define RUN_FILTER_GRAPH
#if defined(RUN_FILTER_GRAPH)
			hr = pMediaControl_->Run();
#else
			hr = pMediaControl_->Pause();
#endif
			if(SUCCEEDED(hr))
			{
				OAFilterState state =  State_Stopped;
				int count = 0;
				while(++count <= 5)
				{
					hr = pMediaControl_->GetState(1000, &state);
					if(hr == VFW_S_STATE_INTERMEDIATE)
						continue;
					else if(hr == VFW_S_CANT_CUE)
						break;
					else
						break;
				}

				if(		(SUCCEEDED(hr))
#if defined(RUN_FILTER_GRAPH)
					&&	(state == State_Running) )
#else
					&&	(state == State_Paused) )
#endif
				{
#if defined(RUN_FILTER_GRAPH)
					OutputDebugStringA("filter graph is running!\n");
#else
					OutputDebugStringA("filter graph is paused!\n");
#endif
					valid_ = true;
				}
			}
		}
	}

	if(!valid_)
		OutputDebugStringA("directshow plugin is NOT valid!\n");
}

directshow_input::~directshow_input( )
{
	HRESULT hr = E_FAIL;

	// Stop the graph if not already stopped
	OAFilterState state;
	if(pMediaControl_)
	{
		hr = pMediaControl_->GetState(1000, &state);
		if(SUCCEEDED(hr) && state != State_Stopped)
		{
			hr = pMediaControl_->Stop();
			if(SUCCEEDED(hr))
			{
				int count = 0;
				while(++count <= 5)
				{
					hr = pMediaControl_->GetState(1000, &state);
					if(hr == VFW_S_STATE_INTERMEDIATE)
						continue;
					else if(hr == VFW_S_CANT_CUE)
						break;
					else
						break;
				}

				if(		(SUCCEEDED(hr))
					&&	(state == State_Stopped) )
				{
					OutputDebugStringA("filter graph is stopped!\n");
				}
			}
		}
	}

#ifdef BUFFER_MEDIA_SAMPLES
	if(pISampleGrabberVideo_)
		pISampleGrabberVideo_->SetCallback(NULL, 1);
#else
	if(pISampleGrabberVideo_)
		pISampleGrabberVideo_->SetCallback(NULL, 0);
#endif

	valid_ = false;

	{
		CAutoLock video_mutex(&video_container_cs_);
		video_container_.clear();
	}

	{
		CAutoLock video_mutex(&audio_container_cs_);
		audio_container_.clear();
	}

	// Remove all filters from the filter graph
	CComPtr<IEnumFilters> pEnumFilters;
	hr = pGraph_->EnumFilters(&pEnumFilters);
	if(SUCCEEDED(hr))
	{
		typedef std::deque<IBaseFilter*> FilterContainer;
		typedef FilterContainer::const_iterator FilterContainerIterator;

		FilterContainer filterContainer;
		IBaseFilter* pFilter;

		while(pEnumFilters->Next(1, &pFilter, NULL) == S_OK)
		{
			filterContainer.push_back(pFilter);
		}

		for(FilterContainerIterator it = filterContainer.begin();
			it != filterContainer.end();
			it++)
		{
			pGraph_->RemoveFilter(*it);
		}
	}

#if defined(_DEBUG)
	if(SUCCEEDED(hr))
		print_num_filters_in_filter_graph(pGraph_);
#endif

	if(video_callback_delegate_)
		video_callback_delegate_->Release();

	if(audio_callback_delegate_)
		audio_callback_delegate_->Release();

	if(hVideoReceivedEvent_)
	{
		CloseHandle(hVideoReceivedEvent_);
		hVideoReceivedEvent_ = NULL;
	}

	if(hAudioReceivedEvent_)
	{
		CloseHandle(hAudioReceivedEvent_);
		hAudioReceivedEvent_ = NULL;
	}

	if(get_video_streams())
		FreeMediaType(video_mediatype_);

	if(get_audio_streams())
		FreeMediaType(audio_mediatype_);
}

inline bool directshow_input::is_valid() const
{
	return valid_;
}

inline bool directshow_input::is_seekable( ) const
{
	return pMediaSeeking_ != NULL;
}

inline const openpluginlib::wstring directshow_input::get_uri( ) const
{
	return uri_;
}

inline const openpluginlib::wstring directshow_input::get_mime_type( ) const
{
	return mime_type_;
}

inline int directshow_input::get_frames( ) const
{
	return static_cast<int>(total_frames_);
}

inline int directshow_input::get_video_streams( ) const
{
	return video_mediatype_.majortype == MEDIATYPE_NULL ? 0 : 1;
}

inline int directshow_input::get_audio_streams( ) const
{
	return audio_mediatype_.majortype == MEDIATYPE_NULL ? 0 : 1;
}

HRESULT directshow_input::build_graph()
{
	HRESULT hr = pGraph_.CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC);
	if(FAILED(hr))
		return hr;

	hr = pGraph_->AddSourceFilter(uri_.c_str(), L"Source", &pSourceFilter_);
	if(FAILED(hr))
		return hr;

	hr = pVideoNullRenderer_.CoCreateInstance(CLSID_NullRenderer, NULL, CLSCTX_INPROC_SERVER);
	if(FAILED(hr))
		return hr;

	hr = pGraph_->AddFilter(pVideoNullRenderer_, L"Video Null Renderer");
	if(FAILED(hr))
		return hr;

	hr = pAudioNullRenderer_.CoCreateInstance(CLSID_NullRenderer, NULL, CLSCTX_INPROC_SERVER);
	if(FAILED(hr))
		return hr;

	hr = pGraph_->AddFilter(pAudioNullRenderer_, L"Audio Null Renderer");
	if(FAILED(hr))
		return hr;

	hr = pVideoSampleGrabber_.CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER);
	if(FAILED(hr))
		return hr;

	hr = pVideoSampleGrabber_->QueryInterface(IID_ISampleGrabber, (void**)&pISampleGrabberVideo_);
	if(FAILED(hr))
		return hr;

	hr = pGraph_->AddFilter(pVideoSampleGrabber_, L"Video Sample Grabber");
	if(FAILED(hr))
		return hr;

	AM_MEDIA_TYPE video_mt;
	memset((unsigned char*)&video_mt, 0, sizeof(AM_MEDIA_TYPE));
	video_mt.majortype	= MEDIATYPE_Video;
	video_mt.subtype	= MEDIASUBTYPE_ARGB32;
	video_mt.formattype	= FORMAT_VideoInfo;

	hr = pISampleGrabberVideo_->SetMediaType(&video_mt);
	if(FAILED(hr))
		return hr;

#ifdef BUFFER_MEDIA_SAMPLES
	hr = pISampleGrabberVideo_->SetBufferSamples(true);
#else
	hr = pISampleGrabberVideo_->SetBufferSamples(false);
#endif
	if(FAILED(hr))
		return hr;

#ifdef BUFFER_MEDIA_SAMPLES
	hr = pISampleGrabberVideo_->SetCallback(dynamic_cast<ISampleGrabberCB*>(video_callback_delegate_), 1); // copies media sample; set param 2 to 0 if you don't want to copy!
#else
	hr = pISampleGrabberVideo_->SetCallback(dynamic_cast<ISampleGrabberCB*>(video_callback_delegate_), 0);
#endif
	if(FAILED(hr))
		return hr;

	hr = pAudioSampleGrabber_.CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER);
	if(FAILED(hr))
		return hr;

	hr = pAudioSampleGrabber_->QueryInterface(IID_ISampleGrabber, (void**)&pISampleGrabberAudio_);
	if(FAILED(hr))
		return hr;

	hr = pGraph_->AddFilter(pAudioSampleGrabber_, L"Audio Sample Grabber");
	if(FAILED(hr))
		return hr;

	AM_MEDIA_TYPE audio_mt;
	memset((unsigned char*)&audio_mt, 0, sizeof(AM_MEDIA_TYPE));
	audio_mt.majortype	= MEDIATYPE_Audio;
	audio_mt.subtype	= MEDIASUBTYPE_PCM;
	audio_mt.formattype	= FORMAT_WaveFormatEx;

	hr = pISampleGrabberAudio_->SetMediaType(&audio_mt);
	if(FAILED(hr))
		return hr;

#ifdef BUFFER_MEDIA_SAMPLES
	hr = pISampleGrabberAudio_->SetBufferSamples(true);
#else
	hr = pISampleGrabberAudio_->SetBufferSamples(false);
#endif
	if(FAILED(hr))
		return hr;

#ifdef BUFFER_MEDIA_SAMPLES
	hr = pISampleGrabberAudio_->SetCallback(dynamic_cast<ISampleGrabberCB*>(audio_callback_delegate_), 1); // copies media sample; set param 2 to 0 if you don't want to copy!
#else
	hr = pISampleGrabberAudio_->SetCallback(dynamic_cast<ISampleGrabberCB*>(audio_callback_delegate_), 0);
#endif
	if(FAILED(hr))
		return hr;

	// Connect these filters together

	// Get all pins ready for connection
	CComPtr<IPin> pSourceOutputPin;
	hr = get_pin(pSourceFilter_, PINDIR_OUTPUT, &pSourceOutputPin);
	if(FAILED(hr))
		return hr;

	CComPtr<IPin> pVideoSampleGrabberInputPin;
	hr = get_pin(pVideoSampleGrabber_, PINDIR_INPUT, &pVideoSampleGrabberInputPin);
	if(FAILED(hr))
		return hr;

	CComPtr<IPin> pVideoSampleGrabberOutputPin;
	hr = get_pin(pVideoSampleGrabber_, PINDIR_OUTPUT, &pVideoSampleGrabberOutputPin);
	if(FAILED(hr))
		return hr;

	CComPtr<IPin> pVideoNullRendererInputPin;
	hr = get_pin(pVideoNullRenderer_, PINDIR_INPUT, &pVideoNullRendererInputPin);
	if(FAILED(hr))
		return hr;

	CComPtr<IPin> pAudioSampleGrabberInputPin;
	hr = get_pin(pAudioSampleGrabber_, PINDIR_INPUT, &pAudioSampleGrabberInputPin);
	if(FAILED(hr))
		return hr;

	CComPtr<IPin> pAudioSampleGrabberOutputPin;
	hr = get_pin(pAudioSampleGrabber_, PINDIR_OUTPUT, &pAudioSampleGrabberOutputPin);
	if(FAILED(hr))
		return hr;

	CComPtr<IPin> pAudioNullRendererInputPin;
	hr = get_pin(pAudioNullRenderer_, PINDIR_INPUT, &pAudioNullRendererInputPin);
	if(FAILED(hr))
		return hr;

	// Connect pins appropriately
	bool audio_connected = false;
	bool video_connected = false;
	hr = pGraph_->Connect(pSourceOutputPin, pVideoSampleGrabberInputPin);
	if(FAILED(hr))
	{
		hr = pGraph_->Connect(pSourceOutputPin, pAudioSampleGrabberInputPin);
		if(FAILED(hr))
			return hr;

		hr = pGraph_->Connect(pAudioSampleGrabberOutputPin, pAudioNullRendererInputPin);
		if(FAILED(hr))
			return hr;

		audio_connected = true;
	}
	else
	{
		hr = pGraph_->Connect(pVideoSampleGrabberOutputPin, pVideoNullRendererInputPin);
		if(FAILED(hr))
			return hr;

		video_connected = true;

		CComPtr<IEnumFilters> pIEnumFilters;
		hr = pGraph_->EnumFilters(&pIEnumFilters);
		if(SUCCEEDED(hr))
		{
			const int FILTERS_TO_REQUEST = 100;
			CComPtr<IBaseFilter> pFiltersArray[FILTERS_TO_REQUEST];
			ULONG num_got = 0;
			hr = pIEnumFilters->Next(FILTERS_TO_REQUEST, &pFiltersArray[0], &num_got);
			if(SUCCEEDED(hr))
			{
				OutputDebugStringA((std::string("Current number of filters in graph: ") + boost::lexical_cast<std::string>(num_got) + std::string("\n")).c_str());
				for(unsigned long idx = 0; idx < num_got; idx++)
				{
					std::deque< CComPtr<IPin> > output_pin_container;
					hr = get_pins(pFiltersArray[idx], PINDIR_OUTPUT, output_pin_container);
					if(FAILED(hr))
						continue;

					if(output_pin_container.size() > 1)
					{
						for(std::deque< CComPtr<IPin> >::iterator it = output_pin_container.begin();
							it != output_pin_container.end();
							++it)
						{
							IPin* pPin;
							hr = (*it)->ConnectedTo(&pPin);
							if(pPin)
								pPin->Release();
							if(hr == VFW_E_NOT_CONNECTED)
							{
								hr = pGraph_->Connect((*it), pAudioSampleGrabberInputPin);
								if(FAILED(hr))
									continue;

								hr = pGraph_->Connect(pAudioSampleGrabberOutputPin, pAudioNullRendererInputPin);
								if(SUCCEEDED(hr))
									audio_connected = true;
								break;
							}
						}
					}
				}
			}
		}
	}

	if(video_connected)
	{
		hr = pISampleGrabberVideo_->GetConnectedMediaType(&video_mediatype_); // media type needs to be processed seperately for video & audio
		if(SUCCEEDED(hr))
		{
			parse_AM_MEDIA_TYPE(video_mediatype_);

			if(		(video_mediatype_.majortype != MEDIATYPE_Video)
				||	(video_mediatype_.formattype != FORMAT_VideoInfo) )
			{
				memset(&video_mediatype_, 0, sizeof(AM_MEDIA_TYPE));
				FreeMediaType(video_mediatype_);

				return E_FAIL;
			}
		}
	}

	if(audio_connected)
	{
		hr = pISampleGrabberAudio_->GetConnectedMediaType(&audio_mediatype_); // media type needs to be processed seperately for video & audio
		if(SUCCEEDED(hr))
		{
			parse_AM_MEDIA_TYPE(audio_mediatype_);

			if(		(audio_mediatype_.majortype != MEDIATYPE_Audio)
				||	(audio_mediatype_.formattype != FORMAT_WaveFormatEx) )
			{
				memset(&audio_mediatype_, 0, sizeof(AM_MEDIA_TYPE));
				FreeMediaType(audio_mediatype_);

				return E_FAIL;
			}
		}
	}

	return S_OK;
}
	
void directshow_input::seek( const int position, const bool relative )
{
	if(!valid_)
		return;

#if defined(_DEBUG)
	try
	{
		std::string msg("Seeking to ");
		msg += boost::lexical_cast<std::string>(position);
		msg += " (relative=";
		msg += relative ? "TRUE" : "FALSE";
		msg += ")\n";
		OutputDebugStringA(msg.c_str());
	}
	catch(const boost::bad_lexical_cast&)
	{
		OutputDebugStringA("Seeking\n");
	}
#endif

	int lastPosition = input_type::get_position();
	input_type::seek(position, relative);
	int currentPosition = input_type::get_position();
	if(currentPosition == lastPosition)
		return;

	// Turn off callbacks
#ifdef BUFFER_MEDIA_SAMPLES
	pISampleGrabberVideo_->SetCallback(NULL, 1);
	pISampleGrabberAudio_->SetCallback(NULL, 1);
#else
	pISampleGrabberVideo_->SetCallback(NULL, 0);
	pISampleGrabberAudio_->SetCallback(NULL, 0);
#endif

	{	// Clear the video container
		CAutoLock video_mutex(&video_container_cs_);
		video_container_.clear();
		if(hVideoReceivedEvent_)
			ResetEvent(hVideoReceivedEvent_);
	}
	{	// Clear the audio container
		CAutoLock audio_mutex(&audio_container_cs_);
#if 0
		WAVEFORMATEX* wfex = reinterpret_cast<WAVEFORMATEX*>(audio_mediatype_.pbFormat);
		for(MediaSampleContainer::const_iterator it = audio_container_.begin();
			it != audio_container_.end();
			++it)
		{
			long bufferSize = (*it).second->GetSize();
			long samplesInBuffer = bufferSize / (2 * wfex->nChannels);

			try
			{
				OutputDebugStringA((std::string("Audio sample time: ") + boost::lexical_cast<std::string>((*it).first) + std::string("; bufferSize: ") + boost::lexical_cast<std::string>(bufferSize) + std::string("; datasize: ") + boost::lexical_cast<std::string>((*it).second->GetActualDataLength()) + std::string("\n")).c_str());
			}
			catch(const boost::bad_lexical_cast&)
			{
			}
		}
#endif
		audio_container_.clear();
		if(hAudioReceivedEvent_)
			ResetEvent(hAudioReceivedEvent_);
	}

	bool seekedOK = false;
	if(		(currentPosition == lastPosition + 1)
		&&	(pIVideoFrameStep_) )
	{
		if(		(pIVideoFrameStep_->CanStep(0, NULL) == S_OK)
			&&	(SUCCEEDED(pIVideoFrameStep_->Step(1, NULL))) )
		{
			seekedOK = true;
		}
	}

	if(		(!seekedOK)
		&&	(pMediaSeeking_) )
	{
		LONGLONG desiredPosition = currentPosition;
		HRESULT hr = pMediaSeeking_->SetPositions(	&desiredPosition,
													AM_SEEKING_AbsolutePositioning,
													NULL,
													AM_SEEKING_NoPositioning);
		if(SUCCEEDED(hr))
			seekedOK = true;
		else
			input_type::seek(lastPosition, false);
	}

	/*
	if(seekedOK)
	{
		if(hVideoReceivedEvent_)
			ResetEvent(hVideoReceivedEvent_);
		if(hAudioReceivedEvent_)
			ResetEvent(hAudioReceivedEvent_);
	}
	*/

#ifdef BUFFER_MEDIA_SAMPLES
	pISampleGrabberVideo_->SetCallback(dynamic_cast<ISampleGrabberCB*>(video_callback_delegate_), 1);
	pISampleGrabberAudio_->SetCallback(dynamic_cast<ISampleGrabberCB*>(audio_callback_delegate_), 1);
#else
	pISampleGrabberVideo_->SetCallback(dynamic_cast<ISampleGrabberCB*>(video_callback_delegate_), 0);
	pISampleGrabberAudio_->SetCallback(dynamic_cast<ISampleGrabberCB*>(audio_callback_delegate_), 0);
#endif
}

frame_type_ptr directshow_input::fetch( )
{
	if(!valid_)
		return frame_type_ptr();

	int currentPosition = get_position();

#if defined(_DEBUG)
	try
	{
		std::string msg("Fetching frame: ");
		msg += boost::lexical_cast<std::string>(currentPosition);
		msg += " [video container size=";
		msg += boost::lexical_cast<std::string>(video_container_.size());
		msg += " audio container size=";
		msg += boost::lexical_cast<std::string>(audio_container_.size());
		msg += "]\n";
		OutputDebugStringA(msg.c_str());
	}
	catch(const boost::bad_lexical_cast&)
	{
		OutputDebugStringA("Fetch!\n");
	}
#endif

	acquire_values( );

	frame_type_ptr frame = frame_type_ptr(new frame_type( ));
	frame->set_position( currentPosition );

	int process_flags = get_process_flags( );
	//*
	if (  (process_flags & process_image)
	   && (get_video_streams()) )
	{
#if 1

		bool foundMediaSample = false;
		{
			CAutoLock mutex(&video_container_cs_);
			if(video_container_.size())
				foundMediaSample = true;
		}

		VIDEOINFOHEADER* vih = reinterpret_cast<VIDEOINFOHEADER*>(video_mediatype_.pbFormat);

		if(!foundMediaSample)
		{
			try
			{
				OutputDebugStringA((std::string("AvgTimePerFrame: ") + boost::lexical_cast<std::string>(double(vih->AvgTimePerFrame)/10000) + std::string("\n")).c_str());
			}
			catch(const boost::bad_lexical_cast&)
			{
			}

			switch(WaitForSingleObject(hVideoReceivedEvent_, double(vih->AvgTimePerFrame) / 10000)) 
			{ 
			case WAIT_TIMEOUT: 
				OutputDebugStringA("timed out waiting for frame\n");
				return frame_type_ptr();
			case WAIT_OBJECT_0:
				{
					CAutoLock mutex(&video_container_cs_);
					if(video_container_.empty())
						return frame_type_ptr();
				}
				break; 
			}
		}
#else
		VIDEOINFOHEADER* vih = reinterpret_cast<VIDEOINFOHEADER*>(video_mediatype_.pbFormat);

		switch(WaitForSingleObject(hVideoReceivedEvent_, double(vih->AvgTimePerFrame) / 10000)) 
		{ 
		case WAIT_TIMEOUT: 
			OutputDebugStringA("timed out waiting for frame\n");
			return frame_type_ptr();
		case WAIT_OBJECT_0:
			{
				CAutoLock mutex(&video_container_cs_);
				if(video_container_.empty())
					return frame_type_ptr();
			}
			break; 
		}
#endif

		double frameDurationInSeconds = double(vih->AvgTimePerFrame) / 10000000;
		frame->set_duration( frameDurationInSeconds );
		frame->set_sar( vih->bmiHeader.biXPelsPerMeter, vih->bmiHeader.biYPelsPerMeter );
		frame->set_fps( 1/frameDurationInSeconds, 1 );
		frame->set_pts( (*video_container_.begin()).first );

		format_converter_ptr converter = get_format_converter(vih->bmiHeader.biCompression);
		if(!converter)
		{
			OutputDebugStringA((std::string("ERROR: FOURCC format not supported: ") + converter->get_source_format() + std::string("\n")).c_str());
			return frame_type_ptr();
		}

		// create the image object
		image_type_ptr image = oil::il::allocate( converter->get_target_format(), vih->bmiHeader.biWidth, vih->bmiHeader.biHeight );
		if(!image)
			return frame_type_ptr();

		{
			CAutoLock mutex(&video_container_cs_);
#ifdef BUFFER_MEDIA_SAMPLES
			converter->convert((*video_container_.begin()).second->pBuffer, (*video_container_.begin()).second->BufferLen, image);
#else
			CComPtr<IMediaSample> pMS = (*video_container_.begin()).second;
			unsigned char* buffer;
			HRESULT hr = pMS->GetPointer(&buffer);
			if(FAILED(hr))
				return frame_type_ptr();
			if(pMS->IsPreroll() != S_OK)
				converter->convert(buffer, unsigned long(pMS->GetActualDataLength()), image);
#endif
		}

		// Set the image attributes appropriately
		image->set_pts( frame->get_pts() );
		image->set_position( currentPosition );
		image->set_writable( false );
		image->set_flipped(true);

		// This should be done in the player itself as last step before pushing to store
		// this will save any unnecessary processing, in case frames aren't actually used/skipped
		image = oil::il::conform( image, oil::il::flipped | oil::il::cropped | oil::il::flopped );

		// Assign the image to the frame
		frame->set_image(image);
	}
	else
	//*/
	{
		frame->set_duration( 1.0 / 25 );
		frame->set_sar( 1, 1 );
		frame->set_fps( 25, 1 );
		frame->set_pts( currentPosition * 1.0 / 25 );
	}

	if(  (process_flags & process_audio)
	  && (get_audio_streams()) )
	{
		//*
		switch(WaitForSingleObject(hAudioReceivedEvent_, 400)) 
		{ 
		case WAIT_TIMEOUT: 
			OutputDebugStringA("timed out waiting for audio\n");
			return frame_type_ptr();
		case WAIT_OBJECT_0:
			{
				CAutoLock mutex(&audio_container_cs_);
				if(audio_container_.empty())
					return frame_type_ptr();
			}
			break; 
		}
		//*/

		WAVEFORMATEX* wfex = reinterpret_cast<WAVEFORMATEX*>(audio_mediatype_.pbFormat);

		if(wfex->wBitsPerSample == 16)
		{
			int fps_numerator	= 0,
				fps_denominator	= 0;
			frame->get_fps(fps_numerator, fps_denominator);

			int       numSamplesForFrame = audio_samples_for_frame( currentPosition, wfex->nSamplesPerSec, fps_numerator, fps_denominator );
			long long numSamplesToFrame  = audio_samples_to_frame( currentPosition, wfex->nSamplesPerSec, fps_numerator, fps_denominator );

			typedef audio< unsigned char, pcm16 > pcm16_audio_type;
			audio_type_ptr a = audio_type_ptr( new audio_type( pcm16_audio_type( wfex->nSamplesPerSec, wfex->nChannels, numSamplesForFrame ) ) );
			a->set_position( currentPosition );

			unsigned char* pAudioBuffer = a->data();
			int samplesLeftToCopy = numSamplesForFrame;
			bool first = true;
			CAutoLock mutex(&audio_container_cs_);
			for(MediaSampleContainer::const_iterator it = audio_container_.begin();
				it != audio_container_.end();
				++it)
			{
				long bufferSize = (*it).second->GetSize();
				long samplesInBuffer = bufferSize / (2 * wfex->nChannels);
				unsigned long dataSize = (*it).second->GetActualDataLength();

				OutputDebugStringA((std::string("Audio sample time: ") + boost::lexical_cast<std::string>((*it).first) + std::string("; bufferSize: ") + boost::lexical_cast<std::string>(bufferSize) + std::string("; datasize: ") + boost::lexical_cast<std::string>((*it).second->GetActualDataLength()) + std::string("\n")).c_str());

				int modulus = 0;
				int remainder = 0;
				if(first)
				{
					modulus = (!samplesInBuffer) ? 0 : numSamplesToFrame / samplesInBuffer;
					remainder = (!samplesInBuffer) ? 0 : numSamplesToFrame % samplesInBuffer;
				}

				unsigned char* pSampleBuffer = NULL;
				HRESULT hr = (*it).second->GetPointer(&pSampleBuffer);
				if(FAILED(hr))
					return frame_type_ptr();
				pSampleBuffer += (2 * remainder * wfex->nChannels);

				long remainingSamples = (samplesInBuffer - remainder);
				if(remainingSamples >= samplesLeftToCopy)
				{
					memcpy(	pAudioBuffer, 
							pSampleBuffer,
							samplesLeftToCopy * wfex->nChannels * 2);
					break;
				}
				else
				{
					memcpy( pAudioBuffer, 
							pSampleBuffer,  
							remainingSamples * wfex->nChannels * 2);
				}
				
				pAudioBuffer += remainingSamples * 2 * wfex->nChannels;
				samplesLeftToCopy -= remainingSamples;

/*
				{
					CAutoLock mutex(&audio_container_cs_);
#if defined(BUFFER_MEDIA_SAMPLES)
					memcpy( a->data( ), audio_container_[0].pBuffer, a->size( ) );
#else
					unsigned char* buffer;
					HRESULT hr = (*it).second->GetPointer(&buffer);
					if(FAILED(hr))
						return frame_type_ptr();
					memcpy( a->data( ), buffer, a->size( ) );
#endif
				}
*/
			}

			frame->set_audio(a);
		}
	}

	return frame;
}


//
// Plugin object
//

class ML_PLUGIN_DECLSPEC directshow_plugin : public openmedialib_plugin
{
public:
	directshow_plugin()
		: openmedialib_plugin()
	{
		::CoInitialize(0);
	}

	~directshow_plugin()
	{
		::CoUninitialize();
	}

	virtual input_type_ptr input( const opl::wstring &resource )
	{
 		typedef boost::shared_ptr< directshow_input > directshow_input_ptr;
		directshow_input_ptr input = directshow_input_ptr( new directshow_input( resource ) );
		if ( !input->is_valid( ) )
			input = directshow_input_ptr( );
		return input;
	}

	virtual store_type_ptr store( const opl::wstring &resource, const frame_type_ptr &frame )
	{
		return store_type_ptr();
	}
};

} } }

namespace
{
	void reflib( int init )
	{
		static long refs = 0;
	
		assert( refs >= 0 && L"directshow_plugin::refinit: refs is negative." );
		
		if( init > 0 && ++refs )
		{
		}
		else if( init < 0 && --refs == 0 )
		{
		}
	}
	
	CCritSec mutex;
}

//
// Access methods for openpluginlib
//

extern "C"
{
	ML_PLUGIN_DECLSPEC bool openplugin_init( void )
	{
		CAutoLock lock(&mutex);
		reflib( 1 );
		return true;
	}

	ML_PLUGIN_DECLSPEC bool openplugin_uninit( void )
	{
		CAutoLock lock(&mutex);
		reflib( -1 );
		return true;
	}

	ML_PLUGIN_DECLSPEC bool openplugin_create_plugin( const char*, opl::openplugin** plug )
	{
		*plug = new oml::ml::directshow_plugin;
		return true;
	}

	ML_PLUGIN_DECLSPEC void openplugin_destroy_plugin( opl::openplugin* plug )
	{ 
		delete static_cast< oml::ml::directshow_plugin * >( plug ); 
	}
}

#pragma warning ( pop )

#endif
