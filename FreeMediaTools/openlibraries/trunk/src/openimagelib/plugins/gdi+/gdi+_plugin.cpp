
// GDI+ - A GDI+ plugin to il.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifdef WIN32
#include <windows.h>
#include <gdiplus.h>


#include <cstdlib>
#include <vector>
#include <string>

#include <boost/thread/recursive_mutex.hpp>

#include <openpluginlib/pl/utf8_utils.hpp>
#include <openimagelib/il/openimagelib_plugin.hpp>

namespace il = olib::openimagelib::il;
namespace pl = olib::openpluginlib;

namespace olib { namespace openimagelib { namespace il {

namespace
{
	void destroy( il::image_type* im )
	{ delete im; }

	struct mime_type_equals_png 
		: public std::unary_function<bool, Gdiplus::ImageCodecInfo>
	{
		bool operator( )( const Gdiplus::ImageCodecInfo& info )
		{
			return wcscmp( info.MimeType, L"image/png" ) == 0;
		}
	};

	bool has_png_encoder_installed( CLSID* clsid )
	{
		using namespace Gdiplus;

		UINT num, size;
		GetImageEncodersSize( &num, &size );

		ImageCodecInfo* encoders = reinterpret_cast<ImageCodecInfo*>( malloc( size ) );
		GetImageEncoders( num, size, encoders );

		bool found = false;

		typedef const ImageCodecInfo* const_iterator;
		const_iterator I = std::find_if( encoders, encoders + num, mime_type_equals_png( ) );
		if( I != encoders + num )
		{
			*clsid = I->Clsid;
			found = true;
		}
	
		free( encoders );
	
		return found;
	}

	il::image_type_ptr gdiplus_pixelformat_to_image_type( Gdiplus::PixelFormat pixelformat, int width, int height )
	{
		switch( pixelformat )
		{
			case PixelFormat24bppRGB:
				return il::allocate( L"b8g8r8", width, height );
				
			case PixelFormat32bppARGB:
				return il::allocate( L"b8g8r8a8", width, height );

			case PixelFormat32bppPARGB:
			default:
				return il::image_type_ptr( );
		}
	}
				
	il::image_type_ptr load_image( const pl::string& uri )
	{
		Gdiplus::Bitmap bitmap( pl::to_wstring( uri ).c_str( ) );

		Gdiplus::BitmapData bitmapData;
		Gdiplus::Rect rect( 0, 0, bitmap.GetWidth( ), bitmap.GetHeight( ) );

		bitmap.LockBits( &rect, Gdiplus::ImageLockModeRead, bitmap.GetPixelFormat( ), &bitmapData );

		il::image_type_ptr image = gdiplus_pixelformat_to_image_type( 
										bitmap.GetPixelFormat( ),
										bitmap.GetWidth( ),
										bitmap.GetHeight( ) );

		if( !image ) return il::image_type_ptr( );

		unsigned char* scan0	= static_cast<unsigned char*>( bitmapData.Scan0 );
		unsigned char* pixels	= image->data( );

		int stride = bitmapData.Stride;
		image->set_flipped( stride < 0 );

		size_t linesize = image->linesize( );
		for( image_type::size_type i = 0; i < image->height( ); ++i )
		{
			memcpy( pixels, scan0, linesize );

			scan0	+= stride;
			pixels	+= image->pitch( );
		}

		bitmap.UnlockBits( &bitmapData );

		return image;
	}
/*
	bool store_png( const fs::path& path, const image_type_ptr& image )
	{
		using namespace Gdiplus;

		EncoderParameters encoder_params;
		ULONG			  param_value;

		encoder_params.Count = 1;
		encoder_params.Parameter[ 0 ].Guid			 = EncoderSaveFlag;
		encoder_params.Parameter[ 0 ].Type			 = EncoderParameterValueTypeLong;
		encoder_params.Parameter[ 0 ].NumberOfValues = 1;
		encoder_params.Parameter[ 0 ].Value			 = &param_value;

		CLSID encoder_clsid;
		if( !has_png_encoder_installed( &encoder_clsid ) )
			return false;

		PixelFormat pf = image_pf_to_gdiplus_pixelformat( image->pf( ) );

		std::auto_ptr<Bitmap> im( new Bitmap( static_cast<INT>( image->width( ) ), static_cast<INT>( image->height( ) ), static_cast<INT>( image->pitch( ) ), pf, image->data( ) ) );

		Status status = im->Save( to_wstring( path.native_file_string( ) ).c_str( ), &encoder_clsid, &encoder_params );
		if( status != Ok ) return false;

		return true;
	}
*/
}

class IL_DECLSPEC gdi_plugin : public il::openimagelib_plugin
{
public:
	virtual il::image_type_ptr load( const pl::string& uri )
	{ return load_image( uri ); }
	
	virtual bool store( const pl::string&, il::image_type_ptr )
	{ return false; }
};

} } }

namespace
{
	void reflib( int init )
	{
		static long refs = 0;
#ifdef WIN32
		static ULONG_PTR gdiplusToken;
#endif // WIN32

		assert( refs >= 0 && L" GDI+_plugin::refinit: refs is negative." );
		
		if( init > 0 && ++refs )
		{
#	ifdef WIN32
			Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		
			Gdiplus::GdiplusStartup( &gdiplusToken, &gdiplusStartupInput, NULL );
#	endif
		}
		else if( init < 0 && --refs == 0 )
		{
#	ifdef WIN32
			Gdiplus::GdiplusShutdown( gdiplusToken );
#	endif
		}
	}
	
	boost::recursive_mutex mutex;
}

extern "C"
{
	IL_DECLSPEC bool openplugin_init( void )
	{
		boost::recursive_mutex::scoped_lock lock( mutex );
		
		reflib( 1 );

		return true;
	}

	IL_DECLSPEC bool openplugin_uninit( void )
	{
		boost::recursive_mutex::scoped_lock lock( mutex );

		reflib( -1 );
		
		return true;
	}
	
	IL_DECLSPEC bool openplugin_create_plugin( const char*, pl::openplugin** plug )
	{
		*plug = new il::gdi_plugin;
		
		return true;
	}
	
	IL_DECLSPEC void openplugin_destroy_plugin( pl::openplugin* plug )
	{ delete static_cast<il::gdi_plugin*>( plug ); }
}

#ifdef WIN32
extern "C" BOOL WINAPI DllMain( HINSTANCE hInstDLL, DWORD fdwReason, LPVOID )
{
	if( fdwReason == DLL_PROCESS_ATTACH )
		DisableThreadLibraryCalls( hInstDLL );
	
	return TRUE;
}
#endif


#endif // WIN32
