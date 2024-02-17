#version 450

in vec2 tc;
in vec3 varyingNormal;
in vec3 varyingTangent;
in vec3 varyingLightDir;
in vec3 varyingHalfVector;

out vec4 fragColor;

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
uniform PositionLight light;
uniform Material material;

layout(binding = 0) uniform sampler2D normMap;

// define gamma value
const float gamma = 2.2;

vec3 calcNewNormal() {
  vec3 normal = normalize(varyingNormal);
  vec3 tangent = normalize(varyingTangent);
  tangent = normalize(tangent - dot(tangent, normal) * normal);
  vec3 bitangent = cross(tangent, normal);
  mat3 tbn = mat3(tangent, bitangent, normal);
  vec3 retrievedNormal = texture(normMap, tc).xyz;
  retrievedNormal=retrievedNormal*2.0-1.0;
  vec3 newNormal = tbn * retrievedNormal;
  newNormal = normalize(newNormal);
  return newNormal;
}

void main() {
  // normalize the light,normal and view vectors
  vec3 l = normalize(varyingLightDir);
  vec3 n = calcNewNormal();
  // compute half vector for blinn optimization
  vec3 h = normalize(varyingHalfVector);

  // get the angle between the light and surface normal
  float cosTheta = dot(l, n);

  // angle between the view vector and reflected light
  float cosPhi = dot(h, n);

  // compute ADS contributions (per pixel)
  vec3 ambient =
      ((globalAmbient * material.ambient) + (light.ambient *
      material.ambient))
          .xyz;
  vec3 diffuse = light.diffuse.xyz * material.diffuse.xyz * max(cosTheta,
  0.0); vec3 specular = light.specular.xyz * material.specular.xyz *
                  pow(max(cosPhi, 0.0), material.shininess);

  // // apply gamma correction
  // vec3 finalColor = pow(ambient + specular + diffuse, vec3(1.0 / gamma));
  // fragColor = vec4(finalColor, 1.0);
  fragColor = vec4(ambient+diffuse+specular,1.0);
}