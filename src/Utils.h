/*
 * @Author: jiyang Gui
 * @Date: 2023-03-23 15:20:19
 * @LastEditors: jiyang Gui
 * @LastEditTime: 2023-03-29 23:05:43
 * @Description:
 * guijiyang@163.com
 * Copyright (c) 2023 by jiyang Gui/GuisGame, All Rights Reserved.
 */

#pragma once
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

} // namespace opengltest