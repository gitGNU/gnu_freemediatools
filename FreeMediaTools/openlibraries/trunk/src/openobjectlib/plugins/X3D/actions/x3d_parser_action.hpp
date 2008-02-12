
// sg - A scene graph representation.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef X3D_PARSER_ACTION_INC_
#define X3D_PARSER_ACTION_INC_

#include <map>
#include <stack>
#include <string>

#include <boost/filesystem/path.hpp>
#include <boost/shared_ptr.hpp>

#include <openobjectlib/sg/scene.hpp>

// forward declarations
#ifdef WIN32
struct MSXML2::ISAXAttributes;
#else
#include <libxml/parser.h>
#endif // WIN32

namespace olib { namespace openobjectlib { namespace sg { namespace actions {

typedef int ACTION_RETURN;

const ACTION_RETURN ACTION_RETURN_CONTINUE	= 0;
const ACTION_RETURN ACTION_RETURN_ERROR		= 1;
const ACTION_RETURN ACTION_RETURN_IGNORE	= 2;

class x3d_parser_action;

typedef ACTION_RETURN ( *x3d_dispatcher )( x3d_parser_action& pa );

class x3d_parser_action
{
public:
	typedef std::map<openpluginlib::wstring, x3d_dispatcher>	container;
	typedef boost::filesystem::path								path;

	explicit x3d_parser_action( );
	virtual ~x3d_parser_action( );

	ACTION_RETURN dispatch( const openpluginlib::wstring& node );

	virtual void start( const sg::node_ptr& node );
	virtual void finish( const sg::node_ptr& node );
				
	void			push( const sg::node_ptr& node );
	void			pop( );
	sg::node_ptr	top( );
	sg::scene_ptr	get_scene( );
				
	openpluginlib::wstring	get_current_texture_name( ) const;
	void					set_current_texture_name( const openpluginlib::wstring& name );
	
	path get_base_url( ) const;
	void set_base_url( const path& base_url );

