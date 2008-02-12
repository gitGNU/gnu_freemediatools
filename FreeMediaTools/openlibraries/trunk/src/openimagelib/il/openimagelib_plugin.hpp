
// il - A image library representation.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef OPENIMAGELIB_PLUGIN_INC_
#define OPENIMAGELIB_PLUGIN_INC_

#include <openpluginlib/pl/openpluginlib.hpp>
#include <openpluginlib/pl/stream.hpp>

#include <openimagelib/il/il.hpp>

namespace olib { namespace openimagelib { namespace il {

struct IL_DECLSPEC openimagelib_plugin : public olib::openpluginlib::openplugin
{
	virtual image_type_ptr	load(  const openpluginlib::string& uri ) = 0;
	virtual bool			store( const openpluginlib::string& uri, image_type_ptr image ) = 0;
};

typedef boost::shared_ptr<openimagelib_plugin> openimagelib_plugin_ptr;

} } }

#endif
