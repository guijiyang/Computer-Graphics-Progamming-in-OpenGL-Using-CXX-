/*
 * @Author: jiyang Gui
 * @Date: 2023-04-03 11:18:16
 * @LastEditors: jiyang Gui
 * @LastEditTime: 2023-04-03 15:14:34
 * @Description:
 * guijiyang@163.com
 * Copyright (c) 2023 by jiyang Gui/GuisGame, All Rights Reserved.
 */
#pragma once
#include <array>
#include <span>
namespace opengltest {
class Pyramid {
  std::array<float, 54> positions_{
      -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f,
      1.0f,  0.0f,  1.0f,  0.0f, // front face
      1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f,
      -1.0f, 0.0f,  1.0f,  0.0f, // right face
      1.0f,  -1.0f, -1.0f, -1.0f, -1.0f,
      -1.0f, 0.0f,  1.0f,  0.0f, // back face
      -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,
      1.0f,  0.0f,  1.0f,  0.0f, // left face
      -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f,
      1.0f,  -1.0f, -1.0f, 1.0f, // base – left front
      1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,
      -1.0f, 1.0f,  -1.0f, -1.0f // base – right back
  };

  std::array<float, 36> texcoords_{
      // texture coordinates
      0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, // front face
      0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, // right face
      0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, // back face
      0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, // left face
      0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, // base – left front
      0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f  // base – right back
  };

public:
  constexpr std::span<float> getPosition() {
    return std::span{positions_.data(), positions_.size()};
  }
  constexpr std::span<float> getTextCoords() {
    return std::span{texcoords_.data(), texcoords_.size()};
  }
  constexpr uint64_t getNumIndices() { return positions_.size() / 3; }
};
} // namespace opengltest
