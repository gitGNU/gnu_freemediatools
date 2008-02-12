
// sg - A scene graph representation.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifdef WIN32
#import <msxml6.dll> raw_interfaces_only
using namespace MSXML2;

#define _USE_MATH_DEFINES
#endif // WIN32

#if !defined( GCC_VERSION ) && defined( __GNUC__ )
#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#endif

#include <cassert>
#include <cmath>

#include <boost/filesystem/exception.hpp>

#include <openpluginlib/pl/utf8_utils.hpp>

#include <openobjectlib/sg/appearance.hpp>
#include <openobjectlib/sg/meshes.hpp>
#include <openobjectlib/sg/shaders.hpp>
#include <openobjectlib/sg/spatial.hpp>

#include <openobjectlib/plugins/X3D/actions/x3d_parser_action.hpp>
#include <openobjectlib/plugins/X3D/xml_value_tokenizer.hpp>

namespace fs  = boost::filesystem;
namespace sg  = olib::openobjectlib::sg;
namespace opl = olib::openpluginlib;

namespace olib { namespace openobjectlib { namespace sg { namespace actions {

namespace
{
	// insert the current directory as a search path for resources.
	// assumes the resource name is always the same.
	void expand_url( opl::m_url& url, const x3d_parser_action::path& base_url  )
	{
		if( !url.empty( ) )
			url.push_back( base_url / url[ 0 ] );
			
		fs::path u = base_url / url[ 0 ];
	}

	template<class R, class T>
	R create_node( const x3d_parser_action& pa )
	{
		std::vector<opl::wstring> value;
		if( xml_value_tokenizer( ).tokenize<opl::wstring>( L"USE", pa.attributes_, value ) )
			return boost::dynamic_pointer_cast<T>( pa.find( value[ 0 ] ) );

		return R( new T( ) );
	}

	struct viewpoint : public sg::node
	{
		explicit viewpoint( )
		{
			insert( L"DEF",					viewpoint::field_type( opl::v_string_typename,	 opl::v_string( ) ) );
			insert( L"centerOfRotation",	viewpoint::field_type( opl::v_vec3f_typename,	 opl::v_vec3f( ) ) );
			insert( L"description",			viewpoint::field_type( opl::v_string_typename,	 opl::v_string( ) ) );
			insert( L"fieldOfView",			viewpoint::field_type( opl::v_float_typename,	 opl::v_float( float( M_PI ) / 4.0f ) ) );
			insert( L"orientation",			viewpoint::field_type( opl::v_rotation_typename, opl::v_rotation( opl::axis_angle<float>( 0.0f, 0.0f, 1.0f, 0.0f ) ) ) );
			insert( L"position",			viewpoint::field_type( opl::v_vec3f_typename,	 opl::v_vec3f( ) ) );
		}
	};

	struct transform : public sg::node
	{
		explicit transform( )
		{
			insert( L"DEF",					transform::field_type( opl::v_string_typename,   opl::v_string( ) ) );
			insert( L"center",				transform::field_type( opl::v_vec3f_typename,	 opl::v_vec3f( ) ) );
			insert( L"rotation",			transform::field_type( opl::v_rotation_typename, opl::v_rotation( opl::axis_angle<float>( 0.0f, 0.0f, 1.0f, 0.0f ) ) ) );
			insert( L"scale",				transform::field_type( opl::v_vec3f_typename,	 opl::v_vec3f( opl::vec3f( 1.0f, 1.0f, 1.0f ) ) ) );
			insert( L"scaleOrientation",	transform::field_type( opl::v_rotation_typename, opl::v_rotation( opl::axis_angle<float>( 0.0f, 0.0f, 1.0f, 0.0f ) ) ) );
			insert( L"translation",			transform::field_type( opl::v_vec3f_typename,	 opl::v_vec3f( ) ) );
		}
	};

	struct texture_transform : public sg::node
	{
		explicit texture_transform( )
		{
			insert( L"DEF",					texture_transform::field_type( opl::v_string_typename,   opl::v_string( ) ) );
			insert( L"center",				texture_transform::field_type( opl::v_vec3f_typename,	 opl::v_vec3f( ) ) );
			insert( L"rotation",			texture_transform::field_type( opl::v_rotation_typename, opl::v_rotation( opl::axis_angle<float>( 0.0f, 0.0f, 1.0f, 0.0f ) ) ) );
			insert( L"scale",				texture_transform::field_type( opl::v_vec3f_typename,	 opl::v_vec3f( opl::vec3f( 1.0f, 1.0f, 1.0f ) ) ) );
			insert( L"translation",			texture_transform::field_type( opl::v_vec3f_typename,	 opl::v_vec3f( ) ) );
		}
	};

