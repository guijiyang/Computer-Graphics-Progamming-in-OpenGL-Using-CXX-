/*
 * @Author: jiyang Gui
 * @Date: 2023-03-27 22:13:31
 * @LastEditors: jiyang Gui
 * @LastEditTime: 2023-04-01 14:24:57
 * @Description:
 * guijiyang@163.com
 * Copyright (c) 2023 by jiyang Gui/GuisGame, All Rights Reserved.
 */
#pragma once
#include <GLFW/glfw3.h>
#include <Utils.h>
#include <array>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/texture.h>
#include <gl/glew.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <memory_resource>
#include <vector>
#include <map>

namespace opengltest {
class Drawer {
private:
  std::array<GLuint, 1> vert_arr_obj_{};
  std::pmr::vector<GLuint> vert_buf_obj_{&memory_pool_};
  // std::pmr::vector<GLuint> norm_buf_obj_{&memory_pool_};
  // std::pmr::vector<GLuint> tex_buf_obj_{&memory_pool_};
  // std::pmr::vector<GLuint> idx_buf_obj_{&memory_pool_};
  Position cam_pos_;
  // // represent location of model view matrix in shader
  // GLuint mv_loc_;
  // // represent projection matrix in shader
  // GLuint proj_loc_;
  // a shader program ID
  GLuint rendering_program_;
  // projection matrix
  static glm::mat4 kProjMat;
  // model view matrix
  glm::mat4 mv_mat_{};
  // // view matrix
  Assimp::Importer importer_;

  uint32_t mesh_nums_{0};

  std::pmr::vector<GLuint> texture_ids_{&memory_pool_};

  std::pmr::vector<uint32_t> indices_num_{&memory_pool_};

  std::pmr::unsynchronized_pool_resource memory_pool_{};

private:
  void init(GLFWwindow *window);
  void display(GLFWwindow *window, double current_time);
  void setupModels();
  GLuint setupTexture(aiTexture *texture);

  void setTextureParameters();
  void setAnisotropicFiltering();
  void uploadTextureData(const Image &image);

  void clearBuffers();
  void setLightDirection();
  void setMatrices(double current_time);
  void enableVertexAttribute(GLuint location, GLint size, GLuint vertex_obj);
  void enableDepthTest();
  void drawModel(uint32_t idx_num,GLuint buf_id);

public:
  void run();
};

} // namespace opengltest