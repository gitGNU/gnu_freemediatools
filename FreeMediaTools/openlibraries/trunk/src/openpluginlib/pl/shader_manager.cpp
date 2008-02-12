
// openpluginlib - An plugin interface to openlibraries.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#include <fstream>

#include <openpluginlib/pl/openpluginlib.hpp>
#include <openpluginlib/pl/shader_manager.hpp>
#include <openpluginlib/pl/utf8_utils.hpp>
#include <openpluginlib/pl/cg.hpp>

namespace fs = boost::filesystem;

#ifdef HAVE_GL_GLEW_H

namespace olib { namespace openpluginlib {

shader_manager*	shader_manager::instance_		= 0;
bool			shader_manager::destroyed_		= false;
bool			shader_manager::was_destroyed_	= false;

namespace
{
#if 0
	string GL_2_0_shader_info_log( GLhandleARB shader )
	{
//		GLint length;
//		glGetProgramivARB( program, GL_INFO_LOG_LENGTH_ARB, &length );
		
		std::vector<char> log;
		log.resize( 2048 );
		
		glGetShaderInfoLog( shader, 2048, NULL, &log[ 0 ] );
		
		return string( log.begin( ), log.end( ) );
	}

	string GL_2_0_info_log( GLhandleARB program )
	{
//		GLint length;
//		glGetProgramivARB( program, GL_INFO_LOG_LENGTH_ARB, &length );
		
		std::vector<char> log;
		log.resize( 2048 );
		
		glGetInfoLogARB( program, 2048, NULL, &log[ 0 ] );
		
		return string( log.begin( ), log.end( ) );
	}
#endif

	// GLSL related utilities.	
	GLhandleARB create_GLSL_program_object( )
	{
		return glCreateProgramObjectARB( );
	}
		
	GLhandleARB create_GLSL_shader_object( GLenum type )
	{ 
		return glCreateShaderObjectARB( type );
	}
		
	void download_GLSL_shader_code_to_gpu( GLhandleARB shader, const shader_string& code )
	{
		const GLcharARB* src[ 1 ] = { code.c_str( ) };
		
		glShaderSourceARB( shader, 1, &src[ 0 ], 0 );
	}

	bool compile_GLSL_shader( GLhandleARB shader )
	{
		GLint compiled;

		glCompileShaderARB( shader );
		glGetObjectParameterivARB( shader, GL_OBJECT_COMPILE_STATUS_ARB, &compiled );

		return compiled != 0;
	}
		
	bool link_GLSL_program( GLhandleARB program, GLhandleARB vertex, GLhandleARB fragment )
	{
		GLint linked;

		glAttachObjectARB( program, vertex );
		glAttachObjectARB( program, fragment );

		glLinkProgramARB( program );
		glGetObjectParameterivARB( program, GL_OBJECT_LINK_STATUS_ARB, &linked );

		return linked != 0;
	}
	
