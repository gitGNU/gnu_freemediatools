// database.hpp

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef DATABASE_INC_
#define DATABASE_INC_

#include <boost/filesystem/path.hpp>
#include <openpluginlib/pl/utf8_utils.hpp>
#include <openassetlib/al/asset.hpp>
#include <openassetlib/al/set.hpp>

#ifdef WIN32
#include <openassetlib/plugins/sqlite/sqlite3.h>
#else
#include <sqlite3.h>
#endif

namespace fs  = boost::filesystem;
namespace opl = olib::openpluginlib;
namespace al = olib::openassetlib::al;

namespace olib { namespace openassetlib { namespace plugins { namespace sqlite3 {

class database
{
public:
	static const unsigned short SCHEMA_VERSION_MAJOR;
	static const unsigned short SCHEMA_VERSION_MINOR;
	static const unsigned short SCHEMA_VERSION_REVISION;

	database(void);
	virtual ~database(void);

	bool	open(const fs::path& filepath);
	void	close();
	bool	isOpen();

	bool			store(const al::asset_ptr& asset);
	al::asset_ptr	restore(const opl::string& guid_string);

private:
	bool			validate();
	bool			validateSchemaVersion();
	bool			validateAssetTable();
	bool			validateMetadataTable();
	void			dropTables();
	bool			createTables();
	bool			createSchemaVersionTable();
	bool			createAssetTable();
	bool			createMetadataTable();
	int				queryAssetID(const opl::string& guid_string);
	opl::string		quote(const opl::string& string);
	bool			executeCommand(const char* sql);
	bool			executeQuery(const char* sql, char**& resultSet, int& numRows, int& numColumns) const;
	void			freeResultSet(char**& resultSet) const;

	bool		open_;
	::sqlite3*	pdb_;
};

} } } }

#endif
