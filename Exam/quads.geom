#version 330 core
        
layout(triangles) in;
layout(triangle_strip, max_vertices = 36) out;

in vec4 vfrontColor[];
out vec4 gfrontColor;

uniform vec3 boundingBoxMin; // cantonada minima de la capsa englobant 
uniform vec3 boundingBoxMax; // cantonada maxima de la capsa englobant


void main( void )
{

	//1. To windows
	




	//2. COPIES
	//POSICIÓ TO WINDOWS
	float w;

	//POSICIÓ ORIGINAL EN NCD
	float distx = boundingBoxMax.x - boundingBoxMin.x;
	float disty = boundingBoxMax.y - boundingBoxMin.y;
	float dis = distance(boundingBoxMax,boundingBoxMin);
	// 0,0
	for( int i = 0 ; i < 3 ; i++ )
	{
		gfrontColor = vfrontColor[i];
		//1. To windows space
		w = gl_in[i].gl_Position.w;
		vec4 pos = gl_in[i].gl_Position/w;
		pos.x = pos.x/dis + 0.5;
		pos.y = pos.y/dis + 0.5;
		gl_Position =  pos;
		//2. NCD			
		EmitVertex();
	}
    EndPrimitive();

	// 0,1
	for( int i = 0 ; i < 3 ; i++ )
	{
		gfrontColor = vfrontColor[i];
		//1. To windows space
		w = gl_in[i].gl_Position.w;
		vec4 pos = gl_in[i].gl_Position/w;
		pos.x = pos.x/dis + 0.5;
		pos.y = pos.y/dis - 0.5;
		gl_Position =  pos;
		//2. NCD			
		EmitVertex();
	}
    EndPrimitive();

	// 1,0
	for( int i = 0 ; i < 3 ; i++ )
	{
		gfrontColor = vfrontColor[i];
		//1. To windows space
		w = gl_in[i].gl_Position.w;
		vec4 pos = gl_in[i].gl_Position/w;
		pos.x = pos.x/dis -0.5;
		pos.y = pos.y/dis + 0.5;
		gl_Position = pos;  
		//2. NCD			
		EmitVertex();
	}
    EndPrimitive();

	// 1,1
	for( int i = 0 ; i < 3 ; i++ )
	{
		gfrontColor = vfrontColor[i];
		//1. To windows space
		w = gl_in[i].gl_Position.w;
		vec4 pos = gl_in[i].gl_Position/w;
		pos.x = pos.x/dis - 0.5;
		pos.y = pos.y/dis -  0.5;
		gl_Position =  pos;
		//2. NCD			
		EmitVertex();
	}
    EndPrimitive();
}

// Si va de 0 a max
// de 0 a 1
//Si 
