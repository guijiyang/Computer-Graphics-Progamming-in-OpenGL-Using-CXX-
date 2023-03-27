/*
 * @Author: jiyang Gui
 * @Date: 2023-03-25 19:12:18
 * @LastEditors: jiyang Gui
 * @LastEditTime: 2023-03-27 16:48:58
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

namespace opengltest {
class Drawer {
  std::array<GLuint, 1> vert_arr_obj_{};
  std::array<GLuint, 4> vert_buff_obj_{};
  struct Position {
    float x, y, z = 0.0f;
  };
  Position cam_pos_;
  // Position cub_pos_;
  // Position tri_pos_;
  GLuint rendering_program_;

  GLuint mv_loc_;
  GLuint proj_loc_;
  // GLuint tex_loc_;
  GLFWwindow *p_window_;

  std::array<GLuint, 2> texture_ids_;

  glm::mat4 mmat_;
  glm::mat4 vmat_;
  std::stack<glm::mat4> mv_stack_{};

  static glm::mat4 pmat_;

private:
  void init(GLFWwindow *window);
  void display(GLFWwindow *window, double current_time);
  void setupVertices();
  GLuint setupTexture(const char* filename);

  static void windowReshapeCallback(GLFWwindow *window, int new_width,
                                    int new_height);

public:
  void run();
};

} // namespace opengltest