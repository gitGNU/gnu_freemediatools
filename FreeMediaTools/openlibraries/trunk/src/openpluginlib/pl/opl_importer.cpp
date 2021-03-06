
// openpluginlib - A plugin interface to openlibraries.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#define _WIN32_DCOM
#include <windows.h>
#include <objbase.h>
#else
#include <libxml/parser.h>
#endif

#ifdef WIN32
#include <openpluginlib/pl/content_handler_msxml.hpp>
#else
#include <openpluginlib/pl/content_handler_libxml.hpp>
#endif

#include <openpluginlib/pl/opl_importer.hpp>
#include <openpluginlib/pl/utf8_utils.hpp>

namespace olib { namespace openpluginlib {

opl_importer::opl_importer( )
{
#ifdef WIN32
	CoInitializeEx( NULL, COINIT_SPEED_OVER_MEMORY | COINIT_APARTMENTTHREADED );
#endif
}

opl_importer::~opl_importer( )
{
#ifdef WIN32
	CoUninitialize( );
#endif
}

void opl_importer::operator( )( const boost::filesystem::path& file )
{
	namespace fs = boost::filesystem;
	
#ifdef WIN32
	MSXML2::ISAXXMLReaderPtr pSAXReader = NULL;
	HRESULT hr = pSAXReader.CreateInstance( __uuidof( MSXML2::SAXXMLReader60 ) );
	if( FAILED( hr ) )
		return;
	
	content_handler_msxml* ch = new content_handler_msxml( );
	ch->set_branch_path( file.branch_path( ) );

	hr = pSAXReader->putContentHandler( ch );
	if( FAILED( hr ) )
		return;

	hr = pSAXReader->parseURL( ( unsigned short* ) to_wstring( file.native_file_string( ).c_str( ) ).c_str( ) );
	if( FAILED( hr ) )
		return;
#else
	std::auto_ptr<content_handler_libxml> ch( new content_handler_libxml( ) );
	ch->set_branch_path( file.branch_path( ) );

	if( xmlSAXUserParseFile( ch->get_content_handler( ), &ch->get_action( ), file.native_file_string( ).c_str( ) ) < 0 )
		; // we need log functionality!
#endif

	plugins = ch->get_action( ).plugins;
}

} }
