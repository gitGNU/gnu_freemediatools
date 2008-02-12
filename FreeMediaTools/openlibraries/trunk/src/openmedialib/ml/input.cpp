

// ml - A media library representation.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef OPENMEDIALIB_INPUT_SRC
#define OPENMEDIALIB_INPUT_SRC

#include <openpluginlib/pl/pcos/property_container.hpp>
#include <openmedialib/ml/frame.hpp>

#include <openmedialib/ml/input.hpp>

namespace olib {
  
  namespace openmedialib {
    
    namespace ml {
      
      namespace pcos = olib::openpluginlib::pcos;
      
      // A callback can be registered with each input - the concept being that when a frame
      // is fetched via the fetch method, the application has a means to assign property values
      // prior to the execution.
      //
      // NB: Whilst all inputs accept a callback registration, the execution is dependent on
      // the implementations use of the acquire_values protected method. Whilst it is
      // conceivable that an input might wish to expose this, it's not mandated - it is
      // generally considered more useful for filters.
      
      int y;
      input_callback::~input_callback( )
      {
	
      }


int x;


  // Constructor/destructor
  input_type::input_type( )
    : properties_( )
    , initialized_( false )
    , prop_debug_( pcos::key::from_string( "debug" ) )
    , position_( 0 )
    , process_( process_image | process_audio )
    , callback_( )
    {
      properties( ).append( prop_debug_ = 0 );
    }
  
input_type::~input_type( ) { }

		// Provides a mechanism for defering initialisation from the ctor
 bool input_type::init( ) { 
   if ( !initialized_ ) initialized_ = initialize( ); return initialized_; 
 }
	
 const bool input_type::initialized( ) const 
   {
     return 
       initialized_; 
   }

		// Filters reimplement these
 const size_t input_type::slot_count( ) const 
  {
   return 0; 
  }

 bool input_type::connect( input_type_ptr type , size_t size ) 
{ 
  return false; 

}

input_type_ptr 
input_type::fetch_slot( size_t slot  ) 
  const
{ 
  return input_type_ptr( ); 
}

		// Property object
		pcos::property_container input_type::properties( ) const { return properties_; }

		// Virtual method for state reset
void input_type::reset( ) { }

		// Register the callback
void input_type::register_callback( input_callback_ptr callback )
{ callback_ = callback; 

}

		// Basic information

		// Set video and audio streams
 bool input_type::set_video_stream( const int ) { return false; }
bool input_type::set_audio_stream( const int ) { return false; }

		// Provides a hint to the input implementation - allows the user to say 
		// 'I only want image or audio, or just fetch the nearest intra frame to here' in the next fetch
		// NB: usage is not enforced - up to the implementation to decide whether they do it or not
void input_type::set_process_flags( int flags ) 
{
  process_ = flags; 
}

int input_type::get_process_flags( )
{
  return process_; 
}

		// Default seek functionality
void input_type::seek( const int position, const bool relative  )
{
  if ( relative )
    position_ = ( position_ + position );
  else
    position_ = position;

  if ( position_ < 0 )
    position_ = 0;
  else if ( position_ >= int( get_frames( ) ) )
    position_ = int( get_frames( ) - 1 );
}

// Query the current position
int input_type::get_position( ) const 
{
  return position_; 
}

// Virtual push method for a frame
bool input_type::push( frame_type_ptr ptr ) 
{
  return false; 
}

		// Allow the app to refresh a cached frame
void input_type::refresh_cache( frame_type_ptr ptr) 
{

}

// Invoke the callback - note that it is the input implementation responsibility to
// invoke this (normally as the first line of a fetch) - the properties object provided
// may assist the recipient in determining the specific input object.
void input_type::acquire_values( ) 
{
  if ( callback_ ) 
    callback_->assign( 
		      properties( ), 
		      get_position( ) );
}

		// Determines if the particular input is thread safe (should be re-implemented as required
bool input_type::is_thread_safe( ) const 
{
  return callback_ ? callback_->is_thread_safe( ) : true; 
}

		// Retrieve the callback
input_callback_ptr input_type::fetch_callback( ) const 
{
  return callback_; 
}

// Indication for node reuse
bool input_type::reuse( ) 
{
  return true; 
}

// Determine the debug level
int input_type::debug_level( )
{
  return prop_debug_.value< int >( ); 
}


		// Virtual method for initialization
 bool input_type::initialize( ) 
{
  return true; 
}



bool input_callback::is_thread_safe( ) const 
{ 
  return false; 
}


}
}

}

#endif
