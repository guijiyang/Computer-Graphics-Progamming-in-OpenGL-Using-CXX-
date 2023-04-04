/*
 * @Author: jiyang Gui
 * @Date: 2023-03-28 18:41:04
 * @LastEditors: jiyang Gui
 * @LastEditTime: 2023-04-04 16:10:12
 * @Description:
 * guijiyang@163.com
 * Copyright (c) 2023 by jiyang Gui/GuisGame, All Rights Reserved.
 */
#pragma once
#include <GLFW/glfw3.h>
#include <Pyramid.h>
#include <Torus.h>
#include <Utils.h>
#include <array>
#include <gl/glew.h>
#include <glm/ext/matrix_float4x4.hpp>

namespace opengltest {
class Drawer {
private:
  std::array<GLuint, 1> vert_arr_obj_{};
  std::array<GLuint, 6> vert_buf_obj_{};

  Position cam_loc_{0.f, 0.2f, 6.f};

  // light location
  Position light_loc_{-3.8f, 2.2f, 1.1f};
  // current light position
  glm::vec3 light_position_;
  // sphere used for drawing
  Torus torus_{0.6f, 0.4f, 48};
  Position torus_loc_{1.6f, 0.f, -0.3f};

  // pyramid used for drawing
  Pyramid pyramid_{};
  Position pyramid_loc_{-1.0f, 0.1f, 0.3f};

  // light properties
  std::array<float, 4> global_ambient_{0.7f, 0.7f, 0.7f, 1.0f};
  std::array<float, 4> light_ambient_{0.0f, 0.f, 0.f, 1.0f};
  std::array<float, 4> light_diffuse_{1.f, 1.f, 1.f, 1.f};
  std::array<float, 4> light_specular_{1.f, 1.f, 1.f, 1.f};

  // a shader program ID
  GLuint rendering_program2_;
  GLuint rendering_program1_;
  GLuint texture_id_;

  // projection matrix
  static glm::mat4 kProjMat;
  // model view matrix
  glm::mat4 mvmat_;
  // view matrix
  glm::mat4 vmat_;
  glm::mat4 mmat_;

  // shadow-related variables
  static GLuint kShadowTex, kShadowBuf;
  glm::mat4 light_v_mat_;
  // glm::mat4 light_p_mat_;
  glm::mat4 shadow_mvp1_;
  glm::mat4 shadow_mvp2_;
  glm::mat4 b_;

private:
  void init(GLFWwindow *window);
  void display(GLFWwindow *window, double current_time);
  void setupVertices();
  GLuint setupTexture(const char *filename);

  void setTextureParameters();
  void setAnisotropicFiltering();
  void uploadTextureData(const Image &image);

  void clearBuffers(int clear_flags, int rotate_direction = GL_CCW);
  void setLightDirection(GLFWwindow *window);
  void installLights(glm::mat4 view_mat);
  void installGoldMaterial();
  void installBronzeMaterial();
  void enableVertexAttribute(GLuint location, GLint size, GLuint vertex_obj);
  void drawTorus();
  void drawPyramid();
  void passOne();
  void passTwo();

  static void setupShdowBuffers(GLFWwindow *window, int width, int height);
  static void windowReshapeCallback(GLFWwindow *window, int new_width,
                                    int new_height);

public:
  void run(const char *filename);
};

} // namespace opengltest