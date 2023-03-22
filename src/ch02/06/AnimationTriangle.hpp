/*
 * @Author: jiyang Gui
 * @Date: 2023-03-22 14:12:01
 * @LastEditors: jiyang Gui
 * @LastEditTime: 2023-03-23 14:39:50
 * @Description: 
 * guijiyang@163.com
 * Copyright (c) 2023 by jiyang Gui/GuisGame, All Rights Reserved. 
 */
/*
 * @Author: jiyang Gui
 * @Date: 2023-03-22 10:16:28
 * @LastEditors: jiyang Gui
 * @LastEditTime: 2023-03-22 12:06:25
 * @Description:
 * guijiyang@163.com
 * Copyright (c) 2023 by jiyang Gui/GuisGame, All Rights Reserved.
 */

#include <GLFW/glfw3.h>
#include <format>
#include <fstream>
#include <gl/glew.h>
#include <iostream>
#include <stdexcept>
#include <string>

static const int kNumVertices = 1;
using namespace std;
class Triangle {
  private:
  GLuint vert_obj_[kNumVertices];
  GLuint render_image_;
  GLFWwindow* p_window_;

  float x{0.0f}; //location of triangle on x axis
  float inc{0.01f}; //offset for moving the triangle

private:
  void printShaderLog(GLuint shader) {
    int len = 0;
    int chWrittn = 0;
    char *log;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    if (len > 0) {
      log = (char *)malloc(len);
      glGetShaderInfoLog(shader, len, &chWrittn, log);
      cout << "Shader Info Log: " << log << endl;
      free(log);
    }
  }
  void printProgramLog(int prog) {
    int len = 0;
    int chWrittn = 0;
    char *log;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
    if (len > 0) {
      log = (char *)malloc(len);
      glGetProgramInfoLog(prog, len, &chWrittn, log);
      cout << "Program Info Log: " << log << endl;
      free(log);
    }
  }
  bool checkOpenGLError() {
    bool foundError = false;
    int glErr = glGetError();
    while (glErr != GL_NO_ERROR) {
      cout << "glError: " << glErr << endl;
      foundError = true;
      glErr = glGetError();
    }
    return foundError;
  }

  auto readFile(const char *filename) {
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
  auto createShaderProgram() {
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
      cout << "vertex shader compilation failed!" << endl;
      printShaderLog(vert_shader);
    }
    glCompileShader(frag_shader);
    checkOpenGLError();
    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &frag_compiled);
    if (frag_compiled != 1) {
      cout << "frag shader compilaton failed!" << endl;
      printShaderLog(frag_shader);
    }
    auto vf_program = glCreateProgram();
    glAttachShader(vf_program, vert_shader);
    glAttachShader(vf_program, frag_shader);
    glLinkProgram(vf_program);
    checkOpenGLError();
    glGetProgramiv(vf_program, GL_LINK_STATUS, &linked);
    if (linked != 1) {
      cout << "linked failed!" << endl;
      printProgramLog(vf_program);
    }

    return vf_program;
  }
  auto init(GLFWwindow *window) {
    render_image_ = createShaderProgram();
    glGenVertexArrays(kNumVertices, vert_obj_);
    glBindVertexArray(vert_obj_[0]);
  }

  auto display(GLFWwindow *window, double current_time) {
    glClear(GL_DEPTH_BUFFER_BIT);  // every frame first clear the depth buffer
    glClearColor(0.0, 0.0, 0.0, 1);
    glClear(GL_COLOR_BUFFER_BIT); // clear background to black, each frame

    glUseProgram(render_image_);

    x+=inc;
    if(x>1.0f) inc=-0.01f;
    if(x<-1.0f) inc=0.01f;
    auto offset_loc=glGetUniformLocation(render_image_, "offset"); // get ptr to "offset"
    glProgramUniform1f(render_image_, offset_loc,x); // send value in "x" to "offset"
    glDrawArrays(GL_TRIANGLES, 0, 3);
    
  }

  public:
  void run(){
    if(!glfwInit())
    {
      throw std::runtime_error("glfw init failed!");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    p_window_=glfwCreateWindow(600, 600, "2_6", nullptr, nullptr);
    glfwMakeContextCurrent(p_window_);

    if(glewInit()!=GLEW_OK)
    {
      throw std::runtime_error("glew init failed!");
    }

    glfwSwapInterval(1);

    init(p_window_);

    while(!glfwWindowShouldClose(p_window_))
    {
      glfwPollEvents();
      display(p_window_, glfwGetTime());
      glfwSwapBuffers(p_window_);
    }

  }
};