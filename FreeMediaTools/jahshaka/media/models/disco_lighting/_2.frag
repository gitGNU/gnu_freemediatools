uniform samplerCube LightCube;

varying vec3  vDir;


void main(void)
{
   // Grab our fancy shading from a cubemap
   gl_FragColor = textureCube( LightCube, vDir );

}