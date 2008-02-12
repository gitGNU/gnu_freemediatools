
// sg - A scene graph representation.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifdef HAVE_CONFIG_H
#include <openlibraries_global_config.hpp>
#endif

#include <map>

#include <boost/bind.hpp>

#include <openobjectlib/sg/calculate_tangent_space.hpp>
#include <openobjectlib/sg/meshes.hpp>

namespace opl = olib::openpluginlib;

namespace olib { namespace openobjectlib { namespace sg {

namespace
{
	// GC - this stuff kinda follows NVMeshMender. I would use NVMeshMender if it wasnt a D3D implementation.
	struct triangle
	{
		opl::vec3f	normal, binormal, tangent;
		int			indices[ 3 ];
		int			neighbour_id;
		int			tri_id;
		bool		visited;
		
		void reset( )
		{
			neighbour_id = -1;
			tri_id		 = -1;
			visited		 = false;
		}
	};
	
	typedef std::vector<triangle>				m_triangle;
	typedef std::vector<int>					m_triangle_id;
	typedef std::map<opl::vec3f, m_triangle_id>	mm_vert_tris;
	
	void calculate_triangle_basis( triangle& t, const opl::m_float& verts )
	{
		opl::vec3f v0( verts[ t.indices[ 0 ] * 3 + 0 ], verts[ t.indices[ 0 ] * 3 + 1 ], verts[ t.indices[ 0 ] * 3 + 2 ] );
		opl::vec3f v1( verts[ t.indices[ 1 ] * 3 + 0 ], verts[ t.indices[ 1 ] * 3 + 1 ], verts[ t.indices[ 1 ] * 3 + 2 ] );
		opl::vec3f v2( verts[ t.indices[ 2 ] * 3 + 0 ], verts[ t.indices[ 2 ] * 3 + 1 ], verts[ t.indices[ 2 ] * 3 + 2 ] );
		
		opl::vec3f e0 = v1 - v0;
		opl::vec3f e1 = v2 - v0;
		
		t.normal = opl::cross_product( e0, e1 );
	}
	
	bool process_triangle_mesh( const opl::m_float& verts, const opl::m_int32& index, opl::m_float& norms )
	{
		typedef mm_vert_tris::iterator iterator;

		m_triangle		tris;
		mm_vert_tris	vtris;
		
		for( size_t i = 0; i < index.size( ); i += 3 )
		{
			triangle t;
			
			t.indices[ 0 ] = index[ i + 0 ];
			t.indices[ 1 ] = index[ i + 1 ];
			t.indices[ 2 ] = index[ i + 2 ];
			
			calculate_triangle_basis( t, verts );
			
			t.tri_id = static_cast<int>( tris.size( ) );
			tris.push_back( t );
		}
		
		for( size_t i = 0; i < tris.size( ); ++i )
		{
			for( int j = 0; j < 3; ++j )
			{
				opl::vec3f v( verts[ tris[ i ].indices[ j ] * 3 + 0 ], verts[ tris[ i ].indices[ j ] * 3 + 1 ], verts[ tris[ i ].indices[ j ] * 3 + 2 ] );
				
				iterator I = vtris.find( v );
				if( I != vtris.end( ) )
				{
					I->second.push_back( static_cast<int>( i ) );
				}
				else
				{
					vtris[ v ] = m_triangle_id( );
					vtris[ v ].push_back( static_cast<int>( i ) );
				}
			}
		}
		
		for( size_t i = 0; i < verts.size( ); i += 3 )
		{
			opl::vec3f v( verts[ i + 0 ], verts[ i + 1 ], verts[ i + 2 ] );
			opl::vec3f normal;
			
			iterator I = vtris.find( v );
			if( I != vtris.end( ) )
			{
				for( size_t j = 0; j < I->second.size( ); ++j )
					normal += tris[ I->second[ j ] ].normal;
			
				normal = opl::normalize( normal );
			}
			
			norms[ i + 0 ] = normal[ 0 ];
			norms[ i + 1 ] = normal[ 1 ];
			norms[ i + 2 ] = normal[ 2 ];
		}

		return true;
	}
}

namespace
{
	bool calculate_camera( calculate_tangent_space& /*r*/, node_ptr& /*v*/ )
	{ return true; }

	bool calculate_group( calculate_tangent_space& r, node_ptr& v )
	{
		m_spatial_ptr* children = v->pointer<m_spatial_ptr>( L"children" );
		std::for_each( children->begin( ), children->end( ), boost::bind( &calculate_tangent_space::calculate, boost::ref( r ), _1 ) );

		return true;
	}

	bool calculate_shape( calculate_tangent_space& /*r*/, node_ptr& v )
	{
		v_mesh_ptr* m = v->pointer<v_mesh_ptr>( L"geometry" );
		if( !( m && m->get( ) ) || m->get( )->prim_type( ) != mesh::TRIANGLES )
			return false;
		
		v_attribute_array_ptr* coord  = m->get( )->pointer<v_attribute_array_ptr>( L"coord" );
		v_attribute_array_ptr* normal = m->get( )->pointer<v_attribute_array_ptr>( L"normal" );
		if( !( coord && coord->value( ) && normal ) || normal->value( ) )
			return false;
			
		opl::m_float* value = coord->get( )->pointer<opl::m_float>( L"value" );
		if( value->empty( ) )
			return false;
			
		opl::m_int32* index = m->get( )->pointer<opl::m_int32>( L"index" );
		if( index->empty( ) )
			return false;
			
		attribute_array_ptr arr( new attribute_array( ) );
		normal->get( ) = arr;

		opl::m_float* val = arr->pointer<opl::m_float>( L"value" );
		opl::v_int32* nc  = arr->pointer<opl::v_int32>( L"components" );
		
		val->resize( value->size( ) );
		nc->get( ) = 3;

		process_triangle_mesh( *value, *index, *val );
		
		return true;
	}
}

calculate_tangent_space::calculate_tangent_space( )
{
	add<camera>( calculate_camera );
	add<group>(  calculate_group );
	add<shape>(  calculate_shape );
}

calculate_tangent_space::~calculate_tangent_space( )
{ }

bool calculate_tangent_space::calculate( const node_ptr& n )
{ return back_end_( *this, boost::const_pointer_cast<node>( n ) ); }

} } }
