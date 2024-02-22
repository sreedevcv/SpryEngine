#version 330 core 

out vec4 frag_color;
in vec4 vertColor;

void main() {
    frag_color = vertColor;
}