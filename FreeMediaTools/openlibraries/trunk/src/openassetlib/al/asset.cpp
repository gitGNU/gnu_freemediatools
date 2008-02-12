// asset.cpp
//
// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#include "asset.hpp"
#include <openassetlib/al/al.hpp>

namespace olib { namespace openassetlib { namespace al {

	asset_factory*	asset_factory::instance_	= NULL;

	asset_factory& asset_factory::instance()
	{
		if(!instance_)
			instance_ = new asset_factory();

		return *instance_;
	}

	asset_factory::asset_factory()
		:	count_(0)
	{
	}

	asset_factory::~asset_factory()
	{
		asset_container_.clear();
	}

	asset_ptr asset_factory::create_asset()
	{
		if(++count_ >= 100)
			housekeep();

		asset_ptr a = asset_ptr(new asset());
		asset_container_[a->guid()] = a;
		return a;
	}

	asset_ptr asset_factory::create_asset(const guid_ptr& guid)
	{
		if(++count_ >= 100)
			housekeep();

		asset_container_t::iterator it = asset_container_.find(guid);
		if(it != asset_container_.end())
			return it->second;

		asset_ptr a = asset_ptr(new asset(guid));
		asset_container_[a->guid()] = a;
		return a;
	}

	asset_ptr asset_factory::create_asset(const asset_ptr& original_asset)
	{
		if(++count_ >= 100)
			housekeep();

		asset_ptr a = asset_ptr(new asset(original_asset));
		asset_container_[a->guid()] = a;
		return a;
	}

	void asset_factory::housekeep()
	{
		count_ = 0;
		asset_container_t container;
		for(asset_container_t::const_iterator it = asset_container_.begin();
			it != asset_container_.end();
			it++)
		{
			if(it->second.use_count() > 1)
				container[it->first] = it->second;
		}

		asset_container_.swap(container);
	}

	asset::asset()
		: dirty_(true)
	{
		guid_ = guid_factory::instance().create_guid();
	}

	asset::asset(const guid_ptr& guid)
		: dirty_(true)
	{
		guid_ = guid;
	}

	asset::asset(const asset_ptr& asset)
		: dirty_(true)
	{
		if(asset == asset_ptr())
			throw new std::invalid_argument("Null object passed to asset copy constructor");

		guid_ = guid_factory::instance().create_guid();

		// copy all metadata
		if(!asset->empty())
		{
			const_iterator cit = asset->begin();
			do
			{
				map_.insert(value_type(cit->first, cit->second));

			}	while(++cit != asset->end());
		}
	}

	asset::~asset()
	{
	}

	const guid_ptr asset::guid() const
	{
		return guid_;
	}

	bool asset::is_dirty() const
	{
		return dirty_;
	}

	void asset::clear_dirty_flag()
	{
		dirty_ = false;
	}

	void asset::set(const key_type& key, const bool value)
	{
		set(key, opl::string(value ? "true" : "false"));
	}

	void asset::set(const key_type& key, const unsigned char value)
	{
		set(key, boost::lexical_cast<opl::string>(static_cast<unsigned short>(value)));
	}

	void asset::set(const key_type& key, const char value)
	{
		set(key, boost::lexical_cast<opl::string>(static_cast<short>(value)));
	}

	void asset::set(const key_type& key, const unsigned short value)
	{
		set(key, boost::lexical_cast<opl::string>(value));
	}

	void asset::set(const key_type& key, const short value)
	{
		set(key, boost::lexical_cast<opl::string>(value));
	}

	void asset::set(const key_type& key, const unsigned long value)
	{
		set(key, boost::lexical_cast<opl::string>(value));
	}

	void asset::set(const key_type& key, const long value)
	{
		set(key, boost::lexical_cast<opl::string>(value));
	}

	void asset::set(const key_type& key, const unsigned int value)
	{
		set(key, boost::lexical_cast<opl::string>(value));
	}

	void asset::set(const key_type& key, const int value)
	{
		set(key, boost::lexical_cast<opl::string>(value));
	}

	void asset::set(const key_type& key, const float value)
	{
		set(key, boost::lexical_cast<opl::string>(value));
	}

	void asset::set(const key_type& key, const double value)
	{
		set(key, boost::lexical_cast<opl::string>(value));
	}

	void asset::set(const key_type& key, const long double value)
	{
		set(key, boost::lexical_cast<opl::string>(value));
	}

	void asset::set(const key_type& key, const long long value)
	{
		set(key, boost::lexical_cast<opl::string>(value));
	}

	void asset::set(const key_type& key, const opl::string value)
	{
		if(!map_.insert(value_type(key, value)).second)
		{
			iterator pair = map_.find(key);
			pair->second = value;
		}

		dirty_ = true;
	}

	const asset::mapped_type asset::get(const key_type& key) const
	{
		const_iterator pair = map_.find(key);
		return (pair != map_.end() ? pair->second : mapped_type());
	}

	asset::const_iterator asset::begin() const
	{
		return map_.begin();
	}

	asset::size_type asset::count(const key_type& key) const
	{
		return map_.count(key);
	}

	bool asset::empty() const
	{
		return map_.empty();
	}

	asset::const_iterator asset::end() const
	{
		return map_.end();
	}

	std::pair<asset::const_iterator, asset::const_iterator> asset::equal_range(const key_type& key) const
	{
		return map_.equal_range(key);
	}

	asset::const_iterator asset::find(const key_type& key) const
	{
		return map_.find(key);
	}

	asset::allocator_type asset::get_allocator() const
	{
		return map_.get_allocator();
	}

	asset::key_compare asset::key_comp() const
	{
		return map_.key_comp();
	}

	asset::const_iterator asset::lower_bound(const key_type& key) const
	{
		return map_.lower_bound(key);
	}

	asset::size_type asset::max_size() const
	{
		return map_.max_size();
	}

	asset::const_reverse_iterator asset::rbegin() const
	{
		return map_.rbegin();
	}

	asset::const_reverse_iterator asset::rend() const
	{
		return map_.rend();
	}

	asset::size_type asset::size() const
	{
		return map_.size();
	}

	asset::const_iterator asset::upper_bound(const key_type& key) const
	{
		return map_.upper_bound(key);
	}

	asset::value_compare asset::value_comp() const
	{
		return map_.value_comp();
	}

	// additional helper functions to be exported to python interface
	asset::value_type asset::find_metadata(const key_type& key) const
	{	
		const_iterator it = map_.find(key);
		if ( it != end( ) )
			return *it;
		else
			return value_type( );
	}

	bool asset::operator==(const asset& rhs) const
	{
		return guid_ == rhs.guid_;
	}

	bool asset::operator!=(const asset& rhs) const
	{
		return !operator==(rhs);
	}

} } }
