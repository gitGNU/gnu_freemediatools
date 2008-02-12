uniform vec4 lightPos;
uniform vec4 view_position;
uniform float time_0_X;
uniform float rotateZ;
uniform float rotateY;
uniform float rotateX;

varying vec3  vDir;

void main(void)
{
   vec4 Pos = gl_Vertex;
   
   vec3 dir = normalize(Pos.xyz);
   Pos.xyz = 10.0 * dir + lightPos.xyz;   

   gl_Position = gl_ModelViewProjectionMatrix * Pos;

   // Create a rotation matrix from three Euler angles.
   // This work is better done on the CPU as it is constant for the
   // whole scene. In RenderMonkey we'll have to do it in the shader though.
   float cosX,sinX;
   float cosY,sinY;
   float cosZ,sinZ;

   float time_0 = 10.0;

   sinX = sin(rotateX * time_0);
   cosX = cos(rotateX * time_0);

   sinY = sin(rotateY * time_0);
   cosY = cos(rotateY * time_0);

   sinZ = sin(rotateZ * time_0);
   cosZ = cos(rotateZ * time_0);

   mat3 rotationMatrix = mat3(
      cosY * cosZ + sinX * sinY * sinZ, -cosX * sinZ,  sinX * cosY * sinZ - sinY * cosZ,
      cosY * sinZ - sinX * sinY * cosZ,  cosX * cosZ, -sinY * sinZ - sinX * cosY * cosZ,
      cosX * sinY,                       sinX,         cosX * cosY
   );

   // Rotate our fancy shading
   vDir = rotationMatrix * dir;

}
