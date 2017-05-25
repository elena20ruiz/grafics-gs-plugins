#version 330 core
uniform mat4 modelViewProjectionMatrix;

in vec3 vertex;
out vec2 texCoord;

void main() {
	texCoord = (vertex.xy+vec2(1))/2;
	gl_Position    = modelViewProjectionMatrix * vec4(vertex,1.0);
}

