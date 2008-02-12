
// HDR - A HDR plugin to il.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifdef WIN32
#include <windows.h>
#endif

#include <cmath>
#include <limits>
#include <vector>
#include <sstream>

#include <openimagelib/il/openimagelib_plugin.hpp>

namespace il = olib::openimagelib::il;
namespace pl = olib::openpluginlib;

namespace olib { namespace openimagelib { namespace il {

namespace
{
	const int RGBE_VALID_PROGRAMTYPE	= 0x01;
	const int RGBE_VALID_GAMMA			= 0x02;
	const int RGBE_VALID_EXPOSURE		= 0x04;

	struct RgbeInfo
	{
		int   valid;
		char  program_type[ 8 ];
		float gamma;
		float exposure;
		char  format[ 16 ];
	};

	bool read_rgbe_pixels_raw( pl::stream_ptr stream, unsigned char* data, int width, int height )
	{
		int numpixels = width * height;
		while( numpixels-- )
		{
			if( !stream->read( ( char* ) data, 4 ) )
				return false;
				
			data += 4;
		}
		
		return true;
	}
	
	bool read_rgbe_pixels( pl::stream_ptr stream, il::image_type_ptr im, int width, int height )
	{
		il::image_type::pointer data = im->data( );
				
		if( width < 8 || width > 0x7FFF )
			return read_rgbe_pixels_raw( stream, data, width, height );
		
		std::vector<unsigned char> line;
		line.resize( width * 4 * 4 );
		
		unsigned char rgbe[ 4 ];
		for( int i = 0; i < height; ++i )
		{
			if( !stream->read( ( char* ) rgbe, sizeof( rgbe ) ) )
				return false;
				
			if( ( rgbe[ 0 ] != 2 ) || ( rgbe[ 1 ] != 2 ) || ( rgbe[ 2 ] & 0x80 ) )
			{
				data[ 0 ] = rgbe[ 0 ];
				data[ 1 ] = rgbe[ 1 ];
				data[ 2 ] = rgbe[ 2 ];
				data[ 3 ] = rgbe[ 3 ];
				
				data += 4;
				
				return read_rgbe_pixels_raw( stream, data, width, height );
			}
			
			if( ( ( ( int ) rgbe[ 2 ] ) << 8 | rgbe[ 3 ] ) != width )
				return false;
			
			typedef std::vector<unsigned char>::const_pointer	const_pointer;
			typedef std::vector<unsigned char>::pointer			pointer;

			pointer I = &line[ 0 ];
			
			for( int j = 0; j < 4; ++j )
			{				
				const_pointer J = &line[ ( j + 1 ) * width ];

				while( I < J )
				{
					unsigned char buf[ 2 ];
					if( !stream->read( ( char* ) buf, 2 ) )
						return false;
						
					if( buf[ 0 ] > 128 )
					{
						int count = buf[ 0 ] - 128;
						if( !count || ( count > J - I ) )
							return false;
						
						while( count-- > 0 ) *I++ = buf[ 1 ];
					}
					else
					{
						int count = buf[ 0 ];
						if( !count || ( count > ( J - I ) ) )
							return false;
						
						*I++ = buf[ 1 ];
						if( --count > 0 )
						{
							if( !stream->read( ( char* ) I, count ) )
								return false;
						}
						
						I += count;
					}
				}				
			}
			
			for( int k = 0; k < width; ++k )
			{
				data[ 0 ] = line[ k + 0 * width ];
				data[ 1 ] = line[ k + 1 * width ];
				data[ 2 ] = line[ k + 2 * width ];
				data[ 3 ] = line[ k + 3 * width ];

				data += 4;
			}
			
			data += im->pitch( ) - im->linesize( );
		}
		
		return true;
	}

