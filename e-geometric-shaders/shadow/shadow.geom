#version 330 core
        
layout(triangles) in;
layout(triangle_strip, max_vertices = 36) out;

in vec4 vfrontColor[];
out vec4 gfrontColor;

const vec4 black = vec4(0);

uniform mat4 modelViewProjectionMatrix;
uniform vec3 boundingBoxMin; 


void main( void )
{
	//Per a cada triangle emet:
	//	- Un corresponent a l'orginal
	//	- Corresponent a l'ombra sense il en el plaY:
	
	for( int i = 0 ; i < 3 ; i++ )
	{
		gfrontColor = vfrontColor[i];
		gl_Position = modelViewProjectionMatrix*gl_in[i].gl_Position; //EN OBJECT SPACE
		EmitVertex();
	}
    EndPrimitive();

	for(int i = 0; i < 3; i++) {
		gfrontColor = black;
		vec4 aux = gl_in[i].gl_Position;
		aux.y = boundingBoxMin.y;
		aux = modelViewProjectionMatrix*aux;//EN CLIP SPACE
		gl_Position = aux; 
		EmitVertex();

	}
    EndPrimitive();
}
