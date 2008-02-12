// asset.cpp

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#include <openassetlib/al/guid.hpp>
#include <openassetlib/py/python.hpp>
#include <openassetlib/py/py.hpp>

namespace al = olib::openassetlib::al;
namespace py = boost::python;

namespace olib { namespace openassetlib { namespace detail {

	namespace {
	
		al::guid_ptr create_guid()
		{
			return al::guid_factory::instance().create_guid();
		}

		al::guid_ptr create_guid(const opl::string& guid_string)
		{
			return al::guid_factory::instance().create_guid(guid_string);
		}

		al::guid_ptr create_guid(long long upper, long long lower)
		{
			return al::guid_factory::instance().create_guid(upper, lower);
		}

		al::guid_ptr (*create_guid1)()									= &create_guid;
		al::guid_ptr (*create_guid2)(const opl::string& guid_string)	= &create_guid;
		al::guid_ptr (*create_guid3)(long long upper, long long lower)	= &create_guid;
	
	}

	void py_guid()
	{
		py::class_<al::guid, boost::noncopyable, al::guid_ptr>("guid", py::no_init)
			.def("upper", &al::guid::upper)
			.def("lower", &al::guid::lower)
			.def("string", &al::guid::string)
			.def("__eq__", &al::guid::operator==)
			.def("__ne__", &al::guid::operator!=)
			;

		py::def("create_guid", create_guid1);
		py::def("create_guid", create_guid2);
		py::def("create_guid", create_guid3);
	}

}}}
