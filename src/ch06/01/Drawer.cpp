/*
 * @Author: jiyang Gui
 * @Date: 2023-03-27 22:25:34
 * @LastEditors: jiyang Gui
 * @LastEditTime: 2023-03-30 15:10:08
 * @Description:
 * guijiyang@163.com
 * Copyright (c) 2023 by jiyang Gui/GuisGame, All Rights Reserved.
 */
#include "Drawer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>

namespace opengltest {

glm::mat4 Drawer::kProjMat{};

void Drawer::setupVertices() {
  auto indices = sphere_.getIndices();
  auto verts = sphere_.getVertices();
  auto texcoords = sphere_.getTexCoords();
  auto norms = sphere_.getNormals();

  std::vector<float> pvalues;
  std::vector<float> tvalues;
  std::vector<float> nvalues;

  auto indice_nums = sphere_.getNumIndices();
  for (uint32_t i = 0; i < indice_nums; i++) {
    pvalues.push_back(verts[indices[i]].x);
    pvalues.push_back(verts[indices[i]].y);
    pvalues.push_back(verts[indices[i]].z);

    tvalues.push_back(texcoords[indices[i]].s);
    tvalues.push_back(texcoords[indices[i]].t);

    nvalues.push_back(norms[indices[i]].x);
    nvalues.push_back(norms[indices[i]].y);
    nvalues.push_back(norms[indices[i]].z);
  }

  glGenVertexArrays(static_cast<GLsizei>(vert_arr_obj_.size()),
                    vert_arr_obj_.data());
  glBindVertexArray(vert_arr_obj_[0]);
  glGenBuffers(static_cast<GLsizei>(vert_buf_obj_.size()),
               vert_buf_obj_.data());

  glBindBuffer(GL_ARRAY_BUFFER, vert_buf_obj_[0]);
  glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, pvalues.data(),
               GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, vert_buf_obj_[1]);
  glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, tvalues.data(),
               GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, vert_buf_obj_[2]);
  glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, nvalues.data(),
               GL_STATIC_DRAW);
}

GLuint Drawer::setupTexture(const char *filename) {
  GLuint texture_id;
  Image image{filename};
  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);

  // Set texture parameters
  setTextureParameters();

  // Check for anisotropic filtering extension
  setAnisotropicFiltering();

  // Generate mipmaps and upload texture data
  uploadTextureData(image);

  return texture_id;
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

void Drawer::uploadTextureData(const Image &image) {
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0,
               GL_RGBA, GL_UNSIGNED_BYTE, image.data());
  glGenerateMipmap(GL_TEXTURE_2D);
}

void Drawer::init(GLFWwindow *window) {
  rendering_program_ = createShaderProgram();
  cam_pos_.x = cam_pos_.y = 0.0f;
  cam_pos_.z = 8.0f;
  setupVertices();
  texture_id_ = setupTexture("texture/earth.jpg");
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  auto aspect = static_cast<float>(width) / height;
  kProjMat = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 1000.0f);
}

// Refactored code for displaying a sphere with texture and lighting
void Drawer::display(GLFWwindow *window, double current_time) {
  clearBuffers(); // Clear depth and color buffers
  glUseProgram(rendering_program_);
  setLightDirection();       // Set the direction of the light source
  setMatrices(current_time); // Set the projection and model-view matrices
  activateTexture(); // Activate the texture and bind it to a texture unit
  // pushViewMatrix(); // Push the view matrix onto the stack

  // Bind vertex buffer objects and enable vertex attributes
  enableVertexAttribute(0, 3, vert_buf_obj_[0]);
  enableVertexAttribute(1, 2, vert_buf_obj_[1]);
  enableVertexAttribute(2, 3, vert_buf_obj_[2]);

  enableDepthTest();        // Enable depth testing
  drawSphere();             // Draw the sphere
}

// Clear depth and color buffers
void Drawer::clearBuffers() {
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

// Set the direction of the light source
void Drawer::setLightDirection() {
  glm::vec3 lightDirection = glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f));
  glUniform3fv(glGetUniformLocation(rendering_program_, "lightDirection"), 1,
               glm::value_ptr(lightDirection));
}

// Set the projection and model-view matrices
void Drawer::setMatrices(double current_time) {
  glUniformMatrix4fv(glGetUniformLocation(rendering_program_, "proj_mat"), 1,
                     GL_FALSE, glm::value_ptr(kProjMat));
  mvmat_ = glm::translate(glm::mat4(1.0f),
                          glm::vec3(-cam_pos_.x, -cam_pos_.y, -cam_pos_.z)) *
           glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) *
           glm::rotate(glm::mat4(1.0f), static_cast<float>(current_time),
                       glm::vec3(0.0f, 1.0f, 0.0f));
  glUniformMatrix4fv(glGetUniformLocation(rendering_program_, "mv_mat"), 1,
                     GL_FALSE, glm::value_ptr(mvmat_));
}

// Activate the texture and bind it to a texture unit
void Drawer::activateTexture() {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture_id_);
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
  glDrawArrays(GL_TRIANGLES, 0, sphere_.getNumIndices());
}

void Drawer::run() {
  if (!glfwInit()) {
    throw std::runtime_error("failed to initialize glfw");
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  auto p_window = glfwCreateWindow(600, 600, "6_1", nullptr, nullptr);
  glfwMakeContextCurrent(p_window);

  if (glewInit() != GLEW_OK) {
    throw std::runtime_error("failed to intilialize glew!");
  }

  glfwSwapInterval(1);

  glfwSetWindowSizeCallback(p_window, windowReshapeCallback);
  init(p_window);

  while (!glfwWindowShouldClose(p_window)) {
    glfwPollEvents();
    display(p_window, glfwGetTime());
    glfwSwapBuffers(p_window);
  }
}

void Drawer::windowReshapeCallback(GLFWwindow *window, int new_width,
                                   int new_height) {
  auto aspect = static_cast<float>(new_width) / new_height;
  glViewport(0, 0, new_width, new_height);
  kProjMat = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 1000.0f);
}

} // namespace opengltest