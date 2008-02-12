
// openpluginlib - A plugin interface to openlibraries.

// Copyright (C) 2005-2007 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifdef WIN32
#define STRICT
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shlobj.h>
#endif

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#endif

#include <algorithm>
#include <cassert>
#include <functional>

#include <boost/bind.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/regex.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/tokenizer.hpp>

#include <openpluginlib/pl/openpluginlib.hpp>
#include <openpluginlib/pl/registry.hpp>
#include <openpluginlib/pl/utf8_utils.hpp>
#include <openpluginlib/pl/shader_manager.hpp>

namespace fs = boost::filesystem;

namespace olib { namespace openpluginlib {

namespace
{
	typedef detail::registry::container			db_container;
	typedef db_container::const_iterator		db_const_iterator;
	typedef db_container::iterator				db_iterator;
	typedef std::pair<db_iterator, db_iterator>	query_iterator_pair;

	void release_plugins( )
	{
		db_container& el_reg_db = detail::registry::instance( ).get_db( );

		query_iterator_pair pair;
		pair.first  = el_reg_db.begin( );
		pair.second = el_reg_db.end( );

		while( pair.first != pair.second )
		{
			release_shared_symbols( ( *pair.first ).second.resolver );
			++pair.first;
		}
	}

	class add_to_filter_string : public std::unary_function<detail::registry::container::value_type, void>
	{
	public:
		explicit add_to_filter_string( wstring& str, bool in_filter )
			: str_( str )
			, in_filter_( in_filter )
		{ }
		
		void operator( )( const detail::registry::container::value_type& str )
		{
			wstring filter;
			if( in_filter_ )
				filter = str.second.in_filter;
			else
				filter = str.second.out_filter;

			if( str_.find( filter ) == wstring::npos )
				str_ += filter + wstring( L" " );
		}

		wstring filter( ) const
		{ return str_; }

	private:
		add_to_filter_string& operator=( const add_to_filter_string& );

	private:
		wstring& str_;
		bool in_filter_;
	};

	// OFX search paths.
	string_list get_ofx_plugin_path( )
	{
		string_list paths;
		
		// Follow the discovery algorithm as in the OFX spec:
		// search for paths in the OFX_PLUGIN_PATH
		// environment variable, look in Common Files (both
		// localised and non-localised locations).
		char* ofx_path_env = getenv( "OFX_PLUGIN_PATH" );
		if( ofx_path_env )
		{
			std::string plugin_path( ofx_path_env );
			
			typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
#	ifdef WIN32
			boost::char_separator<char> sep( ";" );
#	else
			boost::char_separator<char> sep( ":" );
#	endif
			tokenizer tok( plugin_path.begin( ), plugin_path.end( ), sep );
			for( tokenizer::const_iterator I = tok.begin( ); I != tok.end( ); ++I )
			{
				if( !fs::exists( fs::path( *I, fs::native ) ) || !fs::is_directory( fs::path( *I, fs::native ) ) ) continue;
				paths.push_back( *I );
			}
		}
		
#	ifdef WIN32
		TCHAR buffer[ MAX_PATH ];
		SHGetFolderPath( NULL, CSIDL_PROGRAM_FILES_COMMON, NULL, SHGFP_TYPE_CURRENT, buffer );
		
		paths.push_back( to_string( wstring( buffer ) ) + "\\OFX\\Plugins" );
		
		// Check non-localised path for backward compatibility reasons.
		if( wcscmp( buffer, L"C:\\Program Files\\Common Files" ) )
			paths.push_back( "C:\\Program Files\\Common Files\\OFX\\Plugins" );
#	elif defined __APPLE__
		paths.push_back( "/Library/OFX/Plugins" );
#	else
		paths.push_back( "/usr/OFX/Plugins" );
#	endif

		return paths;
	}

	void reflib( int init, const string& lookup_path = "", int mode = 0 )
	{
		static long refs = 0;

		assert( refs >= 0 && L"openpluginlib::refinit: refs is negative." );

		detail::registry& el_reg = detail::registry::instance( );

		if( init > 0 && ++refs )
		{
			if( refs == 1 )
			{
				if( ( mode & STD_PLUGINS ) != 0 )
				{
#		ifdef WIN32
					el_reg.insert( plugins_path( ) );
#		elif !defined HAVE_UMBRELLAFRAMEWORK
					el_reg.insert( OPENIMAGELIB_PLUGINS );
					el_reg.insert( OPENMEDIALIB_PLUGINS );
					el_reg.insert( OPENOBJECTLIB_PLUGINS );
					el_reg.insert( OPENASSETLIB_PLUGINS );
					el_reg.insert( OPENEFFECTSLIB_PLUGINS );
#		elif defined __APPLE__ && defined HAVE_UMBRELLAFRAMEWORK
					el_reg.insert( "/Library/Frameworks/OpenLibraries.framework/Frameworks/OpenImageLib.framework/PlugIns" );
					el_reg.insert( "/Library/Frameworks/OpenLibraries.framework/Frameworks/OpenMediaLib.framework/PlugIns" );
					el_reg.insert( "/Library/Frameworks/OpenLibraries.framework/Frameworks/OpenAssetLib.framework/PlugIns" );
					el_reg.insert( "/Library/Frameworks/OpenLibraries.framework/Frameworks/OpenEffectsLib.framework/PlugIns" );
#		endif
				}
				
				if( ( mode & OFX_PLUGINS ) != 0 )
				{
					string_list ofx_paths = get_ofx_plugin_path( );
					std::for_each( ofx_paths.begin( ), ofx_paths.end( ), boost::bind( &detail::registry::insert, boost::ref( el_reg ), _1 ) );
				}

#		ifdef HAVE_CG_RUNTIME
				shader_manager_instance( ).create_Cg_context( );
#		endif
			}

			if( !lookup_path.empty( ) )
				el_reg.insert( lookup_path );
		}
		else if( init < 0 && --refs == 0 )
		{
			release_plugins( );
			
			if( !lookup_path.empty( ) )
				el_reg.remove( );
			else
				el_reg.clear( );

#	ifdef HAVE_CG_RUNTIME
			shader_manager_instance( ).destroy_Cg_context( );
#	endif
		}
	}
		
