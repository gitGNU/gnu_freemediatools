	
// il - A asset library representation.

// Copyright (C) 2005 Visual Media FX Ltd.
// Released under the LGPL.
// For more information, see http://www.openassetlib.org

// testClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DataSource.h"
#include "AssetManager.h"
#include "DesktopAsset.h"

namespace VMFX
{
	namespace OpenAssetLib
	{
		namespace Test
		{
			class DataSourceTest
			{
			public:
				DataSourceTest()	{};
				~DataSourceTest()	{};

				bool Run()
				{
					OutputDebugString("Testing DataSource...\n");

					DataSource* pDataSource = NULL;
					std::string dataSourceName = "DataSource for DataSourceTest";
					if(CreateDataSource(pDataSource, dataSourceName) == false)
						return false;
					
					if(pDataSource->name().compare(dataSourceName) != 0)
					{
						OutputDebugString("Unexpected data source name\n");
						return false;
					}

					if(pDataSource->type() != DataSource::Type::LocalDatabase)
					{
						OutputDebugString("Unexpected data source type\n");
						return false;
					}

					if(TestConnect(pDataSource, "file://C:/Jahshaka/Database/jahshaka.db") == false)
						return false;

					if(TestDataSourceCollectionCreation(pDataSource) == false)
						return false;

					if(TestDataSourceCollectionRename(pDataSource) == false)
						return false;

					const unsigned char numAssets = 5;
					DesktopAsset* pDesktopAsset[numAssets];
					if(TestAddingAssetToCollection(pDataSource, pDesktopAsset, numAssets) == false)
						return false;

					if(TestUpdatingAssetInCollection(pDataSource, pDesktopAsset, numAssets) == false)
						return false;

					if(TestAssetSearch(pDataSource, numAssets) == false)
						return false;

					if(TestRemovingAssetFromCollection(pDataSource, pDesktopAsset, numAssets) == false)
						return false;

					if(TestDisconnect(pDataSource) == false)
						return false;

					if(DeleteDataSource(pDataSource) == false)
						return false;

					OutputDebugString("DataSource tests complete.\n");
					return true;
				};
			
			private:
				bool CreateDataSource(DataSource*& pDataSource, const std::string& dataSourceName)
				{
					try
					{
						pDataSource = AssetManager::instance().createDataSource(DataSource::Type::LocalDatabase, dataSourceName);
						return true;
					}
					catch(std::runtime_error& exception)
					{
						std::string msg("Failed to create local database data source: ");
						msg += exception.what();
						msg += "\n";
						OutputDebugString(msg.c_str());
						_asm int 3;
						return false;
					}
				};
				bool DeleteDataSource(DataSource*& pDataSource)
				{
					try
					{
						AssetManager::instance().deleteDataSource(pDataSource);
					}
					catch(std::runtime_error& exception)
					{
						std::string msg("Failed to delete local database data source: ");
						msg += exception.what();
						msg += "\n";
						OutputDebugString(msg.c_str());
						_asm int 3;
						return false;
					}

					return true;
				};
				bool TestConnect(DataSource* pDataSource, std::string databaseURL)
				{
					if(pDataSource->isConnected() != false)
					{
						OutputDebugString("Unexpected connection initial state\n");
						_asm int 3;
						return false;
					}

					try
					{
						pDataSource->connect(databaseURL);
					}
					catch(std::runtime_error& exception)
					{
						std::string msg("Failed to open/create local database data source: ");
						msg += exception.what();
						msg += "\n";
						OutputDebugString(msg.c_str());
						_asm int 3;
						return false;
					}

					if(pDataSource->isConnected() != true)
					{
						OutputDebugString("Unexpected connection state after connect\n");
						_asm int 3;
						return false;
					}

					return true;
				};

