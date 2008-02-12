// set.cpp
//
// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#include "set.hpp"

namespace olib { namespace openassetlib { namespace al {

set_ptr set::create(const opl::string& name)
{
	return set_ptr(new set(name));
}

set_ptr set::create(const asset_ptr& asset, const opl::string& name)
{
	return set_ptr(new set(asset, name));
}

set_ptr set::create(const set_ptr& s, const opl::string& name)
{
	return set_ptr(new set(s, false, name));
}

set::set(const opl::string& name)
	:	name_(name)
{
}

set::set(const asset_ptr asset, const opl::string& name)
	:	name_(name)
{
	if(asset == asset_ptr())
		return;

//	vec_.push_back(asset_factory::create_asset(asset));
	vec_.push_back(asset);
}

set::set(const set_ptr set, bool deepcopy, const opl::string& name)
	:	name_(name)
{
	if(set == set_ptr())
		return;

	// copy of all assets into new set
	if(!set->empty())
	{
		const_iterator cit = set->begin();
		do
		{
			if(deepcopy)
				vec_.push_back(asset_factory::instance().create_asset(*cit));
			else
				vec_.push_back(*cit);

		}	while(++cit != set->end());
	}
}

set::~set(void)
{
	vec_.clear();
}

opl::string set::name() const
{
	return name_;
}

void set::name(const opl::string& name)
{
	name_ = name;
}

set::const_reference set::at(size_type pos) const
{
	return vec_.at(pos);
}

set::reference set::at(size_type pos)
{
	return vec_.at(pos);
}

set::const_reference set::back() const
{
	return vec_.back();
}

set::reference set::back()
{
	return vec_.back();
}

set::const_iterator set::begin() const
{
	return vec_.begin();
}

set::iterator set::begin()
{
	return vec_.begin();
}

set::size_type set::capacity() const
{
	return vec_.capacity();
}

void set::clear()
{
	vec_.clear();
}

bool set::empty() const
{
	return vec_.empty();
}

set::const_iterator set::end() const
{
	return vec_.end();
}

set::iterator set::end()
{
	return vec_.end();
}

set::iterator set::erase(iterator where)
{
	return vec_.erase(where);
}

set::iterator set::erase(iterator first, iterator last)
{
	return vec_.erase(first, last);
}

set::const_reference set::front() const
{
	return vec_.front();
}

set::reference set::front()
{
	return vec_.front();
}

template<>
void set::insert(iterator where, iterator first, iterator last)
{
	vec_.insert(where, first, last);
}

set::iterator set::insert(iterator where, const value_type& val)
{
	return vec_.insert(where, val);
}

void set::insert(iterator where, size_type count, const value_type& val)
{
	vec_.insert(where, count, val);
}

set::size_type set::max_size() const
{
	return vec_.max_size();
}

void set::pop_back()
{
	vec_.pop_back();
}

void set::push_back(const value_type& val)
{
	vec_.push_back(val);
}

set::const_reverse_iterator set::rbegin() const
{
	return vec_.rbegin();
}

set::reverse_iterator set::rbegin()
{
	return vec_.rbegin();
}

set::reverse_iterator set::rend()
{
	return vec_.rend();
}

set::const_reverse_iterator set::rend() const
{
	return vec_.rend();
}

void set::resize(size_type newsize)
{
	vec_.resize(newsize);
}

void set::resize(size_type newsize, value_type val)
{
	vec_.resize(newsize, val);
}

void set::reserve(size_type count)
{
	vec_.reserve(count);
}

set::size_type set::size() const
{
	return vec_.size();
}

void set::swap(set& right)
{
	vec_.swap(right.vec_);
}

void set::remove(const asset_ptr& asset)
{
	if(asset == asset_ptr())
		return;

	iterator it = begin();
	while(it != end())
	{
		if(*(asset->guid().get()) == *((*it)->guid().get()))
			it = vec_.erase(it);
		else
			it++;
	}
}

bool set::insert(const asset_ptr& asset, int offset)
{
	if(asset == asset_ptr())
		return false;

	if((size_type)offset > size())
		return false;

	iterator it = begin();
	it += offset;
	insert(it, asset);
	return true;
}

bool set::insert(const set_ptr& set, int offset)
{
	if(set == set_ptr())
		return false;

	if((size_type)offset > size())
		return false;

	iterator it = begin();
	it += offset;
	insert(it, set->begin(), set->end());
	return true;
}

asset_ptr set::find(const guid_ptr& guid)
{
	for(iterator it = begin(); it != end(); it++)
	{
		if(*((*it)->guid().get()) == *(guid.get()))
			return (*it);
	}

	return asset_ptr();
}

} } }
