#version 330 core

in vec4 frontColor;
in vec2 vtexCoord;
out vec4 fragColor;

uniform sampler2D colorMap;

void main()
{
	//1. Passar a interval [-1,1]
	    float s,t;
	    s = vtexCoord.s*2-1;
	    t = vtexCoord.t*2-1;
	//2. Modificació segons r
	    vec2 Q = vec2(s,t);
	    float r = length(Q);
	    r = r + 0.22*pow(r,3) + 0.24*pow(r,5);
	    Q = r*normalize(Q);
	//3. Tornar al interval [0,1]
 	    s = (Q.x+1)/2;
	    t = (Q.y+1)/2;
	    Q = vec2(s,t);
    	if( s >= 0 && s <=1 && t >=0 && t <= 1) {
    		fragColor = texture(colorMap,Q);
	} 
	else fragColor = vec4(vec3(0.0),1.0);
	
    
}
