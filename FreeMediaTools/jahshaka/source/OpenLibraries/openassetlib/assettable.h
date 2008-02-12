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
		virtual ~AssetTable( ) { }
		virtual void selectAsset( assetData & ) = 0;
};

class AssetTables
{
	private:
		AssetTables( ) 
		{ 
		}

	public:
		static AssetTables *getInstance( )
		{
			static AssetTables *tables = NULL;
			if ( tables == NULL )
				tables = new AssetTables( );
			return tables;
		}

		void registerTable( AssetTable *table ) 
		{ 
			m_tables.push_back( table ); 
		}

		assetData &getSelectedAsset( ) 
		{
			return m_current_asset;
		}

		void setSelectedAsset( assetData &asset )
		{
			m_current_asset = asset;
			for ( std::vector< AssetTable * >::iterator iter = m_tables.begin( ); iter != m_tables.end( ); iter ++ )
				( *iter )->selectAsset( asset );
		}

	private:
		std::vector< AssetTable * > m_tables;
		assetData m_current_asset;
};

#endif

