#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texcoord;
layout(location = 2) in vec3 normals;
out vec4 varingColor;
out vec2 tc;
out vec3 fragNormal;

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
uniform mat4 mv_mat;
uniform mat4 proj_mat;
uniform mat4 norm_matrix; // for transforming normals

void main() {
  vec4 color;
  // convert vertex position to view space
  // convert normals to view space, and
  // calculate view space light vector(from vertex to light)
  vec4 p = mv_mat * vec4(position, 1.0);
  vec3 n = normalize((norm_matrix * vec4(normals, 1.0)).xyz);
  vec3 l = normalize(light.position - p.xyz);
  // view vector is equivalent to the negative of view space vertex position
  vec3 v = normalize(-p.xyz);
  // reflect
  vec3 r = reflect(-l, n);
  // ambient,diffuse,specular
  vec3 ambient =
      (globalAmbient * material.ambient + light.ambient * material.ambient).xyz;
  vec3 diffuse = light.diffuse.xyz * material.diffuse.xyz * max(dot(n, l), 0.0);
  vec3 specular = light.specular.xyz * material.specular.xyz *
                  pow(max(dot(r, v), 0.0), material.shininess);
  // send the color output to the fragment shader
  varingColor = vec4((ambient + diffuse + specular), 1.0);
  gl_Position = proj_mat * mv_mat * vec4(position, 1.0);
  tc = texcoord;
  fragNormal = normals;
}