/*
 * @Author: jiyang Gui
 * @Date: 2023-03-28 16:24:24
 * @LastEditors: jiyang Gui
 * @LastEditTime: 2023-04-01 17:31:28
 * @Description:
 * guijiyang@163.com
 * Copyright (c) 2023 by jiyang Gui/GuisGame, All Rights Reserved.
 */

#include "Torus.h"
#include <glm/gtx/compatibility.hpp>
// #include <stdint.h>

namespace opengltest {
Torus::Torus(float inner_radius, float outer_radius, uint8_t precision)
    : precision_(precision), inner_radius_(inner_radius),
      outer_radius_(outer_radius) {
  init();
}

void Torus::init() {
  vert_nums_ = (precision_ + 1) * (precision_ + 1);
  indx_nums_ = precision_ * precision_ * 6;

  vertices_ = std::vector<glm::vec3>(vert_nums_, glm::vec3());
  texcoords_ = std::vector<glm::vec2>(vert_nums_, glm::vec2());
  normals_ = std::vector<glm::vec3>(vert_nums_, glm::vec3());
  s_tangents_ = std::vector<glm::vec3>(vert_nums_, glm::vec3());
  t_tangents_ = std::vector<glm::vec3>(vert_nums_, glm::vec3());

  indices_ = std::vector<uint32_t>(indx_nums_, 0);

  // calculate first ring
  for (uint8_t i = 0; i <= precision_; i++) {
    auto amt = glm::radians(i * 360.0f / precision_);

    // build the ring by rotating points around the origin, then moving them
    // outward.
    auto r_mat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::vec3 init_pos(r_mat * glm::vec4(outer_radius_, 0.0f, 0.0f, 0.0f));
    vertices_[i] = glm::vec3(init_pos + glm::vec3(inner_radius_, 0.0f, 0.0f));

    // compute texture coordinates for each vertex on the ring
    texcoords_[i] = glm::vec2(0.0f, static_cast<float>(i) / precision_);

    // compute tangents and normals -- first tangent is Y-axis rotated around Z
    r_mat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 0.0f, 1.0f));
    t_tangents_[i] = glm::vec3(r_mat * glm::vec4(0.0f, -1.0f, 0.0f, 1.0f));
    s_tangents_[i] = glm::vec3(0.0f, 0.0f, -1.0f); // second tangent is -Z.
    normals_[i] = glm::cross(t_tangents_[i], s_tangents_[i]);
  }

  // rotate the first ring around Y to  get torus
  for (uint8_t t = 1; t <= precision_; t++) {
    // rotate the vertex positions of the original ring around Y-axis
    auto amt = glm::radians(t * 360.0f / precision_);
    for (uint8_t r = 0; r <= precision_; r++) {
      glm::mat4 r_mat =
          glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
      vertices_[t * (precision_ + 1) + r] =
          glm::vec3(r_mat * glm::vec4(vertices_[r], 1.0f));

      // compute the texture coordinates for the vertex in the new rings
      texcoords_[t * (precision_ + 1) + r] =
          glm::vec2(t * 2.0f / precision_, texcoords_[r].t);

      // rotate the tangent and bitangent vectors around the Y axis
      r_mat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
      s_tangents_[t * (precision_ + 1) + r] =
          glm::vec3(r_mat * glm::vec4(s_tangents_[r], 1.0f));
      r_mat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
      t_tangents_[t * (precision_ + 1) + r] =
          glm::vec3(r_mat * glm::vec4(t_tangents_[r], 1.0f));

      // rotate the normal vector around the Y axis
      r_mat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
      normals_[t * (precision_ + 1) + r] =
          glm::vec3(r_mat * glm::vec4(normals_[r], 1.0f));
    }
  }

  // calculate triangle indices corresponding to the rwo triangles build per
  // vertex
  for (uint8_t t = 0; t < precision_; t++) {
    for (uint8_t r = 0; r < precision_; r++) {
      indices_[(t*precision_+r)*6 + 0] = t * (precision_ + 1) + r;
      indices_[(t*precision_+r)*6 + 1] =
          t * (precision_ + 1) + r + 1;
      indices_[(t*precision_+r)*6 + 2] =
          (t + 1) * (precision_ + 1) + r;
      indices_[(t*precision_+r)*6 + 3] =
          t * (precision_ + 1) + r + 1;
      indices_[(t*precision_+r)*6 + 4] =
          (t + 1) * (precision_ + 1) + r + 1;
      indices_[(t*precision_+r)*6 + 5] =
          (t + 1) * (precision_ + 1) + r;
    }
  }
}

uint32_t Torus::getNumVertices() const { return vert_nums_; }
uint32_t Torus::getNumIndices() const { return indx_nums_; }
std::vector<uint32_t> Torus::getIndices() const { return indices_; }
std::vector<glm::vec3> Torus::getVertices() const { return vertices_; }
std::vector<glm::vec2> Torus::getTexCoords() const { return texcoords_; }
std::vector<glm::vec3> Torus::getNormals() const { return normals_; }
std::vector<glm::vec3> Torus::getSTangents() const { return s_tangents_; }
std::vector<glm::vec3> Torus::getTTangents() const { return t_tangents_; }
} // namespace opengltest