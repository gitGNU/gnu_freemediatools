// set.cpp

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#include <openassetlib/al/set.hpp>
#include <openassetlib/py/python.hpp>
#include <openassetlib/py/py.hpp>

namespace al = olib::openassetlib::al;
namespace py = boost::python;

namespace olib { namespace openassetlib { namespace detail {

	namespace {

	al::set::iterator	(al::set::*begin)()	= &al::set::begin;
	al::set::iterator	(al::set::*end)()	= &al::set::end;

	al::set::reference	(al::set::*at)(al::set::size_type pos)	= &al::set::at;

	bool (al::set::*insert1)(const al::asset_ptr& asset, int offset)	= &al::set::insert;
	bool (al::set::*insert2)(const al::set_ptr& set, int offset)		= &al::set::insert;

	opl::string (al::set::*get_name)() const					= &al::set::name;
	void		(al::set::*set_name)(const opl::string& name)	= &al::set::name;

	al::set_ptr create_set()
	{
		return al::set::create();
	}

	al::set_ptr create_set(const opl::string& name)
	{
		return al::set::create(name);
	}

	al::set_ptr create_set(const al::asset_ptr& asset)
	{
		return al::set::create(asset);
	}

	al::set_ptr create_set(const al::asset_ptr& asset, const opl::string& name)
	{
		return al::set::create(asset, name);
	}

	al::set_ptr create_set(const al::set_ptr& set)
	{
		return al::set::create(set);
	}

	al::set_ptr create_set(const al::set_ptr& set, const opl::string& name)
	{
		return al::set::create(set, name);
	}

	al::set_ptr (*create_set1)()													= &create_set;
	al::set_ptr (*create_set2)(const opl::string& name)								= &create_set;
	al::set_ptr (*create_set3)(const al::asset_ptr& asset)							= &create_set;
	al::set_ptr (*create_set4)(const al::asset_ptr& asset, const opl::string& name)	= &create_set;
	al::set_ptr (*create_set5)(const al::set_ptr& set)								= &create_set;
	al::set_ptr (*create_set6)(const al::set_ptr& set, const opl::string& name)		= &create_set;

	}

	void py_set()
	{
		py::class_<al::set, boost::noncopyable, al::set_ptr>("set", py::no_init)
			.def("__iter__", py::range(begin, end))
			.def("name", get_name)
			.def("name", set_name)
			.def("at", at, py::return_value_policy<py::return_by_value>())
			.def("clear", &al::set::clear)
			.def("empty", &al::set::empty)
			.def("insert", insert1)
			.def("insert", insert2)
			.def("pop_back", &al::set::pop_back)
			.def("push_back", &al::set::push_back)
			.def("size", &al::set::size)
			.def("remove", &al::set::remove)
			.def("find", &al::set::find)
			;

		py::def("create_set", create_set1);
		py::def("create_set", create_set2);
		py::def("create_set", create_set3);
		py::def("create_set", create_set4);
		py::def("create_set", create_set5);
		py::def("create_set", create_set6);
	}

}}}
