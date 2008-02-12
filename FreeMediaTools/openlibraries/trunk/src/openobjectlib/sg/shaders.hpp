
// sg - A scene graph representation.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef SHADERS_INC_
#define SHADERS_INC_

#include <openobjectlib/sg/core.hpp>

namespace olib { namespace openobjectlib { namespace sg {

class SG_DECLSPEC shader : public node
{
public:
	explicit shader( );
	virtual ~shader( );

public:
	virtual bool GL_2_0_usable( ) const = 0;

public:
	bool insert_param( const key_type& key, const field_type& value );

	const_iterator begin_param( )	{ return param_.begin( ); }
	const_iterator end_param( )		{ return param_.end( );	  }
	
	template<class U>
	U value_param( const key_type& key )
	{ return *boost::any_cast<U>( &param_[ key ].second ); }

	template<class U>
	U* pointer_param( const key_type& key )
	{ return boost::any_cast<U>( &param_[ key ].second ); }

	template<class U>
	const U* pointer_param( const key_type& key ) const
	{
		const_iterator I = param_.find( key );
		if( I != param_.end( ) )
			return boost::any_cast<U>( &I->second.second );
						
		return static_cast<const U*>( 0 );
	}
	
	bool erase( const key_type& key )
	{ return param_.erase( key ) != 0; }

private:
	container param_;
};

struct SG_DECLSPEC composed_shader : public shader
{
	explicit composed_shader( );
	virtual ~composed_shader( );
	
	virtual bool GL_2_0_usable( ) const;
};

struct SG_DECLSPEC packaged_shader : public shader
{
	explicit packaged_shader( );
	virtual ~packaged_shader( );
};

struct SG_DECLSPEC program_shader : public node
{
	explicit program_shader( );
	virtual ~program_shader( );
	
	virtual bool GL_2_0_usable( ) const;
};

struct SG_DECLSPEC shader_part : public node
{
	explicit shader_part( );
	virtual ~shader_part( );
};

class SG_DECLSPEC shader_program : public shader
{
public:
	explicit shader_program( );
	virtual ~shader_program( );
};

typedef boost::shared_ptr<shader>			shader_ptr;
typedef boost::shared_ptr<composed_shader>	composed_shader_ptr;
typedef boost::shared_ptr<packaged_shader>	packaged_shader_ptr;
typedef boost::shared_ptr<program_shader>	program_shader_ptr;
typedef boost::shared_ptr<shader_part>		shader_part_ptr;
typedef boost::shared_ptr<shader_program>	shader_program_ptr;

typedef openpluginlib::value_property<shader_ptr>			v_shader_ptr;
typedef openpluginlib::value_property<composed_shader_ptr>	v_composed_shader_ptr;
typedef openpluginlib::value_property<packaged_shader_ptr>	v_packaged_shader_ptr;
typedef openpluginlib::value_property<program_shader_ptr>	v_program_shader_ptr;

typedef openpluginlib::multi_value_property<shader_program_ptr>	 m_shader_program_ptr;
typedef openpluginlib::multi_value_property<shader_part_ptr>	 m_shader_part_ptr;

} } }

#endif
