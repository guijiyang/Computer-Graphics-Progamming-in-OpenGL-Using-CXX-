#version 450

in vec2 tc;
out vec4 color;

uniform mat4 mv_mat;
uniform mat4 proj_mat;
layout(binding = 0) uniform sampler2D samp;

void main() {
  color = texture(samp,tc);
}