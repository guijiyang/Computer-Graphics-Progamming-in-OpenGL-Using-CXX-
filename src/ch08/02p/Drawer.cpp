/*
 * @Author: jiyang Gui
 * @Date: 2023-03-27 22:25:34
 * @LastEditors: jiyang Gui
 * @LastEditTime: 2023-04-04 15:34:34
 * @Description:
 * guijiyang@163.com
 * Copyright (c) 2023 by jiyang Gui/GuisGame, All Rights Reserved.
 */
#include "Drawer.h"
#include "GLFW/glfw3.h"
#include <easylogging++.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>

namespace opengltest {

glm::mat4 Drawer::kProjMat{};
GLuint Drawer::kShadowTex{0};
GLuint Drawer::kShadowBuf{0};

void Drawer::setupVertices() {
  /**
   * @brief set up torus vertex,texture coordinates,normalize and indices
   *
   */
  auto indices = torus_.getIndices();
  auto verts = torus_.getVertices();
  auto texcoords = torus_.getTexCoords();
  auto norms = torus_.getNormals();

  std::vector<float> pvalues(verts.size() * 3, 0.f);
  std::vector<float> tvalues(texcoords.size() * 2, 0.f);
  std::vector<float> nvalues(norms.size() * 3, 0.f);

  auto vert_nums = torus_.getNumVertices();
  for (uint32_t i = 0; i < vert_nums; i++) {
    pvalues[i * 3] = verts[i].x;
    pvalues[i * 3 + 1] = verts[i].y;
    pvalues[i * 3 + 2] = verts[i].z;

    tvalues[i * 2] = texcoords[i].s;
    tvalues[i * 2 + 1] = texcoords[i].t;

    nvalues[i * 3] = norms[i].x;
    nvalues[i * 3 + 1] = norms[i].y;
    nvalues[i * 3 + 2] = norms[i].z;
  }

  /**
   * @brief set up pyramide vertex and texture coordinates
   *
   */
  auto p_verts = pyramid_.getPosition();
  auto p_texcoords = pyramid_.getTextCoords();

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

  glBindBuffer(GL_ARRAY_BUFFER, vert_buf_obj_[4]);
  glBufferData(GL_ARRAY_BUFFER, p_verts.size() * 4, p_verts.data(),
               GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, vert_buf_obj_[5]);
  glBufferData(GL_ARRAY_BUFFER, p_texcoords.size() * 4, p_texcoords.data(),
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
  // read shader parameters
  rendering_program1_ = createShaderProgram("shader1.vert", "shader1.frag");
  rendering_program2_ = createShaderProgram("shader2.vert", "shader2.frag");

  // set up model vertices,etc.
  setupVertices();

  // get window width,height and aspect
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  auto aspect = static_cast<float>(width) / height;
  setupShdowBuffers(window, width, height);
  b_ = glm::mat4(0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
                 0.5f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f);
  // texture_id_ = setupTexture("texture/rectangleBricks.jpg");
  kProjMat = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 1000.0f);
}

// sets up a custom frame buffer and a texture to hold depth information for
// shadow mapping
void Drawer::setupShdowBuffers(GLFWwindow *window, int width, int height) {
  // create the custom frame buffer
  glGenFramebuffers(1, &kShadowBuf);

  // create the  shadow texture and configure it to hold depth information
  // these steps are similar to those in Program 5.2
  glGenTextures(1, &kShadowTex);
  glBindTexture(GL_TEXTURE_2D, kShadowTex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0,
               GL_DEPTH_COMPONENT, GL_FLOAT, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE,
                  GL_COMPARE_REF_TO_TEXTURE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
}

// displaying a sphere with  lighting
void Drawer::display(GLFWwindow *window, double current_time) {
  clearBuffers(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT,
               GL_CW); // Clear depth and color buffers

  // light be positioned by moving the mouse
  double mouse_x, mouse_y;
  glfwGetCursorPos(window, &mouse_x, &mouse_y);

  int width, height;
  glfwGetWindowSize(window, &width, &height);
  glm::vec3 ndc = glm::vec3((2.0f * mouse_x) / width - 1.0f,
                            1.0f - (2.0f * mouse_y) / height, 0.f);

  // Convert the NDC coordinates to world coordinates using the inverse
  // view-projection matrix
  vmat_ = glm::translate(glm::mat4(1.f),
                         glm::vec3(-cam_loc_.x, -cam_loc_.y, -cam_loc_.z));
  glm::mat4 viewProjectionInv = glm::inverse(kProjMat * vmat_);
  glm::vec4 worldPos = viewProjectionInv * glm::vec4(ndc, 1.0f);
  worldPos /= worldPos.w;

  // set up lights to The resulting world position is in the last three
  // components of the `worldPos` vector
  light_loc_ = {worldPos.x,worldPos.y,worldPos.z};

  // set up view and prespective matrix from the light of view, for pass 1
  light_v_mat_ =
      glm::lookAt(glm::vec3(light_loc_.x, light_loc_.y, light_loc_.z),
                  glm::vec3(0.0f), glm::vec3(-light_loc_.z, 0.f, light_loc_.x));

  // make the custom frame buffer current, and associate it with the shadow
  // texture
  glBindFramebuffer(GL_FRAMEBUFFER, kShadowBuf);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, kShadowTex, 0);

  // disable drawing colors, but enable the depth computation
  glDrawBuffer(GL_NONE);
  glEnable(GL_DEPTH_TEST);

  // for reducing shadow artifacts
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(2.f, 4.f);

  passOne();

  glDisable(GL_POLYGON_OFFSET_FILL);

  // restore the default display buffer, and re-enable drawing
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, kShadowTex);
  glDrawBuffer(GL_FRONT); // re-enables drawing colors

