#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in vec2 texCoord;

out vec4 frontColor;
out vec2 vtexCoord;

uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;

uniform float amplitude;
uniform float freq;
uniform float time;
const float PI = 3.1416;
 

void main()
{
    vec3 N = normalize(normalMatrix * normal);
    frontColor = vec4(N.z);
 
   //amplitude*sin(2*PI*freq*time) == angulo

   //El vertex es modifica en funcio de la normal
    vec3 v = vertex + normal*abs(amplitude*sin(2*PI*freq*time));

    gl_Position = modelViewProjectionMatrix * vec4(v,1.0);

}
