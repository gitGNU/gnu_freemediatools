// py.cpp

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#include <openassetlib/py/python.hpp>
#include <openassetlib/py/py.hpp>

namespace oal = olib::openassetlib;

BOOST_PYTHON_MODULE( openassetlib )
{
	oal::detail::py_plugin();
	oal::detail::py_guid();
	oal::detail::py_asset();
	oal::detail::py_set();
	oal::detail::py_openassetlib();

}
