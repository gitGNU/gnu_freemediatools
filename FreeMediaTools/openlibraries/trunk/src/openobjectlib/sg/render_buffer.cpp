
// sg - An scene graph library representation.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifdef HAVE_CONFIG_H
#include <openlibraries_global_config.hpp>
#endif

#if defined ( __linux__ )
#include <GL/glxew.h>
#endif

#include <openobjectlib/sg/render_buffer.hpp>
#include <openobjectlib/sg/spirit.hpp>

namespace opl	 = olib::openpluginlib;
namespace spirit = boost::spirit;

namespace olib { namespace openobjectlib { namespace sg {

namespace
{
	struct eval_type
	{
		int  r_bits, g_bits, b_bits, a_bits;
		int  depth_bits, stencil_bits;
		bool is_float, doublebuffer, mipmap;
		std::string target;
	};
	
	class int_action
	{
	public:
		explicit int_action( int& v )
			: v_( v )
		{ }

		void operator( )( const char* first, const char* last ) const
		{ v_ = atoi( std::string( first, last ).c_str( ) ); }

	private:
		int_action& operator=( const int_action& );

		int& v_;
	};

	class bool_action
	{
	public:
		explicit bool_action( bool& v )
			: v_( v )
		{ }

		void operator( )( const char* first, const char* last ) const
		{ v_ = std::string( first, last ) == "true" ? true : false; }

	private:
		bool_action& operator=( const bool_action& );
		
		bool& v_;
	};

	class texture_action
	{
	public:
		explicit texture_action( eval_type& eval )
			: eval_( eval )
		{ }

		void operator( )( const char* first, const char* last ) const
		{ eval_.target = std::string( first, last ); }
	
	private:
		texture_action& operator=( const texture_action& );

		eval_type& eval_;
	};

	// skip grammar (whitespace)
	struct skip_grammar : public spirit::grammar<skip_grammar>
	{
		template<class ScannerT>
		struct definition
		{
			definition( const skip_grammar& /*self*/)
			{
				skip
					=	spirit::space_p
					;
					
#		ifndef NDEBUG
				BOOST_SPIRIT_DEBUG_NODE( skip );
#		endif
			}

			spirit::rule<ScannerT> skip;
		
			const spirit::rule<ScannerT>& start( ) const
			{ return skip; }
		};
	};

	struct buffer_options_grammar : public spirit::grammar<buffer_options_grammar>
	{
		buffer_options_grammar( )
		{ }

		template<class ScannerT>
		struct definition
		{
			definition( const buffer_options_grammar& self )
				: COMMA( ',' )
				, EQUAL( '=' )
			{
				using namespace boost::spirit;
				
				RGBA			= strlit<>( "rgba" );
				FLOAT			= strlit<>( "float" );
				TEX2D			= strlit<>( "tex2D" );
				TEXRECT			= strlit<>( "texRECT" );
				TEXCUBE			= strlit<>( "TEXCUBE" );
				DEPTHTEX2D		= strlit<>( "depthTex2D" );
				DEPTHTEXRECT	= strlit<>( "depthTexRECT" );
				DEPTHTEXCUBE	= strlit<>( "depthTexCUBE" );
				
				CONSTANT
					= lexeme_d[ +digit_p ]
					;
				
				texture
					= ( TEX2D | TEXRECT | TEXCUBE )[ texture_action( const_cast<eval_type&>( self.eval( ) ) ) ]
					;
				
				initial
					= RGBA >> EQUAL >> CONSTANT[ int_action( const_cast<int&>( self.eval( ).r_bits ) ) ]
						   >> COMMA >> CONSTANT[ int_action( const_cast<int&>( self.eval( ).g_bits ) ) ]
						   >> COMMA >> CONSTANT[ int_action( const_cast<int&>( self.eval( ).b_bits ) ) ]
						   >> COMMA >> CONSTANT[ int_action( const_cast<int&>( self.eval( ).a_bits ) ) ]
						   >> texture
						   >> !FLOAT[ bool_action( const_cast<bool&>( self.eval( ).is_float ) ) ]
						   >> !MIPMAP[ bool_action( const_cast<bool&>( self.eval( ).mipmap ) ) ]
						   >> !DOUBLEBUFFER[ bool_action( const_cast<bool&>( self.eval( ).doublebuffer ) ) ]
					;

#		ifndef NDEBUG
				BOOST_SPIRIT_DEBUG_NODE( CONSTANT );
				BOOST_SPIRIT_DEBUG_NODE( RGBA );
				BOOST_SPIRIT_DEBUG_NODE( FLOAT );
				BOOST_SPIRIT_DEBUG_NODE( MIPMAP );
				BOOST_SPIRIT_DEBUG_NODE( DOUBLEBUFFER );
				BOOST_SPIRIT_DEBUG_NODE( TEX2D );
				BOOST_SPIRIT_DEBUG_NODE( TEXRECT );
				BOOST_SPIRIT_DEBUG_NODE( TEXCUBE );
				BOOST_SPIRIT_DEBUG_NODE( DEPTHTEX2D );
				BOOST_SPIRIT_DEBUG_NODE( DEPTHTEXRECT );
				BOOST_SPIRIT_DEBUG_NODE( DEPTHTEXCUBE );
				
				BOOST_SPIRIT_DEBUG_NODE( initial );
				BOOST_SPIRIT_DEBUG_NODE( texture );
#		endif
			}
			
