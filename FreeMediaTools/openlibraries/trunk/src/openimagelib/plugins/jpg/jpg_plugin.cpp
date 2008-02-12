
// JPG - A JPG plugin to il.

// Copyright (C) 2005-2007 Editopia Inc.
// Released under the GPL.
// For more information, see http://www.openlibraries.org.

#ifdef HAVE_CONFIG_H
#include <openlibraries_global_config.hpp>
#endif

#include <cstdlib>
#include <vector>
#include <string>

#include <openimagelib/il/openimagelib_plugin.hpp>

extern "C" {
#include <jpeglib.h>
}

#ifdef HAVE_JPEGHDR
#include <jpeghdr.h>
#include <color.h>
#endif

namespace il = olib::openimagelib::il;
namespace pl = olib::openpluginlib;

namespace olib { namespace openimagelib { namespace il {

namespace
{
	struct olib_jpeg_source_mgr
	{
		struct jpeg_source_mgr pub;
		
		pl::stream_ptr stream;
		JOCTET* buffer;
		boolean start_of_file;
	};
	
	typedef olib_jpeg_source_mgr* olib_jpeg_source_mgr_ptr;
	
	extern "C" void init_source( j_decompress_ptr cinfo )
	{
		olib_jpeg_source_mgr_ptr src = ( olib_jpeg_source_mgr_ptr ) cinfo->src;
		src->start_of_file = TRUE;
	}

	extern "C" boolean fill_input_buffer( j_decompress_ptr cinfo )
	{
		olib_jpeg_source_mgr_ptr src = ( olib_jpeg_source_mgr_ptr ) cinfo->src;
		
		src->pub.next_input_byte = ( JOCTET* ) src->stream->data( );
		src->pub.bytes_in_buffer = src->stream->max_size( );
		src->start_of_file = FALSE;
		
		return TRUE;
	}

	extern "C" void skip_input_data( j_decompress_ptr cinfo, long num_bytes )
	{
		olib_jpeg_source_mgr_ptr src = ( olib_jpeg_source_mgr_ptr ) cinfo->src;
		
		src->pub.next_input_byte += static_cast<std::size_t>( num_bytes );
		src->pub.bytes_in_buffer -= static_cast<std::size_t>( num_bytes );
	}

	extern "C" void term_source( j_decompress_ptr )
	{
	}
	
	void jpeg_oil_stream_src( j_decompress_ptr cinfo, pl::stream_ptr stream )
	{
		olib_jpeg_source_mgr_ptr src = 0;
		
		if( cinfo->src == NULL )
		{
			cinfo->src = ( jpeg_source_mgr* ) ( *cinfo->mem->alloc_small )( ( j_common_ptr ) cinfo, JPOOL_PERMANENT, sizeof( olib_jpeg_source_mgr ) );
			memset( cinfo->src, 0, sizeof( olib_jpeg_source_mgr ) );
		}	
	
		src = ( olib_jpeg_source_mgr_ptr ) cinfo->src;
		src->stream = stream;
		src->buffer = ( JOCTET* ) stream->data( );
		src->pub.init_source = init_source;
		src->pub.fill_input_buffer = fill_input_buffer;
		src->pub.skip_input_data = skip_input_data;
		src->pub.resync_to_restart = jpeg_resync_to_restart;
		src->pub.term_source = term_source;
		src->pub.bytes_in_buffer = 0;
		src->pub.next_input_byte = NULL;
	}

	struct olib_jpeg_destination_mgr
	{
		struct jpeg_destination_mgr pub;
		
		pl::stream_ptr stream;
		JOCTET* buffer;
	};
	
	typedef olib_jpeg_destination_mgr* olib_jpeg_destination_mgr_ptr;
	
