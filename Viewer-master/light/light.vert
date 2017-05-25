#version 330 core

uniform vec4 lightAmbient;
uniform vec4 lightDiffuse;
uniform vec4 lightSpecular;
uniform vec4 lightPosition;

uniform vec4 matAmbient;
uniform vec4 matDiffuse;
uniform vec4 matSpecular;
uniform float matShininess;

uniform mat3 normalMatrix;

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in vec2 texCoord;

out vec3 pos;
out vec3 N;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 viewMatrix;

void main() {
    N = normalMatrix*normal;
    pos = (viewMatrix*vec4(vertex, 1.0)).xyz;
    gl_Position = modelViewProjectionMatrix * vec4(vertex, 1.0);
}
