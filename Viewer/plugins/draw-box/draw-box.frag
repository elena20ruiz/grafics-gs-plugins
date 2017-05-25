
#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in vec2 texCoord;

out vec3 frontColor;
out vec3 vvertex;

uniform mat4 modelViewProjectionMatrix;

uniform vec3 boundingBoxMax;
uniform vec3 boundingBoxMin;

void main() {
        frontColor = color;
        vvertex = vertex;
        vec3 pos = vertex * (boundingBoxMax-boundingBoxMin) + boundingBoxMin;
    gl_Position = modelViewProjectionMatrix * vec4(pos, 1.0);
}
