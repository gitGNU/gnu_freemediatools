/* -*- mode: C++; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t -*- */
// avformat - A avformat plugin to ml.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#include <openmedialib/ml/openmedialib_plugin.hpp>
#include <openpluginlib/pl/utf8_utils.hpp>
#include <openpluginlib/pl/pcos/isubject.hpp>
#include <openpluginlib/pl/pcos/observer.hpp>

#ifdef WIN32
#include <windows.h>
#endif // WIN32

#ifndef INT64_C
#define INT64_C(x)	x ## LL
#endif // INT64_C

#include <iostream>
#include <cstdlib>
#include <vector>
#include <deque>
#include <map>
#include <string>
#include <cmath>
#include <cstdio>

#include <boost/thread/thread.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/recursive_mutex.hpp>

#ifdef USE_FFMPEG
/*
  i am getting issues with compilng here
./openlibraries/trunk/src/openmedialib/plugins/avformat/avformat_plugin.cpp:1504: error: 'SWS_BICUBIC' was not declared in this scope

just comment this out for now...
*/
//error: avformat.h FFmpeg libavformat-dev
//error: swscale.h: swscale.h

extern "C" {
#include <avformat.h>
#include <swscale.h>
}

namespace oml = olib::openmedialib;
namespace opl = olib::openpluginlib;
namespace plugin = olib::openmedialib::ml;
namespace il = olib::openimagelib::il;
namespace pcos = olib::openpluginlib::pcos;

namespace olib { namespace openmedialib { namespace ml { 

// Alternative to Julian's patch?
static const AVRational ml_av_time_base_q = { 1, AV_TIME_BASE };

// libavformat codec codec context
AVCodecContext *avctx_opts[ CODEC_TYPE_NB ];
AVFormatContext *avformat_opts;

static const std::wstring avformat_to_oil( int fmt )
{
	if ( fmt == PIX_FMT_YUV420P )
		return L"yuv420p";
	else if ( fmt == PIX_FMT_YUVJ420P )
		return L"yuv420p";
	else if ( fmt == PIX_FMT_UYVY411 )
		return L"yuv411";
	else if ( fmt == PIX_FMT_YUV411P )
		return L"yuv411p";
	else if ( fmt == PIX_FMT_YUV422 )
		return L"yuv422";
	else if ( fmt == PIX_FMT_YUV422P )
		return L"yuv422p";
	else if ( fmt == PIX_FMT_YUVJ422P )
		return L"yuv422p";
	else if ( fmt == PIX_FMT_RGB24 )
		return L"r8g8b8";
	else if ( fmt == PIX_FMT_BGR24 )
		return L"b8g8r8";
	else if ( fmt == PIX_FMT_RGBA32 )
		return L"r8g8b8a8";
	return L"";
}

#if 0
static const int oil_to_avformat( const std::wstring &fmt )
{
	if ( fmt == L"yuv420p" )
		return PIX_FMT_YUV420P;
	else if ( fmt == L"yuv411" )
		return PIX_FMT_UYVY411;
	else if ( fmt == L"yuv411p" )
		return PIX_FMT_YUV411P;
	else if ( fmt == L"yuv422" )
		return PIX_FMT_YUV422;
	else if ( fmt == L"yuv422p" )
		return PIX_FMT_YUV422P;
	else if ( fmt == L"r8g8b8" )
		return PIX_FMT_RGB24;
	else if ( fmt == L"b8g8r8" )
		return PIX_FMT_BGR24;
	else if ( fmt == L"r8g8b8a8" )
		return PIX_FMT_RGBA32;
	return -1;
}
#endif

class ML_PLUGIN_DECLSPEC avformat_store : public store_type
{
	public:
		avformat_store( const opl::wstring &resource, const frame_type_ptr &frame )
			: store_type( )
			, resource_( resource )
			, prop_enable_audio_( pcos::key::from_string( "enable_audio" ) )
			, prop_enable_video_( pcos::key::from_string( "enable_video" ) )
			, oc_( NULL )
			, fmt_( NULL )
			, audio_stream_( NULL )
			, video_stream_( NULL )
			, audio_input_frame_size_( 0 )
			, prop_show_stats_( pcos::key::from_string( "show_stats" ) )
			, prop_format_( pcos::key::from_string( "format" ) )
			, prop_vcodec_( pcos::key::from_string( "vcodec" ) )
			, prop_acodec_( pcos::key::from_string( "acodec" ) )
			, prop_pix_fmt_( pcos::key::from_string( "pix_fmt" ) )
			, prop_vfourcc_( pcos::key::from_string( "vfourcc" ) )
			, prop_afourcc_( pcos::key::from_string( "afourcc" ) )
			, prop_width_( pcos::key::from_string( "width" ) )
			, prop_height_( pcos::key::from_string( "height" ) )
			, prop_aspect_ratio_( pcos::key::from_string( "aspect_ratio" ) )
			, prop_fps_num_( pcos::key::from_string( "fps_num" ) )
			, prop_fps_den_( pcos::key::from_string( "fps_den" ) )
			, prop_frequency_( pcos::key::from_string( "frequency" ) )
			, prop_channels_( pcos::key::from_string( "channels" ) )
			, prop_audio_bit_rate_( pcos::key::from_string( "audio_bit_rate" ) )
			, prop_video_bit_rate_( pcos::key::from_string( "video_bit_rate" ) )
			, prop_video_bit_rate_tolerance_( pcos::key::from_string( "video_bit_rate_tolerance" ) )
			, prop_gop_size_( pcos::key::from_string( "gop_size" ) )
			, prop_b_frames_( pcos::key::from_string( "b_frames" ) )
			, prop_mb_decision_( pcos::key::from_string( "mb_decision" ) )
			, prop_qscale_( pcos::key::from_string( "qscale" ) )
			, prop_me_method_( pcos::key::from_string( "me_method" ) )
			, prop_mb_cmp_( pcos::key::from_string( "mb_cmp" ) )
			, prop_ildct_cmp_( pcos::key::from_string( "ildct_cmp" ) )
			, prop_sub_cmp_( pcos::key::from_string( "sub_cmp" ) )
			, prop_cmp_( pcos::key::from_string( "cmp" ) )
			, prop_pre_cmp_( pcos::key::from_string( "pre_cmp" ) )
			, prop_pre_me_( pcos::key::from_string( "pre_me" ) )
			, prop_lumi_mask_( pcos::key::from_string( "lumi_mask" ) )
			, prop_dark_mask_( pcos::key::from_string( "dark_mask" ) )
			, prop_scplx_mask_( pcos::key::from_string( "scplx_mask" ) )
			, prop_tcplx_mask_( pcos::key::from_string( "tcplx_mask" ) )
			, prop_p_mask_( pcos::key::from_string( "p_mask" ) )
			, prop_qns_( pcos::key::from_string( "qns" ) )
			, prop_video_qmin_( pcos::key::from_string( "video_qmin" ) )
			, prop_video_qmax_( pcos::key::from_string( "video_qmax" ) )
			, prop_video_lmin_( pcos::key::from_string( "video_lmin" ) )
			, prop_video_lmax_( pcos::key::from_string( "video_lmax" ) )
			, prop_video_mb_qmin_( pcos::key::from_string( "video_mb_qmin" ) )
			, prop_video_mb_qmax_( pcos::key::from_string( "video_mb_qmax" ) )
			, prop_video_qdiff_( pcos::key::from_string( "video_qdiff" ) )
			, prop_video_qblur_( pcos::key::from_string( "video_qblur" ) )
			, prop_video_qcomp_( pcos::key::from_string( "video_qcomp" ) )
			, prop_mux_rate_( pcos::key::from_string( "mux_rate" ) )
			, prop_mux_preload_( pcos::key::from_string( "mux_preload" ) )
			, prop_mux_delay_( pcos::key::from_string( "mux_delay" ) )
			, prop_video_packet_size_( pcos::key::from_string( "video_packet_size" ) )
			, prop_video_rc_max_rate_( pcos::key::from_string( "video_rc_max_rate" ) )
			, prop_video_rc_min_rate_( pcos::key::from_string( "video_rc_min_rate" ) )
			, prop_video_rc_buffer_size_( pcos::key::from_string( "video_rc_buffer_size" ) )
			, prop_video_rc_buffer_aggressivity_( pcos::key::from_string( "video_rc_buffer_aggresivity" ) )
			, prop_video_rc_initial_cplx_( pcos::key::from_string( "video_rc_initial_cplx" ) )
			, prop_video_i_qfactor_( pcos::key::from_string( "video_i_qfactor" ) )
			, prop_video_b_qfactor_( pcos::key::from_string( "video_b_qfactor" ) )
			, prop_video_i_qoffset_( pcos::key::from_string( "video_i_qoffset" ) )
			, prop_video_b_qoffset_( pcos::key::from_string( "video_b_qoffset" ) )
			, prop_video_intra_quant_bias_( pcos::key::from_string( "video_intra_quant_bias" ) )
			, prop_video_inter_quant_bias_( pcos::key::from_string( "video_inter_quant_bias" ) )
			, prop_dct_algo_( pcos::key::from_string( "dct_algo" ) )
			, prop_idct_algo_( pcos::key::from_string( "idct_algo" ) )
			, prop_me_threshold_( pcos::key::from_string( "me_threshold" ) )
			, prop_mb_threshold_( pcos::key::from_string( "mb_threshold" ) )
			, prop_intra_dc_precision_( pcos::key::from_string( "intra_dc_precision" ) )
			, prop_strict_( pcos::key::from_string( "strict" ) )
			, prop_error_rate_( pcos::key::from_string( "error_rate" ) )
			, prop_noise_reduction_( pcos::key::from_string( "noise_reduction" ) )
			, prop_sc_threshold_( pcos::key::from_string( "sc_threshold" ) )
			, prop_me_range_( pcos::key::from_string( "me_range" ) )
			, prop_coder_( pcos::key::from_string( "coder" ) )
			, prop_context_( pcos::key::from_string( "context" ) )
			, prop_predictor_( pcos::key::from_string( "predictor" ) )
			, prop_pass_( pcos::key::from_string( "pass" ) )
			, prop_pass_log_( pcos::key::from_string( "pass_log" ) )
			, log_file_( 0 )
			, log_( 0 )
			, frame_pos_( 0 )
		{
			// Show stats
			properties( ).append( prop_show_stats_ = 100 );

			// Define the audio output buffer
			audio_outbuf_size_ = 10000;
			audio_outbuf_ = ( uint8_t * )av_malloc( audio_outbuf_size_ );

			// Extract rules from the frame
			// TODO: Ensure this doesn't cause a foul up...
			properties( ).append( prop_enable_audio_ = frame->get_audio( ) != 0 ? 1 : 0 );
			properties( ).append( prop_enable_video_ = frame->get_image( ) != 0 ? 1 : 0 );

			// Extract video related info from frame
			properties( ).append( prop_width_ = 0 );
			properties( ).append( prop_height_ = 0 );
			properties( ).append( prop_aspect_ratio_ = 1.0 );
			properties( ).append( prop_fps_num_ = 25 );
			properties( ).append( prop_fps_den_ = 1 );

			if ( prop_enable_video_.value< int >( ) == 1 )
			{
				prop_width_ = frame->get_image( )->width( );
				prop_height_ = frame->get_image( )->height( );
				prop_aspect_ratio_ = frame->aspect_ratio( );
				int num;
				int den;
				frame->get_fps( num, den );
				prop_fps_num_ = num;
				prop_fps_den_ = den;
			}

			// Allocate the video output buffer 
			// TODO: Don't allocate when video is disabled
			video_outbuf_size_ = 256 * 1024 < prop_width_.value< int >( ) * prop_height_.value< int >( ) * 4 ? 
								 prop_width_.value< int >( ) * prop_height_.value< int >( ) * 4 : 256 * 1024;
			video_outbuf_ = ( uint8_t * )av_malloc( video_outbuf_size_ );

			// Extract audio related info from frame
			properties( ).append( prop_frequency_ = 0 );
			properties( ).append( prop_channels_ = 0 );

			if ( prop_enable_audio_.value< int >( ) == 1 )
			{
				prop_frequency_ = frame->get_audio( )->frequency( );
				prop_channels_ = frame->get_audio( )->channels( );
			}

			// All of these need to migrate to application accessible properties
			properties( ).append( prop_format_ = opl::wstring( L"" ) );
			properties( ).append( prop_acodec_ = opl::wstring( L"" ) );
			properties( ).append( prop_vcodec_ = opl::wstring( L"" ) );
			properties( ).append( prop_vfourcc_ = opl::wstring( L"" ) );
			properties( ).append( prop_afourcc_ = opl::wstring( L"" ) );
			properties( ).append( prop_pix_fmt_ = opl::wstring( L"" ) );

			properties( ).append( prop_audio_bit_rate_ = 128000 );
			properties( ).append( prop_video_bit_rate_ = 400000 );
			properties( ).append( prop_video_bit_rate_tolerance_ = 4000 * 1000 );
			properties( ).append( prop_gop_size_ = 12 );
			properties( ).append( prop_b_frames_ = 0 );
			properties( ).append( prop_mb_decision_ = FF_MB_DECISION_SIMPLE );
			properties( ).append( prop_qscale_ = 0.0 );
			properties( ).append( prop_me_method_ = int( ME_EPZS ) );
			properties( ).append( prop_mb_cmp_ = FF_CMP_SAD );
			properties( ).append( prop_ildct_cmp_ = FF_CMP_VSAD );
			properties( ).append( prop_sub_cmp_ = FF_CMP_SAD );
			properties( ).append( prop_cmp_ = FF_CMP_SAD );
			properties( ).append( prop_pre_cmp_ = FF_CMP_SAD );
			properties( ).append( prop_pre_me_ = 0 );
			properties( ).append( prop_lumi_mask_ = 0.0 );
			properties( ).append( prop_dark_mask_ = 0.0 );
			properties( ).append( prop_scplx_mask_ = 0.0 );
			properties( ).append( prop_tcplx_mask_ = 0.0 );
			properties( ).append( prop_p_mask_ = 0.0 );
			properties( ).append( prop_qns_ = 0 );
			properties( ).append( prop_video_qmin_ = 2 );
			properties( ).append( prop_video_qmax_ = 31 );
			properties( ).append( prop_video_lmin_ = 2 * FF_QP2LAMBDA );
			properties( ).append( prop_video_lmax_ = 31 * FF_QP2LAMBDA );
			properties( ).append( prop_video_mb_qmin_ = 2 );
			properties( ).append( prop_video_mb_qmax_ = 31 );
			properties( ).append( prop_video_qdiff_ = 3 );
			properties( ).append( prop_video_qblur_ = 0.5 );
			properties( ).append( prop_video_qcomp_ = 0.5 );
			properties( ).append( prop_mux_rate_ = 0 );
			properties( ).append( prop_mux_preload_ = 0.5 );
			properties( ).append( prop_mux_delay_ = 0.7 );
			properties( ).append( prop_video_packet_size_ = 0 );
			properties( ).append( prop_video_rc_max_rate_ = 0 );
			properties( ).append( prop_video_rc_min_rate_ = 0 );
			properties( ).append( prop_video_rc_buffer_size_ = 0 );
			properties( ).append( prop_video_rc_buffer_aggressivity_ = 1.0 );
			properties( ).append( prop_video_rc_initial_cplx_ = 0.0 );
			properties( ).append( prop_video_i_qfactor_ = -0.8 );
			properties( ).append( prop_video_b_qfactor_ = 1.25 );
			properties( ).append( prop_video_i_qoffset_ = 0.0 );
			properties( ).append( prop_video_b_qoffset_ = 1.25 );
			properties( ).append( prop_video_intra_quant_bias_ = FF_DEFAULT_QUANT_BIAS );
			properties( ).append( prop_video_inter_quant_bias_ = FF_DEFAULT_QUANT_BIAS );
			properties( ).append( prop_dct_algo_ = 0 );
			properties( ).append( prop_idct_algo_ = 0 );
			properties( ).append( prop_me_threshold_ = 0 );
			properties( ).append( prop_mb_threshold_ = 0 );
			properties( ).append( prop_intra_dc_precision_ = 8 );
			properties( ).append( prop_strict_ = 0 );
			properties( ).append( prop_error_rate_ = 0 );
			properties( ).append( prop_noise_reduction_ = 0 );
			properties( ).append( prop_sc_threshold_ = 0 );
			properties( ).append( prop_me_range_ = 1023 );
			properties( ).append( prop_coder_ = 0 );
			properties( ).append( prop_context_ = 0 );
			properties( ).append( prop_predictor_ = 0 );
			properties( ).append( prop_pass_ = 0 );
			properties( ).append( prop_pass_log_ = opl::wstring( L"oml_avformat.log" ) );
		}

