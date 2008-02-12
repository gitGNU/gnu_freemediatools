	
// il - A asset library representation.

// Copyright (C) 2000-2006 VM Inc.
// Released under the LGPL.
// For more information, see http://www.openassetlib.org

#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include "Database.h"
#include "Asset.h"
#include "DesktopAsset.h"

#if !defined(_WIN32)
#include "StringConversions.h"
#endif

namespace olib
{
	namespace openassetlib
	{
		//#####################################################################################################################################
		const unsigned short Database::schemaVersionMajor		= 2;
		const unsigned short Database::schemaVersionMinor		= 0;
		const unsigned short Database::schemaVersionRevision	= 2;

		const unsigned char	Database::numSearchableFields = 6;
		const std::string	Database::searchableField[numSearchableFields] =	{	"Collection Name",
																					"Path",
																					"Filename",
																					"Length",
																					"Type",
																					"Date Added"
																				};

		//#####################################################################################################################################
		class Database::SQLiteDatabase
		{
		public:
			SQLiteDatabase(void)	throw();
			~SQLiteDatabase(void)	throw();

			void	open(const char* databaseURL)	throw(std::runtime_error);
			void	close()							throw();

			void	executeCommand(const char* sql)															throw(std::runtime_error);
			void	executeQuery(const char* sql, char**& resultSet, int& numRows, int& numColumns) const	throw(std::runtime_error);
			void	freeResultSet(char**& resultSet) const													throw();

			bool	operator==(const SQLiteDatabase& rhs) const	throw();
			bool	operator!=(const SQLiteDatabase& rhs) const throw();

		private:
			sqlite3* _pDatabase;
		};

		Database::SQLiteDatabase Database::_database;

		Database::SQLiteDatabase::SQLiteDatabase(void) throw()
			: _pDatabase(NULL)
		{
		}

		Database::SQLiteDatabase::~SQLiteDatabase(void) throw()
		{
			if(_pDatabase != NULL)
				sqlite3_close(_pDatabase);
		}

		void Database::SQLiteDatabase::open(const char* databaseURL) throw(std::runtime_error)
		{
			if(sqlite3_open(databaseURL, &_pDatabase) != SQLITE_OK)
			{
				std::string msg("Failed to open SQLite database (");
				msg += std::string(databaseURL);
				msg += ") :";
				msg += sqlite3_errmsg(_pDatabase);
				std::runtime_error exception(msg);
				throw exception;
			}
		}

		void Database::SQLiteDatabase::close() throw()
		{
			if(_pDatabase != NULL)
			{
				sqlite3_close(_pDatabase);
				_pDatabase = NULL;
			}
		}

		void Database::SQLiteDatabase::executeCommand(const char* sql) throw(std::runtime_error)
		{
			fprintf(stdout, "%s\n", sql);
			char*	sqliteErrorMsg	= 0;
			int		rc = sqlite3_exec(	_pDatabase, 
										sql, 
										NULL, 
										NULL, 
										&sqliteErrorMsg);
			if(rc != SQLITE_OK)
			{
				std::string msg("SQLite database failed to execute command - \"");
				msg += sql;
				msg += "\" : ";
				msg += std::string(sqliteErrorMsg);
				free(sqliteErrorMsg);
				std::runtime_error exception(msg);
				throw exception;
			}
		}

		void Database::SQLiteDatabase::executeQuery(const char* sql, char**& resultSet, int& numRows, int& numColumns) const throw(std::runtime_error)
		{
			fprintf(stdout, "%s\n", sql);
			char*	sqliteErrorMsg	= 0;
			int		rc	= sqlite3_get_table(_pDatabase, 
											sql, 
											&resultSet, 
											&numRows, 
											&numColumns, 
											&sqliteErrorMsg);
			if(rc != SQLITE_OK)
			{
				std::string msg("SQLite database failed to execute query - \"");
				msg += sql;
				msg += "\" : ";
				msg += std::string(sqliteErrorMsg);
				free(sqliteErrorMsg);
				std::runtime_error exception(msg);
				throw exception;
			}
		}


		void Database::SQLiteDatabase::freeResultSet(char**& resultSet) const throw()
		{
			if(resultSet != NULL)
			{
				sqlite3_free_table(resultSet);
				resultSet = NULL;
			}
		}

		bool Database::SQLiteDatabase::operator==(const SQLiteDatabase& rhs) const throw()
		{
			return ((_pDatabase == rhs._pDatabase) ? true : false);
		}

		bool Database::SQLiteDatabase::operator!=(const SQLiteDatabase& rhs) const throw()
		{
			return !operator==(rhs);
		}

		//#####################################################################################################################################
		// Public member functions.............................................................................................................
		Database::Database(const std::string& name) throw()
			:	DataSource(name),
				_connected(false)
		{
		}

