/*
 * @Author: jiyang Gui
 * @Date: 2023-03-25 19:13:29
 * @LastEditors: jiyang Gui
 * @LastEditTime: 2023-03-30 16:00:00
 * @Description:
 * guijiyang@163.com
 * Copyright (c) 2023 by jiyang Gui/GuisGame, All Rights Reserved.
 */

#include "Drawer.h"
#include "Sphere.h"
#include <Utils.h>
#include <array>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/matrix.hpp>
#include <stdexcept>
#include <vector>

namespace opengltest {
glm::mat4 Drawer::pmat_ = {};

void Drawer::setupVertices() {
  Sphere sphere{};
  auto vertices = sphere.getVertices();

  auto texcoords = sphere.getTexCoords();

  auto indices = sphere.getIndices();
  indices_nums_ = indices.size();
  auto vert_nums = vertices.size();
  std::vector<float> pvertices{};
  pvertices.reserve(vert_nums * 3);
  std::vector<float> ptexcoords{};
  ptexcoords.reserve(vert_nums * 2);

  for (uint32_t i = 0; i < vert_nums; i++) {
    pvertices.push_back(vertices[i].x);
    pvertices.push_back(vertices[i].y);
    pvertices.push_back(vertices[i].z);
    ptexcoords.push_back(texcoords[i].s);
    ptexcoords.push_back(texcoords[i].t);
  }

  glGenVertexArrays(static_cast<GLsizei>(vert_arr_obj_.size()),
                    vert_arr_obj_.data());
  glBindVertexArray(vert_arr_obj_[0]);

  glGenBuffers(static_cast<GLsizei>(vert_buf_obj_.size()),
               vert_buf_obj_.data());
  glBindBuffer(GL_ARRAY_BUFFER, vert_buf_obj_[0]);
  glBufferData(GL_ARRAY_BUFFER, pvertices.size() * 4, pvertices.data(),
               GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, vert_buf_obj_[1]);
  glBufferData(GL_ARRAY_BUFFER, ptexcoords.size() * 4, ptexcoords.data(),
               GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vert_buf_obj_[2]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_nums_ * 4, indices.data(),
               GL_STATIC_DRAW);
}

void Drawer::setupTexture() {
  std::array<const char*, 3> image_names{"texture/sun.jpg","texture/earth.jpg","texture/moon.jpg"};
  for (auto const name : image_names) {
  GLuint texture_id;
  Image image{name};
  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);

  texture_ids_.push_back(texture_id);

  // Set texture parameters
  setTextureParameters();

  // Check for anisotropic filtering extension
  setAnisotropicFiltering();

  // Generate mipmaps and upload texture data
  uploadTextureData(image.width(),image.height(),image.data());
  }
}

void Drawer::setTextureParameters() {
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Drawer::setAnisotropicFiltering() {
  if (glewIsSupported("GL_EXT_texture_filter_anisotropic")) {
    float max_anisotropy;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &max_anisotropy);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT,
                    max_anisotropy);
  }
}

void Drawer::uploadTextureData(int width, int height, const void* img_data) {
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width,height, 0,
               GL_RGBA, GL_UNSIGNED_BYTE, img_data);
  glGenerateMipmap(GL_TEXTURE_2D);
}

void Drawer::init(GLFWwindow *window) {
  rendering_program_ = createShaderProgram();
  // set cam position
  cam_pos_.x = cam_pos_.y = 0.0f;
  cam_pos_.z = 15.0f;
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

// Clear depth and color buffers
void Drawer::clearBuffers() {
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

// Activate the texture and bind it to a texture unit
void Drawer::activateTexture(int tex_id) {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex_id);
}

void Drawer::enableVertexAttribute(GLuint location, GLint size,
                                   GLuint vertex_obj) {
  glBindBuffer(GL_ARRAY_BUFFER, vertex_obj);
  glVertexAttribPointer(location, size, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(location);
}

// Enable depth testing
void Drawer::enableDepthTest() {
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
}

// Draw the sphere
void Drawer::drawSphere() {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vert_buf_obj_[2]);
  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices_nums_), GL_UNSIGNED_INT, 0);
}

void Drawer::display(GLFWwindow *window, double current_time) {
  clearBuffers();

  glUseProgram(rendering_program_);

  mv_loc_ = glGetUniformLocation(rendering_program_, "mv_mat");
  proj_loc_ = glGetUniformLocation(rendering_program_, "proj_mat");

  glUniformMatrix4fv(proj_loc_, 1, GL_FALSE, glm::value_ptr(pmat_));

  // push view matrix onto the stack
  vmat_ = glm::translate(glm::mat4(1.0f),
                         glm::vec3(-cam_pos_.x, -cam_pos_.y, -cam_pos_.z));
  mv_stack_.push(vmat_);
  // --------------------------sun---------------------
  mv_stack_.push(mv_stack_.top());
  mv_stack_.top() *=
      glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
  mv_stack_.push(mv_stack_.top());
  mv_stack_.top() *=
      glm::rotate(glm::mat4(1.0f), static_cast<float>(current_time),
                  glm::vec3(1.0f, 0.0f, 0.0f));

  mv_stack_.top() *= glm::scale(
      glm::mat4(1.0f), glm::vec3(3.0f, 3.0f, 3.0f)); // scale large 3 times
  glUniformMatrix4fv(mv_loc_, 1, GL_FALSE, glm::value_ptr(mv_stack_.top()));
  enableVertexAttribute(0, 3, vert_buf_obj_[0]);
  enableVertexAttribute(1, 2, vert_buf_obj_[1]);
  enableDepthTest();
  activateTexture(texture_ids_[0]);
  drawSphere();
  mv_stack_.pop();

  // ---------------------------planet------------------------
  mv_stack_.push(mv_stack_.top());
  mv_stack_.top() *= glm::translate(
      glm::mat4(1.0f),
      glm::vec3(sin(static_cast<float>(current_time)) * 6.0, 0.0f,
                cos(static_cast<float>(current_time)) * 4.0));
  mv_stack_.push(mv_stack_.top());
  mv_stack_.top() *=
      glm::rotate(glm::mat4(1.0f), static_cast<float>(current_time),
                  glm::vec3(0.0, 1.0, 0.0));

  glUniformMatrix4fv(mv_loc_, 1, GL_FALSE, glm::value_ptr(mv_stack_.top()));
  enableVertexAttribute(0, 3, vert_buf_obj_[0]);
  enableVertexAttribute(1, 2, vert_buf_obj_[1]);
  activateTexture(texture_ids_[1]);
  drawSphere();
  mv_stack_.pop();

  // --------------------------moon------------------------
  // mv_stack_.push(
  //     mv_stack_.top()); // as last element to draw it can use top stack mat
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
  enableVertexAttribute(0, 3, vert_buf_obj_[0]);
  enableVertexAttribute(1, 2, vert_buf_obj_[1]);
  activateTexture(texture_ids_[2]);
  drawSphere();

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