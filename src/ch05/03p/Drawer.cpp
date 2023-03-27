/*
 * @Author: jiyang Gui
 * @Date: 2023-03-25 19:13:29
 * @LastEditors: jiyang Gui
 * @LastEditTime: 2023-03-27 16:57:36
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

  float cube_texcoords[72] = {
      // texture coordinates
      0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // front face
      0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // right face
      0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // back face
      0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // left face
      0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // base – left front
      0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f  // base – right back
  };
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

  float triangle_texcoords[36] = {
      // texture coordinates
      0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, // front face
      0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, // right face
      0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, // back face
      0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, // left face
      0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, // base – left front
      0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f  // base – right back
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
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube_texcoords), &cube_texcoords,
               GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, vert_buff_obj_[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_positions), triangle_positions,
               GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, vert_buff_obj_[3]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_texcoords), &triangle_texcoords,
               GL_STATIC_DRAW);
}

GLuint Drawer::setupTexture(const char* filename) {
  GLuint texture_id;
  Image image{filename};
  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);

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

  return texture_id;
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
  texture_ids_[0]=setupTexture("texture/bricks.jpg");
  texture_ids_[1]=setupTexture("texture/bricks2.jpg");

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
  // tex_loc_=glGetUniformLocation(rendering_program_, "tex");

  glUniformMatrix4fv(proj_loc_, 1, GL_FALSE, glm::value_ptr(pmat_));

  // activate a texture unit and bind it to a particular texture object
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture_ids_[0]); // use bricks.jpg

  // // set texture unit uniform
  // glUniform1i(tex_loc_,0);

  // push view matrix onto the stack
  vmat_ = glm::translate(glm::mat4(1.0f),
                         glm::vec3(-cam_pos_.x, -cam_pos_.y, -cam_pos_.z));
  mv_stack_.push(vmat_);
  // --------------------------pyramid==sun---------------------
  mv_stack_.push(mv_stack_.top());
  mv_stack_.top() *=
      glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
  mv_stack_.push(mv_stack_.top());
  mv_stack_.top() *=
      glm::rotate(glm::mat4(1.0f), static_cast<float>(current_time),
                  glm::vec3(1.0f, 0.0f, 0.0f));

  glUniformMatrix4fv(mv_loc_, 1, GL_FALSE, glm::value_ptr(mv_stack_.top()));
  glBindBuffer(GL_ARRAY_BUFFER, vert_buff_obj_[2]);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, vert_buff_obj_[3]);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(1);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glDrawArrays(GL_TRIANGLES, 0, 18); // draw the sun
  mv_stack_.pop();

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture_ids_[1]); // use bricks2.jpg

  // ---------------------------cube==planet------------------------
  mv_stack_.push(mv_stack_.top());
  mv_stack_.top() *= glm::translate(
      glm::mat4(1.0f),
      glm::vec3(sin(static_cast<float>(current_time)) * 4.0, 0.0f,
                cos(static_cast<float>(current_time)) * 4.0));
  mv_stack_.push(mv_stack_.top());
  mv_stack_.top() *=
      glm::rotate(glm::mat4(1.0f), static_cast<float>(current_time),
                  glm::vec3(0.0, 1.0, 0.0));

  glUniformMatrix4fv(mv_loc_, 1, GL_FALSE, glm::value_ptr(mv_stack_.top()));
  glBindBuffer(GL_ARRAY_BUFFER, vert_buff_obj_[0]);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, vert_buff_obj_[1]);
  glVertexAttribPointer(1,2,GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(1);

  // glEnable(GL_DEPTH_TEST);
  // glDepthFunc(GL_LEQUAL);
  glDrawArrays(GL_TRIANGLES, 0, 36); // draw planet
  mv_stack_.pop();

  // --------------------------cube=moon------------------------
  // mv_stack_.push(
  //     mv_stack_.top()); // as last element to draw it can use top stack
  mv_stack_.top() *= glm::translate(
      glm::mat4(1.0f),
      glm::vec3(0.0f, sin(static_cast<float>(current_time)) * 2.0f,
                cos(static_cast<float>(current_time)) * 2.0f));
  mv_stack_.top() *=
      glm::rotate(glm::mat4(1.0f), static_cast<float>(current_time),
                  glm::vec3(0.0, 0.0, 1.0));
  mv_stack_.top() *=
      glm::scale(glm::mat4(1.0f), glm::vec3(0.25f, 0.25f, 0.25f));
  glUniformMatrix4fv(mv_loc_, 1, GL_FALSE, glm::value_ptr(mv_stack_.top()));
  glBindBuffer(GL_ARRAY_BUFFER, vert_buff_obj_[0]);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, vert_buff_obj_[1]);
  glVertexAttribPointer(1,2,GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(1);

  // glEnable(GL_DEPTH_TEST);
  // glDepthFunc(GL_LEQUAL);
  glDrawArrays(GL_TRIANGLES, 0, 36);

  // clear stack
  while (!mv_stack_.empty())
    mv_stack_.pop();
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