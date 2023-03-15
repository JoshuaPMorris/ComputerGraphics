// A simple flat-color
#version 410

out vec4 FragColor;

uniform vec4 BaseColor;

void main() {
    FragColor = BaseColor;
}