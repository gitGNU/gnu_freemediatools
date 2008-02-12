
// sg - A scene graph representation.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifdef HAVE_CONFIG_H
#include <openlibraries_global_config.hpp>
#endif

#include <openobjectlib/sg/meshes.hpp>

namespace opl = olib::openpluginlib;

namespace olib { namespace openobjectlib { namespace sg {

mesh::mesh( )
	: prim_type_( mesh::UNKNOWN )
{
	insert( L"DEF",			mesh::field_type( opl::v_string_typename,	opl::v_string( ) ) );
	insert( L"set_index",	mesh::field_type( opl::m_int32_typename,	opl::m_int32( ) ) );
	insert( L"coord",		mesh::field_type( v_node_typename,			v_attribute_array_ptr( ) ) );
	insert( L"normal",		mesh::field_type( v_node_typename,			v_attribute_array_ptr( ) ) );
	insert( L"color",		mesh::field_type( v_node_typename,			v_attribute_array_ptr( ) ) );
	insert( L"uv",			mesh::field_type( m_node_typename,			mv_uv_set_ptr( ) ) );
	insert( L"attrib",		mesh::field_type( m_node_typename,			mv_attribute_array_ptr( ) ) );
	insert( L"index",		mesh::field_type( opl::m_int32_typename,	opl::m_int32( ) ) );
	insert( L"count",		mesh::field_type( opl::m_int32_typename,	opl::m_int32( ) ) );
}

mesh::~mesh( )
{ }

multi_index_mesh::multi_index_mesh( )
{
	insert( L"colorIndex",		multi_index_mesh::field_type( opl::m_int32_typename, opl::m_int32( ) ) );
	insert( L"normalIndex",		multi_index_mesh::field_type( opl::m_int32_typename, opl::m_int32( ) ) );
	insert( L"texCoordIndex",	multi_index_mesh::field_type( opl::m_int32_typename, opl::m_int32( ) ) );
	insert( L"ccw",				multi_index_mesh::field_type( opl::v_bool_typename,	 opl::v_bool( true ) ) );
	insert( L"convex",			multi_index_mesh::field_type( opl::v_bool_typename,	 opl::v_bool( true ) ) );
	insert( L"colorPerVertex",	multi_index_mesh::field_type( opl::v_bool_typename,	 opl::v_bool( true ) ) );
	insert( L"normalPerVertex",	multi_index_mesh::field_type( opl::v_bool_typename,	 opl::v_bool( true ) ) );
	insert( L"solid",			multi_index_mesh::field_type( opl::v_bool_typename,	 opl::v_bool( true ) ) );
	insert( L"creaseAngle",		multi_index_mesh::field_type( opl::v_float_typename, opl::v_float( 3.14f ) ) );
}

multi_index_mesh::~multi_index_mesh( )
{ }

uv_set::uv_set( )
{
	insert( L"uv",			uv_set::field_type( m_node_typename,		v_attribute_array_ptr( ) ) );
	insert( L"mode",		uv_set::field_type( opl::v_string_typename, opl::v_string( L"SPHERE" ) ) );
	insert( L"parameter",	uv_set::field_type( opl::m_float_typename,  opl::m_float( ) ) );
	insert( L"auto",		uv_set::field_type( opl::v_bool_typename,	opl::v_bool( false ) ) );
}

uv_set::~uv_set( )
{ }

attribute_array::attribute_array( )
{
	insert( L"value",		attribute_array::field_type( opl::m_float_typename,  opl::m_float( ) ) );
	insert( L"name",		attribute_array::field_type( opl::v_string_typename, opl::v_string( L"VERTEX" ) ) );
	insert( L"components",	attribute_array::field_type( opl::v_int32_typename,  opl::v_int32( 3 ) ) );
}

attribute_array::~attribute_array( )
{ }

} } }
