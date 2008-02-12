/*******************************************************************************
**
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include <stdio.h>

#include <qpaintdevice.h>

#include <gpumathlib.h>
#include "pbuffer.h"

#ifdef WIN32

#define MAX_ATTRIBS 32
#define MAX_FORMATS 32

////////////////////////////////////////////////////////////////////////////////
PBuffer::PBuffer( int width, int height, int bpp, bool isFloat, bool dBuffer, 
                 bool hasDepth, bool hasStencil, bool texture, bool share) :
            m_gl_pbuffer(0), m_rendering_context(0), m_display_context(0), 
            m_previous_rendering_context(0), m_previous_display_context(0)

{
    int              iAttribs[2*MAX_ATTRIBS];
    float            fAttribs[2*MAX_ATTRIBS];
    int              niAttribs = 0;
    int              nfAttribs = 0;
    int              pformats[MAX_FORMATS];
    unsigned int     nformat;

    HGLRC current_rendering_context = wglGetCurrentContext();
    HDC current_display_context = wglGetCurrentDC();

    //full float buffer
    niAttribs = 0;
    nfAttribs = 0;

    setWidth(width);
    setHeight(height);

    memset( iAttribs, 0, sizeof(int)*2*MAX_ATTRIBS);
    memset( fAttribs, 0, sizeof(int)*2*MAX_ATTRIBS);

    iAttribs[ niAttribs*2] = WGL_DRAW_TO_PBUFFER_ARB;
    iAttribs[ niAttribs*2 + 1] = 1;
    niAttribs++;
    iAttribs[ niAttribs*2] = WGL_PIXEL_TYPE_ARB;
    iAttribs[ niAttribs*2 + 1] = (isFloat) ? WGL_TYPE_RGBA_FLOAT_ATI : WGL_TYPE_RGBA_ARB;
    niAttribs++;
    iAttribs[ niAttribs*2] = WGL_DOUBLE_BUFFER_ARB;
    iAttribs[ niAttribs*2 + 1] = (dBuffer) ? 1 : 0;
    niAttribs++;
    iAttribs[ niAttribs*2] = WGL_SUPPORT_OPENGL_ARB;
    iAttribs[ niAttribs*2 + 1] = 1;
    niAttribs++;
    iAttribs[ niAttribs*2] = WGL_RED_BITS_ARB;
    iAttribs[ niAttribs*2 + 1] = bpp / 4;
    niAttribs++;
    iAttribs[ niAttribs*2] = WGL_GREEN_BITS_ARB;
    iAttribs[ niAttribs*2 + 1] = bpp / 4;
    niAttribs++;
    iAttribs[ niAttribs*2] = WGL_BLUE_BITS_ARB;
    iAttribs[ niAttribs*2 + 1] = bpp / 4;
    niAttribs++;
    iAttribs[ niAttribs*2] = WGL_ALPHA_BITS_ARB;
    iAttribs[ niAttribs*2 + 1] = bpp / 4;
    niAttribs++;
    iAttribs[ niAttribs*2] = WGL_COLOR_BITS_ARB;
    iAttribs[ niAttribs*2 + 1] = bpp;
    niAttribs++;

    if (hasDepth)
    {
        iAttribs[ niAttribs*2] = WGL_DEPTH_BITS_ARB;
        iAttribs[ niAttribs*2 + 1] = 24;
        niAttribs++;
    }

    if (hasStencil)
    {
        iAttribs[ niAttribs*2] = WGL_STENCIL_BITS_ARB;
        iAttribs[ niAttribs*2 + 1] = 8;
        niAttribs++;
    }

    if (texture)
    {
        iAttribs[ niAttribs*2] = WGL_BIND_TO_TEXTURE_RGBA_ARB;
        iAttribs[ niAttribs*2 + 1] = GL_TRUE;
        niAttribs++;
    }

    if (wglChoosePixelFormatARB( current_display_context, 
								 iAttribs, fAttribs, MAX_FORMATS, pformats, &nformat))
    {
        niAttribs = 0;

        if (texture)
        {
            iAttribs[niAttribs++] = WGL_TEXTURE_FORMAT_ARB;
            iAttribs[niAttribs++] = WGL_TEXTURE_RGBA_ARB;
            iAttribs[niAttribs++] = WGL_TEXTURE_TARGET_ARB;
            iAttribs[niAttribs++] = WGL_TEXTURE_2D_ARB;
        }

        iAttribs[niAttribs] = 0;
        m_gl_pbuffer = wglCreatePbufferARB( current_display_context, 
										 pformats[0], width, height, iAttribs);

        if (m_gl_pbuffer)
        {
            m_display_context = wglGetPbufferDCARB( m_gl_pbuffer);

            if (m_display_context)
            {
                m_rendering_context = wglCreateContext( m_display_context);

                if (m_rendering_context)
                {
                    if (share)
                    {
                        wglShareLists( current_rendering_context, m_rendering_context);
                    }
                }
                else //floatRC
                {
                    wglReleasePbufferDCARB( m_gl_pbuffer, m_display_context);
                    m_display_context = NULL;
                    wglDestroyPbufferARB( m_gl_pbuffer);
                    m_gl_pbuffer = NULL;
                }
            }
            else //floatDC
            {
                wglDestroyPbufferARB( m_gl_pbuffer);
                m_gl_pbuffer = NULL;
            }
        }
        else //floatBuffer
        {
            //nothing presently
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
PBuffer::~PBuffer()
{
    if (m_rendering_context)
    {
        wglDeleteContext( m_rendering_context);
        m_rendering_context = NULL;
    }

    if (m_display_context)
    {
        wglReleasePbufferDCARB( m_gl_pbuffer, m_display_context );
        m_display_context = NULL;
    }

    if (m_gl_pbuffer)
    {
        wglDestroyPbufferARB( m_gl_pbuffer);
        m_gl_pbuffer = NULL;
    }
}

////////////////////////////////////////////////////////////////////////////////
bool
 PBuffer::IsValid()
{
    return (m_rendering_context != NULL);
}

////////////////////////////////////////////////////////////////////////////////
void 
PBuffer::Bind()
{
    m_previous_display_context = wglGetCurrentDC();
    m_previous_rendering_context = wglGetCurrentContext();

    wglMakeCurrent( m_display_context, m_rendering_context);
}

////////////////////////////////////////////////////////////////////////////////
void 
PBuffer::Unbind()
{
    wglMakeCurrent( m_previous_display_context, m_previous_rendering_context);
}

////////////////////////////////////////////////////////////////////////////////
void 
PBuffer::BindAsTexture( int buffer)
{
    wglBindTexImageARB( m_gl_pbuffer, buffer);
}

////////////////////////////////////////////////////////////////////////////////
void 
PBuffer::ReleaseTexture( int buffer)
{
    wglReleaseTexImageARB( m_gl_pbuffer, buffer);
}

////////////////////////////////////////////////////////////////////////////////
void 
PBuffer::Swap()
{
    wglSwapLayerBuffers( m_display_context, WGL_SWAP_MAIN_PLANE);
}

#else

namespace
{
	bool texture_target( size_t width, size_t height, GLenum& target )
	{
		if( ( !( width & ( width - 1 ) ) ) && ( !( height & ( height - 1 ) ) ) )
		{
			target = GL_TEXTURE_2D;
			return true;
		}
		else if( GLEW_ARB_texture_rectangle )
		{
			target = GL_TEXTURE_RECTANGLE_ARB;
			return true;
		}
		else if( GLEW_EXT_texture_rectangle )
		{
			target = GL_TEXTURE_RECTANGLE_EXT;
			return true;
		}
		else if( GLEW_NV_texture_rectangle )
		{
			target = GL_TEXTURE_RECTANGLE_NV;
			return true;
		}

	return false;
	}
}

PBuffer::PBuffer( int width, int height, int, bool, bool dBuffer, bool, bool, bool, bool )
	: fb_id_( 0 )
	, rb_id_( 0 )
	, tex_id_( 0 )
	, width_( width )
	, height_( height )
	, valid_( true )
{

    setWidth(width);
    setHeight(height);

	if( GLEW_EXT_framebuffer_object )
	{
		glGenFramebuffersEXT( 1, &fb_id_ );
		glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, fb_id_ );

		getPowerOfTwoTextureSize( width, height, width, height );
		if( texture_target( width, height, target_ ) )
		{
			glGenTextures( 1, &tex_id_ );
			glBindTexture( target_, tex_id_ );
			glTexParameteri( target_, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			glTexParameteri( target_, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			glTexParameteri( target_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
			glTexParameteri( target_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
			glTexImage2D( target_, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0 );

			glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, target_, tex_id_, 0 );
		}

		if( dBuffer )
		{
			glGenRenderbuffersEXT( 1, &rb_id_ );
			glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, rb_id_ );
			glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, width, height );
			glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, rb_id_ );
		}

		GLenum status;
		if( ( status = glCheckFramebufferStatusEXT( GL_FRAMEBUFFER_EXT ) ) != GL_FRAMEBUFFER_COMPLETE_EXT )
			valid_ = false;

		glClear( GL_COLOR_BUFFER_BIT );
	
		glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
	}
	else
	{
		valid_ = false;
	}
}

PBuffer::~PBuffer()
{
	if( valid_ )
	{
		glDeleteTextures( 1, &tex_id_ );
		glDeleteRenderbuffersEXT( 1, &rb_id_ );
		glDeleteFramebuffersEXT( 1, &fb_id_ );
	}
}

bool PBuffer::IsValid()
{
    return valid_;
}

void PBuffer::Bind()
{
    if( valid_ )
	{
		glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, fb_id_ );
		glViewport( 0, 0, width_, height_ );
	}
}

void PBuffer::Unbind()
{
	if( valid_ )
		glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
}

void PBuffer::BindAsTexture( int )
{
	if( valid_ )
	{
		glEnable( target_ );
		glBindTexture( target_, tex_id_ );
	}
}

void PBuffer::ReleaseTexture( int )
{
	if( valid_ )
		glBindTexture( target_, 0 );
}

void 
PBuffer::Swap()
{
}

#endif

PBuffer* 
createRgbaPbuffer(int width, int height)
{
	int			bits_per_pixel = 32;
	bool		is_float = false;
	bool		double_buffered = false;
	bool		has_depth = false;
	bool		has_stencil = false;
	bool		render_to_texture = false;
	bool		share = true;

	PBuffer* buffer = new PBuffer(
  		width, height, bits_per_pixel, is_float, double_buffered,
		has_depth, has_stencil, render_to_texture, share);

	return buffer;
}

PBuffer* 
createAndSetRgbaPbuffer(PBuffer** pbuffer, int width, int height)
{
    PBuffer* new_pbuffer = createRgbaPbuffer(width, height);
    *pbuffer = new_pbuffer;
    return new_pbuffer;
}

