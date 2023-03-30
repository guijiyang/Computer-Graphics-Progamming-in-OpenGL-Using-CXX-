#version 450

in vec2 tc;
in vec3 fragNormal;
out vec4 color;
layout(binding = 0) uniform sampler2D samp;
uniform mat4 mv_mat;
uniform mat4 proj_mat;
uniform vec3 lightDirection;

void main() {
  color = texture(samp, tc);
  // vec3 texColor = texture(samp, tc).rgb;
  // float diffuse = max(dot(fragNormal, lightDirection), 0.0);
  // vec3 finalColor = texColor*diffuse;
  // color = vec4(finalColor,1.0);
}