		Database::~Database(void) throw()
		{
			disconnect();
		}

		std::string Database::name() const throw()
		{
			return _name;
		}

		DataSource::Type Database::type() const throw()
		{
			return LocalDatabase;
		}

		bool Database::isConnected() const throw()
		{
			return _connected;
		}

		void Database::connect(std::string url) throw(std::runtime_error)
		{
			try
			{
				std::string::size_type position = url.find(std::string("file://"), 0);
				if(position != std::string::npos)
					url = url.substr((position + 7), url.size() - (position + 7));
				_database.open(url.c_str());
				testDatabase();
				_connected = true;
			}
			catch(std::runtime_error& exception)
			{
				fprintf(stderr, "%s\n", exception.what());
				throw exception;
			}
		}

		void Database::disconnect() throw()
		{
			_database.close();
			_connected = false;
		}

		std::list<std::string> Database::collectionNames() const throw()
		{
			std::list<std::string> list;

			try
			{
				int		numRows		= 0,
						numColumns	= 0;
				char**	resultSet	= NULL;	
				char*	sql			= "select name from Collection";

				_database.executeQuery(	sql,
										resultSet,
										numRows,
										numColumns);

				if(		(numColumns != 0)
					&&	(numRows != 0) )
				{
					const unsigned char numRequestedColumns = 1;

					if(numColumns != numRequestedColumns)
					{
						fprintf(stderr, "Unexpected number of columns returned from database! Requested %d, Received %d\n", numRequestedColumns, numColumns);
					}
					else
					{
						for(int rowIdx = 0; rowIdx < numRows; rowIdx++)
						{
							std::string name(resultSet[numColumns + rowIdx]);
							list.push_back(name);
						}
					}
				}

				_database.freeResultSet(resultSet);
			}
			catch(std::runtime_error& exception)
			{
				fprintf(stderr, "%s\n", exception.what());
			}

			return list;
		}

		std::list<std::string> Database::searchableFields() const throw()
		{
			std::list<std::string> list;
			for(unsigned char idx = 0; idx < numSearchableFields; idx++)
			{
				list.push_back(searchableField[idx]);
			}

			return list;
		}

		std::list<Asset*> Database::search(const Query& query) const throw()
		{
			std::list<Asset*> hitList;

			try
			{
				int		numRows		= 0,
						numColumns	= 0;
				char**	resultSet	= NULL;

				char buffer[10];
				std::string hitLimit(itoa(query.maxHits(), buffer, 10));

				std::string sql("SELECT path, filename, startFrame, endFrame FROM Asset WHERE ");
				sql += buildSQL(query.condition());
				sql += " LIMIT ";
				sql += hitLimit;

				_database.executeQuery(	sql.c_str(),
										resultSet,
										numRows,
										numColumns);

				if(		(numColumns != 0)
					&&	(numRows != 0) )
				{
					const unsigned char columnOffset_path		= 0;
					const unsigned char columnOffset_filename	= 1;
					const unsigned char columnOffset_startFrame	= 2;
					const unsigned char columnOffset_endFrame	= 3;
					const int numRequestedColumns		= 4;

					if(numColumns != int(numRequestedColumns))
					{
						fprintf(stderr, "Unexpected number of columns returned from database! Requested %d, Received %d\n", numRequestedColumns, numColumns);
					}
					else
					{
						unsigned long rowOffset;
						for(int rowIdx = 0; rowIdx < numRows; rowIdx++)
						{
							rowOffset = ((rowIdx + 1) * numRequestedColumns);
							Asset* pAsset = new Asset(	resultSet[rowOffset + columnOffset_path],
														resultSet[rowOffset + columnOffset_filename],
														atoi(resultSet[rowOffset + columnOffset_startFrame]),
														atoi(resultSet[rowOffset + columnOffset_endFrame]));
							hitList.push_back(pAsset);
						}
					}
				}

				_database.freeResultSet(resultSet);
			}
			catch(std::runtime_error& exception)
			{
				fprintf(stderr, "%s\n", exception.what());
			}

			return hitList;
		}

		void Database::createCollection(const std::string& name) throw(std::runtime_error)
		{
			bool collectionNameExists = false;
			try
			{
				queryCollectionId(name);
				collectionNameExists = true;
			}
			catch(std::runtime_error& exception)
			{
				fprintf(stderr, "%s\n", exception.what());

				std::string sql("insert into Collection (id, name) values (NULL, \"");
				sql += name;
				sql += "\")";
				_database.executeCommand(sql.c_str());
			}

			if(collectionNameExists)
			{
				std::runtime_error exception("Collection of this name already exists!");
				throw exception;
			}
		}

