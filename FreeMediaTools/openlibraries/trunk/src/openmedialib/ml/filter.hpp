// ml - A media library representation.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef OPENMEDIALIB_FILTER_INC_
#define OPENMEDIALIB_FILTER_INC_

#include <vector>
#include <boost/shared_ptr.hpp>
#include <openmedialib/ml/frame.hpp>
#include <openmedialib/ml/input.hpp>
#include <openpluginlib/pl/utf8_utils.hpp>

namespace olib { namespace openmedialib { namespace ml {

namespace opl  = olib::openpluginlib;
namespace pcos = olib::openpluginlib::pcos;

class ML_DECLSPEC filter_type : public input_type
{
	public:
		explicit filter_type( ) : input_type( ), slots_( ), position_( 0 ) { slots_.push_back( input_type_ptr( ) ); }
		virtual ~filter_type( ) { }

		// Filters can request any number of inputs - override this method as required
		virtual const size_t slot_count( ) const 
		{ return 1; }

		// Connect an input to a slot
		virtual bool connect( input_type_ptr input, size_t slot = 0 )
		{
			if ( slots_.size( ) < slot_count( ) )
				for ( size_t i = slots_.size( ); i < slot_count( ); i ++ )
					slots_.push_back( input_type_ptr( ) );

			bool result = slot < slot_count( );
			if ( result )
			{
				slots_[ slot ] = input;
				if ( input )
					seek( input->get_position( ) );
				on_slot_change( input, static_cast<int>(slot) );
			}

			return result;
		}

		// NB: The default implementation assumes that the filter wraps a single connected
		// input and that it doesn't affect duration, image constitution or image/audio 
		// availability - a specific implementation of a filter can override all or part
		// of this behaviour (ie: to allow temporal interpolation, transitions, audio
		// visualisation, rescalers and so on). It might prove beneficial for OML to 
		// provide additional subtypes, but for now, this will be seen as an exercise for
		// the plugin implementation to address.
		//
		// Whilst no rules are enforced, the conceptual view is that even in the case of a
		// multiple input filter, the first connected input drives the rules of the output
		// (ie: frame rate, duration, aspect ratio, colour space, audio params and so on) - 
		// thus the output should conform to the first input. In other words, the default
		// implementation of these methods should be sufficient for the majority of cases.

		// Basic information
		virtual const openpluginlib::wstring get_uri( )  const = 0;

		virtual const openpluginlib::wstring get_mime_type( ) const
		{ return slots_[ 0 ] ? slots_[ 0 ]->get_mime_type( ) : L""; }

		// Default seek functionality
		// NB: This implementation allows position to overrun the length of the clip
		// whereas the input implementation clamps the position to the length
		virtual void seek( const int position, const bool relative = false )
		{
			if ( relative )
				position_ = ( position_ + position );
			else
				position_ = position;

			if ( position_ < 0 ) 
				position_ = 0;
		}

		// Query the current position
		virtual int get_position( ) const { return position_; }

		// Audio/Visual
		virtual int get_frames( ) const
		{ return slots_[ 0 ] ? slots_[ 0 ]->get_frames( ) : 0; }

		virtual bool is_seekable( ) const 
		{ return slots_[ 0 ] ? slots_[ 0 ]->is_seekable( ) : false; }

		// Visual
		virtual int get_video_streams( ) const
		{ return slots_[ 0 ] ? slots_[ 0 ]->get_video_streams( ) : 0; }

		// Audio
		virtual int get_audio_streams( ) const
		{ return slots_[ 0 ] ? slots_[ 0 ]->get_audio_streams( ) : 0; }

		// Set video and audio streams
		virtual bool set_video_stream( const int stream )
		{ return slots_[ 0 ] ? slots_[ 0 ]->set_video_stream( stream ) : false; }

		virtual bool set_audio_stream( const int stream )
		{ return slots_[ 0 ] ? slots_[ 0 ]->set_audio_stream( stream ) : false; }

		// Virtual frame fetch method
		virtual frame_type_ptr fetch( ) = 0;

		// Invoked when a slot is changed
		virtual void on_slot_change( input_type_ptr, int ) { }

