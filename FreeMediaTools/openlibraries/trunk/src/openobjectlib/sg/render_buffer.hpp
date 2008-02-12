
// sg - A scene graph library representation.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef RENDER_BUFFER_INC_
#define RENDER_BUFFER_INC_

#include <openobjectlib/sg/config.hpp>

#include <string>

#include <boost/shared_ptr.hpp>

#include <GL/glew.h>

#ifdef WIN32
#include <GL/wglew.h>
#endif

#include <openpluginlib/pl/GL_utility.hpp>
#include <openpluginlib/pl/string.hpp>

#include <openimagelib/il/il.hpp>

namespace oil = olib::openimagelib::il;

namespace olib { namespace openobjectlib { namespace sg {

SG_DECLSPEC bool pbuffer_verify_extensions( bool is_float );

//
class SG_DECLSPEC default_surface_format
{
public:
	explicit default_surface_format( const openpluginlib::string& options );
	
public:
	int r_bits( ) const { return r_bits_; }
	int g_bits( ) const { return g_bits_; }
	int b_bits( ) const { return b_bits_; }
	int a_bits( ) const { return a_bits_; }	

public:
	int depth_bits( )   const { return depth_bits_; }
	int stencil_bits( ) const { return stencil_bits_; }
	
public:
	bool is_float( )	 const { return float_; }
	bool doublebuffer( ) const { return doublebuffer_; }
	bool mipmap( )		 const { return mipmap_; }
	
public:
	openpluginlib::string target( ) const { return target_; }

private:
	int  r_bits_, g_bits_, b_bits_, a_bits_;
	int  depth_bits_, stencil_bits_;
	bool float_, doublebuffer_, mipmap_;
	openpluginlib::string target_;
};

// mix-in class for different buffer types.
template<class surface_format = default_surface_format>
class buffer_type : public surface_format
{
public:
	explicit buffer_type( const openpluginlib::string& options = "" )
		: surface_format( options )
		, width_( 0 )
		, height_( 0 )
		, p_max_s_( 0.0f )
		, p_max_t_( 0.0f )
		, max_s_( 0.0f )
		, max_t_( 0.0f )
	{ }

	virtual ~buffer_type( )
	{ }

public:
	virtual bool				init( int /*width*/, int /*height*/, bool /*force_GL_2_0_targets*/, int /*pw*/, int /*ph*/, bool /*share*/ )	{ return false; }
	virtual bool				init( const oil::image_type_ptr /*im*/, int /*pw*/, int /*ph*/ )				{ return false; }
	virtual void				bind_as_GL_texture( bool force_2_0_targets = false ) const = 0;
	virtual void				unbind_as_GL_texture( bool force_2_0_targets = false ) const = 0;
	virtual GLuint				get_GL_texture( ) const = 0;
	virtual oil::image_type_ptr	data( )	const = 0;
	virtual bool				initialised( ) const = 0;
	virtual void				reset( ) = 0;

public:
	virtual bool begin_render_to( bool clear,  bool force_GL_2_0_targets = false ) const = 0;
	virtual bool end_render_to( ) const	= 0;

protected:
	virtual bool verify_extensions( ) const	= 0;

public:
	bool power_of_two( ) const
	{ return ( !( width_ & ( width_ - 1 ) ) && !( height_ & ( height_ - 1 ) ) ); }

	GLenum get_GL_target( bool force_2_0_targets = false ) const
	{
		if( surface_format::target( ) == "tex2D" || surface_format::target( ) == "texRECT" )
		{
			float tex_w, tex_h;
			GLenum target;
			
			if( openpluginlib::texture_target( width( ), height( ), target, tex_w, tex_h, force_2_0_targets ) )
				return target;
		}
		else if( surface_format::target( ) == "texCUBE" )
		{
		}
		
		return 0;
	}
	
public:
	int width( )  const { return width_;  }
	int height( ) const { return height_; }

