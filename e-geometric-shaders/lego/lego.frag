#version 330 core


in vec4 gfrontColor;
in vec2 gtexCoord;
in float gtop;
in vec3 gnormal;
out vec4 fragColor;


const vec4 blue = vec4(0,0,1,0);
const vec4 green = vec4(0,1,0,0);
const vec4 red = vec4(1,0,0,0);
const vec4 cian = vec4(0,1,1,0);
const vec4 yellow = vec4(1,0,1,0);

uniform sampler2D lego;

float min5(float x1, float x2, float x3, float x4, float x5) {
    float min1 = min(x1, x2);
    float min2 = min(x3, x4);
    return min(min(min1, min2), x5);
}

vec4 nearestColor(vec4 color) {
    float dR = distance(color, red);
    float dG = distance(color, green);
    float dB = distance(color, blue);
    float dC = distance(color, cian);
    float dY = distance(color, yellow);
    
    float dMin = min5(dR, dG, dB, dC, dY);

    if (dMin == dR) return red;
    if (dMin == dG) return green;
    if (dMin == dB) return blue;
    if (dMin == dC) return cian;
    return yellow;
}


void main()
{

    vec4 C = nearestColor(gfrontColor);
    vec4 TC = texture2D(lego, gtexCoord);
    if (gtop > 0) fragColor = C*TC*gnormal.z;
    else fragColor = C*gnormal.z;
}

