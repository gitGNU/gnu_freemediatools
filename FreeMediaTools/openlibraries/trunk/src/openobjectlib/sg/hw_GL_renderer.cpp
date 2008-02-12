
// sg - A scene graph representation.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifdef HAVE_CONFIG_H
#include <openlibraries_global_config.hpp>
#endif

#include <sstream>

#include <boost/bind.hpp>

#include <openpluginlib/pl/GL_utility.hpp>
#include <openpluginlib/pl/shader_manager.hpp>
#include <openpluginlib/pl/utf8_utils.hpp>

#include <openimagelib/il/openimagelib_plugin.hpp>

#include <openobjectlib/sg/appearance.hpp>
#include <openobjectlib/sg/meshes.hpp>
#include <openobjectlib/sg/spatial.hpp>

#include <openobjectlib/sg/hw_GL_renderer.hpp>

namespace fs  = boost::filesystem;
namespace opl = olib::openpluginlib;

namespace olib { namespace openobjectlib { namespace sg {

namespace
{
	GLenum blend_func_to_gl_enum( const opl::wstring& func )
	{
		if( func == L"ZERO" )
			return GL_ZERO;
		else if( func == L"ONE" )
			return GL_ONE;
		else if( func == L"SRC_COLOR" )
			return GL_SRC_COLOR;
		else if( func == L"INV_SRC_COLOR" )
			return GL_ONE_MINUS_SRC_COLOR;
		else if( func == L"DST_COLOR" )
			return GL_DST_COLOR;
		else if( func == L"INV_DST_COLOR" )
			return GL_ONE_MINUS_DST_COLOR;
		else if( func == L"SRC_ALPHA" )
			return GL_SRC_ALPHA;
		else if( func == L"INV_SRC_ALPHA" )
			return GL_ONE_MINUS_SRC_ALPHA;
		else if( func == L"DST_ALPHA" )
			return GL_DST_ALPHA;
		else if( func == L"INV_DST_ALPHA" )
			return GL_ONE_MINUS_DST_ALPHA;
		else if( func == L"CONST_COLOR" )
			return GL_CONSTANT_COLOR;
		else if( func == L"INV_CONST_COLOR" )
			return GL_ONE_MINUS_CONSTANT_COLOR;
		else if( func == L"CONST_ALPHA" )
			return GL_CONSTANT_ALPHA;
		else if( func == L"INV_CONST_ALPHA" )
			return GL_ONE_MINUS_CONSTANT_ALPHA;
		else if( func == L"SRC_ALPHA_SATURATE" )
			return GL_SRC_ALPHA_SATURATE;

		return static_cast<GLenum>( -1 );
	}

	GLenum blend_equation_to_gl_enum( const opl::wstring& equation )
	{
		if( equation == L"ADD" )
			return GL_FUNC_ADD;
		else if( equation == L"SUBTRACT" )
			return GL_FUNC_SUBTRACT;
		else if( equation == L"REV_SUBTRACT" )
			return GL_FUNC_REVERSE_SUBTRACT;
		else if( equation == L"MIN" )
			return GL_MIN;
		else if( equation == L"MAX" )
			return GL_MAX;
		
		return static_cast<GLenum>( -1 );
	}

	GLenum cull_mode_to_gl_enum( const opl::wstring& mode )
	{
		if( mode == L"BACK" )
			return GL_BACK;
		else if( mode == L"FRONT" )
			return GL_FRONT;
		else if( mode == L"FRONT_AND_BACK" )
			return GL_FRONT_AND_BACK;

		return static_cast<GLenum>( -1 );
	}
	
	/////////////////////////////////////////
	//
	GLenum depth_test_to_gl_enum( const opl::wstring& test )
	{
		if( test == L"NEVER" )
			return GL_NEVER;
		else if( test == L"ALWAYS" )
			return GL_ALWAYS;
		else if( test == L"LESS" )
			return GL_LESS;
		else if( test == L"LEQUAL" )
			return GL_LEQUAL;
		else if( test == L"EQUAL" )
			return GL_EQUAL;
		else if( test == L"GREATER" )
			return GL_GREATER;
		else if( test == L"GEQUAL" )
			return GL_GEQUAL;
		else if( test == L"NOTEQUAL" )
			return GL_NOTEQUAL;

		return static_cast<GLenum>( -1 );
	}

	opl::m_int32::const_pointer update_index_array( opl::m_int32& m )
	{
		if( m.empty( ) ) return 0;
		if( !GLEW_ARB_vertex_buffer_object ) return m.data( );
		
		GLuint id = m.bi( ).value<GLuint>( L"buffer_id" );
		if( !glIsBufferARB( id ) )
		{
			glGenBuffersARB( 1, &id );
			
			m.bi( ).insert( L"buffer_id", id );
			
			glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, id );
			glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB, m.size( ) * sizeof( opl::m_int32::value_type ), m.data( ), GL_STATIC_DRAW_ARB );
		}

		glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, id );
		
		return 0;
	}

	opl::m_float::const_pointer update_attribute_array( v_attribute_array_ptr& m )
	{
		opl::m_float* value = m.get( )->pointer<opl::m_float>( L"value" );
		
		if( value->empty( ) ) return 0;
		if( !GLEW_ARB_vertex_buffer_object ) return value->data( );

		GLuint id = m.bi( ).value<GLuint>( L"buffer_id" );
		if( !glIsBufferARB( id ) )
		{
			glGenBuffersARB( 1, &id );
			
			m.bi( ).insert( L"buffer_id", id );
			
			glBindBufferARB( GL_ARRAY_BUFFER_ARB, id );
			glBufferDataARB( GL_ARRAY_BUFFER_ARB, value->size( ) * sizeof( opl::m_float::value_type ), value->data( ), GL_STATIC_DRAW_ARB );
		}

		glBindBufferARB( GL_ARRAY_BUFFER_ARB, id );

		return 0;
	}

