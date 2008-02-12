
// tonemap - Tonemapping plugin functionality.

// Copyright (C) 2007 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifdef HAVE_CONFIG_H
#include <openlibraries_global_config.hpp>
#endif

#include <cmath>
#include <limits>

#ifdef HAVE_OPENEXR
#ifdef WIN32
#	define PLATFORM_WINDOWS
#endif
#include <ImfConvert.h>
#include <ImathMath.h>
#include <ImathFun.h>
#include <halfFunction.h>
#endif

#include <openmedialib/ml/openmedialib_plugin.hpp>

namespace pl = olib::openpluginlib;
namespace plugin = olib::openmedialib::ml;
namespace il = olib::openimagelib::il;
namespace pcos = olib::openpluginlib::pcos;

namespace olib { namespace openmedialib { namespace ml {

namespace
{
	typedef il::image_type::size_type size_type;

#ifdef HAVE_OPENEXR
	// From ILM OpenEXR examples.
	float knee (double x, double f)
	{ return float( Imath::Math<double>::log( x * f + 1 ) / f ); }

	float findKneeF( float x, float y )
	{
		float f0 = 0;
		float f1 = 1;

		while( knee( x, f1 ) > y )
		{
			f0 = f1;
			f1 = f1 * 2;
		}

		for( int i = 0; i < 30; ++i )
		{
			float f2 = ( f0 + f1 ) / 2;
			float y2 = knee( x, f2 );

			if( y2 < y )
				f1 = f2;
			else
				f0 = f2;
		}

		return ( f0 + f1 ) / 2.0f;
	}

	struct gamma
	{
		explicit gamma( float exposure, float defog, float kneeLow, float kneeHigh )
			: m( Imath::Math<float>::pow( 2, exposure + 2.47393f ) )
			, d( defog )
			, kl( Imath::Math<float>::pow( 2, kneeLow ) )
			, f( findKneeF( Imath::Math<float>::pow( 2, kneeHigh ) - kl, Imath::Math<float>::pow( 2, 3.5 ) - kl ) )
		{ }

		float operator( )( half h )
		{
			// Defog
			float x = ( std::max )( 0.0f, ( h - d ) );

			// Exposure
			x *= m;

			// Knee
			if( x > kl )
				x = kl + knee( x - kl, f );

			// Gamma
			x = Imath::Math<float>::pow( x, 0.4545f );

			// Scale and clamp
			return Imath::clamp( x * 84.66f, 0.f, 255.f );
		}
    
		float m, d, kl, f;
	};

	unsigned char dither( float v, int x, int y )
	{
		static const float d[ 4 ][ 4 ] =
		{
			0.0f  / 16.0f,  8.0f / 16.0f,  2.0f / 16.0f, 10.0f / 16.0f,
			12.0f / 16.0f,  4.0f / 16.0f, 14.0f / 16.0f,  6.0f / 16.0f,
			3.0f  / 16.0f, 11.0f / 16.0f,  1.0f / 16.0f,  9.0f / 16.0f,
			15.0f / 16.0f,  7.0f / 16.0f, 13.0f / 16.0f,  5.0f / 16.0f,
		};

		return ( unsigned char ) ( v + d[ y & 3 ][ x & 3 ] );
	}

	void compute_fog_color( const image_type_ptr& im, float& fog_r, float& fog_g, float& fog_b )
	{
		fog_r = fog_g = fog_b = 0.0f;
	
		image_type::const_pointer src_im = im->data( );
		for( int i = 0; i < im->height( ); ++i )
		{
			for( int j = 0; j < im->width( ); ++j )
			{
				for( int k = 0; k < 3; ++k )
				{
					half h;
			
					h.setBits( ( ( unsigned short* ) src_im )[ 0 ] );
					if( h.isFinite( ) )
						fog_r += h;
			
					src_im += sizeof( unsigned short );
				}
			}
		}
	
		fog_r /= im->width( ) * im->height( );
		fog_g /= im->width( ) * im->height( );
		fog_b /= im->width( ) * im->height( );
	}