				bool TestDisconnect(DataSource* pDataSource)
				{
					try
					{
						pDataSource->disconnect();
					}
					catch(std::runtime_error& exception)
					{
						std::string msg("Failed to disconnect local database data source: ");
						msg += exception.what();
						msg += "\n";
						OutputDebugString(msg.c_str());
						_asm int 3;
						return false;
					}

					if(pDataSource->isConnected() != false)
					{
						OutputDebugString("Unexpected connection state after disconnect\n");
						_asm int 3;
						return false;
					}

					return true;
				};
				bool TestDataSourceCollectionCreation(DataSource* pDataSource)
				{
					std::list<std::string> collectionNameList = pDataSource->collectionNames();
					if(collectionNameList.empty() != true)
					{
						OutputDebugString("Database Collection table not empty\n");
						_asm int 3;
						return false;
					}

					try
					{
						const unsigned char numCollections = 5;
						std::string collectionName("myCollection");
						for(unsigned char idx = 0; idx < numCollections; idx++)
						{
							char Buffer[10];
							pDataSource->createCollection(collectionName + itoa(idx + 1, Buffer, 10));

							collectionNameList = pDataSource->collectionNames();
							if(unsigned char(collectionNameList.size()) != (idx + 1))
							{
								OutputDebugString("Unexpected number of collections\n");
								_asm int 3;
								return false;
							}
						}
					}
					catch(std::runtime_error& exception)
					{
						std::string msg("Failed to create collection in local database data source: ");
						msg += exception.what();
						msg += "\n";
						OutputDebugString(msg.c_str());
						_asm int 3;
						return false;
					}
					return true;
				};
				bool TestDataSourceCollectionRename(DataSource* pDataSource)
				{
					try
					{
						pDataSource->renameCollection("myCollection5", "myCollection500");

						std::list<std::string> collectionNameList = pDataSource->collectionNames();
						bool found = false;
						if(!collectionNameList.empty())
						{
							std::list<std::string>::const_iterator it = collectionNameList.begin();
							do
							{
								if((*it).compare(std::string("myCollection500")) == 0)
								{
									found = true;
									break;
								}

							} while(++it != collectionNameList.end());
						}

						if(found == false)
						{
							OutputDebugString("Failed to change collection name\n");
							_asm int 3;
							return false;
						}
					}
					catch(std::runtime_error& exception)
					{
						std::string msg("Failed to rename collection in local database data source: ");
						msg += exception.what();
						msg += "\n";
						OutputDebugString(msg.c_str());
						_asm int 3;
						return false;
					}

					return true;
				};
				bool TestDataSourceCollectionDeletion(DataSource* pDataSource)
				{
					try
					{
						std::list<std::string> collectionNameList = pDataSource->collectionNames();
						if(!collectionNameList.empty())
						{
							std::list<std::string>::const_iterator it = collectionNameList.begin();
							do
							{
								pDataSource->deleteCollection(*it);

							} while(++it != collectionNameList.end());

							collectionNameList.clear();
						}
					}
					catch(std::runtime_error& exception)
					{
						std::string msg("Failed to delete collection in local database data source: ");
						msg += exception.what();
						msg += "\n";
						OutputDebugString(msg.c_str());
						_asm int 3;
						return false;
					}
					return true;
				};
				bool TestAddingAssetToCollection(DataSource* pDataSource, DesktopAsset* pDesktopAsset[], unsigned char numAssets)
				{
					try
					{
						for(unsigned char idx = 0; idx < numAssets; idx++)
						{
							char Buffer[10];
							pDesktopAsset[idx] = new DesktopAsset(	std::string("C:/Jahshaka/Media/"), 
																	std::string("name") + itoa(idx + 1, Buffer, 10) + std::string(".extension"), 
																	0, 
																	1000, 
																	std::string("myDesktopAsset") + itoa(idx + 1, Buffer, 10), 
																	111, 
																	222, 
																	333, 
																	444, 
																	555, 
																	true);
							pDataSource->addAsset(std::string("myCollection500"), pDesktopAsset[idx]);

							std::list<std::string> searchableFields = pDataSource->searchableFields();
							if(!searchableFields.empty())
							{
								std::list<std::string>::const_iterator it = searchableFields.begin();
								do
								{
									OutputDebugString((*it).c_str());
									OutputDebugString("\n");

								} while(++it != searchableFields.end());
							}
						}

						if(pDataSource->numAssets("myCollection500") != numAssets)
						{
							_asm int 3;
							return false;
						}
					}
					catch(std::invalid_argument& exception)
					{
						std::string msg("Failed to add asset to collection in local database data source: ");
						msg += exception.what();
						msg += "\n";
						OutputDebugString(msg.c_str());
						_asm int 3;
						return false;
					}
					catch(std::runtime_error& exception)
					{
						std::string msg("Failed to add asset to collection in local database data source: ");
						msg += exception.what();
						msg += "\n";
						OutputDebugString(msg.c_str());
						_asm int 3;
						return false;
					}
					return true;
				};

