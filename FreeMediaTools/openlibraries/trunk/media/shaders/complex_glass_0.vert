varying vec3 vTexCoord;

void main(void)
{
   vec3 Position = gl_Vertex.xyz;
   
   Position.xyz +=  vec3(0.0, 0.0, 200.0);
     
   gl_Position = gl_ModelViewProjectionMatrix *  vec4(Position, 1.0);
   
   
   vTexCoord.xyz = vec3(gl_Vertex.xyz);

}