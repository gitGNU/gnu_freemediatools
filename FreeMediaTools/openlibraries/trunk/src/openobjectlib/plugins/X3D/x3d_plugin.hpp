
// X3D - An X3D plugin to sg.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef X3D_PLUGIN_INC_
#define X3D_PLUGIN_INC_

#include <boost/filesystem/path.hpp>

#include <openobjectlib/plugins/X3D/config.hpp>

#include <openobjectlib/sg/openobjectlib_plugin.hpp>

namespace olib { namespace openobjectlib { namespace plugins { namespace X3D {

struct X3D_DECLSPEC X3D_plugin : public sg::openobjectlib_plugin
{
	virtual sg::scene_ptr	load(  const boost::filesystem::path& path, const openpluginlib::string& options = "" );
	virtual bool			store( const boost::filesystem::path& path, const sg::scene_ptr& root );
};

} } } }

#endif
