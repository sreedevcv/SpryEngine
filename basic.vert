#version 330 core
layout (location = 0) in vec3 apos;
layout (location = 1) in vec4 aVertColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float x_max;
uniform float y_max;
uniform float z_max;

out vec4 vertColor;

void main() {
    gl_Position = projection * view * model * vec4(apos, 1.0);
    // float dist = distance(gl_Position, vec4(0.0, 0.0, 0.0, 0.0));
    vec4 distColor = vec4(gl_Position.x / x_max, gl_Position.y / y_max, gl_Position.z / y_max, 1.0);
    vertColor = (vec4(aVertColor) + distColor);
}