		void Database::deleteCollection(const std::string& name) throw(std::runtime_error)
		{
			try
			{
				_database.executeCommand("begin transaction");

				// Delete all assets solely in this collection
				std::string sql("delete from Asset where id in (select assetId from CollectionAsset where collectionId=(select id from Collection where name=\"");
				sql += name;
				sql += "\") except select assetId from CollectionAsset where collectionId!=(select id from Collection where name=\"";
				sql += name;
				sql += "\"))";
				_database.executeCommand(sql.c_str());

				// Delete all rows in the CollectionAsset table that refer to the collection being deleted
				sql.clear();
				sql = "delete from CollectionAsset where collectionId=(select id from Collection where name=\"";
				sql += name;
				sql += "\")";
				_database.executeCommand(sql.c_str());

				// Delete row in Collection table (i.e. delete collection entry itself)
				sql.clear();
				sql = "delete from Collection where name=\"";
				sql += name;
				sql += "\"";
				_database.executeCommand(sql.c_str());

				_database.executeCommand("commit transaction");
			}
			catch(std::runtime_error& exception)
			{
				fprintf(stderr, "%s\n", exception.what());

				try
				{
					_database.executeCommand("rollback transaction");
				}
				catch(std::runtime_error& exception)
				{
					fprintf(stderr, "%s\n", exception.what());
					throw exception;
				}

				throw exception;
			}
		}

		void Database::renameCollection(const std::string& oldName, const std::string& newName) throw(std::runtime_error)
		{
			try
			{
				std::string sql("update Collection set name=\"");
				sql += newName;
				sql += "\" where id=(select id from Collection where name=\"";
				sql += oldName;
				sql += "\")";
				_database.executeCommand(sql.c_str());
			}
			catch(std::runtime_error& exception)
			{
				fprintf(stderr, "%s\n", exception.what());
				throw exception;
			}
		}

		void Database::addAsset(const std::string& collectionName, const Asset* const pAsset) throw(std::invalid_argument, std::runtime_error)
		{
			// TODO: Handle case sensitive & case insensitive

			if(pAsset == NULL)
			{
				std::invalid_argument exception("Database::AddAsset was passed a NULL pointer");
				throw exception;
			}

			try
			{
				// Check the collection exists - if it doesn't fail
				std::string collectionId = queryCollectionId(collectionName);

				// Check Asset already exists - if it doesn't create it
				bool assetInDatabase;
				std::string assetId;
				try
				{
					assetId = queryAssetId(pAsset->path(), pAsset->filename());
					assetInDatabase = true;
				}
				catch(std::runtime_error&)
				{
					assetInDatabase = false;
				}

				// Add new Asset and CollectionAsset to tables as required
				try
				{
					_database.executeCommand("begin transaction");

					if(!assetInDatabase)
					{
						// The Asset doesn't exist therefore insert it into Asset table
						insertAsset(pAsset);

						// Read back the asset ID generated by the database for this asset
						assetId = queryAssetId(pAsset->path(), pAsset->filename());
					}

					int		numRows		= 0,
							numColumns	= 0;
					char**	resultSet	= NULL;	

					std::string sql("select count(*) from CollectionAsset where collectionId=");
					sql += collectionId;
					sql += " and assetId=";
					sql += assetId;

					_database.executeQuery(	sql.c_str(),
											resultSet,
											numRows,
											numColumns);

					if(		(numColumns != 0)
						&&	(numRows != 0) )
					{
						int result = atoi(resultSet[1]);
						_database.freeResultSet(resultSet);

						if(result != 0)
						{
							std::runtime_error exception("Asset already in collection!");
							throw exception;
						}

						// Insert asset into CollectionAsset table
						insertCollectionAsset(collectionId, assetId, pAsset);
					}

					_database.executeCommand("commit transaction");
				}
				catch(std::runtime_error& exception)
				{
					fprintf(stderr, "%s\n", exception.what());

					try
					{
						_database.executeCommand("rollback transaction");
					}
					catch(std::runtime_error& exception)
					{
						fprintf(stderr, "%s\n", exception.what());
						throw exception;
					}

					throw exception;
				}
			}
			catch(std::runtime_error& exception)
			{
				fprintf(stderr, "%s\n", exception.what());
				throw exception;
			}
		}

