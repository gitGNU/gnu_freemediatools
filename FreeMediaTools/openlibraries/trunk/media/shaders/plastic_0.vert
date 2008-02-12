
varying vec3 vNormal;
varying vec3 vViewVec;

void main(void)
{
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

   // World-space lighting
   vNormal = gl_Normal;
   vViewVec = vec3( 0.0, 0.0, -150.0 ) - gl_Vertex.xyz;
}