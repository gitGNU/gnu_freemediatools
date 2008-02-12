
// sg - A scene graph representation.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#include <algorithm>

#include <boost/filesystem/exception.hpp>
#include <boost/filesystem/operations.hpp>

#include <openpluginlib/pl/GL_utility.hpp>
#include <openpluginlib/pl/utf8_utils.hpp>

#include <openobjectlib/sg/appearance.hpp>
#include <openobjectlib/sg/shaders.hpp>

#ifdef HAVE_OPENIMAGELIB
#include <openimagelib/il/openimagelib_plugin.hpp>
#endif

namespace il  = olib::openimagelib::il;
namespace opl = olib::openpluginlib;
namespace fs  = boost::filesystem;

namespace olib { namespace openobjectlib { namespace sg {

bool find_url_availability( const olib::openpluginlib::m_url& m,   boost::filesystem::path& p );

namespace SGANON
{
#if defined ( HAVE_OPENIMAGELIB )

  query_traits::query_traits( const opl::wstring& filename )
    : filename_( filename )
  { }
  
  opl::wstring query_traits::libname( ) const
  { return L"openimagelib"; }
  
  opl::wstring query_traits::to_match( ) const
  { return filename_; }
  
  opl::wstring query_traits::type( ) const
  { return L""; }
  
  int query_traits::merit( ) const
  { return 0; }
  
  opl::wstring filename_;

