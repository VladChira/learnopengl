#version 330 core

out vec4 fragColor;

uniform vec3 markerColor;

void main() {
    fragColor = vec4(markerColor, 1.0);
}
