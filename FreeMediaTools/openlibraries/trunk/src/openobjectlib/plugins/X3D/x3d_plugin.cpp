
// X3D - An X3D plugin to sg.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#include <boost/filesystem/path.hpp>

#include <openpluginlib/pl/utf8_utils.hpp>
#include <openobjectlib/plugins/X3D/x3d_plugin.hpp>

#ifdef WIN32
#include <openobjectlib/plugins/X3D/x3d_content_handler_msxml.hpp>
#else
#include <libxml/parser.h>
#include <openobjectlib/plugins/X3D/x3d_content_handler_libxml.hpp>
#endif

namespace sg	= olib::openobjectlib::sg;
namespace fs	= boost::filesystem;
namespace opl	= olib::openpluginlib;

namespace olib { namespace openobjectlib { namespace plugins { namespace X3D {

namespace
{
	sg::scene_ptr load_x3d( const fs::path& path )
	{
		sg::scene_ptr sc( new sg::scene( ) );
		
#ifdef WIN32
		MSXML2::ISAXXMLReaderPtr pSAXReader = NULL;
		HRESULT hr = pSAXReader.CreateInstance( __uuidof( MSXML2::SAXXMLReader40 ) );

		if( FAILED( hr ) )
			return sc;

		x3d_content_handler_msxml* ch = new x3d_content_handler_msxml( );
		ch->set_base_url( path.branch_path( ) );

		hr = pSAXReader->putContentHandler( ch );
		if( FAILED( hr ) )
			return sc;

		hr = pSAXReader->parseURL( ( unsigned short* ) opl::to_wstring( path.native_file_string( ).c_str( ) ).c_str( ) );
		if( FAILED( hr ) )
			return sc;
#else
		x3d_content_handler_libxml* ch = new x3d_content_handler_libxml( );
		ch->set_base_url( path.branch_path( ) );

		if( xmlSAXUserParseFile( ch->get_content_handler( ), ch->get_action( ), path.native_file_string( ).c_str( ) ) < 0 )
			return sc;
#endif

		return ch->root( );
	}
}
			
sg::scene_ptr X3D_plugin::load( const fs::path& path, const opl::string& /*options*/ )
{ return load_x3d( path ); }

bool X3D_plugin::store( const fs::path& /*path*/, const sg::scene_ptr& /*root*/ )
{ return false; }

} } } }
