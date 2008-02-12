// sqlite3_metadata_plugin.hpp
//
// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef SQLITE3_METADATA_PLUGIN_INC_
#define SQLITE3_METADATA_PLUGIN_INC_

#include <openassetlib/plugins/sqlite/config.hpp>
#include <openassetlib/al/al_plugin.hpp>

namespace opl = olib::openpluginlib;
namespace al = olib::openassetlib::al;

namespace olib { namespace openassetlib { namespace plugins { namespace sqlite3 {

class database;
class AL_SQLITE3_DECLSPEC sqlite3_metadata_plugin : public al::al_plugin
{
public:
	sqlite3_metadata_plugin(const char* config);
	virtual ~sqlite3_metadata_plugin(void);

	al::set_ptr	store(const al::set_ptr& set);
	al::set_ptr	load(const opl::string& query_xml);

private:
	database*	pdb_;
};

} } } }

#endif
