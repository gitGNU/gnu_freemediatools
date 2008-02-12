uniform float roomSize;
uniform vec4 lightPos;
uniform vec4 view_position;
uniform float time_0_X;
uniform float rotateZ;
uniform float rotateY;
uniform float rotateX;


varying vec2  vTexCoord;
varying vec3  vLightVec;
varying vec3  vViewVec;
varying vec3  vDir;

void main(void)
{
   // Scale and flip
   vec4 Pos = gl_Vertex;
   
     
  // vTexCoord = vec2(gl_MultiTexCoord0);

   // The model we're using doesn't contain any texture coordinates,
   // however,being a simple cube we can easily generate them ourselves.
   vTexCoord.x = 0.5 * mix(Pos.x, Pos.y, abs(gl_Normal.x));
   vTexCoord.y = 0.5 * mix(Pos.z, Pos.y, abs(gl_Normal.z));

   // No texture coordinates means no tangent space, so we'll
   // have to generate it ourselves too.
   vec3 tangent  = -vec3(abs(gl_Normal.y) + abs(gl_Normal.z), abs(gl_Normal.x), 0);
   vec3 binormal = -vec3(0, abs(gl_Normal.z), abs(gl_Normal.x) + abs(gl_Normal.y));


   Pos.xyz *= -200.0;
   gl_Position = gl_ModelViewProjectionMatrix * Pos;// vec4((gl_Vertex.xyz*roomSize),1.0);

   // Transform light vector into tangent space
   vec3 lightVec = lightPos.xyz - Pos.xyz;
   vLightVec.x = dot(lightVec, tangent);
   vLightVec.y = dot(lightVec, binormal);
   vLightVec.z = dot(lightVec, gl_Normal);

   // Transform view vector into tangent space
   vec3 viewVec = vec3( -112.6071, 146.56329, -77.20706 ) - Pos.xyz;
   vViewVec.x = dot(viewVec, tangent);
   vViewVec.y = dot(viewVec, binormal);
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
