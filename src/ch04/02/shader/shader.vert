#version 450
layout(location = 0) in vec3 position;

// now use separate model view matrix
uniform mat4 m_mat;
uniform mat4 v_mat;
uniform mat4 proj_mat;
uniform float tf; // time factor for animation and placement of cubes

out vec4 varying_color;

// declaration of matrix transformation utility functions
// GLSL requires functions to be declared prior to invocation
mat4 buildRotateX(float rad);
mat4 buildRotateY(float rad);
mat4 buildRotateZ(float rad);
mat4 buildTranslate(float x, float y, float z);

void main() {
  float i =
      gl_InstanceID +
      tf; // value based on time factor,but different for eah cube instance
  float a = sin(203.0 * i/8000.0) *
            403.0; // these are the x,y,z components for the translation, below
  float b = sin(301.0 * i/4001.0) * 401.0;
  float c = sin(400.0 * i/6003.0) * 405.0;
  // build the rotation and tranlation matrices to be applied to this cube's
  // model matrix
  mat4 local_rot_x = buildRotateX(1000 * i);
  mat4 local_rot_y = buildRotateY(1000 * i);
  mat4 local_rot_z = buildRotateZ(1000 * i);
  mat4 local_trans = buildTranslate(a, b, c);

  // build the model matrix and then the model-view matrix
  mat4 new_m_mat =
      m_mat * local_trans * local_rot_x * local_rot_y * local_rot_z;
  mat4 mv_mat = v_mat * new_m_mat;

  gl_Position = proj_mat * mv_mat * vec4(position, 1.0);
  varying_color = vec4(position, 1.0) + vec4(0.5, 0.5, 0.5, 0.5);
}

// utility function to build a tranlation matrix
mat4 buildTranslate(float x, float y, float z) {
  mat4 trans = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,
                    x, y, z, 1.0);
  return trans;
}

// util function to build a rotation matrix
mat4 buildRotateX(float rad) {
  mat4 rotate_x = mat4(1.0, 0.0, 0.0, 0.0, 0.0, cos(rad), -sin(rad), 0.0, 0.0,
                       sin(rad), cos(rad), 0.0, 0.0, 0.0, 0.0, 1.0);
  return rotate_x;
}

mat4 buildRotateY(float rad) {
  mat4 rotate_y = mat4(cos(rad), 0.0, sin(rad), 0.0, 0.0, 1.0, 0.0, 0.0,
                       -sin(rad), 0.0, cos(rad), 0.0, 0.0, 0.0, 0.0, 1.0);
  return rotate_y;
}

mat4 buildRotateZ(float rad) {
  mat4 rotate_z = mat4(cos(rad), -sin(rad), 0.0, 0.0, sin(rad), cos(rad), 0.0,
                       0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
  return rotate_z;
}