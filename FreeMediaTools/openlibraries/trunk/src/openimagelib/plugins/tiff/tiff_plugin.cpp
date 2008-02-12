
// TIFF - A TIFF plugin to il.

// Copyright (C) 2005 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#include <vector>
#include <sstream>

#include <tiffio.h>

#include <openimagelib/il/openimagelib_plugin.hpp>

namespace il = olib::openimagelib::il;
namespace pl = olib::openpluginlib;

namespace olib { namespace openimagelib { namespace il {

namespace
{
	il::image_type_ptr tiff_image_type_to_image_type( unsigned short bpp, unsigned short components, int width, int height, bool is_float = false )
	{
		std::wstringstream str;
		if( components >= 3 )
		{
			str << L"r" << bpp << L"g" << bpp << L"b" << bpp;
			if( components == 4 )
				str << L"a" << bpp;
		}
		
		if( is_float ) str << L"f";
		
		return il::allocate( str.str( ), width, height );
	}

	il::image_type_ptr tiff_null_image( TIFF* tif )
	{
		TIFFClose( tif );
		return il::image_type_ptr( );
	}

	il::image_type_ptr load_tiff( const pl::string& uri )
	{
		TIFF* tif = TIFFOpen( uri.c_str( ), "r" );
		if( tif == NULL )
			return il::image_type_ptr( );

		int width, height, depth;

		TIFFGetFieldDefaulted( tif, TIFFTAG_IMAGEWIDTH,  &width );
		TIFFGetFieldDefaulted( tif, TIFFTAG_IMAGELENGTH, &height );
		if( !TIFFGetFieldDefaulted( tif, TIFFTAG_IMAGEDEPTH, &depth ) ) depth = 1;

		unsigned short config;
		TIFFGetFieldDefaulted( tif, TIFFTAG_PLANARCONFIG, &config );

		if( config != PLANARCONFIG_CONTIG )
			return tiff_null_image( tif );

		unsigned short photo, compression;
		unsigned short sample_format, orientation;
		TIFFGetFieldDefaulted( tif, TIFFTAG_PHOTOMETRIC, &photo );
		TIFFGetFieldDefaulted( tif, TIFFTAG_SAMPLEFORMAT, &sample_format );
		TIFFGetFieldDefaulted( tif, TIFFTAG_COMPRESSION, &compression );
		TIFFGetFieldDefaulted( tif, TIFFTAG_ORIENTATION, &orientation );

		bool is_float = false;
		switch( photo )
		{
			case PHOTOMETRIC_RGB:
				if( sample_format == SAMPLEFORMAT_IEEEFP )
					is_float = true;
				break;
				
			default:
				return tiff_null_image( tif );
		}
		
		unsigned short bpp, components;
		TIFFGetFieldDefaulted( tif, TIFFTAG_BITSPERSAMPLE,   &bpp );
		TIFFGetFieldDefaulted( tif, TIFFTAG_SAMPLESPERPIXEL, &components );

		il::image_type_ptr im = tiff_image_type_to_image_type( bpp, components, width, height, is_float );
		if( !im )
			return il::image_type_ptr( );

		il::image_type::pointer data = im->data( );
		int bytes_per_pixel = components * bpp / 8;

		if( TIFFIsTiled( tif ) )
		{
			int tile_width, tile_height, tile_depth;

			TIFFGetField( tif, TIFFTAG_TILEWIDTH,  &tile_width );
			TIFFGetField( tif, TIFFTAG_TILELENGTH, &tile_height );

			if( !TIFFGetField( tif, TIFFTAG_TILEDEPTH,  &tile_depth ) )
				tile_depth = 1;

			std::vector<unsigned char> buf( TIFFTileSize( tif ) );
			for( int k = 0; k < depth; k += tile_depth )
			{
				for( int j = 0; j < height; j += tile_height )
				{
					for( int i = 0; i < width; i += tile_width )
					{
						TIFFReadTile( tif, &buf[ 0 ], i, j, k, 0 );

						for( int l = 0; l < tile_depth; ++l )
						{
							for( int m = 0; m < tile_height; ++m )
								memcpy( data + ( ( ( k + l ) * height + j + m ) * width + i ) * bytes_per_pixel, 
										&buf[ 0 ] + ( l * tile_height + m ) * tile_width * bytes_per_pixel, tile_width * bytes_per_pixel );
						}
					}
				}
			}
		}
		else
		{
			size_t pitch = im->pitch( );
			if( is_float )
				pitch *= sizeof( float );

			unsigned int rows, strip_size;
			TIFFGetField( tif, TIFFTAG_ROWSPERSTRIP, &rows );
			strip_size = TIFFStripSize( tif );
			
			typedef std::vector<unsigned char> buffer_t;
			buffer_t buf( strip_size );
			
			int linesize = strip_size / rows;
			
			tstrip_t nstrips = TIFFNumberOfStrips( tif );
			for( tstrip_t i = 0; i < nstrips; ++i )
			{
				TIFFReadEncodedStrip( tif, i, &buf[ 0 ], strip_size );
				
				buffer_t::pointer buf_data = &buf[ 0 ];
				
				for( unsigned int j = 0; j < rows; ++j )
				{
					memcpy( data, buf_data, linesize );

					buf_data += linesize;
					data += pitch;
				}
			}
		}

		TIFFClose( tif );

		return im;
	}
}

class IL_DECLSPEC tiff_plugin : public il::openimagelib_plugin
{
public:
	virtual il::image_type_ptr load( const pl::string& uri )
	{ return load_tiff( uri ); }
	
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
		*plug = new il::tiff_plugin;
		return true;
	}
	
	IL_DECLSPEC void openplugin_destroy_plugin( pl::openplugin* plug )
	{ delete static_cast<il::tiff_plugin*>( plug ); }
}

#ifdef WIN32
extern "C" BOOL WINAPI DllMain( HINSTANCE hInstDLL, DWORD fdwReason, LPVOID )
{
	if( fdwReason == DLL_PROCESS_ATTACH )
		DisableThreadLibraryCalls( hInstDLL );
	
	return TRUE;
}
#endif
