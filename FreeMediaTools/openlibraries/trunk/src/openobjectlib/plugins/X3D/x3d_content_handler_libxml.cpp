
// sg - A scene graph representation.

// Copyright (C) 2005-2006 Visual Media FX Ltd.
// Released under the LGPL.
// For more information, see http://www.openobjectlib.org.

#include <openpluginlib/pl/utf8_utils.hpp>

#include <openobjectlib/plugins/X3D/x3d_content_handler_libxml.hpp>

namespace opl = olib::openpluginlib;
namespace sg  = olib::openobjectlib::sg;

namespace
{
#ifdef __cplusplus
extern "C" {
#endif

	// callbacks to interface with libxml2 sax interface.
	void ool_startDocument( void* user_data )
	{ }

	void ool_endDocument( void* user_data )
	{ }

	void ool_startElement( void* user_data, const xmlChar* name, const xmlChar** attrs )
	{
		sg::actions::x3d_parser_action* action = static_cast<sg::actions::x3d_parser_action*>( user_data );

		action->set_attributes( const_cast<xmlChar**>( attrs ) );
		action->dispatch( opl::to_wstring( ( const char* ) BAD_CAST name ) );
	}

	void ool_endElement( void* user_data, const xmlChar* name )
	{
		namespace sg = olib::openobjectlib::sg;

		sg::actions::x3d_parser_action* action = static_cast<sg::actions::x3d_parser_action*>( user_data );
		
		// this pop is not totally symmetric with the dispatch above.
		// ideally we would like to do the pop in a dispatch type
		// method. however it seems totally unnecessary since it
		// only adds to a bit of ugly code.

		action->pop( );
	}

#ifdef __cplusplus
}
#endif
}

namespace olib { namespace openobjectlib { namespace plugins { namespace X3D {

//
x3d_content_handler_libxml::x3d_content_handler_libxml( )
	: action_( new sg::actions::x3d_parser_action( ) )
{
	memset( &handler_, 0, sizeof( xmlSAXHandler ) );

	handler_.startDocument 	= ool_startDocument;
	handler_.endDocument 	= ool_endDocument;
	handler_.startElement 	= ool_startElement;
	handler_.endElement 	= ool_endElement;
}

//
sg::scene_ptr x3d_content_handler_libxml::root( )
{
	return action_->get_scene( );
}

//
sg::actions::x3d_parser_action* x3d_content_handler_libxml::get_action( )
{ return action_.get( ); }

//
xmlSAXHandler* x3d_content_handler_libxml::get_content_handler( )
{ return &handler_; }

//
void x3d_content_handler_libxml::set_base_url( const boost::filesystem::path& base_url )
{ action_->set_base_url( base_url ); }

} } } }
