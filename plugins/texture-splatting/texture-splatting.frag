#version 330 core

in vec3 vNormal;
in vec2 vtexCoord;
out vec4 fragColor;

uniform sampler2D noise0;
uniform sampler2D rock1;
uniform sampler2D grass2;

void main() {
  float f=texture(noise0, vtexCoord).r;
  vec4 rock=texture(rock1, vtexCoord);
  vec4 grass=texture(grass2, vtexCoord);
  fragColor=mix(rock, grass, fract(f))*vNormal.z;
}
