#version 450

in vec3 varyingVertPos;
in vec3 varyingNormal;
in vec2 tc;
out vec4 color;

uniform mat4 mv_mat;
uniform mat4 proj_mat;
uniform mat4 norm_mat;
layout(binding = 0) uniform samplerCube samp;
layout(binding = 1) uniform sampler2D samp2d;

// ratio of diffuse/specular
const float lightRatio = 0.7;
void main() {
  vec3 r = -reflect(normalize(-varyingVertPos), normalize(varyingNormal));
  vec4 reflectColor = texture(samp, r);
  vec4 diffuseColor = texture(samp2d, tc);
  color = lightRatio * diffuseColor + (1.0 - lightRatio) * reflectColor;
  // color=diffuseColor;
}