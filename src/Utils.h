/*
 * @Author: jiyang Gui
 * @Date: 2023-03-23 15:20:19
 * @LastEditors: jiyang Gui
 * @LastEditTime: 2023-03-25 20:59:33
 * @Description:
 * guijiyang@163.com
 * Copyright (c) 2023 by jiyang Gui/GuisGame, All Rights Reserved.
 */

#pragma once
#include <gl/glew.h>
#include <string>
namespace opengltest {
static void printShaderLog(GLuint shader);
static void printProgramLog(int prog);
static bool checkOpenGLError();
std::string readFile(const char *filename);
GLuint createShaderProgram();
} // namespace opengltest