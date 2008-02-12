
// sg - A scene graph representation.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef OPENOBJECTLIB_PLUGIN_INC_
#define OPENOBJECTLIB_PLUGIN_INC_

#include <boost/filesystem/path.hpp>

#include <openpluginlib/pl/openpluginlib.hpp>

#include <openobjectlib/sg/scene.hpp>

namespace olib { namespace openobjectlib { namespace sg {

class SG_DECLSPEC openobjectlib_plugin : public olib::openpluginlib::openplugin
{
public:
	explicit openobjectlib_plugin( );

public:
	virtual scene_ptr	load(  const boost::filesystem::path& path, const openpluginlib::string& options = "" ) = 0;
	virtual bool		store( const boost::filesystem::path& path, const scene_ptr& sc ) = 0;
};

} } }

#endif
