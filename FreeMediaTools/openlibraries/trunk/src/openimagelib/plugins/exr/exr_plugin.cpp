
// EXR - An ILM OpenEXR plugin to il.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#include <ImfRgbaFile.h>
#include <ImfCRgbaFile.h>
#include <ImfArray.h>

#include <openimagelib/il/openimagelib_plugin.hpp>

namespace il = olib::openimagelib::il;
namespace pl = olib::openpluginlib;

namespace olib { namespace openimagelib { namespace il {

namespace
{
	void destroy( il::image_type* im )
	{ delete im; }

	il::image_type_ptr load_exr( const pl::string& uri )
	{
		Imf::RgbaInputFile file( uri.c_str( ) );
		
		Imath::Box2i dw = file.header( ).dataWindow( );
		int width  = dw.max.x - dw.min.x + 1;
		int height = dw.max.y - dw.min.y + 1;
		
		Imf::Array2D<Imf::Rgba> exr_texels;
		exr_texels.resizeErase( height, width );
		
		file.setFrameBuffer( &exr_texels[ 0 ][ 0 ], 1, width );
		file.readPixels( dw.min.y, dw.max.y );

		il::image_type_ptr im = il::allocate( L"r32g32b32f", width, height );
		il::image_type::pointer texels = im->data( );
		
		for( int j = 0; j < height; ++j )
		{
			for( int i = 0; i < width; ++i )
			{
				( ( float* ) texels )[ 0 ] = exr_texels[ j ][ i ].r;
				( ( float* ) texels )[ 1 ] = exr_texels[ j ][ i ].g;
				( ( float* ) texels )[ 2 ] = exr_texels[ j ][ i ].b;

				texels += 3 * sizeof( float );
			}

			texels += ( im->pitch( ) - im->linesize( ) ) * sizeof( float );
		}

		return im;
	}
}

struct IL_DECLSPEC exr_plugin : public il::openimagelib_plugin
{
	virtual il::image_type_ptr load( const pl::string& uri )
	{ return load_exr( uri ); }
	
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
		*plug = new il::exr_plugin;
		return true;
	}
	
	IL_DECLSPEC void openplugin_destroy_plugin( pl::openplugin* plug )
	{ delete static_cast<il::exr_plugin*>( plug ); }
}

#ifdef WIN32
extern "C" BOOL WINAPI DllMain( HINSTANCE hInstDLL, DWORD fdwReason, LPVOID )
{
	if( fdwReason == DLL_PROCESS_ATTACH )
		DisableThreadLibraryCalls( hInstDLL );
	
	return TRUE;
}
#endif

