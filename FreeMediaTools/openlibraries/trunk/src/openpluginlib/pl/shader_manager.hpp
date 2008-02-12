
// openpluginlib - An plugin interface to openlibraries.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef SHADER_MANAGER_INC_
#define SHADER_MANAGER_INC_

#ifdef _MSC_VER
#pragma warning ( push )
#pragma warning ( disable:4251 )
#endif

#ifdef HAVE_CONFIG_H
#include <openlibraries_global_config.hpp>
#endif

#include <cassert>

#include <boost/filesystem/path.hpp>
#include <boost/shared_ptr.hpp>

#include <GL/glew.h>

#if defined ( HAVE_CG_RUNTIME )
#include <Cg/cg.h>
#include <Cg/cgGL.h>
#endif

#include <openpluginlib/pl/config.hpp>
#include <openpluginlib/pl/property.hpp>

#ifdef HAVE_GL_GLEW_H

namespace olib { namespace openpluginlib {

typedef std::basic_string<GLcharARB> shader_string;

class OPENPLUGINLIB_DECLSPEC shader_object_base
{
public:
	explicit shader_object_base( ) { }
	virtual ~shader_object_base( ) { }
	
	virtual bool bind_uniform( v_float& v, const wstring& name, bool vertex = false ) const				= 0;
	virtual bool bind_uniform( v_int32& v, const wstring& name, bool vertex = false ) const				= 0;
	virtual bool bind_uniform( v_vec2f& v, const wstring& name, bool vertex = false ) const				= 0;
	virtual bool bind_uniform( v_vec4f& v, const wstring& name, bool vertex = false ) const				= 0;
	virtual bool bind_uniform( v_matrix4f& v, const wstring& name, bool vertex = false ) const			= 0;
	virtual bool bind_texture( v_int32& v, const wstring& name, bool vertex = false ) const				= 0;
	virtual bool bind_matrix_parameter( v_matrix4f& v, const wstring& name, bool vertex = false ) const	= 0;
	
	virtual void bind_shader( ) const	= 0;
	virtual void unbind_shader( ) const	= 0;
	
	virtual bool is_valid( ) const = 0;
	
	virtual void fini( ) = 0;
	
	virtual bool retry( ) const { return false; }
};

class OPENPLUGINLIB_DECLSPEC GLSL_shader_object : public shader_object_base
{
public:
	explicit GLSL_shader_object( )
		: program_( 0 )
		, vertex_( 0 )
		, fragment_( 0 )
		, valid_( false )
	{ }
	
public:
	GLhandleARB& program( )			{ return program_; }
	GLhandleARB  program( )  const	{ return program_; }
	GLhandleARB& vertex( )			{ return vertex_; }
	GLhandleARB  vertex( )	 const	{ return vertex_; }
	GLhandleARB& fragment( )		{ return fragment_; }
	GLhandleARB  fragment( ) const	{ return fragment_; }
	bool&		 valid( )			{ return valid_; }
	bool		 valid( )	 const  { return valid_; }

public:
	virtual bool bind_uniform( v_float& v, const wstring& name, bool vertex = false ) const;
	virtual bool bind_uniform( v_int32& v, const wstring& name, bool vertex = false ) const;
	virtual bool bind_uniform( v_vec2f& v, const wstring& name, bool vertex = false ) const;
	virtual bool bind_uniform( v_vec4f& v, const wstring& name, bool vertex = false ) const;
	virtual bool bind_uniform( v_matrix4f& v, const wstring& name, bool vertex = false ) const;
	virtual bool bind_texture( v_int32& v, const wstring& name, bool vertex = false ) const;
	virtual bool bind_matrix_parameter( v_matrix4f& /*v*/, const wstring& /*name*/, bool /*vertex*/ = false ) const { return false; }

	virtual void bind_shader( ) const;
	virtual void unbind_shader( ) const;

	virtual bool is_valid( ) const;
	
	virtual void fini( );

private:
	GLhandleARB program_;
	GLhandleARB vertex_;
	GLhandleARB fragment_;
	bool valid_;
};

#if defined ( HAVE_CG_RUNTIME )
class OPENPLUGINLIB_DECLSPEC Cg_shader_object : public shader_object_base
{
public:
	explicit Cg_shader_object( )
		: vert_( 0 )
		, frag_( 0 )
		, v_profile_( static_cast<CGprofile>( 0 ) )
		, f_profile_( static_cast<CGprofile>( 0 ) )
		, valid_( false )
	{ }
	
public:
	CGprogram& vert( )				{ return vert_; }
	CGprogram  vert( ) const		{ return vert_; }
	CGprogram& frag( )				{ return frag_; }
	CGprogram  frag( ) const		{ return frag_; }
	CGprofile& v_profile( )			{ return v_profile_; }
	CGprofile  v_profile( ) const	{ return v_profile_; }
	CGprofile& f_profile( )			{ return f_profile_; }
	CGprofile  f_profile( ) const	{ return f_profile_; }
	bool&	   valid( )				{ return valid_; }
	bool	   valid( ) const		{ return valid_; }

public:
	virtual bool bind_uniform( v_float& v, const wstring& name, bool vertex = false ) const;
	virtual bool bind_uniform( v_int32& v, const wstring& name, bool vertex = false ) const;
	virtual bool bind_uniform( v_vec2f& v, const wstring& name, bool vertex = false ) const;
	virtual bool bind_uniform( v_vec4f& v, const wstring& name, bool vertex = false ) const;
	virtual bool bind_uniform( v_matrix4f& v, const wstring& name, bool vertex = false ) const;
	virtual bool bind_texture( v_int32& v, const wstring& name, bool vertex = false ) const;
	virtual bool bind_matrix_parameter( v_matrix4f& v, const wstring& name, bool vertex = false ) const;

