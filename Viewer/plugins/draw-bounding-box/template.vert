#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;

out vec4 frontColor;

uniform mat4 modelViewProjectionMatrix;
uniform vec3 boundingBoxMin;
uniform vec3 boundingBoxMax;
uniform mat3 normalMatrix;

void main() {
    vec3 N = normalize(normalMatrix * normal);
    frontColor = vec4(0.5,0.5,0.5,1.0) * N.z;
    vec3 res;
    if (vertex.x > 0) res.x = boundingBoxMax.x;
    else res.x = boundingBoxMin.x;

    if (vertex.y > 0) res.y = boundingBoxMax.y;
    else res.y = boundingBoxMin.y;

    if (vertex.z > 0) res.z = boundingBoxMax.z;
    else res.z = boundingBoxMin.z;

    gl_Position = modelViewProjectionMatrix * vec4(res, 1.0);
}
