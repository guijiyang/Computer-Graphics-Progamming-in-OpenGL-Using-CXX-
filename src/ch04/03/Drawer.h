/*
 * @Author: jiyang Gui
 * @Date: 2023-03-24 20:35:50
 * @LastEditors: jiyang Gui
 * @LastEditTime: 2023-03-24 21:19:40
 * @Description:
 * guijiyang@163.com
 * Copyright (c) 2023 by jiyang Gui/GuisGame, All Rights Reserved.
 */
#pragma once
#include <GLFW/glfw3.h>
#include <array>
#include <gl/glew.h>

namespace opengltest {
class Drawer {
  std::array<GLuint, 1> vert_arr_obj_{};
  std::array<GLuint, 2> vert_buff_obj_{};
  struct Position {
    float x, y, z = 0.0f;
  };
  Position cam_pos_;
  Position cub_pos_;
  Position tri_pos_;
  GLuint rendering_program_;

  GLuint mv_loc_;
  GLuint proj_loc_;
  GLFWwindow *p_window_;
  private:
  void init(GLFWwindow* window);
  void display(GLFWwindow* window, double current_time);
  void setupVertices();
  public:
  void run();
};
} // namespace opengltest