	boost::recursive_mutex mutex;
}

namespace detail
{
	namespace
	{
		class is_match : public std::unary_function<wstring, bool>
		{
		public:
			explicit is_match( const wstring& to_match )
				: to_match_( to_match )
			{ }

			bool operator( )( const wstring& expr )
			{
				boost::wregex ex( expr.c_str( ), boost::wregex::extended | boost::wregex::icase );
				if( boost::regex_match( to_match_.c_str( ), ex ) )
					return true;

				return false;
			}

		private:
			is_match& operator=( const is_match& );

		private:
			const wstring& to_match_;
		};

		bool if_matches_expression( const wstring& to_match, const std::vector<wstring>& expression )
		{
			typedef std::vector<wstring>::const_iterator const_iterator;

			const_iterator I = std::find_if( expression.begin( ), expression.end( ), is_match( to_match ) );

			return I != expression.end( );
		}

		void null_delete( void* )
		{ }
	}

	opl_ptr discover_query_impl::plugin_proxy::create_plugin( const string& options ) const
	{
		if( !item_.resolver.dlopened )
			acquire_shared_symbols( const_cast<plugin_resolver&>( item_.resolver ), item_.filename );

		if( item_.resolver.dlopened )
		{
			if( item_.resolver.init( ) )
			{
				openplugin* plugin = 0;
				item_.resolver.create_plugin( options.c_str( ), &plugin );
	
				if( plugin ) return opl_ptr( plugin, item_.resolver.destroy_plugin );
			}
		}

		return opl_ptr( static_cast<openplugin*>( 0 ), null_delete );
	}

	bool discover_query_impl::operator( )( const wstring& libname, const wstring& type, const wstring& to_match )
	{
		typedef boost::recursive_mutex mutex;

		mutex mtx;

		db_container& el_reg_db = detail::registry::instance( ).get_db( );

		query_iterator_pair pair;
		if( libname.empty( ) )
		{
			pair.first  = el_reg_db.begin( );
			pair.second = el_reg_db.end( );
		}
		else
		{
			pair = el_reg_db.equal_range( libname );
		}

		while( pair.first != pair.second )
		{
			const wstring& item_type					= ( *pair.first ).second.type;
			const std::vector<wstring>& item_extension	= ( *pair.first ).second.extension;

			if( ( type.empty( ) || type == item_type ) && ( to_match.empty( ) || if_matches_expression( to_match, item_extension ) ) )
				plugins_.push_back( ( *pair.first ).second );

			++pair.first;
		}
		
		sort<highest_merit_sort>( );

		return true;
	}
}

#ifdef WIN32
extern "C" BOOL WINAPI DllMain( HINSTANCE hInstDLL, DWORD fdwReason, LPVOID )
{
	if( fdwReason == DLL_PROCESS_ATTACH )
		DisableThreadLibraryCalls( hInstDLL );
	
	return TRUE;
}
#endif

bool init( const string& lookup_path, int mode )
{
	boost::recursive_mutex::scoped_lock lock( mutex );

	reflib( 1, lookup_path, mode );

	return true;
}

bool uninit( )
{
	boost::recursive_mutex::scoped_lock lock( mutex );

	reflib( -1 );

	return true;
}

wstring registered_filters( bool in_filter )
{
	typedef detail::registry::container					db;
	typedef detail::registry::container::const_iterator	const_iterator;
	
	const db& el_reg = detail::registry::instance( ).get_db( );
	
	wstring filter;
	add_to_filter_string f( filter, in_filter );
	
	std::for_each( el_reg.begin( ), el_reg.end( ), f );

	return f.filter( );
}

#ifdef WIN32
namespace
{
	string get_registry_key( const wstring& key_str )
	{
		const int allocsiz = 512;

		TCHAR buf[ allocsiz ];
		DWORD bufsiz = allocsiz;

		string value;
		
		HKEY key;
		if( RegOpenKeyEx( HKEY_LOCAL_MACHINE, L"Software\\OpenLibraries", 0, KEY_READ, &key ) == ERROR_SUCCESS )
		{
			if( RegQueryValueEx( key, key_str.c_str( ), 0, NULL, ( LPBYTE ) buf, &bufsiz ) == ERROR_SUCCESS )
				value = to_string( buf );

			RegCloseKey( key );
		}

		return value;
	}
}

string plugins_path( )
{
#ifndef NDEBUG
	return get_registry_key( L"PluginsDirDebug" );
#else
	return get_registry_key( L"PluginsDirRelease" );
#endif	
}

string kernels_path( )
{ return get_registry_key( L"KernelsDir" ); }

string shaders_path( )
{ return get_registry_key( L"ShadersDir" ); }
#endif 

} }


//template<class query = default_query_traits> class discovery
/*
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
*/
/*
template <class T>
discovery<T>::discovery(T const&)
{
 
}

template <class T>
discovery<T>::empty() const
{

}

template <class T>
discovery<T>::begin() const
{

}

template <class T>
discovery<T>::discovery(T const&)
{

}
*/
