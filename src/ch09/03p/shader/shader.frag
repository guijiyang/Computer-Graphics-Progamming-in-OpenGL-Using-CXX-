#version 450

in vec3 varyingVertPos;
in vec3 varyingNormal;
out vec4 color;

uniform mat4 mv_mat;
uniform mat4 proj_mat;
uniform mat4 norm_mat;
layout(binding = 0) uniform samplerCube samp;

void main() {
  vec3 r=-reflect(normalize(-varyingVertPos),normalize(varyingNormal));
  color=texture(samp,r);
}