#version 330 core
        
layout(triangles) in;
layout(triangle_strip, max_vertices = 36) out;

in vec4 vfrontColor[];
out float gtop;
out vec3 gnormal;
out vec2 gtexCoord;
out vec4 gfrontColor;

uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;
uniform float step=0.2;

void cube(float x, float y, float z, vec3 BT, vec3 N, bool isTop) {

	//1. Si esta arriba
	gtop = 0;	
	if(isTop) gtop = 1; 
	
	//2. Textura
    	gtexCoord = vec2(x,z);

	//3. Color: Mezcla de los tres   	
	gfrontColor = (vfrontColor[0] + vfrontColor[1] + vfrontColor[2])/3;
	
	//4. Normal: Mezcla o
    	gnormal = normalize(N);
	
	//5. Posicion respecto step
    	vec3 V = vec3(x, y, z)*step;
	
	//6. Object to Clip
   	gl_Position = modelViewProjectionMatrix*vec4(V + BT, 1);

   	EmitVertex();
}

void pintarCub(vec3 bari) {
    
    // Front
    vec3 N = normalMatrix*vec3(0, 0, -1);
    cube(0, 0, 0, bari, N, false);
    cube(1, 0, 0, bari, N, false);
    cube(0, 1, 0, bari, N, false);
    cube(1, 1, 0, bari, N, false);
    EndPrimitive();

    // Back
    N = normalMatrix*vec3(0, 0, 1);
    cube(0, 0, 1, bari, N, false);
    cube(1, 0, 1, bari, N, false);
    cube(0, 1, 1, bari, N, false);
    cube(1, 1, 1, bari, N, false);
    EndPrimitive();

    // Left
    N = normalMatrix*vec3(-1, 0, 0);
    cube(0, 0, 0, bari, N, false);
    cube(0, 0, 1, bari, N, false);
    cube(0, 1, 0, bari, N, false);
    cube(0, 1, 1, bari, N, false);
    EndPrimitive();

    // Right
    N = normalMatrix*vec3(1, 0, 0);
    cube(1, 0, 0, bari, N, false);
    cube(1, 0, 1, bari, N, false);
    cube(1, 1, 0, bari, N, false);
    cube(1, 1, 1, bari, N, false);
    EndPrimitive();

    // Top
    N = normalMatrix*vec3(0, 1, 0);
    cube(0, 1, 0, bari, N, true);
    cube(1, 1, 0, bari, N, true);
    cube(0, 1, 1, bari, N, true);
    cube(1, 1, 1, bari, N, true);
    EndPrimitive();

    // Bottom 
    N = normalMatrix*vec3(0, -1, 0);
    cube(0, 0, 0, bari, N, false);
    cube(1, 0, 0, bari, N, false);
    cube(0, 0, 1, bari, N, false);
    cube(1, 0, 1, bari, N, false);
    EndPrimitive();
}

void main( void )
{
	//Calcul del baricentre del triangle
   	 vec3 bari= (gl_in[0].gl_Position.xyz + 
             		  gl_in[1].gl_Position.xyz + 
              		  gl_in[2].gl_Position.xyz)/3;
   	bari /= step;
    	bari = step*vec3(round(bari));

	pintarCub(bari);
}
