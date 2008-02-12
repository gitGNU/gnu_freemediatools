
// sg - A scene graph representation.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifdef HAVE_CONFIG_H
#include <openlibraries_global_config.hpp>
#endif

#include <openobjectlib/sg/shaders.hpp>

namespace opl = olib::openpluginlib;

namespace olib { namespace openobjectlib { namespace sg {

shader::shader( )
{ };

shader::~shader( )
{ }

bool shader::insert_param( const shader::key_type& key, const shader::field_type& value )
{
	return param_.insert( value_type( key, value ) ).second;
}

composed_shader::composed_shader( )
{
	insert( L"DEF",			composed_shader::field_type( opl::v_string_typename, opl::v_string( ) ) );
	insert( L"parts",		composed_shader::field_type( m_node_typename,		 m_shader_part_ptr( ) ) );
	insert( L"language",	composed_shader::field_type( opl::v_string_typename, opl::v_string( ) ) );
}

composed_shader::~composed_shader( )
{ }

bool composed_shader::GL_2_0_usable( ) const
{ return true; }

packaged_shader::packaged_shader( )
{
	insert( L"DEF",			packaged_shader::field_type( opl::v_string_typename, opl::v_string( ) ) );
	insert( L"url",			packaged_shader::field_type( opl::m_url_typename,	 opl::m_url( ) ) );
	insert( L"language",	packaged_shader::field_type( opl::v_string_typename, opl::v_string( ) ) );
}

packaged_shader::~packaged_shader( )
{ }

program_shader::program_shader( )
{
	insert( L"DEF",			program_shader::field_type( opl::v_string_typename, opl::v_string( ) ) );
	insert( L"programs",	program_shader::field_type( m_node_typename,		m_shader_program_ptr( ) ) );
	insert( L"language",	program_shader::field_type( opl::v_string_typename, opl::v_string( ) ) );
}

program_shader::~program_shader( )
{ }

bool program_shader::GL_2_0_usable( ) const
{ return false; }

shader_part::shader_part( )
{
	insert( L"DEF",      shader_part::field_type( opl::v_string_typename, opl::v_string( ) ) );
	insert( L"url",      shader_part::field_type( opl::m_url_typename,	  opl::m_url( ) ) );
	insert( L"type",     shader_part::field_type( opl::v_string_typename, opl::v_string( L"VERTEX" ) ) );
	insert( L"source",   shader_part::field_type( opl::v_string_typename, opl::v_string( ) ) );
	insert( L"embedded", shader_part::field_type( opl::v_bool_typename,   opl::v_bool( false ) ) );
}

shader_part::~shader_part( )
{ }

shader_program::shader_program( )
{
	insert( L"DEF",  shader_part::field_type( opl::v_string_typename, opl::v_string( ) ) );
	insert( L"url",  shader_part::field_type( opl::m_url_typename,	  opl::m_url( ) ) );
	insert( L"type", shader_part::field_type( opl::v_string_typename, opl::v_string( L"VERTEX" ) ) );
}

shader_program::~shader_program( )
{ }

} } }
