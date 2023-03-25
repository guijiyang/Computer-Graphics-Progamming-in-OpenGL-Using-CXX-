#version 450
in vec4 varying_color;
uniform mat4 mv_mat;
uniform mat4 proj_mat;
out vec4 color;
void main()
{
  color=varying_color;
}