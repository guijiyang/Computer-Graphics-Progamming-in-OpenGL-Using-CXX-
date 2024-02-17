#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texcoord;
layout(location = 2) in vec3 normals;
out vec3 varingNormal;   // eye-space vertex normal
out vec3 varingLightDir; // vector pointing to the light
out vec3 varingVertPos;  // vertex position in eye space
out vec3 originalVertPos;

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

void main() {
  // output vertex position, light direction, and normal to the rasterizer for
  // interpolation
  varingVertPos = (mv_mat * vec4(position, 1.0)).xyz;
  varingLightDir = light.position - varingVertPos;
  varingNormal = (norm_matrix * vec4(normals, 1.0)).xyz;
  gl_Position = proj_mat * mv_mat * vec4(position, 1.0);
  //  include this pass-through of original vertex for interpolation
  originalVertPos=position;
}