	typedef boost::shared_ptr<viewpoint> viewpoint_ptr;
	typedef boost::shared_ptr<transform> transform_ptr;
	typedef boost::shared_ptr<texture_transform> texture_transform_ptr;
}

x3d_parser_action::x3d_parser_action( )
	: scene_( new sg::scene( ) )
{
	dispatch_.insert( container::value_type( L"X3D",							default_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"Scene",							default_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"head",							default_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"component",						default_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"meta",							default_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"field",							field_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"ROUTE",							default_x3d_parser_action ) );
		
	// Cube map environmental texturing component.
	dispatch_.insert( container::value_type( L"ComposedCubeMapTexture",			composed_cubemap_texture_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"GeneratedCubeMapTexture",		generated_cubemap_texture_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"ImageCubeMapTexture",			image_cubemap_texture_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"PixelCubeMapTexture",			default_x3d_parser_action ) );
	
	// Environmental Effects component.
	dispatch_.insert( container::value_type( L"Background",						default_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"Fog",							default_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"TextureBackground",				default_x3d_parser_action ) );
	
	// Geometry3D component.
	dispatch_.insert( container::value_type( L"Box",							default_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"Cone",							default_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"Cylinder",						default_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"ElevationGrid",					default_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"Extrusion",						default_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"IndexedFaceSet",					indexed_face_set_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"Sphere",							default_x3d_parser_action ) );
				
	// Grouping component.
	dispatch_.insert( container::value_type( L"Group",							group_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"StaticGroup",					static_group_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"Switch",							switch_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"Transform",						transform_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"WorldInfo",						default_x3d_parser_action ) );
	
	// Interpolation component.
	dispatch_.insert( container::value_type( L"ColorInterpolator",				default_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"CoordinateInterpolator",			default_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"CoordinateInterpolator2D",		default_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"NormalInterpolator",				default_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"OrientationInterpolator",		default_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"PositionInterpolator",			default_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"PositionInterpolator2D",			default_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"ScalarInterpolator",				default_x3d_parser_action ) );
				
	// Lighting component.
	dispatch_.insert( container::value_type( L"DirectionalLight",				default_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"PointLight",						default_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"SpotLight",						default_x3d_parser_action ) );

	// Navigation component.
	dispatch_.insert( container::value_type( L"Billboard",						billboard_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"Collision",						collision_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"LOD",							lod_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"NavigationInfo",					default_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"Viewpoint",						viewpoint_x3d_parser_action ) );
	
	// Networking component.
	dispatch_.insert( container::value_type( L"Anchor",							anchor_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"Inline",							inline_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"LoadSensor",						load_sensor_x3d_parser_action ) );
	
	// Pointing Device Sensor component.
	dispatch_.insert( container::value_type( L"CylinderSensor",					default_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"PlaneSensor",					default_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"SphereSensor",					default_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"TouchSensor",					default_x3d_parser_action ) );
	
	// Programmable Shaders component.
	dispatch_.insert( container::value_type( L"ComposedShader",					composed_shader_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"FloatVertexAttribute",			float_vertex_attribute_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"Matrix4VertexAttribute",			default_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"PackagedShader",					default_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"ProgramShader",					default_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"ShaderPart",						shader_part_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"ShaderProgram",					default_x3d_parser_action ) );

	// Render Properties component
	dispatch_.insert( container::value_type( L"Blend",							blend_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"CullMode",						cull_mode_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"DepthTest",						depth_test_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"FrontFace",						front_face_x3d_parser_action ) );

	// Rendering component.
	dispatch_.insert( container::value_type( L"Color",							default_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"ColorRGBA",						default_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"Coordinate",						coordinate_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"IndexedLineSet",					default_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"IndexedTriangleFanSet",			default_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"IndexedTriangleSet",				indexed_triangle_set_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"IndexedTriangleStripSet",		indexed_triangle_strip_set_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"LineSet",						default_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"Normal",							normal_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"PointSet",						default_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"TriangleFanSet",					default_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"TriangleSet",					default_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"TriangleStripSet",				default_x3d_parser_action ) );
				
	// Script component.
	dispatch_.insert( container::value_type( L"Script",							default_x3d_parser_action ) );
	
	// Sound component.
	dispatch_.insert( container::value_type( L"AudioClip",						default_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"Sound",							default_x3d_parser_action ) );
				
	// Shape component.
	dispatch_.insert( container::value_type( L"Appearance",						appearance_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"FillProperties",					default_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"LineProperties",					default_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"Material",						material_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"Shape",							shape_x3d_parser_action ) );
				
	// Texturing component.
	dispatch_.insert( container::value_type( L"ImageTexture",					image_texture_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"MovieTexture",					default_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"MultiTextureCoordinate",			multi_texture_coordinate_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"MultiTexture",					multi_texture_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"MultiTextureTransform",			default_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"PixelTexture",					default_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"TextureCoordinate",				texture_coordinate_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"TextureCoordinateGenerator",		texture_coordinate_generator_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"TextureTransform",				texture_transform_x3d_parser_action ) );
				
	// Texturing3D component.
	dispatch_.insert( container::value_type( L"Composed3DTexture",				composed3D_texture_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"Image3DTexture",					image3D_texture_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"Pixel3DTexture",					pixel3D_texture_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"TextureCoordinate3D",			texture_coordinate3D_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"TextureCoordinate4D",			texture_coordinate4D_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"TextureMatrixTransform",			texture_matrix_transform_x3d_parser_action ) );
	dispatch_.insert( container::value_type( L"TextureTransform3D",				texture_transform3D_x3d_parser_action ) );
				
	// Time component.
	dispatch_.insert( container::value_type( L"TimeSensor",						default_x3d_parser_action ) );

	sg::spatial_ptr grp( new sg::group( ) );
	push( grp );
	
	scene_->set_root( grp );
}

x3d_parser_action::~x3d_parser_action( )
{ }

ACTION_RETURN x3d_parser_action::dispatch( const opl::wstring& node )
{
	typedef container::const_iterator const_iterator;
				
	const_iterator I = dispatch_.find( node );
	if( I != dispatch_.end( ) ) 
		return I->second( *this );

	assert( 0 && L"sg::x3d_parser_actions::x3d_parser_action invalid node." );
				
	return ACTION_RETURN_ERROR;
}

void x3d_parser_action::start( const sg::node_ptr& /*node*/ )
{
	while( !st_.empty( ) ) 
		st_.pop( );
				
	set_current_texture_name( L"" );
	
	use_map_.clear( );
}

void x3d_parser_action::finish( const sg::node_ptr& /*node*/ )
{ }

void x3d_parser_action::push( const sg::node_ptr& node )
{ st_.push( node ); }

void x3d_parser_action::pop( )
{ st_.pop( ); }

sg::node_ptr x3d_parser_action::top( )
{
	assert( !st_.empty( ) && L"sg::x3d_parser_actions::x3d_parser_action stack is empty." );
 
	return st_.top( );
}

sg::scene_ptr x3d_parser_action::get_scene( )
{ return scene_; }

opl::wstring x3d_parser_action::get_current_texture_name( ) const
{ return name_; }

void x3d_parser_action::set_current_texture_name( const opl::wstring& name )
{ name_ = name; }

x3d_parser_action::path x3d_parser_action::get_base_url( ) const
{ return base_url_; }

void x3d_parser_action::set_base_url( const x3d_parser_action::path& base_url )
{ base_url_ = base_url; }

sg::node_ptr x3d_parser_action::get_current_ifs( ) const
{ return ifs_; }

void x3d_parser_action::set_current_ifs( const sg::node_ptr& ifs )
{ ifs_ = ifs; }

#ifdef WIN32
void x3d_parser_action::set_attributes( ISAXAttributes __RPC_FAR* attributes )
#else
void x3d_parser_action::set_attributes( xmlChar** attributes )
#endif
{
	attributes_ = attributes;
}

sg::node_ptr x3d_parser_action::find( const opl::wstring& name ) const
{
	typedef use_map_container::const_iterator const_iterator;
	
	const_iterator I = use_map_.find( name );
	if( I != use_map_.end( ) )
		return I->second;
		
	return sg::node_ptr( );
}

void x3d_parser_action::insert( const sg::node_ptr& node )
{
	opl::v_string* def = node->pointer<opl::v_string>( L"DEF" );
	if( def && !def->value( ).empty( ) )
		use_map_.insert( use_map_container::value_type( def->value( ), node ) );
}

template<class T, bool shader = false>
struct assign_field
{ };
		
template<class T>
struct assign_field<opl::multi_value_property<T> >
{
	typedef opl::multi_value_property<T>	field_type;
	typedef typename field_type::value_type value_type;

	void assign( const x3d_parser_action& pa, const sg::node_ptr& node, const opl::wstring& name, const opl::wstring& xml_name = L"" )
	{
#if _MSC_VER >= 1310 || GCC_VERSION >= 30400
		field_type* field = node->pointer<field_type>( name );

		std::vector<value_type> value;
		if( xml_value_tokenizer( ).tokenize<value_type>( xml_name.empty( ) ? name : xml_name, pa.attributes_, value ) )
		{
			field->resize( value.size( ) );
			memcpy( field->data( ), &value[ 0 ], value.size( ) * sizeof( T ) );
		}
#endif
	}
};

template<>
struct assign_field<opl::m_url>
{
	typedef opl::m_url		field_type;
	typedef opl::wstring	value_type;
			
	void assign( const x3d_parser_action& pa, const sg::node_ptr& node, const opl::wstring& name, const opl::wstring& xml_name = L"" )
	{
		field_type* field = node->pointer<field_type>( name );
				
		std::vector<value_type> value;
		if( xml_value_tokenizer( ).tokenize<value_type>( xml_name.empty( ) ? name : xml_name, pa.attributes_, value ) )
		{
			typedef std::vector<value_type>::iterator iterator;
			
			field->reserve( value.size( ) );
					
			for( iterator I = value.begin( ); I != value.end( ); ++I )
			{
				// trap boost::filesystem exceptions for now til we get the bugger to accept urls.
				try
				{
					field->push_back( fs::path( opl::to_string( *I ).c_str( ), fs::native ) );
				} 
				catch( fs::filesystem_error& )
				{
				}
			}
		}
	}
};

template<class T>
struct assign_field<opl::value_property<T> >
{
	typedef opl::value_property<T> field_type;

	void assign( const x3d_parser_action& pa, const sg::node_ptr& node, const opl::wstring& name, const opl::wstring& xml_name = L"" )
	{
#if _MSC_VER >= 1310 || GCC_VERSION >= 30400
		field_type* field = node->pointer<field_type>( name );

		std::vector<float> value;
		if( xml_value_tokenizer( ).tokenize<float>( xml_name.empty( ) ? name : xml_name, pa.attributes_, value ) )
		{
			for( size_t i = 0; i < value.size( ); ++i )
				field->get( )[ i ] = value[ i ];
		}
#endif
	}
};

template<class T>
struct assign_field<opl::value_property<T>, true>
{
	typedef opl::value_property<T> field_type;

