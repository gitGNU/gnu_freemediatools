//------------------------------------------------------------------//
// Polynomial texture map lighting                                  //
//                                                                  //
// (C) Nathaniel Hoffman, 2003                                      //
//                                                                  //
// Based on 'Polynomial Texture Maps', SIGGRAPH 2001, by Tom        //
// Malzender, Dan Gelb and Hans Wolters from HP Labs                //
// Converted to OpenGL Shading Language by ATI.                     //
//------------------------------------------------------------------//

uniform vec4 light;

varying vec4 vPosition;
varying vec2 vTex;
varying vec3 vLight;

attribute vec3 rm_Tangent;
attribute vec3 rm_Binormal;

void main(void)
{
   
   vPosition = gl_ModelViewProjectionMatrix * gl_Vertex;
   gl_Position = vPosition;
   
   vTex = vec2(gl_MultiTexCoord0);
   
   vec3 lightPos     = vec3(light.x, light.y, -light.z);
   vec3 light_vector = normalize( (lightPos * gl_NormalMatrix) - vPosition.xyz);

   vLight.x = dot(light_vector, rm_Tangent.xyz);
   vLight.y = dot(light_vector, rm_Binormal.xyz);
   vLight.z = dot(light_vector, gl_Normal);   
   
}