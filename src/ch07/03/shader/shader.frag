#version 450

// in vec2 tc;
// in vec3 fragNormal;
in vec3 varyingNormal;
in vec3 varyingLightDir;
in vec3 varyingVertPos;
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

layout(binding = 0) uniform sampler2D samp;
uniform vec4 globalAmbient;
uniform PositionLight light;
uniform Material material;

void main() {
  // normalize the light,normal and view vectors
  vec3 l = normalize(varyingLightDir);
  vec3 n = normalize(varyingNormal);
  vec3 v = normalize(-varyingVertPos);
  vec3 h = normalize(varyingHalfVector);

  // vec3 r = normalize(reflect(-l, n));

  vec3 ambient =
      ((globalAmbient * material.ambient) + (light.ambient * material.ambient))
          .xyz;
  vec3 diffuse = light.diffuse.xyz * material.diffuse.xyz * max(dot(l, n), 0.0);
  vec3 specular = light.specular.xyz * material.specular.xyz *
                  pow(max(dot(h, n), 0.0), material.shininess);

  fragColor = vec4((ambient + specular + diffuse), 1.0);
}