
// DAE - An DAE plugin to sg.

// Copyright (C) 2005-2006 VM Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifdef HAVE_CONFIG_H
#include <openlibraries_global_config.hpp>
#endif

#include <boost/filesystem/path.hpp>

#include <openobjectlib/plugins/Collada/dae_plugin.hpp>
#include <openpluginlib/pl/utf8_utils.hpp>

#ifdef WIN32
#include <openobjectlib/plugins/Collada/dae_content_handler_msxml.hpp>
#else
#include <libxml/parser.h>
#include <openobjectlib/plugins/Collada/dae_content_handler_libxml.hpp>
#endif

namespace fs	= boost::filesystem;
namespace opl	= olib::openpluginlib;

namespace olib { namespace openobjectlib { namespace plugins { namespace dae {

    namespace
    {
        sg::scene_ptr load_dae( const fs::path& path )
        {
            sg::scene_ptr sc( new sg::scene( ) );

#ifdef WIN32
            MSXML2::ISAXXMLReaderPtr pSAXReader = NULL;
            HRESULT hr = pSAXReader.CreateInstance( __uuidof( MSXML2::SAXXMLReader40 ) );

            if( FAILED( hr ) )
                return sc;

            dae_content_handler_msxml* ch = new dae_content_handler_msxml( );
            ch->set_base_url( path.branch_path( ) );

            hr = pSAXReader->putContentHandler( ch );
            if( FAILED( hr ) )
                return sc;

            hr = pSAXReader->parseURL( ( unsigned short* ) opl::to_wstring( path.native_file_string( ).c_str( ) ).c_str( ) );
            if( FAILED( hr ) )
                return sc;
#else
            std::auto_ptr<dae_content_handler_libxml> ch( new dae_content_handler_libxml );
            ch->set_base_url( path.branch_path( ) );

            if( xmlSAXUserParseFile( ch->get_content_handler( ), ch->get_action( ), path.native_file_string( ).c_str( ) ) < 0 )
                return sc;
#endif

            return ch->root( );
        }
    }

    sg::scene_ptr DAE_plugin::load( const boost::filesystem::path& path, const openpluginlib::string& /*options*/ )
    { return load_dae( path ); }

    bool DAE_plugin::store( const boost::filesystem::path& /*path*/, const sg::scene_ptr& /*root*/ )
    { return false; }

} } } }
