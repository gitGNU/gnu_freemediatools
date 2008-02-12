// plugin.cpp

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#include <openassetlib/al/al_plugin.hpp>
#include <openassetlib/py/python.hpp>
#include <openassetlib/py/py.hpp>

namespace py = boost::python;

namespace olib { namespace openassetlib { namespace detail {

	namespace {

		class al_plugin_wrapper : public al::al_plugin, public py::wrapper<al::al_plugin>
		{
		public:
			al::set_ptr store(const al::set_ptr&)
			{
				return py::call<al::set_ptr>(this->get_override("store").ptr());
			}

			al::set_ptr load(const opl::string&)
			{
				return py::call<al::set_ptr>(this->get_override("load").ptr());
			}
		};

		typedef boost::shared_ptr<al_plugin_wrapper>	al_plugin_wrapper_ptr;
	}

	void py_plugin()
	{
		py::class_<al_plugin_wrapper, boost::noncopyable, al_plugin_wrapper_ptr, al::al_plugin_ptr>("al_plugin")
			.def("store", py::pure_virtual(&al::al_plugin::store))
			.def("load", py::pure_virtual(&al::al_plugin::load))
			;
	}

}}}
