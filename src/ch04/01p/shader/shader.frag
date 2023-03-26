#version 450

out vec4 color;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
in vec4 varying_color;
void main() { color = varying_color; }