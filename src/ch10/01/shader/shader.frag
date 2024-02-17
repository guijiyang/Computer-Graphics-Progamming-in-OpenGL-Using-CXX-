#version 450

// in vec2 tc;
// in vec3 fragNormal;
in vec3 varingNormal;
in vec3 varingLightDir;
in vec3 varingVertPos;
in vec3 originalVertPos;
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

void main() {
  // add the following to perturb the imcoming normal vector
  float a=0.25; // a controls height of bumps
  float b=100.0; // b controls width of bumps
  float x=originalVertPos.x;
  float y=originalVertPos.y;
  float z=originalVertPos.z;
  vec3 n;
  n.x=varingNormal.x+a*sin(b*x); // perturb incoming normal using sine function
  n.y=varingNormal.y+a*sin(b*y);
  n.z=varingNormal.z+a*sin(b*z);
  // normalize the light,normal and view vectors
  vec3 l = normalize(varingLightDir);
  n = normalize(n);
  vec3 v = normalize(-varingVertPos);

  vec3 r = normalize(reflect(-l, n));

  vec3 ambient =
      ((globalAmbient * material.ambient) + (light.ambient *
      material.ambient))
          .xyz;
  vec3 diffuse = light.diffuse.xyz * material.diffuse.xyz * max(dot(l, n),
  0.0); vec3 specular = light.specular.xyz * material.specular.xyz *
                  pow(max(dot(v, r), 0.0), material.shininess);

  fragColor = vec4((ambient + specular + diffuse), 1.0);
}