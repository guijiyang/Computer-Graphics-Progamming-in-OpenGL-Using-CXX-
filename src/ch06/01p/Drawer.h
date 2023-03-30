/*
 * @Author: jiyang Gui
 * @Date: 2023-03-25 19:12:18
 * @LastEditors: jiyang Gui
 * @LastEditTime: 2023-03-30 15:47:17
 * @Description:
 * guijiyang@163.com
 * Copyright (c) 2023 by jiyang Gui/GuisGame, All Rights Reserved.
 */

#pragma once
#include <GLFW/glfw3.h>
#include <array>
#include <gl/glew.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <stack>
#include <vector>

namespace opengltest {
class Drawer {
  std::array<GLuint, 1> vert_arr_obj_{};
  std::array<GLuint, 3> vert_buf_obj_{};
  struct Position {
    float x, y, z = 0.0f;
  };
  Position cam_pos_;
  // Position cub_pos_;
  // Position tri_pos_;
  GLuint rendering_program_;

  GLuint mv_loc_;
  GLuint proj_loc_;
  GLFWwindow *p_window_;

  glm::mat4 mmat_;
  glm::mat4 vmat_;
  std::stack<glm::mat4> mv_stack_{};

  static glm::mat4 pmat_;

  uint64_t indices_nums_;

  std::vector<GLuint> texture_ids_;

private:
  void init(GLFWwindow *window);
  void display(GLFWwindow *window, double current_time);
  void setupVertices();
  void clearBuffers();

  void setupTexture();
  
void setTextureParameters();

void setAnisotropicFiltering();

void uploadTextureData(int width, int height, const void* img_data);

  // Activate the texture and bind it to a texture unit
  void activateTexture(int tex_id);
  void enableVertexAttribute(GLuint location, GLint size, GLuint vertex_obj);

  // Enable depth testing
  void enableDepthTest();

  // Draw the sphere
  void drawSphere();

  static void windowReshapeCallback(GLFWwindow *window, int new_width,
                                    int new_height);

public:
  void run();
};

} // namespace opengltest