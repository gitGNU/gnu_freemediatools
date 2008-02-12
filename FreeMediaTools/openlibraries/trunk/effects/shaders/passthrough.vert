
// fx - A compositing library representation.

// Copyright (C) 2005-2006 VM Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

varying vec2 v_tex;

void main( void )
{
	v_tex = vec2( gl_MultiTexCoord0 );

	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
