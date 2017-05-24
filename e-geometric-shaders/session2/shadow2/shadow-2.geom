#version 330 core
        
layout(triangles) in;
layout(triangle_strip, max_vertices = 36) out;

in vec4 vfrontColor[];
out vec4 gfrontColor;

const vec4 black = vec4(0,0,0,1);
const vec4 cyan = vec4(0, 1, 1, 1);

uniform mat4 modelViewProjectionMatrix;
uniform vec3 boundingBoxMin; 
uniform vec3 boundingBoxMax; 


void drawRectangle(){

	//float x = boundingBoxMax.x - boundingBoxMin.x;
	//float y = boundingBoxMax.y - boundingBoxMin.y;
	//float R = sqrt(pow(x,2)+pow(y,2))/2;
	float R = distance(boundingBoxMax,boundingBoxMin)/2.0;
	gfrontColor = cyan;

	vec3 C = (boundingBoxMax+boundingBoxMin)/2;
	vec4 centre = vec4(C.x,boundingBoxMin.y-0.01,C.z,1.0);	
	//TRIANGLE STRIP

	//Vec1 =
	vec4 V1 = vec4(centre.x-R,centre.y,centre.z-R,1.0);
	gl_Position = modelViewProjectionMatrix*V1;//EN CLIP SPACE
	EmitVertex();

	vec4 V2 = vec4(centre.x+R,centre.y,centre.z-R,1.0);
	gl_Position = modelViewProjectionMatrix*V2;//EN CLIP SPACE
	EmitVertex();
	
	vec4 V3 = vec4(centre.x-R,centre.y,centre.z+R,1.0);
	gl_Position = modelViewProjectionMatrix*V3;//EN CLIP SPACE
	EmitVertex();

	vec4 V4 = vec4(centre.x+R,centre.y,centre.z+R,1.0);
	gl_Position = modelViewProjectionMatrix*V4;//EN CLIP SPACE
	EmitVertex();

	EndPrimitive();

}


void main( void )
{

	
	if(gl_PrimitiveIDIn == 0) drawRectangle();

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
		gl_Position = modelViewProjectionMatrix*aux;//EN CLIP SPACE
		EmitVertex();

	}
    	EndPrimitive();


}
