/*
 * @Author: jiyang Gui
 * @Date: 2023-03-25 19:13:29
 * @LastEditors: jiyang Gui
 * @LastEditTime: 2023-03-27 14:48:53
 * @Description:
 * guijiyang@163.com
 * Copyright (c) 2023 by jiyang Gui/GuisGame, All Rights Reserved.
 */

#include "Drawer.h"
#include <Utils.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/matrix.hpp>
#include <stdexcept>

namespace opengltest {
glm::mat4 Drawer::pmat_ = {};

void Drawer::setupVertices() {
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

  float texture_coords[36] = {
      0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
      0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
      0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f};

  glGenVertexArrays(static_cast<GLsizei>(vert_arr_obj_.size()),
                    vert_arr_obj_.data());
  glBindVertexArray(vert_arr_obj_[0]);

  glGenBuffers(static_cast<GLsizei>(vert_buff_obj_.size()),
               vert_buff_obj_.data());

  glBindBuffer(GL_ARRAY_BUFFER, vert_buff_obj_[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_positions), triangle_positions,
               GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, vert_buff_obj_[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(texture_coords), texture_coords,
               GL_STATIC_DRAW);
}

void Drawer::setupTexture() {
  Image image{"texture/bricks.jpg"};
  glGenTextures(1, &texture_id_);
  glBindTexture(GL_TEXTURE_2D, texture_id_);

  // Set texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  // Check for anisotropic filtering extension
  if (glewIsSupported("GL_EXT_texture_filter_anisotropic")) {
    float max_anisotropy;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_anisotropy);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT,
                    max_anisotropy);
  }

  // Generate mipmaps and upload texture data
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0,
               GL_RGBA, GL_UNSIGNED_BYTE, image.data());
  glGenerateMipmap(GL_TEXTURE_2D);
}

void Drawer::init(GLFWwindow *window) {
  rendering_program_ = createShaderProgram();
  // set cam position
  cam_pos_.x = cam_pos_.y = 0.0f;
  cam_pos_.z = 12.0f;
  // // set cube position
  // cub_pos_.x = cub_pos_.z = 0.0f;
  // cub_pos_.y = -1.5f;
  // // set triangle position
  // tri_pos_.x = tri_pos_.y = 2.5f;
  // tri_pos_.z = 0.0f;
  setupVertices();
  setupTexture();
  int width, height;
  glfwGetFramebufferSize(p_window_, &width, &height);
  auto aspect = static_cast<float>(width) / height;
  pmat_ = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 1000.0f);
}

void Drawer::display(GLFWwindow *window, double current_time) {
  glClear(GL_DEPTH_BUFFER_BIT);
  // glClearColor(0.0, 0.0, 0.0, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(rendering_program_);

  mv_loc_ = glGetUniformLocation(rendering_program_, "mv_mat");
  proj_loc_ = glGetUniformLocation(rendering_program_, "proj_mat");

  glUniformMatrix4fv(proj_loc_, 1, GL_FALSE, glm::value_ptr(pmat_));

  // push view matrix onto the stack
  vmat_ = glm::translate(glm::mat4(1.0f),
                         glm::vec3(-cam_pos_.x, -cam_pos_.y, -cam_pos_.z));
  mmat_ = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
  mmat_ *= glm::rotate(glm::mat4(1.0f), static_cast<float>(current_time),
                       glm::vec3(1.0f, 0.0f, 0.0f));
  vmat_ *= mmat_;

  glUniformMatrix4fv(mv_loc_, 1, GL_FALSE, glm::value_ptr(vmat_));
  glBindBuffer(GL_ARRAY_BUFFER, vert_buff_obj_[0]);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, vert_buff_obj_[1]);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(1);

  // activate a texture unit and bind it to a particular texture object
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture_id_);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glDrawArrays(GL_TRIANGLES, 0, 18); // draw the sun
}

void Drawer::run() {
  if (!glfwInit()) {
    throw std::runtime_error("failed to initialize glfw");
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  p_window_ = glfwCreateWindow(600, 600, "4_4", nullptr, nullptr);
  glfwMakeContextCurrent(p_window_);

  if (glewInit() != GLEW_OK) {
    throw std::runtime_error("failed to intilialize glew!");
  }

  glfwSwapInterval(1);

  glfwSetWindowSizeCallback(p_window_, windowReshapeCallback);
  init(p_window_);

  while (!glfwWindowShouldClose(p_window_)) {
    glfwPollEvents();
    display(p_window_, glfwGetTime());
    glfwSwapBuffers(p_window_);
  }
}

void Drawer::windowReshapeCallback(GLFWwindow *window, int new_width,
                                   int new_height) {
  auto aspect = static_cast<float>(new_width) / new_height;
  glViewport(0, 0, new_width, new_height);
  pmat_ = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 1000.0f);
}

} // namespace opengltest