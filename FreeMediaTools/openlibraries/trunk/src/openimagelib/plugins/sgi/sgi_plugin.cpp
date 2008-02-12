
// SGI - A SGI plugin to il.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#include <cassert>
#include <vector>
#include <string>

#include <openimagelib/il/openimagelib_plugin.hpp>

namespace il = olib::openimagelib::il;
namespace pl = olib::openpluginlib;

namespace olib { namespace openimagelib { namespace il {

namespace
{
	struct SGI_image
	{
		unsigned short magic;
		unsigned short type;
		unsigned short dim;
		unsigned short xsize, ysize, zsize;
		unsigned long rle_end;
		std::vector<unsigned int> row_start;
		std::vector<unsigned int> row_size;
	};
				
	void convert_short( unsigned short* array, long length )
	{
		unsigned char* ptr = reinterpret_cast<unsigned char*>( array );
		
		while( length-- )
		{
			unsigned int t1 = *ptr++;
			unsigned int t2 = *ptr++;
			
			*array++ = static_cast<unsigned short>( ( t1 << 8 ) | t2 );
		}
	}

	void convert_int( unsigned int* array, long length )
	{
		unsigned char* ptr = reinterpret_cast<unsigned char*>( array );
		
		while( length-- )
		{
			unsigned int t1 = *ptr++;
			unsigned int t2 = *ptr++;
			unsigned int t3 = *ptr++;
			unsigned int t4 = *ptr++;
			
			*array++ = ( t1 << 24 ) | ( t2 << 16 ) | ( t3 << 8 ) | t4;
		}
	}

	il::image_type_ptr sgi_image_type_to_image_type( int /*dimension*/, int channels, int width, int height )
	{
		switch( channels )
		{
			case 4:
				return il::allocate( L"r8g8b8a8", width, height );

			case 3:
				return il::allocate( L"r8g8b8a8", width, height );

			case 2:
				return il::allocate( L"l8a8", width, height );

			case 1:
				return il::allocate( L"l8", width, height );

			default:
				break;
		}

		return il::image_type_ptr( );
	}
	
	bool read_row( pl::stream_ptr stream, SGI_image& di, unsigned char* buf, int y, int z )
	{
		if( ( di.type & 0xFF00 ) == 0x0100 )
		{
			std::vector<unsigned char> tmp;
			tmp.resize( di.row_size[ y + z * di.ysize ] );
			
			stream->seek( di.row_start[ y + z * di.ysize ], std::ios::beg );
			stream->read( reinterpret_cast<char*>( &tmp[ 0 ] ), di.row_size[ y + z * di.ysize ] );

			unsigned char* s = &tmp[ 0 ];
			unsigned char* t = buf;
			
			for( ;; )
			{
				unsigned char texel = *s++;
				int count = ( int ) ( texel & 0x7F );
				
				if( !count ) break;
				
				if( ( texel & 0x80 ) != 0 )
				{
					while( count-- ) *t++ = *s++;
				}
				else
				{
					texel = *s++;
					while( count-- ) *t++ = texel;
				}
			}
		}
		else
		{
			stream->seek( 512 + ( y * di.xsize ) + ( z * di.xsize * di.ysize ), std::ios::beg );
			stream->read( reinterpret_cast<char*>( buf ), di.xsize );
		}

		return true;
	}
	
