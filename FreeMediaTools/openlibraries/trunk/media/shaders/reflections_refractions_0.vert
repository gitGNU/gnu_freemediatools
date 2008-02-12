
uniform mat4 inv_view_matrix;
uniform vec4 view_position;

varying vec3 vTexCoord;
void main(void)
{
   vec4 newPos = vec4(1.0);

   newPos.xyz =   gl_Vertex.xyz + vec3( -199.42, -15.1467, -1.49337 );
  
   gl_Position = gl_ModelViewProjectionMatrix * vec4(newPos.xyz, 1.0);
   vTexCoord = gl_Vertex.yzx;
}