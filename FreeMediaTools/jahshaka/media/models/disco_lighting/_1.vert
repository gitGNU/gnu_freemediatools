uniform vec4 lightPos;
uniform vec4 view_position;
uniform float time_0_X;
uniform float rotateZ;
uniform float rotateY;
uniform float rotateX;

attribute vec3 rm_Tangent;
attribute vec3 rm_Binormal;

varying vec2  vTexCoord;
varying vec3  vLightVec;
varying vec3  vViewVec;
varying vec3  vDir;

void main(void)
{
   
   gl_Position = ftransform();
   
   vTexCoord = vec2(gl_MultiTexCoord0);

   // Transform light vector into tangent space
   vec3 lightVec = lightPos.xyz - gl_Vertex.xyz;
   vLightVec.x = dot(lightVec, rm_Tangent);
   vLightVec.y = dot(lightVec, rm_Binormal);
   vLightVec.z = dot(lightVec, gl_Normal);

   // Transform view vector into tangent space
   vec3 viewVec = vec3( -112.6071, 146.56329, -77.20706 ) - gl_Vertex.xyz;
   vViewVec.x = dot(viewVec, rm_Tangent);
   vViewVec.y = dot(viewVec, rm_Binormal);
   vViewVec.z = dot(viewVec, gl_Normal);

   // Direction to look up our fancy shading at
   vec3 dir = -lightVec;

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