	float physical_max_s( ) const	{ return p_max_s_; }
	float physical_max_t( ) const	{ return p_max_t_; }
	float max_s( ) const			{ return max_s_; }
	float max_t( ) const			{ return max_t_; }

protected:
	int width_, height_;
	float p_max_s_, p_max_t_;
	float max_s_, max_t_;
};

//
template<class surface_format = default_surface_format>
class pbuffer : public buffer_type<surface_format>
{
public:
	explicit pbuffer( const openpluginlib::string& options = "" )
		: buffer_type<surface_format>( options )
#ifdef WIN32
		, hdc_( NULL )
		, old_hdc_( NULL )
		, hglrc_( NULL )
		, old_hglrc_( NULL )
		, pbuffer_( NULL )
#endif
		, initialised_( false )
		, clear_flags_( GL_COLOR_BUFFER_BIT )
		, tex_id_( 0 )
	{ }
	
	virtual ~pbuffer( )
	{ }

public:
	virtual bool init( int width, int height, bool force_GL_2_0_targets, int pw = -1, int ph = -1, bool share = true )
	{
		if( !verify_extensions( ) )
		{
			initialised_ = false;
			return false;
		}
	
		reset( );

		clear_flags_ = GL_COLOR_BUFFER_BIT;

		buffer_type<surface_format>::width_  = width;
		buffer_type<surface_format>::height_ = height;
	
		GLenum target = get_GL_target( force_GL_2_0_targets );
	
		std::vector<int> pf_attrs, pb_attrs;

#ifdef WIN32
		pf_attrs.push_back( WGL_DRAW_TO_PBUFFER_ARB );
		pf_attrs.push_back( 1 );
		pf_attrs.push_back( WGL_SUPPORT_OPENGL_ARB );
		pf_attrs.push_back( 1 );
		pf_attrs.push_back( WGL_RED_BITS_ARB );
		pf_attrs.push_back( r_bits( ) );
		pf_attrs.push_back( WGL_GREEN_BITS_ARB );
		pf_attrs.push_back( g_bits( ) );
		pf_attrs.push_back( WGL_BLUE_BITS_ARB );
		pf_attrs.push_back( b_bits( ) );
		pf_attrs.push_back( WGL_ALPHA_BITS_ARB );
		pf_attrs.push_back( a_bits( ) );
		pf_attrs.push_back( WGL_DEPTH_BITS_ARB );
		pf_attrs.push_back( depth_bits( ) );
		pf_attrs.push_back( WGL_STENCIL_BITS_ARB );
		pf_attrs.push_back( stencil_bits( ) );
		
		if( buffer_type<surface_format>::doublebuffer( ) )
		{
			pf_attrs.push_back( WGL_DOUBLE_BUFFER_ARB );
			pf_attrs.push_back( 1 );
		}
		
		if( buffer_type<surface_format>::is_float( ) )
		{
			if( WGLEW_NV_float_buffer )
			{
				pf_attrs.push_back( WGL_PIXEL_TYPE_ARB );
				pf_attrs.push_back( WGL_TYPE_RGBA_ARB );

				pf_attrs.push_back( WGL_FLOAT_COMPONENTS_NV );
				pf_attrs.push_back( 1 );
			}
			else if( WGLEW_ATI_pixel_format_float )
			{
				pf_attrs.push_back( WGL_PIXEL_TYPE_ARB );
				pf_attrs.push_back( WGL_TYPE_RGBA_FLOAT_ATI );
			}
			else
			{
				pf_attrs.push_back( WGL_PIXEL_TYPE_ARB );
				pf_attrs.push_back( WGL_TYPE_RGBA_ARB );
			}
		}
		else
		{
			pf_attrs.push_back( WGL_PIXEL_TYPE_ARB );
			pf_attrs.push_back( WGL_TYPE_RGBA_ARB );
		}

		pb_attrs.push_back( WGL_PBUFFER_LARGEST_ARB );
		pb_attrs.push_back( 1 );

		if( target == GL_TEXTURE_2D )
		{
			pb_attrs.push_back( WGL_TEXTURE_TARGET_ARB );
			pb_attrs.push_back( WGL_TEXTURE_2D_ARB );
		}
		else if( target == GL_TEXTURE_RECTANGLE_NV || target == GL_TEXTURE_RECTANGLE_ARB || target == GL_TEXTURE_RECTANGLE_EXT )
		{
			pb_attrs.push_back( WGL_TEXTURE_TARGET_ARB );
			if( WGLEW_ATI_render_texture_rectangle )
				pb_attrs.push_back( WGL_TEXTURE_RECTANGLE_ATI );
				
			if( WGLEW_NV_render_texture_rectangle )
				pb_attrs.push_back( WGL_TEXTURE_RECTANGLE_NV );
		}
		
		if( buffer_type<surface_format>::mipmap( ) )
		{
			pb_attrs.push_back( WGL_MIPMAP_TEXTURE_ARB );
			pb_attrs.push_back( 1 );
		}
		
		if( buffer_type<surface_format>::is_float( ) )
		{
			if( WGLEW_NV_float_buffer )
			{
				pf_attrs.push_back( WGL_BIND_TO_TEXTURE_RECTANGLE_FLOAT_RGBA_NV );
				pf_attrs.push_back( 1 );
				
				pb_attrs.push_back( WGL_TEXTURE_FORMAT_ARB );
				pb_attrs.push_back( WGL_TEXTURE_FLOAT_RGBA_NV );
			}
			else if( WGLEW_ATI_pixel_format_float )
			{
				pf_attrs.push_back( WGL_BIND_TO_TEXTURE_RGBA_ARB );
				pf_attrs.push_back( 1 );
				
				pb_attrs.push_back( WGL_TEXTURE_FORMAT_ARB );
				pb_attrs.push_back( WGL_TEXTURE_RGBA_ARB );
			}
			else
			{
				pf_attrs.push_back( WGL_BIND_TO_TEXTURE_RGBA_ARB );
				pf_attrs.push_back( 1 );
			
				pb_attrs.push_back( WGL_TEXTURE_FORMAT_ARB );
				pb_attrs.push_back( WGL_TEXTURE_RGBA_ARB );
			}
		}
		else
		{
			pf_attrs.push_back( WGL_BIND_TO_TEXTURE_RGBA_ARB );
			pf_attrs.push_back( 1 );
			
			pb_attrs.push_back( WGL_TEXTURE_FORMAT_ARB );
			pb_attrs.push_back( WGL_TEXTURE_RGBA_ARB );
		}

		pf_attrs.push_back( 0 );
		pb_attrs.push_back( 0 );
		
		HDC hdc = wglGetCurrentDC( );
		if( hdc == NULL )
			return false;
			
		HGLRC hglrc = wglGetCurrentContext( );
		if( hglrc == NULL )
			return false;
			
		int format = 0;
		unsigned int num_formats;
		if( !wglChoosePixelFormatARB( hdc, &pf_attrs[ 0 ], NULL, 1, &format, &num_formats ) || format <= 0 )
			return false;
		if( ( pbuffer_ = wglCreatePbufferARB( hdc, format, width, height, &pb_attrs[ 0 ] ) ) == NULL )
			return false;
		if( ( ( hdc_ = wglGetPbufferDCARB( pbuffer_ ) ) == NULL ) || ( ( hglrc_ = wglCreateContext( hdc_ ) ) == NULL ) )
			return false;
		if( share && !wglShareLists( hglrc, hglrc_ ) )
			return false;
			
		wglQueryPbufferARB( pbuffer_, WGL_PBUFFER_WIDTH_ARB, &width_ );
		wglQueryPbufferARB( pbuffer_, WGL_PBUFFER_HEIGHT_ARB, &height_ );
	
		initialised_ = true;

		old_hdc_   = wglGetCurrentDC( );
		old_hglrc_ = wglGetCurrentContext( );
		
		if( !wglMakeCurrent( hdc_, hglrc_ ) )
			return false;
#else
		return false;
#endif	
		glGenTextures( 1, &tex_id_ );
		glBindTexture( target, tex_id_ );
		glTexParameteri( target, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		
		if( buffer_type<surface_format>::is_float( ) )
		{
			int internal_format = 0;
			
			if( GLEW_NV_float_buffer )
				internal_format = buffer_type<surface_format>::r_bits( ) > 16 ? GL_FLOAT_RGBA32_NV : GL_FLOAT_RGBA16_NV;
			else if( GLEW_ATI_texture_float )
				internal_format = buffer_type<surface_format>::r_bits( ) > 16 ? GL_RGBA_FLOAT32_ATI : GL_RGBA_FLOAT16_ATI;
			
			glTexImage2D( target, 0, internal_format, width, height, 0, GL_RGBA, GL_FLOAT, 0 );
		}
		else
		{
			glTexImage2D( target, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0 );
		}
		
#ifdef WIN32
		wglMakeCurrent( old_hdc_, old_hglrc_ );
#endif

		buffer_type<surface_format>::p_max_s_ = target == GL_TEXTURE_2D ? 1.0f : ( pw != -1 ? pw : width );
		buffer_type<surface_format>::p_max_t_ = target == GL_TEXTURE_2D ? 1.0f : ( ph != -1 ? ph : height );
		buffer_type<surface_format>::max_s_	  = target == GL_TEXTURE_2D ? 1.0f : width;
		buffer_type<surface_format>::max_t_	  = target == GL_TEXTURE_2D ? 1.0f : height;
		
		return true;
	}

	virtual void reset( )
	{
#ifdef WIN32
		if( initialised( ) && pbuffer_ )
		{
			glDeleteTextures( 1, &tex_id_ );

			if( hglrc_ == wglGetCurrentContext( ) )
				wglMakeCurrent( 0, 0 );
				
			wglDeleteContext( hglrc_ );
			
			wglReleasePbufferDCARB( pbuffer_, hdc_ );
			wglDestroyPbufferARB( pbuffer_ );
			
			pbuffer_ = 0;
		}
#endif
	}

protected:
	virtual bool verify_extensions( ) const
	{
		return pbuffer_verify_extensions( buffer_type<surface_format>::is_float( ) );
	}

public:
	virtual bool begin_render_to( bool clear, bool force_GL_2_0_targets = false ) const
	{
#ifdef WIN32
		old_hdc_   = wglGetCurrentDC( );
		old_hglrc_ = wglGetCurrentContext( );
		
		GLenum target = get_GL_target( force_GL_2_0_targets );
		
		glBindTexture( target, get_GL_texture( ) );
		wglReleaseTexImageARB( pbuffer_, WGL_FRONT_LEFT_ARB );

		wglMakeCurrent( hdc_, hglrc_ );
#endif

		glViewport( 0, 0, buffer_type<surface_format>::width( ), buffer_type<surface_format>::height( ) );
		
		if( clear ) glClear( clear_flags_ );
	
		glMatrixMode( GL_PROJECTION );
		glPushMatrix( );
		glLoadIdentity( );
		gluOrtho2D( 0.0f, buffer_type<surface_format>::width( ), 0.0f, buffer_type<surface_format>::height( ) );
		
		glMatrixMode( GL_MODELVIEW );
		glPushMatrix( );
		glLoadIdentity( );
		
		return true;
	}
	
	virtual bool end_render_to( ) const
	{
		glMatrixMode( GL_PROJECTION );
		glPopMatrix( );
	
		glMatrixMode( GL_MODELVIEW );
		glPopMatrix( );

#ifdef WIN32
		wglMakeCurrent( old_hdc_, old_hglrc_ );
#endif

		return true;
	}

public:
	virtual void bind_as_GL_texture( bool force_GL_2_0_targets = false ) const
	{
		GLenum target = get_GL_target( force_GL_2_0_targets );
		
#ifdef WIN32
		glEnable( target );
		glBindTexture( target, get_GL_texture( ) );
		wglBindTexImageARB( pbuffer_, WGL_FRONT_LEFT_ARB );
#endif
	}
	
	virtual void unbind_as_GL_texture( bool force_GL_2_0_targets = false ) const
	{
		GLenum target = get_GL_target( force_GL_2_0_targets );
		
#ifdef WIN32
		glBindTexture( target, 0 );
		wglReleaseTexImageARB( pbuffer_, WGL_FRONT_LEFT_ARB );
		glDisable( target );
#endif
	}

public:
	virtual GLuint get_GL_texture( ) const
	{ return tex_id_; }

	virtual GLenum get_GL_target( bool force_2_0_targets = false ) const
	{ return buffer_type<surface_format>::get_GL_target( force_2_0_targets ); }
	
	virtual oil::image_type_ptr data( ) const { return oil::image_type_ptr( ); }

public:
	virtual bool initialised( ) const
	{ return initialised_; }
	
private:
#ifdef WIN32
	HDC				hdc_;
	mutable HDC		old_hdc_;
	HGLRC			hglrc_;
	mutable HGLRC	old_hglrc_;
	HPBUFFERARB		pbuffer_;
#else
#endif
	bool initialised_;
	GLenum clear_flags_;
	GLuint tex_id_;
};

//
template<class surface_format = default_surface_format>
class framebuffer_object : public buffer_type<surface_format>
{
public:
	explicit framebuffer_object( const openpluginlib::string& options = "" )
		: buffer_type<surface_format>( options )
		, fb_id_( 0 )
		, rb_id_( 0 )
		, tex_id_( 0 )
		, clear_flags_( GL_COLOR_BUFFER_BIT )
		, initialised_( false )
	{ }

