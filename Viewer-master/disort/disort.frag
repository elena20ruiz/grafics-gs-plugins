#version 330 core
out vec4 fragColor;
in vec2 texCoord;
uniform sampler2D colorMap;

uniform float time;

void main() {
    vec2 newST = texCoord +  0.01*sin(10.0*time + 30.0*texCoord.s);
    fragColor =  texture(colorMap, newST);
}

