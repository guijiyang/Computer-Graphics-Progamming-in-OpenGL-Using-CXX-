#version 450

in vec4 varying_color;
out vec4 color;
uniform mat4 mv_mat;
uniform mat4 proj_mat;

void main() { color = varying_color; }