	virtual ~framebuffer_object( )
	{ }

public:
	virtual bool init( int width, int height, bool env_GL_2_0_targets = false, int pw = -1, int ph = -1, bool /*share*/ = true )
	{
		if( !verify_extensions( ) )
		{
			initialised_ = false;
			return false;
		}

		reset( );

		clear_flags_ = GL_COLOR_BUFFER_BIT;

		buffer_type<surface_format>::width_  = width;
		buffer_type<surface_format>::height_ = height;

		glGenFramebuffersEXT( 1, &fb_id_ );
		glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, fb_id_ );

		GLenum target = get_GL_target( env_GL_2_0_targets );
		
		glGenTextures( 1, &tex_id_ );
		glBindTexture( target, tex_id_ );
		glTexParameteri( target, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		glTexImage2D( target, 0, 4, width, height, 0, GL_RGBA, buffer_type<surface_format>::is_float( ) ? GL_FLOAT : GL_UNSIGNED_BYTE, 0 );

		if( buffer_type<surface_format>::mipmap( ) ) glGenerateMipmapEXT( target );
		glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, target, tex_id_, 0 );

		if( buffer_type<surface_format>::doublebuffer( ) )
		{
			glGenRenderbuffersEXT( 1, &rb_id_ );
			glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, rb_id_ );
			glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, width, height );
			glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, rb_id_ );