		virtual ~avformat_store( )
		{
			if ( oc_ )
			{
				// Write the trailer, if any
				av_write_trailer( oc_ );

				// Free the streams
				for( size_t i = 0; i < oc_->nb_streams; i++ )
					av_freep( &oc_->streams[ i ] );

				// Close the output file
				if ( !( fmt_->flags & AVFMT_NOFILE ) )
					url_fclose( &oc_->pb );

				// Free the context
				av_free( oc_ );
			}

			// Clean up the allocated image
			if ( video_stream_ )
				av_free( av_image_.data[0] );

			// Clean up audio buffer
			av_free( audio_outbuf_ );

			// Clean up video buffer
			av_free( video_outbuf_ );

			// Clean up the log
			if ( log_file_ )
				fclose( log_file_ );
			av_free( log_ );

			// Hopefully leave the last stats line visible
			if ( prop_show_stats_.value< int >( ) )
				fprintf( stderr, "\n" );
		}

		// Initialise method
		virtual bool init( )
		{
			bool ret = false;

			// Allocate the output context
			oc_ = av_alloc_format_context( );

			// Derive the output format
			fmt_ = alloc_output_format( );
			ret = fmt_ != 0;

			// Handle the 2 pass settings
			if ( ret )
			{
				if ( prop_pass_.value< int >( ) == 1 )
				{
					log_file_ = fopen( opl::to_string( prop_pass_log_.value< opl::wstring >( ) ).c_str( ), "w" );
					ret = log_file_ != 0;
					if ( !ret )
						fprintf( stderr, "Unable to open log file for write %s\n", opl::to_string( prop_pass_log_.value< opl::wstring >( ) ).c_str( ) );
				}
				else if ( prop_pass_.value< int >( ) == 2 )
				{
					FILE *f = fopen( opl::to_string( prop_pass_log_.value< opl::wstring >( ) ).c_str( ), "r" );
					ret = f != 0;
					if ( ret )
					{
                    	fseek( f, 0, SEEK_END );
                    	size_t size = ftell( f );
                    	log_ = ( char * )av_malloc( size + 1 );
						ret = log_ != 0;
						if ( ret )
						{
                    		fseek( f, 0, SEEK_SET );
                    		size = fread( log_, 1, size, f );
                    		log_[ size ] = '\0';
						}
						else
						{
							fprintf( stderr, "Unable to allocate log for %s\n", opl::to_string( prop_pass_log_.value< opl::wstring >( ) ).c_str( ) );
						}
                    	fclose( f );
					}
					else
					{
						fprintf( stderr, "Unable to open log file for read %s\n", opl::to_string( prop_pass_log_.value< opl::wstring >( ) ).c_str( ) );
					}
				}
			}

			// Set up the remainder of the encoder properties
			if ( ret )
			{
				// Assign the output format
				oc_->oformat = fmt_;

				// Open video and audio codec streams
				if ( prop_enable_video_.value< int >( ) == 1 && fmt_->video_codec != CODEC_ID_NONE )
					video_stream_ = add_video_stream( obtain_video_codec_id( ) );

				// we don't want to check for the audio_codec being non-null here, as it is
				// possible for the guessing routing to find a codec pairing with no 
				// audio codec e.g. flv usually has mp3, however if ffmpeg is built without lame
				// fmt_->audio_codec would be CODEC_ID_NONE. Hence we need to allow add_audio_stream
				// to be called to allow the acodec property to be checked and potentially used.
				if ( prop_enable_audio_.value< int >( ) == 1 /* && fmt_->audio_codec != CODEC_ID_NONE */ )
					audio_stream_ = add_audio_stream( obtain_audio_codec_id( ) );

				// Set up the parameters and open all codecs
				if ( av_set_parameters( oc_, NULL ) >= 0 )
				{
					// Attempt to open video and audio streams
					if ( open_video_stream( ) && open_audio_stream( ) )
					{
						// We'll allow encoding to stdout
						if ( !( fmt_->flags & AVFMT_NOFILE ) ) 
							ret = url_fopen( &oc_->pb, opl::to_string( resource_ ).c_str( ), URL_WRONLY ) >= 0;
						else
							ret = true;
					}

					// Write the header
					if ( ret )
						av_write_header( oc_ );
				}
			}

			return ret;
		}

		// Push a frame to the store
		virtual bool push( frame_type_ptr frame )
		{
			double audio_pts = 0.0;
			double video_pts = 0.0;

			queue( frame );

			while ( 1 )
			{
				if ( audio_stream_ != NULL )
					audio_pts = double( audio_stream_->pts.val ) * audio_stream_->time_base.num / audio_stream_->time_base.den;
				else
					audio_pts = 0.0;
        
				if ( video_stream_ )
					video_pts = double( video_stream_->pts.val ) * video_stream_->time_base.num / video_stream_->time_base.den;
				else
					video_pts = 0.0;

 				// Write interleaved audio and video frames
 				if ( !video_stream_ || ( video_stream_ && audio_stream_ && audio_pts < video_pts ) )
				{
					if ( !audio_queue_.size( ) )
						break;
					if ( !process_audio( ) )
						return false;
				}
 				else if ( video_stream_ )
				{
					if ( !video_queue_.size( ) )
						break;
					if ( !process_video( ) )
						return false;
				}
				else
				{
					return false;
				}
			}

			if ( prop_show_stats_.value< int >( ) && frame_pos_ ++ % prop_show_stats_.value< int >( ) == 0 )
				fprintf( stderr, "%06d: audio %10.4f video %10.4f\r", frame_pos_ - 1, audio_pts, video_pts );

			return true;
		}

		// Tell the store to flush all pending frames, while returning the
		// one that was pending. The default implementation applies when 
		// the store doesn't queue frames.
		virtual frame_type_ptr flush( )
		{ 
			if ( prop_show_stats_.value< int >( ) )
			{
				double audio_pts = audio_stream_ != 0 ? double( audio_stream_->pts.val ) * audio_stream_->time_base.num / audio_stream_->time_base.den : 0.0;
				double video_pts = video_stream_ != 0 ? double( video_stream_->pts.val ) * video_stream_->time_base.num / video_stream_->time_base.den : 0.0;
				fprintf( stderr, "%06d: audio %10.4f video %10.4f\r", frame_pos_ - 1, audio_pts, video_pts );
			}
			return frame_type_ptr( ); 
		}

