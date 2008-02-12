
// sg - A scene graph representation.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef GRAPH_UPDATER_INC_
#define GRAPH_UPDATER_INC_

#include <openobjectlib/sg/constant_time_dispatcher.hpp>
#include <openobjectlib/sg/spatial.hpp>

namespace olib { namespace openobjectlib { namespace sg {

class SG_DECLSPEC graph_updater
{
public:
	explicit graph_updater( );
	virtual ~graph_updater( );

public:
	bool update( const node_ptr& n );

private:
	typedef node_ptr argument_type;
	typedef bool return_type;
	typedef boost::function<return_type ( graph_updater& r, argument_type )> functor_type;

private:
	template<class U>
	void add( const functor_type& f )
	{ back_end_.template add<U>( f ); }

private:
	constant_time_dispatcher<return_type, argument_type, functor_type>	back_end_;
};

} } }

#endif
