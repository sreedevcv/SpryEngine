#version 330 core 

out vec4 frag_color;
in vec4 vertColor;

void main() {
    // frag_color = vec4(0.7, 0.7, 0.8, 0.1);
    frag_color = vertColor;
}