		// Playout all queued frames and return when done. The default
		// implementation applies when the store doesn't queue frames.
		virtual void complete( )
		{ 
			if ( prop_show_stats_.value< int >( ) )
			{
				double audio_pts = audio_stream_ != 0 ? double( audio_stream_->pts.val ) * audio_stream_->time_base.num / audio_stream_->time_base.den : 0.0;
				double video_pts = video_stream_ != 0 ? double( video_stream_->pts.val ) * video_stream_->time_base.num / video_stream_->time_base.den : 0.0;
				fprintf( stderr, "%06d: audio %10.4f video %10.4f\r", frame_pos_ - 1, audio_pts, video_pts );
			}
		}

		// Allows the store to dictate when it is running empty (ie: any
		// realtime store such as an audio player or device feed needs more
		// frames in order to provide smooth playout)
		virtual bool empty( )
		{ 
			return true; 
		}

	private:

		// Very rough mapping of file name and format property
		AVOutputFormat *alloc_output_format( )
		{
			AVOutputFormat *fmt = NULL;

			if ( prop_format_.value< opl::wstring >( ) != L"" )
				fmt = guess_format( opl::to_string( prop_format_.value< opl::wstring >( ) ).c_str( ), NULL, NULL );

			if ( fmt == NULL )
				fmt = guess_format( NULL, opl::to_string( resource_ ).c_str( ), NULL );

			if ( fmt == NULL )
				fmt = guess_format( "mpeg", NULL, NULL );

			return fmt;
		}

		// Obtain the requested video codec id
		CodecID obtain_video_codec_id( )
		{
			CodecID codec_id = fmt_->video_codec;
			if ( prop_vcodec_.value< opl::wstring >( ) != L"" )
			{
				AVCodec *codec = avcodec_find_encoder_by_name( opl::to_string( prop_vcodec_.value< opl::wstring >( ) ).c_str( ) );
				if ( codec != NULL )
					codec_id = codec->id;
			}
			return codec_id;
		}

		// Obtain the requested audio codec id
		CodecID obtain_audio_codec_id( )
		{
			CodecID codec_id = fmt_->audio_codec;
			if ( prop_acodec_.value< opl::wstring >( ) != L"" )
			{
				AVCodec *codec = avcodec_find_encoder_by_name( opl::to_string( prop_acodec_.value< opl::wstring >( ) ).c_str( ) );
				if ( codec != NULL )
					codec_id = codec->id;
				else
					std::wcerr << L"obtain_audio_codec_id: failed to find codec for value: "
							   << prop_acodec_.value< opl::wstring >( ) 
							   << L"\n";
			}

// 			std::wcerr << L"obtain_audio_codec_id: found: " << codec_id << " for "
// 					   << prop_acodec_.value< opl::wstring >( ) << L"\n";
			return codec_id;
		}


		// Generate a video stream
		AVStream *add_video_stream( CodecID codec_id )
		{
			// Create a new stream
			AVStream *st = av_new_stream( oc_, 0 );

			if ( st != NULL ) 
			{
				AVCodecContext *c = st->codec;
				c->codec_id = codec_id;
				c->codec_type = CODEC_TYPE_VIDEO;

				// Specify stream parameters
				c->bit_rate = prop_video_bit_rate_.value< int >( );
				c->bit_rate_tolerance = prop_video_bit_rate_tolerance_.value< int >( );
				c->width = prop_width_.value< int >( );
				c->height = prop_height_.value< int >( );
				c->time_base.den = prop_fps_num_.value< int >( );
				c->time_base.num = prop_fps_den_.value< int >( );
				c->gop_size = prop_gop_size_.value< int >( );
				opl::string pixfmt = opl::to_string( prop_pix_fmt_.value< opl::wstring >( ) );
				c->pix_fmt = pixfmt != "" ? avcodec_get_pix_fmt( pixfmt.c_str( ) ) : PIX_FMT_YUV420P;
		
				// Fix b frames
				if ( prop_b_frames_.value< int >( ) )
				{
	 				c->max_b_frames = prop_b_frames_.value< int >( );
					c->b_frame_strategy = 0;
					c->b_quant_factor = 2.0;
				}

				// Specifiy miscellaneous properties
	 			c->mb_decision = prop_mb_decision_.value< int >( );
				c->sample_aspect_ratio = av_d2q( prop_aspect_ratio_.value< double >( ) * c->height / c->width, 255 );
				c->mb_cmp = prop_mb_cmp_.value< int >( );
				c->ildct_cmp = prop_ildct_cmp_.value< int >( );
				c->me_sub_cmp = prop_sub_cmp_.value< int >( );
				c->me_cmp = prop_cmp_.value< int >( );
				c->me_pre_cmp = prop_pre_cmp_.value< int >( );
				c->pre_me = prop_pre_me_.value< int >( );
				c->lumi_masking = prop_lumi_mask_.value< double >( );
				c->dark_masking = prop_dark_mask_.value< double >( );
				c->spatial_cplx_masking = prop_scplx_mask_.value< double >( );
				c->temporal_cplx_masking = prop_tcplx_mask_.value< double >( );
				c->p_masking = prop_p_mask_.value< double >( );
				c->quantizer_noise_shaping= prop_qns_.value< int >( );
				c->qmin = prop_video_qmin_.value< int >( );
				c->qmax = prop_video_qmax_.value< int >( );
				c->lmin = prop_video_lmin_.value< int >( );
				c->lmax = prop_video_lmax_.value< int >( );
				c->mb_qmin = prop_video_mb_qmin_.value< int >( );
				c->mb_qmax = prop_video_mb_qmax_.value< int >( );
				c->max_qdiff = prop_video_qdiff_.value< int >( );
				c->qblur = prop_video_qblur_.value< double >( );
				c->qcompress = prop_video_qcomp_.value< double >( );

        		c->rc_eq = "tex^qComp";
        		c->rc_override_count = 0;
        		c->thread_count = 1;
		
				// Handle fixed quality override
				if ( prop_qscale_.value< double >( ) > 0.0 )
				{
					c->flags |= CODEC_FLAG_QSCALE;
					st->quality = float( FF_QP2LAMBDA * prop_qscale_.value< double >( ) );
				}
		
				// Allow the user to override the video fourcc
				opl::string vfourcc = opl::to_string( prop_vfourcc_.value< opl::wstring >( ) );
				if ( vfourcc != "" )
				{
					char *tail = NULL;
					const char *arg = vfourcc.c_str( );
    				int tag = strtol( arg, &tail, 0);
    				if( !tail || *tail )
        				tag = arg[ 0 ] + ( arg[ 1 ] << 8 ) + ( arg[ 2 ] << 16 ) + ( arg[ 3 ] << 24 );
					c->codec_tag = tag;
				}
		
				// Some formats want stream headers to be seperate
				if ( oc_->oformat->flags & AVFMT_GLOBALHEADER ) 
					c->flags |= CODEC_FLAG_GLOBAL_HEADER;
		
    			oc_->preload = int( prop_mux_preload_.value< double >( ) * AV_TIME_BASE );
    			oc_->max_delay = int( prop_mux_delay_.value< double >( ) * AV_TIME_BASE );
    			oc_->loop_output = AVFMT_NOOUTPUTLOOP;

				oc_->mux_rate = prop_mux_rate_.value< int >( );
				oc_->packet_size = prop_video_packet_size_.value< int >( );
				c->rc_max_rate = prop_video_rc_max_rate_.value< int >( );
				c->rc_min_rate = prop_video_rc_min_rate_.value< int >( );
				c->rc_buffer_size = prop_video_rc_buffer_size_.value< int >( );
				c->rc_buffer_aggressivity = prop_video_rc_buffer_aggressivity_.value< double >( );
				c->rc_initial_cplx= prop_video_rc_initial_cplx_.value< double >( );
				c->i_quant_factor = prop_video_i_qfactor_.value< double >( );
				c->b_quant_factor = prop_video_b_qfactor_.value< double >( );
				c->i_quant_offset = prop_video_i_qoffset_.value< double >( );
				c->b_quant_offset = prop_video_b_qoffset_.value< double >( );
				c->intra_quant_bias = prop_video_intra_quant_bias_.value< int >( );
				c->inter_quant_bias = prop_video_inter_quant_bias_.value< int >( );
				c->dct_algo = prop_dct_algo_.value< int >( );
				c->idct_algo = prop_idct_algo_.value< int >( );
				c->rc_initial_buffer_occupancy = c->rc_buffer_size*3/4;
				c->me_threshold = prop_me_threshold_.value< int >( );
				c->mb_threshold = prop_mb_threshold_.value< int >( );
				c->intra_dc_precision = prop_intra_dc_precision_.value< int >( ) - 8;
				c->strict_std_compliance = prop_strict_.value< int >( );
				c->error_rate = prop_error_rate_.value< int >( );
				c->noise_reduction = prop_noise_reduction_.value< int >( );
				c->scenechange_threshold = prop_sc_threshold_.value< int >( );
				c->me_range = prop_me_range_.value< int >( );
				c->coder_type = prop_coder_.value< int >( );
				c->context_model = prop_context_.value< int >( );
				c->prediction_method = prop_predictor_.value< int >( );
				c->me_method = prop_me_method_.value< int >( );
 			}
 
			return st;
		}

		AVStream *add_audio_stream( CodecID codec_id )
		{
			// Create a new stream
			AVStream *st = av_new_stream( oc_, 1 );

			if ( !st )
				std::cerr << "add_audio_stream: failed to create stream for: " << codec_id << "\n";

			// If created, then initialise from properties
			if ( st != NULL ) 
			{
				AVCodecContext *c = st->codec;
				c->codec_id = codec_id;
				c->codec_type = CODEC_TYPE_AUDIO;

				// Specify sample parameters
				c->bit_rate = prop_audio_bit_rate_.value< int >( );
				c->sample_rate = prop_frequency_.value< int >( );
				c->channels = prop_channels_.value< int >( );

    			if ( oc_->oformat->flags & AVFMT_GLOBALHEADER ) 
        			c->flags |= CODEC_FLAG_GLOBAL_HEADER;

				// Allow the user to override the audio fourcc
				opl::string afourcc = opl::to_string( prop_afourcc_.value< opl::wstring >( ) );
				if ( afourcc != "" )
				{
					char *tail = NULL;
					const char *arg = afourcc.c_str( );
    				int tag = strtol( arg, &tail, 0);
    				if( !tail || *tail )
        				tag = arg[ 0 ] + ( arg[ 1 ] << 8 ) + ( arg[ 2 ] << 16 ) + ( arg[ 3 ] << 24 );
					c->codec_tag = tag;
				}
			}

			return st;
		}

		// Open the video stream
		// Note: This method returns true if there is no video stream created
		bool open_video_stream( )
		{
			bool ret = true;

			if ( video_stream_ != NULL )
			{
				// Get the codec
				AVCodecContext *video_enc = video_stream_->codec;

				// find the video encoder
				AVCodec *codec = avcodec_find_encoder( video_enc->codec_id );

				if( codec && codec->pix_fmts )
				{
					const enum PixelFormat *p = codec->pix_fmts;
					for( ; *p != -1; p++ )
					{
						if( *p == video_enc->pix_fmt )
							break;
					}
					if( *p == -1 )
						video_enc->pix_fmt = codec->pix_fmts[ 0 ];
				}

        		if ( prop_pass_.value< int >( ) )
				{
            		if ( prop_pass_.value< int >( ) == 1 )
                		video_enc->flags |= CODEC_FLAG_PASS1;
            		else 
                		video_enc->flags |= CODEC_FLAG_PASS2;
					video_enc->stats_in = log_;
				}

				// Allocate the image
				avcodec_get_frame_defaults( &av_image_ );
				avpicture_alloc( ( AVPicture * )&av_image_, video_enc->pix_fmt, prop_width_.value< int >( ), prop_height_.value< int >( ) );

				// Open the codec safely
				ret = codec != NULL && avcodec_open( video_enc, codec ) >= 0;
			}

			return ret;
		}

