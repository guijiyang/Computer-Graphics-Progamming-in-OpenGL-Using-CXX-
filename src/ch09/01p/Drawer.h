/*
 * @Author: jiyang Gui
 * @Date: 2023-03-28 18:41:04
 * @LastEditors: jiyang Gui
 * @LastEditTime: 2023-04-07 09:56:11
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

// #include <memory_resource>

namespace opengltest {
class Drawer {
private:
  std::array<GLuint, 1> vert_arr_obj_{};
  std::array<GLuint, 6> vert_buf_obj_{};
  Position cam_pos_{0.f, 0.2f, 6.f};
  // camera direction,default is axis z negative.
  // Position cam_dir_u_{1.f, 0.f,0.f};
  // Position cam_dir_v_{0.f, 1.f,0.f};
  // Position cam_dir_w_{0.f, 0.f,1.f};
  glm::mat4 cam_dir_{1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f,
                     0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.0f};
  // cursor location
  double cursor_x_{0.}, cursor_y_{0.};

  // sphere used for drawing
  Torus torus_{0.6f, 0.4f, 48};
  Position torus_loc_{1.6f, 0.f, -0.3f};

  // a shader program ID
  GLuint rendering_program_;
  std::array<GLuint, 2> texture_ids_;

  // model view matrix
  glm::mat4 mvmat_;
  // view matrix
  glm::mat4 vmat_;
  // projection matrix
  static glm::mat4 kProjMat;
  // keybutton W pressed
  static bool keybutton_w_pressed_;

private:
  void init(GLFWwindow *window);
  void display(GLFWwindow *window, double current_time);
  void setupVertices();
  GLuint setupTexture(const char *filename);

  void setTextureParameters();
  void setAnisotropicFiltering();
  void uploadTextureData(const Image &image);

  void activateTexture(GLuint texture_id);
  void enableVertexAttribute(GLuint location, GLint size, GLuint vertex_obj);
  void enableDepthTest();
  void drawObject();

  void setCameraDirection(GLFWwindow *window);
  void setCameraLocation(GLFWwindow*window);

  static void windowReshapeCallback(GLFWwindow *window, int new_width,
                                    int new_height);

public:
  void run(const char *filename);
};

} // namespace opengltest