/*
 * @Author: jiyang Gui
 * @Date: 2023-03-28 18:41:04
 * @LastEditors: jiyang Gui
 * @LastEditTime: 2023-03-30 13:54:59
 * @Description: 
 * guijiyang@163.com
 * Copyright (c) 2023 by jiyang Gui/GuisGame, All Rights Reserved. 
 */
#pragma once
#include "Torus.h"
#include <Utils.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <GLFW/glfw3.h>
#include <gl/glew.h>
#include <array>

namespace opengltest {
class Drawer {
private:
  std::array<GLuint, 1> vert_arr_obj_{};
  std::array<GLuint, 4> vert_buf_obj_{};
  Position cam_pos_;
  // represent location of model view matrix in shader
  GLuint mv_loc_;
  // represent projection matrix in shader
  GLuint proj_loc_;
  // a shader program ID
  GLuint rendering_program_;
  GLuint texture_id_;
  // sphere used for drawing
  Torus torus_{};
  // projection matrix
  static glm::mat4 kProjMat;
  // model view matrix
  glm::mat4 mvmat_;
  // // view matrix
  // glm::mat4 vmat_;

private:
  void init(GLFWwindow *window);
  void display(GLFWwindow *window, double current_time);
  void setupVertices();
  GLuint setupTexture(const char *filename);

  void setTextureParameters();
  void setAnisotropicFiltering();
  void uploadTextureData(const Image &image);

  void clearBuffers();
  void setLightDirection();
  void setMatrices(double current_time);
  void activateTexture();
  void enableVertexAttribute(GLuint location, GLint size, GLuint vertex_obj);
  void enableDepthTest();
  void drawSphere();

  static void windowReshapeCallback(GLFWwindow *window, int new_width,
                                    int new_height);

public:
  void run();
};

} // namespace opengltest