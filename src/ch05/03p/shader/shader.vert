#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texcoord;
out vec2 tc;
uniform mat4 mv_mat;
uniform mat4 proj_mat;

void main() {
  gl_Position = proj_mat * mv_mat * vec4(position, 1.0);
  tc = texcoord;
}