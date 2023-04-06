/*
 * @Author: jiyang Gui
 * @Date: 2023-04-01 18:22:13
 * @LastEditors: jiyang Gui
 * @LastEditTime: 2023-04-07 15:03:55
 * @Description:
 * guijiyang@163.com
 * Copyright (c) 2023 by jiyang Gui/GuisGame, All Rights Reserved.
 */
#include "Drawer.h"
#include "glm/matrix.hpp"
#include <array>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>
namespace opengltest {

glm::mat4 Drawer::kProjMat{};

void Drawer::setupVertices() {
  // set up torus information
  auto indices = torus_.getIndices();
  auto verts = torus_.getVertices();
  auto texcoords = torus_.getTexCoords();
  auto norms = torus_.getNormals();

  std::vector<float> pvalues;
  std::vector<float> tvalues;
  std::vector<float> nvalues;
  pvalues.resize(verts.size() * 3);
  tvalues.resize(texcoords.size() * 2);
  nvalues.resize(norms.size() * 3);

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
  // set up cube information
  std::array<float, 108> vertex_positions{
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

  std::array<float, 72> texture_coordinates{
      1.00f, 0.66f, 1.00f, 0.33f, 0.75f,
      0.33f, // back face lower right
      0.75f, 0.33f, 0.75f, 0.66f, 1.00f, 0.66f,
      // back face upper left
      0.75f, 0.33f, 0.50f, 0.33f, 0.75f,
      0.66f, // right face lower right
      0.50f, 0.33f, 0.50f, 0.66f, 0.75f,
      0.66f, // right face upper left
      0.50f, 0.33f, 0.25f, 0.33f, 0.50f,
      0.66f, // front face lower right
      0.25f, 0.33f, 0.25f, 0.66f, 0.50f,
      0.66f, // front face upper left
      0.25f, 0.33f, 0.00f, 0.33f, 0.25f,
      0.66f,                                    // left face lower right
      0.00f, 0.33f, 0.00f, 0.66f, 0.25f, 0.66f, // left face upper left
      0.25f, 0.33f, 0.50f, 0.33f, 0.50f, 0.00f, // bottom face upper right
      0.50f, 0.00f, 0.25f, 0.00f, 0.25f, 0.33f, // bottom face lower left
      0.25f, 1.00f, 0.50f, 1.00f, 0.50f, 0.66f, // top face upper right
      0.50f, 0.66f, 0.25f, 0.66f, 0.25f,
      1.00f // top face lower left
  };

  glGenVertexArrays(static_cast<GLsizei>(vert_arr_obj_.size()),
                    vert_arr_obj_.data());
  glBindVertexArray(vert_arr_obj_[0]);
  glGenBuffers(static_cast<GLsizei>(vert_buf_obj_.size()),
               vert_buf_obj_.data());

  glBindBuffer(GL_ARRAY_BUFFER, vert_buf_obj_[0]);
  glBufferData(GL_ARRAY_BUFFER, vertex_positions.size() * 4,
               vertex_positions.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, vert_buf_obj_[1]);
  glBufferData(GL_ARRAY_BUFFER, texture_coordinates.size() * 4,
               texture_coordinates.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, vert_buf_obj_[2]);
  glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, pvalues.data(),
               GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, vert_buf_obj_[3]);
  glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, tvalues.data(),
               GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, vert_buf_obj_[4]);
  glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, nvalues.data(),
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vert_buf_obj_[5]);
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
  // compile shader program
  rendering_program_ = createShaderProgram();
  rendering_cube_program_=createShaderProgram("CShader.vert","CShader.frag");
  // set up object information
  setupVertices();
  // load texture from image
  texture_ids_[0] = loadCubeMap("texture/cube_map.jpg");
  // texture_ids_[1] = setupTexture("texture/rectangleBricks.jpg");

  // calculating the projection matrix using the provided window dimensions.
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  auto aspect = static_cast<float>(width) / height;
  kProjMat = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 1000.0f);

  glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

// Refactored code for displaying a sphere with texture and lighting
void Drawer::display(GLFWwindow *window, double current_time) {
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  ; // Clear depth and color buffers
  glUseProgram(rendering_cube_program_);

  // prepare to draw the skybox first. The M matrix places the skybox at the
  vmat_ = glm::translate(glm::mat4(1.0f),
                         glm::vec3(-cam_pos_.x, -cam_pos_.y, -cam_pos_.z));
  // build the MODEL-view matrix
  // mvmat_ = vmat_ * mmat;

  // put MV and PROJ matrix into uniforms, as before
  glUniformMatrix4fv(glGetUniformLocation(rendering_cube_program_, "v_mat"), 1,
                     GL_FALSE, glm::value_ptr(vmat_));
  glUniformMatrix4fv(glGetUniformLocation(rendering_cube_program_, "proj_mat"), 1,
                     GL_FALSE, glm::value_ptr(kProjMat));

  // set up buffer containing vertices
  enableVertexAttribute(0, 3, vert_buf_obj_[0]);

  // // set up buffer containing texture coordinates
  // enableVertexAttribute(1, 2, vert_buf_obj_[1]);

  // activate the skybox texture
  // activateTexture(texture_ids_[0]);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texture_ids_[0]);

  glEnable(GL_CULL_FACE);
  glFrontFace(
      GL_CCW); // cube has CW winding order, but we are viewing its interior

  glDisable(GL_DEPTH_TEST);
  // draw the skybox without depth testing
  glDrawArrays(GL_TRIANGLES, 0, 36);
  enableDepthTest();

  // draw torus
  glUseProgram(rendering_program_);
  // set torous model-view matrix
  mvmat_ = vmat_ *
           glm::translate(glm::mat4(1.0f), glm::vec3(torus_loc_.x, torus_loc_.y, torus_loc_.z)) *
           glm::rotate(glm::mat4(1.0f), static_cast<float>(current_time),
                       glm::vec3(1.0f, 0.f, 0.f));
  glUniformMatrix4fv(glGetUniformLocation(rendering_program_, "mv_mat"), 1,
                     GL_FALSE, glm::value_ptr(mvmat_));
  auto inv_mat=glm::transpose(glm::inverse(mvmat_));
  glUniformMatrix4fv(glGetUniformLocation(rendering_program_, "proj_mat"), 1,
                     GL_FALSE, glm::value_ptr(kProjMat));
  glUniformMatrix4fv(glGetUniformLocation(rendering_program_,"norm_mat"), 1,GL_FALSE, glm::value_ptr(inv_mat));

  // Bind vertex buffer objects and enable vertex attributes
  enableVertexAttribute(0, 3, vert_buf_obj_[2]);
  enableVertexAttribute(1, 2, vert_buf_obj_[3]);
  // activate the torus normals buffer
  enableVertexAttribute(2, 3, vert_buf_obj_[4]);
  
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texture_ids_[0]);

  glClear(GL_DEPTH_BUFFER_BIT);
  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CW);
  // glDepthFunc(GL_LEQUAL);

  drawObject(); // Draw the torus
}

// Activate the texture and bind it to a texture unit
void Drawer::activateTexture(GLuint texture_id) {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture_id);
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

// Draw the object
void Drawer::drawObject() {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vert_buf_obj_[5]);
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