	image_type_ptr convert_to_half( const image_type_ptr& im )
	{
		if( im->pf( ) == L"r16g16b16f" )
			return im;
	
		size_type width  = im->width( );
		size_type height = im->height( );
	
		image_type_ptr new_im = il::allocate( L"r16g16b16f", im->width( ), im->height( ) );

		image_type::const_pointer src = im->data( );
		image_type::pointer		  dst = new_im->data( );
		
		int src_pitch = ( im->pitch( ) - im->linesize( ) ) * sizeof( float );
		int dst_pitch = ( new_im->pitch( ) - new_im->linesize( ) ) * sizeof( unsigned short );
		
		for( int i = 0; i < height; ++i )
		{
			for( int j = 0; j < width; ++j )
			{
				( ( unsigned short* ) dst )[ 0 ] = Imf::floatToHalf( ( ( const float* ) src )[ 0 ] ).bits( );
				( ( unsigned short* ) dst )[ 1 ] = Imf::floatToHalf( ( ( const float* ) src )[ 1 ] ).bits( );
				( ( unsigned short* ) dst )[ 2 ] = Imf::floatToHalf( ( ( const float* ) src )[ 2 ] ).bits( );

				src += 3 * sizeof( float );
				dst += 3 * sizeof( unsigned short );
			}
			
			src += src_pitch;
			dst += dst_pitch;
		}
	
		return new_im;
	}

	image_type_ptr tm_ilm_exr( const image_type_ptr& im, float exposure, float defog, float kneeLow, float kneeHigh )
	{
		image_type_ptr src_img = convert( im, L"r32g32b32f" );
		image_type_ptr dst_img = src_img;
		
		if( src_img->pf( ) == L"r16g16b16f" || src_img->pf( ) == L"r32g32b32f" )
		{
			image_type_ptr half_im = convert_to_half( src_img );

			float fog_r, fog_g, fog_b;
			compute_fog_color( half_im, fog_r, fog_g, fog_b );

			halfFunction<float> r_gamma( gamma( exposure, defog * fog_r, kneeLow, kneeHigh ), -HALF_MAX, HALF_MAX );
			halfFunction<float> g_gamma( gamma( exposure, defog * fog_g, kneeLow, kneeHigh ), -HALF_MAX, HALF_MAX );
			halfFunction<float> b_gamma( gamma( exposure, defog * fog_b, kneeLow, kneeHigh ), -HALF_MAX, HALF_MAX );

			size_type width  = src_img->width( );
			size_type height = src_img->height( );

			dst_img = il::allocate( L"b8g8r8a8", width, height );

			image_type::const_pointer src = half_im->data( );
			image_type::pointer		  dst = dst_img->data( );

			size_type src_pitch = ( half_im->pitch( ) - half_im->linesize( ) ) * sizeof( unsigned short );
			size_type dst_pitch = dst_img->pitch( ) - dst_img->linesize( );

			for( int i = 0; i < height; ++i )
			{
				for( int j = 0; j < width; ++j )
				{
					half h0, h1, h2;

					h0.setBits( ( ( unsigned short* ) src )[ 0 ] ); src += sizeof( unsigned short );
					h1.setBits( ( ( unsigned short* ) src )[ 0 ] ); src += sizeof( unsigned short );
					h2.setBits( ( ( unsigned short* ) src )[ 0 ] ); src += sizeof( unsigned short );

					*dst++ = dither( r_gamma( h2 ), j, i );
					*dst++ = dither( g_gamma( h1 ), j, i );
					*dst++ = dither( b_gamma( h0 ), j, i );
					*dst++ = 255;
				}

				src += src_pitch;
				dst += dst_pitch;
			}
		}

		return dst_img;
	}
#endif

	// Ferwerda et al TVI functions.
	float tp( float L )
	{
		if( L <= -2.6f )
			return -0.72f;
		else if( L >= 1.9f )
			return L - 1.255f;
		else
			return powf( 0.249f * L + 0.65f, 2.7f ) - 0.72f;
	}
	
	float ts( float L )
	{
		if( L <= -3.94f )
			return -2.86f;
		else if( L >= -1.44f )
			return L - 0.395f;
		else
			return powf( 0.405f * L + 1.6f, 2.18f ) - 2.86f;
	}
	
