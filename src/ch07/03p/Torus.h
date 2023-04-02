/*
 * @Author: jiyang Gui
 * @Date: 2023-03-28 16:15:18
 * @LastEditors: jiyang Gui
 * @LastEditTime: 2023-03-28 18:44:21
 * @Description:
 * guijiyang@163.com
 * Copyright (c) 2023 by jiyang Gui/GuisGame, All Rights Reserved.
 */
#pragma once
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <vector>

namespace opengltest {
class Torus {
private:
  uint32_t vert_nums_;
  uint32_t indx_nums_;
  uint8_t precision_;
  float inner_radius_;
  float outer_radius_;
  std::vector<uint32_t> indices_;
  std::vector<glm::vec3> vertices_;
  std::vector<glm::vec2> texcoords_;
  std::vector<glm::vec3> normals_;
  std::vector<glm::vec3> s_tangents_;
  std::vector<glm::vec3> t_tangents_;

private:
  void init();

public:
  Torus(float inner_radius=0.5f, float outer_radius=0.2f, uint8_t precision=48);
  uint32_t getNumVertices() const;
  uint32_t getNumIndices() const;
  std::vector<uint32_t> getIndices() const;
  std::vector<glm::vec2> getTexCoords() const;
  std::vector<glm::vec3> getVertices() const;
  std::vector<glm::vec3> getNormals() const;
  std::vector<glm::vec3> getSTangents() const;
  std::vector<glm::vec3> getTTangents() const;
};

} // namespace opengltest