				bool TestRemovingAssetFromCollection(DataSource* pDataSource, DesktopAsset* pDesktopAsset[], unsigned char numAssets)
				{
					try
					{
						pDataSource->removeAsset(std::string("myCollection500"), pDesktopAsset[2]);
						pDesktopAsset[2] = NULL;
					}
					catch(std::invalid_argument& exception)
					{
						std::string msg("Failed to remove asset from collection in local database data source: ");
						msg += exception.what();
						msg += "\n";
						OutputDebugString(msg.c_str());
						_asm int 3;
						return false;
					}
					catch(std::runtime_error& exception)
					{
						std::string msg("Failed to remove asset from collection in local database data source: ");
						msg += exception.what();
						msg += "\n";
						OutputDebugString(msg.c_str());
						_asm int 3;
						return false;
					}
					return true;
				};
				bool TestUpdatingAssetInCollection(DataSource* pDataSource, DesktopAsset* pDesktopAsset[], unsigned char numAssets)
				{
					try
					{
						DesktopAsset* pda = new DesktopAsset(pDesktopAsset[0]->path(), pDesktopAsset[0]->filename(), 15, 32, "another name", 10, 20, 30, 40, 50, false);
						delete pDesktopAsset[0];
						pDesktopAsset[0] = pda;
						pDataSource->updateAsset(std::string("myCollection500"), pDesktopAsset[0]);
					}
					catch(std::invalid_argument& exception)
					{
						std::string msg("Failed to add asset to collection in local database data source: ");
						msg += exception.what();
						msg += "\n";
						OutputDebugString(msg.c_str());
						_asm int 3;
						return false;
					}
					catch(std::runtime_error& exception)
					{
						std::string msg("Failed to add asset to collection in local database data source: ");
						msg += exception.what();
						msg += "\n";
						OutputDebugString(msg.c_str());
						_asm int 3;
						return false;
					}
					return true;
				};
				bool TestAssetSearch(DataSource* pDataSource, unsigned char numAssets)
				{
					DataSource::Query::Condition* pCondition1 = NULL;
					DataSource::Query::Condition* pCondition2 = NULL;
					DataSource::Query::Condition* pCondition3 = NULL;
					DataSource::Query* pQuery = NULL;
					std::list<Asset*> assetHitList;

					pCondition1 = new DataSource::Query::Condition("Collection Name", "5", DataSource::Query::Condition::Comparitor::Contains);
					pQuery = new DataSource::Query(pCondition1);
					assetHitList = pDataSource->search(*pQuery);
					if(assetHitList.size() != numAssets)
					{
						_asm int 3;
						return false;
					}
					delete pQuery;
					delete pCondition1;

					assetHitList.clear(); // contents should be deleted!
					pCondition1 = new DataSource::Query::Condition("Path", "c:/jahshaka/media/", DataSource::Query::Condition::Comparitor::EqualTo);
					pQuery = new DataSource::Query(pCondition1);
					assetHitList = pDataSource->search(*pQuery);
					if(assetHitList.size() != numAssets)
					{
						_asm int 3;
						return false;
					}
					delete pQuery;
					delete pCondition1;

					assetHitList.clear(); // contents should be deleted!
					pCondition1 = new DataSource::Query::Condition("Filename", "3.", DataSource::Query::Condition::Comparitor::Contains);
					pQuery = new DataSource::Query(pCondition1);
					assetHitList = pDataSource->search(*pQuery);
					if(assetHitList.size() != 1)
					{
						_asm int 3;
						return false;
					}
					delete pQuery;
					delete pCondition1;

					assetHitList.clear(); // contents should be deleted!
					pCondition1 = new DataSource::Query::Condition("Path", "C:/Jahshaka/Media/", DataSource::Query::Condition::Comparitor::EqualTo);
					pCondition2 = new DataSource::Query::Condition("Filename", "3.", DataSource::Query::Condition::Comparitor::Contains);
					pCondition3 = new DataSource::Query::Condition(pCondition1, pCondition2, DataSource::Query::Condition::Logic::AND);
					pQuery = new DataSource::Query(pCondition3);
					assetHitList = pDataSource->search(*pQuery);
					if(assetHitList.size() != 1)
					{
						_asm int 3;
						return false;
					}
					delete pQuery;
					delete pCondition1;
					delete pCondition2;
					delete pCondition3;

					return true;
				};
			};

