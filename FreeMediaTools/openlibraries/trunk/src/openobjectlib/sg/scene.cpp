
// sg - A scene graph representation.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifdef HAVE_CONFIG_H
#include <openlibraries_global_config.hpp>
#endif

#include <functional>

#include <openobjectlib/sg/scene.hpp>

namespace opl = olib::openpluginlib;

namespace olib { namespace openobjectlib { namespace sg {

namespace
{
	camera_ptr make_default_camera( int w, int h, const bbox_volume& bv )
	{
		camera_ptr c( new camera );
		
		opl::vec3f	center = bv.get_center( );
		float		radius = bv.get_radius( );
		
		opl::vec3f eye = center + opl::vec3f( 0.0f, 0.0f, 2.1f * radius );
		
		float ar = ( float ) w / ( float ) h;

		c->perspective( 45.0f, ar, 1.0f, 1000.0f );
		c->look_at( eye, center, opl::vec3f( 0.0f, 1.0f, 0.0f ) );
		
		return c;
	}
	
	struct cam_ar_update
		: public std::unary_function<camera_ptr, void>
	{
		explicit cam_ar_update( float ar )
			: ar_( ar )
		{ }
		
		void operator( )( const camera_ptr& cam )
		{
			opl::v_float* ar = cam->pointer<opl::v_float>( L"aspectRatio" );
			ar->get( ) = ar_;
			
			cam->set_dirty( true );
		}
		
		const float ar_;
	};
}

scene::scene( )
	: i_( 0 )
	, x_( 0 )
	, y_( 0 )
	, w_( 1 )
	, h_( 1 )
{ }

bbox_volume scene::get_world_bounds( ) const
{
	if( r_ )
		return r_->get_world_bounds( );
		
	return bbox_volume( );
}

void scene::set_root( const spatial_ptr& r )
{ r_ = r; }

void scene::add_camera( const camera_ptr& c )
{
	if( r_ )
	{
		m_spatial_ptr* children = r_->pointer<m_spatial_ptr>( L"children" );
		if( children )
			children->push_back( c );
	
		c_.push_back( c );
	}
}

camera_ptr scene::get_active_camera( )
{
	if( c_.empty( ) )
		return camera_ptr( );
		
	return c_[ i_ ];
}

void scene::get_viewport_extents( int& x, int& y, int& w, int& h )
{
	x = x_, y = y_;
	w = w_, h = h_;
}

void scene::set_viewport_extents( int x, int y, int w, int h )
{
	x_ = x, y_ = y;
	w_ = w, h_ = h;
	
	float ar, x_ext, y_ext;
	
	x_ext = ( ( float ) w  );
	y_ext = ( ( float ) h  );
	
	if( x_ext >= y_ext )
		ar = x_ext / y_ext;
	else
		ar = y_ext / x_ext;
	
	std::for_each( c_.begin( ), c_.end( ), cam_ar_update( ar ) );
}

bool scene::update( )
{
	bool valid = false;
	if( r_ )
	{
		valid = u_.update( r_ );
		if( c_.empty( ) )
			add_camera( make_default_camera( w_, h_, get_world_bounds( ) ) );
	}
	
	return valid;
}

} } }
