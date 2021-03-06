/* -*- mode: C; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t -*- */
// openpluginlib - A plugin interface to openlibraries.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#include <openpluginlib/pl/string.hpp>
#include <openpluginlib/pl/pcos/property.hpp>
#include <openpluginlib/pl/pcos/subject.hpp>
#include <openpluginlib/pl/pcos/key.hpp>
#include <openpluginlib/py/python.hpp>
#include <openpluginlib/py/py.hpp>

namespace opl	= olib::openpluginlib;
namespace pcos	= olib::openpluginlib::pcos;
namespace py	= boost::python;

namespace olib { namespace openpluginlib { namespace detail {

void ( pcos::property::*set_string )( const opl::string& ) = &pcos::property::set<opl::string>;
void ( pcos::property::*set_wstring )( const opl::wstring& ) = &pcos::property::set<opl::wstring>;
void ( pcos::property::*set_string_list )( const opl::string_list& ) = &pcos::property::set<opl::string_list>;
void ( pcos::property::*set_wstring_list )( const opl::wstring_list& ) = &pcos::property::set<opl::wstring_list>;
void ( pcos::property::*set_int )( const int& ) = &pcos::property::set<int>;
void ( pcos::property::*set_unsigned_int )( const unsigned int& ) = &pcos::property::set<unsigned int>;
void ( pcos::property::*set_double )( const double& ) = &pcos::property::set<double>;
void ( pcos::property::*set_int_list )( const pcos::int_list& ) = &pcos::property::set<pcos::int_list>;
void ( pcos::property::*set_uint_list )( const pcos::uint_list& ) = &pcos::property::set<pcos::uint_list>;
void ( pcos::property::*set_double_list )( const pcos::double_list& ) = &pcos::property::set<pcos::double_list>;
void ( pcos::property::*set_bool )( const bool& ) = &pcos::property::set<bool>;

wstring			  ( pcos::property::*value_wstring )( ) const = &pcos::property::value<wstring>;
string			  ( pcos::property::*value_string )( ) const = &pcos::property::value<string>;
wstring_list	  ( pcos::property::*value_wstring_list )( ) const = &pcos::property::value<wstring_list>;
string_list		  ( pcos::property::*value_string_list )( ) const = &pcos::property::value<string_list>;
int				  ( pcos::property::*value_int )( ) const = &pcos::property::value<int>;
unsigned int	  ( pcos::property::*value_unsigned_int )( ) const = &pcos::property::value<unsigned int>;
double			  ( pcos::property::*value_double )( ) const = &pcos::property::value<double>;
pcos::int_list	  ( pcos::property::*value_int_list )( ) const = &pcos::property::value<pcos::int_list>;
pcos::uint_list	  ( pcos::property::*value_uint_list )( ) const = &pcos::property::value<pcos::uint_list>;
pcos::double_list ( pcos::property::*value_double_list )( ) const = &pcos::property::value<pcos::double_list>;
bool			  ( pcos::property::*value_bool )( ) const = &pcos::property::value<bool>;

bool			 ( pcos::property::*is_a_wstring )( ) const = &pcos::property::is_a<opl::wstring>;
bool			 ( pcos::property::*is_a_string )( ) const = &pcos::property::is_a<opl::string>;
bool			 ( pcos::property::*is_a_wstring_list )( ) const = &pcos::property::is_a<opl::wstring_list>;
bool			 ( pcos::property::*is_a_string_list )( ) const = &pcos::property::is_a<opl::string_list>;
bool			 ( pcos::property::*is_a_int )( ) const = &pcos::property::is_a<int>;
bool			 ( pcos::property::*is_a_unsigned_int )( ) const = &pcos::property::is_a<unsigned int>;
bool			 ( pcos::property::*is_a_double )( ) const = &pcos::property::is_a<double>;
bool			 ( pcos::property::*is_a_int_list )( ) const = &pcos::property::is_a<pcos::int_list>;
bool			 ( pcos::property::*is_a_uint_list )( ) const = &pcos::property::is_a<pcos::uint_list>;
bool			 ( pcos::property::*is_a_double_list )( ) const = &pcos::property::is_a<pcos::double_list>;
bool			 ( pcos::property::*is_a_bool )( ) const = &pcos::property::is_a<bool>;

template < typename T > struct PyConvertTrait
{};

template <> struct PyConvertTrait< int >
{
	static PyObject* typeToPy( int v ) { return PyInt_FromLong( v ); }
	static bool checkType( PyObject* obj ) { return PyInt_CheckExact( obj ); }
};

template <> struct PyConvertTrait< double >
{
	static PyObject* typeToPy( double v ) { return PyFloat_FromDouble( v ); }
	static bool checkType( PyObject* obj ) { return PyFloat_CheckExact( obj ); }
};

template < typename LIST, typename TRAIT > struct list_to_python_list
{
    static PyObject* convert( const LIST& l )
	{ 
		PyObject* list = PyList_New( static_cast<int>( l.size() ) );
		if ( !list )
			py::throw_error_already_set();
		
		typename LIST::const_iterator I = l.begin();
		for ( size_t i=0; i<l.size(); ++i, ++I )
		{
			PyList_SetItem( list, i, TRAIT::typeToPy( *I ) );
		}
		return py::incref( list ); 
    }
};

template < typename LIST, typename TRAIT > struct list_from_python_list
{
    list_from_python_list( )
	{ 
		py::converter::registry::push_back( &convertible, &construct, py::type_id< LIST >( ) ); 
    }
	
    static void* convertible( PyObject* obj_ptr )
	{
		void* result = NULL;
		if( PyList_Check( obj_ptr ) && TRAIT::checkType( PyList_GET_ITEM( obj_ptr, 0 ) ) )
			result = obj_ptr;
		
		return result;
    }
	
    static void construct( PyObject* obj_ptr, py::converter::rvalue_from_python_stage1_data* data )
	{
		PyObject* value = PyList_AsTuple( obj_ptr );
		if( !value )
			py::throw_error_already_set( );
	
		void* storage = ( ( py::converter::rvalue_from_python_storage< LIST >* ) data )->storage.bytes;
		LIST* l = new ( storage ) LIST;
		for ( int i=0; i<PyTuple_Size( value ); ++i )
			l->push_back( py::extract< typename LIST::value_type >( PyTuple_GetItem( value, i ) )() );

		data->convertible = storage;
    }
};

void py_pcos_property()
{
    py::to_python_converter< pcos::int_list, list_to_python_list< pcos::int_list, PyConvertTrait< int > > >( );	
    list_from_python_list< pcos::int_list, PyConvertTrait< int > >();
    py::to_python_converter< pcos::uint_list, list_to_python_list< pcos::uint_list, PyConvertTrait< int > > >( );	
    list_from_python_list< pcos::uint_list, PyConvertTrait< int > >();
    py::to_python_converter< pcos::double_list, list_to_python_list< pcos::double_list, PyConvertTrait< double > > >( );	
    list_from_python_list< pcos::double_list, PyConvertTrait< double > >();

	py::class_< pcos::property, py::bases<pcos::isubject>, boost::shared_ptr< pcos::property > >( "property", py::init< pcos::key >() )
		.def( "get_key", &pcos::property::get_key )
		.def( py::self == py::self )
		.def( py::self != py::self )
		.def( "set_always_notify", &pcos::property::set_always_notify )
		.def( "valid", &pcos::property::valid )
		.def( "set", set_double )
		.def( "set", set_bool )
		.def( "set", set_int )
		.def( "set", set_double_list )
		.def( "set", set_int_list )
		.def( "set", set_string )
		.def( "set", set_wstring )
		.def( "set", set_string_list )
		.def( "set", set_wstring_list )
		.def( "set_from_string", &pcos::property::set_from_string )
		.def( "value_as_wstring", value_wstring )
		.def( "value_as_string", value_string )
		.def( "value_as_wstring_list", value_wstring_list )
		.def( "value_as_string_list", value_string_list )
		.def( "value_as_int", value_int )
		.def( "value_as_unsigned_int", value_unsigned_int )
		.def( "value_as_double", value_double )
		.def( "value_as_int_list", value_int_list )
		.def( "value_as_uint_list", value_uint_list )
		.def( "value_as_double_list", value_double_list )
		.def( "value_as_bool", value_bool )
		.def( "is_a_wstring", is_a_wstring )
		.def( "is_a_string", is_a_string )
		.def( "is_a_wstring_list", is_a_wstring_list )
		.def( "is_a_string_list", is_a_string_list )
		.def( "is_a_int", is_a_int )
		.def( "is_a_unsigned_int", is_a_unsigned_int )
		.def( "is_a_double", is_a_double )
		.def( "is_a_int_list", is_a_int_list )
		.def( "is_a_uint_list", is_a_uint_list )
		.def( "is_a_double_list", is_a_double_list )
		.def( "is_a_bool", is_a_bool )
        .def( "get_property", &pcos::property::get_property_with_string, py::return_value_policy< py::return_by_value >() )
        .def( "get_property", &pcos::property::get_property_with_key, py::return_value_policy< py::return_by_value >() )
        .def( "get_keys", &pcos::property::get_keys )
//        .def( "append", &pcos::property::append )
//        .def( "remove", &pcos::property::remove )
		.def( "accept", &pcos::property::accept )
		.def( "clone", &pcos::property::clone, py::return_value_policy< py::manage_new_object >() );
}

} } }
