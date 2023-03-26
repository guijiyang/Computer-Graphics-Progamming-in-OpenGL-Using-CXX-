/*
 * @Author: jiyang Gui
 * @Date: 2023-03-23 18:36:19
 * @LastEditors: jiyang Gui
 * @LastEditTime: 2023-03-25 22:11:15
 * @Description:
 * guijiyang@163.com
 * Copyright (c) 2023 by jiyang Gui/GuisGame, All Rights Reserved.
 */
#include <Utils.h>
#include <easylogging++.h>
#include <format>
#include <fstream>
#include <iostream>

namespace opengltest {
using namespace std;
void printShaderLog(GLuint shader) {
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

string readFile(const char *filename) {
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
GLuint createShaderProgram() {
  // register utils logger
  el::Loggers::getLogger("utils");

  GLint vert_compiled;
  GLint frag_compiled;
  GLint linked;
  auto vert_shader_str = readFile("shader.vert");
  auto vert_shader_cstr = vert_shader_str.c_str();
  auto frag_shader_str = readFile("shader.frag");
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
    CLOG(WARNING, "utils")<<"linked failed!";
    // cout << "linked failed!" << endl;
    printProgramLog(vf_program);
  }

  return vf_program;
}
} // namespace opengltest