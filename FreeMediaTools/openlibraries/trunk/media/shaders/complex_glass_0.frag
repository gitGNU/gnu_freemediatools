uniform samplerCube Environment;


varying vec3 vTexCoord;

void main(void)
{

   gl_FragColor = textureCube(Environment, vTexCoord);

}