		void Database::removeAsset(const std::string& collectionName, const Asset* const pAsset) throw(std::invalid_argument, std::runtime_error)
		{
			if(pAsset == NULL)
			{
				std::invalid_argument exception("Database::RemoveAsset was passed a NULL pointer");
				throw exception;
			}

			try
			{
				// Query database for collection ID and asset ID
				std::string collectionId	= queryCollectionId(collectionName);
				std::string assetId			= queryAssetId(pAsset->path(), pAsset->filename());

				_database.executeCommand("begin transaction");

				// Remove asset from CollectionAsset table
				deleteCollectionAsset(collectionId, assetId);

				// Find out if there are any more collections referring to this Asset
				int		numRows		= 0,
						numColumns	= 0;
				char**	resultSet	= NULL;	

				std::string sql("select count(*) from CollectionAsset where assetId=");
				sql += assetId;

				_database.executeQuery(	sql.c_str(),
										resultSet,
										numRows,
										numColumns);

				int collectionAssetCount = atoi(resultSet[1]);
				_database.freeResultSet(resultSet);

				//If there are no more rows in the CollectionAsset table referring to the specified asset, remove it from the Asset table
				if(collectionAssetCount == 0)
				{
					sql = "delete from Asset where id=";
					sql += assetId;

					_database.executeCommand(sql.c_str());
				}

				_database.executeCommand("commit transaction");
			}
			catch(std::runtime_error& exception)
			{
				fprintf(stderr, "%s\n", exception.what());

				try
				{
					_database.executeCommand("rollback transaction");
				}
				catch(std::runtime_error& exception)
				{
					fprintf(stderr, "%s\n", exception.what());
					throw exception;
				}

				throw exception;
			}
		}

		void Database::updateAsset(const std::string& collectionName, const Asset* const pAsset) throw(std::invalid_argument, std::runtime_error)
		{
			if(pAsset == NULL)
			{
				std::invalid_argument exception("Database::UpdateAsset was passed a NULL pointer");
				throw exception;
			}

			try
			{
				// Query database for collection ID and asset ID
				std::string collectionId	= queryCollectionId(collectionName);
				std::string assetId			= queryAssetId(pAsset->path(), pAsset->filename());

				const DesktopAsset* const pDesktopAsset = dynamic_cast<const DesktopAsset* const>(pAsset);
				if(pDesktopAsset != NULL)
				{
					char conversionBuffer[65];
					std::string sql("update CollectionAsset set name=\"");
					sql += pDesktopAsset->name();
					sql += "\", xPos=";
					sql += itoa(pDesktopAsset->xPosition(), conversionBuffer, 10);
					sql += ", yPos=";
					sql += itoa(pDesktopAsset->yPosition(), conversionBuffer, 10);
					sql += ", xSize=";
					sql += itoa(pDesktopAsset->xSize(), conversionBuffer, 10);
					sql += ", ySize=";
					sql += itoa(pDesktopAsset->ySize(), conversionBuffer, 10);
					sql += ", frameOffset=";
					sql += itoa(pDesktopAsset->frameOffset(), conversionBuffer, 10);
					sql += ", filmstrip=";
					sql += itoa((pDesktopAsset->filmstripMode() ? 1 : 0), conversionBuffer, 10);
					sql += " where collectionId=";
					sql += collectionId;
					sql += " and assetId=";
					sql += assetId;

					_database.executeCommand(sql.c_str());
				}
			}
			catch(std::runtime_error& exception)
			{
				fprintf(stderr, "%s\n", exception.what());
				throw exception;
			}
		}

		unsigned long Database::numAssets(const std::string& collectionName) throw(std::runtime_error)
		{
			int		numRows		= 0,
					numColumns	= 0;
			char**	resultSet	= NULL;	

			std::string sql("select count(*) from CollectionAsset where collectionId=(select id from Collection where name=\"");
			sql += collectionName;
			sql += "\")";

			_database.executeQuery(	sql.c_str(),
									resultSet,
									numRows,
									numColumns);

			if(		(numColumns == 0)
				||	(numRows == 0) )
			{
				return 0;
			}

			int result = atoi(resultSet[1]);
			_database.freeResultSet(resultSet);

			return result;
		}

		bool Database::operator==(const DataSource& rhs) const throw()
		{
			const Database* pDB = dynamic_cast<const Database*>(&rhs);
			if(pDB == NULL)
				return false;

			return ((_database == (pDB->_database)) ? true : false);
		}

		bool Database::operator!=(const DataSource& rhs) const throw()
		{
			return !operator==(rhs);
		}

