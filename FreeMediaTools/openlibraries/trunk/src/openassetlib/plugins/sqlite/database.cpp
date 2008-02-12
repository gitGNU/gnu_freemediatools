// database.cpp
//
// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#include <iostream>
#include <stdexcept>

#include <boost/lexical_cast.hpp>

#include <openassetlib/plugins/sqlite/database.hpp>

namespace olib { namespace openassetlib { namespace plugins { namespace sqlite3 {

const unsigned short database::SCHEMA_VERSION_MAJOR		= 0;
const unsigned short database::SCHEMA_VERSION_MINOR		= 0;
const unsigned short database::SCHEMA_VERSION_REVISION	= 1;

database::database(void)
	:	open_(false)
	,	pdb_(NULL)
{
}

database::~database(void)
{
	if(pdb_)
		sqlite3_close(pdb_);
}

bool database::open(const fs::path& filepath)
{
	if(sqlite3_open(filepath.native_file_string().c_str(), &pdb_) != SQLITE_OK)
	{
		std::cerr << "ERROR: Failed to open SQLite database (" << filepath.native_file_string() << ") :" << sqlite3_errmsg(pdb_) << std::endl;
		close();
		return false;
	}

	if(!validate())
	{
		close();
		return false;
	}

	open_ = true;
	return true;
}

void database::close()
{
	if(pdb_)
		sqlite3_close(pdb_);

	pdb_	= NULL;
	open_	= false;
}

bool database::store(const al::asset_ptr& asset)
{
	if(!executeCommand("BEGIN TRANSACTION"))
		return false;

	opl::string sql;
	int assetid = queryAssetID(asset->guid()->string().c_str());
	if(assetid == -1)
	{
		// Asset does not exist in the database so insert it
		sql = "INSERT INTO asset (guid) VALUES ('" + asset->guid()->string() + "')";
		if(!executeCommand(sql.c_str()))
			goto erroneous_exit;

		assetid = queryAssetID(asset->guid()->string().c_str());
		if(assetid == -1)
			goto erroneous_exit;
	}

	try
	{
		opl::string asset_id = boost::lexical_cast<opl::string>(assetid);

		// Delete metadata related to asset that is already in database
		sql = "DELETE FROM metadata WHERE asset_id = " + asset_id;
		if(!executeCommand(sql.c_str()))
			goto erroneous_exit;

		if(!asset->empty())
		{
			al::asset::const_iterator mcit = asset->begin();

			do
			{
				sql = "INSERT INTO metadata ";
				sql += "(asset_id, key, value) VALUES (";
				sql += asset_id;
				sql += ", '";
				sql += mcit->first.c_str();
				sql += "', '";
				sql += quote(mcit->second);
				sql += "')";
				if(!executeCommand(sql.c_str()))
					goto erroneous_exit;

			} while(++mcit != asset->end());
		}

		if(!executeCommand("COMMIT TRANSACTION"))
			goto erroneous_exit;

		return true;
	}
	catch(const boost::bad_lexical_cast& e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
	}

erroneous_exit:
	executeCommand("ROLLBACK TRANSACTION");
	return false;
}

al::asset_ptr database::restore(const opl::string& guid_string)
{
	int			numRows		= 0,
				numColumns	= 0;
	char**		resultSet	= NULL;
	opl::string	sql			= "SELECT key, value FROM metadata WHERE asset_id = (SELECT asset_id FROM asset WHERE guid = '"
								+	guid_string
								+	"')";

	if(!executeQuery(sql.c_str(), resultSet, numRows, numColumns))
		return al::asset_ptr();

	const int columnOffset_key		= 0;
	const int columnOffset_value	= 1;
	const int numRequestedColumns	= 2;

	al::asset_ptr asset = al::asset_ptr();
	if(		(numColumns == numRequestedColumns)
		&&	(numRows)	)
	{
		asset = al::asset_factory::instance().create_asset(al::guid_factory::instance().create_guid(guid_string));
		for(int	rowIdx		= 0, 
				rowOffset	= 0; 
			rowIdx < numRows; 
			rowIdx++)
		{
			rowOffset = ((rowIdx + 1) * numRequestedColumns);
			asset->set(	opl::string(resultSet[rowOffset + columnOffset_key]), 
						opl::string(resultSet[rowOffset + columnOffset_value]));
		}
	}

	freeResultSet(resultSet);
	return asset;
}

bool database::validate()
{
	std::cout << "Validating database (this may cause database errors to be output to stdout)..." << std::endl;
	bool success = true;
	if(		!validateSchemaVersion()
		||	!validateAssetTable()
		||	!validateMetadataTable() )
	{
		dropTables();
		if(!createTables())
			success = false;
	}

	std::cout << "Database validation complete." << std::endl;
	return success;
}

bool database::validateSchemaVersion()
{
	// Query the Version table for database schema version
	int		numRows		= 0,
			numColumns	= 0;
	char**	resultSet	= NULL;
	char*	sql			= "SELECT major, minor, revision FROM schema_version";

	if(!executeQuery(sql, resultSet, numRows, numColumns))
		return false;

	bool success = false;
	if(		(numColumns == 3)
		&&	(numRows == 1) )
	{
		try
		{
			unsigned short major	= boost::lexical_cast<unsigned short>(resultSet[3]);
			unsigned short minor	= boost::lexical_cast<unsigned short>(resultSet[4]);
			unsigned short revision	= boost::lexical_cast<unsigned short>(resultSet[5]);

			if	(	(SCHEMA_VERSION_MAJOR		!= major)
				||	(SCHEMA_VERSION_MINOR		!= minor)
				||	(SCHEMA_VERSION_REVISION	!= revision) )
			{
				fprintf(stdout, 
					"WARNING: database versions differ. Database V%d.%d.%d. Expecting V%d.%d.%d\n", 
					major,
					minor,
					revision,
					SCHEMA_VERSION_MAJOR,
					SCHEMA_VERSION_MINOR,
					SCHEMA_VERSION_REVISION);

				// When the database tables change again this is where any intellegent database modifications need to go!!
			}

			success = true;
		}
		catch(const boost::bad_lexical_cast& e)
		{
			std::cerr << "Exception: " << e.what() << std::endl;
		}
	}

	freeResultSet(resultSet);
	return success;
}

bool database::validateAssetTable()
{
	// Note: If any of the fields are not present executeQuery will fail

	int		numRows		= 0,
			numColumns	= 0;
	char**	resultSet	= NULL;
	char*	sql			= "SELECT asset_id, guid FROM asset ORDER BY asset_id LIMIT 1";

	if(!executeQuery(sql, resultSet, numRows, numColumns))
		return false;

	freeResultSet(resultSet);
	return true;
}

bool database::validateMetadataTable()
{
	// Note: If any of the fields are not present executeQuery will fail

	int		numRows		= 0,
			numColumns	= 0;
	char**	resultSet	= NULL;
	char*	sql			= "SELECT metadata_id, asset_id, key, value FROM metadata ORDER BY metadata_id LIMIT 1";

	if(!executeQuery(sql, resultSet, numRows, numColumns))
		return false;

	freeResultSet(resultSet);
	return true;
}

void database::dropTables()
{
	executeCommand("DROP TABLE metadata");
	executeCommand("DROP TABLE asset");
	executeCommand("DROP TABLE schema_version");
}

bool database::createTables()
{
	if(		createSchemaVersionTable()
		&&	createAssetTable()
		&&	createMetadataTable()	)
		return true;

	return false;
}

bool database::createSchemaVersionTable()
{
	if(!executeCommand("CREATE TABLE schema_version(major INT NOT NULL, minor INT, revision INT)"))
		return false;

	opl::string major_ver(boost::lexical_cast<opl::string>(SCHEMA_VERSION_MAJOR));
	opl::string minor_ver(boost::lexical_cast<opl::string>(SCHEMA_VERSION_MINOR));
	opl::string revision(boost::lexical_cast<opl::string>(SCHEMA_VERSION_REVISION));

	opl::string sql =
		"INSERT INTO schema_version (major, minor, revision) VALUES("
		+ major_ver + ","
		+ minor_ver + ","
		+ revision + ")";
	if(executeCommand(sql.c_str()))
		return true;

	return false;
}

bool database::createAssetTable()
{
	if(executeCommand("CREATE TABLE asset(asset_id INTEGER NOT NULL PRIMARY KEY, guid VARCHAR(40) NOT NULL)"))
		return true;

	return false;
}

bool database::createMetadataTable()
{
	if(executeCommand("CREATE TABLE metadata(metadata_id INTEGER NOT NULL PRIMARY KEY, asset_id INTEGER NOT NULL, key varchar(255), value TEXT)"))
		return true;

	return false;
}

int	database::queryAssetID(const opl::string& guid_string)
{
	int		numRows		= 0,
			numColumns	= 0;
	char**	resultSet	= NULL;

	opl::string sql = "SELECT asset_id FROM asset WHERE guid = '";
	sql += guid_string;
	sql += "'";

	if(!executeQuery(sql.c_str(), resultSet, numRows, numColumns))
		return -1;

	int asset_id = -1;
	if(		(numRows)
		&&	(numColumns == 1)	)
	{
		try
		{
			asset_id = boost::lexical_cast<int>(opl::string(resultSet[1]));
		}
		catch(const boost::bad_lexical_cast& e)
		{
			std::cerr << "Exception: " << e.what();
		}
	}

	freeResultSet(resultSet);
	return asset_id;
}

inline bool database::isOpen()
{
	return open_;
}

inline opl::string database::quote(const opl::string& string)
{
	opl::string return_string;
	for(opl::string::const_iterator it = string.begin();
		it != string.end();
		it++)
	{
		if(*it != '\'')
			return_string.push_back(*it);
		else
		{
			return_string.push_back('\'');
			return_string.push_back('\'');
		}
	}
	return return_string;
}

inline bool database::executeCommand(const char* sql)
{
#if defined(_DEBUG)
	fprintf(stdout, "%s\n", sql);
#endif

	char*	sqliteErrorMsg	= 0;
	int		rc = sqlite3_exec(	pdb_, 
								sql, 
								NULL, 
								NULL, 
								&sqliteErrorMsg);
	if(rc != SQLITE_OK)
	{
		std::cerr << "ERROR: SQLite database failed to execute command: \"" << sql << "\" : " << sqliteErrorMsg << std::endl;
		free(sqliteErrorMsg);
		return false;
	}
	return true;
}

inline bool database::executeQuery(const char* sql, char**& resultSet, int& numRows, int& numColumns) const
{
#if defined(_DEBUG)
	fprintf(stdout, "%s\n", sql);
#endif

	char*	sqliteErrorMsg	= 0;
	int		rc	= sqlite3_get_table(pdb_, 
									sql, 
									&resultSet, 
									&numRows, 
									&numColumns, 
									&sqliteErrorMsg);
	if(rc != SQLITE_OK)
	{
		std::cerr << "ERROR: SQLite database failed to execute query: \"" << sql << "\" : " << sqliteErrorMsg << std::endl;
		free(sqliteErrorMsg);
		return false;
	}
	return true;
}


inline void database::freeResultSet(char**& resultSet) const
{
	if(resultSet != NULL)
	{
		sqlite3_free_table(resultSet);
		resultSet = NULL;
	}
}

} } } }
