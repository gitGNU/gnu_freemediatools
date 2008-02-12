	
// il - A asset library representation.

// Copyright (C) 2000-2006 VM Inc.
// Released under the LGPL.
// For more information, see http://www.openassetlib.org

#pragma once
#include <string>
#include <stdexcept>

#if defined(_WIN32)
#pragma warning( disable : 4290 )
#pragma warning( push )
#pragma warning( disable : 4702 )
#endif

#include <list>

#if defined(_WIN32)
#pragma warning( pop )
#endif

namespace olib
{
	namespace openassetlib
	{
		class Asset;
		class DataSource
		{
		public:
			enum Type
			{
				LocalDatabase,
				Peer,
				VMFXNet,
				WorkGroupServer,
			};

			class Query
			{
			public:
				class Condition
				{
				public:
					enum Comparitor
					{
						EqualTo,
						NotEqualTo,
						GreaterThan,
						GreaterThanOrEqualTo,
						LessThan,
						LessThanOrEqualTo,
						Contains,
						DoesNotContain,
					};

					enum Logic
					{
						AND,
						OR,
					};

					Condition(std::string field, std::string value, Comparitor comparitor)	throw();
					Condition(Condition* pCondition1, Condition* pCondition2, Logic logic)	throw(std::invalid_argument);
					~Condition(void)														throw();

					std::string		field()			const throw();
					std::string		value()			const throw();
					Comparitor		comparitor()	const throw();
					Condition*		conditionPtr1()	const throw();
					Condition*		conditionPtr2()	const throw();
					Logic			logic()			const throw();

				private:
					std::string		_field;
					std::string		_value;
					Comparitor		_comparitor;
					Condition*		_pCondition1;
					Condition*		_pCondition2;
					Logic			_logic;
				};

				static const unsigned char maxHitsDefault;

				Query(Condition* pCondition)							throw(std::invalid_argument);
				Query(Condition* pCondition, unsigned short maxHits)	throw(std::invalid_argument);
				~Query(void)											throw();

				unsigned short	maxHits(void)					const	throw();
				void			maxHits(unsigned short maxHits)			throw();

				Condition*		condition(void)					const	throw();

			private:
				unsigned short			_maxHits;
				Condition*				_pCondition;
			};

			DataSource(const std::string& name)	throw();
			virtual ~DataSource(void)			throw();

			virtual std::string	name() const throw()	= 0;
			virtual Type		type() const throw()	= 0;

			virtual bool	isConnected() const throw() = 0;

			virtual void	connect(std::string url)	throw(std::runtime_error)	= 0;
			virtual void	disconnect()				throw()						= 0;

			virtual std::list<std::string>	collectionNames()			const throw()	= 0;
			virtual std::list<std::string>	searchableFields()			const throw()	= 0;
			virtual std::list<Asset*>		search(const Query& query)	const throw()	= 0;

			virtual void	createCollection(const std::string& name)									throw(std::runtime_error)	= 0;
			virtual void	deleteCollection(const std::string& name)									throw(std::runtime_error)	= 0;
			virtual void	renameCollection(const std::string& oldName, const std::string& newName)	throw(std::runtime_error)	= 0;

			virtual void			addAsset(const std::string& collectionName, const Asset* const pAsset)		throw(std::runtime_error, std::invalid_argument)	= 0;
			virtual void			removeAsset(const std::string& collectionName, const Asset* const pAsset)	throw(std::runtime_error, std::invalid_argument)	= 0;
			virtual void			updateAsset(const std::string& collectionName, const Asset* const pAsset)	throw(std::runtime_error, std::invalid_argument)	= 0;
			virtual unsigned long	numAssets(const std::string& collectionName)								throw(std::runtime_error)							= 0;

			virtual bool	operator==(const DataSource& rhs)	const throw() = 0;
			virtual bool	operator!=(const DataSource& rhs)	const throw() = 0;

		protected:
			std::string		_name;
		};
	}
}
