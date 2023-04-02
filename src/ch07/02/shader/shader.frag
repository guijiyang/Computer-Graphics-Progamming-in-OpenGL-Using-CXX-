#version 450

// in vec2 tc;
// in vec3 fragNormal;
in vec3 varingNormal;
in vec3 varingLightDir;
in vec3 varingVertPos;
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

layout(binding = 0) uniform sampler2D samp;
uniform vec4 globalAmbient;
uniform PositionLight light;
uniform Material material;

// define gamma value
const float gamma = 2.2;

void main() {
  // normalize the light,normal and view vectors
  vec3 l = normalize(varingLightDir);
  vec3 n = normalize(varingNormal);
  vec3 v = normalize(-varingVertPos);

  vec3 r = normalize(reflect(-l, n));

  vec3 ambient =
      ((globalAmbient * material.ambient) + (light.ambient *
      material.ambient))
          .xyz;
  vec3 diffuse = light.diffuse.xyz * material.diffuse.xyz * max(dot(l, n),
  0.0); vec3 specular = light.specular.xyz * material.specular.xyz *
                  pow(max(dot(v, r), 0.0), material.shininess);

  // apply gamma correction
  vec3 finalColor = pow(ambient + specular + diffuse, vec3(1.0 / gamma));
  fragColor = vec4((ambient + specular + diffuse), 1.0);
}