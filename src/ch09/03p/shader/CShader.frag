#version 450
in vec3 tc;
out vec4 color;

uniform mat4 v_mat;
uniform mat4 proj_mat;
layout(binding = 0) uniform samplerCube samp;

void main() { color = texture(samp, tc); }