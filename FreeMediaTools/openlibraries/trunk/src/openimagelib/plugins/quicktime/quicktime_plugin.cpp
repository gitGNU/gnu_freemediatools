
// QuickTime - An QuickTime plugin to il.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifdef WIN32
#include <ImageCompression.h>
#include <QuickTimeComponents.h>
#include <QTML.h>
#else
#include <QuickTime/ImageCompression.h>
#include <QuickTime/QuickTimeComponents.h>
#endif

#include <boost/thread/recursive_mutex.hpp>

#include <openpluginlib/pl/utf8_utils.hpp>
#include <openimagelib/il/openimagelib_plugin.hpp>

namespace il = olib::openimagelib::il;
namespace pl = olib::openpluginlib;

namespace olib { namespace openimagelib { namespace il {

namespace
{
	void destroy( il::image_type* im )
	{ delete im; }
	
	il::image_type_ptr load_quicktime( const pl::string& uri )
	{
		OSStatus err;
		
		FSSpec fsspec;
#ifdef WIN32
		err = NativePathNameToFSSpec( const_cast<char*>( uri.c_str( ) ), &fsspec, kErrorIfFileNotFound );
#else
		FSRef ref;
		err = FSPathMakeRef( ( const UInt8* ) uri.c_str( ), &ref, 0 );
		if( err != noErr )
			return il::image_type_ptr( );

		err = FSGetCatalogInfo( &ref, kFSCatInfoNone, NULL, NULL, &fsspec, NULL );
#endif
		if( err != noErr )
			return il::image_type_ptr( );
		
		GraphicsImportComponent gi_comp = 0;
		err = GetGraphicsImporterForFile( &fsspec, &gi_comp );
		if( err != noErr )
			return il::image_type_ptr( );
		
		Rect rect;
		err = GraphicsImportGetNaturalBounds( gi_comp, &rect );
		if( err != noErr )
			return il::image_type_ptr( );
			
		ImageDescriptionHandle image_desc;
		image_desc = ( ImageDescriptionHandle ) NewHandle( sizeof( ImageDescriptionHandle ) );
		
		HLock( ( Handle ) image_desc );
		
		err = GraphicsImportGetImageDescription( gi_comp, &image_desc );
		if( err != noErr )
			return il::image_type_ptr( );
		
		int width  = rect.right  - rect.left;
		int height = rect.bottom - rect.top;
		
		il::image_type_ptr im = il::allocate( L"a8r8g8b8", width, height );
		if( !im )
			return il::image_type_ptr( );
			
		GWorldPtr gworld;
		NewGWorldFromPtr( &gworld, k32ARGBPixelFormat, &rect, 0, 0, 0, ( Ptr ) im->data( ), im->pitch( ) );
		if( gworld == NULL )
		{
			CloseComponent( gi_comp );
			return il::image_type_ptr( );
		}
		
		GDHandle device;
		CGrafPtr port;
		GetGWorld( &port, &device );
		
		MatrixRecord matrix;
		SetIdentityMatrix( &matrix );
		err = GraphicsImportSetMatrix( gi_comp, &matrix );
		if( err == noErr )
			err = GraphicsImportSetGWorld( gi_comp, gworld, 0 );
		if( err == noErr )
			err = GraphicsImportSetQuality( gi_comp, codecLosslessQuality );
		if( ( err == noErr ) && GetGWorldPixMap( gworld ) && LockPixels( GetGWorldPixMap( gworld ) ) )
			GraphicsImportDraw( gi_comp );
		else
		{
			DisposeGWorld( gworld );
			CloseComponent( gi_comp );
			
			gworld = 0;
			
			return il::image_type_ptr( );
		}
		
		UnlockPixels( GetGWorldPixMap( gworld ) );
		CloseComponent( gi_comp );
		SetGWorld( port, device );
		DisposeGWorld( gworld );
		
		im = il::convert( im, L"a8b8g8r8" );
		//im->set_flipped( true );
		return im;
	}
}

struct IL_DECLSPEC qt_plugin : public il::openimagelib_plugin
{
	virtual il::image_type_ptr load( const pl::string& uri )
	{ return load_quicktime( uri ); }
	
	virtual bool store( const pl::string& uri, il::image_type_ptr )
	{ return false; }
};

} } }

#ifdef WIN32
namespace
{
	bool reflib( int init )
	{
		static long refs = 0;

		assert( refs >= 0 && L" QuickTime_plugin::refinit: refs is negative." );
		
		if( init > 0 && ++refs == 1 )
		{
			OSStatus err = InitializeQTML( 0L );
			if( err != noErr )
				return false;
		}
		else if( init < 0 && --refs == 0 )
		{
		}
		
		return true;
	}
	
	boost::recursive_mutex mutex;
}
#endif

extern "C"
{
	IL_DECLSPEC bool openplugin_init( void )
	{
#ifdef WIN32
		boost::recursive_mutex::scoped_lock lock( mutex );
	
		return reflib( 1 );
#else
		return true;
#endif
	}

	IL_DECLSPEC bool openplugin_uninit( void )
	{
#ifdef WIN32
		boost::recursive_mutex::scoped_lock lock( mutex );
		
		return reflib( -1 );
#else
		return true;
#endif
	}

	IL_DECLSPEC bool openplugin_create_plugin( const char*, pl::openplugin** plug )
	{
		*plug = new il::qt_plugin;
		return true;
	}
	
	IL_DECLSPEC void openplugin_destroy_plugin( pl::openplugin* plug )
	{ delete static_cast<il::qt_plugin*>( plug ); }
}

#ifdef WIN32
extern "C" BOOL WINAPI DllMain( HINSTANCE hInstDLL, DWORD fdwReason, LPVOID )
{
	if( fdwReason == DLL_PROCESS_ATTACH )
		DisableThreadLibraryCalls( hInstDLL );
	
	return TRUE;
}
#endif