			class AssetManagerTest
			{
			public:
				AssetManagerTest()	{};
				~AssetManagerTest()	{};

				bool Run()
				{
					OutputDebugString("Testing AssetManager...\n");

					const unsigned char numDataSources = 1; // 4;
					DataSource* pDataSource[numDataSources] = {NULL};

					if(TestDataSourceCreation(pDataSource, numDataSources) == false)
						return false;

					if(TestDataSourceDeletion(pDataSource, numDataSources) == false)
						return false;

					OutputDebugString("AssetManager tests complete.\n");
					return true;
				};

			private:
				bool TestDataSourceCreation(DataSource* pDataSource[], unsigned char numDataSources)
				{
					for(unsigned char idx = 0; idx < numDataSources; idx++)
					{
						if(		(AssetManager::instance().dataSourceListEmpty() == false)
							||	(AssetManager::instance().dataSourceListSize() != 0)
							||	(AssetManager::instance().dataSourceListBeginIterator() != AssetManager::instance().dataSourceListEndIterator()) )
						{
							OutputDebugString("Incorrect initial state of DataSourceList\n");
							_asm int 3;
							return false;
						}

						try
						{
							pDataSource[idx] = AssetManager::instance().createDataSource((DataSource::Type)idx, "DataSource for AssetManagerTest");

							if(		(AssetManager::instance().dataSourceListEmpty() == true)
								||	(unsigned char(AssetManager::instance().dataSourceListSize()) != (idx + 1)) )
							{
								OutputDebugString("Incorrect state of DataSourceList\n");
								_asm int 3;
								return false;
							}
						}
						catch(std::runtime_error& exception)
						{
							std::string msg("Failed to create data source: ");
							msg += exception.what();
							msg += "\n";
							OutputDebugString(msg.c_str());
							_asm int 3;
							return false;
						}
					}

					return true;
				};

				bool TestDataSourceDeletion(DataSource* pDataSource[], unsigned char numDataSources)
				{
					for(unsigned char idx = 0; idx < numDataSources; idx++)
					{
						try
						{
							if(		(AssetManager::instance().dataSourceListEmpty() == true)
								||	(unsigned char(AssetManager::instance().dataSourceListSize()) != (numDataSources - idx)) )
							{
								OutputDebugString("Incorrect state of DataSourceList\n");
								_asm int 3;
								return false;
							}

							AssetManager::instance().deleteDataSource((DataSource*)pDataSource[idx]);
						}
						catch(std::runtime_error& exception)
						{
							std::string msg("Failed to create local database data source: ");
							msg += exception.what();
							msg += "\n";
							OutputDebugString(msg.c_str());
							_asm int 3;
							return false;
						}
					}

					if(		(AssetManager::instance().dataSourceListEmpty() == false)
						||	(AssetManager::instance().dataSourceListSize() != 0)
						||	(AssetManager::instance().dataSourceListBeginIterator() != AssetManager::instance().dataSourceListEndIterator()) )
					{
						OutputDebugString("Incorrect initial state of DataSourceList\n");
						_asm int 3;
						return false;
					}

					return true;
				};
			};

			class OpenAssetLibTest
			{
			public:
				OpenAssetLibTest()	{};
				~OpenAssetLibTest()	{};

				bool Run()
				{
					try
					{
						AssetManagerTest assetManagerTest;
						if(assetManagerTest.Run() == false)
							return false;

						DataSourceTest dataSourceTest;
						if(dataSourceTest.Run() == false)
							return false;

						return true;
					}
					catch(std::runtime_error& exception)
					{
						OutputDebugString(exception.what());
						_asm int 3;
						return false;
					}
				};
			};
		}
	}
}

using namespace VMFX::OpenAssetLib;

int _tmain(int argc, _TCHAR* argv[])
{
	remove("C:/Jahshaka/Database/jahshaka.db");
	Test::OpenAssetLibTest test;
	if(test.Run() == false)
		_asm int 3;
	remove("C:/Jahshaka/Database/jahshaka.db");
	return 0;
}

