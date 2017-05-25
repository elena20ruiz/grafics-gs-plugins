#version 330 core
out vec4 fragColor;

uniform sampler2D colorMap;

void main() {
    fragColor = texture(colorMap, gl_FragCoord.xy/1024)*0.5;
}