	void assign( const x3d_parser_action& pa, const sg::shader_ptr& node, const opl::wstring& name, const opl::wstring& xml_name = L"" )
	{
#if _MSC_VER >= 1310 || GCC_VERSION >= 30400
		field_type* field = node->pointer_param<field_type>( name );

		std::vector<float> value;
		if( xml_value_tokenizer( ).tokenize<float>( xml_name.empty( ) ? name : xml_name, pa.attributes_, value ) )
		{
			for( size_t i = 0; i < value.size( ); ++i )
				field->get( )[ i ] = value[ i ];
		}
#endif
	}
};

template<>
struct assign_field<opl::v_float>
{
	void assign( const x3d_parser_action& pa, const sg::node_ptr& node, const opl::wstring& name, const opl::wstring& xml_name = L"" )
	{
		opl::v_float* field = node->pointer<opl::v_float>( name );

		std::vector<float> value;
		if( xml_value_tokenizer( ).tokenize<float>( xml_name.empty( ) ? name : xml_name, pa.attributes_, value ) )
		{
			if( value.empty( ) )
				return;

			field->get( ) = value[ 0 ];
		}
	}
};

template<>
struct assign_field<opl::v_float, true>
{
	void assign( const x3d_parser_action& pa, const sg::shader_ptr& node, const opl::wstring& name, const opl::wstring& xml_name = L"" )
	{
		opl::v_float* field = node->pointer_param<opl::v_float>( name );

		std::vector<float> value;
		if( xml_value_tokenizer( ).tokenize<float>( xml_name.empty( ) ? name : xml_name, pa.attributes_, value ) )
		{
			if( value.empty( ) )
				return;

			field->get( ) = value[ 0 ];
		}
	}
};

template<>
struct assign_field<opl::v_int32>
{
	void assign( const x3d_parser_action& pa, const sg::node_ptr& node, const opl::wstring& name, const opl::wstring& xml_name = L"" )
	{
		opl::v_int32* field = node->pointer<opl::v_int32>( name );

		std::vector<int> value;
		if( xml_value_tokenizer( ).tokenize<int>( xml_name.empty( ) ? name : xml_name, pa.attributes_, value ) )
		{
			if( value.empty( ) )
				return;

			field->get( ) = value[ 0 ];
		}
	}
};

template<>
struct assign_field<opl::v_int32, true>
{
	void assign( const x3d_parser_action& pa, const sg::shader_ptr& node, const opl::wstring& name, const opl::wstring& xml_name = L"" )
	{
		opl::v_int32* field = node->pointer_param<opl::v_int32>( name );

		std::vector<int> value;
		if( xml_value_tokenizer( ).tokenize<int>( xml_name.empty( ) ? name : xml_name, pa.attributes_, value ) )
		{
			if( value.empty( ) )
				return;

			field->get( ) = value[ 0 ];
		}
	}
};

template<>
struct assign_field<opl::v_string>
{
	void assign( const x3d_parser_action& pa, const sg::node_ptr& node, const opl::wstring& name, const opl::wstring& xml_name = L"" )
	{
		opl::v_string* field = node->pointer<opl::v_string>( name );

		std::vector<opl::wstring> value;
		if( xml_value_tokenizer( ).tokenize<opl::wstring>( xml_name.empty( ) ? name : xml_name, pa.attributes_, value ) )
		{
			if( value.empty( ) )
				return;

			field->get( ).assign( value[ 0 ] );
		}
	}
};

template<>
struct assign_field<opl::v_string, true>
{
	void assign( const x3d_parser_action& pa, const sg::shader_ptr& node, const opl::wstring& name, const opl::wstring& xml_name = L"" )
	{
		opl::v_string* field = node->pointer_param<opl::v_string>( name );

		std::vector<opl::wstring> value;
		if( xml_value_tokenizer( ).tokenize<opl::wstring>( xml_name.empty( ) ? name : xml_name, pa.attributes_, value ) )
		{
			if( value.empty( ) )
				return;

			field->get( ).assign( value[ 0 ] );
		}
	}
};

/////////////////////////////////////////////////////
// special case is needed since boost::lexical_cast 
// does not recognize either "false" or "true".
template<>
struct assign_field<opl::v_bool>
{
	typedef opl::v_bool				field_type;
	typedef field_type::value_type	value_type;

	void assign( const x3d_parser_action& pa, const sg::node_ptr& node, const opl::wstring& name, const opl::wstring& xml_name = L"" )
	{
		field_type* field = node->pointer<field_type>( name );
		
		opl::wstring name_( xml_name.empty( ) ? name : xml_name );

#ifdef WIN32
		wchar_t* value = NULL;
		int length;

		if( SUCCEEDED(	pa.attributes_->getValueFromName(
						( unsigned short*  ) L"", 0,
						( unsigned short*  ) name_.c_str( ), ( int ) name_.size( ),
						( unsigned short** ) &value, &length ) ) )
#else
		opl::wstring str;
		if( xml_value_tokenizer( ).get_value_from_name( name_, pa.attributes_, str ) )
#endif
		{
#ifdef WIN32
			opl::wstring str( value, length );
#endif
			field->get( ) = wcscmp( str.c_str( ), L"true" ) == 0 ? true : false;
		}
	}
};

template<>
struct assign_field<opl::v_matrix4f>
{
	typedef opl::v_matrix4f			field_type;
	typedef field_type::value_type	value_type;

	void assign( const x3d_parser_action& pa, const sg::node_ptr& node, const opl::wstring& name, const opl::wstring& xml_name = L"" )
	{
		field_type* field = node->pointer<field_type>( name );

		std::vector<float> value;
		if( xml_value_tokenizer( ).tokenize<float>( xml_name.empty( ) ? name : xml_name, pa.attributes_, value ) )
			field->get( ) = opl::matrixf( &value[ 0 ] );
	}
};

template<>
struct assign_field<opl::v_url>
{
	typedef opl::v_url		field_type;
	typedef opl::wstring	value_type;

