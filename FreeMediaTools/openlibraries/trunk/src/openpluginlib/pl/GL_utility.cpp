
// openpluginlib - An plugin interface to openlibraries.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#include <openpluginlib/pl/GL_utility.hpp>

#ifdef HAVE_GL_GLEW_H

namespace olib { namespace openpluginlib {

bool pf_to_gl_format( const wstring& pf, GLint& internal_format, GLenum& format, GLenum& type )
{
	if( pf == L"dxt1" )
	{
		internal_format = -1;
		format = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
		type = GL_UNSIGNED_BYTE;
		
		return true;
	}
	else if( pf == L"dxt3" )
	{
		internal_format = -1;
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		type = GL_UNSIGNED_BYTE;
		
		return true;
	}
	else if( pf == L"dxt5" )
	{
		internal_format = -1;
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		type = GL_UNSIGNED_BYTE;
		
		return true;
	}
	else if( pf == L"b8g8r8a8" )
	{
		internal_format = GL_RGBA;
		format = GL_BGRA_EXT;
		type = GL_UNSIGNED_BYTE;

		return true;
	}
	else if( pf == L"a8b8g8r8" )
	{
		internal_format = GL_RGBA;
		format = GL_ABGR_EXT;
		type = GL_UNSIGNED_BYTE;

		return true;
	}
	else if( pf == L"b8g8r8" )
	{
		internal_format = GL_RGB;
		format = GL_BGR_EXT;
		type = GL_UNSIGNED_BYTE;

		return true;
	}
	else if( pf == L"r8g8b8" )
	{
		internal_format = GL_RGB;
		format = GL_RGB;
		type = GL_UNSIGNED_BYTE;

		return true;
	}
	else if( pf == L"r8g8b8a8" )
	{
		internal_format = GL_RGBA;
		format = GL_RGBA;
		type = GL_UNSIGNED_BYTE;

		return true;
	}
	else if( pf == L"l8" )
	{
		internal_format = GL_LUMINANCE;
		format = GL_LUMINANCE;
		type = GL_UNSIGNED_BYTE;

		return true;
	}
	else if( pf == L"l8a8" )
	{
		internal_format = GL_LUMINANCE_ALPHA;
		format = GL_LUMINANCE_ALPHA;
		type = GL_UNSIGNED_BYTE;

		return true;
	}
	else if( pf == L"r10g10b10" )
	{
		internal_format = GL_RGB10;
		format = GL_RGB;
		type = GL_UNSIGNED_SHORT;

		return true;
	}
	else if( pf == L"r12g12b12" )
	{
		internal_format = GL_RGB12;
		format = GL_RGB;
		type = GL_UNSIGNED_SHORT;

		return true;
	}
	else if( pf == L"r12g12b12a12" )
	{
		internal_format = GL_RGBA12;
		format = GL_RGBA;
		type = GL_UNSIGNED_SHORT;

		return true;
	}
	else if( pf == L"r16g16b16" )
	{
		internal_format = GL_RGB16;
		format = GL_RGB;
		type = GL_UNSIGNED_SHORT;

		return true;
	}
	else if( pf == L"r16g16b16a16" )
	{
		internal_format = GL_RGBA16;
		format = GL_RGBA;
		type = GL_UNSIGNED_SHORT;

		return true;
	}
	else if( pf == L"yuv422" )
	{
		if( GLEW_APPLE_ycbcr_422 )
		{
			internal_format = GL_RGB;
			format = GL_YCBCR_422_APPLE;
			type = GL_UNSIGNED_SHORT_8_8_APPLE;

			return true;
		}
	}
	else if( pf == L"r32g32b32f" )
	{
		if( GLEW_ARB_texture_float )
		{
			internal_format = GL_RGB32F_ARB;
			format = GL_RGB;
			type = GL_FLOAT;

			return true;
		}
	}
	else if( pf == L"r32g32b32a32f" )
	{
		if( GLEW_ARB_texture_float )
		{
			internal_format = GL_RGBA32F_ARB;
			format = GL_RGBA;
			type = GL_FLOAT;

			return true;
		}
	}
	
	return false;
}

bool is_compressed_format( const wstring& pf )
{
	return pf == L"dxt1" || pf == L"dxt3" || pf == L"dxt5";
}

bool is_yuv_format( const wstring& pf )
{
	return pf == L"yuv444" || pf == L"yuv420p";
}

bool texture_target( size_t width, size_t height, GLenum& target, float& tex_w, float& tex_h, bool force_2_0_targets )
{
	if( ( ( GLEW_VERSION_2_0 || GLEW_ARB_texture_non_power_of_two ) && force_2_0_targets ) || ( !( width & ( width - 1 ) ) ) && ( !( height & ( height - 1 ) ) ) )
	{
		target = GL_TEXTURE_2D;
		tex_w = 1.0f, tex_h = 1.0f;
		return true;
	}
	else if( GLEW_ARB_texture_rectangle )
	{
		target = GL_TEXTURE_RECTANGLE_ARB;
		tex_w = static_cast<float>( width ), tex_h = static_cast<float>( height );
		return true;
	}
	else if( GLEW_EXT_texture_rectangle )
	{
		target = GL_TEXTURE_RECTANGLE_EXT;
		tex_w = static_cast<float>( width ), tex_h = static_cast<float>( height );
		return true;
	}
	else if( GLEW_NV_texture_rectangle )
	{
		target = GL_TEXTURE_RECTANGLE_NV;
		tex_w = static_cast<float>( width ), tex_h = static_cast<float>( height );
		return true;
	}

	return false;
}

} }

#endif
