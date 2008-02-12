
// sg - A scene graph representation.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef CALCULATE_TANGENT_SPACE_INC_
#define CALCULATE_TANGENT_SPACE_INC_

#include <openobjectlib/sg/constant_time_dispatcher.hpp>
#include <openobjectlib/sg/spatial.hpp>

namespace olib { namespace openobjectlib { namespace sg {

class SG_DECLSPEC calculate_tangent_space
{
public:
	explicit calculate_tangent_space( );
	virtual ~calculate_tangent_space( );
	
public:
	bool calculate( const node_ptr& n );

private:
	typedef node_ptr argument_type;
	typedef bool return_type;
	typedef boost::function<return_type ( calculate_tangent_space& r, argument_type )> functor_type;

private:
	template<class U>
	void add( const functor_type& f )
	{ back_end_.template add<U>( f ); }

private:
	constant_time_dispatcher<return_type, argument_type, functor_type>	back_end_;
};

} } }

#endif
