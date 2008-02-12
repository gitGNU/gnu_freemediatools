
// fx - A compositing library representation.

// Copyright (C) 2005-2006 VM Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

uniform sampler2D inColor0;
uniform float alpha;
uniform vec2 offsets;
uniform mat4 channel_selector;

varying vec2 v_tex;

const vec4 shift = vec4( 0.0625, 0.5, 0.5, 0.0 );

void main( void )
{
	mat4 RGB = mat4( vec4( 1.1640,  1.16400, 1.164, 0.0 ),
					 vec4( 0.0000, -0.39173, 2.017, 0.0 ),
					 vec4( 1.5958, -0.81290, 0.000, 0.0 ),
					 vec4( 0.0, 0.0, 0.0, 1.0 ) );
	
	vec4 color_0 = RGB * ( texture2D( inColor0, v_tex ) - shift );
	vec4 color_1 = RGB * ( texture2D( inColor0, v_tex + offsets ) - shift );
	
	vec4 color_final = mix( color_0, color_1, 0.5 );

	gl_FragColor = channel_selector * color_final;
}
