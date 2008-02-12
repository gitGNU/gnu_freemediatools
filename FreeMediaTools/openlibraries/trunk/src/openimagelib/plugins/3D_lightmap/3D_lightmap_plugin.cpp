
// 3D_lightmap - A lightmap generator plugin to il.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#include <cstdlib>
#include <boost/tokenizer.hpp>

#include <openimagelib/il/openimagelib_plugin.hpp>

namespace il = olib::openimagelib::il;
namespace pl = olib::openpluginlib;

namespace olib { namespace openimagelib { namespace il {

namespace
{
	void destroy( il::image_type* im )
	{ delete im; }

/*
	bool discover_lightmap_size( const boost::filesystem::path& path, int& radius )
	{
		typedef boost::tokenizer< boost::char_separator<char> > tokenizer;
		
		tokenizer tok( path.leaf( ).begin( ), path.leaf( ).end( ) );
		
		radius = 1;
		
		tokenizer::const_iterator I = tok.begin( );
		if( I != tok.end( ) ) 
			radius = static_cast<int>( strtol( I->c_str( ), 0, 10 ) );

		return true;
	}
	
	il::image_type_ptr generator_image_type_to_image_type( int radius )
	{
		using namespace olib::openimagelib::il;

		typedef boost::shared_ptr<image_type>	image_type_ptr;
		typedef image<unsigned char, l8>		l8_image_type;

		return image_type_ptr( new image_type( l8_image_type( radius, radius, radius ) ), destroy );
	}

	il::image_type_ptr generate_lightmap( const boost::filesystem::path& path )
	{
		int radius;
		
		discover_lightmap_size( path, radius );
		il::image_type_ptr im = generator_image_type_to_image_type( radius );
		
		unsigned char* texels = im->data( );
		
		float radius_sq = static_cast<float>( radius * radius );
		for( int i = 0; i < radius; ++i )
		{
			for( int j = 0; j < radius; ++j )
			{
				for( int k = 0; k < radius; ++k )
				{
					float dist_sq = static_cast<float>( i * i + j * j + k * k );

					if( dist_sq < radius_sq )
					{
						float falloff = ( radius_sq - dist_sq ) / radius_sq;
						falloff *= falloff;
						
						texels[ i * radius * radius + j * radius + k ] = static_cast<unsigned char>( 255.0f * falloff );
					}
					else
					{
						texels[ i * radius * radius + j * radius + k ] = 0;
					}
				}
			}
		}
	
		return im;
	}
*/
}

class IL_DECLSPEC lightmap3D_plugin : public il::openimagelib_plugin
{
public:
	virtual il::image_type_ptr load( const pl::string& )
	{ return il::image_type_ptr( ); }
	 
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
		*plug = new il::lightmap3D_plugin;
		return true;
	}
	
	IL_DECLSPEC void openplugin_destroy_plugin( pl::openplugin* plug )
	{ delete static_cast<il::lightmap3D_plugin*>( plug ); }
}

#ifdef WIN32
extern "C" BOOL WINAPI DllMain( HINSTANCE hInstDLL, DWORD fdwReason, LPVOID )
{
	if( fdwReason == DLL_PROCESS_ATTACH )
		DisableThreadLibraryCalls( hInstDLL );
	
	return TRUE;
}
#endif
