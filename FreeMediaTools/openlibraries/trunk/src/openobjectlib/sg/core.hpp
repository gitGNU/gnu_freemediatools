
// sg - A scene graph representation.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef CORE_INC_
#define CORE_INC_

#include <utility>

#if _MSC_VER >= 1400
#include <hash_map>
#else
#include <map>
#endif

#include <boost/any.hpp>
#include <boost/shared_ptr.hpp>

#include <openpluginlib/pl/property.hpp>

#include <openobjectlib/sg/config.hpp>

namespace olib { namespace openobjectlib { namespace sg {

#define IMPLEMENT_INDEXABLE_CLASS					\
	static int& get_class_index_static( )			\
	{												\
		static int index = -1;						\
		return index;								\
	}												\
													\
	virtual int get_class_index( )					\
	{ return get_class_index_static( ); }

//
class SG_DECLSPEC node
{
public:
	typedef openpluginlib::wstring					key_type;
	typedef std::pair<key_type, boost::any>			field_type;
#if _MSC_VER >= 1400
	typedef stdext::hash_map<key_type, field_type>	container;
#else
	typedef std::map<key_type, field_type>			container;
#endif
	typedef container::reference					reference;
	typedef container::const_reference				const_reference;
	typedef container::iterator						iterator;
	typedef container::const_iterator				const_iterator;
	typedef container::size_type					size_type;
	typedef container::value_type					value_type;

public:
	explicit node( const container& fields = container( ) )
		: fields_( fields )
		, dirty_( true )
	{ }

	node( const node& other )
		: fields_( other.fields_ )
		, dirty_( other.dirty_ )
	{ }

	node& operator=( const node& other )
	{ 
		node( other ).swap( *this ); 
		return *this;
	}
	
	void swap( node& other )
	{
		fields_.swap( other.fields_ );
		std::swap( dirty_, other.dirty_ );
	}

public:
	virtual ~node( )
	{ }

	IMPLEMENT_INDEXABLE_CLASS

public:
	template<class U>
	U value( const key_type& key )
	{ return *boost::any_cast<U>( &fields_[ key ].second ); }

	template<class U>
	U value( const key_type& key ) const
	{
		const_iterator I = fields_.find( key );
		if( I != fields_.end( ) )
			return *boost::any_cast<U>( &I->second.second );
						
		return U( ); // GC - the safest thing here would be to throw.
	}

	template<class U>
	U* pointer( const key_type& key )
	{ return boost::any_cast<U>( &fields_[ key ].second ); }

	template<class U>
	const U* pointer( const key_type& key ) const
	{
		const_iterator I = fields_.find( key );
		if( I != fields_.end( ) )
			return boost::any_cast<U>( &I->second.second );
						
		return static_cast<const U*>( 0 );
	}

public:
	bool insert( const key_type& key, const field_type& value )
	{ return fields_.insert( value_type( key, value ) ).second; }

public:
	virtual bool get_dirty( ) { return dirty_; }
	virtual void set_dirty( bool dirty ) { dirty_ = dirty; }

private:
	container fields_;
	bool dirty_;
};

typedef boost::shared_ptr<node> node_ptr;

class v_node_ptr: public openpluginlib::value_property<node_ptr> {};
class m_node_ptr: public openpluginlib::multi_value_property<node_ptr> {};

const openpluginlib::property_typename v_node_typename = L"SFNode";
const openpluginlib::property_typename m_node_typename = L"MFNode";

} } }

#endif
