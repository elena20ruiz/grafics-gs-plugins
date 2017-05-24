#version 330 core
        
layout(triangles) in;
layout(triangle_strip, max_vertices = 36) out;

uniform float step=0.2;

in vec4 vfrontColor[];
out vec4 gfrontColor;

uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;
const vec4 grey = vec4(vec3(0.8),1);

vec4 calculBar(const vec4 x1,const vec4 x2,const vec4 x3) {
	vec4 bar = (x1 + x2 + x3)/3;
	bar = vec4(round(bar))*step;
	return bar; 
}

//emet cara respecte punt
void emetre_vertex(float x, float y, float z, vec4 p) {
	
	vec3 V = vec3(x-0.5, y-0.5, z-0.5);
	gl_Position = modelViewProjectionMatrix*vec4(V+p.xyz,1);
	EmitVertex();
}

void dibuixarCub(){
	
	vec4 bari = calculBar(gl_in[0].gl_Position,
				gl_in[1].gl_Position,
				gl_in[2].gl_Position)*step;
	


	//Cara 1
	vec3 N = normalMatrix*vec3(0,0,-1);
	gfrontColor = grey*N.z;

	emetre_vertex(0,0,0,bari);
	emetre_vertex(1,0,0,bari);
	emetre_vertex(0,1,0,bari);
	emetre_vertex(1,1,0,bari);
	EndPrimitive();

	//Cara 2
	N = normalMatrix*vec3(0,0,1);
	gfrontColor = grey*N.z;
	emetre_vertex(0,0,1,bari);
	emetre_vertex(1,0,1,bari);
	emetre_vertex(0,1,1,bari);
	emetre_vertex(1,1,1,bari);
	EndPrimitive();

	//Cara 3
	N = normalMatrix*vec3(-1,0,0);
	gfrontColor = grey*N.z;
	emetre_vertex(0,0,0,bari);
	emetre_vertex(0,0,1,bari);
	emetre_vertex(0,1,0,bari);
	emetre_vertex(0,1,1,bari);
	EndPrimitive();


	//Cara 4
	N = normalMatrix*vec3(1,0,0);
	gfrontColor = grey*N.z;
	emetre_vertex(1,0,0,bari);
	emetre_vertex(1,0,1,bari);
	emetre_vertex(1,1,0,bari);
	emetre_vertex(1,1,1,bari);
	EndPrimitive();


	//Cara 5
	N = normalMatrix*vec3(0,1,0);
	gfrontColor = grey*N.z;
	emetre_vertex(0,1,0,bari);
	emetre_vertex(1,1,0,bari);
	emetre_vertex(0,1,1,bari);
	emetre_vertex(1,1,1,bari);
	EndPrimitive();


	//Cara 6
	N = normalMatrix*vec3(0,-1,0);
	gfrontColor = grey*N.z;	
	emetre_vertex(0,0,0,bari);
	emetre_vertex(1,0,0,bari);
	emetre_vertex(0,0,1,bari);
	emetre_vertex(1,0,1,bari);
	EndPrimitive();

}

void main( void )
{
	dibuixarCub();
}
