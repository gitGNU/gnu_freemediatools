	
// il - A asset library representation.

// Copyright (C) 2000-2006 VM Inc.
// Released under the LGPL.
// For more information, see http://www.openassetlib.org

#pragma once
#include "DataSource.h"

namespace olib
{
	namespace openassetlib
	{
		class AssetManager
		{
		public:
			static AssetManager& instance() throw(std::bad_alloc);

			~AssetManager(void)	throw();

			DataSource*	createDataSource(DataSource::Type type, const std::string& dataSourceName)	throw(std::invalid_argument, std::bad_alloc);
			void		deleteDataSource(DataSource* pDataSource)									throw(std::invalid_argument);

			bool									dataSourceListEmpty()			const throw();
			std::list<DataSource*>::size_type		dataSourceListSize()			const throw();
			std::list<DataSource*>::const_iterator	dataSourceListBeginIterator()	const throw();
			std::list<DataSource*>::const_iterator	dataSourceListEndIterator()		const throw();

		private:
			AssetManager(void) throw();

			class  AssetManagerDestroyer;
			static AssetManagerDestroyer	_destroyer;
			static AssetManager*			_pInstance;
			std::list<DataSource*>			_dataSourceList;
		};
	}
}
