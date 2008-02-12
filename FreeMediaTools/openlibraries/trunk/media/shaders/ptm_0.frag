//------------------------------------------------------------------//
// Polynomial texture map lighting                                  //
//                                                                  //
// (C) Nathaniel Hoffman, 2003                                      //
//                                                                  //
// Based on 'Polynomial Texture Maps', SIGGRAPH 2001, by Tom        //
// Malzender, Dan Gelb and Hans Wolters from HP Labs                //
// Converted to OpenGL Shading Language by ATI.                     //
//------------------------------------------------------------------//


uniform samplerCube normalizer;
uniform sampler2D   rgb_map;
uniform sampler2D   a012_map;
uniform sampler2D   a345_map;

uniform float mode;

varying vec4 vPosition;
varying vec2 vTex;
varying vec3 vLight;

void main(void)
{

   vec3 lu2_lv2_lulv;
   vec4 c;
   vec3 a012;
   vec3 a345;

   // Normalize light direction
   vec4 nLight = vec4(normalize(vLight),1.0);// textureCube(normalizer, vLight) * 2.0 - 1.0;


   // z-extrapolation
   if (mode > 0.0 && nLight.z < 0.0)
   {
      nLight.xy = normalize(nLight.xy);
      nLight.xy *= (1.0 - nLight.z);
   }
   
   nLight.z = 1.0;

   // Prepare higher-order terms
   lu2_lv2_lulv = nLight.xyx * nLight.xyy;

   // read higher-order coeffs from texture and unbias
   a012 = vec3(texture2D(a012_map, vTex) * 2.0 - 1.0);

   // read lower-order coeffs from texture and unbias
   // (a5 isn't biased, just halved)
   a345 = vec3(texture2D(a345_map, vTex) * 2.0 - 1.0);
   a345[2] += 1.0;

   // Evaluate polynomial
   c = vec4(dot(lu2_lv2_lulv, a012) + dot(nLight.xyz, a345));

   // Multiply by rgb factor
   c = c * texture2D(rgb_map, vTex);
   
   gl_FragColor = c;
}
