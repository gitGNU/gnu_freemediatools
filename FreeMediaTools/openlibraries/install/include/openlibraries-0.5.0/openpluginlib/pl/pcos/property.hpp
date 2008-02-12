
// openpluginlib - A plugin interface to openlibraries.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef PCOS_PROPERTY_H
#define PCOS_PROPERTY_H

#ifdef _MSC_VER
#pragma warning ( push )
#pragma warning ( disable: 4251 4503 4512 4800 )
#endif

#include <vector>

#include <boost/variant.hpp>
#include <boost/shared_ptr.hpp>

#include <openpluginlib/pl/config.hpp>
#include <openpluginlib/pl/string.hpp>
#include <openpluginlib/pl/pcos/isubject.hpp>
#include <openpluginlib/pl/pcos/key.hpp>
#include <openpluginlib/pl/pcos/visitor.hpp>

namespace olib { namespace openpluginlib { namespace pcos {

typedef std::vector<int> int_list;
typedef std::vector<unsigned int> uint_list;
typedef std::vector<double> double_list;
typedef std::vector<string> string_list;
typedef std::vector<wstring> wstring_list;

typedef std::vector<int> int_vec;
typedef std::vector<unsigned int> uint_vec;
typedef std::vector<double> double_vec;
typedef std::vector<void*> void_vec;

typedef boost::variant<bool, int, unsigned int, float, double, void*, string, wstring, int_list, uint_list, double_list, string_list, wstring_list, void_vec> any;

/// A property class i.e. a pairing of a key to a value.
/// Properties are reference counted, and so may be passed by 
/// value if desired without much overhead.
class OPENPLUGINLIB_DECLSPEC property : public isubject
{
public:
    static property NULL_PROPERTY;

    property( const key& );
    virtual ~property();

    // subject interface
    void attach( boost::shared_ptr< observer > );
    void detach( boost::shared_ptr< observer > );
    void update();
    void block( boost::shared_ptr< observer > );
    void unblock( boost::shared_ptr< observer > );

    // iproperty_container interface
    property get_property_with_string( const char* k ) const;
    property get_property_with_key( const key& k ) const;
    key_vector get_keys( ) const;

    // clonable interface
    property* clone( ) const;

    // property
    /// allow retrieval of the key
    key get_key() const;

    /// allow direct assignment
    property& operator=( const any& );
    property& operator=( const property& );

    /// equality check; impl pointer comparison is enough
    bool operator==( const property& ) const;
    bool operator!=( const property& ) const;

    /// direct set of a value
    template<typename T>
    void set( const T& );

    /// allow set of a property value from a string
    /// i.e. parse string for value of appropriate type
    void set_from_string( const wstring& );

	/// allow set of a property value from a property
    void set_from_property( const property& );

    /// return the value; will throw a bad_any_cast if 
    /// we try to extract the wrong value type
    template<typename T> 
    T value() const;

    template<typename T> 
    T* pointer() const;
    
    /// query the type held by the property
    template<typename T>
    bool is_a() const;

    /// always notify any observers on an assignment
    /// (by default, notification only happens if the assigned
    /// value and the current value are different)
    void set_always_notify( bool );

    /// visitor interface
    void accept( visitor& );

    /// Check if we're usable
    bool valid() const;

private:
    property( );

    class property_impl;
    boost::shared_ptr< property_impl > impl_;
};

} } }

#ifdef _MSC_VER
#pragma warning ( pop )
#endif

#endif // PCOS_PROPERTY_H
