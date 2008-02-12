	
// il - A asset library representation.

// Copyright (C) 2000-2006 VM Inc.
// Released under the LGPL.

// For more information, see http://www.openassetlib.org
#pragma once
#include <string>
#include "DataSource.h"

struct sqlite3;

namespace olib
{
	namespace openassetlib
	{
		class Database : public DataSource
		{
		public:
			static const unsigned short schemaVersionMajor;
			static const unsigned short schemaVersionMinor;
			static const unsigned short schemaVersionRevision;

			static const std::string	searchableField[];
			static const unsigned char	numSearchableFields;

			Database(const std::string& name)	throw();
			~Database(void)						throw();

			std::string			name() const throw();
			DataSource::Type	type() const throw();

			bool	isConnected() const	throw();

			void	connect(std::string url)	throw(std::runtime_error);
			void	disconnect()				throw();

			std::list<std::string>	collectionNames()			const throw();
			std::list<std::string>	searchableFields()			const throw();
			std::list<Asset*>		search(const Query& query)	const throw();

			void	createCollection(const std::string& name)									throw(std::runtime_error);
			void	deleteCollection(const std::string& name)									throw(std::runtime_error);
			void	renameCollection(const std::string& oldName, const std::string& newName)	throw(std::runtime_error);

			void			addAsset(const std::string& collectionName, const Asset* const pAsset)		throw(std::invalid_argument, std::runtime_error);
			void			removeAsset(const std::string& collectionName, const Asset* const pAsset)	throw(std::invalid_argument, std::runtime_error);
			void			updateAsset(const std::string& collectionName, const Asset* const pAsset)	throw(std::invalid_argument, std::runtime_error);
			unsigned long	numAssets(const std::string& collectionName)								throw(std::runtime_error);

			bool	operator==(const DataSource& rhs) const throw();
			bool	operator!=(const DataSource& rhs) const throw();

		private:
			void	testDatabase()					throw(std::runtime_error);
			void	dropTables()					throw();
			void	createTables()					throw(std::runtime_error);
			void	createVersionTable()			throw(std::runtime_error);
			void	createAssetTable()				throw(std::runtime_error);
			void	createCollectionTable()			throw(std::runtime_error);
			void	createCollectionAssetTable()	throw(std::runtime_error);

			std::string		queryCollectionId(const std::string& collectionName)				throw(std::runtime_error);
			std::string		queryAssetId(const std::string& path, const std::string& filename)	throw(std::runtime_error);

			void	insertAsset(const Asset* const pAsset)																			throw(std::runtime_error);
			void	insertCollectionAsset(const std::string& collectionId, const std::string& assetId, const Asset* const pAsset)	throw(std::runtime_error);
			void	deleteCollectionAsset(const std::string& collectionId, const std::string& assetId)								throw(std::runtime_error);

			std::string	buildSQL(DataSource::Query::Condition* pCondition) const throw(std::runtime_error);

			class SQLiteDatabase;
			static SQLiteDatabase	_database;
			bool					_connected;
		};
	}
}
