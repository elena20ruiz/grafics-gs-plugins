#version 330 core

out vec4 fragColor;

in vec2 vtexCoord;

uniform sampler2D noise;
uniform sampler2D sampler0;
uniform sampler2D sampler1;

void main() {
	vec4 rockC = texture(sampler1, vtexCoord);
	vec4 noiseC = texture(noise, vtexCoord);
	vec4 grassC = texture(sampler0, vtexCoord);
    fragColor = mix(rockC, grassC, noiseC.r);
}