  /*
    
  bool find_url_availability( const opl::m_url& m, fs::path& p )
  {
  typedef opl::m_url::const_iterator const_iterator;
  
  const_iterator I = std::find_if( m.begin( ), m.end( ), fs::exists<fs::path> );
  if( I != m.end( ) )
  {
  p = *I;
			return true;
			}
			
			return false;
			}*/
#endif
}

appearance::appearance( )
{
	insert( L"DEF",				 appearance::field_type( opl::v_string_typename,	opl::v_string( ) ) );
	//	TODO: insert( L"material",		 appearance::field_type( v_node_typename,			v_material_ptr( ) ) );
	insert( L"renderProperties", appearance::field_type( m_node_typename,			m_node_ptr( ) ) );
	insert( L"shader",			 appearance::field_type( v_node_typename,			v_shader_ptr( ) ) );
	//	TODO:insert( L"texture",			 appearance::field_type( m_node_typename,			mv_texture_ptr( ) ) );
	//TODO:	insert( L"textureTransform", appearance::field_type( m_node_typename,			m_texture_transform_ptr( ) ) );
}

appearance::~appearance( )
{ }

material::material( )
{
	opl::color3f color;
	color[ 0 ] = 0.8f;
	color[ 1 ] = 0.8f;
	color[ 2 ] = 0.8f;

	insert( L"DEF",					material::field_type( opl::v_string_typename, opl::v_string( ) ) );
	insert( L"ambientIntensity",	material::field_type( opl::v_float_typename,  opl::v_float( 0.2f ) ) );
	insert( L"diffuseColor",		material::field_type( opl::v_color_typename,  opl::v_color( color ) ) );
	insert( L"emissiveColor",		material::field_type( opl::v_color_typename,  opl::v_color( ) ) );
	insert( L"shininess",			material::field_type( opl::v_float_typename,  opl::v_float( 0.2f ) ) );
	insert( L"specularColor",		material::field_type( opl::v_color_typename,  opl::v_color( ) ) );
	insert( L"transparency",		material::field_type( opl::v_float_typename,  opl::v_float( ) ) );
}

material::~material( )
{ }

texture::texture( )
	: ml_( false )
{
	insert( L"DEF",		 texture::field_type( opl::v_string_typename, opl::v_string( ) ) );
	//	insert( L"url",		 texture::field_type( opl::m_url_typename,	  opl::m_url( ) ) );
	insert( L"mode",	 texture::field_type( opl::m_url_typename,	  opl::v_string( ) ) );
	insert( L"source",	 texture::field_type( opl::m_url_typename,	  opl::v_string( ) ) );
	insert( L"function", texture::field_type( opl::m_url_typename,	  opl::v_string( ) ) );
	insert( L"repeatS",	 texture::field_type( opl::v_bool_typename,   opl::v_bool( true ) ) );
	insert( L"repeatT",	 texture::field_type( opl::v_bool_typename,   opl::v_bool( true ) ) );
	insert( L"repeatR",	 texture::field_type( opl::v_bool_typename,   opl::v_bool( true ) ) );
	insert( L"cubemap",	 texture::field_type( opl::v_bool_typename,   opl::v_bool( false ) ) );
}

texture::~texture( )
{ }

texture_transform::texture_transform( )
{
	insert( L"DEF",	   texture_transform::field_type( opl::v_string_typename,	opl::v_string( ) ) );
	insert( L"matrix", texture_transform::field_type( opl::v_matrix4f_typename, opl::v_matrix4f( ) ) );
}

texture_transform::~texture_transform( )
{ }

bool texture::update( )
{
#ifdef HAVE_OPENIMAGELIB
	typedef il::image_type::const_pointer	const_pointer;
	typedef il::image_type::size_type		size_type;

	typedef opl::discovery<
	  olib::openobjectlib::sg::SGANON::query_traits> oil_discovery;
	
	opl::m_url* m = pointer<opl::m_url>( L"url" );
    
#ifdef USE_DISCOVERY

	fs::path path;
	if( !find_url_availability( *m, path ) ) return false;
	
	oil_discovery plugins( 
			      olib::openobjectlib::sg::SGANON::query_traits( 
									    opl::to_wstring( 
											    path.native_file_string( ).c_str( ) ) ) );

	if( plugins.empty( ) ) return false; 

	oil_discovery::const_iterator i = plugins.begin( );
	il::openimagelib_plugin_ptr plug = boost::shared_dynamic_cast<il::openimagelib_plugin>( i->create_plugin( "" ) );
	if( !plug ) return false;
	
	im_ = plug->load( path.native_file_string( ) );
	if( !im_ ) return false;
	
	if( !opl::is_compressed_format( im_->pf( ).c_str( ) ) && im_->count( ) == 1 && im_->depth( ) == 1 )
		im_ = il::conform( im_, il::cropped | il::flipped | il::flopped );

	return true;
#else
	return false;
#endif


#else
	return false;
#endif
}

#ifdef HAVE_OPENIMAGELIB
il::image_type_ptr texture::image( bool release )
{
	il::image_type_ptr im( im_ );
	
	im_.reset( );
	
	return im;
}
#endif

blend::blend( )
{
	insert( L"DEF",				blend::field_type( opl::v_string_typename,		opl::v_string( L"" ) ) );
	insert( L"color",			blend::field_type( opl::v_color_rgba_typename,	opl::v_color_rgba( ) ) );
	// one of ZERO, ONE, SRC_COLOR, INV_SRC_COLOR, SRC_ALPHA, INV_SRC_ALPHA, DST_ALPHA, INV_DST_ALPHA, CONST_COLOR, INV_CONST_COLOR, CONST_ALPHA, INV_CONST_ALPHA
	insert( L"destAlpha",		blend::field_type( opl::v_string_typename,		opl::v_string( L"ZERO" ) ) );
	insert( L"destColor",		blend::field_type( opl::v_string_typename,		opl::v_string( L"ZERO" ) ) );
	insert( L"enable",			blend::field_type( opl::v_bool_typename,		opl::v_bool( false ) ) );
	// one of ADD, SUBTRACT, REV_SUBTRACT, MIN, MAX
	insert( L"equationAlpha",	blend::field_type( opl::v_string_typename,		opl::v_string( L"ADD" ) ) );
	insert( L"equationColor",	blend::field_type( opl::v_string_typename,		opl::v_string( L"ADD" ) ) );
	// one of ZERO, ONE, DEST_COLOR, INV_DEST_COLOR, SRC_ALPHA, INV_SRC_ALPHA, DST_ALPHA, INV_DST_ALPHA, CONST_COLOR, INV_CONST_COLOR, CONST_ALPHA, INV_CONST_ALPHA, SRC_ALPHA_SATURATE
	insert( L"sourceAlpha",		blend::field_type( opl::v_string_typename,		opl::v_string( L"ONE" ) ) );
	insert( L"sourceColor",		blend::field_type( opl::v_string_typename,		opl::v_string( L"ONE" ) ) );
}

blend::~blend( )
{ }

cull_mode::cull_mode( )
{
	insert( L"DEF",		 cull_mode::field_type( opl::v_string_typename, opl::v_string( ) ) );
	insert( L"enable",	 cull_mode::field_type( opl::v_bool_typename,	opl::v_bool( false ) ) );
	// one of BACK, FRONT, FRONT_AND_BACK
	insert( L"mode",	 cull_mode::field_type( opl::v_string_typename,	opl::v_string( L"BACK" ) ) );
}

cull_mode::~cull_mode( )
{ }

depth_test::depth_test( )
{
	insert( L"DEF",			depth_test::field_type( opl::v_string_typename, opl::v_string( ) ) );
	insert( L"enable",		depth_test::field_type( opl::v_bool_typename,	opl::v_bool( false ) ) );
	// one of NEVER, ALWAYS, LESS, LEQUAL, EQUAL, GREATER, GEQUAL, NOTEQUAL
	insert( L"test",		depth_test::field_type( opl::v_string_typename, opl::v_string( L"LESS" ) ) );
	insert( L"writeEnable",	depth_test::field_type( opl::v_bool_typename,	opl::v_bool( false ) ) );
}

depth_test::~depth_test( )
{ }

front_face::front_face( )
{
	insert( L"DEF",	front_face::field_type( opl::v_string_typename, opl::v_string( ) ) );
	insert( L"ccw",	front_face::field_type( opl::v_bool_typename,	opl::v_bool( true ) ) );
}

front_face::~front_face( )
{ }

} } }



/*
olib::openpluginlib::discovery<olib::openobjectlib::sg::SGANON::query_traits>::empty()
{

}
*/
