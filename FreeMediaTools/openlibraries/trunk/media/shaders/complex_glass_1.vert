
varying vec3 vNormal;
varying vec3 vViewVec;


void main(void)
{

   // Tiny little model .. give it some size
   vec3 Pos = gl_Vertex.xyz * 1.0;

   gl_Position = gl_ModelViewProjectionMatrix * vec4(Pos,1.0);
   vNormal    = gl_Normal;
   vViewVec   = vec3(0.0,0.0,200.0) - Pos;
   vViewVec.z = vViewVec.z;
}
