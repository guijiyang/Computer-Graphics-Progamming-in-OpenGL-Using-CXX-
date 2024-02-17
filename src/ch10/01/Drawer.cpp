/*
 * @Author: jiyang Gui
 * @Date: 2023-03-27 22:25:34
 * @LastEditors: jiyang Gui
 * @LastEditTime: 2024-02-06 23:30:48
 * @Description:
 * guijiyang@163.com
 * Copyright (c) 2023 by jiyang Gui/GuisGame, All Rights Reserved.
 */
#include "Drawer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <easylogging++.h>
#include <stdexcept>

namespace opengltest {

static glm::mat4 kProjMat{};

void windowReshapeCallback(GLFWwindow *window, int new_width, int new_height) {
  auto aspect = static_cast<float>(new_width) / new_height;
  glViewport(0, 0, new_width, new_height);
  kProjMat = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 1000.0f);
}

void Drawer::setupVertices() {
  auto indices = torus_.getIndices();
  auto verts = torus_.getVertices();
  auto texcoords = torus_.getTexCoords();
  auto norms = torus_.getNormals();

  std::vector<float> pvalues;
  std::vector<float> tvalues;
  std::vector<float> nvalues;

  auto vert_nums = torus_.getNumVertices();
  for (uint32_t i = 0; i < vert_nums; i++) {
    pvalues.push_back(verts[i].x);
    pvalues.push_back(verts[i].y);
    pvalues.push_back(verts[i].z);

    tvalues.push_back(texcoords[i].s);
    tvalues.push_back(texcoords[i].t);

    nvalues.push_back(norms[i].x);
    nvalues.push_back(norms[i].y);
    nvalues.push_back(norms[i].z);
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

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vert_buf_obj_[3]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * 4, indices.data(),
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
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
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
  // cam_pos_.x = cam_pos_.y = 0.0f;
  // cam_pos_.z = 8.0f;
  setupVertices();
  // texture_id_ = setupTexture("texture/rectangleBricks.jpg");
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  auto aspect = static_cast<float>(width) / height;
  kProjMat = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 1000.0f);
}

// displaying a sphere with  lighting
void Drawer::display(GLFWwindow *window, double current_time) {
  clearBuffers(); // Clear depth and color buffers
  glUseProgram(rendering_program_);
  setMatrices(current_time); // Set the projection and model-view matrices
  setLightDirection(window); // Set the direction of the light source
  // activateTexture(); // Activate the texture and bind it to a texture unit
  // pushViewMatrix(); // Push the view matrix onto the stack

  // Bind vertex buffer objects and enable vertex attributes
  enableVertexAttribute(0, 3, vert_buf_obj_[0]);
  enableVertexAttribute(1, 2, vert_buf_obj_[1]);
  enableVertexAttribute(2, 3, vert_buf_obj_[2]);

  enableDepthTest(); // Enable depth testing

  // // create a buffer to hold the output data
  // GLuint buffer;
  // glGenBuffers(1, &buffer);
  // glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, buffer);
  // glBufferData(GL_TRANSFORM_FEEDBACK_BUFFER, sizeof(float) * 4, NULL,
  //              GL_DYNAMIC_COPY);
  // glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, buffer);

  // // create a feedback object
  // GLuint feedback;
  // glGenTransformFeedbacks(1, &feedback);
  // glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback);
  // glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, buffer);

  // // enable transform feedback
  // glEnable(GL_RASTERIZER_DISCARD);

  // // begin the feedback operation
  // glBeginTransformFeedback(GL_POINTS);

  drawSphere(); // Draw the sphere

  // // end the feedback operation
  // glEndTransformFeedback();

  // // disable transform feedback
  // glDisable(GL_RASTERIZER_DISCARD);

  // // read the output data from the buffer
  // float data[4];
  // glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, buffer);
  // glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(float) * 4,
  // data); el::Loggers::getLogger("Drawer"); CLOG(INFO, "Drawer") << data[0] <<
  // " " << data[1] << " " << data[2] << " "
  //                      << data[3];

  //  // cleanup
  // glDeleteTransformFeedbacks(1, &feedback);
  // glDeleteBuffers(1, &buffer);
}

