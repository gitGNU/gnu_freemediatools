
// openpluginlib - A plugin interface to openlibraries.

// Copyright (C) 2005-2007 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifdef HAVE_CONFIG_H
#include <openlibraries_global_config.hpp>
#endif

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#endif

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/regex.hpp>
#include <boost/tokenizer.hpp>

#ifdef HAVE_OFX
#include <OfxCore.h>
#endif

#include <openpluginlib/pl/registry.hpp>
#include <openpluginlib/pl/utf8_utils.hpp>
#include <openpluginlib/pl/opl_importer.hpp>
#include <openpluginlib/pl/shlib.hpp>

namespace fs = boost::filesystem;

namespace olib { namespace openpluginlib { namespace detail {

namespace
{
#ifdef WIN32
	// Replicate some of OFX types on Win32 to avoid an include dependency.
	// In the unlikely event that they will change then this will have to be
	// updated.
	typedef struct	OfxPropertySetStruct* OfxPropertySetHandle;
	typedef int		OfxStatus;
	typedef			OfxStatus ( OfxPluginEntryPoint )( const char* action, const void* handle, OfxPropertySetHandle inArgs, OfxPropertySetHandle outArgs );

	typedef struct OfxHost
	{
		OfxPropertySetHandle host;
		void *( *fetchSuite )( OfxPropertySetHandle host, const char* suiteName, int suiteVersion );
	} OfxHost;

	typedef struct OfxPlugin
	{
		const char*		pluginApi;
		int				apiVersion;
		const char*		pluginIdentifier; 
		unsigned int 	pluginVersionMajor;
		unsigned int	pluginVersionMinor;
		void ( *setHost )( OfxHost* host );
		OfxPluginEntryPoint *mainEntry;
	} OfxPlugin;
#endif

#if defined( HAVE_OFX ) || defined( WIN32 )
	typedef OfxPlugin*	( *OfxGetPluginFunc )( int );
	typedef int			( *OfxGetNumPluginsFunc )( );
#endif
}

registry*	registry::instance_		 = 0;
bool		registry::destroyed_	 = false;
bool		registry::was_destroyed_ = false;

registry::registry( const string& /*lookup_path*/ )
{
}

registry::~registry( )
{
}

bool registry::insert( const string& lookup_path )
{
	namespace fs = boost::filesystem;

	if( lookup_path.empty( ) )
		return false;

	boost::regex opl_ex( ".*\\.opl", boost::regex::extended | boost::regex::icase );
	boost::regex ofx_ex( ".*\\.ofx\\.bundle", boost::regex::extended | boost::regex::icase );

	typedef boost::tokenizer<boost::char_separator<char> > tokenizer;

#ifdef WIN32
	boost::char_separator<char> sep( ";" );
#else
	boost::char_separator<char> sep( ":" );
#endif

	std::string lp( lookup_path.c_str( ) );

	tokenizer tok( lp.begin( ), lp.end( ), sep );
	for( tokenizer::const_iterator I = tok.begin( ); I != tok.end( ); ++I )
	{
		if( !fs::exists( fs::path( *I, fs::native ) ) || !fs::is_directory( fs::path( *I, fs::native ) ) ) continue;

		fs::directory_iterator end_iter;
		for( fs::directory_iterator dir_iter( fs::path( *I, fs::native ) ); dir_iter != end_iter; ++dir_iter )
		{
			// OLIBs native plugins.
			if( boost::regex_match( ( *dir_iter ).string( ), opl_ex ) )
			{
				opl_importer importer;
				importer( fs::path( ( *dir_iter ).string( ), fs::native ) );
				
				std::copy( importer.plugins.begin( ), importer.plugins.end( ), std::inserter( db_, db_.begin( ) ) );
			}

#		if defined( HAVE_OFX ) || defined( WIN32 )
			// OPL has support for a 3rd party API (OFX). In order to maintain
			// consistency and uniformity within the framework when dealing with
			// plugins, the discovery mechanism has been extended to incorporate
			// discovery of OFX plugins.
			// Subdirectories are not being checked - a must for OFX.
			if( ( *dir_iter ).string( )[ 0 ] != '@' && boost::regex_match( ( *dir_iter ).string( ), ofx_ex ) )
			{
				std::string plugin_shared = ( *dir_iter ).string( );
#		ifdef WIN32
				plugin_shared += "/Contents/Win32/" + 
									std::string( plugin_shared.begin( ) + plugin_shared.find_last_of( '/' ), 
												 plugin_shared.begin( ) + plugin_shared.rfind( ".bundle" ) );
#		elif defined __APPLE__
#		else
				plugin_shared += "/Contents/Linux-x86/" + 
									std::string( plugin_shared.begin( ) + plugin_shared.find_last_of( '/' ), 
												 plugin_shared.begin( ) + plugin_shared.rfind( ".bundle" ) );
#		endif
				module_t module = dlopen_( plugin_shared.c_str( ) );
				if( module )
				{
					OfxGetNumPluginsFunc num_plugins = ( OfxGetNumPluginsFunc ) dlsym_( module, "OfxGetNumberOfPlugins" );
					OfxGetPluginFunc get_plugin = ( OfxGetPluginFunc ) dlsym_( module, "OfxGetPlugin" );

					// OFX plugin instantiation is a little bit more complicated than shown here.
					// What's below is a simplification. Please check the spec for full details.
					if( num_plugins && get_plugin )
					{
						int n = num_plugins( );
						for( int i = 0; i < n; ++i )
						{
							OfxPlugin* plugin = get_plugin( i );
							
							// For each plugin build the opl item struct.
							plugin_item item;
							
							item.name = L"ofx_" + to_wstring( plugin->pluginIdentifier );
							item.extension.push_back( item.name );
							item.type = L"ofx";
							item.category = to_wstring( plugin->pluginApi );
							item.context = plugin;
							
							db_.insert( container::value_type( L"OFX", item ) );
						}
					}
				}
			}
#		endif
		}
	}

	return true;
}

bool registry::remove( )
{
	return false;
}

void registry::clear( )
{
	db_.clear( );	
}

} } }
