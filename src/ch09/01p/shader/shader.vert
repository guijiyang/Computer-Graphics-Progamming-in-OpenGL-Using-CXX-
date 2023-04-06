#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texcoord;
layout(location = 2) in vec3 normals;
out vec2 tc;

uniform mat4 mv_mat;
uniform mat4 proj_mat;
layout(binding = 0) uniform sampler2D samp;

void main() {
  tc=texcoord;
  gl_Position =proj_mat*mv_mat*vec4(position,1.0);
}