uniform vec4 view_position;
uniform vec4 scale;
uniform float BoxSize;

varying vec3 vTexCoord;
varying vec3 vNormal;
varying vec3 vViewVec;

void main(void)
{
   vec4 Position = gl_Vertex.xyzw;
   
   // Get some size on the water
   Position.xy *= 1000.0;
   Position.z  = -30.0;
   

   vTexCoord   = Position.xyz * scale.xyz;
   vViewVec    = Position.xyz - vec3( -199.42, -15.1467, -1.49337 );
   vViewVec.z *= 1.0;
   vNormal     = gl_Normal;   

   gl_Position = gl_ModelViewProjectionMatrix * Position;
}