		// Private member functions............................................................................................................
		void Database::testDatabase() throw(std::runtime_error)
		{
			try
			{
				fprintf(stderr, "Testing database...\n");

				// Query the Version table for database schema version
				int		numRows		= 0,
						numColumns	= 0;
				char**	resultSet	= NULL;
				char*	sql			= "select major, minor, revision from Version";

				_database.executeQuery(sql, resultSet, numRows, numColumns);

				if(		(numColumns != 3)
					||	(numRows != 1) )
				{
					// Returned table isn't valid
					_database.freeResultSet(resultSet);

					char columnsReceived[65];
					char rowsReceived[65];

					itoa(numColumns, columnsReceived, 10);
					itoa(numRows, rowsReceived, 10);

					std::string msg("Unexpected number of columns/rows returned from Version table: numColumns=");
					msg += columnsReceived;
					msg += ", numRows=";
					msg += rowsReceived;
					std::runtime_error exception(msg);
					throw exception;
				}

				// Check the version against what is current for this release
				char dbSchemaVersionMajor[65];
				char dbSchemaVersionMinor[65];
				char dbSchemaVersionRevision[65];

				strcpy(dbSchemaVersionMajor, resultSet[3]);
				strcpy(dbSchemaVersionMinor, resultSet[4]);
				strcpy(dbSchemaVersionRevision, resultSet[5]);

				_database.freeResultSet(resultSet);

				unsigned short dbVersionMajor		= (unsigned short)(atoi(dbSchemaVersionMajor));
				unsigned short dbVersionMinor		= (unsigned short)(atoi(dbSchemaVersionMinor));
				unsigned short dbVersionRevision	= (unsigned short)(atoi(dbSchemaVersionRevision));

				if	(	(dbVersionMajor		!= schemaVersionMajor)
					||	(dbVersionMinor		!= schemaVersionMinor)
					||	(dbVersionRevision	!= schemaVersionRevision) )
				{
					fprintf(stdout, 
							"Warning: database version: %s.%s.%s. Lastest verion: %d.%d.%d\n", 
							dbSchemaVersionMajor,
							dbSchemaVersionMinor,
							dbSchemaVersionRevision,
							schemaVersionMajor,
							schemaVersionMinor,
							schemaVersionRevision);

					// When the database tables change again this is where any intellegent database modifications need to go!!
				}

				// Check the rest of the tables are present and correct
				// Collection table
				numRows		= 0,
				numColumns	= 0;
				resultSet	= NULL;
				sql			= "select id, name from Collection";
				_database.executeQuery(sql, resultSet, numRows, numColumns);
				_database.freeResultSet(resultSet);

				// Asset table
				numRows		= 0,
				numColumns	= 0;
				resultSet	= NULL;
				sql			= "select id, path, filename, startFrame, endFrame from Asset";
				_database.executeQuery(sql, resultSet, numRows, numColumns);
				_database.freeResultSet(resultSet);

				// CollectionAsset table
				numRows		= 0,
				numColumns	= 0;
				resultSet	= NULL;
				sql			= "select id, collectionId, assetId, name, xPos, yPos, xSize, ySize, frameOffset, filmstrip from CollectionAsset";
				_database.executeQuery(sql, resultSet, numRows, numColumns);
				_database.freeResultSet(resultSet);

				fprintf(stderr, "Database tests complete\n");
			}
			catch(std::runtime_error& exception)
			{
				fprintf(stderr, "%s\n", exception.what());
				dropTables();
				createTables();
			}
		}

		void Database::dropTables() throw()
		{
			fprintf(stdout, "Dropping database tables...\n");

			// Old database schema
			try
			{
				_database.executeCommand("drop table desktop");
			}
			catch(std::runtime_error& exception)
			{
				fprintf(stdout, "%s\n", exception.what());
			}

			try
			{
				_database.executeCommand("drop table videodata");
			}
			catch(std::runtime_error& exception)
			{
				fprintf(stdout, "%s\n", exception.what());
			}

			// New database schema
			try
			{
				_database.executeCommand("drop table CollectionAsset");
			}
			catch(std::runtime_error& exception)
			{
				fprintf(stdout, "%s\n", exception.what());
			}

			try
			{
				_database.executeCommand("drop table Collection");
			}
			catch(std::runtime_error& exception)
			{
				fprintf(stdout, "%s\n", exception.what());
			}

			try
			{
				_database.executeCommand("drop table Asset");
			}
			catch(std::runtime_error& exception)
			{
				fprintf(stdout, "%s\n", exception.what());
			}

			try
			{
				_database.executeCommand("drop table Version");
			}
			catch(std::runtime_error& exception)
			{
				fprintf(stdout, "%s\n", exception.what());
			}

			fprintf(stdout, "All database tables dropped\n");
		}

		void Database::createTables() throw(std::runtime_error)
		{
			fprintf(stdout, "Creating database tables...\n");
			createVersionTable();
			createAssetTable();
			createCollectionTable();
			createCollectionAssetTable();
			fprintf(stdout, "All database tables created\n");
		}

