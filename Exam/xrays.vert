#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 3) in vec2 texCoord;

out vec2 vtexCoord;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;

void main()
{

    vtexCoord = texCoord;
	//ESCALAT
    vec3 p = vec3(0.5,1,1)*vertex;
    gl_Position =  modelViewProjectionMatrix * vec4(p, 1.0);

}