			clear_flags_ |= GL_DEPTH_BUFFER_BIT;
		}

		GLenum status;
		if( ( status = glCheckFramebufferStatusEXT( GL_FRAMEBUFFER_EXT ) ) != GL_FRAMEBUFFER_COMPLETE_EXT )
		{
			reset( );
		
			return false;
		}

		buffer_type<surface_format>::p_max_s_ = target == GL_TEXTURE_2D ? 1.0f : ( pw != -1 ? pw : width );
		buffer_type<surface_format>::p_max_t_ = target == GL_TEXTURE_2D ? 1.0f : ( ph != -1 ? ph : height );
		buffer_type<surface_format>::max_s_	 = target == GL_TEXTURE_2D ? 1.0f : width;
		buffer_type<surface_format>::max_t_	 = target == GL_TEXTURE_2D ? 1.0f : height;

		glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );

		initialised_ = true;

		return true;
	}

	void reset( )
	{
		if( initialised( ) )
		{
			glDeleteTextures( 1, &tex_id_ );
			glDeleteRenderbuffersEXT( 1, &rb_id_ );
			glDeleteFramebuffersEXT( 1, &fb_id_ );
		
			tex_id_ = 0;
			rb_id_  = 0;
			fb_id_  = 0;
		
			initialised_ = false;
		}
	}