		void Database::createVersionTable() throw(std::runtime_error)
		{
			try
			{
				fprintf(stdout, "Creating Version table...\n");
				_database.executeCommand("create table Version(major int not null, minor int, revision int)");
				fprintf(stdout, "Done\n");

				char major[65];
				char minor[65];
				char revision[65];
				itoa(schemaVersionMajor, major, 10);
				itoa(schemaVersionMinor, minor, 10);
				itoa(schemaVersionRevision, revision, 10);

				fprintf(stdout,
						"Inserting version information (%d.%d.%d) into Version table...\n",
						schemaVersionMajor,
						schemaVersionMinor,
						schemaVersionRevision);

				std::string sql("insert into Version values(");
				sql += major;
				sql += ",";
				sql += minor;
				sql += ",";
				sql += revision;
				sql += ")";

				_database.executeCommand(sql.c_str());

				fprintf(stdout, "Done\n");
			}
			catch(std::runtime_error& exception)
			{
				fprintf(stderr, "%s\n", exception.what());
				throw exception;
			}
		}

		void Database::createAssetTable() throw(std::runtime_error)
		{
			try
			{
				fprintf(stdout, "Creating Asset table...\n");
				_database.executeCommand("create table Asset(id integer not null primary key, path varchar(4096) not null, filename varchar(512) not null, startFrame int, endFrame int)");
				fprintf(stdout, "Done\n");
			}
			catch(std::runtime_error& exception)
			{
				fprintf(stderr, "%s\n", exception.what());
				throw exception;
			}
		}

		void Database::createCollectionTable() throw(std::runtime_error)
		{
			try
			{
				fprintf(stdout, "Creating Collection table...\n");
				_database.executeCommand("create table Collection(id integer not null primary key, name varchar(512) unique)");
				fprintf(stdout, "Done\n");
			}
			catch(std::runtime_error& exception)
			{
				fprintf(stderr, "%s\n", exception.what());
				throw exception;
			}
		}

		void Database::createCollectionAssetTable() throw(std::runtime_error)
		{
			try
			{
				fprintf(stdout, "Creating CollectionAsset table...\n");
				_database.executeCommand("create table CollectionAsset(id integer not null primary key, collectionId int not null, assetId int not null, name varchar(512), xPos int, yPos int, xSize int, ySize int, frameOffset int, filmstrip tinyint)");
				fprintf(stdout, "Done\n");
			}
			catch(std::runtime_error& exception)
			{
				fprintf(stderr, "%s\n", exception.what());
				throw exception;
			}
		}

		std::string Database::queryCollectionId(const std::string& collectionName) throw(std::runtime_error)
		{
			int		numRows		= 0,
					numColumns	= 0;
			char**	resultSet	= NULL;	

			std::string sql("select id from Collection where name=\"");
			sql += collectionName;
			sql += "\"";

			_database.executeQuery(	sql.c_str(),
									resultSet,
									numRows,
									numColumns);

			if(		(numColumns == 0)
				||	(numRows == 0) )
			{
				_database.freeResultSet(resultSet);

				std::string msg("No collection named \"");
				msg += collectionName;
				msg += "\"";

				std::runtime_error exception(msg.c_str());
				throw exception;
			}

			std::string ID(resultSet[1]);
			_database.freeResultSet(resultSet);
			return ID;
		}

		std::string Database::queryAssetId(const std::string& path, const std::string& filename) throw(std::runtime_error)
		{
			int		numRows		= 0,
					numColumns	= 0;
			char**	resultSet	= NULL;	

			std::string sql("select id from Asset where path=\"");
			sql += path;
			sql += "\" and filename=\"";
			sql += filename;
			sql += "\"";

			_database.executeQuery(	sql.c_str(),
									resultSet,
									numRows,
									numColumns);

			if(		(numColumns == 0)
				||	(numRows == 0) )
			{
				_database.freeResultSet(resultSet);

				std::string msg("Asset \"");
				msg += path;
				msg += filename;
				msg += "\" is not in the database";

				std::runtime_error exception(msg.c_str());
				throw exception;
			}

			std::string ID(resultSet[1]);
			_database.freeResultSet(resultSet);
			return ID;
		}

		void Database::insertAsset(const Asset* const pAsset) throw(std::runtime_error)
		{
			char stringConversionBuffer[100];

			std::string sql("insert into Asset (id, path, filename, startFrame, endFrame) values (NULL, \"");
			sql += pAsset->path();
			sql += "\", \"";
			sql += pAsset->filename();
			sql += "\", ";
			sql += itoa(pAsset->startFrame(), stringConversionBuffer, 10);
			sql += ", ";
			sql += itoa(pAsset->endFrame(), stringConversionBuffer, 10);
			sql += ")";

			_database.executeCommand(sql.c_str());
		}

