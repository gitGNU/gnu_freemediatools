/*******************************************************************************
**
** The header file for the asset table interface and singleton which tracks the selected asset
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef JAH_ASSET_TABLE
#define JAH_ASSET_TABLE

#include "assetdata.h"

class AssetTable
{
	public:
  virtual ~AssetTable( );
  virtual void selectAsset( assetData & ) = 0;
};

class AssetTables
{
 private:
  AssetTables( ) ;
 public:
  static AssetTables *getInstance( );
  void registerTable( AssetTable *table ) ;
  assetData &getSelectedAsset( ) ;
  void setSelectedAsset( assetData &asset );
 private:
  std::vector< AssetTable * > m_tables;
  assetData m_current_asset;
};

#endif