	image_type_ptr tm_ferwerda( image_type_ptr im, float factor, float Ldmax )
	{
		image_type_ptr src_img = convert( im, L"r32g32b32f" );
		image_type_ptr dst_img = il::allocate( L"r32g32b32f", im->width( ), im->height( ) );

		size_type width	 = src_img->width( );
		size_type height = src_img->height( );

		const float* src = ( const float* ) src_img->data( );
		size_type src_pitch = src_img->pitch( );

		float* dst = ( float* ) dst_img->data( );
		size_type dst_pitch = dst_img->pitch( );

		const float* sptr = src;
		float* dptr = dst;

		// apply linear scaling factor.
		for( int i = 0; i < height; ++i )
		{
			for( int j = 0; j < width; ++j )
			{
				*dst++ = *src++ * factor;
				*dst++ = *src++ * factor;
				*dst++ = *src++ * factor;
			}

			dst = dptr += dst_pitch;
			src = sptr += src_pitch;
		}
			
		// reset the pointer.
		dst = ( float* ) dst_img->data( );
		dptr = dst;

		float Lmax = 0.0f;

		// calculate maximum luminance value.
		for( int i = 0; i < height; ++i )
		{
			for( int j = 0; j < width; ++j )
			{
				float r = *dst++;
				float g = *dst++;
				float b = *dst++;
			
				float lum = r * 0.2125f + g * 0.7154f + b * 0.0721f;
				
				if( lum > Lmax )
					Lmax = lum;
			}

			dst = dptr += dst_pitch;
		}
			
		float Lwa = Lmax * 0.5f;
		float log_Lwa = log10f( Lwa );
		float log_Lda = log10f( Ldmax * 0.5f );
			
		// calculate photopic and scotopic factors.
		float mp = powf( 10.0f, tp( log_Lda ) - tp( log_Lwa ) );
		float ms = powf( 10.0f, ts( log_Lda ) - ts( log_Lwa ) );
		float df = Lmax / Ldmax;

		// reset the dst pointer.
		dst = ( float* ) dst_img->data( );
		dptr = dst;

		// tonemap
		for( int i = 0; i < height; ++i )
		{
			for( int j = 0; j < width; ++j )
			{
				float r = *( dst + 0 );
				float g = *( dst + 1 );
				float b = *( dst + 2 );

				float lum = r * 0.2125f + g * 0.7154f + b * 0.0721f;
				float sf  = ms * lum;

				float dr = df * (  mp * r + sf );
				float dg = df * (  mp * g + sf );
				float db = df * (  mp * b + sf );

				*dst++ = dr > Ldmax ? Ldmax : dr;
				*dst++ = dg > Ldmax ? Ldmax : dg;
				*dst++ = db > Ldmax ? Ldmax : db;
			}

			dst = dptr += dst_pitch;
		}

		return dst_img;
	}
}

class ML_PLUGIN_DECLSPEC tm_linear_filter : public filter_type
{
public:
	explicit tm_linear_filter( )
		: prop_scale_factor_( pcos::key::from_string( "scale_factor" ) )
		, prop_gamma_( pcos::key::from_string( "gamma" ) )
		, prop_minval_( pcos::key::from_string( "minval" ) )
		, prop_maxval_( pcos::key::from_string( "maxval" ) )
	{
		properties( ).append( prop_scale_factor_ = 100.0f );
		properties( ).append( prop_gamma_		 = 2.0f );
		properties( ).append( prop_minval_		 = 0.0f );
		properties( ).append( prop_maxval_		 = ( std::numeric_limits<float>::max )( ) );
	}

	virtual const opl::wstring get_uri( ) const { return L"tm_linear"; }

