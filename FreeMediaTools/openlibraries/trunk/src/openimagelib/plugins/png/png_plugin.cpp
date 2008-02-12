
// PNG - A PNG plugin to il.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#include <cstdlib>
#include <memory>
#include <vector>
#include <string>

#include <png.h>

#include <openimagelib/il/openimagelib_plugin.hpp>

namespace pl = olib::openpluginlib;
namespace il = olib::openimagelib::il;

namespace olib { namespace openimagelib { namespace il {

namespace
{
	void destroy( il::image_type* im )
	{ delete im; }
				
	il::image_type_ptr png_pixelformat_to_image_type( int channels, int width, int height )
	{
		switch( channels )
		{
			case 4:
				return il::allocate( L"r8g8b8a8", width, height );

			case 3:
				return il::allocate( L"r8g8b8", width, height );

			default:
				return il::image_type_ptr( );
		}
	}

	il::image_type_ptr load_png( const pl::string& uri )
	{
		// TODO: some proper exception handling is needed.
		FILE* infile = fopen( uri.c_str( ), "rb" );
		if( infile == NULL ) return il::image_type_ptr( );
		
		png_byte sig[ 8 ];
		fread( sig, 1, 8, infile );
		if( !png_check_sig( sig, 8 ) ) return il::image_type_ptr( ); 
		
		png_structp png_ptr;
		png_ptr = png_create_read_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );
		
		png_infop info_ptr;
		info_ptr = png_create_info_struct( png_ptr );
		
		png_init_io( png_ptr, infile );
		png_set_sig_bytes( png_ptr, 8 );
		png_read_info( png_ptr, info_ptr );
		
		png_uint_32 width, height;
		int bit_depth, color_type;
		png_get_IHDR( png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, NULL, NULL, NULL );

		png_uint_32 row_bytes = png_get_rowbytes( png_ptr, info_ptr );
		png_uint_32 channels  = png_get_channels( png_ptr, info_ptr );
		
		il::image_type_ptr image = png_pixelformat_to_image_type( channels, width, height );
		if( !image ) return il::image_type_ptr( );
		
		png_bytepp buffer = new png_bytep [ height ];
		for( image_type::size_type i = 0; i < image->height( ); ++i )
			buffer[ i ] = new png_byte [ row_bytes ];
		
		int 			linesize = image->linesize( );
		unsigned char* 	pixels   = image->data( );
		
		png_read_image( png_ptr, buffer );
		
		for( image_type::size_type i = 0; i < image->height( ); ++i )
		{
			memcpy( pixels, buffer[ i ], linesize );
			pixels += image->pitch( );
		}

		for( image_type::size_type i = 0; i < image->height( ); ++i )
			delete[ ] buffer[ i ];
		delete[ ] buffer;

		png_destroy_info_struct( png_ptr, &info_ptr );
		png_destroy_read_struct( &png_ptr, NULL, NULL );
		
		fclose( infile );
		
		return il::image_type_ptr( image );
	}
}

struct IL_DECLSPEC png_plugin : public il::openimagelib_plugin
{
	virtual il::image_type_ptr load( const pl::string& uri )
	{ return load_png( uri ); }
	
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
		*plug = new il::png_plugin;
		return true;
	}
	
	IL_DECLSPEC void openplugin_destroy_plugin( pl::openplugin* plug )
	{ delete static_cast<il::png_plugin*>( plug ); }
}

#ifdef WIN32
extern "C" BOOL WINAPI DllMain( HINSTANCE hInstDLL, DWORD fdwReason, LPVOID )
{
	if( fdwReason == DLL_PROCESS_ATTACH )
		DisableThreadLibraryCalls( hInstDLL );
	
	return TRUE;
}
#endif
