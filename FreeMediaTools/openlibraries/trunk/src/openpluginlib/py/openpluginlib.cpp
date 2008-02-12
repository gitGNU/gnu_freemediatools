
// openpluginlib - A plugin interface to openlibraries.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#include <openpluginlib/pl/openpluginlib.hpp>
#include <openpluginlib/py/python.hpp>
#include <openpluginlib/py/py.hpp>

namespace opl	= olib::openpluginlib;
namespace py	= boost::python;

namespace olib { namespace openpluginlib { namespace detail {

struct all_query_traits
{
	all_query_traits( const wstring& libname = L"", const wstring& type = L"", const wstring& to_match = L"", int merit = 0 )
		: libname_( libname )
		, type_( type )
		, to_match_( to_match )
		, merit_( merit )
	{ }

	wstring libname( ) const
	{ return libname_; }
	wstring type( ) const
	{ return type_; }
	wstring to_match( ) const
	{ return to_match_; }
	int merit( ) const
	{ return merit_; }
	
	wstring libname_, type_, to_match_;
	int merit_;
};

BOOST_PYTHON_FUNCTION_OVERLOADS( init_overloads, init, 0, 2 )

void py_openpluginlib( )
{
	py::def( "init", &init, init_overloads( py::args( "lookup_path", "mode" ), "openpluginlib init" ) );
	py::def( "uninit", &uninit );
	py::def( "registered_filters", &registered_filters );
	
	py::class_<detail::all_query_traits>( "all_query_traits", py::init<wstring, wstring, wstring, int>( ) )
		.def( "libname", &detail::all_query_traits::libname )
		.def( "type", &detail::all_query_traits::type )
		.def( "to_match", &detail::all_query_traits::to_match )
		.def( "merit", &detail::all_query_traits::merit )
		;
	
	typedef detail::discover_query_impl::plugin_proxy plugin_proxy;
	opl_ptr ( plugin_proxy::*plugin_creator )( const string& ) const = &plugin_proxy::create_plugin;
	
	py::class_<openplugin, boost::noncopyable, opl_ptr>( "openplugin", py::init<>( ) )
		;
	
	py::class_<plugin_proxy>( "plugin_proxy", py::no_init )
		.def( "create_plugin", plugin_creator )
		.def( "name", &plugin_proxy::name )
		.def( "type", &plugin_proxy::type )
		.def( "mime", &plugin_proxy::mime )
		.def( "category", &plugin_proxy::category )
		.def( "libname", &plugin_proxy::libname )
		.def( "in_filter", &plugin_proxy::in_filter )
		.def( "out_filter", &plugin_proxy::out_filter )
		.def( "merit", &plugin_proxy::merit )
		;

	typedef opl::discovery<detail::all_query_traits> all_discovery;
	py::class_<all_discovery>( "discovery", py::init<detail::all_query_traits>( ) )
		.def( "__iter__", py::range( &all_discovery::begin, &all_discovery::end ) )
		.def( "empty", &all_discovery::empty )
		.def( "size", &all_discovery::size )
		;
}

} } }
