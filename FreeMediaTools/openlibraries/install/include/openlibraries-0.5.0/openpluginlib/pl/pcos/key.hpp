
// openpluginlib - A plugin interface to openlibraries.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef PCOS_KEY_H
#define PCOS_KEY_H

#include <openpluginlib/pl/config.hpp>

// std
#include <vector>

namespace olib { namespace openpluginlib { namespace pcos {

/// A lightweight, comparable key to be used for 
/// properties
class OPENPLUGINLIB_DECLSPEC key
{
public:
    static key from_string( const char* );
    const char* as_string() const;
    
    bool operator<( const key& ) const;
    bool operator==( const key& ) const;
    bool operator!=( const key& ) const;

    OPENPLUGINLIB_DECLSPEC friend std::ostream& operator<<( std::ostream&, const key& );

private:
    key( std::size_t );
    
    std::size_t id_;
};

typedef std::vector< key > key_vector;

OPENPLUGINLIB_DECLSPEC std::ostream& operator<<( std::ostream&, const key& );

} } }

#endif // PCOS_KEY_H
