
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

namespace opl = olib::openpluginlib;
namespace plugin = olib::openmedialib::ml;
namespace il = olib::openimagelib::il;

namespace olib {

  namespace openmedialib {

    namespace ml {

      class ML_PLUGIN_DECLSPEC template_input : public input_type
	{
	public:
	  // Constructor and destructor
	  template_input( );

	  virtual ~template_input( );

	  // Basic information
	  // this is the name of the plugin
	  virtual const opl::wstring get_uri( ) const;
	  virtual const opl::wstring get_mime_type( ) const;

	  // Audio/Visual
	  virtual int get_frames( ) const;
	  virtual bool is_seekable( ) const;

	  // Visual
	  virtual void get_fps( int &num, int &den ) const;

	  virtual void get_sar( int &num, int &den ) const;

	  virtual int get_video_streams( ) const;

	  virtual int get_width( ) const;

	  virtual int get_height( ) const;
	  // Audio
	  virtual int get_audio_streams( ) const;

	  // Fetch method
	  virtual frame_type_ptr fetch( );

	};

      class ML_PLUGIN_DECLSPEC template_store : public store_type
	{
	public:
	  template_store( );
	  virtual ~template_store( );

	  virtual bool push( frame_type_ptr frame );

	  virtual frame_type_ptr flush( );

	  virtual void complete( );
	};

      class ML_PLUGIN_DECLSPEC template_filter : public filter_type
	{
	public:
	  template_filter( );

	  virtual const opl::wstring get_uri( ) const;

	  void fill( image_type_ptr img, size_t plane, unsigned char val );

	  virtual frame_type_ptr fetch( );

	private:
	  pcos::property prop_u_;
	  pcos::property prop_v_;
	};

      //
      // Plugin object
      //

      class ML_PLUGIN_DECLSPEC template_plugin : public openmedialib_plugin
	{
	  int myid;
	public:

	  template_plugin (); // bogus

	  virtual input_type_ptr input(  const opl::wstring & );

	  virtual store_type_ptr store( const opl::wstring &, const frame_type_ptr & );

	  virtual filter_type_ptr filter( const opl::wstring & );

	};

    }

  }
}

//
// Library initialisation mechanism
//

namespace anon2
{
  void reflib( int init );
  //  boost::recursive_mutex mutex12;
}

//
// Access methods for openpluginlib
//

extern "C"
{
  ML_PLUGIN_DECLSPEC bool templateplugin_openplugin_init( void );

  ML_PLUGIN_DECLSPEC bool templateplugin_openplugin_uninit( void );

  ML_PLUGIN_DECLSPEC bool
  templateplugin_openplugin_create_plugin( const char*, 
					   opl::openplugin** plug );

  ML_PLUGIN_DECLSPEC void
  templateplugin_openplugin_destroy_plugin( opl::openplugin* plug );
}
