#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 2) in vec3 color;

out vec4 vfrontColor;

uniform mat4 modelViewProjectionMatrix;

void main()
{

	vfrontColor = vec4(color,1.0);
	//El deixem en object SPACE    
	gl_Position = vec4(vertex, 1.0);
}
