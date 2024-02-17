/*
 * @Author: jiyang Gui
 * @Date: 2023-03-27 20:15:39
 * @LastEditors: jiyang Gui
 * @LastEditTime: 2023-04-10 14:20:42
 * @Description:
 * guijiyang@163.com
 * Copyright (c) 2023 by jiyang Gui/GuisGame, All Rights Reserved.
 */
#pragma once

#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <stdint.h>
#include <vector>

namespace opengltest {
class Sphere {
private:
  uint32_t vert_nums_{};
  uint32_t indx_nums_{};
  std::vector<uint32_t> indices_;
  std::vector<glm::vec3> vertices_;
  std::vector<glm::vec2> texcoords_;
  std::vector<glm::vec3> normals_;
  std::vector<glm::vec3> tangents_;

// private:
//   constexpr void init(uint32_t);
  // float toRadians(float degrees) const;
public:
  uint32_t getNumVertices() const { return vert_nums_; }
  uint32_t getNumIndices() const { return indx_nums_; }
  std::vector<uint32_t> getIndices() { return indices_; }
  std::vector<glm::vec3> getVertices() { return vertices_; }
  std::vector<glm::vec2> getTexCoords() { return texcoords_; }
  std::vector<glm::vec3> getNormals() { return normals_; }
  std::vector<glm::vec3> getTangents() { return tangents_; }

public:
  Sphere(const uint32_t precision = 48);
};

} // namespace opengltest