		void Database::insertCollectionAsset(const std::string& collectionId, const std::string& assetId, const Asset* pAsset) throw(std::runtime_error)
		{
			std::string sql("insert into CollectionAsset (id, collectionId, assetId, name, xPos, yPos, xSize, ySize, frameOffset, filmstrip) values (NULL, ");
			sql += collectionId;
			sql += ", ";
			sql += assetId;

			const DesktopAsset* const pDesktopAsset = dynamic_cast<const DesktopAsset* const>(pAsset);
			if(pDesktopAsset != NULL)
			{
				char conversionBuffer[65];

				sql += ", \"";
				sql += pDesktopAsset->name();
				sql += "\", ";
				sql += itoa(pDesktopAsset->xPosition(), conversionBuffer, 10);
				sql += ", ";
				sql += itoa(pDesktopAsset->yPosition(), conversionBuffer, 10);
				sql += ", ";
				sql += itoa(pDesktopAsset->xSize(), conversionBuffer, 10);
				sql += ", ";
				sql += itoa(pDesktopAsset->ySize(), conversionBuffer, 10);
				sql += ", ";
				sql += itoa(pDesktopAsset->frameOffset(), conversionBuffer, 10);
				sql += ", ";
				sql += itoa((pDesktopAsset->filmstripMode() ? 1 : 0), conversionBuffer, 10);
			}
			else
			{
				sql += ", \"";
				sql += "";
				sql += "\", ";
				sql += "0";
				sql += ", ";
				sql += "0";
				sql += ", ";
				sql += "0";
				sql += ", ";
				sql += "0";
				sql += ", ";
				sql += "0";
				sql += ", ";
				sql += "0";
			}

			sql += ")";

			_database.executeCommand(sql.c_str());
		}

		void Database::deleteCollectionAsset(const std::string& collectionId, const std::string& assetId) throw(std::runtime_error)
		{
			std::string sql("delete from CollectionAsset where collectionId=");
			sql += collectionId;
			sql += " and assetId=";
			sql += assetId;

			_database.executeCommand(sql.c_str());
		}