protected:
	virtual bool verify_extensions( ) const
	{
		bool env_force_fbo_ = false;

		// GC - Use getenv_s if using VC8.
		char* env_force_fbo = getenv( "FBO" );
		if( env_force_fbo )
			env_force_fbo_ = atoi( env_force_fbo ) != 0;

		return GLEW_EXT_framebuffer_object == GL_TRUE || env_force_fbo_;
	}

public:
	virtual bool begin_render_to( bool clear, bool = false ) const
	{
		glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, fb_id_ );
		glViewport( 0, 0, buffer_type<surface_format>::width( ), buffer_type<surface_format>::height( ) );
		
		if( clear ) glClear( clear_flags_ );
	
		glMatrixMode( GL_PROJECTION );
		glPushMatrix( );
		glLoadIdentity( );
		gluOrtho2D( 0.0f, buffer_type<surface_format>::width( ), 0.0f, buffer_type<surface_format>::height( ) );
		
		glMatrixMode( GL_MODELVIEW );
		glPushMatrix( );
		glLoadIdentity( );

		return true;
	}

	virtual bool end_render_to( ) const
	{
		glMatrixMode( GL_PROJECTION );
		glPopMatrix( );
	
		glMatrixMode( GL_MODELVIEW );
		glPopMatrix( );

		glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
		
		return true;
	}

