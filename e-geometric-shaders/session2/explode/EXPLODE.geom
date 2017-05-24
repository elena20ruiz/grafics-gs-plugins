#version 330 core
        
layout(triangles) in;
layout(triangle_strip, max_vertices = 36) out;

in vec4 vfrontColor[];
in vec3 N[];
out vec4 gfrontColor;

uniform float time;
const float speed = 0.05;

uniform mat4 modelViewProjectionMatrix;

void main( void )
{

	float x,y,z;
	x = 0.0;
	y = 0.0;
	z = 0.0;	
	for( int i = 0 ; i < 3 ; i++ )
	{
		x+= N[i].x;
		y+= N[i].y;
		z+= N[i].z;
	} 

	vec4 norm = vec4(x/3,y/3,z/3,0);


	for( int i = 0 ; i < 3 ; i++ )
	{
		gfrontColor = vfrontColor[i];
		vec4 trans = speed*time*norm;
		gl_Position = modelViewProjectionMatrix*(gl_in[i].gl_Position+trans);
		EmitVertex();
	}
    EndPrimitive();
}
