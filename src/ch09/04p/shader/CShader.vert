#version 450
layout(location = 0) in vec3 position;
out vec3 tc;

uniform mat4 v_mat;
uniform mat4 proj_mat;
layout(binding = 0) uniform samplerCube samp;

void main() {
  tc = position;
  mat4 vrot_mat = mat4(mat3(v_mat));
  gl_Position = proj_mat * vrot_mat * vec4(position, 1.0);
}