		std::string Database::buildSQL(Query::Condition* pCondition) const throw(std::runtime_error)
		{
			std::string sql;

			if(		(pCondition->conditionPtr1() != NULL)
				&&	(pCondition->conditionPtr2() != NULL) )
			{
				sql += "(";
				sql += buildSQL(pCondition->conditionPtr1());
				sql += ")";
				sql += ((pCondition->logic() == Query::Condition::AND) ? " AND " : " OR ");
				sql += "(";
				sql += buildSQL(pCondition->conditionPtr2());
				sql += ")";
			}
			else
			{
				// -------------------------------------------------------------------
				// Search field
				// -------------------------------------------------------------------
				// "Collection Name"
				if(pCondition->field().compare(searchableField[0]) == 0)
				{
					sql += "id IN (SELECT assetId FROM CollectionAsset WHERE collectionId IN (SELECT id FROM Collection WHERE LOWER(name)";

					switch(pCondition->comparitor())
					{
					case Query::Condition::Contains:
						sql += " LIKE LOWER(\"%";
						sql += pCondition->value();
						sql += "%";
						break;
					case Query::Condition::DoesNotContain:
						sql += " NOT LIKE LOWER(\"%";
						sql += pCondition->value();
						sql += "%";
						break;
					case Query::Condition::EqualTo:
						sql += " = LOWER(\"";
						sql += pCondition->value();
						break;
					case Query::Condition::NotEqualTo:
						sql += " != LOWER(\"";
						sql += pCondition->value();
						break;
					case Query::Condition::GreaterThan:
						sql += " > LOWER(\"";
						sql += pCondition->value();
						break;
					case Query::Condition::GreaterThanOrEqualTo:
						sql += " >= LOWER(\"";
						sql += pCondition->value();
						break;
					case Query::Condition::LessThan:
						sql += " < LOWER(\"";
						sql += pCondition->value();
						break;
					case Query::Condition::LessThanOrEqualTo:
						sql += " <= LOWER(\"";
						sql += pCondition->value();
						break;
					default:
						{
							std::runtime_error exception("Error: Unknown query condition comparitor.");
							throw exception;
						}
					}

					sql += "\")))";
				}
				// "Path",
				else if(pCondition->field().compare(searchableField[1]) == 0)
				{
					sql += "LOWER(path)";

					switch(pCondition->comparitor())
					{
					case Query::Condition::Contains:
						sql += " LIKE LOWER(\"%";
						sql += pCondition->value();
						sql += "%";
						break;
					case Query::Condition::DoesNotContain:
						sql += " NOT LIKE LOWER(\"%";
						sql += pCondition->value();
						sql += "%";
						break;
					case Query::Condition::EqualTo:
						sql += " = LOWER(\"";
						sql += pCondition->value();
						break;
					case Query::Condition::NotEqualTo:
						sql += " != LOWER(\"";
						sql += pCondition->value();
						break;
					case Query::Condition::GreaterThan:
						sql += " > LOWER(\"";
						sql += pCondition->value();
						break;
					case Query::Condition::GreaterThanOrEqualTo:
						sql += " >= LOWER(\"";
						sql += pCondition->value();
						break;
					case Query::Condition::LessThan:
						sql += " < LOWER\"";
						sql += pCondition->value();
						break;
					case Query::Condition::LessThanOrEqualTo:
						sql += " <= LOWER(\"";
						sql += pCondition->value();
						break;
					default:
						{
							std::runtime_error exception("Error: Unknown query condition comparitor.");
							throw exception;
						}
					}

					sql += "\")";
				}
				// "Filename",
				else if(pCondition->field().compare(searchableField[2]) == 0)
				{
					sql += " LOWER(filename)";
					switch(pCondition->comparitor())
					{
					case Query::Condition::Contains:
						sql += " LIKE LOWER(\"%";
						sql += pCondition->value();
						sql += "%";
						break;
					case Query::Condition::DoesNotContain:
						sql += " NOT LIKE LOWER(\"%";
						sql += pCondition->value();
						sql += "%";
						break;
					case Query::Condition::EqualTo:
						sql += " = LOWER(\"";
						sql += pCondition->value();
						break;
					case Query::Condition::NotEqualTo:
						sql += " != LOWER(\"";
						sql += pCondition->value();
						break;
					case Query::Condition::GreaterThan:
						sql += " > LOWER(\"";
						sql += pCondition->value();
						break;
					case Query::Condition::GreaterThanOrEqualTo:
						sql += " >= LOWER(\"";
						sql += pCondition->value();
						break;
					case Query::Condition::LessThan:
						sql += " < LOWER(\"";
						sql += pCondition->value();
						break;
					case Query::Condition::LessThanOrEqualTo:
						sql += " <= LOWER(\"";
						sql += pCondition->value();
						break;
					default:
						{
							std::runtime_error exception("Error: Unknown query condition comparitor.");
							throw exception;
						}
					}

					sql += "\")";
				}
				// "Length",
				else if(pCondition->field().compare(searchableField[3]) == 0)
				{
					switch(pCondition->comparitor())
					{
					case Query::Condition::Contains:
						sql += " ";
						break;
					case Query::Condition::DoesNotContain:
						sql += " ";
						break;
					case Query::Condition::EqualTo:
						sql += " =";
						break;
					case Query::Condition::NotEqualTo:
						sql += " !=";
						break;
					case Query::Condition::GreaterThan:
						sql += " >";
						break;
					case Query::Condition::GreaterThanOrEqualTo:
						sql += " >=";
						break;
					case Query::Condition::LessThan:
						sql += " <";
						break;
					case Query::Condition::LessThanOrEqualTo:
						sql += " <=";
						break;
					default:
						{
							std::runtime_error exception("Error: Unknown query condition comparitor.");
							throw exception;
						}
					}
				}
				// "Type",
				else if(pCondition->field().compare(searchableField[4]) == 0)
				{
					switch(pCondition->comparitor())
					{
					case Query::Condition::Contains:
						sql += " ";
						break;
					case Query::Condition::DoesNotContain:
						sql += " ";
						break;
					case Query::Condition::EqualTo:
						sql += " =";
						break;
					case Query::Condition::NotEqualTo:
						sql += " !=";
						break;
					case Query::Condition::GreaterThan:
						sql += " >";
						break;
					case Query::Condition::GreaterThanOrEqualTo:
						sql += " >=";
						break;
					case Query::Condition::LessThan:
						sql += " <";
						break;
					case Query::Condition::LessThanOrEqualTo:
						sql += " <=";
						break;
					default:
						{
							std::runtime_error exception("Error: Unknown query condition comparitor.");
							throw exception;
						}
					}
				}
				// "Date Added"
				else if(pCondition->field().compare(searchableField[5]) == 0)
				{
					switch(pCondition->comparitor())
					{
					case Query::Condition::Contains:
						sql += " ";
						break;
					case Query::Condition::DoesNotContain:
						sql += " ";
						break;
					case Query::Condition::EqualTo:
						sql += " =";
						break;
					case Query::Condition::NotEqualTo:
						sql += " !=";
						break;
					case Query::Condition::GreaterThan:
						sql += " >";
						break;
					case Query::Condition::GreaterThanOrEqualTo:
						sql += " >=";
						break;
					case Query::Condition::LessThan:
						sql += " <";
						break;
					case Query::Condition::LessThanOrEqualTo:
						sql += " <=";
						break;
					default:
						{
							std::runtime_error exception("Error: Unknown query condition comparitor.");
							throw exception;
						}
					}
				}
			}

			return sql;
		}
	}
}

