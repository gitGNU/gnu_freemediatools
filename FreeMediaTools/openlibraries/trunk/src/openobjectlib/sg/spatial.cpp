
// sg - A scene graph representation.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifdef HAVE_CONFIG_H
#include <openlibraries_global_config.hpp>
#endif

#include <boost/bind.hpp>

#include <openobjectlib/sg/appearance.hpp>
#include <openobjectlib/sg/meshes.hpp>
#include <openobjectlib/sg/spatial.hpp>

namespace opl = olib::openpluginlib;

namespace olib { namespace openobjectlib { namespace sg {

spatial::spatial( )
{ }

spatial::~spatial( )
{ }

group::group( )
{
	insert( L"DEF",				spatial::field_type( opl::v_string_typename,	opl::v_string( ) ) );
	insert( L"addChildren",		spatial::field_type( m_node_typename,			m_spatial_ptr( ) ) );
	insert( L"removeChildren",	spatial::field_type( m_node_typename,			m_spatial_ptr( ) ) );
	insert( L"children",		spatial::field_type( m_node_typename,			m_spatial_ptr( ) ) );
}

group::~group( )
{ }

transform::transform( )
{
	insert( L"matrix", spatial::field_type( opl::v_matrix4f_typename, opl::v_matrix4f( ) ) );
}

transform::~transform( )
{ }

select::select( )
{
	insert( L"choice", spatial::field_type( opl::v_int32_typename, opl::v_int32( ) ) );
}

select::~select( )
{ }

HDR_group::HDR_group( )
{
}

HDR_group::~HDR_group( )
{ }

shape::shape( )
{
	insert( L"DEF",			shape::field_type( opl::v_string_typename,	opl::v_string( ) ) );
	insert( L"appearance",	shape::field_type( v_node_typename,			v_appearance_ptr( ) ) );
	insert( L"geometry",	shape::field_type( v_node_typename,			v_mesh_ptr( ) ) );
}

shape::~shape( )
{ }

camera::camera( )
{
	insert( L"DEF",			camera::field_type( opl::v_string_typename,	opl::v_string( ) ) );
	insert( L"X",			camera::field_type( opl::v_vec3f_typename,	opl::v_vec3f( opl::vec3f( 1.0f, 0.0f, 0.0f ) ) ) );
	insert( L"Y",			camera::field_type( opl::v_vec3f_typename,	opl::v_vec3f( opl::vec3f( 0.0f, 1.0f, 0.0f ) ) ) );
	insert( L"Z",			camera::field_type( opl::v_vec3f_typename,	opl::v_vec3f( opl::vec3f( 0.0f, 0.0f, 1.0f ) ) ) );
	insert( L"O",			camera::field_type( opl::v_vec3f_typename,	opl::v_vec3f( opl::vec3f( 0.0f, 0.0f, 0.0f ) ) ) );
	insert( L"COP",			camera::field_type( opl::v_vec3f_typename,	opl::v_vec3f( opl::vec3f( 0.0f, 0.0f, 2.4142f ) ) ) );
	insert( L"nearZ",		camera::field_type( opl::v_float_typename,	opl::v_float( 1.0f ) ) );
	insert( L"farZ",		camera::field_type( opl::v_float_typename,	opl::v_float( -1.0f ) ) );
	insert( L"viewZ",		camera::field_type( opl::v_float_typename,	opl::v_float( 0.0f ) ) );
	insert( L"wL",			camera::field_type( opl::v_float_typename,	opl::v_float( -1.0f ) ) );
	insert( L"wR",			camera::field_type( opl::v_float_typename,	opl::v_float( 1.0f ) ) );
	insert( L"wT",			camera::field_type( opl::v_float_typename,	opl::v_float( 1.0f ) ) );
	insert( L"wB",			camera::field_type( opl::v_float_typename,	opl::v_float( -1.0f ) ) );
	insert( L"xFOV",		camera::field_type( opl::v_float_typename,	opl::v_float( 0.0f ) ) );
	insert( L"yFOV",		camera::field_type( opl::v_float_typename,	opl::v_float( 0.0f ) ) );
	insert( L"aspectRatio",	camera::field_type( opl::v_float_typename,	opl::v_float( 1.0f ) ) );
}

camera::~camera( )
{ }

void camera::look_at( const opl::vec3f& eye, const opl::vec3f& at, const opl::vec3f& up )
{
	opl::v_vec3f* X = pointer<opl::v_vec3f>( L"X" );
	opl::v_vec3f* Y = pointer<opl::v_vec3f>( L"Y" );
	opl::v_vec3f* Z = pointer<opl::v_vec3f>( L"Z" );
	opl::v_vec3f* O = pointer<opl::v_vec3f>( L"O" );
	
	Z->get( ) = opl::normalize( eye - at );
	X->get( ) = opl::normalize( opl::cross_product( up, Z->value( ) ) );
	Y->get( ) = opl::normalize( opl::cross_product( Z->value( ), X->value( ) ) );
	O->get( ) = eye;
}

void camera::generalised_perspective( float xfov, float yfov, float nearF, float farF )
{
	opl::v_vec3f* cop	= pointer<opl::v_vec3f>( L"COP" );
	opl::v_float* nearZ = pointer<opl::v_float>( L"nearZ" );
	opl::v_float* farZ	= pointer<opl::v_float>( L"farZ" );
	opl::v_float* viewZ = pointer<opl::v_float>( L"viewZ" );
	opl::v_float* wL	= pointer<opl::v_float>( L"wL" );
	opl::v_float* wR	= pointer<opl::v_float>( L"wR" );
	opl::v_float* wT	= pointer<opl::v_float>( L"wT" );
	opl::v_float* wB	= pointer<opl::v_float>( L"wB" );
	opl::v_float* xFOV	= pointer<opl::v_float>( L"xFOV" );
	opl::v_float* yFOV	= pointer<opl::v_float>( L"yFOV" );
	
	cop->get( )		= opl::vec3f( 0.0f, 0.0f, 0.0f );
	nearZ->get( )	= nearF;
	farZ->get( )	= farF;
	viewZ->get( )	= -1.0f;
	wR->get( )		= nearF * tanf( xfov * 0.5f );
	wL->get( )		= -wR->value( );
	wT->get( )		= nearF * tanf( yfov * 0.5f );
	wB->get( )		= -wT->value( );
	xFOV->get( )	= xfov;
	yFOV->get( )	= yfov;
}

void camera::perspective( float yfov, float aspect, float nearF, float farF )
{
	opl::v_float* ar = pointer<opl::v_float>( L"aspectRatio" );
	
	ar->get( ) = aspect;
	
	yfov *= 0.0174532f;
	float xfov = yfov * aspect;
	generalised_perspective( xfov, yfov, nearF, farF );
}

opl::matrixf view_from_camera( const camera& cam )
{
	opl::matrixf m;
	
	opl::v_vec3f X = cam.value<opl::v_vec3f>( L"X" );
	opl::v_vec3f Y = cam.value<opl::v_vec3f>( L"Y" );
	opl::v_vec3f Z = cam.value<opl::v_vec3f>( L"Z" );
	opl::v_vec3f O = cam.value<opl::v_vec3f>( L"O" );
	
	m( 0, 0 ) = X.value( )[ 0 ];
	m( 1, 0 ) = X.value( )[ 1 ];
	m( 2, 0 ) = X.value( )[ 2 ];
	m( 3, 0 ) = 0.0f;
	
	m( 0, 1 ) = Y.value( )[ 0 ];
	m( 1, 1 ) = Y.value( )[ 1 ];
	m( 2, 1 ) = Y.value( )[ 2 ];
	m( 3, 1 ) = 0.0f;

	m( 0, 2 ) = Z.value( )[ 0 ];
	m( 1, 2 ) = Z.value( )[ 1 ];
	m( 2, 2 ) = Z.value( )[ 2 ];
	m( 3, 2 ) = 0.0f;

	m( 3, 0 ) = opl::dot_product( X.value( ), -O.value( ) );
	m( 3, 1 ) = opl::dot_product( Y.value( ), -O.value( ) );
	m( 3, 2 ) = opl::dot_product( Z.value( ), -O.value( ) );
	m( 3, 3 ) = 1.0f;

	return m;
}

} } }
