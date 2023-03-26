#version 450

layout(location = 0) in vec3 position; // 0th vertex attribute

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

out vec4 varying_color;
void main() {
  gl_Position = proj_matrix * mv_matrix * vec4(position, 1.0);
  varying_color = vec4(position, 1.0) * 0.5 + vec4(0.5, 0.5, 0.5, 0.5);
}