	sg::node_ptr get_current_ifs( ) const;
	void		 set_current_ifs( const sg::node_ptr& ifs );

public:
#ifdef WIN32
	void set_attributes( MSXML2::ISAXAttributes __RPC_FAR* attributes );
#else
	void set_attributes( xmlChar** attributes );
#endif

public:
	sg::node_ptr	find( const openpluginlib::wstring& name ) const;
	void			insert( const sg::node_ptr& node );

private:
	x3d_parser_action( const x3d_parser_action& other );
	x3d_parser_action& operator=( const x3d_parser_action& other );

private:
	container					dispatch_;
	std::stack<sg::node_ptr>	st_;
	openpluginlib::wstring		name_;
	path						base_url_;
	sg::scene_ptr				scene_;
	sg::node_ptr				ifs_;

public:
#ifdef WIN32
	MSXML2::ISAXAttributes __RPC_FAR* attributes_;
#else
	xmlChar** attributes_;
#endif

private:
	typedef std::map<openpluginlib::wstring, sg::node_ptr> use_map_container;
	use_map_container use_map_;
};

ACTION_RETURN default_x3d_parser_action( x3d_parser_action& pa );
ACTION_RETURN field_x3d_parser_action( x3d_parser_action& pa );

// Cube map environmental texturing component.
ACTION_RETURN composed_cubemap_texture_x3d_parser_action( x3d_parser_action& pa );
ACTION_RETURN generated_cubemap_texture_x3d_parser_action( x3d_parser_action& pa );
ACTION_RETURN image_cubemap_texture_x3d_parser_action( x3d_parser_action& pa );
ACTION_RETURN pixel_cubemap_texture_x3d_parser_action( x3d_parser_action& pa );

// Geometry3D component.
ACTION_RETURN indexed_face_set_x3d_parser_action( x3d_parser_action& pa );

// Grouping component.
ACTION_RETURN group_x3d_parser_action( x3d_parser_action& pa );
ACTION_RETURN static_group_x3d_parser_action( x3d_parser_action& pa );
ACTION_RETURN switch_x3d_parser_action( x3d_parser_action& pa );
ACTION_RETURN transform_x3d_parser_action( x3d_parser_action& pa );

// Lighting component.
ACTION_RETURN directional_light_x3d_parser_action( x3d_parser_action& pa );
ACTION_RETURN point_light_x3d_parser_action( x3d_parser_action& pa );
ACTION_RETURN spot_light_x3d_parser_action( x3d_parser_action& pa );

// Navigation component.
ACTION_RETURN billboard_x3d_parser_action( x3d_parser_action& pa );
ACTION_RETURN collision_x3d_parser_action( x3d_parser_action& pa );
ACTION_RETURN lod_x3d_parser_action( x3d_parser_action& pa );
ACTION_RETURN viewpoint_x3d_parser_action( x3d_parser_action& pa );

// Networking component.
ACTION_RETURN anchor_x3d_parser_action( x3d_parser_action& pa );
ACTION_RETURN inline_x3d_parser_action( x3d_parser_action& pa );
ACTION_RETURN load_sensor_x3d_parser_action( x3d_parser_action& pa );

// Programmable Shaders component.
ACTION_RETURN composed_shader_x3d_parser_action( x3d_parser_action& pa );
ACTION_RETURN float_vertex_attribute_x3d_parser_action( x3d_parser_action& pa );
ACTION_RETURN matrix4_vertex_attribute_x3d_parser_action( x3d_parser_action& pa );
ACTION_RETURN packaged_shader_x3d_parser_action( x3d_parser_action& pa );
ACTION_RETURN shader_part_x3d_parser_action( x3d_parser_action& pa );

// Render Properties component.
ACTION_RETURN blend_x3d_parser_action( x3d_parser_action& pa );
ACTION_RETURN cull_mode_x3d_parser_action( x3d_parser_action& pa );
ACTION_RETURN depth_test_x3d_parser_action( x3d_parser_action& pa );
ACTION_RETURN front_face_x3d_parser_action( x3d_parser_action& pa );

// Rendering component.
ACTION_RETURN coordinate_x3d_parser_action( x3d_parser_action& pa );
ACTION_RETURN indexed_triangle_set_x3d_parser_action( x3d_parser_action& pa );
ACTION_RETURN indexed_triangle_strip_set_x3d_parser_action( x3d_parser_action& pa );
ACTION_RETURN normal_x3d_parser_action( x3d_parser_action& pa );

// Shape component.
ACTION_RETURN appearance_x3d_parser_action( x3d_parser_action& pa );
ACTION_RETURN material_x3d_parser_action( x3d_parser_action& pa );
ACTION_RETURN shape_x3d_parser_action( x3d_parser_action& pa );

// Texturing component.
ACTION_RETURN image_texture_x3d_parser_action( x3d_parser_action& pa );
ACTION_RETURN multi_texture_x3d_parser_action( x3d_parser_action& pa );
ACTION_RETURN multi_texture_coordinate_x3d_parser_action( x3d_parser_action& pa );
ACTION_RETURN texture_coordinate_x3d_parser_action( x3d_parser_action& pa );
ACTION_RETURN texture_coordinate_generator_x3d_parser_action( x3d_parser_action& pa );
ACTION_RETURN texture_transform_x3d_parser_action( x3d_parser_action& pa );

// Texturing3D component.
ACTION_RETURN composed3D_texture_x3d_parser_action( x3d_parser_action& pa );
ACTION_RETURN image3D_texture_x3d_parser_action( x3d_parser_action& pa );
ACTION_RETURN pixel3D_texture_x3d_parser_action( x3d_parser_action& pa );
ACTION_RETURN texture_coordinate3D_x3d_parser_action( x3d_parser_action& pa );
ACTION_RETURN texture_coordinate4D_x3d_parser_action( x3d_parser_action& pa );
ACTION_RETURN texture_matrix_transform_x3d_parser_action( x3d_parser_action& pa );
ACTION_RETURN texture_transform3D_x3d_parser_action( x3d_parser_action& pa );

} } } }

#endif
