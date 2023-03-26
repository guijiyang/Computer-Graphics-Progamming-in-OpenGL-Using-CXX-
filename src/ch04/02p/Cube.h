/*
 * @Author: jiyang Gui
 * @Date: 2023-03-23 20:24:18
 * @LastEditors: jiyang Gui
 * @LastEditTime: 2023-03-24 21:16:19
 * @Description:
 * guijiyang@163.com
 * Copyright (c) 2023 by jiyang Gui/GuisGame, All Rights Reserved.
 */

#pragma once
#include <GLFW/glfw3.h>
#include <array>
#include <gl/glew.h>

// #include <>
namespace opengltest {
// static const int kNumVAO = 1;
// static const int kNumVBO = 2;
class Cube {
private:
  std::array<GLuint, 1> vert_arr_obj_{};
  std::array<GLuint, 2> vert_buff_obj_{};
  struct Position {
    float x, y, z = 0.0f;
  };
  Position cam_pos_;
  Position cub_pos_;

  GLuint rendering_program_;

  GLint mv_loc_;
  GLint proj_loc_;
  GLFWwindow *p_window_{nullptr};

private:
  void setupVertices();

public:
  void init(GLFWwindow *window);
  void display(GLFWwindow *window, double current_time);
  void run();
};
} // namespace opengltest
