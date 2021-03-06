
// openpluginlib - A plugin interface to openlibraries.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef PCOS_PROPERTY_CONTAINER_H
#define PCOS_PROPERTY_CONTAINER_H

#ifdef _MSC_VER
#pragma warning ( push )
#	pragma warning ( disable:4251 )
#endif

#include <boost/shared_ptr.hpp>

#include <openpluginlib/pl/config.hpp>
#include <openpluginlib/pl/pcos/property.hpp>
#include <openpluginlib/pl/pcos/key.hpp>
#include <openpluginlib/pl/pcos/iclonable.hpp>
#include <openpluginlib/pl/pcos/isubject.hpp>
#include <openpluginlib/pl/pcos/iproperty_container.hpp>
#include <openpluginlib/pl/pcos/visitor.hpp>

namespace olib { namespace openpluginlib { namespace pcos {

/// A loose container of properties; aggregates property notification
/// i.e. if a contained property changes, so the property_container
/// will notify it has changed
class OPENPLUGINLIB_DECLSPEC property_container : public iclonable,
						  public iproperty_container, 
						  public isubject
{
public:
    explicit property_container( );
    ~property_container( );

    /// equality check; impl pointer comparison is enough
    bool operator==( const property_container& ) const;
    bool operator!=( const property_container& ) const;

    // isubject interface
    void attach( boost::shared_ptr< observer > );
    void detach( boost::shared_ptr< observer > );
    void update();
    void block( boost::shared_ptr< observer > );
    void unblock( boost::shared_ptr< observer > );

    // iproperty_container interface
    property get_property_with_string( const char* k ) const;
    property get_property_with_key( const key& k ) const;
    key_vector get_keys() const;
    void append( const property& );
    void remove( const property& );
    void accept( visitor& );

    // iclonable interface
    property_container* clone() const;

private:
    class property_container_impl;
    boost::shared_ptr< property_container_impl > impl_;
};

} } }

#ifdef _MSC_VER
#pragma warning ( pop )
#endif

#endif
