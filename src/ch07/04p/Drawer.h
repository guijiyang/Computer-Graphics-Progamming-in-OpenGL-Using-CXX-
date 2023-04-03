/*
 * @Author: jiyang Gui
 * @Date: 2023-03-28 18:41:04
 * @LastEditors: jiyang Gui
 * @LastEditTime: 2023-04-03 09:42:04
 * @Description:
 * guijiyang@163.com
 * Copyright (c) 2023 by jiyang Gui/GuisGame, All Rights Reserved.
 */
#pragma once
#include "Torus.h"
#include <GLFW/glfw3.h>
#include <Utils.h>
#include <array>
#include <gl/glew.h>
#include <glm/ext/matrix_float4x4.hpp>


namespace opengltest {
class Drawer {
private:
  std::array<GLuint, 1> vert_arr_obj_{};
  std::array<GLuint, 4> vert_buf_obj_{};
  Position cam_pos_{0.f, 1.f, 5.f};

  // spot light location
  Position light_loc_{0.0f, 0.0f, 2.0f};
  // spot light direction
  Position light_direction_{0.f, 0.f, -1.f};
  // spot light cut off angle
  float spot_light_cutoff_ = 20.0f;
  // spot light falloff exponential
  float spot_light_falloff_ = 10.f;

  // spot light properties
  std::array<float, 4> global_ambient_{0.7f, 0.7f, 0.7f, 1.0f};
  std::array<float, 4> light_ambient_{0.1f, 0.f, 0.f, 1.0f};
  std::array<float, 4> light_diffuse_{1.f, 0.f, 0.f, 1.f};
  std::array<float, 4> light_specular_{1.f, 0.f, 0.f, 1.f};

  // a shader program ID
  GLuint rendering_program_;
  GLuint texture_id_;
  // sphere used for drawing
  Torus torus_{};
  // projection matrix
  static glm::mat4 kProjMat;
  // model view matrix
  glm::mat4 mvmat_;
  // view matrix
  glm::mat4 vmat_;

private:
  void init(GLFWwindow *window);
  void display(GLFWwindow *window, double current_time);
  void setupVertices();
  GLuint setupTexture(const char *filename);

  void setTextureParameters();
  void setAnisotropicFiltering();
  void uploadTextureData(const Image &image);

  void clearBuffers();
  void setLightDirection(GLFWwindow *window);
  void installLights();
  void setMatrices(double current_time);
  void activateTexture();
  void enableVertexAttribute(GLuint location, GLint size, GLuint vertex_obj);
  void enableDepthTest();
  void drawSphere();

  static void windowReshapeCallback(GLFWwindow *window, int new_width,
                                    int new_height);

public:
  void run(const char *filename);
};

} // namespace opengltest