
// PSD - A PSD plugin to il.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#include <cstdlib>
#include <vector>
#include <string>

#include <openimagelib/il/openimagelib_plugin.hpp>

namespace pl = olib::openpluginlib;
namespace il = olib::openimagelib::il;

namespace olib { namespace openimagelib { namespace il {

namespace
{
	void destroy( il::image_type* im )
	{ delete im; }

	bool is_psd_header( const std::string& magic )
	{ return magic == std::string( "8BPS" ); }
}

template<typename T>
int psd_short( T s[ 2 ] )
{ return ( s[ 0 ] << 8 ) + s[ 1 ]; }
				
template<typename T>
long psd_long( T s[ 4 ] )
{ return ( s[ 0 ] << 24 ) + ( s[ 1 ] << 16 ) + ( s[ 2 ] << 8 ) + s[ 3 ]; }

il::image_type_ptr load_psd( pl::stream_ptr stream )
{
#ifdef WIN32
#	pragma pack( push, 1 )
#else
	// TODO: do the equivalent on Linux...
#endif // WIN32

	struct PhotoshopHeader
	{
		char signature[ 4 ];
		char version[ 2 ];
		char reserved[ 6 ];
		char channels[ 2 ];
		char rows[ 4 ];
		char columns[ 4 ];
		char depth[ 2 ];
		char mode[ 2 ];
	};

#ifdef WIN32
#	pragma pack( pop )
#else
	// TODO: do the equivalent on Linux...
#endif // WIN32

	if( stream->is_null( ) )
		return il::image_type_ptr( );
						
	PhotoshopHeader head;
	stream->read( reinterpret_cast<char*>( &head ), sizeof( head ) );
	if( !is_psd_header( std::string( head.signature, 4 ) ) )
		return il::image_type_ptr( );
					
	return il::image_type_ptr( );
}

class IL_DECLSPEC psd_plugin : public il::openimagelib_plugin
{
public:
	virtual il::image_type_ptr load( const pl::string& uri )
	{ return load_psd( pl::make_stream( uri, std::ios::in ) ); }
	
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
		*plug = new il::psd_plugin;
		return true;
	}
	
	IL_DECLSPEC void openplugin_destroy_plugin( pl::openplugin* plug )
	{ delete static_cast<il::psd_plugin*>( plug ); }
}

#ifdef WIN32
extern "C" BOOL WINAPI DllMain( HINSTANCE hInstDLL, DWORD fdwReason, LPVOID )
{
	if( fdwReason == DLL_PROCESS_ATTACH )
		DisableThreadLibraryCalls( hInstDLL );
	
	return TRUE;
}
#endif
