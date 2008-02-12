	
// il - A asset library representation.

// Copyright (C) 2000-2006 VM Inc.
// Released under the LGPL.
// For more information, see http://www.openassetlib.org

#include "AssetManager.h"
#include "DataSource.h"
#include "Database.h"

namespace olib
{
	namespace openassetlib
	{
		//---------------------------------------------------------------
		// This destroyer class ensures that the AssetManagers destructor 
		// is called correctly and any resources are correctly released
		class AssetManager::AssetManagerDestroyer
		{
		public:
			AssetManagerDestroyer() throw()
				: _pAssetManager(NULL)
			{
			};

			~AssetManagerDestroyer() throw()
			{
				delete _pAssetManager;
			};

			void initialize(AssetManager* pAssetManager) throw()
			{
				_pAssetManager = pAssetManager;
			};

		private:
			AssetManager* _pAssetManager;
		};

		//---------------------------------------------------------------
		AssetManager::AssetManagerDestroyer	AssetManager::_destroyer;
		AssetManager*						AssetManager::_pInstance = NULL;

		AssetManager& AssetManager::instance() throw(std::bad_alloc)
		{
			if(_pInstance == NULL)
			{
				_pInstance = new AssetManager();
				_destroyer.initialize(_pInstance);
			}

			return *_pInstance;
		}

		AssetManager::AssetManager(void) throw()
		{
		}

		AssetManager::~AssetManager(void) throw()
		{
			while(!_dataSourceList.empty())
			{
				try
				{
					deleteDataSource(*(_dataSourceList.begin()));
				}
				catch(...)
				{
				}
			}
		}

		DataSource* AssetManager::createDataSource(DataSource::Type dataSourceType, const std::string& dataSourceName) throw(std::invalid_argument, std::bad_alloc)
		{
			DataSource* pDataSource = NULL;
			switch(dataSourceType)
			{
			case DataSource::LocalDatabase:
				pDataSource = new Database(dataSourceName);
				break;
			case DataSource::Peer:
//				pDataSource = new ?;
				break;
			case DataSource::VMFXNet:
//				pDataSource = new ?;
				break;
			case DataSource::WorkGroupServer:
//				pDataSource = new ?;
				break;
			default:
				{
					std::invalid_argument exception("AssetManager::AddDataSource was passed an unknown datasource type");
					throw exception;
				}
			}

			if(pDataSource != NULL)
				_dataSourceList.push_back(pDataSource);

			return pDataSource;
		}

		void AssetManager::deleteDataSource(DataSource* pDataSource) throw(std::invalid_argument)
		{
			if(pDataSource == NULL)
			{
				std::invalid_argument exception("AssetManager::DeleteDataSource was passed a NULL pointer");
				throw exception;
			}

			_dataSourceList.remove(pDataSource);
		}

		bool AssetManager::dataSourceListEmpty() const throw()
		{
			return _dataSourceList.empty();
		}

		std::list<DataSource*>::size_type AssetManager::dataSourceListSize() const throw()
		{
			return _dataSourceList.size();
		}

		std::list<DataSource*>::const_iterator AssetManager::dataSourceListBeginIterator() const throw()
		{
			return _dataSourceList.begin();
		}

		std::list<DataSource*>::const_iterator AssetManager::dataSourceListEndIterator() const throw()
		{
			return _dataSourceList.end();
		}
	}
}
