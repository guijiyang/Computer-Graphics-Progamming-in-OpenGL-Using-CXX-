/*
 * @Author: jiyang Gui
 * @Date: 2023-03-24 17:57:54
 * @LastEditors: jiyang Gui
 * @LastEditTime: 2023-03-24 21:06:04
 * @Description:
 * guijiyang@163.com
 * Copyright (c) 2023 by jiyang Gui/GuisGame, All Rights Reserved.
 */
#pragma once
#include <GLFW/glfw3.h>
#include <array>
#include <gl/glew.h>

namespace opengltest {
class Cube {
  std::array<GLuint, 1> vert_arr_obj_{};
  std::array<GLuint, 2> vert_buff_obj_{};
  struct Position {
    float x, y, z = 0.0f;
  };
  Position cam_pos_;
  Position cub_pos_;

  GLuint rendering_program_;

  GLint m_loc_;
  GLint v_loc_;
  GLint tf_loc_;
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