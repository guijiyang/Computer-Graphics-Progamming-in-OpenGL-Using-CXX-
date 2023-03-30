/*
 * @Author: jiyang Gui
 * @Date: 2023-03-27 22:25:34
 * @LastEditors: jiyang Gui
 * @LastEditTime: 2023-03-30 12:24:11
 * @Description:
 * guijiyang@163.com
 * Copyright (c) 2023 by jiyang Gui/GuisGame, All Rights Reserved.
 */
#include "Drawer.h"
#include <array>
#include <assimp/postprocess.h>
#include <easylogging++.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>
#include <stdint.h>
#include <vector>

namespace opengltest {
glm::mat4 Drawer::kProjMat{};

void Drawer::setupModels() {
  auto scene = importer_.ReadFile("model/finana.glb",
                                  aiProcess_Triangulate | aiProcess_FlipUVs);
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    CLOG(ERROR, "Utils") << "Error loading file: "
                         << importer_.GetErrorString();
  }

  mesh_nums_ = scene->mNumMeshes;
  indices_num_.reserve(mesh_nums_);
  // vert_buf_obj_.resize(mesh_nums_);
  // norm_buf_obj_.resize(mesh_nums_);
  // tex_buf_obj_.resize(mesh_nums_);
  // idx_buf_obj_.resize(mesh_nums_);
  std::pmr::vector<std::pmr::vector<float>> vertices{&memory_pool_};
  std::pmr::vector<std::pmr::vector<float>> normals{&memory_pool_};
  std::pmr::vector<std::pmr::vector<float>> texcoords{&memory_pool_};
  std::pmr::vector<std::pmr::vector<uint32_t>> indices{&memory_pool_};
  vertices.reserve(mesh_nums_);
  normals.reserve(mesh_nums_);
  texcoords.reserve(mesh_nums_);
  indices.reserve(mesh_nums_);
  for (uint32_t i = 0; i < mesh_nums_; i++) {

    auto mesh = scene->mMeshes[i];

    vertices.emplace_back(std::pmr::vector<float>{&memory_pool_});
    normals.emplace_back(std::pmr::vector<float>{&memory_pool_});
    texcoords.emplace_back(std::pmr::vector<float>{&memory_pool_});
    indices.emplace_back(std::pmr::vector<uint32_t>{&memory_pool_});
    vertices[i].reserve(mesh->mNumVertices * 3);
    normals[i].reserve(mesh->mNumVertices * 3);
    texcoords[i].reserve(mesh->mNumVertices * 2);
    for (uint32_t j = 0; j < mesh->mNumVertices; j++) {
      vertices[i].push_back(mesh->mVertices[j].x);
      vertices[i].push_back(mesh->mVertices[j].y);
      vertices[i].push_back(mesh->mVertices[j].z);
      normals[i].push_back(mesh->mNormals[j].x);
      normals[i].push_back(mesh->mNormals[j].y);
      normals[i].push_back(mesh->mNormals[j].z);

      for (uint8_t k = 0; mesh->mTextureCoords[k] != nullptr; k++) {
        texcoords[i].push_back(mesh->mTextureCoords[k][j].x);
        texcoords[i].push_back(mesh->mTextureCoords[k][j].y);
        // texcoords[i].push_back(mesh->mTextureCoords[k][j].z);
      }
    }

    indices[i].reserve(mesh->mNumFaces * 3);
    for (uint32_t j = 0; j < mesh->mNumFaces; j++) {
      auto indice_num = mesh->mFaces[j].mNumIndices;
      for (uint8_t k = 0; k < indice_num; k++) {
        // CLOG(INFO, "Drawer")<<mesh->mFaces[j].mIndices[k];
        // if(mesh->mFaces[j].mIndices == nullptr)
        // {
        //   CLOG(ERROR, "Drawer")<<"primitive indices array shouldn't be
        //   nullptr while indices number big then zero!";
        // }
        indices[i].push_back(mesh->mFaces[j].mIndices[k]);
      }
    }
    indices_num_.push_back(static_cast<uint32_t>(indices[i].size()));
  }

  for (uint8_t i = 0; i < scene->mNumTextures; i++) {

    auto id = setupTexture(scene->mTextures[i]);
    texture_ids_.push_back(id);
  }