		// Open the audio stream
		// Note: This method returns true if there is no audio stream created
		bool open_audio_stream( )
		{
			bool ret = true;

			if ( audio_stream_ != NULL )
			{
				// Get the context
				AVCodecContext *c = audio_stream_->codec;

				// Find the encoder
				AVCodec *codec = avcodec_find_encoder( c->codec_id );

				// Continue if codec found and we can open it
				if ( codec != NULL && avcodec_open( c, codec ) >= 0 )
				{
					if ( c->frame_size <= 1 ) 
					{
						audio_input_frame_size_ = audio_outbuf_size_ / c->channels;
						switch( audio_stream_->codec->codec_id ) 
						{
							case CODEC_ID_PCM_S16LE:
							case CODEC_ID_PCM_S16BE:
							case CODEC_ID_PCM_U16LE:
							case CODEC_ID_PCM_U16BE:
								audio_input_frame_size_ >>= 1;
								break;
							default:
								break;
						}
					} 
					else 
					{
						audio_input_frame_size_ = c->frame_size;
					}

					// Some formats want stream headers to be seperate
					if( oc_->oformat->flags & AVFMT_GLOBALHEADER )
						c->flags |= CODEC_FLAG_GLOBAL_HEADER;

					ret = audio_input_frame_size_ != 0;
				}
			}

			return ret;
		}

		// Queue video and audio components from frame
		void queue( frame_type_ptr frame )
		{
			// Queue video - this is simply the frames image
			if ( video_stream_ && frame->get_image( ) )
				video_queue_.push_back( frame->get_image( ) );

			// Queue audio - audio is carved up here to match the number of bytes required by the audio codec
			if ( audio_stream_ && frame->get_audio( ) )
			{
				typedef audio< unsigned char, pcm16 > pcm16;

				audio_type_ptr current = frame->get_audio( );

				if ( audio_block_ == 0 )
				{
					audio_block_used_ = 0;
					audio_block_ = audio_type_ptr( new audio_type( pcm16( current->frequency( ), current->channels( ), audio_input_frame_size_ ) ) );
				}

				int bytes = audio_input_frame_size_ * current->channels( ) * 2;
				int available = current->allocsize( );
				int offset = 0;

				while ( bytes > 0 && audio_block_used_ + available > bytes )
				{
					memcpy( audio_block_->data( ) + audio_block_used_, current->data( ) + offset, bytes - audio_block_used_ );
					audio_queue_.push_back( audio_block_ );
					available -= bytes - audio_block_used_;
					offset += bytes - audio_block_used_;
					audio_block_used_ = 0;
					audio_block_ = audio_type_ptr( new audio_type( pcm16( current->frequency( ), current->channels( ), audio_input_frame_size_ ) ) );
				}

				if ( offset < current->allocsize( ) )
				{
					memcpy( audio_block_->data( ) + audio_block_used_, current->data( ) + offset, current->allocsize( ) - offset );
					audio_block_used_ += current->allocsize( ) - offset;
				}
			}
		}

		// Process and output an image
		// Precondition: the video queue is not empty
		bool process_video( )
		{
			bool ret = true;

			// Obtain the next image
			image_type_ptr image = *( video_queue_.begin( ) );
			video_queue_.pop_front( );

			AVCodecContext *c = video_stream_->codec;

			// Convert the image to the colour space required
			const std::wstring pf = avformat_to_oil( c->pix_fmt );
			if ( pf != L"" )
			{
				image = il::convert( image, pf );
				// Need an ffmpeg fallback here...
				if ( image == 0 )
					return false;
			}
			else
			{
				// Need an ffmpeg fallback here...
				return false;
			}

			// Convert the oil image to an avformat one
			// TODO: Check if memcpy is needed
			int plane = 0;

			for ( plane = 0; plane < image->plane_count( ); plane ++ )
			{
				uint8_t *src = image->data( plane );
				uint8_t *dst = av_image_.data[ plane ];
				int height = image->height( plane );
				while( height -- )
				{
					memcpy( dst, src, av_image_.linesize[ plane ] );
					src += image->pitch( plane );
					dst += av_image_.linesize[ plane ];
				}
			}

			if ( oc_->oformat->flags & AVFMT_RAWPICTURE )  
			{
 				// raw video case. The API will change slightly in the near future for that
				AVPacket pkt;
				av_init_packet( &pkt );
        
				pkt.flags |= PKT_FLAG_KEY;
				pkt.stream_index = video_stream_->index;
				pkt.data = ( uint8_t * )&av_image_;
				pkt.size = sizeof( AVPicture );

				ret = av_interleaved_write_frame( oc_, &pkt ) == 0;
 			} 
			else 
			{
				// Set the quality
				av_image_.quality = int( video_stream_->quality );
				av_image_.pict_type = 0;

 				// Encode the image
 				int out_size = avcodec_encode_video( c, video_outbuf_, video_outbuf_size_, &av_image_ );

				// If zero size, it means the image was buffered
 				if ( out_size > 0 )
				{
					AVPacket pkt;
					av_init_packet( &pkt );

					if ( c->coded_frame && uint64_t( c->coded_frame->pts ) != AV_NOPTS_VALUE )
						pkt.pts = av_rescale_q( c->coded_frame->pts, c->time_base, video_stream_->time_base );
					if( c->coded_frame && c->coded_frame->key_frame )
						pkt.flags |= PKT_FLAG_KEY;
					pkt.stream_index = video_stream_->index;
					pkt.data = video_outbuf_;
					pkt.size = out_size;

					if ( log_file_ && prop_pass_.value< int >( ) == 1 )
						fprintf( log_file_, "%s", c->stats_out );

					// Write the compressed frame in the media file
					ret = av_interleaved_write_frame( oc_, &pkt ) == 0;

					if ( log_file_ && prop_pass_.value< int >( ) == 1 )
						fprintf( log_file_, "%s", c->stats_out );
 				}
				else
				{
					ret = true;
				}
			}

			return ret;
		}

		// Process and output a block of audio samples (see comments in queue above)
		// Precondition: the audio queue is not empty
		bool process_audio( )
		{
			bool ret = true;

			audio_type_ptr audio = *( audio_queue_.begin( ) );
			audio_queue_.pop_front( );

			AVCodecContext *c = audio_stream_->codec;

			AVPacket pkt;
			av_init_packet( &pkt );

			pkt.size = avcodec_encode_audio( c, audio_outbuf_, audio_outbuf_size_, ( short * )audio->data( ) );

			// Write the compressed frame in the media file
			if ( c->coded_frame && uint64_t( c->coded_frame->pts ) != AV_NOPTS_VALUE )
				pkt.pts = av_rescale_q( c->coded_frame->pts, c->time_base, audio_stream_->time_base );

			pkt.flags |= PKT_FLAG_KEY;
			pkt.stream_index = audio_stream_->index;
			pkt.data = audio_outbuf_;

			if ( pkt.size )
				if ( av_interleaved_write_frame( oc_, &pkt ) != 0 )
					ret = false;

			return ret;
		}

		// The output file or device
		opl::wstring resource_;
		pcos::property prop_enable_audio_;
		pcos::property prop_enable_video_;

		// libavformat structures
		AVFormatContext *oc_;
		AVOutputFormat *fmt_;
		AVStream *audio_stream_;
		AVStream *video_stream_;
		int audio_input_frame_size_;
		int audio_outbuf_size_;
		uint8_t *audio_outbuf_;
		AVFrame av_image_; 
		uint8_t *video_outbuf_;
		int video_outbuf_size_;

		// oml structures
		std::deque< image_type_ptr > video_queue_;
		std::deque< audio_type_ptr > audio_queue_;
		audio_type_ptr audio_block_;
		int audio_block_used_;

		// Application facing properties
		pcos::property prop_show_stats_;
		pcos::property prop_format_;
		pcos::property prop_vcodec_;
		pcos::property prop_acodec_;
		pcos::property prop_pix_fmt_;
		pcos::property prop_vfourcc_;
		pcos::property prop_afourcc_;
		pcos::property prop_width_;
		pcos::property prop_height_;
		pcos::property prop_aspect_ratio_;
		pcos::property prop_fps_num_;
		pcos::property prop_fps_den_;
		pcos::property prop_frequency_;
		pcos::property prop_channels_;
		pcos::property prop_audio_bit_rate_;
		pcos::property prop_video_bit_rate_;
		pcos::property /* int */ prop_video_bit_rate_tolerance_;
		pcos::property /* int */ prop_gop_size_;
		pcos::property /* int */ prop_b_frames_;
		pcos::property /* int */ prop_mb_decision_;
		pcos::property /* double */ prop_qscale_;
		pcos::property /* int */ prop_me_method_;
		pcos::property /* int */ prop_mb_cmp_;
		pcos::property /* int */ prop_ildct_cmp_;
		pcos::property /* int */ prop_sub_cmp_;
		pcos::property /* int */ prop_cmp_;
		pcos::property /* int */ prop_pre_cmp_;
		pcos::property /* int */ prop_pre_me_;
		pcos::property /* double */ prop_lumi_mask_;
		pcos::property /* double */ prop_dark_mask_;
		pcos::property /* double */ prop_scplx_mask_;
		pcos::property /* double */ prop_tcplx_mask_;
		pcos::property /* double */ prop_p_mask_;
		pcos::property /* int */ prop_qns_;
		pcos::property /* int */ prop_video_qmin_;
		pcos::property /* int */ prop_video_qmax_;
		pcos::property /* int */ prop_video_lmin_;
		pcos::property /* int */ prop_video_lmax_;
		pcos::property /* int */ prop_video_mb_qmin_;
		pcos::property /* int */ prop_video_mb_qmax_;
		pcos::property /* int */ prop_video_qdiff_;
		pcos::property /* double */ prop_video_qblur_;
		pcos::property /* double */ prop_video_qcomp_;
		pcos::property /* int */ prop_mux_rate_;
		pcos::property /* double */ prop_mux_preload_;
		pcos::property /* double */ prop_mux_delay_;
		pcos::property /* int */ prop_video_packet_size_;
		pcos::property /* int */ prop_video_rc_max_rate_;
		pcos::property /* int */ prop_video_rc_min_rate_;
		pcos::property /* int */ prop_video_rc_buffer_size_;
		pcos::property /* double */ prop_video_rc_buffer_aggressivity_;
		pcos::property /* double */ prop_video_rc_initial_cplx_;
		pcos::property /* double */ prop_video_i_qfactor_;
		pcos::property /* double */ prop_video_b_qfactor_;
		pcos::property /* double */ prop_video_i_qoffset_;
		pcos::property /* double */ prop_video_b_qoffset_;
		pcos::property /* int */ prop_video_intra_quant_bias_;
		pcos::property /* int */ prop_video_inter_quant_bias_;
		pcos::property /* int */ prop_dct_algo_;
		pcos::property /* int */ prop_idct_algo_;
		pcos::property /* int */ prop_me_threshold_;
		pcos::property /* int */ prop_mb_threshold_;
		pcos::property /* int */ prop_intra_dc_precision_;
		pcos::property /* int */ prop_strict_;
		pcos::property /* int */ prop_error_rate_;
		pcos::property /* int */ prop_noise_reduction_;
		pcos::property /* int */ prop_sc_threshold_;
		pcos::property /* int */ prop_me_range_;
		pcos::property /* int */ prop_coder_;
		pcos::property /* int */ prop_context_;
		pcos::property /* int */ prop_predictor_;
		pcos::property /* int */ prop_pass_;
		pcos::property /* wstring */ prop_pass_log_;

