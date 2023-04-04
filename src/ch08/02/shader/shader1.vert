#version 450

layout(location = 0) in vec3 vertPos;
uniform mat4 shadowMVP1;

void main() { gl_Position = shadowMVP1 * vec4(vertPos, 1.0); }