	extern "C" void init_destination( j_compress_ptr cinfo )
	{
		olib_jpeg_destination_mgr_ptr dest = ( olib_jpeg_destination_mgr_ptr ) cinfo->dest;
		
		dest->buffer = ( JOCTET* ) ( *cinfo->mem->alloc_small )( ( j_common_ptr ) cinfo, JPOOL_IMAGE, dest->stream->max_size( ) );
		dest->pub.next_output_byte = dest->buffer;
		dest->pub.free_in_buffer = dest->stream->max_size( );
	}
	
	extern "C" boolean empty_output_buffer( j_compress_ptr cinfo )
	{
		olib_jpeg_destination_mgr_ptr dest = ( olib_jpeg_destination_mgr_ptr ) cinfo->dest;

		dest->stream->write( ( pl::stream::const_pointer ) dest->buffer, dest->pub.free_in_buffer );
		
		dest->pub.next_output_byte = dest->buffer;
		dest->pub.free_in_buffer = dest->stream->max_size( );
		
		return TRUE;
	}
	
	extern "C" void term_destination( j_compress_ptr )
	{
	}
	
	void jpeg_oil_stream_dest( j_compress_ptr cinfo, pl::stream_ptr stream )
	{
		olib_jpeg_destination_mgr_ptr dest = 0;
		
		if( cinfo->dest == NULL )
		{
			cinfo->dest = ( jpeg_destination_mgr* ) ( *cinfo->mem->alloc_small )( ( j_common_ptr ) cinfo, JPOOL_PERMANENT, sizeof( olib_jpeg_destination_mgr ) );
			memset( cinfo->dest, 0, sizeof( olib_jpeg_destination_mgr ) );
		}

		dest = ( olib_jpeg_destination_mgr_ptr ) cinfo->dest;
		dest->pub.init_destination = init_destination;
		dest->pub.empty_output_buffer = empty_output_buffer;
		dest->pub.term_destination = term_destination;
		dest->stream = stream;
	}
	
	void destroy( il::image_type* im )
	{ delete im; }

	void read_jpeg( jpeg_decompress_struct* info, il::image_type::pointer pixels, size_t pitch )
	{
		int stride = info->output_width * info->output_components;
		
		JSAMPARRAY buffer = ( *info->mem->alloc_sarray )( ( j_common_ptr ) info, JPOOL_IMAGE, stride, 1 );

		while( info->output_scanline < info->output_height )
		{
			jpeg_read_scanlines( info, buffer, 1 );
			memcpy( pixels, buffer[ 0 ], stride );
			pixels += pitch;
		}
		
		jpeg_finish_decompress( info );
		jpeg_destroy_decompress( info );
	}

#ifdef HAVE_JPEGHDR
	void read_jpeg_hdr( jpeghdr_decompress_struct* info, il::image_type::pointer pixels, size_t pitch )
	{
		COLOR* buffer;
		
		buffer = new COLOR [ info->cinfo.output_width ];
		
		while( info->cinfo.output_scanline < info->cinfo.output_height )
		{
			jpeghdr_read_scanline( info, ( float* ) buffer );
			memcpy( pixels, buffer, info->cinfo.output_width * sizeof( COLOR ) );
			pixels += pitch;
		}
		
		delete[ ] buffer;
		
		jpeghdr_finish_decompress( info );
		jpeghdr_destroy_decompress( info );
	}
#endif

