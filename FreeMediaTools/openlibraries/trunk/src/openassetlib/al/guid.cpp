// guid.cpp

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#include <openassetlib/al/guid.hpp>

namespace olib { namespace openassetlib { 

	namespace al {

		guid_generator_ptr	guid_factory::guid_gen_ptr_	= guid_generator_ptr();
		guid_factory*		guid_factory::instance_		= NULL;

		void guid_factory::initialize(guid_generator_ptr guid_gen_ptr)
		{
			guid_gen_ptr_ = guid_gen_ptr;
		}

		void guid_factory::uninitialize()
		{
			guid_gen_ptr_ = guid_generator_ptr();
		}

		guid_factory& guid_factory::instance()
		{
			if(!instance_)
				instance_ = new guid_factory();

			return *instance_;
		}

		guid_factory::guid_factory()
			:	count_(0)
		{
		}

		guid_factory::~guid_factory()
		{
			guid_container_.clear();
		}

		guid_ptr guid_factory::create_guid()
		{
			if(guid_gen_ptr_ == guid_generator_ptr())
				throw new std::runtime_error("guid_factory has not been initialized!");

			opl::string guid_string = guid_gen_ptr_->generate_guid();
			return create_guid_object(guid_string);
		}

		guid_ptr guid_factory::create_guid(const opl::string& guid_string)
		{
			for(guid_container_t::const_iterator it = guid_container_.begin();
				it != guid_container_.end();
				it++)
			{
				if((*it)->string().compare(guid_string) == 0)
					return *it;
			}

			return create_guid_object(guid_string);
		}

		guid_ptr guid_factory::create_guid(long long upper, long long lower)
		{
			if(++count_ >= 100)
				housekeep();

			for(guid_container_t::const_iterator it = guid_container_.begin();
				it != guid_container_.end();
				it++)
			{
				if(		((*it)->upper() == upper)
					&&	((*it)->lower() == lower) )
				{
					return *it;
				}
			}

			guid_ptr g = guid_ptr(new guid(upper, lower));
			guid_container_.push_back(g);
			return g;
		}

		guid_ptr guid_factory::create_guid_object(const opl::string& guid_string)
		{
			if(++count_ >= 100)
				housekeep();

			guid_ptr g = guid_ptr(new guid(guid_string));
			guid_container_.push_back(g);
			return g;
		}

		void guid_factory::housekeep()
		{
			count_ = 0;
			guid_container_t container;
			for(guid_container_t::const_iterator it = guid_container_.begin();
				it != guid_container_.end();
				it++)
			{
				//fprintf(stdout, "%s has %d references\n", (*it)->string().c_str(), (*it).use_count());
				if((*it).use_count() > 1)
					container.push_back(*it);
			}

			guid_container_.swap(container);
		}

		namespace {

			unsigned char char2nibble(char ch)
			{
				if(ch >= '0' && ch <= '9')
					return ch - '0';
				if(ch >= 'a' && ch <= 'f')
					return ch - 'a' + 10;
				if(ch >= 'A' && ch <= 'F')
					return ch - 'A' + 10;
				else
					throw new std::invalid_argument("invalid hex character");
			}
		}

		guid::guid(long long upper, long long lower)
			:	upper_(upper)
			,	lower_(lower)
		{
			char szGuid[39];
			sprintf(szGuid, 
					"{%08x-%04x-%04x-%04x-%04x%08x}", 
					long(upper >> 32), 
					long(upper >> 16) & 0xFFFF, 
					long(upper) & 0xFFFF, 
					long(lower >> 48) & 0xFFFF, 
					long(lower >> 32) & 0xFFFF, 
					long(lower));
			guid_string_ = opl::string(szGuid);
		};

		guid::guid(const opl::string& guid_string) // must be in this format: {%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}
		{
			guid_string_ = guid_string;

			if(		(guid_string.size() == 38)
				&&	(guid_string[0] == '{')
				&&	(guid_string[9] == '-')
				&&	(guid_string[14] == '-')
				&&	(guid_string[19] == '-')
				&&	(guid_string[24] == '-')
				&&	(guid_string[37] == '}') )
			{
				upper_ = 0;
				lower_ = 0;

				for(unsigned char idx = 1;
					idx < 9;
					idx++)
				{
					upper_ <<= 4;
					upper_ |= char2nibble(guid_string[idx]);
				}

				for(unsigned char idx = 10;
					idx < 14;
					idx++)
				{
					upper_ <<= 4;
					upper_ |= char2nibble(guid_string[idx]);
				}

				for(unsigned char idx = 15;
					idx < 19;
					idx++)
				{
					upper_ <<= 4;
					upper_ |= char2nibble(guid_string[idx]);
				}

				for(unsigned char idx = 20;
					idx < 24;
					idx++)
				{
					lower_ <<= 4;
					lower_ |= char2nibble(guid_string[idx]);
				}

				for(unsigned char idx = 25;
					idx < 37;
					idx++)
				{
					lower_ <<= 4;
					lower_ |= char2nibble(guid_string[idx]);
				}
			}
		}
		guid::~guid()
		{
		}

		long long guid::upper() const
		{
			return upper_;
		}

		long long guid::lower() const
		{
			return lower_;
		}

		opl::string guid::string() const
		{
			return guid_string_;
		}

		bool guid::operator==(const guid& rhs) const
		{
			return ((upper_ == rhs.upper_) && (lower_ == rhs.lower_));
		}

		bool guid::operator!=(const guid& rhs) const
		{
			return !operator==(rhs);
		}
	}
}}
