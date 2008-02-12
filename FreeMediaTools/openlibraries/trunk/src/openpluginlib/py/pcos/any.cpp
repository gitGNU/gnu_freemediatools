
// openpluginlib - A plugin interface to openlibraries.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#include <openpluginlib/pl/string.hpp>
#include <openpluginlib/pl/pcos/property.hpp>
#include <openpluginlib/py/python.hpp>
#include <openpluginlib/py/py.hpp>

namespace opl	= olib::openpluginlib;
namespace pcos	= olib::openpluginlib::pcos;
namespace py	= boost::python;

namespace olib { namespace openpluginlib { namespace detail {

template<typename T>
T any_cast( const pcos::any& t )
{
	return boost::get<T>( t );
}

double 				(*double_cast)( const pcos::any& )			= &any_cast< double >;
int    				(*int_cast)( const pcos::any& )				= &any_cast< int >;
unsigned int		(*uint_cast)( const pcos::any& )			= &any_cast< unsigned int >;
string  			(*string_cast)( const pcos::any& )			= &any_cast< string >;
wstring 			(*wstring_cast)( const pcos::any& )			= &any_cast< wstring >;
string_list    		(*string_list_cast)( const pcos::any& )		= &any_cast< string_list >;
wstring_list   		(*wstring_list_cast)( const pcos::any& )	= &any_cast< wstring_list >;
pcos::int_list 		(*int_list_cast)( const pcos::any& )		= &any_cast< pcos::int_list >;
pcos::uint_list		(*uint_list_cast)( const pcos::any& )		= &any_cast< pcos::uint_list >;
pcos::double_list	(*double_list_cast)( const pcos::any& )		= &any_cast< pcos::double_list >;
bool				(*bool_cast)( const pcos::any& )			= &any_cast< bool >;

void py_pcos_any()
{
    py::class_< pcos::any >( "any", py::init< int >() )
	.def( py::init< double >() )
	.def( py::init< int >() )
	.def( py::init< unsigned int >() )
	.def( py::init< string >() )
	.def( py::init< wstring >() )
	.def( py::init< string_list >() )
	.def( py::init< wstring_list >() )
	.def( py::init< pcos::int_list >() )
	.def( py::init< pcos::uint_list >() )
	.def( py::init< pcos::double_list >() )
	.def( py::init< bool >() )
	.def( "as_double", double_cast )
	.def( "as_int", int_cast )
	.def( "as_uint", uint_cast )
	.def( "as_string", string_cast )
	.def( "as_wstring", wstring_cast )
	.def( "as_string_list", string_list_cast )
	.def( "as_wstring_list", wstring_list_cast )
	.def( "as_int_list", int_list_cast )
	.def( "as_uint_list", uint_list_cast )
	.def( "as_double_list", double_list_cast )
	.def( "as_bool", bool_cast )
	;
}

} } }
