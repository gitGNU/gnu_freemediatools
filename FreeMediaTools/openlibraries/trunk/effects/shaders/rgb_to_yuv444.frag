
// fx - A compositing library representation.

// Copyright (C) 2005-2006 VM Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

uniform sampler2D inColor0;

varying vec2 v_tex;

const vec4 shift = vec4( 1.0 / 16.0, 1.0 / 128.0, 1.0 / 128.0, 0.0 );

void main( void )
{
	mat4 YUV = mat4( vec4(  0.257,  0.504,  0.098, 0.0 ),
					 vec4(  0.439, -0.368, -0.071, 0.0 ),
					 vec4( -0.148, -0.291,  0.439, 0.0 ),
					 vec4(  0.000,  0.000,  0.000, 1.0 ) );
	
	vec4 color = YUV * texture2D( inColor0, v_tex );

	gl_FragColor = color + shift;
}
