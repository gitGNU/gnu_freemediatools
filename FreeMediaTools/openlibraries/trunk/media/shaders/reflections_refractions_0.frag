uniform vec4 scale;
uniform samplerCube skyBox;

varying vec3 vTexCoord;
void main(void)
{
   gl_FragColor = textureCube( skyBox, vTexCoord);
}