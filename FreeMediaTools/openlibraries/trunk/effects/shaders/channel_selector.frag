
// fx - A compositing library representation.

// Copyright (C) 2005-2006 VM Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

uniform sampler2D inColor0;
uniform mat4 channel_selector;

varying vec2 v_tex;

void main( void )
{
	gl_FragColor = channel_selector * texture2D( inColor0, v_tex );
}
