
// sg - A scene graph representation.

// Copyright (C) 2005 Visual Media FX Ltd.
// Released under the LGPL.
// For more information, see http://www.openobjectlib.org.

#ifndef DAE_CONTENT_HANDLER_LIBXML_INC_
#define DAE_CONTENT_HANDLER_LIBXML_INC_

#include <openobjectlib/sg/spatial.hpp>
#include <openobjectlib/plugins/Collada/actions/dae_parser_action.hpp>

namespace olib { namespace openobjectlib { namespace plugins { namespace dae {

//
class dae_content_handler_libxml
{
public:
    explicit dae_content_handler_libxml( );

    sg::scene_ptr                       root( );
    sg::actions::dae_parser_action*     get_action( );
    xmlSAXHandler*                      get_content_handler( );
    void                                set_base_url( const boost::filesystem::path& base_url );

private:
    boost::shared_ptr<sg::actions::dae_parser_action>   action_;
    xmlSAXHandler                                       handler_;
};

} } } }

#endif