	void disable_attribute_array( GLenum array )
	{
		if( GLEW_ARB_vertex_buffer_object )
			glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );

		glDisableClientState( array );
	}
	
	bool GL_2_0_shaders_available( )
	{
		if( GLEW_VERSION_2_0 || ( ( GLEW_ARB_shading_language_100 || GLEW_EXT_Cg_shader ) && GLEW_ARB_shader_objects && GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader ) )
			return true;

		return false;
	}

	shader_part_ptr ffp_shader_part( const opl::string& shader, const opl::wstring& type, bool source = false )
	{
		shader_part_ptr p( new shader_part( ) );
		
		opl::m_url*		u = p->pointer<opl::m_url>(	L"url" );
		opl::v_string*	t = p->pointer<opl::v_string>( L"type" );
		opl::v_string*	s = p->pointer<opl::v_string>( L"source" );
		opl::v_bool*	e = p->pointer<opl::v_bool>( L"embedded" );
		
		if( !source )
		{
			u->push_back( fs::path( shader.c_str( ), fs::native ) );
			e->get( ) = false;
		}
		else
		{
			s->get( ) = opl::to_wstring( shader );
			e->get( ) = true;
		}

		t->get( ) = type;
		
		return p;
	}

	shader_ptr ffp_emulator( const mv_texture_ptr& t )
	{
		shader_ptr s;

		if( GL_2_0_shaders_available( ) )
		{
			s = shader_ptr( new composed_shader( ) );
			
			opl::v_string* def  = s->pointer<opl::v_string>( L"DEF" );
			opl::v_string* lang = s->pointer<opl::v_string>( L"language" );
			
			def->get( )  = L"FFPE";
			lang->get( ) = L"GLSL";
			
#	ifdef WIN32
			fs::path vp_shader_path( std::string( opl::shaders_path( ).c_str( ) ) + "\\fixed_function.vert", fs::native );
#	else
			fs::path vp_shader_path( OPENOBJECTLIB_SHADERS "/fixed_function.vert", fs::native );
#	endif
			m_shader_part_ptr* p = s->pointer<m_shader_part_ptr>( L"parts" );
			p->push_back( ffp_shader_part( vp_shader_path.native_file_string( ).c_str( ), L"VERTEX" ) );

			s->insert_param( L"nlights", sg::shader::field_type( opl::v_int32_typename, opl::v_int32( ) ) );

			for( size_t i = 0; i < t.size( ); ++i )
			{
				std::wostringstream tex_name;
				tex_name << L"tex_unit_" << i;
				
				s->insert_param( tex_name.str( ).c_str( ), sg::shader::field_type( v_node_typename, t[ i ] ) );
			}

			std::ostringstream fp_shader;
			for( size_t i = 0; i < t.size( ); ++i )
				fp_shader << "uniform sampler2D tex_unit_" << i << ";\n";
			fp_shader << "void main( void ) { vec4 color; color = gl_Color;";
			
			for( size_t i = 0; i < t.size( ); ++i )
			{
				fp_shader << "color *= texture2D( tex_unit_" << i << ", gl_TexCoord[ " << i << "].xy );";
			}

			fp_shader << "gl_FragColor = color; }";

			p->push_back( ffp_shader_part( fp_shader.str( ).c_str( ), L"FRAGMENT", true ) );
		}

		return s;
	}

	
	std::pair<opl::shader_string, opl::shader_string> concatenate_shader_parts( const m_shader_part_ptr& m )
	{
		typedef std::pair<opl::shader_string, opl::shader_string>	return_type;
		typedef m_shader_part_ptr::const_iterator					const_iterator;
		
		return_type shaders( std::make_pair( "", "" ) );
		
		for( const_iterator I = m.begin( ); I != m.end( ); ++I )
		{
			opl::m_url*		u = ( *I )->pointer<opl::m_url>( L"url" );
			opl::v_string*	t = ( *I )->pointer<opl::v_string>( L"type" );
			opl::v_string*	r = ( *I )->pointer<opl::v_string>( L"source" );
			opl::v_bool*	e = ( *I )->pointer<opl::v_bool>( L"embedded" );
			
			opl::shader_string s;
			if( !e->value( ) )
			{
				for( size_t i = 0; i < u->size( ); ++i )
				{
					s = opl::shader_manager_instance( ).open_shader_file( ( *u )[ i ].native_file_string( ), false );
					if( !s.empty( ) ) break;
				}
			}
			else
			{
				opl::string tmp = opl::to_string( r->value( ) );
				s.assign( tmp.begin( ), tmp.end( ) );
			}

			if( t->value( ) == L"VERTEX" ) shaders.first += s;
			else shaders.second += s;
		}

		return shaders;
	}

	bool enable_vertices( v_attribute_array_ptr& m )
	{
		typedef opl::m_float::const_pointer const_pointer;

		if( !m.value( ) ) return false;
	
		const_pointer data = update_attribute_array( m );
		glEnableClientState( GL_VERTEX_ARRAY );
		glVertexPointer( 3, GL_FLOAT, 0, data );

		return true;
	}

	bool enable_normals( v_attribute_array_ptr& m )
	{
		typedef opl::m_float::const_pointer const_pointer;

		if( !m.value( ) ) return false;

		const_pointer data = update_attribute_array( m );
		glEnableClientState( GL_NORMAL_ARRAY );
		glNormalPointer( GL_FLOAT, 0, data );

		return true;
	}

	bool enable_colors( v_attribute_array_ptr& m )
	{
		typedef opl::m_float::const_pointer const_pointer;

		if( !m.value( ) ) return false;

		opl::v_int32 comp  = m.get( )->value<opl::v_int32>( L"components" );
		const_pointer data = update_attribute_array( m );
	
		glEnableClientState( GL_COLOR_ARRAY );
		glColorPointer( comp.value( ), GL_FLOAT, 0, data );
	
		return true;
	}

	bool enable_attribs( hw_GL_renderer& r, v_attribute_array_ptr& m )
	{
		typedef opl::m_float::const_pointer const_pointer;

		if( !m.value( ) ) return false;
	
		opl::v_int32  comp = m.get( )->value<opl::v_int32>( L"components" );
		const_pointer data = update_attribute_array( m );

		GLint location = m.bi( ).value<GLint>( L"GL_location", -1 );
		if( location == -1 )
		{
			opl::v_string name = m.get( )->value<opl::v_string>( L"name" );
		
			location = glGetAttribLocationARB( r.current_GL_2_0_program( ), opl::to_string( name.value( ) ).c_str( ) );
			m.bi( ).insert( L"GL_location", GLint( location ) );
		}

		glEnableVertexAttribArrayARB( location );
		glVertexAttribPointerARB( location, comp.value( ), GL_FLOAT, GL_TRUE, 0, data );

		return true;
	}

	bool enable_uvs( v_uv_set_ptr& m, int i )
	{
		typedef opl::m_float::const_pointer const_pointer;

		if( !m.value( ) ) return false;

		opl::v_bool* auto_ = m.get( )->pointer<opl::v_bool>( L"auto" );
		if( !auto_->value( ) )
		{
			v_attribute_array_ptr* uv = m.get( )->pointer<v_attribute_array_ptr>( L"uv" );
			if( !uv->value( ) ) return false;

			opl::v_int32 comp  = uv->get( )->value<opl::v_int32>( L"components" );
			const_pointer data = update_attribute_array( *uv );

			glClientActiveTextureARB( GL_TEXTURE0 + i );
			glEnableClientState( GL_TEXTURE_COORD_ARRAY );
			glTexCoordPointer( comp.value( ), GL_FLOAT, 0, data );
		}
		else
		{
			glActiveTextureARB( GL_TEXTURE0 + i );
			
			opl::v_string mode = m.get( )->value<opl::v_string>( L"mode" );
		
			if( mode.value( ) == L"SPHERE" )
			{
				glEnable( GL_TEXTURE_GEN_S );
				glEnable( GL_TEXTURE_GEN_T );
			
				glTexGeni( GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP );
				glTexGeni( GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP );
			}
			else if( mode.value( ) == L"CAMERASPACENORMAL" )
			{
				glEnable( GL_TEXTURE_GEN_S );
				glEnable( GL_TEXTURE_GEN_T );
				glEnable( GL_TEXTURE_GEN_R );

				glTexGeni( GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP );
				glTexGeni( GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP );
				glTexGeni( GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP );
			}
		}

		return true;
	}

	bool disable_vertices( v_attribute_array_ptr& /*m*/ )
	{
		disable_attribute_array( GL_VERTEX_ARRAY );
		return true;
	}

	bool disable_normals( v_attribute_array_ptr& /*m*/ )
	{
		disable_attribute_array( GL_NORMAL_ARRAY );
		return true;
	}

	bool disable_colors( v_attribute_array_ptr& /*m*/ )
	{
		disable_attribute_array( GL_COLOR_ARRAY );
		return true;
	}

	bool disable_attribs( v_attribute_array_ptr& m )
	{
		GLint location = m.bi( ).value<GLint>( L"GL_location", -1 );
		if( location == -1 ) return false;
		
		glDisableVertexAttribArrayARB( location );
	
		return true;
	}

	bool disable_uvs( v_uv_set_ptr& m, int i )
	{
		opl::v_bool* auto_ = m.get( )->pointer<opl::v_bool>( L"auto" );
		if( !auto_->value( ) )
		{
			glClientActiveTextureARB( GL_TEXTURE0 + i );
			disable_attribute_array( GL_TEXTURE_COORD_ARRAY );
		}
		else
		{
			glActiveTextureARB( GL_TEXTURE0 + i );
			
			glDisable( GL_TEXTURE_GEN_S );
			glDisable( GL_TEXTURE_GEN_T );
			glDisable( GL_TEXTURE_GEN_R );
			glDisable( GL_TEXTURE_GEN_Q );
		}
	
		return true;
	}

	bool draw_elements( GLenum prim_type, opl::m_int32& m )
	{
		typedef opl::m_int32::const_pointer const_pointer;

		if( m.empty( ) ) return false;

		const_pointer data = update_index_array( m );
		glDrawElements( prim_type, static_cast<GLsizei>( m.size( ) ), GL_UNSIGNED_INT, data );

		if( GLEW_ARB_vertex_buffer_object )
			glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 );

		return true;
	}

	bool multi_draw_elements( GLenum prim_type, opl::m_int32& m, opl::m_int32& c )
	{
		if( m.empty( ) || c.empty( ) )
			return false;

		opl::m_int32::const_pointer s = m.data( );
		for( size_t i = 0; i < c.size( ); ++i )
		{
			glDrawElements( prim_type, c[ i ], GL_UNSIGNED_INT, s );
			s += c[ i ];
		}
		
		return true;
	}




	bool enable_material( olib::openobjectlib::sg::v_material_ptr& v )
	{
		opl::v_float* ambient_intensity	= v.get( )->pointer<opl::v_float>( L"ambientIntensity" );
		opl::v_color* diffuse_color		= v.get( )->pointer<opl::v_color>( L"diffuseColor" );
		opl::v_color* emissive_color	= v.get( )->pointer<opl::v_color>( L"emissiveColor" );
		opl::v_float* shininess			= v.get( )->pointer<opl::v_float>( L"shininess" );
		opl::v_color* specular_color	= v.get( )->pointer<opl::v_color>( L"specularColor" );
		opl::v_float* transparency		= v.get( )->pointer<opl::v_float>( L"transparency" );

		opl::v_color::value_type ambient_color = diffuse_color->value( ) * ambient_intensity->value( );
	
		//XXX handle transparency.
		glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT,   &ambient_color[ 0 ]  );
		glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE,   &diffuse_color->value( )[ 0 ]  );
		glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION,  &emissive_color->value( )[ 0 ] );
		glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR,  &specular_color->value( )[ 0 ] );
		glMaterialf(  GL_FRONT_AND_BACK, GL_SHININESS, shininess->value( ) * 128.0f );

		return true;
	}
	
	bool render_camera( hw_GL_renderer& /*r*/, node_ptr& /*v*/ )
	{ return true; }

	bool render_mesh( hw_GL_renderer& r, node_ptr& v )
	{
		mesh_ptr m = boost::dynamic_pointer_cast<mesh>( v );
		
		v_attribute_array_ptr*	coord  = m->pointer<v_attribute_array_ptr>(  L"coord" );
		v_attribute_array_ptr*	normal = m->pointer<v_attribute_array_ptr>(  L"normal" );
		v_attribute_array_ptr*	color  = m->pointer<v_attribute_array_ptr>(  L"color" );
		mv_attribute_array_ptr* attrib = m->pointer<mv_attribute_array_ptr>( L"attrib" );
		mv_uv_set_ptr*			uv	   = m->pointer<mv_uv_set_ptr>( L"uv" );
		opl::m_int32*			index  = m->pointer<opl::m_int32>( L"index" );
		opl::m_int32*			count  = m->pointer<opl::m_int32>( L"count" );

		enable_vertices( *coord );
		enable_normals( *normal );
		enable_colors( *color );

		for( size_t i = 0; i < attrib->size( ); ++i )
			enable_attribs( r, ( *attrib )[ i ] );
		for( size_t i = 0; i < uv->size( ); ++i )
			enable_uvs( ( *uv )[ i ], static_cast<int>( i ) );

		if( m->prim_type( ) == mesh::TRIANGLES )
			draw_elements( GL_TRIANGLES, *index );
		else if( m->prim_type( ) == mesh::POLYGONS )
			multi_draw_elements( GL_POLYGON, *index, *count );

		disable_vertices( *coord );
		disable_normals( *normal );
		disable_colors( *color );

		for( size_t i = 0; i < attrib->size( ); ++i )
			disable_attribs( ( *attrib )[ i ] );
		for( size_t i = 0; i < uv->size( ); ++i )
			disable_uvs( ( *uv )[ i ], static_cast<int>( i ) );

		return true;
	}
	
	bool render_multi_index_mesh( hw_GL_renderer& r, node_ptr& v )
	{
		typedef opl::m_int32::const_iterator const_iterator;
		
		multi_index_mesh_ptr m = boost::dynamic_pointer_cast<multi_index_mesh>( v );

		v_attribute_array_ptr*	coord	= m->pointer<v_attribute_array_ptr>( L"coord" );
		v_attribute_array_ptr*	normal	= m->pointer<v_attribute_array_ptr>( L"normal" );
		v_attribute_array_ptr*	color	= m->pointer<v_attribute_array_ptr>( L"color" );
		mv_uv_set_ptr*			uv		= m->pointer<mv_uv_set_ptr>( L"uv" );
		opl::m_int32*			ci		= m->pointer<opl::m_int32>( L"index" );
		opl::m_int32*			ni		= m->pointer<opl::m_int32>( L"normalIndex" );
		opl::m_int32*			ti		= m->pointer<opl::m_int32>( L"texCoordIndex" );
		opl::m_int32*			li		= m->pointer<opl::m_int32>( L"colorIndex" );
		opl::v_bool				npv		= m->value<opl::v_bool>( L"normalPerVertex" );
		opl::v_bool				cpv		= m->value<opl::v_bool>( L"colorPerVertex" );

		opl::m_float* v1 = coord->get( )  ? coord->get( )->pointer<opl::m_float>( L"value" ) : 0;
		opl::m_float* v2 = normal->get( ) ? normal->get( )->pointer<opl::m_float>( L"value" ) : 0;
		opl::m_float* v3 = color->get( )  ? color->get( )->pointer<opl::m_float>( L"value" ) : 0;

		std::vector<opl::m_float*> v4;
		v4.reserve( 8 );

		for( size_t i = 0; i < uv->size( ); ++i )
		{
			v_attribute_array_ptr* uv_ = ( *uv )[ i ].get( )->pointer<v_attribute_array_ptr>( L"uv" );
			
			opl::m_float* tmp = uv_->get( ) ? uv_->get( )->pointer<opl::m_float>( L"value" ) : 0;
			v4.push_back( tmp );
		}

		const_iterator coord_i = ci->begin( );
		const_iterator norm_i  = ni->begin( );
		const_iterator texco_i = ti->begin( );
		const_iterator color_i = li->begin( );

		while( coord_i != ci->end( ) )
		{
			glBegin( GL_POLYGON );

			if( v2 && !npv.value( ) )
				glNormal3fv( &( *v2 )[ *norm_i * 3 ] );

			if( v3 && !cpv.value( ) )
				glColor3fv( &( *v3 )[ *color_i * 3 ] );

			while( coord_i != ci->end( ) )
			{
				if( *coord_i == -1 )
					break;

				if( v2 && npv.value( ) )
					glNormal3fv( &( *v2 )[ *norm_i * 3 ] );
				if( v3 && cpv.value( ) )
					glColor3fv( &( *v3 )[ *color_i * 3 ] );

				for( size_t i = 0; i < v4.size( ); ++i )
					if( v4[ i ] )
						glMultiTexCoord2fv( GL_TEXTURE0 + i, &( *v4[ i ] )[ *texco_i * 2 ] );

				if( v1 )
					glVertex3fv( &( *v1 )[ *coord_i * 3 ] );
				
				++coord_i;
				++texco_i;
				
				if( npv.value( ) )
					++norm_i;
				if( cpv.value( ) )
					++color_i;
			}
			
			glEnd( );
			
			++coord_i;
			++texco_i;
			++norm_i;
			++color_i;
		}

		return true;
	}

	bool render_group( hw_GL_renderer& r, node_ptr& v )
	{
		m_spatial_ptr* children = v->pointer<m_spatial_ptr>( L"children" );
		std::for_each( children->begin( ), children->end( ), boost::bind( &hw_GL_renderer::render, boost::ref( r ), _1 ) );
		return true;
	}

	bool render_select( hw_GL_renderer& r, node_ptr& v )
	{
		m_spatial_ptr*  children = v->pointer<m_spatial_ptr>( L"children" );
		opl::v_int32	choice	 = v->value<opl::v_int32>( L"choice" );
		
		if( choice.value( ) >= 0 && choice.value( ) < children->size( ) )
			r.render( ( *children )[ choice.value( ) ] );
		 
		return true;
	}

	bool render_transform( hw_GL_renderer& r, node_ptr& v )
	{
		m_spatial_ptr*   children  = v->pointer<m_spatial_ptr>( L"children" );
		opl::v_matrix4f* transform = v->pointer<opl::v_matrix4f>( L"matrix" );
		
		glPushMatrix( );
		glMultMatrixf( &transform->get( )[ 0 ] );
		
		std::for_each( children->begin( ), children->end( ), boost::bind( &hw_GL_renderer::render, boost::ref( r ), _1 ) );
		
		glPopMatrix( );
		
		return true;
	}
	
	bool render_shape( hw_GL_renderer& r, node_ptr& v )
	{
		v_appearance_ptr* a = v->pointer<v_appearance_ptr>( L"appearance" );
		v_mesh_ptr*		  m = v->pointer<v_mesh_ptr>( L"geometry" );

		if( a->value( ) )
		{
			v_material_ptr* mat = a->get( )->pointer<v_material_ptr>( L"material" );
			if( mat && mat->value( ) )
				enable_material( *mat );
			
			m_node_ptr* rp = a->get( )->pointer<m_node_ptr>( L"renderProperties" );
			std::for_each( rp->begin( ), rp->end( ), boost::bind( &hw_GL_renderer::render, boost::ref( r ), _1 ) );

	 olib::openobjectlib::sg::m_texture_transform_ptr* tt =
	   a->get( )->pointer<m_texture_transform_ptr>( L"textureTransform" );

			std::for_each( tt->begin( ), tt->end( ), boost::bind( &hw_GL_renderer::render, boost::ref( r ), _1 ) );
			
			mv_texture_ptr* t = a->get( )->pointer<mv_texture_ptr>( L"texture" );

			v_shader_ptr* s = a->get( )->pointer<v_shader_ptr>( L"shader" );
			if( r.ffpe( ) && !s->value( ) )
				s->get( ) = ffp_emulator( *t );
			else
				std::for_each( t->begin( ), t->end( ), boost::bind( &hw_GL_renderer::enable_texture, boost::ref( r ), _1 ) );

			if( s->value( ) && s->value( )->GL_2_0_usable( ) )
				r.enable_GL_2_0_shader( *s );
		}

		if( m->value( ) )
			r.render( m->value( ) );

		if( a->value( ) )
		{
			v_shader_ptr* s = a->get( )->pointer<v_shader_ptr>( L"shader" );
			if( s->value( ) && s->value( )->GL_2_0_usable( ) )
				r.disable_GL_2_0_shader( );
		}

		r.disable_all_texture_units( );
		r.disable_all_texture_transforms( );

		return true;
	}
	
	bool render_texture_transform( hw_GL_renderer& r, node_ptr& v )
	{
		opl::v_matrix4f* mat = v->pointer<opl::v_matrix4f>( L"matrix" );
		
		r.enable_texture_transform( *mat );
		
		return true;
	}
	
	bool render_blend( hw_GL_renderer& /*r*/, node_ptr& v )
	{
		opl::v_color_rgba	color			= v->value<opl::v_color_rgba>( L"color" );
		opl::v_string		destAlpha		= v->value<opl::v_string>( L"destAlpha" );
		opl::v_string		destColor		= v->value<opl::v_string>( L"destColor" );
		opl::v_bool			enable			= v->value<opl::v_bool>( L"enable" );
		opl::v_string		equationAlpha	= v->value<opl::v_string>( L"equationAlpha" );
		opl::v_string		equationColor	= v->value<opl::v_string>( L"equationColor" );
		opl::v_string		sourceAlpha		= v->value<opl::v_string>( L"sourceAlpha" );
		opl::v_string		sourceColor		= v->value<opl::v_string>( L"sourceColor" );
	
		glBlendEquationSeparate( blend_equation_to_gl_enum( equationColor.value( ) ), blend_equation_to_gl_enum( equationAlpha.value( ) ) );
		glBlendFuncSeparate( 
			blend_func_to_gl_enum( sourceColor.value( ) ),
			blend_func_to_gl_enum( sourceAlpha.value( ) ),
			blend_func_to_gl_enum( destColor.value( ) ),
			blend_func_to_gl_enum( destAlpha.value( ) ) );
		glBlendColor( color.value( )[ 0 ], color.value( )[ 1 ], color.value( )[ 2 ], color.value( )[ 3 ] );
	
		enable.value( ) ? glEnable( GL_BLEND ) : glDisable( GL_BLEND );

		return true;
	}
	
	bool render_cull_mode( hw_GL_renderer& /*r*/, node_ptr& v )
	{
		opl::v_bool		enable	= v->value<opl::v_bool>( L"enable" );
		opl::v_string	mode	= v->value<opl::v_string>( L"mode" );
	
		glCullFace( cull_mode_to_gl_enum( mode.value( ) ) );
	
		enable.value( ) ? glEnable( GL_CULL_FACE ) : glDisable( GL_CULL_FACE );

		return true;
	}
	
	bool render_depth_test( hw_GL_renderer& /*r*/, node_ptr& v )
	{
		opl::v_bool		enable	= v->value<opl::v_bool>( L"enable" );
		opl::v_string	test	= v->value<opl::v_string>( L"test" );
		opl::v_bool		write	= v->value<opl::v_bool>( L"writeEnable" );
	
		glDepthFunc( depth_test_to_gl_enum( test.value( ) ) );
		glDepthMask( write.value( ) ? GL_TRUE : GL_FALSE );
	
		enable.value( ) ? glEnable( GL_DEPTH_TEST ) : glDisable( GL_DEPTH_TEST );

		return true;
	}
	
	bool render_front_face( hw_GL_renderer& /*r*/, node_ptr& v )
	{
		opl::v_bool ccw = v->value<opl::v_bool>( L"ccw" );

		glFrontFace( ccw.value( ) ? GL_CCW : GL_CW );

		return true;
	}
	
	bool bind_GL_2_0_v_float( hw_GL_renderer& /*r*/, GLuint id, const shader_ptr& s, const opl::wstring& param )
	{
		opl::v_float* v = s->pointer_param<opl::v_float>( param );
		opl::shader_manager_instance( ).bind_GL_uniform( id, *v, param );

		return true;
	}
	
	bool bind_GL_2_0_v_vec4f( hw_GL_renderer& /*r*/, GLuint id, const shader_ptr& s, const opl::wstring& param )
	{
		opl::v_vec4f* v = s->pointer_param<opl::v_vec4f>( param );
		opl::shader_manager_instance( ).bind_GL_uniform( id, *v, param );
		
		return true;
	}

	bool bind_GL_2_0_v_matrix4f( hw_GL_renderer& /*r*/, GLuint id, const shader_ptr& s, const opl::wstring& param )
	{
		opl::v_matrix4f* v = s->pointer_param<opl::v_matrix4f>( param );
		opl::shader_manager_instance( ).bind_GL_uniform( id, *v, param );

		return true;
	}
	
	bool bind_GL_2_0_v_node( hw_GL_renderer& r, GLuint id, const shader_ptr& s, const opl::wstring& param )
	{
		v_texture_ptr* v = s->pointer_param<v_texture_ptr>( param );

		GLint location = v->bi( ).value<GLint>( L"GL_location", -1 );
		if( location == -1 )
		{
			location = glGetUniformLocationARB( id, opl::to_string( param ).c_str( ) );
			v->bi( ).insert( L"GL_location", GLint( location ) );
		}

		glUniform1iARB( location, r.current_texture_unit( ) );

		r.enable_texture( *v );
			
		return true;
	}
}