		FILE *log_file_;
		char *log_;

		int frame_pos_;
};

class ML_PLUGIN_DECLSPEC avformat_input : public input_type
{
	public:
		typedef audio< unsigned char, pcm16 > pcm16_audio_type;

		// Constructor and destructor
		avformat_input( opl::wstring resource, const opl::wstring mime_type = L"" ) 
			: input_type( ) 
			, uri_( resource )
			, mime_type_( mime_type )
			, frames_( 0 )
			, is_seekable_( true )
			, first_frame_( true )
			, first_found_( 0 )
			, fps_num_( 25 )
			, fps_den_( 1 )
			, sar_num_( 59 )
			, sar_den_( 54 )
			, width_( 720 )
			, height_( 576 )
			, context_( 0 )
			, format_( 0 )
			, params_( 0 )
			, prop_video_index_( pcos::key::from_string( "video_index" ) )
			, prop_audio_index_( pcos::key::from_string( "audio_index" ) )
			, prop_gop_size_( pcos::key::from_string( "gop_size" ) )
			, prop_gop_cache_( pcos::key::from_string( "gop_cache" ) )
			, expected_( 0 )
			, av_frame_( 0 )
			, video_codec_( 0 )
			, audio_codec_( 0 )
			, pkt_( )
			, images_( )
			, audio_( )
			, must_decode_( true )
			, must_reopen_( false )
			, key_search_( false )
			, audio_buf_used_( 0 )
			, audio_buf_offset_( 0 )
			, img_pos_( -1 )
			, aud_pos_( -1 )
			, img_inc_( 0 )
			, sync_diff_( 0 )
			, start_time_( 0 )
			, img_convert_( 0 )
		{
			// Allow property control of video and audio index
			// NB: Should also have read only props for stream counts
			properties( ).append( prop_video_index_ = 0 );
			properties( ).append( prop_audio_index_ = 0 );
			properties( ).append( prop_gop_size_ = -1 );
			properties( ).append( prop_gop_cache_ = 10 );
		}

		virtual ~avformat_input( ) 
		{
			if ( prop_video_index_.value< int >( ) >= 0 )
				close_video_codec( );
			if ( prop_audio_index_.value< int >( ) >= 0 )
				close_audio_codec( );
			if ( context_ )
				av_close_input_file( context_ );
			av_free( av_frame_ );
		}

		virtual bool is_thread_safe( ) const
		{
			// For now, all oml protocol handlers are consider unsafe for threading
			return uri_.find( L"oml:" ) != 0;
		}

		bool is_valid( )
		{
			return context_ != 0;
		}

		virtual double fps( ) const
		{
			int num, den;
			get_fps( num, den );
			return den != 0 ? double( num ) / double( den ) : 1;
		}

		// Analyse streams and set input values
		void populate( )
		{
			// Iterate through all the streams available
			for( size_t i = 0; i < context_->nb_streams; i++ ) 
			{
				// Get the codec context
   				AVCodecContext *codec_context = context_->streams[ i ]->codec;

				// Ignore streams that we don't have a decoder for
				if ( avcodec_find_decoder( codec_context->codec_id ) == NULL )
					continue;
		
				// Determine the type and obtain the first index of each type
   				switch( codec_context->codec_type ) 
				{
					case CODEC_TYPE_VIDEO:
						video_indexes_.push_back( i );
						break;
					case CODEC_TYPE_AUDIO:
						audio_indexes_.push_back( i );
		   				break;
					default:
		   				break;
				}
			}

			// Configure video input properties
			if ( video_indexes_.size( ) > 0 )
			{
				AVStream *stream = get_video_stream( ) ? get_video_stream( ) : context_->streams[ 0 ];

				width_ = stream->codec->width;
				height_ = stream->codec->height;

				// TODO: Need to check this one
				if ( stream->r_frame_rate.num != 0 && stream->r_frame_rate.den != 0 )
				{
					fps_num_ = stream->r_frame_rate.num;
					fps_den_ = stream->r_frame_rate.den;
				}
				else
				{
					fps_num_ = stream->codec->time_base.den;
					fps_den_ = stream->codec->time_base.num;
				}
			}
			else if ( has_audio( ) )
			{
				fps_num_ = 25;
				fps_den_ = 1;
				sar_num_ = 1;
				sar_den_ = 1;
			}

			// Open the video and audio codecs
			if ( has_video( ) ) open_video_codec( );
			if ( has_audio( ) ) open_audio_codec( );

			if ( uint64_t( context_->start_time ) != AV_NOPTS_VALUE )
				start_time_ = context_->start_time;

			// Set the duration
			if ( uint64_t( context_->duration ) != AV_NOPTS_VALUE )
				frames_ = int( ( avformat_input::fps( ) * ( context_->duration - start_time_ ) ) / ( double )AV_TIME_BASE );
			else
				frames_ = 1 << 29;

			std::string format = context_->iformat->name;

			// This is a nasty bit of code to provide support for variable bit rate
			if ( uint64_t( context_->duration ) == AV_NOPTS_VALUE || format == "mp2" || format == "mp3" || format == "ogg" || format == "mpeg" )
			{
				if ( is_seekable_ )
				{
					AVStream *stream = get_video_stream( ) ? get_video_stream( ) : get_audio_stream( );
					int max = 0;

					frames_ = 1 << 29;
					seek( frames_ );
					while( frames_ > 1 )
					{
						seek_to_position( );
						av_init_packet( &pkt_ );
						av_read_frame( context_, &pkt_ );
            			int result = int( av_q2d( stream->time_base ) * ( pkt_.dts - av_rescale_q( start_time_, ml_av_time_base_q, stream->time_base ) ) * fps( ) + 0.5 );
						av_free_packet( &pkt_ );
						if ( result > 0 && result <= max ) break;
						max = result;
						frames_ /= 2;
						seek( frames_ );
					}

					frames_ = max + 1;

					if ( format != "mpeg" )
					{
						av_init_packet( &pkt_ );
						while ( stream && av_read_frame( context_, &pkt_ ) >= 0 )
						{
            				int result = int( av_q2d( stream->time_base ) * ( pkt_.dts - av_rescale_q( start_time_, ml_av_time_base_q, stream->time_base ) ) * fps( ) + 0.5 );
							if ( result > max )
								max = result;
							av_free_packet( &pkt_ );
						}
					}

					frames_ = max > 0 ? max : frames_;
					seek( 0 );
					av_seek_frame( context_, -1, 0, AVSEEK_FLAG_BYTE );
				}
			}

			// Work around for inefficiencies on I frame only seeking
			// - this should be covered by the AVStream discard and need_parsing values
			//   but they're either wrong or poorly documented...
			if ( has_video( ) )
			{
				std::string codec( get_video_stream( )->codec->codec->name );
				if ( codec == "mjpeg" )
					must_decode_ = false;
				else if ( codec == "dvvideo" )
					must_decode_ = false;
				else if ( codec == "rawvideo" )
					must_decode_ = false;
				else if ( codec == "dirac" )
					must_reopen_ = true;

				if ( format == "mpeg" && prop_gop_size_.value< int >( ) == -1 )
					prop_gop_size_ = 24;
				else if ( format == "mp3" && prop_gop_size_.value< int >( ) == -1 )
					prop_gop_size_ = 12;
				else if ( format == "mp2" && prop_gop_size_.value< int >( ) == -1 )
					prop_gop_size_ = 12;
				else if ( prop_gop_size_.value< int >( ) == -1 )
					prop_gop_size_ = 0;
			}
			else
			{
				if ( prop_gop_size_.value< int >( ) == -1 )
					prop_gop_size_ = 0;
			}
		}

		// Returns the current video stream
		inline AVStream *get_video_stream( )
		{
			if ( prop_video_index_.value< int >( ) >= 0 && video_indexes_.size( ) > 0 )
				return context_->streams[ video_indexes_[ prop_video_index_.value< int >( ) ] ];
			else
				return 0;
		}

		// Returns the current audio stream
		inline AVStream *get_audio_stream( )
		{
			if ( prop_audio_index_.value< int >( ) >= 0 && audio_indexes_.size( ) > 0 )
				return context_->streams[ audio_indexes_[ prop_audio_index_.value< int >( ) ] ];
			else
				return 0;
		}

		// Opens the video codec associated to the current stream
		void open_video_codec( )
		{
			AVStream *stream = get_video_stream( );
			AVCodecContext *codec_context = stream->codec;
			video_codec_ = avcodec_find_decoder( codec_context->codec_id );
			if ( video_codec_ == NULL || avcodec_open( codec_context, video_codec_ ) < 0 )
				prop_video_index_ = -1;
		}

		void close_video_codec( )
		{
			AVStream *stream = get_video_stream( );
			if ( stream && stream->codec )
				avcodec_close( stream->codec );
		}
		
		// Opens the audio codec associated to the current stream
		void open_audio_codec( )
		{
			AVStream *stream = get_audio_stream( );
			AVCodecContext *codec_context = stream->codec;
			audio_codec_ = avcodec_find_decoder( codec_context->codec_id );
			if ( audio_codec_ == NULL || avcodec_open( codec_context, audio_codec_ ) < 0 )
				prop_audio_index_ = -1;
		}

		void close_audio_codec( )
		{
			AVStream *stream = get_audio_stream( );
			if ( stream && stream->codec )
				avcodec_close( stream->codec );
		}

		// Seek to the requested frame
		inline bool seek_to_position( )
		{
			if ( is_seekable_ )
			{
				int position = get_position( );

				if ( must_decode_ )
					position -= prop_gop_size_.value< int >( );

				int64_t offset = int64_t( ( ( double )position / avformat_input::fps( ) ) * AV_TIME_BASE ) + start_time_;
				if ( must_reopen_ )
					reopen( );

				int result = av_seek_frame( context_, -1, offset, AVSEEK_FLAG_BACKWARD );
				key_search_ = true;

				aud_pos_ = img_pos_ = 0;
				img_inc_ = 0;
				audio_buf_used_ = 0;

				if ( get_audio_stream( ) )
					avcodec_flush_buffers( get_audio_stream( )->codec );
				if ( get_video_stream( ) )
					avcodec_flush_buffers( get_video_stream( )->codec );

				return result >= 0;
			}
			else
			{
				return false;
			}
		}

