
// sg - A scene graph representation.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifdef HAVE_CONFIG_H
#include <openlibraries_global_config.hpp>
#endif

#ifdef WIN32
#define _USE_MATH_DEFINES
#endif

#include <cmath>

#include <boost/bind.hpp>

#include <openobjectlib/sg/graph_updater.hpp>
#include <openobjectlib/sg/appearance.hpp>
#include <openobjectlib/sg/meshes.hpp>

namespace opl = olib::openpluginlib;

namespace olib { namespace openobjectlib { namespace sg {

namespace
{
	bbox_volume mesh_bounds( const mesh_ptr& m )
	{
		v_attribute_array_ptr* coord = m->pointer<v_attribute_array_ptr>( L"coord" );
		if( !( coord && coord->get( ) ) )
			return bbox_volume( );
		
		opl::m_float* value = coord->get( )->pointer<opl::m_float>( L"value" );
		
		return compute_bbox_volume( *value );
	}
	
	bbox_volume merge_bounds( const m_spatial_ptr& c )
	{
		typedef m_spatial_ptr::const_iterator const_iterator;
		
		bbox_volume bv;
		
		if( c.empty( ) ) return bv;
		
		const_iterator I = c.begin( );
		bv = ( *I )->get_world_bounds( ); ++I;
		
		for( ; I != c.end( ); ++I )
			bv = merge_bbox_volume( bv, ( *I )->get_world_bounds( ) );

		return bv;
	}
	
	bool update_camera( graph_updater& /*r*/, node_ptr& v )
	{
		camera_ptr cam = boost::dynamic_pointer_cast<camera>( v );

		if( !cam->get_dirty( ) )
			return true;

		opl::v_float nearZ	= v->value<opl::v_float>( L"nearZ" );
		opl::v_float farZ	= v->value<opl::v_float>( L"farZ" );
		opl::v_float ar		= v->value<opl::v_float>( L"aspectRatio" );
		opl::v_float yFOV	= v->value<opl::v_float>( L"yFOV" );

		cam->perspective( yFOV.value( ) * 180.0f / static_cast<float>( M_PI ), ar.value( ), nearZ.value( ), farZ.value( ) );
		cam->set_dirty( false );

		return true;
	}

	bool update_group( graph_updater& r, node_ptr& v )
	{
		group_ptr g = boost::dynamic_pointer_cast<group>( v );

		m_spatial_ptr* children = v->pointer<m_spatial_ptr>( L"children" );
		std::for_each( children->begin( ), children->end( ), boost::bind( &graph_updater::update, boost::ref( r ), _1 ) );

		bbox_volume bv = merge_bounds( *children );
		g->set_world_bounds( bv );
		g->set_dirty( false );

		return true;
	}

	bool update_select( graph_updater& r, node_ptr& v )
	{
		select_ptr s = boost::dynamic_pointer_cast<select>( v );

		m_spatial_ptr*	children = v->pointer<m_spatial_ptr>( L"children" );
		opl::v_int32	choice	 = v->value<opl::v_int32>( L"choice" );
		
		if( choice.value( ) >= 0 && choice.value( ) < children->size( ) )
			r.update( ( *children )[ choice.value( ) ] );
		
		bbox_volume bv = merge_bounds( *children );
		s->set_world_bounds( bv );
		s->set_dirty( false );

		return true;
	}

	bool update_transform( graph_updater& r, node_ptr& v )
	{
		transform_ptr g = boost::dynamic_pointer_cast<transform>( v );

		m_spatial_ptr* children = v->pointer<m_spatial_ptr>( L"children" );
		std::for_each( children->begin( ), children->end( ), boost::bind( &graph_updater::update, boost::ref( r ), _1 ) );

		bbox_volume bv = merge_bounds( *children );
		g->set_world_bounds( bv );
		g->set_dirty( false );

		return true;
	}
	
	bool update_shape( graph_updater& /*r*/, node_ptr& v )
	{
		shape_ptr s = boost::dynamic_pointer_cast<shape>( v );

		if( !s->get_dirty( ) )
			return true;

		v_appearance_ptr* a = v->pointer<v_appearance_ptr>( L"appearance" );
		v_mesh_ptr*		  m = v->pointer<v_mesh_ptr>( L"geometry" );

		if( a && a->get( ) )
		{
			typedef mv_texture_ptr::iterator iterator;

			mv_texture_ptr* t = a->get( )->pointer<mv_texture_ptr>( L"texture" );
			for( iterator I = t->begin( ); I != t->end( ); ++I )
				I->get( )->update( );
		}

		bbox_volume bv;
		if( m && m->get( ) )
			bv = mesh_bounds( m->value( ) );

		s->set_world_bounds( bv );
		s->set_dirty( false );

		return true;
	}
}

graph_updater::graph_updater( )
{
	add<camera>( update_camera );
	add<group>( update_group );
	add<select>( update_select );
	add<shape>( update_shape );
	add<transform>( update_transform );
}

graph_updater::~graph_updater( )
{ }

bool graph_updater::update( const node_ptr& n )
{ return back_end_( *this, boost::const_pointer_cast<node>( n ) ); }

} } }