// Clear depth and color buffers
void Drawer::clearBuffers() {
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

/**
 * @brief set light and material properties
 *
 * @param window
 */
void Drawer::setLightDirection(GLFWwindow *window) {
  double mouseX, mouseY;
  glfwGetCursorPos(window, &mouseX, &mouseY);

  // Convert the mouse position from screen coordinates to normalized device
  // coordinates
  int width, height;
  glfwGetWindowSize(window, &width, &height);
  glm::vec3 ndc = glm::vec3((2.0f * mouseX) / width - 1.0f,
                            1.0f - (2.0f * mouseY) / height, 0.0f);
  // Convert the NDC coordinates to world coordinates using the inverse
  // view-projection matrix
  glm::mat4 viewProjectionInv = glm::inverse(kProjMat * vmat_);
  glm::vec4 worldPos = viewProjectionInv * glm::vec4(ndc, 1.0f);
  worldPos /= worldPos.w;

  // set up lights to The resulting world position is in the last three
  // components of the `worldPos` vector
  current_light_pos_ = glm::vec3(worldPos);

  installLights();
  // build the inverse-transpose of the MV matrix by concatenating matrices v
  // and m, as before
  auto inv_tr_mat = glm::transpose(glm::inverse(mvmat_));
  glUniformMatrix4fv(glGetUniformLocation(rendering_program_, "norm_matrix"), 1,
                     GL_FALSE, glm::value_ptr(inv_tr_mat));
}

void Drawer::installLights() {
  // convert light's position to view space, and save it in a float array
  light_pos_v = glm::vec3(vmat_ * glm::vec4(current_light_pos_, 1.0));

  // set the uniform light and material values in the shader
  glProgramUniform4fv(rendering_program_,
                      glGetUniformLocation(rendering_program_, "globalAmbient"),
                      1, global_ambient_.data());
  // el::Loggers::getLogger("Drawer");
  // CLOG(INFO, "Drawer") << global_ambient_[0] << " " << global_ambient_[1] <<
  // " "
  //                      << global_ambient_[2];
  glProgramUniform4fv(rendering_program_,
                      glGetUniformLocation(rendering_program_, "light.ambient"),
                      1, light_ambient_.data());
  glProgramUniform4fv(rendering_program_,
                      glGetUniformLocation(rendering_program_, "light.diffuse"),
                      1, light_diffuse_.data());
  glProgramUniform4fv(
      rendering_program_,
      glGetUniformLocation(rendering_program_, "light.specular"), 1,
      light_specular_.data());
  glProgramUniform3fv(
      rendering_program_,
      glGetUniformLocation(rendering_program_, "light.position"), 1,
      glm::value_ptr(light_pos_v));
  glProgramUniform4fv(
      rendering_program_,
      glGetUniformLocation(rendering_program_, "material.ambient"), 1,
      goldAmbient().data());
  glProgramUniform4fv(
      rendering_program_,
      glGetUniformLocation(rendering_program_, "material.diffuse"), 1,
      goldDiffuse().data());
  glProgramUniform4fv(
      rendering_program_,
      glGetUniformLocation(rendering_program_, "material.specular"), 1,
      goldSpecular().data());
  glProgramUniform1f(
      rendering_program_,
      glGetUniformLocation(rendering_program_, "material.shininess"),
      goldShininess());
}

// Set the projection and model-view matrices
void Drawer::setMatrices(double current_time) {
  glUniformMatrix4fv(glGetUniformLocation(rendering_program_, "proj_mat"), 1,
                     GL_FALSE, glm::value_ptr(kProjMat));
  vmat_ = glm::translate(glm::mat4(1.0f),
                         glm::vec3(-cam_pos_.x, -cam_pos_.y, -cam_pos_.z));
  mvmat_ = vmat_ *
           glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) *
           glm::rotate(glm::mat4(1.0f), static_cast<float>(current_time),
                       glm::vec3(0.0f, 1.0f, 0.0f)) *
           glm::rotate(glm::mat4(1.f), glm::radians(25.f),
                       glm::vec3(1.0f, 0.0f, 0.0f));
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
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vert_buf_obj_[3]);
  glDrawElements(GL_TRIANGLES, torus_.getNumIndices(), GL_UNSIGNED_INT, 0);
}

void Drawer::run(const char *filename) {
  if (!glfwInit()) {
    throw std::runtime_error("failed to initialize glfw");
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  auto p_window = glfwCreateWindow(600, 600, filename, nullptr, nullptr);
  glfwMakeContextCurrent(p_window);

  if (glewInit() != GLEW_OK) {
    throw std::runtime_error("failed to intilialize glew!");
  }

  const GLubyte *versionString = glGetString(GL_VERSION);
  if (versionString == nullptr) {
    throw std::runtime_error(
        "Error: Could not retrieve OpenGL version string!");
  }
  el::Loggers::getLogger("Drawer");
  CLOG(INFO, "Drawer") << "OpenGL version: " << versionString;

  glfwSwapInterval(1);

  glfwSetWindowSizeCallback(p_window, windowReshapeCallback);
  init(p_window);

  while (!glfwWindowShouldClose(p_window)) {
    glfwPollEvents();
    display(p_window, glfwGetTime());
    glfwSwapBuffers(p_window);
  }
}

} // namespace opengltest