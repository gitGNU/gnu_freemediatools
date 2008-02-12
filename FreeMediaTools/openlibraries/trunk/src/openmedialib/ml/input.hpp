
// ml - A media library representation.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef OPENMEDIALIB_INPUT_INC_
#define OPENMEDIALIB_INPUT_INC_

#include <openpluginlib/pl/pcos/property_container.hpp>
#include <openmedialib/ml/frame.hpp>

namespace olib { namespace openmedialib { namespace ml {

namespace pcos = olib::openpluginlib::pcos;

// A callback can be registered with each input - the concept being that when a frame
// is fetched via the fetch method, the application has a means to assign property values 
// prior to the execution.
//
// NB: Whilst all inputs accept a callback registration, the execution is dependent on 
// the implementations use of the acquire_values protected method. Whilst it is 
// conceivable that an input might wish to expose this, it's not mandated - it is 
// generally considered more useful for filters.

class ML_DECLSPEC input_callback
{
	public:
  virtual ~input_callback( );
		virtual void assign( pcos::property_container, int ) = 0;
  virtual bool is_thread_safe( ) const;
};

typedef boost::shared_ptr< input_callback > input_callback_ptr;

// Enumerated type to allow the user of the input to restrict the scope of the 
// fetch - for example, if you only need audio, there is no need to process the
// associated image.

typedef enum
{
	process_image = 0x1,
	process_audio = 0x2,
	process_next_intra = 0x4
}
process_flags;

// Forward declarations
class input_type;
typedef boost::shared_ptr< input_type > input_type_ptr;

// The input abstract class - implementations will extend this class and (minimally)
// provide the pure virtual methods.

class ML_DECLSPEC input_type
{
	public:
		// Constructor/destructor
  explicit input_type( );
    
  virtual ~input_type( );

		// Provides a mechanism for defering initialisation from the ctor
  bool init( ) ;
  const bool initialized( ) const;

		// Filters reimplement these
  virtual const size_t slot_count( ) const;

  virtual bool connect( input_type_ptr, size_t = 0 );

  virtual input_type_ptr fetch_slot( size_t slot = 0 ) const;
  
  
  // Property object
  pcos::property_container properties( ) const;
  
  // Virtual method for state reset
  virtual void reset( );
  
  // Register the callback
  void register_callback( input_callback_ptr callback );
  
  // Basic information
  // the uri to deal with
  virtual const openpluginlib::wstring get_uri( ) const = 0;
  virtual const openpluginlib::wstring get_mime_type( ) const = 0;
  
  // Audio/Visual
  virtual int get_frames( ) const = 0;
  virtual bool is_seekable( ) const = 0;
  
  // Visual
  virtual int get_video_streams( ) const = 0;
  
  // Audio
  virtual int get_audio_streams( ) const = 0;
  
  // Set video and audio streams
  virtual bool set_video_stream( const int ) ;
  virtual bool set_audio_stream( const int ) ;
  
  // Provides a hint to the input implementation - allows the user to say
  // 'I only want image or audio, or just fetch the nearest intra frame to here' in the next fetch
  // NB: usage is not enforced - up to the implementation to decide whether they do it or not
  void set_process_flags( int flags );
  int get_process_flags( );
  
  // Default seek functionality
  virtual void seek( const int position, const bool relative = false );
   
  // Query the current position
  virtual int get_position( ) const ;
  
  // Virtual frame fetch method
  virtual frame_type_ptr fetch( ) = 0;
  
  // Virtual push method for a frame
  virtual bool push( frame_type_ptr );
  
  // Allow the app to refresh a cached frame
  virtual void refresh_cache( frame_type_ptr );
  
  // Invoke the callback - note that it is the input implementation responsibility to
  // invoke this (normally as the first line of a fetch) - the properties object provided
  // may assist the recipient in determining the specific input object.
  virtual void acquire_values( );
  
  // Determines if the particular input is thread safe (should be re-implemented as required
  virtual bool is_thread_safe( ) const;
  
  // Retrieve the callback
  input_callback_ptr fetch_callback( ) const;
  
  // Indication for node reuse
  virtual bool reuse( ) ;
  
  // Determine the debug level
   int debug_level( ) ;
  
protected:
  // Virtual method for initialization
  virtual bool initialize( ) ;
  
private:
  pcos::property_container properties_;
  bool initialized_;
  pcos::property prop_debug_;
  int position_;
  int process_;
  input_callback_ptr callback_;
};

} 

} 

}

#endif
