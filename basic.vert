#version 330 core
layout (location = 0) in vec3 apos;
layout (location = 1) in vec4 aVertColor;

out vec4 vertColor;

void main() {
    // gl_Position = projection * view * model * vec4(apos, 1.0);
    gl_Position = vec4(apos, 1.0);
    // float dist = distance(gl_Position, vec4(0.0, 0.0, 0.0, 0.0));
    vertColor = vec4(aVertColor);
}