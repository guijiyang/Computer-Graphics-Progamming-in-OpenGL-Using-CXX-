/*
 * @Author: jiyang Gui
 * @Date: 2023-03-27 20:15:39
 * @LastEditors: jiyang Gui
 * @LastEditTime: 2023-03-28 16:23:27
 * @Description:
 * guijiyang@163.com
 * Copyright (c) 2023 by jiyang Gui/GuisGame, All Rights Reserved.
 */
#pragma once

#include <stdint.h>
#include <vector>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
namespace opengltest {
class Sphere {
private:
  uint32_t vert_nums_;
  uint32_t indx_nums_;
  std::vector<uint32_t> indices_;
  std::vector<glm::vec3> vertices_;
  std::vector<glm::vec2> texcoords_;
  std::vector<glm::vec3> normals_;

private:
  void init(uint32_t);
  // float toRadians(float degrees) const;
public:
  Sphere(uint32_t precision = 48);
  uint32_t getNumVertices() const;
  uint32_t getNumIndices() const;
  std::vector<uint32_t> getIndices();
  std::vector<glm::vec3> getVertices();
  std::vector<glm::vec2> getTexCoords();
  std::vector<glm::vec3> getNormals();
};

} // namespace opengltest