#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;

out vec4 vfrontColor;

uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;

void main()
{
    vec3 N = normalize(normalMatrix * normal);
    vfrontColor = vec4(N.z);
    gl_Position = modelViewProjectionMatrix * vec4(vertex, 1.0);
}
