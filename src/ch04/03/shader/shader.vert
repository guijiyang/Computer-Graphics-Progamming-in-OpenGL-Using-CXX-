#version 450

layout(location=0) in vec3 position; 

uniform mat4 mv_mat;
uniform mat4 proj_mat;
out vec4 varying_color;
void main()
{
  gl_Position=proj_mat*mv_mat*vec4(position,1.0);
  varying_color=vec4(position,1.0)*0.5+vec4(0.5,0.5,0.5,0.5);
}