	il::image_type_ptr load_jpg( pl::stream_ptr stream )
	{
		if( stream->is_null( ) ) return il::image_type_ptr( );
		
		// TODO: some proper exception handling is needed. can't really be bothered
		// with that setjmp thingie...
#ifdef HAVE_JPEGHDR
		jpeghdr_decompress_struct info;
#else
		struct jpeg_decompress_struct info;
#endif
		// set error structure.
		struct jpeg_error_mgr jerr;

#ifdef HAVE_JPEGHDR
		info.cinfo.err = jpeg_std_error( &jerr );
#else
		info.err = jpeg_std_error( &jerr );
#endif

#ifdef HAVE_JPEGHDR
		jpeghdr_create_decompress( &info );
		jpeg_oil_stream_src( &info.cinfo, stream );
		
		switch( jpeghdr_read_header( &info ) )
		{
			case JPEG_HEADER_OK:
				info.cinfo.out_color_space = JCS_RGB;
				jpeg_start_decompress( &info.cinfo );
				break;
				
			case JPEG_HEADER_HDR:
				jpeghdr_start_decompress( &info );
				break;
				
			default:
				return il::image_type_ptr( );
		}
#else
		jpeg_create_decompress( &info );
		jpeg_oil_stream_src( &info, stream );
		jpeg_read_header( &info, TRUE );
		jpeg_start_decompress( &info );
#endif

#ifdef HAVE_JPEGHDR
		il::image_type_ptr image;
		if( jpeghdr_decompress_ishdr( &info ) )
			image = il::allocate( L"r32g32b32f", info.cinfo.output_width, info.cinfo.output_height );
		else
			image = il::allocate( L"r8g8b8", info.cinfo.output_width, info.cinfo.output_height );
#else
		il::image_type_ptr image = il::allocate( L"r8g8b8", info.output_width, info.output_height );
#endif
		if( !image ) return il::image_type_ptr( );

		il::image_type::pointer pixels = image->data( );
		
#ifdef HAVE_JPEGHDR
		if( jpeghdr_decompress_ishdr( &info ) )
			read_jpeg_hdr( &info, pixels, image->pitch( ) );
		else
			read_jpeg( &info.cinfo, pixels, image->pitch( ) );
#else
		read_jpeg( &info, pixels, image->pitch( ) );
#endif

		return image;
	}

	bool store_jpg( pl::stream_ptr stream, il::image_type_ptr img )
	{
		struct jpeg_compress_struct cinfo;
		struct jpeg_error_mgr jerr;
		
		il::image_type_ptr image = il::convert( img, L"r8g8b8" );
		image = il::conform( image, 0 );

		if ( !stream->is_null( ) ) 
		{
			il::image_type::pointer line = image->data( );
			cinfo.err = jpeg_std_error( &jerr );
			jpeg_create_compress( &cinfo );
			jpeg_oil_stream_dest( &cinfo, stream );
			cinfo.image_width = image->width( );
			cinfo.image_height = image->height( );
			cinfo.input_components = 3;
			cinfo.in_color_space = JCS_RGB;
			cinfo.optimize_coding = 1;
			jpeg_set_defaults( &cinfo );
			jpeg_set_quality( &cinfo, 50, TRUE );
			jpeg_start_compress( &cinfo, TRUE );
			for ( int i = 0 ; i < image->height( ); i ++, line += image->pitch( ) )
				jpeg_write_scanlines( &cinfo, &line, 1 );
			jpeg_finish_compress( &cinfo );
			jpeg_destroy_compress( &cinfo );
			return true;
		}

		return false;
	}
}

class IL_DECLSPEC jpg_plugin : public il::openimagelib_plugin
{
public:
	virtual il::image_type_ptr load( const pl::string& uri )
	{ return load_jpg( pl::make_stream( uri, std::ios::in ) ); }
	
	virtual bool store( const pl::string& uri, il::image_type_ptr im )
	{ return store_jpg( pl::make_stream( uri, std::ios::out ), im ); }
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
		*plug = new il::jpg_plugin;
		return true;
	}
	
	IL_DECLSPEC void openplugin_destroy_plugin( pl::openplugin* plug )
	{ delete static_cast<il::jpg_plugin*>( plug ); }
}

#ifdef WIN32
extern "C" BOOL WINAPI DllMain( HINSTANCE hInstDLL, DWORD fdwReason, LPVOID )
{
	if( fdwReason == DLL_PROCESS_ATTACH )
		DisableThreadLibraryCalls( hInstDLL );
	
	return TRUE;
}
#endif
