/*
 * @Author: jiyang Gui
 * @Date: 2023-03-23 15:20:19
 * @LastEditors: jiyang Gui
 * @LastEditTime: 2023-04-01 14:13:10
 * @Description:
 * guijiyang@163.com
 * Copyright (c) 2023 by jiyang Gui/GuisGame, All Rights Reserved.
 */

#pragma once
#include <array>
#include <assimp/texture.h>
#include <gl/glew.h>

namespace opengltest {
GLuint createShaderProgram();

class Image {
public:
  Image(const char *filename) noexcept;
  Image(aiTexture *texture) noexcept;

  ~Image() noexcept;

  void *data() const { return data_; }

  int width() const { return width_; }

  int height() const { return height_; }

  int channels() const { return channels_; }

private:
  void *data_ = nullptr;
  int width_ = 0;
  int height_ = 0;
  int channels_ = 0;
};

struct Position {
  float x, y, z = 0;
};

// the ambient, diffuse, specular, and shininess values for a gold material
constexpr std::array<float, 4> goldAmbient() {
  std::array<float, 4> a{0.2473f, 0.1995f, 0.0745f, 1.f};
  return a;
}

constexpr std::array<float, 4> goldDiffuse() {
  std::array<float, 4> a{0.7516f, 0.6065f, 0.2265f, 1.f};
  return a;
}

constexpr std::array<float, 4> goldSpecular() {
  std::array<float, 4> a{0.6283f, 0.5559f, 0.3661f, 1.f};
  return a;
}

constexpr float goldShininess() { return 51.2f; }

// the ambient, diffuse, specular, and shininess values for a silver material
constexpr std::array<float, 4> silverAmbient() {
  std::array<float, 4> a{0.1923f, 0.1923f, 0.1923f, 1.f};
  return a;
}

constexpr std::array<float, 4> silverDiffuse() {
  std::array<float, 4> a{0.5075f, 0.5075f, 0.5075f, 1.f};
  return a;
}

constexpr std::array<float, 4> silverSpecular() {
  std::array<float, 4> a{0.5083f, 0.5083f, 0.5083f, 1.f};
  return a;
}

constexpr float silverShininess() { return 51.2f; }

// the ambient, diffuse, specular, and shininess values for a bronze material
constexpr std::array<float, 4> bronzeAmbient() {
  std::array<float, 4> a{0.2125f, 0.1275f, 0.0540f, 1.f};
  return a;
}

constexpr std::array<float, 4> bronzeDiffuse() {
  std::array<float, 4> a{0.7140f, 0.4284f, 0.1814f, 1.f};
  return a;
}

constexpr std::array<float, 4> bronzeSpecular() {
  std::array<float, 4> a{0.3936f, 0.2719f, 0.1667f, 1.f};
  return a;
}
constexpr float bronzeShininess() { return 25.6f; }
} // namespace opengltest