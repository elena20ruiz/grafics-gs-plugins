uniform float time;

void main()
{
    gl_FrontColor  = vec4(gl_Normal,1.0);
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position    = gl_ModelViewProjectionMatrix * gl_Vertex;
}


