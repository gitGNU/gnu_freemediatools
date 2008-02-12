// asset.hpp
//
// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef ASSET_INC_
#define ASSET_INC_

#ifdef _MSC_VER
#pragma warning ( push )
#pragma warning ( disable: 4701 4127 4511 4512 4251 )
#endif

#include <map>

#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include <openpluginlib/pl/openpluginlib.hpp>
#include <openpluginlib/pl/utf8_utils.hpp>

#include <openassetlib/al/config.hpp>
#include <openassetlib/al/guid.hpp>

#if defined ( HAVE_FLEX_STRING )

// flex_string doesn't implement operator>>. The below
// is a hack implementation for use with boost::lexical_cast.

template <typename E, class T, class A, class S>
std::basic_istream<typename flex_string<E, T, A, S>::value_type, typename flex_string<E, T, A, S>::traits_type>& 
operator>>(
    std::basic_istream<typename flex_string<E, T, A, S>::value_type,
    typename flex_string<E, T, A, S>::traits_type>& is,
    flex_string<E, T, A, S>& str )
{
	std::basic_string<E, T, A> tmp;
	
	is >> tmp;
	str.assign( tmp.begin( ), tmp.end( ) );
	
	return is;
}

#endif

namespace opl = olib::openpluginlib;

namespace olib { namespace openassetlib { namespace al {

typedef boost::shared_ptr<class asset> asset_ptr;

class AL_DECLSPEC asset_factory
{
public:
	static asset_factory&	instance();

	asset_ptr	create_asset();
	asset_ptr	create_asset(const guid_ptr& guid);
	asset_ptr	create_asset(const asset_ptr& a);

	void	housekeep();	// removes objects in container that no longer have any other references

private:
	static asset_factory*	instance_;

	asset_factory(void);
	~asset_factory(void);

	typedef std::map<guid_ptr, asset_ptr>	asset_container_t;
	asset_container_t	asset_container_;
	unsigned short		count_;
};

class AL_DECLSPEC asset
{
public:
	typedef std::map<opl::string, opl::string>	map_type;
	typedef map_type::allocator_type			allocator_type;
	typedef map_type::const_iterator			const_iterator;
	typedef map_type::const_reverse_iterator	const_reverse_iterator;
	typedef map_type::iterator					iterator;
	typedef map_type::key_compare				key_compare;
	typedef map_type::key_type					key_type;
	typedef map_type::mapped_type				mapped_type;
	typedef map_type::reference					reference;
	typedef map_type::size_type					size_type;
	typedef map_type::value_type				value_type;
	typedef map_type::value_compare				value_compare;

	virtual ~asset();

	const guid_ptr	guid() const;

	const opl::string guid_string() const
	{
		return guid_ ? guid_->string( ) : "";
	}

	bool is_dirty() const;
	void clear_dirty_flag();

	void set(const key_type& key, const bool           value);
	void set(const key_type& key, const unsigned char  value);
	void set(const key_type& key, const char           value);
	void set(const key_type& key, const unsigned short value);
	void set(const key_type& key, const short          value);
	void set(const key_type& key, const unsigned long  value);
	void set(const key_type& key, const long           value);
	void set(const key_type& key, const unsigned int   value);
	void set(const key_type& key, const int            value);
	void set(const key_type& key, const float          value);
	void set(const key_type& key, const double         value);
	void set(const key_type& key, const long double    value);
	void set(const key_type& key, const long long      value);
	void set(const key_type& key, const opl::string    value);

	const mapped_type get(const key_type& key) const;

	const_iterator								begin() const;
	size_type									count(const key_type& key) const;
	bool										empty() const;
	const_iterator								end() const;
	std::pair<const_iterator, const_iterator>	equal_range(const key_type& key) const;
	const_iterator								find(const key_type& key) const;
	allocator_type								get_allocator() const;
	key_compare									key_comp() const;
	const_iterator								lower_bound(const key_type& key) const;
	size_type									max_size() const;
	const_reverse_iterator						rbegin() const;
	const_reverse_iterator						rend() const;
	size_type									size() const;
	const_iterator								upper_bound(const key_type& key) const;
	value_compare								value_comp() const;

	// additional helper functions to be exported to python interface
	value_type find_metadata(const key_type& key) const;

	bool operator==(const asset& rhs) const;
	bool operator!=(const asset& rhs) const;


private:
	friend class asset_factory;

	asset();
	asset(const guid_ptr& guid);
	asset(const asset_ptr& asset);

	guid_ptr	guid_;
	map_type	map_;
	bool		dirty_;
};

} } }

#ifdef _MSC_VER
#pragma warning ( pop )
#endif

#endif