public:
	virtual void bind_as_GL_texture( bool force_2_0_targets = false ) const
	{
		GLenum target = get_GL_target( force_2_0_targets );
		
		glEnable( target );
		glBindTexture( target, tex_id_ );
	}

	virtual void unbind_as_GL_texture( bool force_2_0_targets = false ) const
	{
		GLenum target = buffer_type<surface_format>::get_GL_target( force_2_0_targets );
		
		glBindTexture( target, 0 );
		glDisable( target );
	}
	
	virtual GLuint get_GL_texture( ) const
	{ return tex_id_; }

	virtual GLenum get_GL_target( bool force_2_0_targets = false ) const
	{ return buffer_type<surface_format>::get_GL_target( force_2_0_targets ); }
	
	virtual oil::image_type_ptr data( ) const { return oil::image_type_ptr( ); }
	
public:
	virtual bool initialised( ) const
	{ return initialised_; }
	
private:
	GLuint fb_id_, rb_id_, tex_id_;
	GLenum clear_flags_;
	bool initialised_;
};

//
template<class surface_format = default_surface_format>
class memory : public buffer_type<surface_format>
{
public:
	explicit memory( const openpluginlib::string& options = "" )
		: buffer_type<surface_format>( options )
		, id_( 0 )
		, initialised_( false )
	{ }

	virtual ~memory( )
	{ }

public:
	virtual bool init( const oil::image_type_ptr im, int pw = -1, int ph = -1 )
	{
		if( !im ) return false;

		if( !glIsTexture( id_ ) )
			glGenTextures( 1, &id_ );

		if( data_ )
		{
			old_phy_w_ = data_->width( );
			old_phy_h_ = data_->height( );
		}
		
		int width  = im->width( );
		int height = im->height( );
		
		if( width != old_phy_w_ || height != old_phy_h_ )
		{
			buffer_type<surface_format>::width_  = width;
			buffer_type<surface_format>::height_ = height;
		
			GLenum target = buffer_type<surface_format>::get_GL_target( );
		
			buffer_type<surface_format>::p_max_s_ = target == GL_TEXTURE_2D ? 1.0f : ( pw != -1 ? pw : width );
			buffer_type<surface_format>::p_max_t_ = target == GL_TEXTURE_2D ? 1.0f : ( ph != -1 ? ph : height );
			buffer_type<surface_format>::max_s_	 = target == GL_TEXTURE_2D ? 1.0f : width;
			buffer_type<surface_format>::max_t_	 = target == GL_TEXTURE_2D ? 1.0f : height;
		}
		
		data_ = im;
		initialised_ = true;

		return true;
	}

	virtual void reset( )
	{ }

protected:
	virtual bool verify_extensions( ) const
	{ return true; }

public:
	virtual bool begin_render_to( bool /*clear*/, bool /*force_GL_2_0_targets*/ = false ) const	{ return false; }
	virtual bool end_render_to( ) const	{ return false; }

