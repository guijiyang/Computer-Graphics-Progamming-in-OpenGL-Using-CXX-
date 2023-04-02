#version 450

// in vec2 tc;
// in vec3 fragNormal;
in vec3 varingNormal;
in vec3 varingLight1Dir;
in vec3 varingLight2Dir;
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
uniform PositionLight light1;
uniform PositionLight light2;
uniform Material material;

// define gamma value
const float gamma = 2.2;

const float lightRatio = 0.35;

void main() {
  // normalize the light,normal and view vectors
  vec3 l1 = normalize(varingLight1Dir);
  vec3 l2 = normalize(varingLight2Dir);
  vec3 n = normalize(varingNormal);
  vec3 v = normalize(-varingVertPos);

  vec3 r1 = normalize(reflect(-l1, n));
  vec3 r2 = normalize(reflect(-l2, n));

  // light1,light2 have same ambient
  vec3 ambient = (globalAmbient*material.ambient+light1.ambient * material.ambient).xyz;
  vec3 diffuse1 =
      light1.diffuse.xyz * material.diffuse.xyz * max(dot(l1, n), 0.0);
  vec3 diffuse2 =
      light2.diffuse.xyz * material.diffuse.xyz * max(dot(l2, n), 0.0);
  vec3 specular1 = light1.specular.xyz * material.specular.xyz *
                   pow(max(dot(v, r1), 0.0), material.shininess);
  vec3 specular2 = light2.specular.xyz * material.specular.xyz *
                   pow(max(dot(v, r2), 0.0), material.shininess);
  
  vec3 finallight1=diffuse1+specular1;
  vec3 finallight2=diffuse2+specular2;
  // apply gamma correction
  vec3 finalColor = pow(ambient+finallight1*lightRatio+finallight2*(1-lightRatio), vec3(1.0 / gamma));
  fragColor = vec4(finalColor, 1.0);
}