/*
 * @Author: jiyang Gui
 * @Date: 2023-03-24 17:59:41
 * @LastEditors: jiyang Gui
 * @LastEditTime: 2023-03-24 20:24:43
 * @Description:
 * guijiyang@163.com
 * Copyright (c) 2023 by jiyang Gui/GuisGame, All Rights Reserved.
 */

#include "Cube.h"
#include "GLFW/glfw3.h"
#include <Utils.h>
#include <stdexcept>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/matrix.hpp>

namespace opengltest {
void Cube::setupVertices() {
  float vertex_positions[108] = {
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
  // create VAO
  glGenVertexArrays(static_cast<GLsizei>(vert_arr_obj_.size()),
                    vert_arr_obj_.data());
  glBindVertexArray(vert_arr_obj_[0]);
  // create VBO
  glGenBuffers(static_cast<GLsizei>(vert_buff_obj_.size()),
               vert_buff_obj_.data());
  glBindBuffer(GL_ARRAY_BUFFER, vert_buff_obj_[0]);
  // copy vertices into active buffer (0th VBO in this case)
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions), vertex_positions,
               GL_STATIC_DRAW);
}

void Cube::init(GLFWwindow *window) {
  rendering_program_ = createShaderProgram();
  cam_pos_.x = 0.0f;
  cam_pos_.y = 0.0f;
  cam_pos_.z = 420.0f;
  cub_pos_.x = 0.0f;
  cub_pos_.y = -2.0f;
  cub_pos_.z = 0.0f;
  setupVertices();
}

void Cube::display(GLFWwindow *window, double current_time) {
  // first clear depth buffer
  glClear(GL_DEPTH_BUFFER_BIT);
  // then clear color buffer
  glClear(GL_COLOR_BUFFER_BIT);

  // enable shader by send glsl code to gpu
  glUseProgram(rendering_program_);

  // get uniform variable location from glsl shader program
  m_loc_ = glGetUniformLocation(rendering_program_, "m_mat");
  v_loc_ = glGetUniformLocation(rendering_program_, "v_mat");
  tf_loc_ = glGetUniformLocation(rendering_program_, "tf");
  proj_loc_ = glGetUniformLocation(rendering_program_, "proj_mat");

  // build perspective matrix
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  float aspect = static_cast<float>(width) / height;
  // auto angles=glm::radians(60.0);
  auto pmat = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 1000.0f);

  // build view matrix, model matrix, and model-view matrix
  auto vmat = glm::translate(glm::mat4(1.0f),
                             glm::vec3(-cam_pos_.x, -cam_pos_.y, -cam_pos_.z));
  auto mmat = glm::translate(glm::mat4(1.0f),
                             glm::vec3(cub_pos_.x, cub_pos_.y, cub_pos_.z));

  //  copy model, view and perspective matrix to corresponding uniform variables
  glUniformMatrix4fv(m_loc_, 1, GL_FALSE, glm::value_ptr(mmat));
  glUniformMatrix4fv(v_loc_, 1, GL_FALSE, glm::value_ptr(vmat));
  glUniform1f(tf_loc_, static_cast<float>(current_time));
  glUniformMatrix4fv(proj_loc_, 1, GL_FALSE, glm::value_ptr(pmat));

  // associate VBO with the corresponding vertex attribute in the vertex
  // shader
  glBindBuffer(GL_ARRAY_BUFFER, vert_buff_obj_[0]);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  // adjust opengl settings(preform hidden surface removal) and draw model
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 100000);
}

void Cube::run() {
  if(!glfwInit())
  {
    throw std::runtime_error("failed to initialize glfw");
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  p_window_=glfwCreateWindow(600, 600, "4_2", nullptr, nullptr);

  glfwMakeContextCurrent(p_window_);

  if(glewInit()!=GLEW_OK)
  {
    throw std::runtime_error("failed to init glew");
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