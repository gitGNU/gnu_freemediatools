
// ml - A media library representation.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef OPENMEDIALIB_FRAME_INC_
#define OPENMEDIALIB_FRAME_INC_

#include <openimagelib/il/il.hpp>
#include <openmedialib/ml/audio.hpp>
#include <boost/shared_ptr.hpp>
#include <openpluginlib/pl/pcos/property_container.hpp>

namespace olib { namespace openmedialib { namespace ml {

namespace pcos = olib::openpluginlib::pcos;

typedef olib::openimagelib::il::image_type image_type;
typedef olib::openimagelib::il::image_type_ptr image_type_ptr;

class frame_type;
typedef boost::shared_ptr< frame_type > frame_type_ptr;

class ML_DECLSPEC frame_type 
{
	public:
		explicit frame_type( ) 
			: properties_( )
			, image_( )
			, alpha_( )
			, audio_( )
			, pts_( 0.0 )
			, position_( 0 )
			, duration_( 0.0 )
			, sar_num_( 1 )
			, sar_den_( 1 )
			, fps_num_( 25 )
			, fps_den_( 1 )
		{ }

		virtual ~frame_type( ) 
		{ }

		static frame_type_ptr shallow_copy( const frame_type_ptr &other )
		{
			frame_type_ptr result;
			if ( other )
			{
				frame_type *copy = new frame_type( );
				std::auto_ptr< pcos::property_container > clone( other->properties_.clone() );
				copy->properties_ = *clone.get( );
				copy->image_ = other->image_;
				copy->alpha_ = other->alpha_;
				copy->audio_ = other->audio_;
				copy->pts_ = other->pts_;
				copy->position_ = other->position_;
				copy->duration_ = other->duration_;
				copy->sar_num_ = other->sar_num_;
				copy->sar_den_ = other->sar_den_;
				copy->fps_num_ = other->fps_num_;
				copy->fps_den_ = other->fps_den_;
				result = frame_type_ptr( copy );
			}
			return result;
		}

		static frame_type_ptr deep_copy( const frame_type_ptr &other )
		{
			frame_type_ptr result;
			if ( other )
			{
				frame_type *copy = new frame_type( );
				std::auto_ptr< pcos::property_container > clone( other->properties_.clone() );
				copy->properties_ = *clone.get( );
				if ( other->image_ )
					copy->image_ = ml::image_type_ptr( other->image_->clone( ) );
				if ( other->alpha_ )
					copy->alpha_ = ml::image_type_ptr( other->alpha_->clone( ) );
				if ( other->audio_ )
				{
					typedef audio< unsigned char, pcm16 > pcm16;
					audio_type *aud = new audio_type( pcm16( other->audio_->frequency( ), other->audio_->channels( ), other->audio_->samples( ) ) );
					memcpy( aud->data( ), other->audio_->data( ), aud->size( ) );
					copy->audio_ = ml::audio_type_ptr( aud );
				}
				copy->pts_ = other->pts_;
				copy->position_ = other->position_;
				copy->duration_ = other->duration_;
				copy->sar_num_ = other->sar_num_;
				copy->sar_den_ = other->sar_den_;
				copy->fps_num_ = other->fps_num_;
				copy->fps_den_ = other->fps_den_;
				result = frame_type_ptr( copy );
			}
			return result;
		}

		pcos::property_container properties( ) { return properties_; }

		virtual void set_image( image_type_ptr image ) { image_ = image; }
		virtual image_type_ptr get_image( ) { return image_; }
		virtual void set_alpha( image_type_ptr image ) { alpha_ = image; }
		virtual image_type_ptr get_alpha( ) { return alpha_; }
		virtual void set_audio( audio_type_ptr audio ) { audio_ = audio; }
		virtual audio_type_ptr get_audio( ) { return audio_; }
		virtual void set_pts( double pts ) { pts_ = pts; }
		virtual double get_pts( ) const { return pts_; }
		virtual void set_position( int position ) { position_ = position; }
		virtual int get_position( ) const { return position_; }
		virtual void set_duration( double duration ) { duration_ = duration; }
		virtual double get_duration( ) const { return duration_; }
		virtual void set_sar( int num, int den ) { sar_num_ = num; sar_den_ = den; }
		virtual void get_sar( int &num, int &den ) const { num = sar_num_; den = sar_den_; }
		virtual void set_fps( int num, int den ) { fps_num_ = num; fps_den_ = den; }
		virtual void get_fps( int &num, int &den ) const { num = fps_num_; den = fps_den_; }

		virtual int get_fps_num( ) { int num, den; get_fps( num, den ); return num; }
		virtual int get_fps_den( ) { int num, den; get_fps( num, den ); return den; }
		virtual int get_sar_num( ) { int num, den; get_sar( num, den ); return num; }
		virtual int get_sar_den( ) { int num, den; get_sar( num, den ); return den; }

		virtual double aspect_ratio( )
		{
			int num, den;
			get_sar( num, den );
			if ( num == 0 )
				return ( double )get_image( )->width( ) / get_image( )->height( );
			else
				return ( ( double ) num / den ) * ( ( double )get_image( )->width( ) / get_image( )->height( ) );
		}

		virtual double fps( ) const
		{
			int num, den;
			get_fps( num, den );
			return den != 0 ? double( num ) / double( den ) : 1;
		}

		virtual double sar( ) const
		{
			int num, den;
			get_sar( num, den );
			return den != 0 ? double( num ) / double( den ) : 1;
		}

	private:
		pcos::property_container properties_;
		image_type_ptr image_;
		image_type_ptr alpha_;
		audio_type_ptr audio_;
		double pts_;
		int position_;
		double duration_;
		int sar_num_;
		int sar_den_;
		int fps_num_;
		int fps_den_;
};

} } }

#endif
