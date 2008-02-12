
// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef OPENPLUGINLIB_INC_
#define OPENPLUGINLIB_INC_

#if _MSC_VER
#pragma warning ( push )
#pragma warning ( disable:4251 )
#endif

#include <algorithm>
#include <functional>
#include <vector>

#include <openpluginlib/pl/config.hpp>
#include <openpluginlib/pl/discovery_traits.hpp>
#include <openpluginlib/pl/openplugin.hpp>
#include <openpluginlib/pl/string.hpp>

namespace olib { namespace openpluginlib {

enum { STD_PLUGINS = 1, OFX_PLUGINS };

OPENPLUGINLIB_DECLSPEC bool init( const string& lookup_path = "", int mode = STD_PLUGINS | OFX_PLUGINS );
OPENPLUGINLIB_DECLSPEC bool uninit( );

// UI construction motivated
OPENPLUGINLIB_DECLSPEC wstring registered_filters( bool in_filter );

#ifdef WIN32
OPENPLUGINLIB_DECLSPEC string plugins_path( );
OPENPLUGINLIB_DECLSPEC string kernels_path( );
OPENPLUGINLIB_DECLSPEC string shaders_path( );
#endif

namespace detail
{
	class OPENPLUGINLIB_DECLSPEC discover_query_impl
	{		
	public:
		class OPENPLUGINLIB_DECLSPEC plugin_proxy
		{
		public:
			plugin_proxy( plugin_item item )
				: item_( item )
			{ }

			opl_ptr create_plugin( const string& options ) const;

		public:
			wstring name( ) const
			{ return item_.name; }
			wstring type( ) const
			{ return item_.type; }
			wstring mime( ) const
			{ return item_.mime; }
			wstring category( ) const
			{ return item_.category; }
			wstring libname( ) const
			{ return item_.libname; }
			wstring in_filter( ) const
			{ return item_.in_filter; }
			wstring out_filter( ) const
			{ return item_.out_filter; }
			wstring extension( ) const
			{
				wstring ext;
				for( std::vector<wstring>::const_iterator I = item_.extension.begin( ); I != item_.extension.end( ); ++I )
					ext += *I + L" ";
					
				return ext;
			}
			int merit( ) const
			{ return item_.merit; }
			void* context( ) const
			{ return item_.context; }

		private:
			plugin_item item_;
		};

	public:
		typedef std::vector<plugin_proxy>	container;
		typedef container::const_iterator	const_iterator;
		typedef container::iterator			iterator;
		typedef container::size_type		size_type;
		typedef	container::value_type		container_value_type;

	public:
		bool operator( )( const wstring& libname, const wstring& type, const wstring& to_match );

		const_iterator begin( ) const
		{ return plugins_.begin( ); }
		const_iterator end( ) const
		{ return plugins_.end( ); }

		bool empty( ) const
		{ return plugins_.empty( ); }
		size_type size( ) const
		{ return plugins_.size( ); }

		template<class StrictWeakOrdering>
		void sort( const StrictWeakOrdering& comp = StrictWeakOrdering( ) )
		{ std::sort( plugins_.begin( ), plugins_.end( ), comp ); }

	private:
		container plugins_;
	};
}

template<class query = default_query_traits>
class discovery
{
public:
	typedef	query												query_type;
	typedef detail::discover_query_impl::const_iterator 		const_iterator;
	typedef detail::discover_query_impl::size_type				size_type;
	typedef detail::discover_query_impl::container_value_type	container_value_type;

public:
	explicit discovery( const query& q = query( ) )
	{ discoverer_( q.libname( ), q.type( ), q.to_match( ) ); }

	const_iterator begin( ) const
	{ return discoverer_.begin( ); }
	const_iterator end( ) const
	{ return discoverer_.end( ); }

	bool empty( ) const
	{ return discoverer_.empty( ); }
	size_type size( ) const
	{ return discoverer_.size( ); }

	template<class StrictWeakOrdering>
	void sort( const StrictWeakOrdering& comp = StrictWeakOrdering( ) )
	{ discoverer_.sort( comp ); }

private:
	detail::discover_query_impl discoverer_;
};

struct highest_merit_sort
	: public std::binary_function<detail::discover_query_impl::plugin_proxy, detail::discover_query_impl::plugin_proxy, bool>
{
	bool operator( )( const detail::discover_query_impl::plugin_proxy& x, const detail::discover_query_impl::plugin_proxy& y ) const
	{ return x.merit( ) > y.merit( ); }
};

} }

#if _MSC_VER
#pragma warning ( pop )
#endif

#endif