		// Decode the image
		int decode_image( bool &got_picture )
		{
			AVCodecContext *codec_context = get_video_stream( )->codec;

			int ret = 0;
			int got_dummy = 0;
			int got_pict = 0;
			
			// Derive the dts
			double dts = 0;
			if ( uint64_t( pkt_.dts ) != AV_NOPTS_VALUE )
            	dts = av_q2d( get_video_stream( )->time_base ) * ( pkt_.dts - av_rescale_q( start_time_, ml_av_time_base_q, get_video_stream( )->time_base ) );

			// Approximate frame position
			int position = int( dts * fps( ) + 0.5 );

			position += img_inc_;

			// Small optimisation - abandon packet now if we can (ie: we don't have to decode
			// and no image is requested for this frame)
			if ( !must_decode_ && ( get_process_flags( ) & ml::process_image ) == 0 )
			{
				got_picture = true;
				return ret;
			}

			// We have to continue processing until we get a dts >= to the requested position
			if ( position >= get_position( ) + first_found_ )
				avcodec_decode_video( codec_context, av_frame_, &got_pict, pkt_.data, pkt_.size );
			else if ( must_decode_ )
				avcodec_decode_video( codec_context, av_frame_, &got_dummy, pkt_.data, pkt_.size );

			got_picture = got_pict != 0;

			// If we have just done a search, then we need to locate the first key frame
			// all others are discarded.
			if ( key_search_ && av_frame_->key_frame == 0 && position < get_position( ) + first_found_ )
			{
				got_picture = false;
				got_dummy = false;
			}
			else
			{
				key_search_ = false;
			}

			// Store the image in the queue
			if ( position >= 0 && ( got_picture || got_dummy ) )
			{
				if ( position >= get_position( ) + first_found_ )
					store_image( dts, position );
				else
					img_pos_ = position;
				if ( aud_pos_ == -1 )
					sync_diff_ = 0;
				else if ( sync_diff_ == 0 )
					sync_diff_ = aud_pos_ - img_pos_;
				if ( !prop_gop_size_.value< int >( ) && aud_pos_ != -1 && sync_diff_ > 0 && aud_pos_ - img_pos_ > sync_diff_ )
				{
					if ( ++ position >= get_position( ) + first_found_ )
						store_image( dts, position );
					else
						img_pos_ = position;
					img_inc_ += 1;
				}
			}

			if ( position - first_found_ >= frames_ )
				frames_ = position - first_found_ + 1;

			return ret;
		}

		void store_image( double pts, int64_t position )
		{
			AVStream *stream = get_video_stream( );
			AVCodecContext *codec_context = stream->codec;
			int width = get_width( );
			int height = get_height( );
			std::wstring format = avformat_to_oil( codec_context->pix_fmt );
			image_type_ptr image;
			if ( format == L"" )
			{
				image = il::allocate( L"yuv420p", width, height );
        		AVPicture output;
        		avpicture_fill( &output, image->data( ), PIX_FMT_YUV420P, width, height );
        		img_convert_ = sws_getCachedContext( img_convert_, width, height, codec_context->pix_fmt, width, height, PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL );
        		if ( img_convert_ != NULL )
        			sws_scale( img_convert_, av_frame_->data, av_frame_->linesize, 0, height, output.data, output.linesize );
			}
			else
			{
				image = il::allocate( format, width, height );
				AVPicture output;
				avpicture_fill( &output, image->data( ), codec_context->pix_fmt, width, height );
        		img_convert_ = sws_getCachedContext( img_convert_, width, height, codec_context->pix_fmt, width, height, codec_context->pix_fmt, SWS_BICUBIC, NULL, NULL, NULL );
        		if ( img_convert_ != NULL )
        			sws_scale( img_convert_, av_frame_->data, av_frame_->linesize, 0, height, output.data, output.linesize );
			}

			if ( first_frame_ )
			{
				first_found_ = position - get_position( );
				first_frame_ = false;
			}

			image->set_pts( pts );
			image->set_position( position );

			img_pos_ = int( position );

			if ( images_.size( ) > 0 )
			{
				int first = images_[ 0 ]->position( );
				int last = images_[ images_.size( ) - 1 ]->position( );
				if ( position < last )
					images_.clear( );
				else if ( first < get_position( ) + first_found_ - prop_gop_cache_.value< int >( ) )
					images_.erase( images_.begin( ) );
			}

			if ( av_frame_->interlaced_frame )
				image->set_field_order( av_frame_->top_field_first ? il::top_field_first : il::bottom_field_first );

			image->set_writable( false );
			images_.push_back( image );
		}

		inline int samples_for_frame( int frequency, int index )
		{
			return ml::audio_samples_for_frame( index, frequency, fps_num_, fps_den_ );
		}

		int decode_audio( bool &got_audio )
		{
			int ret = 0;

			// Get the audio codec context
			AVCodecContext *codec_context = get_audio_stream( )->codec;

			// The number of bytes in the packet
			int len = pkt_.size;
			uint8_t *data = pkt_.data;

			// This is the pts of the packet
			double dts = 0;
			if ( uint64_t( pkt_.dts ) != AV_NOPTS_VALUE )
           		dts = av_q2d( get_audio_stream( )->time_base ) * ( pkt_.dts - av_rescale_q( start_time_, ml_av_time_base_q, get_audio_stream( )->time_base ) );
			int found = int( dts * avformat_input::fps( ) );

			// Get the audio info from the codec context
			int channels = codec_context->channels;
			int frequency = codec_context->sample_rate;
			int bps = 2;
			int skip = 0;

			if ( audio_buf_used_ == 0 )
			{
				if ( audio_.size( ) != 0 )
				{
					audio_buf_offset_ = ( *( audio_.end( ) - 1 ) )->position( ) + 1;
				}
				else
				{
					int64_t samples_to_dts = int64_t( dts * frequency + 0.5 );
					int64_t samples_to_next = ml::audio_samples_to_frame( int( dts * avformat_input::fps( ) + 0.5 ), frequency, fps_num_, fps_den_ );

					if ( samples_to_dts < samples_to_next )
					{
						skip = int( samples_to_next - samples_to_dts ) * channels * bps;
						found = int( dts * avformat_input::fps( ) + 0.5 );
					}
					else
					{
						audio_buf_used_ = size_t( samples_to_dts - samples_to_next ) * channels * bps;
						memset( audio_buf_, 0, audio_buf_used_ );
					}

					audio_buf_offset_ = found;
				}
			}

			// Each packet may need multiple parses
			while( len > 0 )
			{
				int audio_size = sizeof(audio_buf_) - audio_buf_used_;

				// Decode the audio into the buffer
           		ret = avcodec_decode_audio2( codec_context, ( short * )( audio_buf_ + audio_buf_used_ ), &audio_size, data, len );

				// If no samples are returned, then break now
				if ( ret < 0 )
				{
					ret = 0;
					got_audio = true;
					break;
				}

				// Decrement the length by the number of bytes parsed
				len -= ret;
				data += ret;

				// Increment the number of bytes used in the buffer
				if ( audio_size > 0 )
					audio_buf_used_ += audio_size;

				// Current index in the buffer
				int index = 0;

				if ( audio_buf_used_ > skip )
				{
					index = skip;
					skip = 0;
				}
				else
				{
					skip -= audio_buf_used_;
					audio_buf_used_ = 0;
				}

				// Now we need to split the audio up into frame sized dollops
				while( 1 )
				{
					int samples = samples_for_frame( frequency, audio_buf_offset_ );

					int bytes_for_frame = samples * channels * bps;

					if ( bytes_for_frame > ( audio_buf_used_ - index ) )
						break;

					// Create an audio frame
					index += store_audio( audio_buf_offset_, audio_buf_ + index, samples );

					if ( audio_buf_offset_ >= get_position( ) + first_found_ )
						got_audio = 1;

					if ( audio_buf_offset_ - first_found_ >= frames_ )
						frames_ = audio_buf_offset_ - first_found_ + 1;

					audio_buf_offset_ += 1;
				}

				audio_buf_used_ -= index;
				if ( audio_buf_used_ && index != 0 )
					memmove( audio_buf_, audio_buf_ + index, audio_buf_used_ );
			}

			return ret;
		}

		int store_audio( int position, uint8_t *buf, int samples )
		{
			// Get the audio codec context
			AVCodecContext *codec_context = get_audio_stream( )->codec;

			// Get the audio info from the codec context
			int channels = codec_context->channels;
			int frequency = codec_context->sample_rate;

			if ( first_frame_ )
			{
				first_found_ = position - get_position( );
				first_frame_ = false;
			}

			audio_type_ptr aud = audio_type_ptr( new audio_type( pcm16_audio_type( frequency, channels, samples ) ) );
			aud->set_position( position );
			memcpy( aud->data( ), buf, aud->size( ) );

			if ( audio_.size( ) > 0 )
			{
				int first = audio_[ 0 ]->position( );
				int last = audio_[ audio_.size( ) - 1 ]->position( );
				if ( position != last + 1 )
					audio_.clear( );
				else if ( first < get_position( ) + first_found_ - prop_gop_cache_.value< int >( ) )
					audio_.erase( audio_.begin( ) );
			}

			audio_.push_back( aud );

			if ( position >= get_position( ) )
				aud_pos_ = position;

			return aud->size( );
		}

		void clear_stores( bool force = false )
		{
			bool clear = true;

			if ( !force && images_.size( ) )
			{
				int current = get_position( ) + first_found_;
				int first = images_[ 0 ]->position( );
				int last = images_[ images_.size( ) - 1 ]->position( );
				if ( current >= first && current <= last )
					clear = false;
			}

			if ( clear )
			{
				images_.clear( );
				audio_.clear( );
				audio_buf_used_ = 0;
			}
		}

		void find_image( frame_type *frame )
		{
			int current = get_position( ) + first_found_;
			int closest = 1 << 16;
			std::deque< image_type_ptr >::iterator result = images_.end( );
			std::deque< image_type_ptr >::iterator iter;

			for ( iter = images_.begin( ); iter != images_.end( ); iter ++ )
			{
				image_type_ptr img = *iter;
				int diff = current - img->position( );
				if ( std::abs( diff ) <= closest )
				{
					result = iter;
					closest = std::abs( diff );
				}
				else if ( img->position( ) > current )
				{
					break;
				}
			}

			if ( result != images_.end( ) )
				frame->set_image( *result );
		}

		void find_audio( frame_type *frame )
		{
			int current = get_position( ) + first_found_;
			int closest = 1 << 16;
			std::deque< audio_type_ptr >::iterator result = audio_.end( );
			std::deque< audio_type_ptr >::iterator iter;

			for ( iter = audio_.begin( ); iter != audio_.end( ); iter ++ )
			{
				audio_type_ptr aud = *iter;
				int diff = current - aud->position( );
				if ( std::abs( diff ) <= closest )
				{
					result = iter;
					closest = std::abs( diff );
				}
				else if ( aud->position( ) > current )
				{
					break;
				}
			}

			if ( result != audio_.end( ) && ( *result )->position( ) == current )
			{
				frame->set_audio( ml::audio_type_ptr( ( *result )->clone( ) ) );
				frame->set_duration( double( ( *result )->samples( ) ) / double( ( *result )->frequency( ) ) );
			}
			else
			{
				AVCodecContext *codec_context = get_audio_stream( )->codec;
				int channels = codec_context->channels;
				int frequency = codec_context->sample_rate;
				int samples = samples_for_frame( frequency, current );
				audio_type_ptr aud = audio_type_ptr( new audio_type( pcm16_audio_type( frequency, channels, samples ) ) );
				aud->set_position( current );
				memset( aud->data( ), 0, aud->size( ) );
				frame->set_audio( aud );
				frame->set_duration( double( samples ) / double( frequency ) );
			}
		}

		// Basic information
		virtual const opl::wstring get_uri( ) const { return uri_; }
		virtual const opl::wstring get_mime_type( ) const { return mime_type_; }
		virtual bool has_video( ) const { return prop_video_index_.value< int >( ) != -1 && video_indexes_.size( ) > 0; }
		virtual bool has_audio( ) const { return prop_audio_index_.value< int >( ) != -1 && audio_indexes_.size( ) > 0; }

		// Audio/Visual
		virtual int get_frames( ) const { return frames_; }
		virtual bool is_seekable( ) const { return is_seekable_; }

