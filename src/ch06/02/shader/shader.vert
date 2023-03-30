#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texcoord;
layout(location = 2) in vec3 normals;
out vec2 tc;
out vec3 fragNormal;
uniform mat4 mv_mat;
uniform mat4 proj_mat;
uniform vec3 lightDirection;

void main() {
  gl_Position = proj_mat * mv_mat * vec4(position, 1.0);
  tc = texcoord;
  fragNormal = normals;
}