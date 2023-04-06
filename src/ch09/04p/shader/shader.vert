#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texcoord;
layout(location = 2) in vec3 normals;
out vec3 varyingVertPos;
out vec3 varyingNormal;
out vec2 tc;
uniform mat4 mv_mat;
uniform mat4 proj_mat;
uniform mat4 norm_mat;
uniform mat4 position_mat;
layout(binding = 0) uniform samplerCube samp;
layout(binding = 1) uniform sampler2D samp2d;

void main() {
  varyingVertPos=(position_mat*vec4(position,1.0)).xyz;
  varyingNormal=(norm_mat*vec4(normals,1.0)).xyz;
  tc=texcoord;
  gl_Position =proj_mat*mv_mat*vec4(position,1.0);
}