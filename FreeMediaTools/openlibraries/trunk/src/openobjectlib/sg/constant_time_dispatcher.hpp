
// sg - A scene graph representation.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef CONSTANT_TIME_DISPATCHER_INC_
#define CONSTANT_TIME_DISPATCHER_INC_

#include <stdexcept>
#include <vector>

#include <boost/function.hpp>

namespace olib { namespace openobjectlib { namespace sg {

//
template<typename R, class T = bool, class C = boost::function<R ( T& )> >
class constant_time_dispatcher
{
public:
	typedef std::vector<C>	vector_;
	typedef C				functor_type;
				
public:
	template<class U>
	void add( C fun )
	{
		int& i = U::get_class_index_static( );
		if( i < 0 )
		{
			callbacks_.resize( callbacks_.size( ) + 1 );
			i = static_cast<int>( callbacks_.size( ) ) - 1; // XXX possible truncation.
		}
		else if( callbacks_.size( ) <= static_cast<size_t>( i ) )
		{
			callbacks_.resize( i + 1 );
		}
		callbacks_[ i ] = fun;
	}

	void clear( )
	{ callbacks_.clear( ); }

public:
	template<class V, class U>
	R operator( )( V& v, U u )
	{
		int i = u->get_class_index( );
#ifndef NDEBUG
		if( i < 0 || i >= static_cast<int>( callbacks_.size( ) ) )
				throw std::runtime_error( "dispatching to node not in table" );
#endif // NDEBUG
		return callbacks_[ i ]( v, u );
	}
			
private:
	vector_ callbacks_;
};

} } }

#endif