	il::image_type_ptr load_sgi( pl::stream_ptr stream )
	{
		SGI_image di;

		union { int i; char c[ 4 ]; } big_endian_test;
		big_endian_test.i = 1;
		bool is_big_endian = big_endian_test.c[ 0 ] == 1;
	
		if( stream->is_null( ) ) return il::image_type_ptr( );

		if( !stream->read( reinterpret_cast<char*>( &di ), 12 ) )
			return il::image_type_ptr( );
			
		if( is_big_endian )
			convert_short( &di.magic, 6 );
			
		il::image_type_ptr image = sgi_image_type_to_image_type( di.dim, di.zsize, di.xsize, di.ysize );
		if( !image )			  
			return il::image_type_ptr( );
					
		// if run-length encoded.
		if( ( di.type & 0xFF00 ) == 0x0100 )
		{
			unsigned int size = di.ysize * di.zsize * sizeof( unsigned int );
			
			di.row_start.resize( size );
			di.row_size.resize( size );
			
			di.rle_end = 512 + ( 2 * size );
			
			stream->seek( 512, std::ios::beg );
			
			stream->read( reinterpret_cast<char*>( &di.row_start[ 0 ] ), size );
			stream->read( reinterpret_cast<char*>( &di.row_size[ 0 ] ), size );
				
			if( is_big_endian )
			{
				convert_int( &di.row_start[ 0 ], size / sizeof( unsigned long ) );
				convert_int( &di.row_size[ 0 ],  size / sizeof( unsigned long ) );
			}
		}
		
		il::image_type::pointer texels = image->data( );
	
		std::vector<unsigned char> r, g, b, a;
		r.resize( di.xsize );
		g.resize( di.xsize );
		b.resize( di.xsize );
		a.resize( di.xsize );
					
		for( int y = 0; y < di.ysize; ++y )
		{
			if( di.zsize >= 4 )
			{
				if( !read_row( stream, di, &r[ 0 ], y, 0 ) ) return il::image_type_ptr( );
				if( !read_row( stream, di, &g[ 0 ], y, 1 ) ) return il::image_type_ptr( );
				if( !read_row( stream, di, &b[ 0 ], y, 2 ) ) return il::image_type_ptr( );
				if( !read_row( stream, di, &a[ 0 ], y, 3 ) ) return il::image_type_ptr( );
				
				unsigned char* rptr = &r[ 0 ];
				unsigned char* gptr = &g[ 0 ];
				unsigned char* bptr = &b[ 0 ];
				unsigned char* aptr = &a[ 0 ];
				long n = di.xsize;
				
				while( n-- )
				{
					texels[ 0 ] = *rptr++;
					texels[ 1 ] = *gptr++;
					texels[ 2 ] = *bptr++;
					texels[ 3 ] = *aptr++;
					
					texels += 4;
				}
			}
			else if( di.zsize == 3 )
			{
				if( !read_row( stream, di, &r[ 0 ], y, 0 ) ) return il::image_type_ptr( );
				if( !read_row( stream, di, &g[ 0 ], y, 1 ) ) return il::image_type_ptr( );
				if( !read_row( stream, di, &b[ 0 ], y, 2 ) ) return il::image_type_ptr( );
				
				unsigned char* rptr = &r[ 0 ];
				unsigned char* gptr = &g[ 0 ];
				unsigned char* bptr = &b[ 0 ];
				long n = di.xsize;
				
				while( n-- )
				{
					texels[ 0 ] = *rptr++;
					texels[ 1 ] = *gptr++;
					texels[ 2 ] = *bptr++;
					
					texels += 3;
				}
			}
			else if( di.zsize == 2 )
			{
				if( !read_row( stream, di, &r[ 0 ], y, 0 ) ) return il::image_type_ptr( );
				if( !read_row( stream, di, &a[ 0 ], y, 1 ) ) return il::image_type_ptr( );
				
				unsigned char* rptr = &r[ 0 ];
				unsigned char* aptr = &a[ 0 ];
				long n = di.xsize;
				
				while( n-- )
				{
					texels[ 0 ] = *rptr++;
					texels[ 1 ] = *aptr++;
					
					texels += 2;
				}
			}
			else
			{
				if( !read_row( stream, di, &r[ 0 ], y, 0 ) ) return il::image_type_ptr( );
				
				unsigned char* rptr = &r[ 0 ];
				long n = di.xsize;
				
				while( n-- )
				{
					texels[ 0 ] = *rptr++;
					++texels;
				}
			}
		}

		image->set_flipped( true );

		return image;
	}
}

class IL_DECLSPEC sgi_plugin : public il::openimagelib_plugin
{
public:
	virtual il::image_type_ptr load( const pl::string& uri )
	{ return load_sgi( pl::make_stream( uri, std::ios::in ) ); }
	
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
		*plug = new il::sgi_plugin;
		return true;
	}
	
	IL_DECLSPEC void openplugin_destroy_plugin( pl::openplugin* plug )
	{ delete static_cast<il::sgi_plugin*>( plug ); }
}

#ifdef WIN32
extern "C" BOOL WINAPI DllMain( HINSTANCE hInstDLL, DWORD fdwReason, LPVOID )
{
	if( fdwReason == DLL_PROCESS_ATTACH )
		DisableThreadLibraryCalls( hInstDLL );
	
	return TRUE;
}
#endif

