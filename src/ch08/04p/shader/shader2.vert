#version 450

layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertNormal;

out vec3 varyingNormal, varyingLightDir, varyingVertPos, varyingHalfVec;
out vec4 shadowCoord;

struct PositionLight {
  vec4 ambient, diffuse, specular;
  vec3 position;
};
struct Material {
  vec4 ambient, diffuse, specular;
  float shininess;
};
uniform vec4 globalAmbient;
uniform PositionLight light;
uniform Material material;
uniform mat4 mv_mat;
uniform mat4 proj_mat;
uniform mat4 norm_mat;
uniform mat4 shadowMVP2;
uniform float shadowbufferwidth, shadowbufferheight;
layout(binding = 0) uniform sampler2DShadow shTex;

void main() {
  varyingVertPos = (mv_mat * vec4(vertPos, 1.0)).xyz;
  varyingLightDir = light.position - varyingVertPos;
  varyingNormal = (norm_mat * vec4(vertNormal, 1.0)).xyz;
  varyingHalfVec = (varyingLightDir - varyingVertPos).xyz;
  shadowCoord = shadowMVP2 * vec4(vertPos, 1.0);
  gl_Position = proj_mat * mv_mat * vec4(vertPos, 1.0);
}