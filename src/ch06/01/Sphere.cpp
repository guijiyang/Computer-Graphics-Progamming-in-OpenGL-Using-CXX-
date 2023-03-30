/*
 * @Author: jiyang Gui
 * @Date: 2023-03-27 20:24:46
 * @LastEditors: jiyang Gui
 * @LastEditTime: 2023-03-28 17:05:53
 * @Description:
 * guijiyang@163.com
 * Copyright (c) 2023 by jiyang Gui/GuisGame, All Rights Reserved.
 */

#include "Sphere.h"
#include <cmath>
#include <cstdlib>
#include <glm/gtx/compatibility.hpp>

namespace opengltest {
Sphere::Sphere(uint32_t precision) { init(precision); }

void Sphere::init(uint32_t precision) {
  vert_nums_ = (precision + 1) * (precision + 1);
  indx_nums_ = precision * precision * 6;
  vertices_ = std::vector<glm::vec3>(vert_nums_, glm::vec3());
  texcoords_ = std::vector<glm::vec2>(vert_nums_, glm::vec2());
  normals_ = std::vector<glm::vec3>(vert_nums_, glm::vec3());
  indices_ = std::vector<uint32_t>(indx_nums_, 0);

  for (uint8_t i = 0; i <= precision; i++)
    for (uint8_t j = 0; j <= precision; j++) {
      float y = cos(glm::radians(180.0f - i * 180.f / precision));
      float x = -cos(glm::radians(j * 360.0f / precision)) * abs(cos(asin(y)));
      float z = sin(glm::radians(j * 360.0f / precision)) * abs(cos(asin(y)));
      vertices_[i * (precision + 1) + j] = glm::vec3(x, y, z);
      texcoords_[i * (precision + 1) + j] = glm::vec2(
          static_cast<float>(j) / precision, static_cast<float>(i) / precision);
      normals_[i * (precision + 1) + j] = glm::vec3(x, y, z);

      if (i != precision && j != precision) {
        indices_[6 * (i * precision + j) + 0] = i * (precision + 1) + j;
        indices_[6 * (i * precision + j) + 1] = i * (precision + 1) + j + 1;
        indices_[6 * (i * precision + j) + 2] = (i + 1) * (precision + 1) + j;
        indices_[6 * (i * precision + j) + 3] = i * (precision + 1) + j + 1;
        indices_[6 * (i * precision + j) + 4] =
            (i + 1) * (precision + 1) + j + 1;
        indices_[6 * (i * precision + j) + 5] = (i + 1) * (precision + 1) + j;
      }
    }
}

uint32_t Sphere::getNumVertices() const { return vert_nums_; }
uint32_t Sphere::getNumIndices() const { return indx_nums_; }
std::vector<uint32_t> Sphere::getIndices() { return indices_; }
std::vector<glm::vec3> Sphere::getVertices() { return vertices_; }
std::vector<glm::vec2> Sphere::getTexCoords() { return texcoords_; }
std::vector<glm::vec3> Sphere::getNormals() { return normals_; }

} // namespace opengltest