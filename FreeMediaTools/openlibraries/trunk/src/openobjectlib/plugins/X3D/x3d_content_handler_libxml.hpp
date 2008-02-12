
// sg - A scene graph representation.

// Copyright (C) 2005 Visual Media FX Ltd.
// Released under the LGPL.
// For more information, see http://www.openobjectlib.org.

#ifndef X3D_CONTENT_HANDLER_LIBXML_INC_
#define X3D_CONTENT_HANDLER_LIBXML_INC_

#include <boost/filesystem/path.hpp>

#include <openobjectlib/sg/core.hpp>
#include <openobjectlib/plugins/X3D/actions/x3d_parser_action.hpp>

namespace olib { namespace openobjectlib { namespace plugins { namespace X3D {

//
class x3d_content_handler_libxml
{
public:
	explicit x3d_content_handler_libxml( );

	sg::scene_ptr 				root( );
	sg::actions::x3d_parser_action*		get_action( );
	xmlSAXHandler*				get_content_handler( );
	void					set_base_url( const boost::filesystem::path& base_url );
				
private:
	boost::shared_ptr<sg::actions::x3d_parser_action> 	action_;
	xmlSAXHandler										handler_;
};

} } } }

#endif
