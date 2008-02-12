
// TGA - A TGA plugin to il.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifdef WIN32
#include <windows.h>
#endif // WIN32

#include <vector>
#include <string>

#include <openimagelib/il/openimagelib_plugin.hpp>

namespace il = olib::openimagelib::il;
namespace pl = olib::openpluginlib;

namespace olib { namespace openimagelib { namespace il {

namespace
{
	void destroy( il::image_type* im )
	{ delete im; }

	il::image_type_ptr tga_image_type_to_image_type( unsigned char type, unsigned char bpp, int width, int height )
	{
		typedef il::image<unsigned char, il::b8g8r8>	b8g8r8_image_type;
		typedef il::image<unsigned char, il::b8g8r8a8>	b8g8r8a8_image_type;

#	define TGA_TYPE_MAPPED		1
#	define TGA_TYPE_COLOR		2
#	define TGA_TYPE_GRAY		3
#	define TGA_TYPE_MAPPED_RLE	9
#	define TGA_TYPE_COLOR_RLE	10
#	define TGA_TYPE_GRAY_RLE	11

		switch( type )
		{
			case TGA_TYPE_COLOR:
			case TGA_TYPE_COLOR_RLE:
				if( bpp == 32 )
					return il::image_type_ptr( new il::image_type( b8g8r8a8_image_type( width, height, 1 ) ), destroy );
				else if( bpp == 24 )
					return il::image_type_ptr( new il::image_type( b8g8r8_image_type( width, height, 1 ) ), destroy );
							
			default:
				return il::image_type_ptr( );
		}
	}

	il::image_type_ptr load_tga( pl::stream_ptr stream )
	{				
#ifdef WIN32
		#pragma pack( push, 1 )
#else
		// TODO: do the equivalent on Linux...
#endif // WIN32

		struct TgaHeader
		{
			unsigned char idLength;
			unsigned char colorMapType;

			unsigned char imageType;

			unsigned char colorMapIndexLo, colorMapIndexHi;
			unsigned char colorMapLengthLo, colorMapLengthHi;
			unsigned char colorMapSize;

			unsigned char xOriginLo, xOriginHi;
			unsigned char yOriginLo, yOriginHi;

			unsigned char widthLo, widthHi;
			unsigned char heightLo, heightHi;

			unsigned char bpp;

			unsigned char descriptor;
		};

		struct TgaFooter
		{
			unsigned int extensionAreaOffset;
			unsigned int developerDirectoryOffset;
			char signature[ 16 ];
			char dot;
			char null;
		};

#ifdef WIN32
#pragma pack( pop )
#else
	// TODO: do the equivalent on Linux...
#endif // WIN32

		const char* data = stream->data( );

		TgaHeader head;
		head = *( ( TgaHeader* ) data );
		data += sizeof( TgaHeader );

#define TGA_SIGNATURE "TRUEVISION-XFILE"
#undef  TGA_SIGNATURE

#define TGA_DESC_ABITS		0x0F
#define TGA_DESC_HORIZONTAL	0x10
#define TGA_DESC_VERTICAL	0x20

		int width	= ( head.widthHi  << 8 ) | head.widthLo;
		int height	= ( head.heightHi << 8 ) | head.heightLo;
		int horzrev	= ( head.descriptor & TGA_DESC_HORIZONTAL );
		int vertrev = ( head.descriptor & TGA_DESC_VERTICAL );
					
#undef TGA_DESC_ABITS
#undef TGA_DESC_HORIZONTAL
#undef TGA_DESC_VERTICAL
					
		il::image_type_ptr im = tga_image_type_to_image_type( head.imageType, head.bpp, width, height );
		if( !im )
			return il::image_type_ptr( );
					
		im->set_flipped( vertrev == 0 );
		im->set_flopped( horzrev != 0 );

		il::image_type::pointer t = im->data( );
		int linesize = im->linesize( );

		if( head.imageType == TGA_TYPE_MAPPED_RLE || head.imageType == TGA_TYPE_COLOR_RLE || head.imageType == TGA_TYPE_GRAY_RLE )
		{
			int	size		= im->size( );
			int	accum_count	= 0;
			int	remainder	= 0;
			int	texel_size	= head.bpp / 8;
			
			while( size	> 0 )
			{
				unsigned char c;

				c = *( ( unsigned char* ) data );
				++data;
				
				int count = ( int ) ( c & 0x7F ) + 1;

				size		-= count * texel_size;
				accum_count	+= count * texel_size;
				
				if( accum_count > linesize )
					remainder = ( accum_count - linesize ) / texel_size;

				if( ( c & 0x80 ) != 0 )
				{
					unsigned char texel[ 4 ];
					
					memcpy( texel, data, texel_size );
					data += texel_size;
					
					int siz = count - remainder;
					while( siz-- )
					{
						memcpy( t, texel, texel_size );
						t += texel_size;
					}

					if( remainder )
					{
						t += im->pitch( ) - im->linesize( );
						while( remainder-- )
						{
							memcpy( t, texel, texel_size );
							t += texel_size;
						}
						
						accum_count = 0;
						remainder = 0;
					}
				}
				else
				{
					int siz = count - remainder;

					memcpy( t, data, siz * texel_size );
					data += siz * texel_size;
					t	 += siz * texel_size;

					if( remainder )
					{
						t += im->pitch( ) - im->linesize( );
						
						memcpy( t, data, remainder * texel_size );
						data += remainder * texel_size;						
						t	 += remainder * texel_size;
						
						accum_count = 0;
						remainder = 0;
					}
				}
			}
		}
		else
		{
			for( int i = 0; i < im->height( ); ++i )
			{
				memcpy( t, data, linesize );
				data += linesize;
				t	 += im->pitch( );
			}
		}
	
		return im;
	}

#undef TGA_TYPE_MAPPED
#undef TGA_TYPE_COLOR
#undef TGA_TYPE_GRAY
#undef TGA_TYPE_MAPPED_RLE
#undef TGA_TYPE_COLOR_RLE
#undef TGA_TYPE_GRAY_RLE
}

class IL_DECLSPEC tga_plugin : public il::openimagelib_plugin
{
public:
	virtual il::image_type_ptr load( const pl::string& uri )
	{ return load_tga( pl::make_stream( uri, std::ios::in ) ); }

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
		*plug = new il::tga_plugin;
		return true;
	}
	
	IL_DECLSPEC void openplugin_destroy_plugin( pl::openplugin* plug )
	{ delete static_cast<il::tga_plugin*>( plug ); }
}

#ifdef WIN32
extern "C" BOOL WINAPI DllMain( HINSTANCE hInstDLL, DWORD fdwReason, LPVOID )
{
	if( fdwReason == DLL_PROCESS_ATTACH )
		DisableThreadLibraryCalls( hInstDLL );
	
	return TRUE;
}
#endif
