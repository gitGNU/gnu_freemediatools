
// sg - A scene graph representation.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openobjectlib.org.

#ifndef SCENE_INC_
#define SCENE_INC_

#include <openobjectlib/sg/bbox_volume.hpp>
#include <openobjectlib/sg/graph_updater.hpp>

namespace olib { namespace openobjectlib { namespace sg {

class SG_DECLSPEC scene
{
public:
	explicit scene( );

	bbox_volume get_world_bounds( ) const;

	const spatial_ptr&	get_root( ) const { return r_; }
	void				set_root( const spatial_ptr& r );

	void		add_camera( const camera_ptr& c );
	camera_ptr	get_active_camera( );
	
	void get_viewport_extents( int& x, int& y, int& w, int& h );
	void set_viewport_extents( int x, int y, int w, int h );

	bool update( );

private:
	spatial_ptr	  r_;
	graph_updater u_;
	
	std::vector<camera_ptr> c_;
	size_t i_;
	
	int x_, y_, w_, h_;
};

typedef boost::shared_ptr<scene> scene_ptr;

} } }

#endif
