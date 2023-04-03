/*
 * @Author: jiyang Gui
 * @Date: 2023-03-23 18:36:19
 * @LastEditors: jiyang Gui
 * @LastEditTime: 2023-04-03 16:33:27
 * @Description:
 * guijiyang@163.com
 * Copyright (c) 2023 by jiyang Gui/GuisGame, All Rights Reserved.
 */
#include <Utils.h>
#include <easylogging++.h>
#include <format>
#include <fstream>
#include <iostream>
#include <stb_image.h>
#include <stdexcept>
#include <string>

namespace opengltest {
using namespace std;

Image::Image(const char *filename) noexcept {
  stbi_set_flip_vertically_on_load(1);
  data_ = stbi_load(filename, &width_, &height_, &channels_, STBI_rgb_alpha);
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
    data_ = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(texture->pcData) , texture->mWidth*4, &width_,
                                       &height_, &channels_, 0);
  }
  else {
  data_ = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(texture->pcData) , texture->mWidth*texture->mHeight*4, &width_,
                                       &height_, &channels_, 0);
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
GLuint createShaderProgram(const char* vert_shader_name, const char*frag_shader_name) {
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
} // namespace opengltest