	template<class prop_type>
	GLint get_GL_uniform_location( GLhandleARB program, prop_type& v, const wstring& name )
	{
		bind_info& bi = v.bi( );

		GLint location = bi.value<GLint>( L"GL_location", -1 );
		if( location == -1 )
		{
			location = glGetUniformLocationARB( program, to_string( name ).c_str( ) );
			bi.insert( L"GL_location", location );
		}

		return location;
	}
	
#ifdef HAVE_CG_RUNTIME
	template<class prop_type>
	CGparameter get_Cg_uniform_parameter( CGprogram program, prop_type& v, const wstring& name )
	{
		bind_info& bi = v.bi( );

		CGparameter param = bi.value<CGparameter>( L"Cg_parameter", 0 );
		if( !param )
		{
			param = cgGetNamedParameter( program, to_string( name ).c_str( ) );
			bi.insert( L"Cg_parameter", param );
		}

		return param;
	}
#endif
}

void GLSL_shader_object::fini( )
{
	shader_manager_instance( ).delete_GLSL_shader( *this );
}

#if defined ( HAVE_CG_RUNTIME )
void Cg_shader_object::fini( )
{
	shader_manager_instance( ).delete_Cg_shader( *this );
}
#endif

shader_manager::shader_manager( )
#if defined ( HAVE_CG_RUNTIME )
	: cg_context_( 0 )
	, is_cg_initialised_( false )
#endif
{
#if defined ( HAVE_CG_RUNTIME )
	is_cg_initialised_ = init_Cg( );
#endif
}

shader_manager::~shader_manager( )
{
	uninit_Cg( );
}

shader_string shader_manager::open_shader_file( const string& path, bool prefix )
{
	string p = path;

#ifdef WIN32
	namespace opl = olib::openpluginlib;
	
	if( prefix )
		p = opl::shaders_path( ) + string( "/" ) + p;
#else
	if( prefix )
		p = string( OPENEFFECTSLIB_SHADERS ) + string( "/" ) + p;
#endif

	std::ifstream file( p.c_str( ) );
	if( !file.is_open( ) ) return shader_string( );

	return shader_string( ( std::istreambuf_iterator<GLcharARB>( file ) ), std::istreambuf_iterator<GLcharARB>( ) );
}

bool shader_manager::create_GLSL_shader( const shader_string& vertex, const shader_string& fragment, GLSL_shader_object& shader )
{
	if( !GLEW_VERSION_2_0 && !( GLEW_ARB_shading_language_100 && GLEW_ARB_shader_objects && GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader ) )
		return false;
		
	shader.program( )  = create_GLSL_program_object( );
	shader.vertex( )   = create_GLSL_shader_object( GL_VERTEX_SHADER_ARB );
	shader.fragment( ) = create_GLSL_shader_object( GL_FRAGMENT_SHADER_ARB );
	shader.valid( )	   = false;
	
	if( glIsShader && ( !glIsShader( shader.vertex( ) ) || !glIsShader( shader.fragment( ) ) ) )
		return false;
	
	download_GLSL_shader_code_to_gpu( shader.vertex( ), vertex );
	download_GLSL_shader_code_to_gpu( shader.fragment( ), fragment );
		
	if( compile_GLSL_shader( shader.vertex( ) ) && compile_GLSL_shader( shader.fragment( ) ) && link_GLSL_program( shader.program( ), shader.vertex( ), shader.fragment( ) ) )
		shader.valid( ) = true;

#if 0
	string vert_log = GL_2_0_info_log( shader.vertex( ) );
	string frag_log = GL_2_0_info_log( shader.fragment( ) );
	string prog_log = GL_2_0_info_log( shader.program( ) );
#endif

	return shader.valid( );
}

void shader_manager::delete_GLSL_shader( GLSL_shader_object& shader )
{
	glUseProgramObjectARB( 0 );
	
	glDeleteObjectARB( shader.vertex( ) );
	glDeleteObjectARB( shader.fragment( ) );
	glDeleteObjectARB( shader.program( ) );
}

#if defined ( HAVE_CG_RUNTIME )
void shader_manager::create_Cg_context( )
{
	if( is_cg_initialised_ )
		cg_context_ = cgCreateContext( );
}

void shader_manager::destroy_Cg_context( )
{
	if( is_cg_initialised_ && cgIsContext( cg_context_ ) )
		cgDestroyContext( cg_context_ );
}

CGcontext shader_manager::get_Cg_context( )
{
	return cg_context_;
}

bool shader_manager::create_Cg_shader( const std::string& vertex, const std::string& fragment, Cg_shader_object& shader )
{
	if( !is_cg_initialised_ )
		return false;
		
	cgGLRegisterStates( cg_context_ );
	
	CGprofile v_profile	= cgGLGetLatestProfile( CG_GL_VERTEX );
	CGprofile f_profile	= cgGLGetLatestProfile( CG_GL_FRAGMENT );
	
	cgGLSetOptimalOptions( v_profile );
	cgGLSetOptimalOptions( f_profile );
	
	CGprogram vert = cgCreateProgram( cg_context_, CG_SOURCE, vertex.c_str( ), v_profile, "main", 0 );
	CGprogram frag = cgCreateProgram( cg_context_, CG_SOURCE, fragment.c_str( ), f_profile, "main", 0 );

	cgGLLoadProgram( vert );
	if( cgGetError( ) != CG_NO_ERROR )
		return false;
	
	cgGLLoadProgram( frag );
	if( cgGetError( ) != CG_NO_ERROR )
		return false;

	shader.v_profile( ) = v_profile;
	shader.f_profile( ) = f_profile;
	shader.vert( )  = vert;
	shader.frag( )	= frag;
	shader.valid( ) = true; // good to try to bind... need to sort which errors *are* errors - Goncalo

	cgGLSetManageTextureParameters( cg_context_, CG_FALSE );
	
	return shader.valid( );
}

bool shader_manager::delete_Cg_shader( Cg_shader_object& shader )
{
	if( !is_cg_initialised_ )
		return false;

	cgDestroyProgram( shader.vert( ) );
	cgDestroyProgram( shader.frag( ) );

	return true;
}

#endif

bool shader_manager::create_shader_object( const std::string& v_uri, const std::string& f_uri, shader_object& sobj, bool prefix )
{
	return sobj.init( v_uri, f_uri, prefix );
}

bool shader_manager::bind_GL_uniform( GLhandleARB program, v_float& v, const wstring& name ) const
{
	GLint location = get_GL_uniform_location( program, v, name );
	if( location == -1 ) return false;
	
	glUniform1fARB( location, v.value( ) );
	
	return true;
}

bool shader_manager::bind_GL_uniform( GLhandleARB program, v_int32& v, const wstring& name ) const
{
	GLint location = get_GL_uniform_location( program, v, name );
	if( location == -1 ) return false;
	
	glUniform1iARB( location, v.value( ) );
	
	return true;
}

bool shader_manager::bind_GL_uniform( GLhandleARB program, v_vec2f& v, const wstring& name ) const
{
	GLint location = get_GL_uniform_location( program, v, name );
	if( location == -1 ) return false;
	
	glUniform2fvARB( location, 1, &v.value( )[ 0 ] );
	
	return true;
}

bool shader_manager::bind_GL_uniform( GLhandleARB program, v_vec4f& v, const wstring& name ) const
{
	GLint location = get_GL_uniform_location( program, v, name );
	if( location == -1 ) return false;
	
	glUniform4fvARB( location, 1, &v.value( )[ 0 ] );
	
	return true;
}

bool shader_manager::bind_GL_uniform( GLhandleARB program, v_matrix4f& v, const wstring& name ) const
{
	GLint location = get_GL_uniform_location( program, v, name );
	if( location == -1 ) return false;
	
	glUniformMatrix4fvARB( location, 1, false, &v.value( )[ 0 ] );
	
	return true;
}

shader_manager& shader_manager_instance( )
{ return shader_manager::instance( ); }

bool GLSL_shader_object::bind_uniform( v_float& v, const wstring& name, bool /*vertex*/ ) const
{
	return shader_manager_instance( ).bind_GL_uniform( program( ), v, name );
}

bool GLSL_shader_object::bind_uniform( v_int32& v, const wstring& name, bool /*vertex*/ ) const
{
	return shader_manager_instance( ).bind_GL_uniform( program( ), v, name );
}

bool GLSL_shader_object::bind_uniform( v_vec2f& v, const wstring& name, bool /*vertex*/ ) const
{
	return shader_manager_instance( ).bind_GL_uniform( program( ), v, name );
}

bool GLSL_shader_object::bind_uniform( v_vec4f& v, const wstring& name, bool /*vertex*/ ) const
{
	return shader_manager_instance( ).bind_GL_uniform( program( ), v, name );
}

bool GLSL_shader_object::bind_uniform( v_matrix4f& v, const wstring& name, bool /*vertex*/ ) const
{
	return shader_manager_instance( ).bind_GL_uniform( program( ), v, name );
}

bool GLSL_shader_object::bind_texture( v_int32& v, const wstring& name, bool /*vertex*/ ) const
{
	return shader_manager_instance( ).bind_GL_uniform( program( ), v, name );
}

void GLSL_shader_object::bind_shader( ) const
{
	glUseProgramObjectARB( program( ) );
}

void GLSL_shader_object::unbind_shader( ) const
{
	glUseProgramObjectARB( 0 );
}

bool GLSL_shader_object::is_valid( ) const
{
	return valid( );
}

#if defined ( HAVE_CG_RUNTIME )
bool Cg_shader_object::bind_uniform( v_float& v, const wstring& name, bool vertex ) const
{
	CGparameter param = get_Cg_uniform_parameter( vertex ? vert( ) : frag( ), v, name );
	if( !param ) return false;
	
	cgSetParameter1f( param, v.value( ) );
	
	return true;
}

bool Cg_shader_object::bind_uniform( v_int32& /*v*/, const wstring& /*name*/, bool /*vertex*/ ) const
{
	return false;
}

bool Cg_shader_object::bind_uniform( v_vec2f& v, const wstring& name, bool vertex ) const
{
	CGparameter param = get_Cg_uniform_parameter( vertex ? vert( ) : frag( ), v, name );
	if( !param ) return false;
	
	cgSetParameter2fv( param, &v.value( )[ 0 ] );

	return true;
}

bool Cg_shader_object::bind_uniform( v_vec4f& v, const wstring& name, bool vertex ) const
{
	CGparameter param = get_Cg_uniform_parameter( vertex ? vert( ) : frag( ), v, name );
	if( !param ) return false;
	
	cgSetParameter4fv( param, &v.value( )[ 0 ] );

	return true;
}

bool Cg_shader_object::bind_uniform( v_matrix4f& v, const wstring& name, bool vertex ) const
{
	CGparameter param = get_Cg_uniform_parameter( vertex ? vert( ) : frag( ), v, name );
	if( !param ) return false;

	cgSetMatrixParameterfc( param, &v.get( )[ 0 ] );
	
	return true;
}

bool Cg_shader_object::bind_texture( v_int32& v, const wstring& name, bool vertex ) const
{
	CGparameter param = get_Cg_uniform_parameter( vertex ? vert( ) : frag( ), v, name );
	if( !param ) return false;
	
	cgGLSetTextureParameter( param, v.value( ) );
	cgGLEnableTextureParameter( param );

	return true;
}

bool Cg_shader_object::bind_matrix_parameter( v_matrix4f& v, const wstring& name, bool vertex ) const
{
	CGparameter param = get_Cg_uniform_parameter( vertex ? vert( ) : frag( ), v, name );
	if( !param ) return false;

	cgGLSetStateMatrixParameter( param, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY );

	return true;
}

void Cg_shader_object::bind_shader( ) const
{
	cgGLEnableProfile( v_profile( ) );
	cgGLEnableProfile( f_profile( ) );

	cgGLBindProgram( vert( ) );
	cgGLBindProgram( frag( ) );
}

void Cg_shader_object::unbind_shader( ) const
{
	cgGLUnbindProgram( v_profile( ) );
	cgGLUnbindProgram( f_profile( ) );
	
	cgGLDisableProfile( v_profile( ) );
	cgGLDisableProfile( f_profile( ) );
}

bool Cg_shader_object::is_valid( ) const
{
	return valid( );
}
#endif

bool shader_object::init( const std::string& v_uri, const std::string& f_uri, bool prefix )
{
	set_retry( false );

	if( v_uri.find( "glsl:", 0, 5 ) != std::string::npos && f_uri.find( "glsl:", 0, 5 ) != std::string::npos )
	{
		GLSL_shader_object* obj( new GLSL_shader_object );
		
		shader_string vert, frag;
		vert = shader_manager_instance( ).open_shader_file( v_uri.substr( 5 ), prefix );
		frag = shader_manager_instance( ).open_shader_file( f_uri.substr( 5 ), prefix );
		
		if( shader_manager_instance( ).create_GLSL_shader( vert, frag, *obj ) )
		{
			sbase_.reset( obj );
			return true;
		}
	}
#if defined ( HAVE_CG_RUNTIME )
	else if( v_uri.find( "cg:", 0, 3 ) != std::string::npos && f_uri.find( "cg:", 0, 3 ) != std::string::npos )
	{
		Cg_shader_object* obj( new Cg_shader_object( ) );
		
		shader_string vert, frag;
		vert = shader_manager_instance( ).open_shader_file( v_uri.substr( 3 ), prefix );
		frag = shader_manager_instance( ).open_shader_file( f_uri.substr( 3 ), prefix );
		
		if( shader_manager_instance( ).create_Cg_shader( vert, frag, *obj ) )
		{
			sbase_.reset( obj );
			return true;
		}
	}
#endif

	return false;
}

void shader_object::fini( )
{
	if( sbase_ )
	    sbase_->fini( );
}

bool shader_object::bind_uniform( v_float& v, const wstring& name, bool vertex ) const
{
	if( sbase_ )
		return sbase_->bind_uniform( v, name, vertex );

	return false;
}

bool shader_object::bind_uniform( v_int32& v, const wstring& name, bool vertex ) const
{
	if( sbase_ )
		return sbase_->bind_uniform( v, name, vertex );

	return false;
}

bool shader_object::bind_uniform( v_vec2f& v, const wstring& name, bool vertex ) const
{
	if( sbase_ )
		return sbase_->bind_uniform( v, name, vertex );

	return false;
}

bool shader_object::bind_uniform( v_vec4f& v, const wstring& name, bool vertex ) const
{
	if( sbase_ )
		return sbase_->bind_uniform( v, name, vertex );

	return false;
}

bool shader_object::bind_uniform( v_matrix4f& v, const wstring& name, bool vertex ) const
{
	if( sbase_ )
		return sbase_->bind_uniform( v, name, vertex );

	return false;
}

bool shader_object::bind_texture( v_int32& v, const wstring& name, bool vertex ) const
{
	if( sbase_ )
		return sbase_->bind_texture( v, name, vertex );

	return false;
}

bool shader_object::bind_matrix_parameter( v_matrix4f& v, const wstring& name, bool vertex ) const
{
	if( sbase_ )
		return sbase_->bind_matrix_parameter( v, name, vertex );

	return false;
}

void shader_object::bind_shader( ) const
{
	if( sbase_ )
		sbase_->bind_shader( );
}

void shader_object::unbind_shader( ) const
{
	if( sbase_ )
		sbase_->unbind_shader( );
}

bool shader_object::is_valid( ) const
{
	if( sbase_ )
		return sbase_->is_valid( );
		
	return false;
}

} }

#endif