		// Visual
		virtual void get_fps( int &num, int &den ) const { num = fps_num_; den = fps_den_; }
		virtual void get_sar( int &num, int &den ) const { num = sar_num_; den = sar_den_; }
		virtual int get_video_streams( ) const { return video_indexes_.size( ); }
		virtual int get_width( ) const { return width_; }
		virtual int get_height( ) const { return height_; }

		// Audio
		virtual int get_audio_streams( ) const { return audio_indexes_.size( ); }
		virtual bool set_video_stream( const int stream ) { prop_video_index_ = stream; return true; }

		virtual bool set_audio_stream( const int stream ) 
		{
			if ( stream < 0 || stream >= int( audio_indexes_.size( ) ) )
			{
				prop_audio_index_ = -1;
			}
			else if ( stream < int( audio_indexes_.size( ) ) )
			{
				prop_audio_index_ = stream;
			}

			return true; 
		}

		// Fetch method
		virtual frame_type_ptr fetch( )
		{
			int process_flags = get_process_flags( );

			// Seek to the correct position if necessary
			if ( get_position( ) != expected_ )
			{
				int valid = 0;
				if ( images_.size( ) > 0 )
					valid = images_[ 0 ]->position( );
				if ( audio_.size( ) > 0 )
					valid = audio_[ 0 ]->position( );

				if ( get_position( ) + first_found_ >= valid && get_position( ) + first_found_ < int( expected_ + 2 * fps( ) ) )
					;
				else if ( seek_to_position( ) )
					clear_stores( true );
				else
					seek( expected_ );
				expected_ = get_position( ) + first_found_;
			}

			// Clear the packet
			av_init_packet( &pkt_ );

			// Loop until an error or we have what we need
			int error = 0;
			bool got_picture = !has_video( );
			bool got_audio = !has_audio( );
			int current = get_position( ) + first_found_;

			if ( ( process_flags & process_image ) != 0 && ( has_video( ) && images_.size( ) > 0 ) )
			{
				int first = images_[ 0 ]->position( );
				int last = images_[ images_.size( ) - 1 ]->position( );
				if ( current >= first && current <= last )
					got_picture = true;
			}

			if ( ( process_flags & process_audio ) != 0 && ( has_audio( ) && audio_.size( ) > 0 ) )
			{
				int first = audio_[ 0 ]->position( );
				int last = audio_[ audio_.size( ) - 1 ]->position( );
				if ( current >= first && current <= last )
					got_audio = true;
			}

			if ( got_picture && get_position( ) == get_frames( ) - 1 )
				got_picture = false;

			while( error >= 0 && ( !got_picture || !got_audio ) )
			{
				error = av_read_frame( context_, &pkt_ );
				if ( error >= 0 && video_indexes_.size( ) && prop_video_index_.value< int >( ) != -1 && pkt_.stream_index == video_indexes_[ prop_video_index_.value< int >( ) ] )
					error = decode_image( got_picture );
				else if ( error >= 0 && audio_indexes_.size( ) && prop_audio_index_.value< int >( ) != -1 && pkt_.stream_index == audio_indexes_[ prop_audio_index_.value< int >( ) ] )
					error = decode_audio( got_audio );
				else if ( error < 0 && !is_seekable_ )
					frames_ = expected_ - 1;
				av_free_packet( &pkt_ );
			}

			// Hmmph
			if ( has_video( ) )
			{
				sar_num_ = get_video_stream( )->codec->sample_aspect_ratio.num;
				sar_den_ = get_video_stream( )->codec->sample_aspect_ratio.den;
				sar_num_ = sar_num_ != 0 ? sar_num_ : 1;
				sar_den_ = sar_den_ != 0 ? sar_den_ : 1;
			}

			// Create the output frame
			frame_type *result = new frame_type( );

			result->set_sar( sar_num_, sar_den_ );
			result->set_fps( fps_num_, fps_den_ );
			result->set_position( expected_ );
			result->set_pts( expected_ * 1.0 / avformat_input::fps( ) );
			result->set_duration( 1.0 / avformat_input::fps( ) );

			if ( ( process_flags & process_image ) && has_video( ) )
				find_image( result );
			if ( ( process_flags & process_audio ) && has_audio( ) )
				find_audio( result );

			// Update the next expected position
			expected_ ++;

			return frame_type_ptr( result );
		}

	protected:

		virtual bool initialize( )
		{
			opl::wstring resource = uri_;

			// A mechanism to ensure that avformat can always be accessed
			if ( resource.find( L"avformat:" ) == 0 )
				resource = resource.substr( 9 );

			// Convenience expansion for *nix based people
			if ( resource.find( L"~" ) == 0 )
				resource = opl::to_wstring( getenv( "HOME" ) ) + resource.substr( 1 );

			// Ugly - looking to see if a dv1394 device has been specified
			if ( resource.find( L"/dev/" ) == 0 && resource.find( L"1394" ) != opl::wstring::npos )
			{
				format_ = av_find_input_format( "dv" );
			}

			// Allow dv on stdin
			if ( resource == L"dv:-" )
			{
				format_ = av_find_input_format( "dv" );
				resource = L"pipe:";
				is_seekable_ = false;
			}

			// Allow mpeg on stdin
			if ( resource == L"mpeg:-" )
			{
				format_ = av_find_input_format( "mpeg" );
				resource = L"pipe:";
				is_seekable_ = false;
				key_search_ = true;
			}

			// Corrections for file formats
			if ( resource.find( L".mpg" ) == resource.length( ) - 4 )
				key_search_ = true;
			else if ( resource.find( L".dv" ) == resource.length( ) - 3 )
				format_ = av_find_input_format( "dv" );

			// Attempt to open the resource
			int error = av_open_input_file( &context_, opl::to_string( resource ).c_str( ), format_, 0, params_ ) < 0;

			// Check for streaming
			if ( error == 0 && url_is_streamed( &context_->pb ) )
			{
				is_seekable_ = false;
				key_search_ = true;
			}

			// Get the stream info
			if ( error == 0 )
				error = av_find_stream_info( context_ ) < 0;
			
			// Populate the input properties
			if ( error == 0 )
				populate( );

			// Allocate an av frame
			av_frame_ = avcodec_alloc_frame( );

			// If the stream is deemed seekable, then we don't need the first_found logic
			first_frame_ = !is_seekable_;

			if ( error == 0 )
				fetch( );

			return error == 0;
		}


	private:
		void reopen( )
		{
			if ( prop_video_index_.value< int >( ) >= 0 )
				close_video_codec( );
			if ( prop_audio_index_.value< int >( ) >= 0 )
				close_audio_codec( );
			if ( context_ )
				av_close_input_file( context_ );

			// Attempt to open the resource
			int error = av_open_input_file( &context_, opl::to_string( uri_ ).c_str( ), format_, 0, params_ ) < 0;

			// Get the stream info
			if ( error == 0 )
				error = av_find_stream_info( context_ ) < 0;
			
			// Populate the input properties
			if ( error == 0 )
				populate( );
		}

	private:
		opl::wstring uri_;
		opl::wstring mime_type_;
		int frames_;
		bool is_seekable_;
		int first_frame_;
		int first_found_;
		int fps_num_;
		int fps_den_;
		int sar_num_;
		int sar_den_;
		int width_;
		int height_;

		AVFormatContext *context_;
		AVInputFormat *format_;
		AVFormatParameters *params_;
		pcos::property prop_video_index_;
		pcos::property prop_audio_index_;
		pcos::property prop_gop_size_;
		pcos::property prop_gop_cache_;
		std::vector < int > audio_indexes_;
		std::vector < int > video_indexes_;
		int expected_;
		AVFrame *av_frame_;
		AVCodec *video_codec_;
		AVCodec *audio_codec_;
		AVPacket pkt_;
		std::deque < image_type_ptr > images_;
		std::deque < audio_type_ptr > audio_;
		bool must_decode_;
		bool must_reopen_;
		bool key_search_;

    	unsigned char audio_buf_[ ( AVCODEC_MAX_AUDIO_FRAME_SIZE * 3 ) / 2 ]; 
    	int audio_buf_used_;
		int audio_buf_offset_;

		int img_pos_;
		int aud_pos_;
		int img_inc_;
		int sync_diff_;

		int64_t start_time_;
		struct SwsContext *img_convert_;
};


class ML_PLUGIN_DECLSPEC avformat_resampler_filter : public filter_type
{
	public:
		// filter_type overloads
		explicit avformat_resampler_filter(const opl::wstring &)
			: filter_type()
			, prop_output_channels_(pcos::key::from_string("channels"))
			, prop_output_sample_freq_(pcos::key::from_string("frequency"))
			, input_channels_(2)
			, input_sample_freq_(48000)
			, fps_numerator_(25)
			, fps_denominator_(1)
			, context_(NULL)
			, dirty_(true)
			, cache_()
			, in_buffer_( )
			, out_buffer_( )
		{
			property_observer_ = boost::shared_ptr<pcos::observer>(new avformat_resampler_filter::property_observer(const_cast<avformat_resampler_filter*>(this)));
		
			properties( ).append( prop_output_channels_		= int(2)		);
			properties( ).append( prop_output_sample_freq_	= int(44100)	);
		
			prop_output_channels_.attach(property_observer_);
			prop_output_sample_freq_.attach(property_observer_);
		}
	
		virtual ~avformat_resampler_filter()
		{
			if(context_)
				audio_resample_close(context_);
		
			prop_output_channels_.detach(property_observer_);
			prop_output_sample_freq_.detach(property_observer_);
		}
	
		virtual const opl::wstring get_uri( ) const { return L"resampler"; }
	
		void cache( int pos, input_type_ptr input )
		{
			if ( cache_.find( pos ) == cache_.end( ) )
			{
				input->seek( pos );
				frame_type_ptr frame = input->fetch( );
				if ( frame )
					cache_[ pos ] = frame;
			}
		}

		void fill_cache( int current_pos, input_type_ptr input )
		{
			if ( debug_level( ) )
			{
				fprintf( stderr, "ante %d: ", current_pos );
				std::map< int, frame_type_ptr >::iterator iter = cache_.begin( );
				while( iter != cache_.end( ) )
					fprintf( stderr, "%d ", ( iter ++ )->first );
				fprintf( stderr, "\n" );
			}

			int index = current_pos - 1;
			int count = 3;

			if ( current_pos == 0 )
			{
				index = 0;
				count = 2;
			}

			int p = 0;
			while ( p ++ < count && index < input->get_frames( ) )
				cache( index ++, input );

			if ( cache_.size( ) > 3 )
			{
				std::map< int, frame_type_ptr >::iterator iter = cache_.begin( );
				while( iter != cache_.end( ) )
				{
					if ( iter->first < current_pos - 1 || iter->first > current_pos + 1 )
					{
						int index = iter->first;
						iter ++;
						cache_.erase( index );
					}
					else
					{
						iter ++;
					}
				}
			}

			if ( debug_level( ) )
			{
				fprintf( stderr, "post %d: ", current_pos );
				std::map< int, frame_type_ptr >::iterator iter = cache_.begin( );
				while( iter != cache_.end( ) )
					fprintf( stderr, "%d ", ( iter ++ )->first );
				fprintf( stderr, "\n" );
			}
		}

