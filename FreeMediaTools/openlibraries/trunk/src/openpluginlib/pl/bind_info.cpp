
// openpluginlib - A plugin interface to openlibraries.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#include <GL/glew.h>

#include <openpluginlib/pl/bind_info.hpp>

#ifdef HAVE_GL_GLEW_H

namespace olib { namespace openpluginlib {

bind_info::~bind_info( )
{
	GLuint id = value<GLuint>( L"buffer_id" );
	if( id != 0 )
		glDeleteBuffersARB( 1, &id );
		
	id = value<GLuint>( L"tex_id" );
	if( id != 0 )
		glDeleteTextures( 1, &id );
}

} }

#endif
