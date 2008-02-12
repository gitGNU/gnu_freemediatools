
// sg - A scene graph representation.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifdef HAVE_CONFIG_H
#include <openlibraries_global_config.hpp>
#endif

#include <boost/thread/recursive_mutex.hpp>

#include <openobjectlib/sg/sg.hpp>

#include <openobjectlib/sg/appearance.hpp>
#include <openobjectlib/sg/meshes.hpp>
#include <openobjectlib/sg/spatial.hpp>

#include <openobjectlib/sg/constant_time_dispatcher.hpp>

namespace olib { namespace openobjectlib {

namespace
{
	class registry
	{
	public:
		explicit registry( ) { }

	private:
		typedef sg::node_ptr argument_type;
		typedef bool return_type;
		typedef boost::function<return_type ( registry& r, argument_type )> functor_type;

	public:
		template<class U>
		void add( const functor_type& f )
		{ back_end_.template add<U>( f ); }

	private:
		sg::constant_time_dispatcher<return_type, argument_type, functor_type>	back_end_;
	};
	
	bool default_( registry&, sg::node_ptr )
	{ return true; }
	
	void static_initialisation( )
	{
		registry r;
			
		r.add<sg::camera>( default_ );
		r.add<sg::group>( default_ );
		r.add<sg::HDR_group>( default_ );
		r.add<sg::select>( default_ );
		r.add<sg::shape>( default_ );
		r.add<sg::transform>( default_ );
		r.add<sg::texture_transform>( default_ );
		r.add<sg::blend>( default_ );
		r.add<sg::cull_mode>( default_ );
		r.add<sg::depth_test>( default_ );
		r.add<sg::front_face>( default_ );
		r.add<sg::mesh>( default_ );
		r.add<sg::multi_index_mesh>( default_ );
	}
}

namespace
{
	void reflib( int init )
	{
		static long refs = 0;
		
		if( init > 0 && ++refs == 1)
			static_initialisation( );
	}
	
	boost::recursive_mutex mutex;
}

bool init( )
{ 
	boost::recursive_mutex::scoped_lock lock( mutex );
	reflib( 1 );
	return true;
}

bool uninit( )
{
	boost::recursive_mutex::scoped_lock lock( mutex );
	reflib( -1 );
	return true;
}

} }
