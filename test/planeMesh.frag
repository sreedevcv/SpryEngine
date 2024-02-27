#version 330 core

in vec4 shape_color;

out vec4 frag_color;

void main() {
    frag_color = shape_color;
}