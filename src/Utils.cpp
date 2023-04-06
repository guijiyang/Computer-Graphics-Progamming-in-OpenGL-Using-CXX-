/*
 * @Author: jiyang Gui
 * @Date: 2023-03-23 18:36:19
 * @LastEditors: jiyang Gui
 * @LastEditTime: 2023-04-06 18:39:31
 * @Description:
 * guijiyang@163.com
 * Copyright (c) 2023 by jiyang Gui/GuisGame, All Rights Reserved.
 */
#include <Utils.h>
#include <algorithm>
#include <array>
#include <easylogging++.h>
#include <format>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <stb_image.h>

#pragma warning(push)
#pragma warning(disable : 4996)
#include <stb_image_write.h>
#pragma warning(pop)
#include <stdexcept>
#include <string>

namespace opengltest {
using namespace std;

Image::Image(const char *filename, STBIChannelNumbers stbi_channels,
             int vertical_flip) noexcept {
  stbi_set_flip_vertically_on_load(vertical_flip);
  data_ = stbi_load(filename, &width_, &height_, &channels_,
                    static_cast<int>(stbi_channels));
  if (!data_) {
    // register utils logger if not already registered
    el::Loggers::getLogger("utils");
    CLOG(WARNING, "utils") << "Failed to load image: " << stbi_failure_reason();
  }
}

Image::Image(aiTexture *texture) noexcept {
  // stbi_set_flip_vertically_on_load(1);
  if (texture->mHeight == 0) {
    // texture->CheckFormat("png");
    data_ = stbi_load_from_memory(
        reinterpret_cast<const stbi_uc *>(texture->pcData), texture->mWidth * 4,
        &width_, &height_, &channels_, 0);
  } else {
    data_ = stbi_load_from_memory(
        reinterpret_cast<const stbi_uc *>(texture->pcData),
        texture->mWidth * texture->mHeight * 4, &width_, &height_, &channels_,
        0);
  }

  if (!data_) {
    // register utils logger if not already registered
    el::Loggers::getLogger("utils");
    CLOG(WARNING, "utils") << "Failed to load image: " << stbi_failure_reason();
  }
}

Image::~Image() noexcept {
  if (data_) {
    stbi_image_free(data_);
  }
}

static void printShaderLog(GLuint shader) {
  int len = 0;
  int chWrittn = 0;
  char *log;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
  if (len > 0) {
    log = (char *)malloc(len);
    glGetShaderInfoLog(shader, len, &chWrittn, log);
    // auto logger = el::Loggers::getLogger("utils");
    // logger->warn("Shader Info Log: ", log);
    CLOG(WARNING, "utils") << "Shader Info Log" << log;
    // cout << "Shader Info Log: " << log << endl;
    free(log);
  }
}
static void printProgramLog(int prog) {
  int len = 0;
  int chWrittn = 0;
  char *log;
  glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
  if (len > 0) {
    log = (char *)malloc(len);
    glGetProgramInfoLog(prog, len, &chWrittn, log);
    // auto logger = el::Loggers::getLogger("utils");
    // logger->warn("Program Info Log: ", log);
    CLOG(WARNING, "utils") << "Program Info Log: " << log;
    // cout << "Program Info Log: " << log << endl;
    free(log);
  }
}

bool checkOpenGLError() {
  bool foundError = false;
  int glErr = glGetError();
  while (glErr != GL_NO_ERROR) {
    // auto logger = el::Loggers::getLogger("utils");
    // logger->warn("glError: ", glErr);
    CLOG(WARNING, "utils") << "glError: " << glErr;
    // cout << "glError: " << glErr << endl;
    foundError = true;
    glErr = glGetError();
  }
  return foundError;
}

string readShaderFile(const char *filename) {
  string contents{};
  ifstream file(filename, ios::in);
  if (!file.is_open()) {
    throw std::runtime_error(format("Couldn't open {}", filename));
  }

  string line{};
  while (!file.eof()) {
    getline(file, line);
    contents.append(line + '\n');
  }
  file.close();
  return contents;
}
GLuint createShaderProgram(const char *vert_shader_name,
                           const char *frag_shader_name) {
  // register utils logger if not already registered
  el::Loggers::getLogger("utils");

  GLint vert_compiled;
  GLint frag_compiled;
  GLint linked;
  auto vert_shader_str = readShaderFile(vert_shader_name);
  auto vert_shader_cstr = vert_shader_str.c_str();
  auto frag_shader_str = readShaderFile(frag_shader_name);
  auto frag_shader_cstr = frag_shader_str.c_str();
  auto vert_shader = glCreateShader(GL_VERTEX_SHADER);
  auto frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(vert_shader, 1, &vert_shader_cstr, nullptr);
  glShaderSource(frag_shader, 1, &frag_shader_cstr, nullptr);
  glCompileShader(vert_shader);
  checkOpenGLError();
  glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &vert_compiled);
  if (vert_compiled != 1) {
    // auto logger = el::Loggers::getLogger("utils");
    // logger->warn("vertex shader compilation failed!");
    CLOG(WARNING, "utils") << "vertex shader compilation failed";
    // cout << "vertex shader compilation failed!" << endl;
    printShaderLog(vert_shader);
  }
  glCompileShader(frag_shader);
  checkOpenGLError();
  glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &frag_compiled);
  if (frag_compiled != 1) {
    // auto logger = el::Loggers::getLogger("utils");
    // logger->warn("frag shader compilation failed!");
    CLOG(WARNING, "utils") << "frag shader compilation failed";
    // cout << "frag shader compilaton failed!" << endl;
    printShaderLog(frag_shader);
  }
  auto vf_program = glCreateProgram();
  glAttachShader(vf_program, vert_shader);
  glAttachShader(vf_program, frag_shader);
  glLinkProgram(vf_program);
  checkOpenGLError();
  glGetProgramiv(vf_program, GL_LINK_STATUS, &linked);
  if (linked != 1) {
    // auto logger = el::Loggers::getLogger("utils");
    // logger->warn("linked failed!");
    CLOG(WARNING, "utils") << "linked failed!";
    // cout << "linked failed!" << endl;
    printProgramLog(vf_program);
  }

  return vf_program;
}

