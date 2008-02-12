
// fx - A compositing library representation.

// Copyright (C) 2005-2006 VM Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

uniform sampler2D inColor0;
uniform float min;
uniform float max;
uniform vec4 channels;

varying vec2 v_tex;

void main( void )
{
	vec4 color = texture2D( inColor0, v_tex );
	
	float level = clamp( dot( channels, color ), 0.0, 1.0 ) * color.a;
	if( level < min || level >= max )
		discard;
	
	gl_FragColor = color;
}