		frame_type_ptr fetch()
		{
			// Get the filter input
			input_type_ptr input = fetch_slot( );
			if(!input)
				return frame_type_ptr();
			
			// Cache a local copy of current position
			int current_pos = get_position();
		
			// Ensure the cache is full
			fill_cache( current_pos, input );
		
			//-----------------------------------------------------------------------------------
			// Appropriately cache pointers to the audio of the previous, current and next frames
			//-----------------------------------------------------------------------------------
			frame_type_ptr	current_frame;
			audio_type_ptr	previous_audio,
							current_audio,
							next_audio;
		
			// If the current position is not the start, cache a pointer to the audio of the previous frame
			if(current_pos > 0)
				previous_audio = cache_[ current_pos - 1 ]->get_audio( );
			
			// Deep copy of the current frame
			// NB: This should be shallow and both image and audio components should be marked
			// as read only
			current_frame = frame_type::deep_copy( cache_[ current_pos ] );

			// Seek to the current position and cache a pointer to the audio
			if(!current_frame)
				return frame_type_ptr();
			current_audio = current_frame->get_audio();
			if(!current_audio)
				return current_frame;
			
			// If the current position is not the end, cache a pointer to the audio of the next frame
			if(current_pos < get_frames() - 1)
			{
				next_audio = cache_[ current_pos + 1 ]->get_audio( );
				if(!next_audio)
					return current_frame;
			}

			//-----------------------------------------------------------------------------------
			// Check there is a context to work with, if not create one
			//-----------------------------------------------------------------------------------
			if(!context_)
			{
				current_frame->get_fps(fps_numerator_, fps_denominator_);
				input_channels_		= current_audio->channels();
				input_sample_freq_	= current_audio->frequency();
				dirty_ = true;
			}
			
			// since ffmpeg resampler will only accept 2 channels or less, force any input with more than 2 channels to conform
			if(current_audio->channels() > 2)
			{
				input_channels_ = 2;
		
				previous_audio	= audio_channel_convert(previous_audio, input_channels_);
				current_audio	= audio_channel_convert(current_audio, input_channels_);
				next_audio		= audio_channel_convert(next_audio, input_channels_);
			}
		
			acquire_values( );
			
			if(dirty_)
			{
				if(context_)
				{
					audio_resample_close(context_);
					context_ = NULL;
				}
				
				context_ = audio_resample_init(	prop_output_channels_.value<int>(), 
												input_channels_,
												prop_output_sample_freq_.value<int>(),
												input_sample_freq_ );
				
				if(!context_)
					return frame_type_ptr();
				
				dirty_ = false;
			}
		
			// If the sample frequencies are the same save some effort
			if(		(input_sample_freq_	== prop_output_sample_freq_.value<int>())
				&&	(input_channels_	== prop_output_channels_.value<int>()) )
				return current_frame;
			
			//----------------------------------------------------------------------------------
			// Prepare a buffer of input data to be passed to ffmpeg's resampler for resampling
			//----------------------------------------------------------------------------------
			int input_samples_per_chan = current_audio->samples();
			if(previous_audio)
				input_samples_per_chan += previous_audio->samples();
			if(next_audio)
				input_samples_per_chan += next_audio->samples();
			
			// Allocate buffer memory
			size_t in_buffer_size = input_channels_ * input_samples_per_chan * sizeof(short);
			if ( in_buffer_.size( ) < in_buffer_size )
				in_buffer_.resize( in_buffer_size );
			
			// Appropriately copy data from cached audio for previous, current & next frames to buffer
			int offset = 0;
			int tmp = 0;
			if(current_pos > 0)
			{
				tmp = input_channels_ * previous_audio->samples();
				memcpy((void*)&in_buffer_[ 0 ], (void*)previous_audio->data(), tmp * sizeof(short));
				offset = tmp;
			}
			
			tmp = input_channels_ * current_audio->samples();
			memcpy((void*)(&in_buffer_[ offset ]), (void*)current_audio->data(), tmp * sizeof(short));
			offset += tmp;
			
			if(current_pos < get_frames() - 1)
			{
				tmp = input_channels_ * next_audio->samples();
				memcpy((void*)(&in_buffer_[ offset ]), (void*)next_audio->data(), tmp * sizeof(short));
				offset += tmp;
			}
			
			// Determine size of output buffer - can be a little bigger than necessary, but can never be too small!
			int estimated_output_samples_per_chan = 0;
			if(input_sample_freq_ < prop_output_sample_freq_.value<int>())
				estimated_output_samples_per_chan = (int)((double(fps_denominator_)/fps_numerator_) * prop_output_sample_freq_.value<int>() * ((current_pos == 0 || current_pos >= get_frames() - 1) ? 2 : 3)) + 1;
			else
				estimated_output_samples_per_chan = (int)((input_samples_per_chan * prop_output_sample_freq_.value<int>() / input_sample_freq_) * (100 + (10 * prop_output_channels_.value<int>()))/100) + 1;
			
			// Allocate the memory
			size_t out_buffer_size = prop_output_channels_.value<int>() * estimated_output_samples_per_chan * sizeof(short);
			if ( out_buffer_.size( ) < out_buffer_size * 2 )
				out_buffer_.resize( out_buffer_size * 2 );
			
			// use ffmpeg to resample data
			audio_resample(context_, &out_buffer_[ 0 ], &in_buffer_[ 0 ], input_samples_per_chan);
			
			// Calculate the number of samples needed to pack into current frame at new sample frequency
			int output_samples = audio_samples_for_frame(	current_pos,
															prop_output_sample_freq_.value<int>(),
															fps_numerator_,
															fps_denominator_ );
		
			// Determine the in and out points for the audio of the current frame
			int	som = 0,
				eom = 0;
			
			if(current_pos > 0)
				som = audio_samples_for_frame(	current_pos - 1,
												prop_output_sample_freq_.value<int>(),
												fps_numerator_,
												fps_denominator_ );
		
			eom = som + output_samples;
		
			// Create a new audio object to hold the determined portion of the output buffer
			audio_type_ptr output_audio = audio_type_ptr(new audio_type(audio<unsigned char, pcm16>(
																			prop_output_sample_freq_.value<int>(), 
																			prop_output_channels_.value<int>(), 
																			output_samples) ) );
			if(!output_audio)
				return frame_type_ptr();
		
			// Copy data from output buffer to audio object
			memcpy(	(void*)output_audio->data(), 
					(void*)(&out_buffer_[ som * prop_output_channels_.value<int>() ]), 
					output_samples * prop_output_channels_.value<int>() * sizeof(short));
	
			// Set current frame to have this new audio object, which now holds the resampled audio data
			current_frame->set_audio(output_audio);
		
			return current_frame;
		}
	
	private:
	    class property_observer : public pcos::observer
	    {
	    public:
			property_observer(avformat_resampler_filter* filter)
	    		: filter_(filter)
			{
			}
	
			void updated(pcos::isubject*)
			{
	    		filter_->dirty_ = true;
			}
	
	    	private:
				avformat_resampler_filter* filter_;
	    };
	
		boost::shared_ptr<pcos::observer> property_observer_;
		
		pcos::property	prop_output_channels_,
						prop_output_sample_freq_;
	
		int	input_channels_,
			input_sample_freq_,
			fps_numerator_,
			fps_denominator_;
	
		ReSampleContext* 	context_;
		bool				dirty_;
	
		std::map< int, frame_type_ptr > cache_;
		std::vector< short > in_buffer_;
		std::vector< short > out_buffer_;
};

//
// Plugin object
//

class ML_PLUGIN_DECLSPEC avformat_plugin : public openmedialib_plugin
{
public:

	virtual input_type_ptr input( const opl::wstring &resource )
	{
 		typedef boost::shared_ptr< avformat_input > result_type_ptr;
		return result_type_ptr( new avformat_input( resource ) );
	}

	virtual store_type_ptr store( const opl::wstring &resource, const frame_type_ptr &frame )
	{
 		typedef boost::shared_ptr< avformat_store > result_type_ptr;
		return result_type_ptr( new avformat_store( resource, frame ) );
	}
	
	virtual filter_type_ptr filter( const opl::wstring& resource )
	{
		return filter_type_ptr( new avformat_resampler_filter( resource ) );
	}
};

} } }

extern "C" 
{
	typedef struct 
	{
		plugin::stream_handler_ptr ptr;
	}
	priv_stream_handler;

	static int oml_open( URLContext *h, const char *filename, int flags )
	{
		int result = 0;
		priv_stream_handler *priv = new priv_stream_handler;
		priv->ptr = plugin::stream_handler_fetch( opl::to_wstring( filename ), flags );
    	h->priv_data = ( void * )priv;
		if ( priv && priv->ptr )
		{
			result = priv->ptr->open( opl::to_wstring( filename ), flags ) ? 0 : -1;
    		h->is_streamed = priv->ptr->is_stream( ) ? 1 : 0;
		}
		else
		{
			result = -1;
		}
    	return result;
	}

	static int oml_read( URLContext *h, unsigned char *buf, int size )
	{
		priv_stream_handler *priv = static_cast< priv_stream_handler * >( h->priv_data );
		if ( priv && priv->ptr )
		{
			opl::string result = priv->ptr->read( size );
			size_t size = result.size( );
			for ( size_t i = 0; i < size; i ++ )
				*buf ++ = result[ i ];
			return size;
		}
		return -1;
	}

	static int oml_write( URLContext *h, unsigned char *buf, int size )
	{
		priv_stream_handler *priv = static_cast< priv_stream_handler * >( h->priv_data );
		opl::string data( ( char * )buf, size );
		if ( priv && priv->ptr )
			return priv->ptr->write( data );
		return -1;
	}

	static offset_t oml_seek( URLContext *h, offset_t pos, int whence )
	{
		priv_stream_handler *priv = static_cast< priv_stream_handler * >( h->priv_data );
		if ( priv && priv->ptr )
			return priv->ptr->seek( long( pos ), whence );
		return offset_t( -1 );
	}

	static int oml_close( URLContext *h )
	{
		priv_stream_handler *priv = static_cast< priv_stream_handler * >( h->priv_data );
		if ( priv && priv->ptr )
		{
			priv->ptr->close( );
			priv->ptr = plugin::stream_handler_ptr( );
		}
		if ( priv )
		{
			delete priv;
		}
		h->priv_data = ( void * )0;
    	return 0;
	}

	URLProtocol oml_protocol = {
    	"oml",
    	oml_open,
    	oml_read,
    	oml_write,
    	oml_seek,
    	oml_close,
	};
}

//
// Library initialisation mechanism
//

namespace
{
	void reflib( int init )
	{
		static long refs = 0;

		assert( refs >= 0 && L"avformat_plugin::refinit: refs is negative." );
		
		if( init > 0 && ++refs == 1)
		{
			av_register_all( );
			register_protocol( &oml_protocol );
			av_log_set_level( -1 );
		}
		else if( init < 0 && --refs == 0 )
		{
		}
	}
	
	boost::recursive_mutex mutex;
}

//
// Access methods for openpluginlib
//

extern "C"
{
	ML_PLUGIN_DECLSPEC bool openplugin_init( void )
	{
		boost::recursive_mutex::scoped_lock lock( mutex );
		reflib( 1 );
		return true;
	}

	ML_PLUGIN_DECLSPEC bool openplugin_uninit( void )
	{
		boost::recursive_mutex::scoped_lock lock( mutex );
		reflib( -1 );
		return true;
	}
	
	ML_PLUGIN_DECLSPEC bool openplugin_create_plugin( const char*, opl::openplugin** plug )
	{
		*plug = new plugin::avformat_plugin;
		return true;
	}
	
	ML_PLUGIN_DECLSPEC void openplugin_destroy_plugin( opl::openplugin* plug )
	{ 
		delete static_cast< plugin::avformat_plugin * >( plug ); 
	}
}

#endif