			const spirit::rule<ScannerT>& start( ) const
			{ return initial; }

			spirit::chlit<> COMMA, EQUAL;
			spirit::rule<ScannerT> CONSTANT, RGBA, FLOAT, MIPMAP, DOUBLEBUFFER;
			spirit::rule<ScannerT> TEX2D, TEXRECT, TEXCUBE, DEPTHTEX2D, DEPTHTEXRECT, DEPTHTEXCUBE;
			spirit::rule<ScannerT> initial, texture;
		};
		
		const eval_type& eval( ) const
		{ return eval_; }
		eval_type& eval( )
		{ return eval_; }
		
		eval_type eval_;
	};
}

default_surface_format::default_surface_format( const opl::string& options )
	: r_bits_( 8 )
	, g_bits_( 8 )
	, b_bits_( 8 )
	, a_bits_( 8 )
	, depth_bits_( 16 )
	, stencil_bits_( 8 )
	, float_( false )
	, doublebuffer_( false )
	, mipmap_( false )
	, target_( "tex2D" )
{
	buffer_options_grammar g;
	skip_grammar skip_g;
	
#ifndef NDEBUG
	BOOST_SPIRIT_DEBUG_NODE( g );
	BOOST_SPIRIT_DEBUG_NODE( skip_g );
#endif

	spirit::parse_info<> result = spirit::parse( options.c_str( ), g, skip_g );
	if( result.full )
	{
		r_bits_			= g.eval( ).r_bits;
		g_bits_			= g.eval( ).g_bits;
		b_bits_			= g.eval( ).b_bits;
		a_bits_			= g.eval( ).a_bits;
		depth_bits_		= g.eval( ).depth_bits;
		stencil_bits_	= g.eval( ).stencil_bits;
		float_			= g.eval( ).is_float;
		doublebuffer_	= g.eval( ).doublebuffer;
		mipmap_			= g.eval( ).mipmap;
	}
}

bool render_buffer::init( const opl::string& options, int w, int h, bool env_GL_2_0_targets, int pw, int ph, bool share, int mode )
{
	buffer_ptr buf;

	if( structure_ && structure_->initialised( ) && ( structure_->width( ) == w ) && ( structure_->height( ) == h ) )
		return true;
	
	if( mode == NONE )
	{
#ifdef WIN32
		buf = buffer_ptr( new framebuffer_object<>( options ) );
		if( !buf->init( w, h, env_GL_2_0_targets, pw, ph, share ) )
		{
			buf = buffer_ptr( new pbuffer<>( options ) );
			if( !buf->init( w, h, env_GL_2_0_targets, pw, ph, share ) )
			{
				//buf = buffer_ptr( new memory<>( options ) );
				//if( !buf->init( w, h, env_GL_2_0_targets, pw, ph, share ) )
			    return false;
			}
		}
#else
		return false;
#endif

	}
	else if( mode == FBO )
	{
		buf = buffer_ptr( new framebuffer_object<>( options ) );
		if( !buf->init( w, h, env_GL_2_0_targets, pw, ph, share ) )
			return false;
	}
	else if( mode == PBUFFER )
	{
		buf = buffer_ptr( new pbuffer<>( options ) );
		if( !buf->init( w, h, env_GL_2_0_targets, pw, ph, share ) )
			return false;
	}
	else if( mode == MEMORY )
	{
		buf = buffer_ptr( new memory<>( options ) );
		if( !buf->init( w, h, env_GL_2_0_targets, pw, ph, share ) )
			return false;
	}

	structure_ = buf;

	return true;
}

// Goncalo - Apparently Qt doesnt like X...
bool pbuffer_verify_extensions( bool is_float )
{
#ifdef WIN32
	if( !( WGLEW_ARB_pbuffer && WGLEW_ARB_pixel_format && WGLEW_ARB_render_texture ) )
		return false;

	if( is_float && !( GLEW_NV_float_buffer || ( WGLEW_ATI_pixel_format_float && GLEW_ATI_texture_float ) ) )
		return false;
			
#elif defined ( __linux__ )
	if( !( GLXEW_SGIX_pbuffer && GLXEW_SGIX_fbconfig ) )
		return false;
			
	if( is_float && !GLXEW_NV_float_buffer )
		return false;
#endif

	return true;
}

} } }