  passTwo();
}

void Drawer::passOne() {
  // rendering program1 includes the pass one vertex and fragment shaders
  glUseProgram(rendering_program1_);

  // the following blocks of code render the torus to establish the depth buffer
  // from the light point of view
  mmat_ = glm::translate(glm::mat4(1.0f),
                         glm::vec3(torus_loc_.x, torus_loc_.y, torus_loc_.z));
  // slight rotation for viewability
  mmat_ = glm::rotate(mmat_, glm::radians(25.0f), glm::vec3(1.0f, 0.f, 0.f));

  // we are drawing from the light point of view, so we use the light's P and V
  // matrices
  shadow_mvp1_ = kProjMat * light_v_mat_ * mmat_;

  glUniformMatrix4fv(glGetUniformLocation(rendering_program1_, "shadowMVP1"), 1,
                     GL_FALSE, glm::value_ptr(shadow_mvp1_));

  // we only need to set up torus vertices buffer - we don't need its textures
  // or normals for pass one
  enableVertexAttribute(0, 3, vert_buf_obj_[0]);

  clearBuffers(GL_DEPTH_BUFFER_BIT, GL_CW);

  drawTorus();

  // The pyramid is not indexed, so we use glDrawArrarys rather then
  // glDrawElements()
  mmat_ =
      glm::translate(glm::mat4(1.0f),
                     glm::vec3(pyramid_loc_.x, pyramid_loc_.y, pyramid_loc_.z));
  mmat_ = glm::rotate(mmat_, glm::radians(25.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  shadow_mvp1_ = kProjMat * light_v_mat_ * mmat_;
  glUniformMatrix4fv(glGetUniformLocation(rendering_program1_, "shadowMVP1"), 1,
                     GL_FALSE, glm::value_ptr(shadow_mvp1_));
  enableVertexAttribute(0, 3, vert_buf_obj_[4]);

  // clearBuffers(GL_DEPTH_BUFFER_BIT);
  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CCW);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  drawPyramid();
}

void Drawer::passTwo() {
  glUseProgram(rendering_program2_); // pass two vertex and fragment shaders

  // ------------------draw torus--------------
  // draw the torus - this time we need to include lighting,materials,normals,
  // etc. we also need to provide MVP tranforms for BOTH camera space and light
  // space.
  // install the light
  installLights(vmat_);

  mmat_ = glm::translate(glm::mat4(1.f),
                         glm::vec3(torus_loc_.x, torus_loc_.y, torus_loc_.z));
  // slight rotation for viewability
  mmat_ = glm::rotate(mmat_, glm::radians(25.0f), glm::vec3(1.0f, 0.f, 0.f));

  // build mvp matrix for the torus from the light's point of view
  mvmat_ = vmat_ * mmat_;
  auto invtr_mat = glm::transpose(glm::inverse(mvmat_));

  // build the mvp matrix for the torus from the light's point of view
  shadow_mvp2_ = b_ * kProjMat * light_v_mat_ * mmat_;

  // put the mv and proj matrices into the corresponding uniforms
  glUniformMatrix4fv(glGetUniformLocation(rendering_program2_, "mv_mat"), 1,
                     GL_FALSE, glm::value_ptr(mvmat_));
  glUniformMatrix4fv(glGetUniformLocation(rendering_program2_, "proj_mat"), 1,
                     GL_FALSE, glm::value_ptr(kProjMat));
  glUniformMatrix4fv(glGetUniformLocation(rendering_program2_, "norm_mat"), 1,
                     GL_FALSE, glm::value_ptr(invtr_mat));
  glUniformMatrix4fv(glGetUniformLocation(rendering_program2_, "shadowMVP2"), 1,
                     GL_FALSE, glm::value_ptr(shadow_mvp2_));

  //  set up gold materials
  installGoldMaterial();

  // set up torus vertices and normals buffers(and texture corrdinates buffer if
  // used)
  enableVertexAttribute(0, 3, vert_buf_obj_[0]);
  enableVertexAttribute(1, 3, vert_buf_obj_[2]);

  clearBuffers(GL_DEPTH_BUFFER_BIT, GL_CW);

  drawTorus();

  // ---------------------draw pyramid-------------
  mmat_ =
      glm::translate(glm::mat4(1.f),
                     glm::vec3(pyramid_loc_.x, pyramid_loc_.y, pyramid_loc_.z));
  // slight rotation for viewability
  mmat_ = glm::rotate(mmat_, glm::radians(25.0f), glm::vec3(0.0f, 1.f, 0.f));

  // build mvp matrix for the torus from the light's point of view
  mvmat_ = vmat_ * mmat_;
  invtr_mat = glm::transpose(glm::inverse(mvmat_));

  // build the mvp matrix for the pyramid from the light's point of view
  shadow_mvp2_ = b_ * kProjMat * light_v_mat_ * mmat_;

  // put the mv and proj matrices into the corresponding uniforms
  glUniformMatrix4fv(glGetUniformLocation(rendering_program2_, "mv_mat"), 1,
                     GL_FALSE, glm::value_ptr(mvmat_));
  glUniformMatrix4fv(glGetUniformLocation(rendering_program2_, "proj_mat"), 1,
                     GL_FALSE, glm::value_ptr(kProjMat));
  glUniformMatrix4fv(glGetUniformLocation(rendering_program2_, "norm_mat"), 1,
                     GL_FALSE, glm::value_ptr(invtr_mat));
  glUniformMatrix4fv(glGetUniformLocation(rendering_program2_, "shadowMVP2"), 1,
                     GL_FALSE, glm::value_ptr(shadow_mvp2_));

  // set up bronze material
  installBronzeMaterial();

  enableVertexAttribute(0, 3, vert_buf_obj_[4]);
  clearBuffers(GL_DEPTH_BUFFER_BIT);
  drawPyramid();
}

// Clear depth and color buffers
void Drawer::clearBuffers(int clear_flags, int rotate_direction) {
  glClear(clear_flags);
  glEnable(GL_CULL_FACE);
  glFrontFace(rotate_direction);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
}

void Drawer::installLights(glm::mat4 view_mat) {
  auto light_pos_v = glm::vec3(
      view_mat * glm::vec4(light_loc_.x, light_loc_.y, light_loc_.z, 1.0f));

  // set the uniform light and material values in the shader
  glProgramUniform4fv(
      rendering_program2_,
      glGetUniformLocation(rendering_program2_, "globalAmbient"), 1,
      global_ambient_.data());
  glProgramUniform4fv(
      rendering_program2_,
      glGetUniformLocation(rendering_program2_, "light.ambient"), 1,
      light_ambient_.data());
  glProgramUniform4fv(
      rendering_program2_,
      glGetUniformLocation(rendering_program2_, "light.diffuse"), 1,
      light_diffuse_.data());
  glProgramUniform4fv(
      rendering_program2_,
      glGetUniformLocation(rendering_program2_, "light.specular"), 1,
      light_specular_.data());
  glProgramUniform3fv(
      rendering_program2_,
      glGetUniformLocation(rendering_program2_, "light.position"), 1,
      glm::value_ptr(light_pos_v));
}

void Drawer::installGoldMaterial() {
  glProgramUniform4fv(
      rendering_program2_,
      glGetUniformLocation(rendering_program2_, "material.ambient"), 1,
      goldAmbient().data());
  glProgramUniform4fv(
      rendering_program2_,
      glGetUniformLocation(rendering_program2_, "material.diffuse"), 1,
      goldDiffuse().data());
  glProgramUniform4fv(
      rendering_program2_,
      glGetUniformLocation(rendering_program2_, "material.specular"), 1,
      goldSpecular().data());
  glProgramUniform1f(
      rendering_program2_,
      glGetUniformLocation(rendering_program2_, "material.shininess"),
      goldShininess());
}

void Drawer::installBronzeMaterial() {
  glProgramUniform4fv(
      rendering_program2_,
      glGetUniformLocation(rendering_program2_, "material.ambient"), 1,
      bronzeAmbient().data());
  glProgramUniform4fv(
      rendering_program2_,
      glGetUniformLocation(rendering_program2_, "material.diffuse"), 1,
      bronzeDiffuse().data());
  glProgramUniform4fv(
      rendering_program2_,
      glGetUniformLocation(rendering_program2_, "material.specular"), 1,
      bronzeSpecular().data());
  glProgramUniform1f(
      rendering_program2_,
      glGetUniformLocation(rendering_program2_, "material.shininess"),
      bronzeShininess());
}

void Drawer::enableVertexAttribute(GLuint location, GLint size,
                                   GLuint vertex_obj) {
  glBindBuffer(GL_ARRAY_BUFFER, vertex_obj);
  glVertexAttribPointer(location, size, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(location);
}

// Draw the sphere
void Drawer::drawTorus() {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vert_buf_obj_[3]);
  glDrawElements(GL_TRIANGLES, torus_.getNumIndices(), GL_UNSIGNED_INT, 0);
}

void Drawer::drawPyramid() {
  glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(pyramid_.getNumIndices()));
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
  setupShdowBuffers(window, new_width, new_height);
}

} // namespace opengltest