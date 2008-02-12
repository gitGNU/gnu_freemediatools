
// DAE - An DAE plugin to sg.

// Copyright (C) 2005-2006 VM Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef DAE_PLUGIN_INC_
#define DAE_PLUGIN_INC_

#include <boost/filesystem/path.hpp>
#include <openobjectlib/plugins/Collada/config.hpp>
#include <openobjectlib/sg/openobjectlib_plugin.hpp>

namespace olib { namespace openobjectlib { namespace plugins { namespace dae {

struct DAE_DECLSPEC DAE_plugin : public sg::openobjectlib_plugin
{
    virtual sg::scene_ptr load ( const boost::filesystem::path& path, const openpluginlib::string& options = "" );
    virtual bool		  store( const boost::filesystem::path& path, const sg::scene_ptr& image );
};

} } } }

#endif
