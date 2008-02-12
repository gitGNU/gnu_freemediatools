
// openpluginlib - A plugin interface to openlibraries.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef PCOS_SUBJECT_H
#define PCOS_SUBJECT_H

#include <openpluginlib/pl/config.hpp>
#include <openpluginlib/pl/pcos/isubject.hpp>

namespace olib { namespace openpluginlib { namespace pcos {

class observer;

/// A standard subject to be part of the observer pattern.
/// The observer pointers are held by shared references.
class OPENPLUGINLIB_DECLSPEC subject : public isubject
{
public:
    subject();
    virtual ~subject();

    //// attach an observer to this subject
    void attach( boost::shared_ptr< observer > );

    /// detach an observer from this subject
    void detach( boost::shared_ptr< observer > );

    /// notify all observers
    void update();

    /// block notification for an observer; this is 
    /// a counted block operation i.e. if you call
    /// block n times on the same observer, no 
    /// notification will be given until unblock()
    /// has been called a matching number of times.
    void block( boost::shared_ptr< observer > );

    /// unblock notification for an observer
    void unblock( boost::shared_ptr< observer > );

private:
    subject& operator=( const subject& );

    class subject_impl;
    boost::shared_ptr< subject_impl > impl_;
};

} } }

#endif // PCOS_SUBJECT_H