hw_GL_renderer::hw_GL_renderer( )
	: program_( 0 )
	, unit_( 0 )
	, unit_transform_( 0 )
	, ffpe_( false )
{
	add<camera>( render_camera );
	add<group>( render_group );
	add<select>( render_select );
	add<shape>( render_shape );
	add<transform>( render_transform );
	add<texture_transform>( render_texture_transform );
	add<blend>( render_blend );
	add<cull_mode>( render_cull_mode );
	add<depth_test>( render_depth_test );
	add<front_face>( render_front_face );
	add<mesh>( render_mesh );
	add<multi_index_mesh>( render_multi_index_mesh );

	s_param_GL_dispatcher_.insert( s_param_GL_dispatcher_value_type( opl::v_float_typename, bind_GL_2_0_v_float ) );
	s_param_GL_dispatcher_.insert( s_param_GL_dispatcher_value_type( opl::v_vec4f_typename, bind_GL_2_0_v_vec4f ) );
	s_param_GL_dispatcher_.insert( s_param_GL_dispatcher_value_type( opl::v_matrix4f_typename, bind_GL_2_0_v_matrix4f ) );
	s_param_GL_dispatcher_.insert( s_param_GL_dispatcher_value_type( v_node_typename, bind_GL_2_0_v_node ) );

	// GC - Use getenv_s if using VC8.
	char* ffpe = getenv( "EMULATE_FIXED_FUNCTION_PIPELINE" );
	if( ffpe )
		ffpe_ = atoi( ffpe ) != 0;
}

