
// fx - A compositing library representation.

// Copyright (C) 2005-2006 VM Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

uniform sampler2D inColor0;

varying vec2 v_tex;

void main( void )
{
	mat4 YIQ = mat4( vec4( 0.299,  0.587,  0.114, 0 ),
					 vec4( 0.596, -0.275, -0.321, 0 ),
					 vec4( 0.212, -0.523,  0.311, 0 ),
					 vec4( 0, 0, 0, 1 ) );

	mat4 YIQ_inverse = mat4( vec4( 1.0,  0.95568806036115671171,  0.61985809445637075388, 0 ),
							 vec4( 1.0, -0.27158179694405859326, -0.64687381613840131330, 0 ),
							 vec4( 1.0, -1.1081773266826619523,   1.7050645599191817149,  0 ),
							 vec4( 0, 0, 0, 1 ) );

	vec4 color = YIQ * texture2D( inColor0, v_tex );
	color.y = 0.2;
	color.z = 0.0;
	color = YIQ_inverse * color;

	gl_FragColor = color;
}
