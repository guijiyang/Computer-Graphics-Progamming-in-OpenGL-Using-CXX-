#version 450
uniform float angle;
mat4 buildRotateZ(float angle) {
  mat4 ret = mat4(cos(angle), -sin(angle), 0.0, 0.0, sin(angle), cos(angle),
                  0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
  return ret;
}

void main() {
  if (gl_VertexID == 0)
    gl_Position = buildRotateZ(angle) * vec4(0, 0.25, 0.0, 1.0);
  else if (gl_VertexID == 1)
    gl_Position = buildRotateZ(angle) * vec4(0.25, -0.25, 0.0, 1.0);
  else
    gl_Position = buildRotateZ(angle) * vec4(-0.25, -0.25, 0.0, 1.0);
}
