
// EXR - A OpenEXR plugin to QImage.

// Copyright (C) 2000-2006 VM Inc.
// Released under the GPL.

#include <memory>

#include <qimage.h>
#include <qdatastream.h>

#include <openpluginlib/pl/openpluginlib.hpp>
#include <openpluginlib/pl/utf8_utils.hpp>

#include <openimagelib/il/openimagelib_plugin.hpp>
#include <boost/filesystem.hpp>

#include "exr.h"

namespace opl = olib::openpluginlib;
namespace il  = olib::openimagelib::il;
namespace fs  = boost::filesystem;

namespace
{
	struct query_traits
	{
		query_traits( const opl::wstring& filename )
			: filename_( filename )
		{ }
		
		opl::wstring libname( ) const
		{ return L"openimagelib"; }
		
		opl::wstring to_match( ) const
		{ return filename_; }

		opl::wstring type( ) const
		{ return L""; }
		
		int merit( ) const
		{ return 0; }
		
		opl::wstring filename_;
	};
}


class mydiscovery
// :public opl::discovery<myquery_traits>
{
public:
  mydiscovery(const query_traits &)
  {

  };

  bool  empty ()
  {
    return true;
  }
  size_t size ()
  {
    return 0;
  }
};

void vmfx_exr_read( QImageIO* io )
{
  //	typedef opl::discovery<query_traits> oil_discovery;
	
	mydiscovery plugins( query_traits( opl::to_wstring( io->fileName( ).ascii( ) ) ) );
	if( plugins.empty( ) ) return; 
	
#ifdef USEPLUGINS
	oil_discovery::const_iterator i = plugins.begin( );
	boost::shared_ptr<il::openimagelib_plugin> plug = boost::shared_dynamic_cast<il::openimagelib_plugin>( i->create_plugin( "" ) );
	if( !plug ) return;

	il::image_type_ptr image = plug->load( fs::path( io->fileName( ).ascii( ), fs::native ).string() );
	if( !image ) return;

	image = il::conform( image, il::cropped | il::flipped | il::flopped );
	image = il::convert( image, L"r8g8b8" );

	QImage im;
	if( !im.create( image->width( ), image->height( ), 32 ) )
		return;
	
	unsigned char* data = image->data( );
	
	for( int y = 0; y < image->height( ); ++y )
	  {
	    QRgb* scanline = ( QRgb* ) im.scanLine( y );
	    
	    for( int x = 0; x < image->width( ); ++x, data += 3 )
	      scanline[ x ] = qRgb( data[ 0 ], data[ 1 ], data[ 2 ] );
	    
	    data += image->pitch( ) - image->linesize( );
	  }
	
	QImage new_im = im.mirror( image->is_flopped( ), image->is_flipped( ) );
	
	io->setImage( new_im );
	io->setStatus( 0 );
#endif

}

void vmfx_exr_write( QImageIO* /*io*/ )
{
}