		// Retrieve the input associated to a slot
		virtual input_type_ptr fetch_slot( size_t slot = 0 ) const
		{ return slot < slots_.size( ) ? slots_[ slot ] : input_type_ptr( ); }

		virtual void acquire_values( ) 
		{
			if ( fetch_callback( ) ) 
				fetch_callback( )->assign( properties( ), get_position( ) ); 
			for( std::vector< input_type_ptr >::iterator iter = slots_.begin( ); iter < slots_.end( ); iter ++ )
				if ( *iter )
					( *iter )->set_process_flags( get_process_flags( ) );
		}

		virtual bool is_thread_safe( )
		{
			bool result = true;
			if ( fetch_callback( ) )
				result = fetch_callback( )->is_thread_safe( ); 
			for( std::vector< input_type_ptr >::const_iterator iter = slots_.begin( ); result && iter < slots_.end( ); iter ++ )
				if ( *iter )
					result = ( *iter )->is_thread_safe( );
			return result;
		}

		// By default, filters aren't reused
		virtual bool reuse( ) { return false; }

	protected:

		// Convenience function - fetch a frame from the slot and optionally ensure frame properties
		// are assigned to the filter
		frame_type_ptr fetch_from_slot( int index = 0, bool assign = true )
		{
			frame_type_ptr result;
			input_type_ptr input = fetch_slot( index );
			if ( input )
			{
				input->seek( get_position( ) );
				result = input->fetch( );
				if ( assign )
					assign_frame_props( result );
			}
			return result;
		}

		void assign_frame_props( frame_type_ptr frame )
		{
			if ( frame )
			{
				// Obtain the keys on the filter
				pcos::key_vector props = properties( ).get_keys( );

				// For each key...
				for( pcos::key_vector::iterator it = props.begin( ); it != props.end( ); it ++ )
				{
					// Fetch the name and value property
					std::string name( ( *it ).as_string( ) );

					if ( debug_level( ) & 4 ) fprintf( stderr, "name = %s\n", name.c_str( ) );

					if ( name.find( ":" ) != std::string::npos )
						name = name.substr( 0, name.find( ":" ) );

					if ( debug_level( ) & 1 ) fprintf( stderr, "found = %s\n", name.c_str( ) );

					pcos::property input = properties( ).get_property_with_string( name.c_str( ) );

					// Deal with the name part first to determine what the dst property is
					pcos::property dst( pcos::property::NULL_PROPERTY );
					if ( name.substr( 0, 2 ) == "@@" )
						dst = frame->properties( ).get_property_with_string( name.substr( 2 ).c_str( ) );
					else if ( name[ 0 ] == '@' )
						dst = properties( ).get_property_with_string( name.substr( 1 ).c_str( ) );

					// If we don't have a valid dst, then there's no property assignment
					if ( dst.valid( ) )
					{
						std::string value = opl::to_string( input.value< opl::wstring >( ) ).c_str( );

						// Check if there's a default value specified (syntax being <property>:<default>)
						std::string::size_type default_provided = value.find( ":" );
						std::string default_value;

						if ( default_provided != std::string::npos )
						{
							default_value = value.substr( default_provided + 1 );
							value = value.substr( 0, default_provided );
						}

						// Obtain the src
						pcos::property src( pcos::property::NULL_PROPERTY );
						if ( debug_level( ) & 2 ) fprintf( stderr, "looking for = %s\n", value.c_str( ) );
						if ( value.substr( 0, 2 ) == "@@" )
							src = frame->properties( ).get_property_with_string( value.substr( 2 ).c_str( ) );
						else if ( value[ 0 ] == '@' )
							src = properties( ).get_property_with_string( value.substr( 1 ).c_str( ) );

						// Now assign as required
						if ( src.valid( ) && dst.is_a< int >( ) && src.is_a< double >( ) )
							dst = int( src.value< double >( ) );
						else if ( src.valid( ) )
							dst.set_from_property( src );
						else if ( default_provided != std::string::npos )
							dst.set_from_string( opl::to_wstring( default_value ) );
						else if ( value[ 0 ] != '@' )
							dst.set_from_string( opl::to_wstring( value ) );
					}
				}
			}
		}

	private:
		std::vector < input_type_ptr > slots_;
		int position_;
};

typedef boost::shared_ptr< filter_type > filter_type_ptr;

} } }

#endif
