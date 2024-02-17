/*
 * @Author: jiyang Gui
 * @Date: 2023-03-27 20:24:46
 * @LastEditors: jiyang Gui
 * @LastEditTime: 2024-02-17 19:54:10
 * @Description:
 * guijiyang@163.com
 * Copyright (c) 2023 by jiyang Gui/GuisGame, All Rights Reserved.
 */

#include "Sphere.h"
#include <cmath>
#include <cstdlib>
#include <glm/gtx/compatibility.hpp>

namespace opengltest {
Sphere::Sphere(const uint32_t precision) {
  vert_nums_ = (precision + 1) * (precision + 1);
  indx_nums_ = precision * precision * 6;
  vertices_ = std::vector<glm::vec3>(vert_nums_, glm::vec3());
  texcoords_ = std::vector<glm::vec2>(vert_nums_, glm::vec2());
  normals_ = std::vector<glm::vec3>(vert_nums_, glm::vec3());
  indices_ = std::vector<uint32_t>(indx_nums_, 0);
  tangents_ = std::vector<glm::vec3>(vert_nums_, glm::vec3());

  for (uint8_t i = 0; i <= precision; i++)
    for (uint8_t j = 0; j <= precision; j++) {
      float y = cos(glm::radians(180.0f - i * 180.f / precision));
      float x = -cos(glm::radians(j * 360.0f / precision)) * abs(cos(asin(y)));
      float z = sin(glm::radians(j * 360.0f / precision)) * abs(cos(asin(y)));
      vertices_[i * (precision + 1) + j] = glm::vec3(x, y, z);
      texcoords_[i * (precision + 1) + j] = glm::vec2(
          static_cast<float>(j) / precision, static_cast<float>(i) / precision);
      normals_[i * (precision + 1) + j] = glm::vec3(x, y, z);

      // if north or south pole, set tangent to -Z axis
      if (0 == i || precision == i) {
        tangents_[i * (precision + 1) + j] = glm::vec3(0.f, 0.f, -1.f);
      } else {
        tangents_[i * (precision + 1) + j] =
            glm::cross(glm::vec3(0.f, 1.f, 0.f), normals_[i * (precision + 1) + j]);
      }

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
} // namespace opengltest