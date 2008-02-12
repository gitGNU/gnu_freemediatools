uniform sampler2D Base2;
uniform sampler2D Bump2;
uniform samplerCube LightCube;


varying vec2  vTexCoord;
varying vec3  vLightVec;
varying vec3  vViewVec;
varying vec3  vDir;

void main(void)
{
   vec4 base = texture2D( Base2, vTexCoord );
   vec3 bump = texture2D( Bump2, vTexCoord ).xyz * 2.0 - 1.0;
   
   // Grab our fancy shading from a cubemap
   vec4 light = textureCube( LightCube, vDir );

   bump          = normalize( bump );
   vec3 lightVec = normalize( vLightVec );
   vec3 viewVec  = normalize( vViewVec );

   // Standard lighting
   float diffuse = clamp( dot( lightVec, bump ), 0.0, 1.0 );
   float specular = pow( clamp( dot(reflect( -viewVec, bump ), lightVec ) ,0.0, 1.0 ), 12.0 );

   gl_FragColor =  ( diffuse * base + 0.7 * specular ) * light + 0.3 * base;

}