#version 330 core

uniform sampler2D sampler0;

in vec3 vtexCoord;
in vec3 frontColor;
in vec3 norm;
out vec4 fragColor;

void main() {
    float w = vtexCoord.z;
    if (vtexCoord.x >= -w && vtexCoord.x <= w && vtexCoord.y >= -w && vtexCoord.y <= w) {
        vec2 perspDivision =  vtexCoord.xy/w;
        vec2 realTexCoords = perspDivision*0.5 + vec2(0.5);
        
        fragColor = texture(sampler0, realTexCoords)*normalize(norm).z;
    }
    else {
        fragColor = vec4(frontColor, 1)*normalize(norm).z;
    }
}
