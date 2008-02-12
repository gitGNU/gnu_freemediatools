
// sg - A scene graph representation.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef HW_GL_RENDERER_INC_
#define HW_GL_RENDERER_INC_

#if _MSC_VER >= 1400
#include <hash_map>
#else
#include <map>
#endif

#include <boost/function.hpp>

#include <GL/glew.h>

#include <openobjectlib/sg/constant_time_dispatcher.hpp>

#include <openobjectlib/sg/appearance.hpp>
#include <openobjectlib/sg/shaders.hpp>
#include <openobjectlib/sg/scene.hpp>

namespace olib { namespace openobjectlib { namespace sg {

class SG_DECLSPEC hw_GL_renderer
{
public:
	explicit hw_GL_renderer( );
	virtual ~hw_GL_renderer( );
	
public:
	bool render( const node_ptr& n );
	bool render_scene( const scene_ptr& n, bool clear = true );

public:
  	bool enable_texture( v_texture_ptr& v );
  	bool disable_texture( v_texture_ptr& v );
	bool disable_all_texture_units( );

	void enable_texture_transform( const openpluginlib::v_matrix4f& v );
	void disable_all_texture_transforms( );

	bool enable_GL_2_0_shader( v_shader_ptr& v );
	bool disable_GL_2_0_shader( );

	GLhandleARB  current_GL_2_0_program( ) const { return program_; }
	GLhandleARB& current_GL_2_0_program( )		 { return program_; }

	bool ffpe( ) const { return ffpe_; }

public:
	int  current_texture_unit( ) const	{ return unit_; }
	int& current_texture_unit( )		{ return unit_; }

	int  current_texture_unit_transform( ) const	{ return unit_transform_; }
	int& current_texture_unit_transform( )			{ return unit_transform_; }

private:
	typedef node_ptr argument_type;
	typedef bool return_type;
	typedef boost::function<return_type ( hw_GL_renderer& r, argument_type )> functor_type;

private:
	template<class U>
	void add( const functor_type& f )
	{ back_end_.template add<U>( f ); }

private:
	constant_time_dispatcher<return_type, argument_type, functor_type> back_end_;
	GLhandleARB program_;
	int unit_, unit_transform_;
	bool ffpe_;
	
private:
	typedef boost::function<return_type ( hw_GL_renderer& r, GLuint id, const shader_ptr& s, const openpluginlib::wstring& param )> GL_2_0_functor_type;
	
#if _MSC_VER >= 1400
	typedef stdext::hash_map<openpluginlib::wstring, GL_2_0_functor_type>	s_param_GL_dispatcher;
#else
	typedef std::map<openpluginlib::wstring, GL_2_0_functor_type>			s_param_GL_dispatcher;
#endif
	typedef s_param_GL_dispatcher::const_iterator	 s_param_GL_dispatcher_const_iterator;
	typedef s_param_GL_dispatcher::value_type		 s_param_GL_dispatcher_value_type;
	
	s_param_GL_dispatcher s_param_GL_dispatcher_;
};

typedef boost::shared_ptr<hw_GL_renderer> hw_GL_renderer_ptr;

SG_DECLSPEC hw_GL_renderer_ptr create_hw_GL_renderer( );

} } }

#endif
