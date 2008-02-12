
// DDS - A DDS plugin to il.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#include <cstdlib>
#include <vector>
#include <string>

#include <openimagelib/il/openimagelib_plugin.hpp>

namespace il = olib::openimagelib::il;
namespace pl = olib::openpluginlib;

namespace olib { namespace openimagelib { namespace il {

namespace
{
	struct DDPFPIXELFORMAT
	{
		unsigned long dwSize;
		unsigned long dwFlags;
		unsigned long dwFourCC;
		unsigned long dwRGBBitCount;
		unsigned long dwRBitMask;
		unsigned long dwGBitMask;
		unsigned long dwBBitMask;
		unsigned long dwABitMask;
	};
	
	struct DDSCAPS2
	{
		unsigned long dwCaps1;
		unsigned long dwCaps2;
		unsigned long dwReserved[ 2 ];
	};
	
	struct DDSURFACEDESC2
	{
		unsigned long dwSize;
		unsigned long dwFlags;
		unsigned long dwHeight;
		unsigned long dwWidth;
		unsigned long dwPitchOrLinearSize;
		unsigned long dwDepth;
		unsigned long dwMipMapCount;
		unsigned long dwReserved[ 11 ];
		DDPFPIXELFORMAT ddpfPixelFormat;
		DDSCAPS2 ddsCaps;
		unsigned long dwReserved2;
	};
	
#ifdef __BIG_ENDIAN__
		// we're assuming unsigned long is 32 bits.
		void swap_endian( unsigned long& val )
		{
			val = ( ( val >> 24 ) & 0x000000ff ) |
				  ( ( val >> 8  ) & 0x0000ff00 ) |
				  ( ( val << 8  ) & 0x00ff0000 ) |
				  ( ( val << 24 ) & 0xff000000 );
		}
		
		void swap_surface_desc( DDSURFACEDESC2& ddsd )
		{
			swap_endian( ddsd.dwSize );
			swap_endian( ddsd.dwFlags );
			swap_endian( ddsd.dwHeight );
			swap_endian( ddsd.dwWidth );
			swap_endian( ddsd.dwPitchOrLinearSize );
			swap_endian( ddsd.dwDepth );
			swap_endian( ddsd.dwMipMapCount );
			
			swap_endian( ddsd.ddpfPixelFormat.dwSize );
			swap_endian( ddsd.ddpfPixelFormat.dwFlags );
			swap_endian( ddsd.ddpfPixelFormat.dwFourCC );
			swap_endian( ddsd.ddpfPixelFormat.dwRGBBitCount );
			swap_endian( ddsd.ddpfPixelFormat.dwRBitMask );
			swap_endian( ddsd.ddpfPixelFormat.dwGBitMask );
			swap_endian( ddsd.ddpfPixelFormat.dwBBitMask );
			swap_endian( ddsd.ddpfPixelFormat.dwABitMask );
			
			swap_endian( ddsd.ddsCaps.dwCaps1 );
			swap_endian( ddsd.ddsCaps.dwCaps2 );
		}
#endif

	void destroy( il::image_type* im )
	{ delete im; }
				
	bool is_dds_header( const std::string& magic )
	{ return magic == std::string( "DDS " ); }

