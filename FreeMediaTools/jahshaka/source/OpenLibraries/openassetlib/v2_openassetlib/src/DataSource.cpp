	
// il - A asset library representation.

// Copyright (C) 2000-2006 VM Inc.
// Released under the LGPL.
// For more information, see http://www.openassetlib.org

#include "DataSource.h"

namespace olib
{
	namespace openassetlib
	{
		DataSource::Query::Condition::Condition(std::string field, std::string value, Comparitor comparitor) throw()
			:	_field(field),
				_value(value),
				_comparitor(comparitor),
				_pCondition1(NULL),
				_pCondition2(NULL),
				_logic(AND)
		{
		}

		DataSource::Query::Condition::Condition(Condition* pCondition1, Condition* pCondition2, Logic logic) throw(std::invalid_argument)
			:	_field(""),
				_value(""),
				_comparitor(Contains),
				_pCondition1(pCondition1),
				_pCondition2(pCondition2),
				_logic(logic)
		{
			if(		(pCondition1 == NULL)
				||	(pCondition2 == NULL) )
			{
				std::invalid_argument exception("DataSource::Query::Condition constructor was passed a NULL pointer!");
				throw exception;
			}
		}

		DataSource::Query::Condition::~Condition(void) throw()
		{
		}

		std::string DataSource::Query::Condition::field() const throw()
		{
			return _field;
		}

		std::string DataSource::Query::Condition::value() const throw()
		{
			return _value;
		}

		DataSource::Query::Condition::Comparitor DataSource::Query::Condition::comparitor() const throw()
		{
			return _comparitor;
		}

		DataSource::Query::Condition* DataSource::Query::Condition::conditionPtr1() const throw()
		{
			return _pCondition1;
		}

		DataSource::Query::Condition* DataSource::Query::Condition::conditionPtr2() const throw()
		{
			return _pCondition2;
		}

		DataSource::Query::Condition::Logic DataSource::Query::Condition::logic() const throw()
		{
			return _logic;
		}

		//----------------------------------------------------------------------------------------------------------
		const unsigned char DataSource::Query::maxHitsDefault = 100;

		DataSource::Query::Query(Condition* pCondition) throw(std::invalid_argument)
			:	_maxHits(maxHitsDefault),
				_pCondition(pCondition)
        {
			if(pCondition == NULL)
			{
				std::invalid_argument exception("DataSource::Query constructor was passed a NULL pointer!");
				throw exception;
			}
        }

		DataSource::Query::Query(Condition* pCondition, unsigned short maxHits) throw(std::invalid_argument)
			:	_maxHits(maxHits),
				_pCondition(pCondition)
        {
			if(pCondition == NULL)
			{
				std::invalid_argument exception("Query constructor was passed a NULL pointer!");
				throw exception;
			}
        }

        DataSource::Query::~Query() throw()
		{
			//removeAllConditions();
		}

		unsigned short DataSource::Query::maxHits(void) const throw()
		{
			return _maxHits;
		}

		void DataSource::Query::maxHits(unsigned short maxHits) throw()
		{
			_maxHits = maxHits;
		}

		DataSource::Query::Condition* DataSource::Query::condition(void) const throw()
		{
			return _pCondition;
		}

		DataSource::DataSource(const std::string& name) throw()
			:	_name(name)
		{
		}

		DataSource::~DataSource(void) throw()
		{
		}

/*
		void DataSource::Query::addCondition(Condition* pCondition)
		{
			std::list<Condition*>::const_iterator it = _conditionList.begin();
			while(it != _conditionList.end())
			{
				if(*it == pCondition)
					return;
			}

			_conditionList.push_back(pCondition);
		}

        void DataSource::Query::removeCondition(Condition* pCondition)
		{
			_conditionList.remove(pCondition);
		}

        void DataSource::Query::removeAllConditions()
        {
			if(!_conditionList.empty())
			{
				std::list<Condition*>::const_iterator it = _conditionList.begin();
				do
				{
					delete *it;

				} while(++it != _conditionList.end());
				_conditionList.clear();
			}
        }

     	bool DataSource::Query::conditionListEmpty() const
		{
			return _conditionList.empty();
		}

		std::list<DataSource::Query::Condition*>::size_type DataSource::Query::conditionListSize() const
		{
			return _conditionList.size();
		}

		std::list<DataSource::Query::Condition*>::const_iterator DataSource::Query::conditionListBeginIterator() const
		{
			return _conditionList.begin();
		}

		std::list<DataSource::Query::Condition*>::const_iterator DataSource::Query::conditionListEndIterator() const
		{
			return _conditionList.end();
		}
*/
	}
}

