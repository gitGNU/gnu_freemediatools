
// OBJ - An openobjectlib obj plugin.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef SPIRIT_INC_
#define SPIRIT_INC_

#ifdef WIN32
#pragma warning ( push )
#	pragma warning ( disable: 4267 4275 4512 )
#endif

#ifndef NDEBUG
//#define BOOST_SPIRIT_DEBUG
#endif

#include <boost/spirit/core.hpp>
#include <boost/spirit/utility.hpp>
#include <boost/spirit/symbols.hpp>

#ifdef WIN32
#	pragma warning ( default: 4267 4275 4512 )
#pragma warning ( pop )
#endif

#endif
