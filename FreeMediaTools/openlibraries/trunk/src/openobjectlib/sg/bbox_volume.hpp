
// sg - A scene graph representation.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef BBOX_VOLUME_INC_
#define BBOX_VOLUME_INC_

#include <openobjectlib/sg/config.hpp>

#include <openpluginlib/pl/property.hpp>

namespace olib { namespace openobjectlib { namespace sg {

class SG_DECLSPEC bbox_volume
{
public:
	explicit bbox_volume( );

	const openpluginlib::vec3f& get_center( ) const							{ return center_; }
	void						set_center( const openpluginlib::vec3f& c ) { center_ = c; }
	
	const openpluginlib::vec3f& get_frame( int i ) const							{ return frame_[ i ]; }
	void						set_frame( int i, const openpluginlib::vec3f& f )	{ frame_[ i ] = f; }

	float get_radius( ) const;
	void  set_radius( float radius );
	
	float get_extent( int i ) const			{ return extents_[ i ]; }
	void  set_extent( int i, float extent ) { extents_[ i ] = extent; }
	
	void compute_vertices( openpluginlib::vec3f v[ 8 ] ) const;

private:
	openpluginlib::vec3f frame_[ 3 ];
	openpluginlib::vec3f center_;
	float				 extents_[ 3 ];
};

bbox_volume compute_bbox_volume( const openpluginlib::m_float& v );
bbox_volume merge_bbox_volume( const bbox_volume& v1, const bbox_volume& v2 );

} } }

#endif
