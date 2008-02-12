uniform sampler3D Noise;
uniform samplerCube skyBox;

uniform float time_0_X;
uniform vec4  waterColor;
uniform float fadeExp;
uniform float fadeBias;
uniform float noiseSpeed;
uniform vec4  scale;

uniform float waveSpeed;

varying vec3 vTexCoord;
varying vec3 vNormal;
varying vec3 vViewVec;

void main(void)
{
   vec3 tcoord = vTexCoord;
   tcoord.x += waveSpeed  * time_0_X;
   tcoord.z += noiseSpeed * time_0_X;

   vec4 noisy = texture3D(Noise, tcoord);

   // Signed noise 
   vec3 bump = 2.0 * noisy.xyz - 1.0;
   bump.xy *= 0.15;
   
   // Make sure the normal always points upwards
   bump.z = 0.8 * abs(bump.z) + 0.2;
   
   // Offset the surface normal with the bump
   bump = normalize(vNormal + bump);

   // Find the reflection vector
   vec3 reflVec = reflect(vViewVec, bump);
   vec4 refl = textureCube(skyBox, reflVec.yzx);

   float lrp = 1.0 - dot(-normalize(vViewVec), bump);

   // Interpolate between the water color and reflection
   gl_FragColor = mix(waterColor, refl, clamp(fadeBias + pow(lrp, fadeExp),0.0, 1.0));
}
