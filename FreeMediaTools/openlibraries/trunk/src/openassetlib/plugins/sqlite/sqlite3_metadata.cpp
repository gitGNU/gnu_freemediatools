// sqlite3_metadata.cpp
//
// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#include <boost/thread/recursive_mutex.hpp>
#include <openassetlib/plugins/sqlite/sqlite3_metadata_plugin.hpp>

namespace opl = olib::openpluginlib;
namespace oal = olib::openassetlib;

namespace
{
	void reflib( int init )
	{
		static long refs = 0;
	
		assert( refs >= 0 && L"sqlite3_metadata_plugin::refinit: refs is negative." );
		
		if( init > 0 && ++refs )
		{
		}
		else if( init < 0 && --refs == 0 )
		{
		}
	}
}

extern "C"
{
	AL_SQLITE3_DECLSPEC bool openplugin_init( void )
	{
		boost::recursive_mutex mutex;
		
		reflib( 1 );
		
		return true;
	}

	AL_SQLITE3_DECLSPEC bool openplugin_uninit( void )
	{
		boost::recursive_mutex mutex;

		reflib( -1 );
		
		return true;
	}

	AL_SQLITE3_DECLSPEC bool openplugin_create_plugin( const char* szConfig, opl::openplugin** plug )
	{
		*plug = new oal::plugins::sqlite3::sqlite3_metadata_plugin(szConfig);
		
		return true;
	}
	
	AL_SQLITE3_DECLSPEC void openplugin_destroy_plugin( opl::openplugin* plug )
	{
		delete static_cast<oal::plugins::sqlite3::sqlite3_metadata_plugin*>( plug );
	}
}
