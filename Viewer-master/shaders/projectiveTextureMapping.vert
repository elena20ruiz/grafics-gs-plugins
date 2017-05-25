#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;

out vec3 norm;
out vec3 vtexCoord;
out vec3 frontColor;

uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;

void main() {
    frontColor = color;
    norm = normalMatrix*normal;
    vec4 res = modelViewProjectionMatrix*vec4(vertex, 1);
    vtexCoord = vec3(res.xy, res.w);
    gl_Position = modelViewProjectionMatrix * vec4(vertex, 1.0);
}
