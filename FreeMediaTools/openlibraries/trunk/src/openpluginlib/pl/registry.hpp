
// openpluginlib - A plugin interface to openlibraries.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef REGISTRY_INC_
#define REGISTRY_INC_

#include <cstdlib>
#include <new>

#include <boost/filesystem/path.hpp>

#if _MSC_VER >= 1400
#include <hash_map>
#else
#include <map>
#endif

#include <openpluginlib/pl/openplugin.hpp>

namespace olib { namespace openpluginlib { namespace detail {

//
class registry
{
public:
	typedef wstring key_type;
	
#if _MSC_VER >= 1400
	typedef stdext::hash_multimap<key_type, plugin_item> container;
#else
	typedef std::multimap<key_type, plugin_item> container;
#endif

private:
	static registry* revive_if_dead( )
	{
		void* p = new char [ sizeof( registry ) ];

		new( p ) registry;
		
		return static_cast<registry*>( p );
	}

	static void destroy( void )
	{
		instance_->~registry( );

		delete[ ] ( ( char* ) instance_ );
	}

public:	
	static registry& instance( )
	{
		if( !instance_ )
		{
			if( destroyed_ )
			{
				destroyed_     = false;
				was_destroyed_ = true;
			}

			instance_ = revive_if_dead( );

			if( !was_destroyed_ )
				std::atexit( annihilate );
		}
		
		return *instance_;
	}

	static void annihilate( )
	{
		if( instance_ )
		{
			assert( !destroyed_ );
			destroy( );
			destroyed_ = true;
			instance_ = 0;
		}
	}

public:
	bool insert( const string& lookup_path );
	bool remove( );
	void clear( );

	const container& get_db( ) const
	{ return db_; }

	container& get_db( )
	{ return db_; }

private:
	explicit registry( const string& lookup_path = "" );
	~registry( );
	registry( const registry& );
	registry& operator=( const registry& );
	
private:
	container db_;
	static registry* instance_;
	static bool destroyed_, was_destroyed_;
};

} } }

#endif
