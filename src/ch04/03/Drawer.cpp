/*
 * @Author: jiyang Gui
 * @Date: 2023-03-24 21:10:56
 * @LastEditors: jiyang Gui
 * @LastEditTime: 2023-03-25 22:42:39
 * @Description:
 * guijiyang@163.com
 * Copyright (c) 2023 by jiyang Gui/GuisGame, All Rights Reserved.
 */
#include "Drawer.h"
#include <Utils.h>
#include <stdexcept>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/matrix.hpp>

namespace opengltest {

void Drawer::setupVertices() {
  float cube_positions[108] = {
      -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,
      -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  -1.0f,
      -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, 1.0f,
      1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, 1.0f,  -1.0f,
      -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,
      1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f,
      -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f,
      1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f,
      -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,
      -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
      1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f};
  float triangle_positions[54] = {
      -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f,
      1.0f,  0.0f,  1.0f,  0.0f, // front face
      1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f,
      -1.0f, 0.0f,  1.0f,  0.0f, // right face
      1.0f,  -1.0f, -1.0f, -1.0f, -1.0f,
      -1.0f, 0.0f,  1.0f,  0.0f, // back face
      -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,
      1.0f,  0.0f,  1.0f,  0.0f, // left face
      -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f,
      1.0f,  -1.0f, -1.0f, 1.0f, // base – left front
      1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,
      -1.0f, 1.0f,  -1.0f, -1.0f // base – right back
  };

  glGenVertexArrays(static_cast<GLsizei>(vert_arr_obj_.size()),
                    vert_arr_obj_.data());
  glBindVertexArray(vert_arr_obj_[0]);

  glGenBuffers(static_cast<GLsizei>(vert_buff_obj_.size()),
               vert_buff_obj_.data());
  glBindBuffer(GL_ARRAY_BUFFER, vert_buff_obj_[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube_positions), cube_positions,
               GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, vert_buff_obj_[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_positions), triangle_positions,
               GL_STATIC_DRAW);
}

void Drawer::init(GLFWwindow *window) {
  rendering_program_ = createShaderProgram();
  // set cam position
  cam_pos_.x = cam_pos_.y = 0.0f;
  cam_pos_.z = 8.0f;
  // set cube position
  cub_pos_.x = cub_pos_.z = 0.0f;
  cub_pos_.y = -1.5f;
  // set triangle position
  tri_pos_.x = tri_pos_.y = 2.5f;
  tri_pos_.z = 0.0f;
  setupVertices();
}

void Drawer::display(GLFWwindow *window, double current_time) {
  glClear(GL_DEPTH_BUFFER_BIT);
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(rendering_program_);

  // enable cull face
  glEnable(GL_CULL_FACE);

  mv_loc_ = glGetUniformLocation(rendering_program_, "mv_mat");
  proj_loc_ = glGetUniformLocation(rendering_program_, "proj_mat");

  int width, height;
  glfwGetFramebufferSize(p_window_, &width, &height);
  auto aspect = static_cast<float>(width) / height;
  auto pmat = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 1000.f);
  auto vmat = glm::translate(glm::mat4(1.0f),
                             glm::vec3(-cam_pos_.x, -cam_pos_.y, -cam_pos_.z));

  // draw cube
  auto mmat = glm::translate(glm::mat4(1.0f),
                             glm::vec3(cub_pos_.x, cub_pos_.y, cub_pos_.z));
  auto mvmat = vmat * mmat;

  glUniformMatrix4fv(mv_loc_, 1, GL_FALSE, glm::value_ptr(mvmat));
  glUniformMatrix4fv(proj_loc_, 1, GL_FALSE, glm::value_ptr(pmat));

  //  define an array of generic vertex attribute data
  glBindBuffer(GL_ARRAY_BUFFER, vert_buff_obj_[0]);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glFrontFace(GL_CW); // set cube with clockwise direction
  glDrawArrays(GL_TRIANGLES, 0, 36);

  // draw triangle
  mmat = glm::translate(glm::mat4(1.0f),
                        glm::vec3(tri_pos_.x, tri_pos_.y, tri_pos_.z));
  mvmat = vmat * mmat;

  glUniformMatrix4fv(mv_loc_, 1, GL_FALSE, glm::value_ptr(mvmat));
  glUniformMatrix4fv(proj_loc_, 1, GL_FALSE, glm::value_ptr(pmat));

  glBindBuffer(GL_ARRAY_BUFFER, vert_buff_obj_[1]);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glFrontFace(GL_CCW); // set pyramid with counter clockwise direction
  glDrawArrays(GL_TRIANGLES, 0, 18);
}

void Drawer::run() {
  if (!glfwInit()) {
    throw std::runtime_error("Failed to initialize GLFW");
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  p_window_ = glfwCreateWindow(600, 600, "4_3", nullptr, nullptr);
  glfwMakeContextCurrent(p_window_);
  if (glewInit() != GLEW_OK) {
    throw std::runtime_error("Failed to initialize GLEW");
  }
  glfwSwapInterval(1);

  init(p_window_);

  while (!glfwWindowShouldClose(p_window_)) {
    glfwPollEvents();
    display(p_window_, glfwGetTime());
    glfwSwapBuffers(p_window_);
  }
}

} // namespace opengltest