	virtual void bind_as_GL_texture( bool force_2_0_targets = false ) const
	{
		GLenum target = buffer_type<surface_format>::get_GL_target( force_2_0_targets );

		typedef std::pair<GLint, GLenum> internal_and_base_format;
		GLenum type;
		internal_and_base_format pf;
		openpluginlib::pf_to_gl_format( data_->pf( ), pf.first, pf.second, type );

		glEnable( target );
		glBindTexture( target, id_ );
		
		glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );
		glTexParameteri( target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

		int phy_w = data_->width( );
		int phy_h = data_->height( );

		if( phy_w != old_phy_w_ || phy_h != old_phy_h_ )
		{
			glTexParameteri( target, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
			glTexParameteri( target, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
			glTexImage2D( target, 0, pf.first, phy_w, phy_h, 0, pf.second, type, data_->data( ) );
		}
		else
		{
			glTexSubImage2D( target, 0, 0, 0, phy_w, phy_h, pf.second, type, data_->data( ) );
		}
	
		glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	}

	virtual void unbind_as_GL_texture( bool force_2_0_targets = false ) const
	{
		GLenum target = buffer_type<surface_format>::get_GL_target( force_2_0_targets );
		
		glBindTexture( target, 0 );
		glDisable( target );
	}
	
	virtual GLuint get_GL_texture( ) const
	{ return id_; }

	virtual GLenum get_GL_target( bool force_2_0_targets = false ) const
	{ return buffer_type<surface_format>::get_GL_target( force_2_0_targets ); }
	
	virtual oil::image_type_ptr data( ) const { return data_; }

public:
	virtual bool initialised( ) const
	{ return initialised_; }
	
private:
	oil::image_type_ptr data_;
	GLuint id_;
	int old_phy_w_, old_phy_h_;
	bool initialised_;
};

//
class SG_DECLSPEC render_buffer
{
public:
	enum { NONE, FBO, PBUFFER, MEMORY };
	
public:
	explicit render_buffer( )
	{ }

	void swap( render_buffer& other )
	{ structure_.swap( other.structure_ ); }

public:
	bool init( const openpluginlib::string& options, int w, int h, bool env_GL_2_0_targets = false, int pw = -1, int ph = -1, bool share = true, int mode = NONE );
	void reset( ) { if( structure_ ) structure_->reset( ); }

public:
	bool begin_render_to( bool clear = true, bool force_GL_2_0_targets = false ) const { return structure_ ? structure_->begin_render_to( clear, force_GL_2_0_targets ) : false; }
	bool end_render_to( ) const	{ return structure_ ? structure_->end_render_to( ) : false; }
	void bind_as_GL_texture( bool force_2_0_targets = false ) const	{ if( structure_ ) structure_->bind_as_GL_texture( force_2_0_targets ); }
	void unbind_as_GL_texture( bool force_2_0_targets = false ) const { if( structure_ ) structure_->unbind_as_GL_texture( force_2_0_targets ); }
	GLuint get_GL_texture( ) const { return structure_ ? structure_->get_GL_texture( ) : false; }
	GLenum get_GL_target( bool force_2_0_targets = false ) const { return structure_ ? structure_->get_GL_target( force_2_0_targets ) : false; }
	bool initialised( ) const { return structure_ ? structure_->initialised( ) : false; }

public:
	int width( )  const { return structure_ ? structure_->width( ) : 0;  }
	int height( ) const { return structure_ ? structure_->height( ) : 0; }

	float physical_max_s( ) const { return structure_ ? structure_->physical_max_s( ) : 0.0f; }
	float physical_max_t( ) const { return structure_ ? structure_->physical_max_t( ) : 0.0f; }
	float max_s( ) const { return structure_ ? structure_->max_s( ) : 0.0f; }
	float max_t( ) const { return structure_ ? structure_->max_t( ) : 0.0f; }
	
private:
	typedef boost::shared_ptr<buffer_type<default_surface_format> > buffer_ptr;
	buffer_ptr structure_;
};

} } }

#endif
