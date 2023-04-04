#version 450

in vec3 varyingNormal, varyingLightDir, varyingVertPos, varyingHalfVec;
in vec4 shadowCoord;
out vec4 fragColor;

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
layout(binding = 0) uniform sampler2DShadow shTex;

void main() {
  vec3 l = normalize(varyingLightDir);
  vec3 n = normalize(varyingNormal);
  vec3 v = normalize(-varyingVertPos);
  vec3 h = normalize(varyingHalfVec);

  float notInShadow = textureProj(shTex, shadowCoord);
  fragColor =
      globalAmbient * material.ambient + light.ambient * material.ambient;
      // fragColor += light.diffuse * material.diffuse * max(dot(l, n), 0.0) +
      //            light.specular * material.specular *
      //                pow(max(dot(h, n), 0.0), material.shininess * 3.0);
  if (notInShadow == 1.0) {
    fragColor += light.diffuse * material.diffuse * max(dot(l, n), 0.0) +
                 light.specular * material.specular *
                     pow(max(dot(h, n), 0.0), material.shininess * 3.0);
  }
}