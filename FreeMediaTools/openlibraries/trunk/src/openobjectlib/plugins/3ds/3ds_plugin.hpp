
// 3DS - An 3DS plugin to sg.

// Copyright (C) 2005-2006 VM Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef DS_PLUGIN_INC_
#define DS_PLUGIN_INC_

#include <boost/filesystem/path.hpp>

#include <openobjectlib/plugins/3ds/config.hpp>

#include <openobjectlib/sg/openobjectlib_plugin.hpp>

namespace olib { namespace openobjectlib { namespace plugins { namespace DS {

struct DS_DECLSPEC max3ds_plugin : public sg::openobjectlib_plugin
{
	virtual sg::scene_ptr	load(  const boost::filesystem::path& path, const openpluginlib::string& options = "" );
	virtual bool			store( const boost::filesystem::path& path, const sg::scene_ptr& root );
};

} } } }

#endif