hw_GL_renderer::~hw_GL_renderer( )
{ }

bool hw_GL_renderer::render( const node_ptr& n )
{ return back_end_( *this, boost::const_pointer_cast<node>( n ) ); }

bool hw_GL_renderer::render_scene( const scene_ptr& n, bool clear )
{
	int x, y, w, h;
	n->get_viewport_extents( x, y, w, h );
	
	glViewport( x, y, w, h );
	
	if( clear )
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_NORMALIZE );
	glEnable( GL_RESCALE_NORMAL );
	glEnable( GL_LIGHTING );
		
	camera_ptr cam = n->get_active_camera( );

	if( cam )
	{
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity( );

		opl::v_float wL = cam->value<opl::v_float>( L"wL" );
		opl::v_float wR = cam->value<opl::v_float>( L"wR" );
		opl::v_float wB = cam->value<opl::v_float>( L"wB" );
		opl::v_float wT = cam->value<opl::v_float>( L"wT" );
		opl::v_float nZ = cam->value<opl::v_float>( L"nearZ" );
		opl::v_float fZ = cam->value<opl::v_float>( L"farZ" );
		
		glFrustum( wL.value( ), wR.value( ), wB.value( ), wT.value( ), nZ.value( ), fZ.value( ) );
		
		glMatrixMode( GL_MODELVIEW );
	
		opl::matrixf m = view_from_camera( *cam );
		glLoadMatrixf( &m[ 0 ] );
	}

	float lpos[ ]	 = { 1.0f, 1.0f, 1.0f, 0.0f };
	float diffuse[ ] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glLightfv( GL_LIGHT0, GL_POSITION, lpos );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse );
	glEnable( GL_LIGHT0 );

	bool valid = false;

	sg::node_ptr root = n->get_root( );
	if( root )
		valid = render( root );
	
	glDisable( GL_LIGHTING );
	
	return valid;
}

