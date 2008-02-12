
// openpluginlib - A plugin interface to openlibraries.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#include <iostream>
#include <string>
#include <map>

#include <boost/functional/hash.hpp>

#include <openpluginlib/pl/pcos/key.hpp>

namespace olib { namespace openpluginlib { namespace pcos {

typedef std::map< std::string, std::size_t > string_key_map;
typedef std::map< std::size_t, std::string > key_string_map;

static key_string_map& keyStringMap()
{
    static key_string_map static_keyStringMap;
    return static_keyStringMap;
}

static string_key_map& stringKeyMap()
{
    static string_key_map static_stringKeyMap;
    return static_stringKeyMap;
}

key key::from_string( const char* keyAsString )
{
    if ( !stringKeyMap().count( keyAsString ) )
    {
        boost::hash< const char* > hashFn;
        std::size_t h = hashFn( keyAsString );

        stringKeyMap()[ keyAsString ] = h;
        keyStringMap()[ h ] = keyAsString;
    }

    return key( stringKeyMap()[ keyAsString ] );
}

const char* key::as_string() const
{
    return keyStringMap()[ id_ ].c_str();
}

key::key( std::size_t id )
    : id_( id )
{}
    
bool key::operator<( const key& k ) const
{
    return id_ < k.id_;
}

bool key::operator==( const key& k ) const
{
    return id_ == k.id_;
}

bool key::operator!=( const key& k ) const
{
    return id_ != k.id_;
}

std::ostream& operator<<( std::ostream& os, const key& k )
{
    os << k.as_string();
    return os;
}

} } }