	il::image_type_ptr ddsd_pixelformat_to_image_type( DDSURFACEDESC2 ddsd )
	{
#		ifndef MAKEFOURCC
#		define MAKEFOURCC( ch0, ch1, ch2, ch3 )							\
				( ( unsigned long ) ( unsigned char ) ( ch0 )			|	\
				( ( unsigned long ) ( unsigned char ) ( ch1 ) << 8  )	|	\
				( ( unsigned long ) ( unsigned char ) ( ch2 ) << 16 )	|	\
				( ( unsigned long ) ( unsigned char ) ( ch3 ) << 24 ) )
#		endif
#		define DDPF_FOURCC 		0x00000004
#		define DDSCAPS2_CUBEMAP 0x00000200	

		if( ddsd.ddpfPixelFormat.dwFlags & DDPF_FOURCC )
		{
			typedef il::image<unsigned char, il::dxt1> dxt1_image_type;
			typedef il::image<unsigned char, il::dxt3> dxt3_image_type;
			typedef il::image<unsigned char, il::dxt5> dxt5_image_type;
			
			switch( ddsd.ddpfPixelFormat.dwFourCC )
			{
				case MAKEFOURCC( 'D', 'X', 'T', '1' ):
					return il::image_type_ptr( 
							new il::image_type( 
									dxt1_image_type( ddsd.dwWidth, ddsd.dwHeight, ddsd.dwDepth, ddsd.dwMipMapCount, 
										( ddsd.ddsCaps.dwCaps2 & DDSCAPS2_CUBEMAP ) != 0 ) ), destroy );

				case MAKEFOURCC( 'D', 'X', 'T', '3' ):
					return il::image_type_ptr( 
							new il::image_type( 
									dxt3_image_type( ddsd.dwWidth, ddsd.dwHeight, ddsd.dwDepth, ddsd.dwMipMapCount, 
										( ddsd.ddsCaps.dwCaps2 & DDSCAPS2_CUBEMAP ) != 0 ) ), destroy );

				case MAKEFOURCC( 'D', 'X', 'T', '5' ):
					return il::image_type_ptr( 
							new il::image_type( 
									dxt5_image_type( ddsd.dwWidth, ddsd.dwHeight, ddsd.dwDepth, ddsd.dwMipMapCount, 
										( ddsd.ddsCaps.dwCaps2 & DDSCAPS2_CUBEMAP ) != 0 ) ), destroy );

				default:
					return il::image_type_ptr( );
			}
		}
		else
		{
			typedef il::image<unsigned char, il::l8>		l8_image_type;
			typedef il::image<unsigned char, il::r8g8b8>	r8g8b8_image_type;
			typedef il::image<unsigned char, il::b8g8r8a8>	b8g8r8a8_image_type;
			
			switch( ddsd.ddpfPixelFormat.dwRGBBitCount )
			{
				// XXX need to check for the bit masks (high priority).
				case 8:
					return il::image_type_ptr(
							new il::image_type(
									l8_image_type( ddsd.dwWidth, ddsd.dwHeight, ddsd.dwDepth, ddsd.dwMipMapCount,
										( ddsd.ddsCaps.dwCaps2 & DDSCAPS2_CUBEMAP ) != 0 ) ), destroy );
										
				case 24:
					return il::image_type_ptr(
							new il::image_type(
									r8g8b8_image_type( ddsd.dwWidth, ddsd.dwHeight, ddsd.dwDepth, ddsd.dwMipMapCount,
										( ddsd.ddsCaps.dwCaps2 & DDSCAPS2_CUBEMAP ) != 0 ) ), destroy );

				case 32:
					return il::image_type_ptr(
							new il::image_type(
									b8g8r8a8_image_type( ddsd.dwWidth, ddsd.dwHeight, ddsd.dwDepth, ddsd.dwMipMapCount,
										( ddsd.ddsCaps.dwCaps2 & DDSCAPS2_CUBEMAP ) != 0 ) ), destroy );

				default:
					return il::image_type_ptr( );
			}
		}

		#undef MAKEFOURCC
		#undef DDPF_FOURCC
		#undef DDSCAPS2_CUBEMAP
	}
				
	il::image_type_ptr load_dds( pl::stream_ptr stream )
	{
		typedef il::image_type::size_type size_type;
		
		if( stream->is_null( ) )
			return il::image_type_ptr( );
			
		char magic[ 4 ];
		stream->read( magic, 4 );
		if( !is_dds_header( std::string( magic, 4 ) ) )
			return il::image_type_ptr( );

		DDSURFACEDESC2 ddsd;
		stream->read( reinterpret_cast<char*>( &ddsd ), sizeof( DDSURFACEDESC2 ) );
		
#ifdef __BIG_ENDIAN__
		swap_surface_desc( ddsd );
#endif
		
		il::image_type_ptr im = ddsd_pixelformat_to_image_type( ddsd );
		if( !im )
			return il::image_type_ptr( );
					
		stream->read( reinterpret_cast<char*>( im->data( ) ), std::streamsize( im->size( ) ) );

		return im;
	}
}

struct IL_DECLSPEC dds_plugin : public il::openimagelib_plugin
{
	virtual il::image_type_ptr load( const pl::string& uri )
	{ return load_dds( pl::make_stream( uri, std::ios::in ) ); }
	
	virtual bool store( const pl::string&, il::image_type_ptr )
	{ return false; }
};

} } }

extern "C"
{
	IL_DECLSPEC bool openplugin_init( void )
	{
		return true;
	}

	IL_DECLSPEC bool openplugin_uninit( void )
	{
		return true;
	}

	IL_DECLSPEC bool openplugin_create_plugin( const char*, pl::openplugin** plug )
	{
		*plug = new il::dds_plugin;
		return true;
	}
	
	IL_DECLSPEC void openplugin_destroy_plugin( pl::openplugin* plug )
	{ delete static_cast<il::dds_plugin*>( plug ); }
}

#ifdef WIN32
extern "C" BOOL WINAPI DllMain( HINSTANCE hInstDLL, DWORD fdwReason, LPVOID )
{
	if( fdwReason == DLL_PROCESS_ATTACH )
		DisableThreadLibraryCalls( hInstDLL );
	
	return TRUE;
}
#endif