bool hw_GL_renderer::enable_texture( v_texture_ptr& v )
{
	GLenum target = v.bi( ).value<GLenum>( L"tex_target" );
	GLuint id	  = v.bi( ).value<GLuint>( L"tex_id" );

	if( id == -1 )
		return false;

	if( !glIsTexture( id ) )
	{
#ifdef HAVE_OPENIMAGELIB
		namespace il = olib::openimagelib::il;
		
		typedef il::image_type::const_pointer	const_pointer;
		typedef il::image_type::size_type		size_type;

		il::image_type_ptr im = v.get( )->image( );

		if( !im )
		{
			v.bi( ).insert( L"tex_id", GLuint( -1 ) );
			
			return false;
		}
		
		const_pointer texels = im->data( );
		int width			 = im->width( );
		int height			 = im->height( );
		int depth			 = im->depth( );

		opl::v_bool rs = v.get( )->value<opl::v_bool>( L"repeatS" );
		opl::v_bool rt = v.get( )->value<opl::v_bool>( L"repeatT" );
		opl::v_bool rr = v.get( )->value<opl::v_bool>( L"repeatR" );
		opl::v_bool cm = v.get( )->value<opl::v_bool>( L"cubemap" );

		if( !cm.value( ) )
		{
			if( !im->is_volume( ) )
			{
				float dummy_tex_w, dummy_tex_h;
				if( !opl::texture_target( width, height, target, dummy_tex_w, dummy_tex_h ) )
					return false;
			}
			else
			{
				target = GL_TEXTURE_3D;
			}
		}
		else
		{
			target = GL_TEXTURE_CUBE_MAP_ARB;
		}

		glGenTextures( 1, &id );
		glBindTexture( target, id );
		glTexParameteri( target, GL_GENERATE_MIPMAP_SGIS, GL_TRUE );
		glTexParameteri( target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		glTexParameteri( target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		
		if( width  != 0 || im->is_volume( ) || cm.value( ) ) glTexParameteri( target, GL_TEXTURE_WRAP_S, rs.value( ) ? GL_REPEAT : GL_CLAMP_TO_EDGE );
		if( height != 0 || im->is_volume( ) || cm.value( ) ) glTexParameteri( target, GL_TEXTURE_WRAP_T, rt.value( ) ? GL_REPEAT : GL_CLAMP_TO_EDGE );
		if( im->is_volume( ) || cm.value( ) ) glTexParameteri( target, GL_TEXTURE_WRAP_R, rr.value( ) ? GL_REPEAT : GL_CLAMP_TO_EDGE );

		int num_im = 1;
		if( cm.value( ) )
		{
			target = GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB;
			num_im = 6;
		}

		typedef std::pair<GLint, GLenum> internal_and_base_format;
		internal_and_base_format pf;
		GLenum type;
		opl::pf_to_gl_format( im->pf( ).c_str( ), pf.first, pf.second, type );

		glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );

		for( size_type i = 0; i < num_im; ++i )
		{
			if( cm.value( ) )
				texels	= il::cubemap_face( im, static_cast<int>( i ) );

			if( opl::is_compressed_format( im->pf( ).c_str( ) ) )
			{
				size_t mipsize = il::calculate_mipmap_size( im, 0 );
			
				if( width != 0 && height == 0 && !im->is_volume( ) )
					glCompressedTexImage1DARB( target + i, 0, pf.second, static_cast<GLsizei>( width ), 0, mipsize, texels );
				else if( width != 0 && height != 0 && !im->is_volume( ) )
					glCompressedTexImage2DARB( target + i, 0, pf.second, static_cast<GLsizei>( width ), static_cast<GLsizei>( height ), 0, mipsize, texels );
				else if( width != 0 && height != 0 && im->is_volume( ) )
					glCompressedTexImage3DARB( target + i, 0, pf.second, static_cast<GLsizei>( width ), static_cast<GLsizei>( height ), static_cast<GLsizei>( depth ), 0, mipsize, texels );
			}
			else
			{
				if( width != 0 && height == 0 && !im->is_volume( ) )
					glTexImage1D( target + i, 0, pf.first, static_cast<GLsizei>( width ), 0, pf.second, GL_UNSIGNED_BYTE, texels );
				else if( width != 0 && height != 0 && !im->is_volume( ) )
					glTexImage2D( target + i, 0, pf.first, static_cast<GLsizei>( width ), static_cast<GLsizei>( height ), 0, pf.second, type, texels );
				else if( width != 0 && height != 0 && im->is_volume( ) )
					glTexImage3D( target + i, 0, pf.first, static_cast<GLsizei>( width ), static_cast<GLsizei>( height ), static_cast<GLsizei>( depth ), 0, pf.second, type, texels );
			}
		}

		v.bi( ).insert( L"tex_id", id );

		if( !cm.value( ) ) v.bi( ).insert( L"tex_target", target );
		else v.bi( ).insert( L"tex_target", target = GL_TEXTURE_CUBE_MAP_ARB );
#endif
	}

	glActiveTextureARB( GL_TEXTURE0 + current_texture_unit( ) );
	glEnable( target );
	glBindTexture( target, id );
	
	current_texture_unit( ) += 1;

	return true;
}

bool hw_GL_renderer::disable_texture( v_texture_ptr& /*v*/ )
{ return false; }

bool hw_GL_renderer::disable_all_texture_units( )
{
	GLint max_units;
	glGetIntegerv( GL_MAX_TEXTURE_UNITS, &max_units );

	for( int i = 0; i < max_units; ++i )
	{
		glActiveTextureARB( GL_TEXTURE0 + i );
		
		glBindTexture( GL_TEXTURE_1D, 0 );
		glBindTexture( GL_TEXTURE_2D, 0 );
		glBindTexture( GL_TEXTURE_3D, 0 );

		if( GLEW_ARB_texture_cube_map )  glBindTexture( GL_TEXTURE_CUBE_MAP_ARB, 0 );		
		if( GLEW_ARB_texture_rectangle ) glBindTexture( GL_TEXTURE_RECTANGLE_ARB, 0 );
		if( GLEW_EXT_texture_rectangle ) glBindTexture( GL_TEXTURE_RECTANGLE_EXT, 0 );
		if( GLEW_NV_texture_rectangle )  glBindTexture( GL_TEXTURE_RECTANGLE_NV,  0 );
	}
	
	unit_ = 0;

	return true;
}

void hw_GL_renderer::enable_texture_transform( const opl::v_matrix4f& v )
{
	glActiveTexture( GL_TEXTURE0 + current_texture_unit_transform( ) );

	glMatrixMode( GL_TEXTURE );
	glLoadMatrixf( &v.value( )[ 0 ] );
	
	glMatrixMode( GL_MODELVIEW );
	
	current_texture_unit_transform( ) += 1;
}

void hw_GL_renderer::disable_all_texture_transforms( )
{
	for( int i = 0; i < current_texture_unit_transform( ); ++i )
	{
		glActiveTexture( GL_TEXTURE0 + current_texture_unit_transform( ) );
		glMatrixMode( GL_TEXTURE );
		glLoadIdentity( );
	}
	
	glMatrixMode( GL_MODELVIEW );

	current_texture_unit_transform( ) = 0;
}

bool hw_GL_renderer::enable_GL_2_0_shader( v_shader_ptr& s )
{
	if( !GL_2_0_shaders_available( ) ) return false;

	m_shader_part_ptr* p = s.get( )->pointer<m_shader_part_ptr>( L"parts" );
	if( p->empty( ) || !GL_2_0_shaders_available( ) )
		return false;

	GLhandleARB id = s.bi( ).value<GLhandleARB>( L"program_id" );
	if( glIsProgramARB && !id ) // GC - some freaky issues going on with glIsProgram. TBI.
	{
		typedef std::pair<opl::shader_string, opl::shader_string> shader_pair;

		shader_pair c = concatenate_shader_parts( *p );

		opl::GLSL_shader_object sobj;
		if( !opl::shader_manager_instance( ).create_GLSL_shader( c.first, c.second, sobj ) )
			return false;

		id = sobj.program( );
		s.bi( ).insert( L"program_id", id );
	}

	current_GL_2_0_program( ) = id;
	glUseProgramObjectARB( id );

	typedef shader::const_iterator const_iterator;
	for( const_iterator I = s.get( )->begin_param( ); I != s.get( )->end_param( ); ++I )
	{
		s_param_GL_dispatcher_const_iterator J = s_param_GL_dispatcher_.find( I->second.first );
		if( J != s_param_GL_dispatcher_.end( ) )
			J->second( *this, id, s.get( ), I->first );
	}

	return true;
}

bool hw_GL_renderer::disable_GL_2_0_shader( )
{
	if( !GL_2_0_shaders_available( ) )
		return false;
	
	glUseProgramObjectARB( 0 );

	return true;
}

namespace
{
	void destroy( hw_GL_renderer* r )
	{ delete r; }
}

hw_GL_renderer_ptr create_hw_GL_renderer( )
{ return hw_GL_renderer_ptr( new hw_GL_renderer( ), destroy ); }

} } }