	bool read_hdr_header( pl::stream_ptr stream, RgbeInfo& info, int& width, int& height )
	{
		info.valid = 0;
		memset( info.program_type, 0, sizeof( info.program_type ) );
		info.gamma = 1.0f;
		info.exposure = 1.0f;
		memset( info.format, 0, sizeof( info.program_type ) );

		char buffer[ 128 ];

		if( !stream->getline( buffer, 128 ) )
			return false;
			
		if( buffer[ 0 ] != '#' || buffer[ 1 ] != '?' )
			return false;
			
		info.valid = RGBE_VALID_PROGRAMTYPE;
		for( size_t i = 0; i < sizeof( info.program_type ); ++i )
		{
			if( buffer[ i + 2 ] == 0 || isspace( buffer[ i + 2 ] ) )
				break;

			info.program_type[ i ] = buffer[ i + 2 ];
		}

		if( !stream->getline( buffer, 128 ) )
			return false;

		for( ;; )
		{
			int dummy;
			
			if( !strcmp( buffer, "FORMAT=32-bit_rle_rgbe" ) )
			{
#if _MSC_VER >= 1400
				strncpy_s( info.format, "32-bit_rle_rgbe", strlen( "32-bit_rle_rgbe" ) );
#else
				strncpy( info.format, "32-bit_rle_rgbe", strlen( "32-bit_rle_rgbe" ) );
#endif
			}
#if _MSC_VER >= 1400
			else if( sscanf_s( buffer, "GAMMA=%g", &info.gamma ) == 1 )
#else
			else if( sscanf( buffer, "GAMMA=%g", &info.gamma ) == 1 )
#endif
			{
				info.valid |= RGBE_VALID_GAMMA;
			}
#if _MSC_VER >= 1400
			else if( sscanf_s( buffer, "EXPOSURE=%g", &info.exposure ) == 1 )
#else
			else if( sscanf( buffer, "EXPOSURE=%g", &info.exposure ) == 1 )
#endif
			{
				info.valid |= RGBE_VALID_EXPOSURE;
			}
#if _MSC_VER >= 1400
			else if( sscanf_s( buffer, "pvalue -s %d -h -df -r -y %d +x %d", &dummy, &dummy, &dummy ) == 3 )
#else
			else if( sscanf( buffer, "pvalue -s %d -h -df -r -y %d +x %d", &dummy, &dummy, &dummy ) == 3 )
#endif
			{
			}

			stream->getline( buffer, 128 );
			if( !strlen( buffer ) )
				break;
		}

		if( !stream->getline( buffer, 128 ) )
			return false;

#if _MSC_VER >= 1400
		if( sscanf_s( buffer, "-Y %d +X %d", &height, &width ) < 2 )
#else
		if( sscanf( buffer, "-Y %d +X %d", &height, &width ) < 2 )
#endif
			return false;
		
		return true;
	}

	il::image_type_ptr load_hdr( pl::stream_ptr stream )
	{
		if( stream->is_null( ) )
			return il::image_type_ptr( );

		int width, height;

		RgbeInfo rgbe_info;
		if( !read_hdr_header( stream, rgbe_info, width, height ) )
			return il::image_type_ptr( );

		il::image_type_ptr image = il::allocate( L"rgbe", width, height );
		if( !image )
			return il::image_type_ptr( );
			
		if( !read_rgbe_pixels( stream, image, width, height ) )
			return il::image_type_ptr( );
			
		return image;
	}
	
	bool store_hdr( pl::stream_ptr stream, const il::image_type_ptr& im )
	{
		if( stream->is_null( ) )
			return false;
		
		il::image_type_ptr dst = il::convert( im, L"rgbe" );
		if( dst )
		{
			il::image_type::size_type width  = dst->width( );
			il::image_type::size_type height = dst->height( );

			// write header.
			std::ostringstream buf;
			buf << "#?RGBE\nGAMMA=1.0\nEXPOSURE=1.0\nFORMAT=32-bit_rle_rgbe\n\n";
			stream->write( buf.str( ).c_str( ), buf.str( ).size( ) );
			
			buf.clear( );
			buf << "-Y " << height << "+X " << width << "\n";
			stream->write( buf.str( ).c_str( ), buf.str( ).size( ) );
			
			il::image_type::const_pointer data = dst->data( );
			il::image_type::size_type pitch = dst->pitch( );
			
			while( height-- )
			{
				stream->write( ( char* ) data, dst->linesize( ) );
				data += pitch;
			}
		}	
		
		return true;
	}
}

struct IL_DECLSPEC hdr_plugin : public il::openimagelib_plugin
{
	virtual il::image_type_ptr load( const pl::string& uri )
	{ return load_hdr( pl::make_stream( uri, std::ios::in ) ); }
	
	virtual bool store( const pl::string& uri, il::image_type_ptr im )
	{ return store_hdr( pl::make_stream( uri, std::ios::out ), im ); }
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
		*plug = new il::hdr_plugin;
		return true;
	}
	
	IL_DECLSPEC void openplugin_destroy_plugin( pl::openplugin* plug )
	{ delete static_cast<il::hdr_plugin*>( plug ); }
}

#ifdef WIN32
extern "C" BOOL WINAPI DllMain( HINSTANCE hInstDLL, DWORD fdwReason, LPVOID )
{
	if( fdwReason == DLL_PROCESS_ATTACH )
		DisableThreadLibraryCalls( hInstDLL );
	
	return TRUE;
}
#endif
