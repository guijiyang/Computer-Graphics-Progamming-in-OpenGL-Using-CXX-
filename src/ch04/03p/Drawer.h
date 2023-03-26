/*
 * @Author: jiyang Gui
 * @Date: 2023-03-25 19:12:18
 * @LastEditors: jiyang Gui
 * @LastEditTime: 2023-03-26 15:59:16
 * @Description:
 * guijiyang@163.com
 * Copyright (c) 2023 by jiyang Gui/GuisGame, All Rights Reserved.
 */

#pragma once
#include <GLFW/glfw3.h>
#include <array>
#include <stack>
#include <gl/glew.h>
#include <glm/ext/matrix_float4x4.hpp>

namespace opengltest {
class Drawer {
  std::array<GLuint, 1> vert_arr_obj_{};
  std::array<GLuint, 3> vert_buff_obj_{};
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


private:
  void init(GLFWwindow *window);
  void display(GLFWwindow *window, double current_time);
  void setupVertices();

  static void windowReshapeCallback(GLFWwindow *window, int new_width, int new_height);

public:
  void run();
};

} // namespace opengltest