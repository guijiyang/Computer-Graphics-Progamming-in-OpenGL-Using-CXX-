#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texcoord;
layout(location = 2) in vec3 normals;
layout(location = 3) in vec3 tangents;
out vec3 varyingNormal;   // eye-space vertex normal
out vec3 varyingTangent;  // eye-space vertex tangent
out vec3 varyingLightDir; // vector pointing to the light
out vec3 varyingHalfVector; // half-vector "H" is an additional output varying
out vec2 tc;
struct PositionLight {
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  vec3 position;
};

struct Material {
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  float shininess;
};

uniform vec4 globalAmbient;
uniform mat4 mv_mat;
uniform mat4 proj_mat;
uniform mat4 norm_matrix; // for transforming normals
uniform PositionLight light;

layout(binding = 0) uniform sampler2D normMap;

void main() {
  // output vertex position, light direction, and normal to the rasterizer for
  // interpolation
  vec3 varyingVertPos = (mv_mat * vec4(position, 1.0)).xyz;
  varyingLightDir = light.position - varyingVertPos;
  varyingNormal = (norm_matrix * vec4(normals, 1.0)).xyz;
  varyingTangent = (mv_mat * vec4(tangents, 1.0)).xyz;
   // computations same as before, plus the following that computes L+V
  varyingHalfVector = (varyingLightDir + (-varyingVertPos)).xyz;
  gl_Position = proj_mat * mv_mat * vec4(position, 1.0);
  // output texcoord
  tc = texcoord;
}