	void assign( x3d_parser_action& pa, const sg::node_ptr& node, const opl::wstring& name, const opl::wstring& xml_name = L"" )
	{
		field_type* field = node->pointer<field_type>( name );

		std::vector<value_type> value;
		if( xml_value_tokenizer( ).tokenize<value_type>( xml_name.empty( ) ? name : xml_name, pa.attributes_, value ) )
		{
			typedef opl::v_string::value_type::iterator iterator;
			
			if( value.empty( ) ) return;
			
			field->get( ) = fs::path( opl::to_string( value[ 0 ] ).c_str( ), fs::native );
		}
	}
};

ACTION_RETURN default_x3d_parser_action( x3d_parser_action& pa )
{
	// very big hack to bypass our push and pop scheme.
	pa.push( pa.top( ) );

	return ACTION_RETURN_CONTINUE;
}

ACTION_RETURN field_x3d_parser_action( x3d_parser_action& pa )
{
	sg::shader_ptr top = boost::shared_dynamic_cast<sg::shader>( pa.top( ) );
	
	pa.push( top );
	
	if( !top )
		return ACTION_RETURN_CONTINUE;

	std::vector<opl::wstring> name, type, value;
	xml_value_tokenizer( ).tokenize<opl::wstring>( L"name", pa.attributes_, name );
	xml_value_tokenizer( ).tokenize<opl::wstring>( L"type", pa.attributes_, type );

	if( name.empty( ) || type.empty( ) ) return ACTION_RETURN_CONTINUE;
	
	if( type[ 0 ] == opl::m_float_typename )
	{
	}
	else if( type[ 0 ] == opl::m_int32_typename )
	{
	}
	else if( type[ 0 ] == opl::m_string_typename )
	{
	}
	else if( type[ 0 ] == opl::v_bool_typename )
	{
		top->insert_param( name[ 0 ], sg::shader::field_type( type[ 0 ], opl::v_bool( ) ) );
	}
	else if( type[ 0 ] == opl::v_color_typename )
	{
		top->insert_param( name[ 0 ], sg::shader::field_type( type[ 0 ], opl::v_color( ) ) );
		assign_field<opl::v_color, true>( ).assign( pa, top, name[ 0 ], L"value" );
	}
	else if( type[ 0 ] == opl::v_float_typename )
	{
		top->insert_param( name[ 0 ], sg::shader::field_type( type[ 0 ], opl::v_float( ) ) );
		assign_field<opl::v_float, true>( ).assign( pa, top, name[ 0 ], L"value" );
	}
	else if( type[ 0 ] == opl::v_int32_typename )
	{
		top->insert_param( name[ 0 ], sg::shader::field_type( type[ 0 ], opl::v_int32( ) ) );
		assign_field<opl::v_int32, true>( ).assign( pa, top, name[ 0 ], L"value" );
	}
	else if( type[ 0 ] == opl::v_matrix4f_typename )
	{
		top->insert_param( name[ 0 ], sg::shader::field_type( type[ 0 ], opl::v_matrix4f( ) ) );
		assign_field<opl::v_matrix4f, true>( ).assign( pa, top, name[ 0 ], L"value" );
	}
	else if( type[ 0 ] == v_node_typename )
	{
		top->insert_param( name[ 0 ], sg::shader::field_type( type[ 0 ], v_texture_ptr( ) ) );
		pa.set_current_texture_name( name[ 0 ] );
	}
	else if( type[ 0 ] == opl::v_rotation_typename )
	{
		top->insert_param( name[ 0 ], sg::shader::field_type( type[ 0 ], opl::v_rotation( ) ) );
		assign_field<opl::v_rotation, true>( ).assign( pa, top, name[ 0 ], L"value" );
	}
	else if( type[ 0 ] == opl::v_string_typename )
	{
		top->insert_param( name[ 0 ], sg::shader::field_type( type[ 0 ], opl::v_string( ) ) );
		assign_field<opl::v_string, true>( ).assign( pa, top, name[ 0 ], L"value" );
	}
	else if( type[ 0 ] == opl::v_vec2f_typename )
	{
		top->insert_param( name[ 0 ], sg::shader::field_type( type[ 0 ], opl::v_vec2f( ) ) );
		assign_field<opl::v_vec2f, true>( ).assign( pa, top, name[ 0 ], L"value" );
	}
	else if( type[ 0 ] == opl::v_vec3f_typename )
	{
		top->insert_param( name[ 0 ], sg::shader::field_type( type[ 0 ], opl::v_vec3f( ) ) );
		assign_field<opl::v_vec3f, true>( ).assign( pa, top, name[ 0 ], L"value" );
	}
	else if( type[ 0 ] == opl::v_vec4f_typename )
	{
		top->insert_param( name[ 0 ], sg::shader::field_type( type[ 0 ], opl::v_vec4f( ) ) );
		assign_field<opl::v_vec4f, true>( ).assign( pa, top, name[ 0 ], L"value" );
	}

	return ACTION_RETURN_CONTINUE;
}
			
// Cube map environmental texturing component.
ACTION_RETURN composed_cubemap_texture_x3d_parser_action( x3d_parser_action& pa )
{
/*
	opl::MFNode* texture = pa.top( )->pointer<opl::MFNode>( L"texture" );
				
	sg::spatial_ptr cct( create_node( L"ComposedCubeMapTexture", pa ) );
	texture->push_back( cct );

	assign_field<opl::v_string>( ).assign( pa, cct, L"DEF" );
			
	pa.push( cct );
*/		
	return ACTION_RETURN_CONTINUE;
}

ACTION_RETURN generated_cubemap_texture_x3d_parser_action( x3d_parser_action& pa )
{
/*
	opl::MFNode* texture = pa.top( )->pointer<opl::MFNode>( L"texture" );
				
	sg::spatial_ptr gct( create_node( L"GeneratedCubeMapTexture", pa ) );
	texture->push_back( gct );
				
	assign_field<opl::v_string>( ).assign( pa, gct, L"DEF" );
	assign_field<opl::v_string>( ).assign( pa, gct, L"update" );
	assign_field<opl::SFInt32>( ).assign(  pa, gct, L"size" );
				
	pa.push( gct );
*/	
	return ACTION_RETURN_CONTINUE;
}

ACTION_RETURN image_cubemap_texture_x3d_parser_action( x3d_parser_action& pa )
{
	sg::texture_ptr ict( new sg::texture( ) );

	assign_field<opl::v_string>( ).assign( pa, ict, L"DEF" );
	assign_field<opl::m_url>( ).assign(	   pa, ict, L"url" );
	assign_field<opl::v_bool>( ).assign(   pa, ict, L"repeatS" );
	assign_field<opl::v_bool>( ).assign(   pa, ict, L"repeatT" );
	assign_field<opl::v_bool>( ).assign(   pa, ict, L"repeatR" );

/*
	opl::MFNode* texture = pa.top( )->pointer<opl::MFNode>( L"texture" );
	if( texture )
	{
		texture->push_back( ict );
	}
	else
*/
	{
		sg::shader_ptr top = boost::shared_dynamic_cast<sg::shader>( pa.top( ) );
		v_texture_ptr* node = top->pointer_param<v_texture_ptr>( pa.get_current_texture_name( ) );
		if( node )
			node->get( ) = ict;
	}

	opl::v_bool* cubemap = ict->pointer<opl::v_bool>( L"cubemap" );
	opl::m_url*  url	 = ict->pointer<opl::m_url>( L"url" );
	
	cubemap->get( ) = true;
	expand_url( *url, pa.get_base_url( ) );
	
	ict->update( );

	pa.push( ict );

	return ACTION_RETURN_CONTINUE;
}

// Geometry3D component.
ACTION_RETURN indexed_face_set_x3d_parser_action( x3d_parser_action& pa )
{
	sg::v_mesh_ptr* geo = pa.top( )->pointer<sg::v_mesh_ptr>( L"geometry" );

	sg::multi_index_mesh_ptr m( new sg::multi_index_mesh( ) );
	m->prim_type( ) = sg::mesh::POLYGONS;
	
	geo->get( ) = m;

	assign_field<opl::v_string>( ).assign( pa, m, L"DEF" );
	assign_field<opl::v_bool>( ).assign(   pa, m, L"ccw" );
	assign_field<opl::m_int32>( ).assign(  pa, m, L"colorIndex" );
	assign_field<opl::v_bool>( ).assign(   pa, m, L"colorPerVertex" );
	assign_field<opl::v_bool>( ).assign(   pa, m, L"convex" );
	assign_field<opl::m_int32>( ).assign(  pa, m, L"index", L"coordIndex" );
	assign_field<opl::v_float>( ).assign(  pa, m, L"creaseAngle" );
	assign_field<opl::m_int32>( ).assign(  pa, m, L"normalIndex" );
	assign_field<opl::v_bool>( ).assign(   pa, m, L"normalPerVertex" );
	assign_field<opl::v_bool>( ).assign(   pa, m, L"solid" );
	assign_field<opl::m_int32>( ).assign(  pa, m, L"texCoordIndex" );
	
	opl::m_int32* ci = m->pointer<opl::m_int32>( L"index" );
	
	opl::m_int32* ni = m->pointer<opl::m_int32>( L"normalIndex" );
	opl::m_int32* li = m->pointer<opl::m_int32>( L"colorIndex" );
	
	opl::v_bool npv = m->value<opl::v_bool>( L"normalPerVertex" );
	opl::v_bool cpv = m->value<opl::v_bool>( L"colorPerVertex" );
	
	if( ni->empty( ) && !npv.value( ) )
	{
		for( size_t i = 0, j = 0; i < ci->size( ); ++i )
			if( ( *ci )[ i ] == -1 )
				ni->push_back( j ), ++j;
	}

	if( li->empty( ) && !cpv.value( ) )
	{
		for( size_t i = 0, j = 0; i < ci->size( ); ++i )
			if( ( *ci )[ i ] == -1 )
				li->push_back( j ), ++j;
	}

	pa.push( m );

	return ACTION_RETURN_CONTINUE;
}

// Grouping component.
ACTION_RETURN group_x3d_parser_action( x3d_parser_action& pa )
{
	sg::m_spatial_ptr* fc = pa.top( )->pointer<sg::m_spatial_ptr>( L"children" );

	sg::spatial_ptr grp = create_node<sg::spatial_ptr, sg::group>( pa );
	fc->push_back( grp );

	assign_field<opl::v_string>( ).assign( pa, grp, L"DEF" );

	pa.insert( grp );

	pa.push( grp );

	return ACTION_RETURN_CONTINUE;
}

ACTION_RETURN static_group_x3d_parser_action( x3d_parser_action& pa )
{
	sg::m_spatial_ptr* fc = pa.top( )->pointer<sg::m_spatial_ptr>( L"children" );
				
	sg::spatial_ptr grp = create_node<sg::spatial_ptr, sg::group>( pa );
	fc->push_back( grp );
				
	assign_field<opl::v_string>( ).assign( pa, grp, L"DEF" );

	pa.insert( grp );

	pa.push( grp );

	return ACTION_RETURN_CONTINUE;
}

ACTION_RETURN switch_x3d_parser_action( x3d_parser_action& pa )
{
	sg::m_spatial_ptr* fc = pa.top( )->pointer<sg::m_spatial_ptr>( L"children" );

	sg::spatial_ptr sw = create_node<sg::spatial_ptr, sg::select>( pa );
	fc->push_back( sw );

	assign_field<opl::v_string>( ).assign( pa, sw, L"DEF" );
	assign_field<opl::v_int32>( ).assign(  pa, sw, L"choice" );

	pa.insert( sw );

	pa.push( sw );

	return ACTION_RETURN_CONTINUE;
}

ACTION_RETURN transform_x3d_parser_action( x3d_parser_action& pa )
{
	sg::m_spatial_ptr* children = pa.top( )->pointer<sg::m_spatial_ptr>( L"children" );
				
	transform_ptr t( new transform( ) );
				
	assign_field<opl::v_string>( ).assign(   pa, t, L"DEF" );
	assign_field<opl::v_vec3f>( ).assign(    pa, t, L"center" );
	assign_field<opl::v_rotation>( ).assign( pa, t, L"rotation" );
	assign_field<opl::v_vec3f>( ).assign(    pa, t, L"scale" );
	assign_field<opl::v_rotation>( ).assign( pa, t, L"scaleOrientation" );
	assign_field<opl::v_vec3f>( ).assign(    pa, t, L"translation" );

	opl::v_string	d = t->value<opl::v_string>(	L"DEF" );
	opl::v_vec3f	c = t->value<opl::v_vec3f>(		L"center" );
	opl::v_rotation r = t->value<opl::v_rotation>(  L"rotation" );
	opl::v_vec3f	s = t->value<opl::v_vec3f>(		L"scale" );
	opl::v_rotation o = t->value<opl::v_rotation>(  L"scaleOrientation" );
	opl::v_vec3f	l = t->value<opl::v_vec3f>(		L"translation" );

	opl::axis_angle<float> mso( o.value( ) );
	mso[ 3 ] = -mso[ 3 ];
	
	opl::matrixf c_mat	 = opl::make_translation( -c.value( ) );
	opl::matrixf r_mat	 = opl::from_axis_angle_rotation( r.value( ) );
	opl::matrixf s_mat	 = opl::make_scale( s.value( ) );
	opl::matrixf so_mat	 = opl::from_axis_angle_rotation( o.value( ) );
	opl::matrixf mso_mat = opl::from_axis_angle_rotation( mso );
	opl::matrixf t_mat	 = opl::make_translation( c.value( ) + l.value( ) );

	opl::matrixf f_mat = t_mat * r_mat * so_mat * s_mat * mso_mat * c_mat;

	sg::spatial_ptr tm = create_node<sg::spatial_ptr, sg::transform>( pa );
	children->push_back( tm );

	opl::v_matrix4f* fm = tm->pointer<opl::v_matrix4f>( L"matrix" );
	fm->get( ) = f_mat;

	pa.insert( tm );

	pa.push( tm );

	return ACTION_RETURN_CONTINUE;
}
			
// Lighting component.
ACTION_RETURN directional_light_x3d_parser_action( x3d_parser_action& pa )
{
/*
	opl::MFNode* children = pa.top( )->pointer<opl::MFNode>( L"children" );
				
	sg::spatial_ptr dl( create_node( L"DirectionalLight", pa ) );
	children->push_back( dl );
				
	assign_field<opl::v_string>( ).assign( pa, dl, L"DEF" );
	assign_field<opl::SFFloat>( ).assign(  pa, dl, L"ambientIntensity" );
	assign_field<opl::v_color>( ).assign(  pa, dl, L"color" );
	assign_field<opl::v_vec3f>( ).assign(  pa, dl, L"direction" );
	assign_field<opl::v_bool>( ).assign(   pa, dl, L"global" );
	assign_field<opl::SFFloat>( ).assign(  pa, dl, L"intensity" );
	assign_field<opl::v_bool>( ).assign(   pa, dl, L"on" );
				
	pa.push( dl );
*/				
	return ACTION_RETURN_CONTINUE;
}

ACTION_RETURN point_light_x3d_parser_action( x3d_parser_action& pa )
{
/*
	opl::MFNode* children = pa.top( )->pointer<opl::MFNode>( L"children" );
				
	sg::spatial_ptr dl( create_node( L"PointLight", pa ) );
	children->push_back( dl );
				
	assign_field<opl::v_string>( ).assign( pa, dl, L"DEF" );
	assign_field<opl::SFFloat>( ).assign(  pa, dl, L"ambientIntensity" );
	assign_field<opl::v_vec3f>( ).assign(  pa, dl, L"attenuation" );
	assign_field<opl::v_color>( ).assign(  pa, dl, L"color" );
	assign_field<opl::v_bool>( ).assign(   pa, dl, L"global" );
	assign_field<opl::SFFloat>( ).assign(  pa, dl, L"intensity" );
	assign_field<opl::v_vec3f>( ).assign(  pa, dl, L"location" );
	assign_field<opl::v_bool>( ).assign(   pa, dl, L"on" );
	assign_field<opl::SFFloat>( ).assign(  pa, dl, L"radius" );
				
	pa.push( dl );
*/				
	return ACTION_RETURN_CONTINUE;
}

ACTION_RETURN spot_light_x3d_parser_action( x3d_parser_action& pa )
{
/*
	opl::MFNode* children = pa.top( )->pointer<opl::MFNode>( L"children" );
				
	sg::spatial_ptr dl( create_node( L"SpotLight", pa ) );
	children->push_back( dl );
				
	assign_field<opl::v_string>( ).assign( pa, dl, L"DEF" );
	assign_field<opl::SFFloat>( ).assign(  pa, dl, L"ambientIntensity" );
	assign_field<opl::v_vec3f>( ).assign(  pa, dl, L"attenuation" );
	assign_field<opl::SFFloat>( ).assign(  pa, dl, L"beamWidth" );
	assign_field<opl::v_color>( ).assign(  pa, dl, L"color" );
	assign_field<opl::SFFloat>( ).assign(  pa, dl, L"cutOffAngle" );
	assign_field<opl::v_vec3f>( ).assign(  pa, dl, L"direction" );
	assign_field<opl::v_bool>( ).assign(   pa, dl, L"global" );
	assign_field<opl::SFFloat>( ).assign(  pa, dl, L"intensity" );
	assign_field<opl::v_vec3f>( ).assign(  pa, dl, L"location" );
	assign_field<opl::v_bool>( ).assign(   pa, dl, L"on" );
	assign_field<opl::SFFloat>( ).assign(  pa, dl, L"radius" );
				
	pa.push( dl );
*/				
	return ACTION_RETURN_CONTINUE;
}
			
// Navigation component.
ACTION_RETURN billboard_x3d_parser_action( x3d_parser_action& pa )
{
/*
	sg::spatial_ptr b( create_node( L"Billboard", pa ) );
				
	pa.push( b );
*/				
	return ACTION_RETURN_CONTINUE;
}

ACTION_RETURN collision_x3d_parser_action( x3d_parser_action& pa )
{
/*
	sg::spatial_ptr c( create_node( L"Collision", pa ) );
				
	pa.push( c );
*/				
	return ACTION_RETURN_CONTINUE;
}

ACTION_RETURN lod_x3d_parser_action( x3d_parser_action& pa )
{
/*
	sg::spatial_ptr l( create_node( L"LOD", pa ) );
				
	pa.push( l );
*/				
	return ACTION_RETURN_CONTINUE;
}

ACTION_RETURN viewpoint_x3d_parser_action( x3d_parser_action& pa )
{
	sg::m_spatial_ptr* children = pa.top( )->pointer<sg::m_spatial_ptr>( L"children" );

	viewpoint_ptr v( new viewpoint( ) );

	assign_field<opl::v_string>( ).assign(		pa, v, L"DEF" );
	assign_field<opl::v_vec3f>( ).assign(		pa, v, L"centerOfRotation" );
	assign_field<opl::v_string>( ).assign(		pa, v, L"description" );
	assign_field<opl::v_float>( ).assign(		pa, v, L"fieldOfView" );
	assign_field<opl::v_rotation>( ).assign(	pa, v, L"orientation" );
	assign_field<opl::v_vec3f>( ).assign(		pa, v, L"position" );

	opl::v_float	yfov	= v->value<opl::v_float>(	 L"fieldOfView" );
	opl::v_rotation orient	= v->value<opl::v_rotation>( L"orientation" );
	opl::v_vec3f	pos		= v->value<opl::v_vec3f>(	 L"position" );

	opl::matrixf view = opl::from_axis_angle_rotation( orient.value( ) );
	opl::vec4f	 up	  = view * opl::vec4f( 0.0f, 1.0f, 0.0f, 0.0f );
	
	sg::camera_ptr c = create_node<sg::camera_ptr, sg::camera>( pa );
	children->push_back( c );

	c->perspective( yfov.value( ) * 180.0f / static_cast<float>( M_PI ), 1, 0.1f, 1000.0f );
	c->look_at( pos.value( ),
				opl::vec3f( ),
				opl::vec3f( up[ 0 ], up[ 1 ], up[ 2 ] ) );
	pa.get_scene( )->add_camera( c );

	pa.insert( c );

	pa.push( c );

	return ACTION_RETURN_CONTINUE;
}

// Networking component.
ACTION_RETURN anchor_x3d_parser_action( x3d_parser_action& pa )
{
/*
	opl::MFNode* children = pa.top( )->pointer<opl::MFNode>( L"children" );

	sg::spatial_ptr a( create_node( L"Anchor", pa ) );
	children->push_back( a );
	
	assign_field<opl::v_string>( ).assign( pa, a, L"DEF" );
	assign_field<opl::v_string>( ).assign( pa, a, L"description" );
	assign_field<opl::m_string>( ).assign( pa, a, L"parameter" );
	assign_field<opl::m_url>( ).assign(	pa, a, L"url" );
	assign_field<opl::v_vec3f>( ).assign(  pa, a, L"bboxCenter" );
	assign_field<opl::v_vec3f>( ).assign(  pa, a, L"bboxSize" );

	pa.push( a );
*/				
	return ACTION_RETURN_CONTINUE;
}

ACTION_RETURN inline_x3d_parser_action( x3d_parser_action& pa )
{
/*
	opl::MFNode* children = pa.top( )->pointer<opl::MFNode>( L"children" );

	sg::spatial_ptr i( create_node( L"Inline", pa ) );
	children->push_back( i );
	
	assign_field<opl::v_string>( ).assign( pa, i, L"DEF" );
	assign_field<opl::v_bool>( ).assign(   pa, i, L"load" );
	assign_field<opl::m_url>( ).assign(	pa, i, L"url" );
	assign_field<opl::v_vec3f>( ).assign(  pa, i, L"bboxCenter" );
	assign_field<opl::v_vec3f>( ).assign(  pa, i, L"bboxSize" );
				
	pa.push( i );
*/				
	return ACTION_RETURN_CONTINUE;
}

ACTION_RETURN load_sensor_x3d_parser_action( x3d_parser_action& pa )
{
/*
	opl::MFNode* children = pa.top( )->pointer<opl::MFNode>( L"children" );

	sg::spatial_ptr l( create_node( L"LoadSensor", pa ) );
	children->push_back( l );
	
	assign_field<opl::v_string>( ).assign( pa, l, L"DEF" );
	assign_field<opl::v_bool>( ).assign(   pa, l, L"enabled" );
	assign_field<opl::SFTime>( ).assign(   pa, l, L"timeOut" );
				
	pa.push( l );
*/				
	return ACTION_RETURN_CONTINUE;
}

// Programmable Shaders component.
ACTION_RETURN composed_shader_x3d_parser_action( x3d_parser_action& pa )
{
	sg::v_shader_ptr* shader = pa.top( )->pointer<sg::v_shader_ptr>( L"shader" );

	sg::composed_shader_ptr c = create_node<sg::composed_shader_ptr, sg::composed_shader>( pa );
	shader->get( ) = c;

	assign_field<opl::v_string>( ).assign( pa, c, L"DEF" );
	assign_field<opl::v_string>( ).assign( pa, c, L"language" );

	pa.insert( c );

	pa.push( c );

	return ACTION_RETURN_CONTINUE;
}
			
ACTION_RETURN float_vertex_attribute_x3d_parser_action( x3d_parser_action& pa )
{
	sg::mv_attribute_array_ptr* attr = pa.top( )->pointer<sg::mv_attribute_array_ptr>( L"attrib" );

	sg::attribute_array_ptr arr = create_node<sg::attribute_array_ptr, sg::attribute_array>( pa );
	attr->push_back( sg::v_attribute_array_ptr( arr ) );

	assign_field<opl::m_float>( ).assign( pa, arr, L"value" );
	assign_field<opl::v_int32>( ).assign( pa, arr, L"components" );
	assign_field<opl::v_string>( ).assign( pa, arr, L"name" );

	pa.insert( arr );

	pa.push( arr );

	return ACTION_RETURN_CONTINUE;
}

ACTION_RETURN packaged_shader_x3d_parser_action( x3d_parser_action& pa )
{
/*
	opl::MFNode* attrib = pa.top( )->pointer<opl::MFNode>( L"shaders" );
				
	sg::spatial_ptr ps( create_node( L"PackagedShader", pa ) );
	attrib->push_back( ps );

	assign_field<opl::v_string>( ).assign( pa, ps, L"DEF" );
	assign_field<opl::m_url>( ).assign(	pa, ps, L"url" );
	assign_field<opl::v_string>( ).assign( pa, ps, L"language" );

	opl::m_url* url = ps->pointer<opl::m_url>( L"url" );
	expand_url( *url, pa.get_base_url( ) );

	pa.push( ps );
*/
	return ACTION_RETURN_CONTINUE;
}
			
ACTION_RETURN shader_part_x3d_parser_action( x3d_parser_action& pa )
{
	sg::m_shader_part_ptr* parts = pa.top( )->pointer<sg::m_shader_part_ptr>( L"parts" );

	sg::shader_part_ptr part = create_node<sg::shader_part_ptr, sg::shader_part>( pa );
	parts->push_back( part );

	assign_field<opl::v_string>( ).assign( pa, part, L"DEF" );
	assign_field<opl::m_url>( ).assign(	pa, part, L"url" );
	assign_field<opl::v_string>( ).assign( pa, part, L"type" );

	opl::m_url* url = part->pointer<opl::m_url>( L"url" );
	expand_url( *url, pa.get_base_url( ) );
	
	pa.insert( part );

	pa.push( part );

	return ACTION_RETURN_CONTINUE;
}

// Render Properties component.
ACTION_RETURN blend_x3d_parser_action( x3d_parser_action& pa )
{
	sg::m_node_ptr* rp = pa.top( )->pointer<sg::m_node_ptr>( L"renderProperties" );
	
	sg::blend_ptr blend = create_node<sg::blend_ptr, sg::blend>( pa );
	rp->push_back( blend );

	assign_field<opl::v_string>( ).assign(		pa, blend, L"DEF" );
	assign_field<opl::v_color_rgba>( ).assign(	pa, blend, L"color" );
	assign_field<opl::v_string>( ).assign(		pa, blend, L"destAlpha" );
	assign_field<opl::v_string>( ).assign(		pa, blend, L"destColor" );
	assign_field<opl::v_bool>( ).assign(		pa, blend, L"enable" );
	assign_field<opl::v_string>( ).assign(		pa, blend, L"equationAlpha" );
	assign_field<opl::v_string>( ).assign(		pa, blend, L"equationColor" );
	assign_field<opl::v_string>( ).assign(		pa, blend, L"sourceAlpha" );
	assign_field<opl::v_string>( ).assign(		pa, blend, L"sourceColor" );

	pa.insert( blend );

	pa.push( blend );

	return ACTION_RETURN_CONTINUE;
}

ACTION_RETURN cull_mode_x3d_parser_action( x3d_parser_action& pa )
{
	sg::m_node_ptr* rp = pa.top( )->pointer<sg::m_node_ptr>( L"renderProperties" );

	sg::cull_mode_ptr cm = create_node<sg::cull_mode_ptr, sg::cull_mode>( pa );
	rp->push_back( cm );
	
	assign_field<opl::v_string>( ).assign(  pa, cm, L"DEF" );
	assign_field<opl::v_bool>( ).assign(	pa, cm, L"enable" );
	assign_field<opl::v_string>( ).assign(	pa, cm, L"mode" );

	pa.insert( cm );

	pa.push( cm );

	return ACTION_RETURN_CONTINUE;
}

ACTION_RETURN depth_test_x3d_parser_action( x3d_parser_action& pa )
{
	sg::m_node_ptr* rp = pa.top( )->pointer<sg::m_node_ptr>( L"renderProperties" );

	sg::depth_test_ptr dp = create_node<sg::depth_test_ptr, sg::depth_test>( pa );
	rp->push_back( dp );
	
	assign_field<opl::v_string>( ).assign(  pa, dp, L"DEF" );
	assign_field<opl::v_bool>( ).assign(	pa, dp, L"enable" );
	assign_field<opl::v_string>( ).assign(	pa, dp, L"test" );
	assign_field<opl::v_bool>( ).assign(	pa, dp, L"writeEnable" );
	
	pa.insert( dp );

	pa.push( dp );

	return ACTION_RETURN_CONTINUE;
}

ACTION_RETURN front_face_x3d_parser_action( x3d_parser_action& pa )
{
	sg::m_node_ptr* rp = pa.top( )->pointer<sg::m_node_ptr>( L"renderProperties" );

	sg::front_face_ptr ff = create_node<sg::front_face_ptr, sg::front_face>( pa );
	rp->push_back( ff );
	
	assign_field<opl::v_string>( ).assign(  pa, ff, L"DEF" );
	assign_field<opl::v_bool>( ).assign(	pa, ff, L"ccw" );
	
	pa.insert( ff );

	pa.push( ff );

	return ACTION_RETURN_CONTINUE;
}

// Rendering component.
ACTION_RETURN coordinate_x3d_parser_action( x3d_parser_action& pa )
{
	sg::v_attribute_array_ptr* coord = pa.top( )->pointer<sg::v_attribute_array_ptr>( L"coord" );

	sg::attribute_array_ptr arr = create_node<sg::attribute_array_ptr, sg::attribute_array>( pa );
	coord->get( ) = arr;

	assign_field<opl::m_float>( ).assign(  pa, arr, L"value", L"point" );
	opl::v_int32* comp = arr->pointer<opl::v_int32>( L"components" );
	comp->get( ) = 3;
	
	pa.insert( arr );
				
	pa.push( arr );
	
	return ACTION_RETURN_CONTINUE;
}
			
ACTION_RETURN indexed_triangle_set_x3d_parser_action( x3d_parser_action& pa )
{
	sg::v_mesh_ptr* geometry = pa.top( )->pointer<sg::v_mesh_ptr>( L"geometry" );

	sg::mesh_ptr its = create_node<sg::mesh_ptr, sg::mesh>( pa );
	geometry->get( ) = its;

	assign_field<opl::v_string>( ).assign( pa, its, L"DEF" );
	//assign_field<opl::v_bool>( ).assign(   pa, its, L"ccw" );
	//assign_field<opl::v_bool>( ).assign(   pa, its, L"colorPerVertex" );
	//assign_field<opl::v_bool>( ).assign(   pa, its, L"normalPerVertex" );
	//assign_field<opl::v_bool>( ).assign(   pa, its, L"solid" );
	assign_field<opl::m_int32>( ).assign(  pa, its, L"index" );
	its->prim_type( ) = sg::mesh::TRIANGLES;

	pa.insert( its );

	pa.push( its );

	return ACTION_RETURN_CONTINUE;
}

ACTION_RETURN indexed_triangle_strip_set_x3d_parser_action( x3d_parser_action& pa )
{
/*
	opl::SFNode* geometry = pa.top( )->pointer<opl::SFNode>( L"geometry" );

	sg::spatial_ptr itss( create_node( L"IndexedTriangleStripSet", pa ) );
	geometry->get( ) = itss;
				
	assign_field<opl::v_string>( ).assign( pa, itss, L"DEF" );
	assign_field<opl::SFFloat>( ).assign(  pa, itss, L"creaseAngle" );
	assign_field<opl::v_bool>( ).assign(   pa, itss, L"ccw" );
	assign_field<opl::v_bool>( ).assign(   pa, itss, L"normalPerVertex" );
	assign_field<opl::v_bool>( ).assign(   pa, itss, L"solid" );
	assign_field<opl::MFInt32>( ).assign(  pa, itss, L"index" );
				
	pa.push( itss );
*/				
	return ACTION_RETURN_CONTINUE;
}

ACTION_RETURN normal_x3d_parser_action( x3d_parser_action& pa )
{
	sg::v_attribute_array_ptr* normal = pa.top( )->pointer<sg::v_attribute_array_ptr>( L"normal" );

	sg::attribute_array_ptr arr = create_node<sg::attribute_array_ptr, sg::attribute_array>( pa );
	normal->get( ) = arr;

	assign_field<opl::m_float>( ).assign( pa, arr, L"value", L"vector" );
	opl::v_int32* comp = arr->pointer<opl::v_int32>( L"components" );
	comp->get( ) = 3;

	pa.insert( arr );

	pa.push( arr );

	return ACTION_RETURN_CONTINUE;
}

// Shape component.
ACTION_RETURN appearance_x3d_parser_action( x3d_parser_action& pa )
{
	sg::v_appearance_ptr* appearance = pa.top( )->pointer<sg::v_appearance_ptr>( L"appearance" );

	sg::appearance_ptr app = create_node<sg::appearance_ptr, sg::appearance>( pa );
	appearance->get( ) = app;

	assign_field<opl::v_string>( ).assign( pa, app, L"DEF" );

	pa.insert( app );

	pa.push( app );

	return ACTION_RETURN_CONTINUE;
}

ACTION_RETURN material_x3d_parser_action( x3d_parser_action& pa )
{
	sg::v_material_ptr* material = pa.top( )->pointer<sg::v_material_ptr>( L"material" );

	sg::material_ptr mat = create_node<sg::material_ptr, sg::material>( pa );
	material->get( ) = mat;

	assign_field<opl::v_string>( ).assign( pa, mat, L"DEF" );
	assign_field<opl::v_float>( ).assign(  pa, mat, L"ambientIntensity" );
	assign_field<opl::v_color>( ).assign(  pa, mat, L"diffuseColor" );
	assign_field<opl::v_color>( ).assign(  pa, mat, L"emissiveColor" );
	assign_field<opl::v_float>( ).assign(  pa, mat, L"shininess" );
	assign_field<opl::v_color>( ).assign(  pa, mat, L"specularColor" );
	assign_field<opl::v_float>( ).assign(  pa, mat, L"transparency" );

	pa.insert( mat );
	
	pa.push( mat );

	return ACTION_RETURN_CONTINUE;
}
			
ACTION_RETURN shape_x3d_parser_action( x3d_parser_action& pa )
{
	sg::m_spatial_ptr* children = pa.top( )->pointer<sg::m_spatial_ptr>( L"children" );

	sg::shape_ptr shape = create_node<sg::shape_ptr, sg::shape>( pa );
	children->push_back( shape );

	assign_field<opl::v_string>( ).assign( pa, shape, L"DEF" );

	pa.insert( shape );

	pa.push( shape );

	return ACTION_RETURN_CONTINUE;
}
			
// Texturing component.
ACTION_RETURN image_texture_x3d_parser_action( x3d_parser_action& pa )
{
	sg::texture_ptr im = create_node<sg::texture_ptr, sg::texture>( pa );

	assign_field<opl::v_string>( ).assign( pa, im, L"DEF" );
	assign_field<opl::m_url>( ).assign(	   pa, im, L"url" );
	assign_field<opl::v_bool>( ).assign(   pa, im, L"repeatS" );
	assign_field<opl::v_bool>( ).assign(   pa, im, L"repeatT" );
	
	sg::mv_texture_ptr* texture = pa.top( )->pointer<sg::mv_texture_ptr>( L"texture" );
	if( texture )
	{
		texture->push_back( sg::v_texture_ptr( im ) );
	}
	else
	{
		sg::shader_ptr top = boost::shared_dynamic_cast<sg::shader>( pa.top( ) );
		v_texture_ptr* node = top->pointer_param<v_texture_ptr>( pa.get_current_texture_name( ) );
		if( node )
			node->get( ) = im;
	}

	opl::m_url* url = im->pointer<opl::m_url>( L"url" );
	expand_url( *url, pa.get_base_url( ) );
	
	im->update( );

	pa.insert( im );

	pa.push( im );

	return ACTION_RETURN_CONTINUE;
}

ACTION_RETURN multi_texture_x3d_parser_action( x3d_parser_action& pa )
{
/*
	opl::MFNode* texture = pa.top( )->pointer<opl::MFNode>( L"texture" );
				
	sg::spatial_ptr mt( create_node( L"MultiTexture", pa ) );
	texture->push_back( mt );

	assign_field<opl::v_string>( ).assign( pa, mt, L"DEF" );
	assign_field<opl::SFFloat>( ).assign(  pa, mt, L"alpha" );
	assign_field<opl::v_color>( ).assign(  pa, mt, L"color" );
	assign_field<opl::m_string>( ).assign( pa, mt, L"function" );
	assign_field<opl::m_string>( ).assign( pa, mt, L"mode" );
	assign_field<opl::m_string>( ).assign( pa, mt, L"source" );
				
	pa.push( mt );
*/	
	pa.push( pa.top( ) );
				
	return ACTION_RETURN_CONTINUE;
}

ACTION_RETURN multi_texture_coordinate_x3d_parser_action( x3d_parser_action& pa )
{
	pa.push( pa.top( ) );

	return ACTION_RETURN_CONTINUE;
}

ACTION_RETURN texture_coordinate_x3d_parser_action( x3d_parser_action& pa )
{
	sg::mv_uv_set_ptr* uv_ = pa.top( )->pointer<sg::mv_uv_set_ptr>( L"uv" );
	
	sg::uv_set_ptr uv_set = create_node<sg::uv_set_ptr, sg::uv_set>( pa );
	uv_->push_back( sg::v_uv_set_ptr( uv_set ) );

	sg::v_attribute_array_ptr* uv = uv_set->pointer<sg::v_attribute_array_ptr>( L"uv" );

	sg::attribute_array_ptr arr( new attribute_array( ) );
	uv->get( ) = arr;

	assign_field<opl::m_float>( ).assign(  pa, arr, L"value", L"point" );
	opl::v_int32* comp = arr->pointer<opl::v_int32>( L"components" );
	comp->get( ) = 2;

	pa.insert( uv_set );
	
	pa.push( arr );

	return ACTION_RETURN_CONTINUE;
}
			
ACTION_RETURN texture_coordinate_generator_x3d_parser_action( x3d_parser_action& pa )
{
	sg::mv_uv_set_ptr* uv_ = pa.top( )->pointer<sg::mv_uv_set_ptr>( L"uv" );

	sg::uv_set_ptr uv_set = create_node<sg::uv_set_ptr, sg::uv_set>( pa );
	uv_->push_back( sg::v_uv_set_ptr( uv_set ) );
	
	assign_field<opl::v_string>( ).assign( pa, uv_set, L"DEF" );
	assign_field<opl::v_string>( ).assign( pa, uv_set, L"mode" );
	assign_field<opl::m_float>( ).assign(  pa, uv_set, L"parameter" );

	opl::v_bool* a = uv_set->pointer<opl::v_bool>( L"auto" );
	a->get( ) = true;

	pa.insert( uv_set );

	pa.push( uv_set );

	return ACTION_RETURN_CONTINUE;
}
			
ACTION_RETURN texture_transform_x3d_parser_action( x3d_parser_action& pa )
{
	sg::m_texture_transform_ptr* tt = pa.top( )->pointer<sg::m_texture_transform_ptr>( L"textureTransform" );

	texture_transform_ptr t( new texture_transform( ) );

	assign_field<opl::v_string>( ).assign(   pa, t, L"DEF" );
	assign_field<opl::v_vec3f>( ).assign(    pa, t, L"center" );
	assign_field<opl::v_rotation>( ).assign( pa, t, L"rotation" );
	assign_field<opl::v_vec3f>( ).assign(    pa, t, L"scale" );
	assign_field<opl::v_vec3f>( ).assign(    pa, t, L"translation" );

	opl::v_string	d = t->value<opl::v_string>(	L"DEF" );
	opl::v_vec3f	c = t->value<opl::v_vec3f>(		L"center" );
	opl::v_rotation r = t->value<opl::v_rotation>(  L"rotation" );
	opl::v_vec3f	s = t->value<opl::v_vec3f>(		L"scale" );
	opl::v_vec3f	l = t->value<opl::v_vec3f>(		L"translation" );

	opl::matrixf c_mat	 = opl::make_translation( -c.value( ) );
	opl::matrixf r_mat	 = opl::from_axis_angle_rotation( r.value( ) );
	opl::matrixf s_mat	 = opl::make_scale( s.value( ) );
	opl::matrixf t_mat	 = opl::make_translation( c.value( ) + l.value( ) );

	opl::matrixf f_mat = c_mat * s_mat * r_mat * t_mat;

	sg::texture_transform_ptr tm = create_node<sg::texture_transform_ptr, sg::texture_transform>( pa );
	tt->push_back( tm );

	opl::v_matrix4f* fm = tm->pointer<opl::v_matrix4f>( L"matrix" );
	fm->get( ) = f_mat;

	pa.insert( tm );

	pa.push( tm );
	
	return ACTION_RETURN_CONTINUE;
}
			
// Texturing3D component.
ACTION_RETURN composed3D_texture_x3d_parser_action( x3d_parser_action& pa )
{
/*
	opl::MFNode* texture = pa.top( )->pointer<opl::MFNode>( L"texture" );
				
	sg::spatial_ptr c3t( create_node( L"Composed3DTexture", pa ) );
	texture->push_back( c3t );

	assign_field<opl::v_string>( ).assign( pa, c3t, L"DEF" );
	assign_field<opl::v_bool>( ).assign(   pa, c3t, L"repeatS" );
	assign_field<opl::v_bool>( ).assign(   pa, c3t, L"repeatT" );
	assign_field<opl::v_bool>( ).assign(   pa, c3t, L"repearR" );
				
	pa.push( c3t );
*/				
	return ACTION_RETURN_CONTINUE;
}
			
ACTION_RETURN image3D_texture_x3d_parser_action( x3d_parser_action& pa )
{
	sg::texture_ptr im = create_node<sg::texture_ptr, sg::texture>( pa );

	assign_field<opl::v_string>( ).assign( pa, im, L"DEF" );
	assign_field<opl::m_url>( ).assign(	   pa, im, L"url" );
	assign_field<opl::v_bool>( ).assign(   pa, im, L"repeatS" );
	assign_field<opl::v_bool>( ).assign(   pa, im, L"repeatT" );
	assign_field<opl::v_bool>( ).assign(   pa, im, L"repeatR" );
	
/*
	opl::MFNode* texture = pa.top( )->pointer<opl::MFNode>( L"texture" );
	if( texture )
	{
		texture->push_back( im );
	}
	else
*/
	{
		sg::shader_ptr top = boost::shared_dynamic_cast<sg::shader>( pa.top( ) );
		v_texture_ptr* node = top->pointer_param<v_texture_ptr>( pa.get_current_texture_name( ) );
		if( node )
			node->get( ) = im;
	}

	opl::m_url* url = im->pointer<opl::m_url>( L"url" );
	expand_url( *url, pa.get_base_url( ) );
	
	im->update( );

	pa.insert( im );

	pa.push( im );

	return ACTION_RETURN_CONTINUE;
}
			
ACTION_RETURN pixel3D_texture_x3d_parser_action( x3d_parser_action& pa )
{
/*
	opl::MFNode* texture = pa.top( )->pointer<opl::MFNode>( L"texture" );
				
	sg::spatial_ptr p3t( create_node( L"Pixel3DTexture", pa ) );
	texture->push_back( p3t );

	assign_field<opl::v_string>( ).assign( pa, p3t, L"DEF" );
	assign_field<opl::MFInt32>( ).assign(  pa, p3t, L"image" );
	assign_field<opl::v_bool>( ).assign(   pa, p3t, L"repeatS" );
	assign_field<opl::v_bool>( ).assign(   pa, p3t, L"repeatT" );
	assign_field<opl::v_bool>( ).assign(   pa, p3t, L"repearR" );
				
	pa.push( p3t );
*/				
	return ACTION_RETURN_CONTINUE;
}
			
ACTION_RETURN texture_coordinate3D_x3d_parser_action( x3d_parser_action& pa )
{
/*
	opl::MFNode* texCoord = pa.top( )->pointer<opl::MFNode>( L"texCoord" );
				
	sg::spatial_ptr t3c( create_node( L"TextureCoordinate3D", pa ) );
	texCoord->push_back( t3c );

	assign_field<opl::v_string>( ).assign( pa, t3c, L"DEF" );
	assign_field<opl::m_float>( ).assign(  pa, t3c, L"point" );
				
	pa.push( t3c );
*/
	return ACTION_RETURN_CONTINUE;
}
			
ACTION_RETURN texture_coordinate4D_x3d_parser_action( x3d_parser_action& pa )
{
/*
	opl::MFNode* texCoord = pa.top( )->pointer<opl::MFNode>( L"texCoord" );
				
	sg::spatial_ptr t4c( create_node( L"TextureCoordinate4D", pa ) );
	texCoord->push_back( t4c );

	assign_field<opl::v_string>( ).assign( pa, t4c, L"DEF" );
	assign_field<opl::m_float>( ).assign(  pa, t4c, L"point" );
				
	pa.push( t4c );
*/	
	return ACTION_RETURN_CONTINUE;
}
			
ACTION_RETURN texture_matrix_transform_x3d_parser_action( x3d_parser_action& pa )
{
/*
	opl::MFNode* texTransform = pa.top( )->pointer<opl::MFNode>( L"texTransform" );
				
	sg::spatial_ptr t( create_node( L"TextureMatrixTransform", pa ) );
	texTransform->push_back( t );
				
	assign_field<opl::v_string>( ).assign(   pa, t, L"DEF" );
	assign_field<opl::v_matrix4f>( ).assign( pa, t, L"matrix" );
				
	pa.push( t );
*/				
	return ACTION_RETURN_CONTINUE;
}
			
ACTION_RETURN texture_transform3D_x3d_parser_action( x3d_parser_action& pa )
{
/*
	opl::MFNode* texTransform = pa.top( )->pointer<opl::MFNode>( L"texTransform" );
				
	sg::spatial_ptr t( create_node( L"TextureTransform3D", pa ) );
	texTransform->push_back( t );

	assign_field<opl::v_string>( ).assign(   pa, t, L"DEF" );
	assign_field<opl::v_vec3f>( ).assign(    pa, t, L"center" );
	assign_field<opl::v_rotation>( ).assign( pa, t, L"rotation" );
	assign_field<opl::v_vec3f>( ).assign(    pa, t, L"scale" );
	assign_field<opl::v_vec3f>( ).assign(    pa, t, L"translation" );
				
	pa.push( t );
*/				
	return ACTION_RETURN_CONTINUE;
}
			
} } } }
