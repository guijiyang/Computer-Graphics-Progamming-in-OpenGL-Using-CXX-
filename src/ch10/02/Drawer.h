/*
 * @Author: jiyang Gui
 * @Date: 2023-03-28 18:41:04
 * @LastEditors: jiyang Gui
 * @LastEditTime: 2023-04-10 16:27:31
 * @Description:
 * guijiyang@163.com
 * Copyright (c) 2023 by jiyang Gui/GuisGame, All Rights Reserved.
 */
#pragma once
#include "Sphere.h"
#include <GLFW/glfw3.h>
#include <Utils.h>
#include <array>
#include <gl/glew.h>
#include <glm/ext/matrix_float4x4.hpp>


namespace opengltest {
class Drawer {
private:
  std::array<GLuint, 1> vert_arr_obj_{};
  std::array<GLuint, 5> vert_buf_obj_{};
  Position cam_pos_{0.f, 0.f, 3.f};

  glm::vec3 current_light_pos_{};
  glm::vec3 light_pos_v{};
  Position light_pos_{};

  // initial light location
  glm::vec3 initial_light_loc_{5.0f, 2.0f, 2.0f};

  // white light properties
  std::array<float, 4> global_ambient_{0.7f, 0.7f, 0.7f, 1.0f};
  std::array<float, 4> light_ambient_{0.f, 0.f, 0.f, 1.0f};
  std::array<float, 4> light_diffuse_{1.f, 1.f, 1.f, 1.f};
  std::array<float, 4> light_specular_{1.f, 1.f, 1.f, 1.f};

  // a shader program ID
  GLuint rendering_program_{};
  GLuint texture_id_{};
  // sphere used for drawing
  Sphere sphere_{};
  // model view matrix
  glm::mat4 mvmat_{};
  // // view matrix
  // static glm::mat4 vmat_;
  // // projection matrix
  // static glm::mat4 proj_mat_;

  // // window information
  // static int width_, height_;
  // static float aspect_;
  // // view project matrix inverse
  // static glm::mat4 view_proj_inv_mat_;

private:
  /**
   * @brief This function initializes the Drawer object by loading configuration
   * values from a YAML file, creating a shader program, setting up vertices,
   * textures, and matrices for rendering.
   *
   * @param window
   */
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
  /**
   * @brief Set the projection and model-view matrices
   * 
   * @param current_time 
   */
  void setMatrices(double current_time);
  void activateTexture();
  void enableVertexAttribute(GLuint location, GLint size, GLuint vertex_obj);
  void enableDepthTest();
  void drawSphere();

  // static void windowReshapeCallback(GLFWwindow *window, int new_width,
  //                                   int new_height);

  Drawer() {}

public:
  static Drawer getInstance() noexcept {
    static Drawer draw{};
    return draw;
  }

public:
  void run(const char *filename);
};

} // namespace opengltest