
// sg - A scene graph representation.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifdef HAVE_CONFIG_H
#include <openlibraries_global_config.hpp>
#endif

#include <openobjectlib/sg/bbox_volume.hpp>

namespace opl = olib::openpluginlib;

namespace olib { namespace openobjectlib { namespace sg {

bbox_volume::bbox_volume( )
{
	center_	= opl::vec3f( 0.0f, 0.0f, 0.0f );

	frame_[ 0 ]	= opl::vec3f( 1.0f, 0.0f, 0.0f );
	frame_[ 1 ]	= opl::vec3f( 0.0f, 1.0f, 0.0f );
	frame_[ 2 ]	= opl::vec3f( 0.0f, 0.0f, 1.0f );
	
	extents_[ 0 ] = 0.0f;
	extents_[ 1 ] = 0.0f;
	extents_[ 2 ] = 0.0f;
}

float bbox_volume::get_radius( ) const
{
	float radius = extents_[ 0 ];
	
	if( radius < extents_[ 1 ] ) radius = extents_[ 1 ];
	if( radius < extents_[ 2 ] ) radius = extents_[ 2 ];
	
	return radius;
}

void bbox_volume::set_radius( float radius )
{
	extents_[ 0 ] = radius;
	extents_[ 1 ] = radius;
	extents_[ 2 ] = radius;
}

void bbox_volume::compute_vertices( opl::vec3f v[ 8 ] ) const
{
	opl::vec3f ext_frame[ 3 ];
	
	ext_frame[ 0 ] = extents_[ 0 ] * frame_[ 0 ];
	ext_frame[ 1 ] = extents_[ 1 ] * frame_[ 1 ];
	ext_frame[ 2 ] = extents_[ 2 ] * frame_[ 2 ];
	
	v[ 0 ] = center_ - ext_frame[ 0 ] - ext_frame[ 1 ] - ext_frame[ 2 ];
	v[ 1 ] = center_ + ext_frame[ 0 ] - ext_frame[ 1 ] - ext_frame[ 2 ];
	v[ 2 ] = center_ + ext_frame[ 0 ] + ext_frame[ 1 ] - ext_frame[ 2 ];
	v[ 3 ] = center_ - ext_frame[ 0 ] + ext_frame[ 1 ] - ext_frame[ 2 ];
	v[ 4 ] = center_ - ext_frame[ 0 ] - ext_frame[ 1 ] + ext_frame[ 2 ];
	v[ 5 ] = center_ + ext_frame[ 0 ] - ext_frame[ 1 ] + ext_frame[ 2 ];
	v[ 6 ] = center_ + ext_frame[ 0 ] + ext_frame[ 1 ] + ext_frame[ 2 ];
	v[ 7 ] = center_ - ext_frame[ 0 ] + ext_frame[ 1 ] + ext_frame[ 2 ];
}

bbox_volume compute_bbox_volume( const opl::m_float& v )
{
	bbox_volume bv;
	opl::vec3f min, max;
	
	if( v.size( ) < 3 )
		return bv;
	
	min[ 0 ] = max[ 0 ] = v[ 0 ];
	min[ 1 ] = max[ 1 ] = v[ 1 ];
	min[ 2 ] = max[ 2 ] = v[ 2 ];
	
	for( opl::m_float::size_type i = 3; i < v.size( ); i += 3 )
	{
		if( min[ 0 ] > v[ i + 0 ] ) min[ 0 ] = v[ i + 0 ];
		else if( max[ 0 ] < v[ i + 0 ] ) max[ 0 ] = v[ i + 0 ];
		
		if( min[ 1 ] > v[ i + 1 ] ) min[ 1 ] = v[ i + 1 ];
		else if( max[ 1 ] < v[ i + 1 ] ) max[ 1 ] = v[ i + 1 ];
		
		if( min[ 2 ] > v[ i + 2 ] ) min[ 2 ] = v[ i + 2 ];
		else if( max[ 2 ] < v[ i + 2 ] ) max[ 2 ] = v[ i + 2 ];
	}
	
	bv.set_center( ( min + max ) * 0.5f );

	bv.set_extent( 0, ( max[ 0 ] - min[ 0 ] ) * 0.5f );
	bv.set_extent( 1, ( max[ 1 ] - min[ 1 ] ) * 0.5f );
	bv.set_extent( 2, ( max[ 2 ] - min[ 2 ] ) * 0.5f );
	
	return bv;
}

bbox_volume merge_bbox_volume( const bbox_volume& v1, const bbox_volume& v2 )
{
	bbox_volume bv;
	opl::vec3f v[ 8 ];
	opl::vec3f min, max, center;

	center = ( v1.get_center( ) + v2.get_center( ) ) * 0.5f;
	
	opl::matrixf m0, m1;
	
	m0 = opl::make_identity<float>( );
	m1 = opl::make_identity<float>( );
	
	for( int i = 0; i < 3; ++i )
	{
		m0( 0, i ) = v1.get_frame( i )[ 0 ];
		m0( 1, i ) = v1.get_frame( i )[ 1 ];
		m0( 2, i ) = v1.get_frame( i )[ 2 ];
		
		m1( 0, i ) = v2.get_frame( i )[ 0 ];
		m1( 1, i ) = v2.get_frame( i )[ 1 ];
		m1( 2, i ) = v2.get_frame( i )[ 2 ];
	}
	
	opl::quaternionf q0, q1;
	q0 = opl::quaternion_from_rotation( m0 );
	q1 = opl::quaternion_from_rotation( m1 );
	
	if( opl::dot_product( q0, q1 ) < 0.0f )
		q1 = -q1;
		
	opl::quaternionf q = q0 + q1;
	float inv_l = 1.0f / sqrtf( opl::dot_product( q, q ) );
	q *= inv_l;
	
	opl::matrixf m;
	m = opl::quaternion_to_rotation( q );
	
	v1.compute_vertices( v );
	
	bv.set_frame( 0, opl::vec3f( m( 0, 0 ), m( 1, 0 ), m( 2, 0 ) ) );
	bv.set_frame( 1, opl::vec3f( m( 0, 1 ), m( 1, 1 ), m( 2, 1 ) ) );
	bv.set_frame( 2, opl::vec3f( m( 0, 2 ), m( 1, 2 ), m( 2, 2 ) ) );

	for( int i = 0; i < 8; ++i )
	{
		opl::vec3f tmp = v[ i ] - center;

		for( int j = 0; j < 3; ++j )
		{
			float dot = opl::dot_product( tmp, bv.get_frame( j ) );
			if( min[ j ] > dot ) min[ j ] = dot;
			else if( max[ j ] < dot ) max[ j ] = dot;
		}
	}
	
	v2.compute_vertices( v );
	for( int i = 0; i < 8; ++i )
	{
		opl::vec3f tmp = v[ i ] - center;
		
		for( int j = 0; j < 3; ++j )
		{
			float dot = opl::dot_product( tmp, bv.get_frame( j ) );
			if( min[ j ] > dot ) min[ j ] = dot;
			else if( max[ j ] < dot ) max[ j ] = dot;
		}
	}

	center += ( min[ 0 ] + max[ 0 ] ) * 0.5f * bv.get_frame( 0 );
	center += ( min[ 1 ] + max[ 1 ] ) * 0.5f * bv.get_frame( 1 );
	center += ( min[ 2 ] + max[ 2 ] ) * 0.5f * bv.get_frame( 2 );
	
	bv.set_extent( 0, ( max[ 0 ] - min[ 0 ] ) * 0.5f );
	bv.set_extent( 1, ( max[ 1 ] - min[ 1 ] ) * 0.5f );
	bv.set_extent( 2, ( max[ 2 ] - min[ 2 ] ) * 0.5f );
	bv.set_center( center );

	return bv;
}

} } }
