#version 450

in vec2 tc;
in vec3 fragNormal;
in vec4 varingColor;
out vec4 color;

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

layout(binding = 0) uniform sampler2D samp;
uniform mat4 mv_mat;
uniform mat4 proj_mat;

void main() {
  color = varingColor;
}