	virtual frame_type_ptr fetch( )
	{
		acquire_values( );

		frame_type_ptr result = frame_type::deep_copy( fetch_from_slot( ) );

		if( result && result->get_image( ) )
			result->set_image( il::tm_linear( result->get_image( ), prop_scale_factor_.value<float>( ), prop_gamma_.value<float>( ),
																	prop_minval_.value<float>( ), prop_maxval_.value<float>( ) ) );
		return result;
	}
	
private:
	pcos::property prop_scale_factor_;
	pcos::property prop_gamma_;
	pcos::property prop_minval_;
	pcos::property prop_maxval_;
};

#ifdef HAVE_OPENEXR
class ML_PLUGIN_DECLSPEC tm_ilm_exr_filter : public filter_type
{
public:
	explicit tm_ilm_exr_filter( )
		: prop_exposure_( pcos::key::from_string( "exposure" ) )
		, prop_defog_( pcos::key::from_string( "defog" ) )
		, prop_knee_low_( pcos::key::from_string( "knee_low" ) )
		, prop_knee_high_( pcos::key::from_string( "knee_high" ) )
	{
		properties( ).append( prop_exposure_ = 0.0f );
		properties( ).append( prop_defog_ = 0.0f );
		properties( ).append( prop_knee_low_ = 0.0f );
		properties( ).append( prop_knee_high_ = 5.0f );
	}

	virtual const opl::wstring get_uri( ) const { return L"tm_ilm_exr"; }

	virtual frame_type_ptr fetch( )
	{
		acquire_values( );

		frame_type_ptr result = frame_type::deep_copy( fetch_from_slot( ) );

		if ( result && result->get_image( ) )
			result->set_image( tm_ilm_exr( result->get_image( ), prop_exposure_.value<float>( ), prop_defog_.value<float>( ), 
																 prop_knee_low_.value<float>( ), prop_knee_high_.value<float>( ) ) );

		return result;
	}

private:
	pcos::property prop_exposure_;
	pcos::property prop_defog_;
	pcos::property prop_knee_low_;
	pcos::property prop_knee_high_;
};
#endif

class ML_PLUGIN_DECLSPEC tm_ferwerda_filter : public filter_type
{
public:
	explicit tm_ferwerda_filter( )
		: prop_scale_factor_( pcos::key::from_string( "scale_factor" ) )
		, prop_gamma_( pcos::key::from_string( "gamma" ) )
		, prop_ldmax_( pcos::key::from_string( "Ldmax" ) )
	{
		properties( ).append( prop_scale_factor_ = 1.0f );
		properties( ).append( prop_gamma_		 = 2.0f );
		properties( ).append( prop_ldmax_		 = 100.0f );
	}
	
	virtual const opl::wstring get_uri( ) const { return L"tm_ferwerda"; }
	
	virtual frame_type_ptr fetch( )
	{
		acquire_values( );

		frame_type_ptr result = frame_type::deep_copy( fetch_from_slot( ) );
		if( result && result->get_image( ) )
		{
			il::image_type_ptr im = tm_ferwerda( result->get_image( ), prop_scale_factor_.value<float>( ), prop_ldmax_.value<float>( ) );

			im = il::normalise( im, 1.0f );
			im = il::gamma( im, prop_gamma_.value<float>( ) );
			im = il::normalise( im, 255.0f );
			im = il::convert( im, L"ldr8" );

			result->set_image( im );
		}

		return result;
	}
	
private:
	pcos::property prop_scale_factor_;
	pcos::property prop_gamma_;
	pcos::property prop_ldmax_;
};

class ML_PLUGIN_DECLSPEC tonemap_plugin : public openmedialib_plugin
{
public:
	virtual filter_type_ptr filter( const pl::wstring &request )
	{
		if( request == L"tm_linear" )
			return filter_type_ptr( new tm_linear_filter( ) );
#	ifdef HAVE_OPENEXR
		else if( request == L"tm_ilm_exr" )
			return filter_type_ptr( new tm_ilm_exr_filter( ) );
#	endif
		else if( request == L"tm_ferwerda" )
			return filter_type_ptr( new tm_ferwerda_filter( ) );
		else
			return filter_type_ptr( );
	}
};

} } }

extern "C"
{
	ML_PLUGIN_DECLSPEC bool openplugin_init( void )
	{
		return true;
	}

	ML_PLUGIN_DECLSPEC bool openplugin_uninit( void )
	{
		return true;
	}
	
	ML_PLUGIN_DECLSPEC bool openplugin_create_plugin( const char*, pl::openplugin** plug )
	{
		*plug = new plugin::tonemap_plugin;
		return true;
	}
	
	ML_PLUGIN_DECLSPEC void openplugin_destroy_plugin( pl::openplugin* plug )
	{ 
		delete static_cast<plugin::tonemap_plugin*>( plug ); 
	}
}