  glGenVertexArrays(static_cast<GLsizei>(vert_arr_obj_.size()),
                    vert_arr_obj_.data());
  glBindVertexArray(vert_arr_obj_[0]);

  vert_buf_obj_.resize(mesh_nums_ * 4);

  glGenBuffers(static_cast<GLsizei>(vert_buf_obj_.size()),
               vert_buf_obj_.data());

  for (uint8_t i = 0; i < mesh_nums_; i++) {
    glBindBuffer(GL_ARRAY_BUFFER, vert_buf_obj_[i * 4]);
    glBufferData(GL_ARRAY_BUFFER, vertices[i].size() * 4, vertices[i].data(),
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vert_buf_obj_[i * 4 + 1]);
    glBufferData(GL_ARRAY_BUFFER, normals[i].size() * 4, normals[i].data(),
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vert_buf_obj_[i * 4 + 2]);
    glBufferData(GL_ARRAY_BUFFER, texcoords[i].size() * 4, texcoords[i].data(),
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vert_buf_obj_[i * 4 + 3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices[i].size() * 4,
                 indices[i].data(), GL_STATIC_DRAW);
  }
}

GLuint Drawer::setupTexture(aiTexture *texture) {
  const Image image{texture};
  GLuint texture_id;
  glGenTextures(1, &texture_id);
  // auto texture_dims = texture->mHeight == 0 ? GL_TEXTURE_1D : GL_TEXTURE_2D;
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
  if (image.channels() == 3) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width(), image.height(), 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image.data());
  } else {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image.data());
  }
  glGenerateMipmap(GL_TEXTURE_2D);
}

void Drawer::init(GLFWwindow *window) {
  // register Drawer logger if not registered already
  el::Loggers::getLogger("Drawer");
  rendering_program_ = createShaderProgram();
  cam_pos_.x = cam_pos_.y = 0.0f;
  cam_pos_.z = 20.0f;
  setupModels();

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
  // pushViewMatrix(); // Push the view matrix onto the stack

  glActiveTexture(GL_TEXTURE0); // Activate the texture
  // Bind vertex buffer objects and enable vertex attributes
  for (uint32_t i = 0; i < mesh_nums_; i++) {
    glBindTexture(GL_TEXTURE_2D, texture_ids_[i]); // bind it to a texture unit
    enableVertexAttribute(0, 3, vert_buf_obj_[i * 4]);
    enableVertexAttribute(1, 3, vert_buf_obj_[i * 4 + 1]);
    enableVertexAttribute(2, 2, vert_buf_obj_[i * 4 + 2]);
    enableDepthTest(); // Enable depth testing
    drawModel(indices_num_[i], vert_buf_obj_[i * 4 + 3]); // Draw the sphere
  }
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
  mv_mat_ = glm::translate(glm::mat4(1.0f),
                           glm::vec3(-cam_pos_.x, -cam_pos_.y, -cam_pos_.z)) *
            glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -10.0f, 0.0f)) *
            glm::rotate(glm::mat4(1.0f), static_cast<float>(current_time),
                        glm::vec3(0.0f, 1.0f, 0.0f));
  glUniformMatrix4fv(glGetUniformLocation(rendering_program_, "mv_mat"), 1,
                     GL_FALSE, glm::value_ptr(mv_mat_));
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

// Draw the Model
void Drawer::drawModel(uint32_t idx_num, GLuint buf_id) {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf_id);
  glDrawElements(GL_TRIANGLES, idx_num, GL_UNSIGNED_INT, 0);
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

  auto window_reshape_callback = [](GLFWwindow *window, int new_width,
                                    int new_height) {
    auto aspect = static_cast<float>(new_width) / new_height;
    glViewport(0, 0, new_width, new_height);
    kProjMat = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 1000.0f);
  };

  glfwSetWindowSizeCallback(p_window, window_reshape_callback);
  init(p_window);

  while (!glfwWindowShouldClose(p_window)) {
    glfwPollEvents();
    display(p_window, glfwGetTime());
    glfwSwapBuffers(p_window);
  }
}
} // namespace opengltest