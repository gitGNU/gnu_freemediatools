
// template - A template plugin to ml.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the GPL.
// For more information, see http://www.openlibraries.org.

#include <openmedialib/ml/openmedialib_plugin.hpp>

#ifdef WIN32
#include <windows.h>
#endif // WIN32

#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>

#include <boost/thread/recursive_mutex.hpp>

#include <template_plugin.hpp>

namespace opl = olib::openpluginlib;
namespace plugin = olib::openmedialib::ml;
namespace il = olib::openimagelib::il;

namespace olib { 
  namespace openmedialib { 
    namespace ml {
      
      
      
      template_input::template_input( ) : input_type( )
      {
	
      }
      
      
      template_input::~template_input( )
      {
	
      }
      
      
  const opl::wstring template_input::get_uri( ) const 
  {
    return L"test:"; 
  }
  
  const opl::wstring 
  template_input::get_mime_type( ) const 
  { 
    return L"";
  }

		// Audio/Visual
  int template_input::get_frames( ) const 
  {
    return 250; 
  }

  bool template_input::is_seekable( ) const 
  {
    return true; 
  }

		// Visual
  virtual void template_input::get_fps( int &num, int &den ) const { num = 25; den = 1; }
		virtual void template_input::get_sar( int &num, int &den ) const { num = 1; den = 1; }
		virtual int template_input::get_video_streams( ) const { return 1; }
		virtual int template_input::get_width( ) const { return 512; }
		virtual int template_input::get_height( ) const { return 512; }

		// Audio
		virtual int template_input::get_audio_streams( ) const { return 0; }

		// Fetch method
		virtual template_input::frame_type_ptr fetch( )
		{
			typedef il::image< unsigned char, il::r8g8b8 > r8g8b8_image_type;

			// Construct a frame and populate with basic information
			frame_type *frame = new frame_type( );
			int num, den;
			get_sar( num, den );
			frame->set_sar( num, den );
			frame->set_pts( get_position( ) * 1.0 / 25.0 );
			frame->set_duration( 1.0 / 25.0 );
			frame->set_position( get_position( ) );

			// Generate an image
			int width = get_width( );
			int height = get_height( );
			image_type_ptr image = image_type_ptr( new image_type( r8g8b8_image_type( width, height, 1 ) ) );
			memset( image->data( ), int( 255 * ( double( get_position( ) ) / double( get_frames( ) ) ) ), image->size( ) );
			frame->set_image( image );

			// Return a frame
			return frame_type_ptr( frame );
		}
};

  //class ML_PLUGIN_DECLSPEC template_store : public store_type
  //{
  //	public:
  template_store::template_store( )
    : store_type( )
  { }

  virtual template_store~template_store( )
  { }

  virtual bool template_storepush( frame_type_ptr frame )
  {
    image_type_ptr img = frame->get_image( );
    if ( img != 0 )
      {
	img = il::convert( img, L"r8g8b8" );
	int w = img->width( );
	int h = img->height( );
	fprintf( stdout, "P6\n%d %d\n255\n", w, h );
	fwrite( img->data( ), w * h * 3, 1, stdout );
	fflush( stdout );
      }
    return img != 0;
  }

  virtual frame_type_ptr template_storeflush( )
  { return frame_type_ptr( ); }

  virtual void template_storecomplete( )
  { }



  //class ML_PLUGIN_DECLSPEC template_filter : public filter_type{

		template_filter::template_filter( )
			: filter_type( )
			, prop_u_( pcos::key::from_string( "u" ) )
			, prop_v_( pcos::key::from_string( "v" ) )
		{
			properties( ).append( prop_u_ = 128 );
			properties( ).append( prop_v_ = 128 );
		}

  const opl::wstring template_filter::get_uri( ) const { return L"template:"; }

  void template_filter::fill( image_type_ptr img, size_t plane, unsigned char val )
  {
    unsigned char *ptr = img->data( plane );
    int width = img->width( plane );
    int height = img->height( plane );
    int diff = img->pitch( plane );
    if ( ptr )
      {
	while( height -- )
	  {
	    memset( ptr, val, width );
	    ptr += diff;
	  }
      }
  }
  
  frame_type_ptr template_filter::fetch( )
  {
    acquire_values( );
    
    frame_type_ptr result;
			input_type_ptr input = fetch_slot( );

			if ( input )
			{
				input->seek( get_position( ) );
				result = input->fetch( );
				if ( result && result->get_image( ) )
				{
					image_type_ptr img = result->get_image( );
					if ( img )
					{
						img = il::convert( img, L"yuv420p" );
						fill( img, 1, prop_u_.value< int >( ) );
						fill( img, 2, prop_v_.value< int >( ) );
					}
					result->set_image( img );
				}
			}
			
			return result;
		}

	private:
		pcos::property prop_u_;
		pcos::property prop_v_;
};

//
// Plugin object
//

  input_type_ptr template_plugin::input(  const opl::wstring & )
  {
    return input_type_ptr( new template_input( ) );
  }
  
  store_type_ptr template_plugin::store( const opl::wstring &, const frame_type_ptr & )
  {
    return store_type_ptr( new template_store( ) );
  }
  
  filter_type_ptr template_plugin::filter( const opl::wstring & )
  {
    return filter_type_ptr( new template_filter( ) );
  }


} 

}

}

//
// Library initialisation mechanism
//

namespace anon
{
	void reflib( int init )
	{
		static long refs = 0;

		assert( refs >= 0 && L"template_plugin::refinit: refs is negative." );
		
		if( init > 0 && ++refs )
		{
			// Initialise
		}
		else if( init < 0 && --refs == 0 )
		{
			// Uninitialise
		}
	}
	
	boost::recursive_mutex mutex;
}

//
// Access methods for openpluginlib
//

extern "C"
{
  ML_PLUGIN_DECLSPEC bool templateplugin_openplugin_init( void )
	{
		boost::recursive_mutex::scoped_lock lock( mutex );
		reflib( 1 );
		return true;
	}

	ML_PLUGIN_DECLSPEC bool templateplugin_openplugin_uninit( void )
	{
		boost::recursive_mutex::scoped_lock lock( mutex );
		reflib( -1 );
		return true;
	}
	
	ML_PLUGIN_DECLSPEC bool templateplugin_openplugin_create_plugin( const char*, opl::openplugin** plug )
	{
		*plug = new plugin::template_plugin;
		return true;
	}
	
	ML_PLUGIN_DECLSPEC void templateplugin_openplugin_destroy_plugin( opl::openplugin* plug )
	{ 
		delete static_cast< plugin::template_plugin * >( plug ); 
	}
}


//templateplugin_openplugin_create_plugin
