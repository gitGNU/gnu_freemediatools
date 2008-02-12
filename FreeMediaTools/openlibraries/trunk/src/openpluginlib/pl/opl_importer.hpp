
// openpluginlib - A plugin interface to openlibraries.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef OPL_IMPORTER_INC_
#define OPL_IMPORTER_INC_

#if _MSC_VER >= 1400
#include <hash_map>
#endif

#include <map>
#include <string>

#include <boost/filesystem/path.hpp>

#include <openpluginlib/pl/openplugin.hpp>

namespace olib { namespace openpluginlib {

struct opl_importer
{
	typedef wstring key_type;
	
#if _MSC_VER >= 1400
	typedef stdext::hash_multimap<key_type, detail::plugin_item> container;
#else
	typedef std::multimap<key_type, detail::plugin_item> container;
#endif

	explicit opl_importer( );
	~opl_importer( );

	void operator( )( const boost::filesystem::path& file );
	
	container plugins;
};

} }

#endif