GLuint loadCubeMap(const char *filename) {
  // Load the image file using stb_image library
  Image image{filename, STBIChannelNumbers::kSTBIRgb, 0};
  if (image.data() == nullptr) {
    throw std::runtime_error("Failed to load image!");
  }

  auto data = image.data();
  auto channels = image.channels();
  auto width = image.width();
  auto height = image.height();

  // Generate a single texture ID for the cube map texture
  GLuint texture_id;
  glGenTextures(1, &texture_id);

  // Bind the texture object to the GL_TEXTURE_CUBE_MAP target
  glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

  // Set the texture parameters
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // Reduce seams
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  // Split the equirectangular image into six separate images for the cube map
  auto faceWidth = width / 4;
  auto faceHeight = height / 3;
  auto rect_lens = max(faceWidth, faceHeight);
  // Use std::unique_ptr to store the image data for each face of the cube map
  std::array<
      std::unique_ptr<unsigned char[], std::function<void(unsigned char *)>>, 6>
      faceData{};
  for (int i = 0; i < 6; i++) {
    // Allocate memory for each face data using a lambda function to release the
    // memory
    faceData[i] = {new unsigned char[rect_lens * rect_lens * channels]{0},
                   [](unsigned char *ptr) { delete[] ptr; }};
    // Copy the pixels from the equirectangular image to the current face of the
    // cube map
    for (int y = 0; y < faceHeight; y++) {
      for (int x = 0; x < faceWidth; x++) {
        int srcX, srcY;
        switch (i) {
        case 0: // Right face
          srcX = 2 * faceWidth + x;
          srcY = faceHeight + y;
          break;
        case 1: // Left face
          srcX = 0 + x;
          srcY = faceHeight + y;
          break;
        case 2: // Top face
          srcX = faceWidth + x;
          srcY = 0 + y;
          break;
        case 3: // Bottom face
          srcX = faceWidth + x;
          srcY = 2 * faceHeight + y;
          break;
        case 4: // Front face
          srcX = faceWidth + x;
          srcY = faceHeight + y;
          break;
        case 5: // Back face
          srcX = 3 * faceWidth + x;
          srcY = faceHeight + y;
          break;
        }
        int srcIndex = (srcY * width + srcX) * channels;
        int dstIndex = (y * rect_lens + x) * channels;
        for (int c = 0; c < channels; c++) {
          faceData[i][dstIndex + c] = data[srcIndex + c];
        }
      }
    }
  }
  // Set the texture data for each face of the cube map using glTexImage2D
  for (int i = 0; i < 6; i++) {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, rect_lens,
                 rect_lens, 0, GL_RGB, GL_UNSIGNED_BYTE, faceData[i].get());
    // stbi_write_png(file_paths[i], rect_lens, rect_lens, channels,
    // faceData[i].get(),
    //                rect_lens * channels);
  }

  // Check for any OpenGL errors
  checkOpenGLError();

  return texture_id;
}
} // namespace opengltest