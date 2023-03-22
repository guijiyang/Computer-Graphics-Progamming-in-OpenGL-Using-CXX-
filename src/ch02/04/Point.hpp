#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

static const int kNumVertices = 1;

class Point {
private:
  GLuint vertex_obj_[kNumVertices];
  GLuint render_program_;

  GLFWwindow *p_window_{nullptr};

public:
  void run() {
    initWindow();
    glfwDestroyWindow(p_window_);
    glfwTerminate();
  }

private:
  void initWindow() {
    if (!glfwInit()) {
      throw std::runtime_error("failed to initialize GLFW");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

    p_window_ = glfwCreateWindow(600, 600, "2_4", nullptr, nullptr);

    glfwMakeContextCurrent(p_window_);

    if (glewInit() != GLEW_OK) {
      throw std::runtime_error("failed to initialize GLew");
    }

    // vertical synchronizer is enabled by default
    glfwSwapInterval(1);

    init(p_window_);

    while (!glfwWindowShouldClose(p_window_)) {
      display(p_window_, glfwGetTime());
      glfwSwapBuffers(p_window_);
      glfwPollEvents();
    }
  }
  auto readFile(const char *filename) {
    string content{};
    ifstream file(filename, ios::in);
    if (!file.is_open()) {
      throw std::runtime_error("failed to open file!");
    }
    string line{};
    while (!file.eof()) {
      getline(file, line);
      content.append(line + "\n");
    }
    file.close();
    return content;
  }

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

  auto checkOpenGLError() {
    auto have_errors = false;
    auto gl_err = glGetError();
    while (gl_err != GL_NO_ERROR) {
      cout << "GL error: " << gl_err << endl;
      gl_err = glGetError();
      have_errors = true;
    }
    return have_errors;
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
    glCompileShader(vert_shader);
    checkOpenGLError();
    glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &vert_compiled);
    if (vert_compiled != 1) {
      cout << "vertex shader compilation failed" << endl;
      printShaderLog(vert_shader);
    }
    glShaderSource(frag_shader, 1, &frag_shader_cstr, nullptr);
    glCompileShader(frag_shader);
    checkOpenGLError();
    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &frag_compiled);
    if (frag_compiled != 1) {
      cout << "fragment shader compilation failed" << endl;
      printShaderLog(frag_shader);
    }
    auto vf_program = glCreateProgram();
    glAttachShader(vf_program, vert_shader);
    glAttachShader(vf_program, frag_shader);
    glLinkProgram(vf_program);
    checkOpenGLError();
    glGetProgramiv(vf_program, GL_LINK_STATUS, &linked);
    if (linked != 1) {
      cout << "link failed" << endl;
      printProgramLog(vf_program);
    }

    return vf_program;
  }

  void init(GLFWwindow *window) {
    render_program_ = createShaderProgram();
    glGenVertexArrays(kNumVertices, vertex_obj_);
    glBindVertexArray(vertex_obj_[0]);
  }

  void display(GLFWwindow *window, double currentTime) {
    glUseProgram(render_program_);
    glPointSize(30.0f);
    glDrawArrays(GL_POINTS, 0, 1);
  }
};