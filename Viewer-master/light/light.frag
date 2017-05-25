#version 330 core

uniform vec4 lightAmbient;
uniform vec4 lightDiffuse;
uniform vec4 lightSpecular;
uniform vec4 lightPosition;

uniform vec4 matAmbient;
uniform vec4 matDiffuse;
uniform vec4 matSpecular;
uniform float matShininess;

in vec3 pos;
in vec3 N;

out vec4 fragColor;

void main() {
    vec3 n = normalize(N);
    vec3 L = normalize(lightPosition.xyz - pos);
    vec3 H = normalize(L+vec3(0.0, 0.0, 1.0));
    fragColor = matAmbient*lightAmbient
	+ matDiffuse*lightDiffuse*max(0.0, dot(n, L))
	+ matSpecular*lightSpecular*max(0.0, pow(dot(n, H), matShininess));
}