	virtual void bind_shader( ) const;
	virtual void unbind_shader( ) const;

	virtual bool is_valid( ) const;
	
	virtual void fini( );

private:
	CGprogram vert_, frag_;
	CGprofile v_profile_, f_profile_;
	bool valid_;
};
#endif

class OPENPLUGINLIB_DECLSPEC shader_object
{
public:
	explicit shader_object( )
		: retry_( true )
	{ }

	virtual ~shader_object( ) { }

	bool init( const std::string& v_uri, const std::string& f_uri, bool prefix = true );
	void fini( );
	
public:	
	bool bind_uniform( v_float& v, const wstring& name, bool vertex = false ) const;
	bool bind_uniform( v_int32& v, const wstring& name, bool vertex = false ) const;
	bool bind_uniform( v_vec2f& v, const wstring& name, bool vertex = false ) const;
	bool bind_uniform( v_vec4f& v, const wstring& name, bool vertex = false ) const;
	bool bind_uniform( v_matrix4f& v, const wstring& name, bool vertex = false ) const;
	bool bind_texture( v_int32& v, const wstring& name, bool vertex = false ) const;
	bool bind_matrix_parameter( v_matrix4f& v, const wstring& name, bool vertex = false ) const;
	
	void bind_shader( ) const;
	void unbind_shader( ) const;
	
public:
	bool is_valid( ) const;
	
	bool get_retry( ) const
	{ return retry_; }
	void set_retry( bool retry )
	{ retry_ = retry; }

private:
	typedef boost::shared_ptr<shader_object_base> shader_object_base_ptr;
	shader_object_base_ptr sbase_;
	bool retry_;
};

class OPENPLUGINLIB_DECLSPEC shader_manager
{
private:
	static shader_manager* revive_if_dead( )
	{
		void* p = new char [ sizeof( shader_manager ) ];

		new( p ) shader_manager;
		
		return static_cast<shader_manager*>( p );
	}

	static void destroy( void )
	{
		instance_->~shader_manager( );

		delete[ ] ( ( char* ) instance_ );
	}

public:	
	static shader_manager& instance( )
	{
		if( !instance_ )
		{
			if( destroyed_ )
			{
				destroyed_     = false;
				was_destroyed_ = true;
			}

			instance_ = revive_if_dead( );

			if( !was_destroyed_ )
				std::atexit( annihilate );
		}
		
		return *instance_;
	}

	static void annihilate( )
	{
		if( instance_ )
		{
			assert( !destroyed_ );
			destroy( );
			destroyed_ = true;
			instance_ = 0;
		}
	}
	
public:
	shader_string	open_shader_file( const string& path, bool prefix = true );
	bool			create_GLSL_shader( const shader_string& vertex, const shader_string& fragment, GLSL_shader_object& shader );
	void			delete_GLSL_shader( GLSL_shader_object& shader );

#if defined ( HAVE_CG_RUNTIME )
	void create_Cg_context( );
	void destroy_Cg_context( );
	CGcontext get_Cg_context( );
	
	bool create_Cg_shader( const std::string& vertex, const std::string& fragment, Cg_shader_object& shader );
	bool delete_Cg_shader( Cg_shader_object& shader );
#endif

	bool create_shader_object( const std::string& v_uri, const std::string& f_uri, shader_object& sobj, bool prefix = true );

public:
	bool bind_GL_uniform( GLhandleARB program, v_float& v, const wstring& name ) const;
	bool bind_GL_uniform( GLhandleARB program, v_int32& v, const wstring& name ) const;
	bool bind_GL_uniform( GLhandleARB program, v_vec2f& v, const wstring& name ) const;
	bool bind_GL_uniform( GLhandleARB program, v_vec4f& v, const wstring& name ) const;
	bool bind_GL_uniform( GLhandleARB program, v_matrix4f& v, const wstring& name ) const;

private:
	explicit shader_manager( );
	~shader_manager( );
	shader_manager( const shader_manager& );
	shader_manager& operator=( const shader_manager& );

private:
#if defined ( HAVE_CG_RUNTIME )
	CGcontext cg_context_;
	bool is_cg_initialised_;
#endif
	static shader_manager* instance_;
	static bool destroyed_;
	static bool was_destroyed_;
};

OPENPLUGINLIB_DECLSPEC shader_manager& shader_manager_instance( );

} }

#endif

#ifdef _MSC_VER
#pragma warning ( pop )
#endif

#endif
