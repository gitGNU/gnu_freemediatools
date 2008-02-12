
// openpluginlib - An plugin interface to openlibraries.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef GL_UTILITY_INC_
#define GL_UTILITY_INC_

#include <openpluginlib/pl/config.hpp>
#include <openpluginlib/pl/string.hpp>

#ifdef HAVE_GL_GLEW_H

#include <GL/glew.h>

namespace olib { namespace openpluginlib {

OPENPLUGINLIB_DECLSPEC bool pf_to_gl_format( const wstring& pf, 
					     GLint& internal_format, 
					     GLenum& external_format, 
					     GLenum& type );
OPENPLUGINLIB_DECLSPEC bool	is_compressed_format( const wstring& pf );
OPENPLUGINLIB_DECLSPEC bool	is_yuv_format( const wstring& pf );
OPENPLUGINLIB_DECLSPEC bool	texture_target( size_t width, 
						size_t height, 
						GLenum& target, 
						float& tex_w, 
						float& tex_h, 
						bool force_2_0_targets = false 
						);

} }

#endif

#endif
