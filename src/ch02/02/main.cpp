#include <GLFW/glfw3.h>
// #include <gl/GL.h>
#include <fstream>
#include <gl/glew.h>
#include <ios>
#include <iostream>
#include <stdlib.h>
#include <string>

using std::cout;
using std::endl;
using std::ifstream;
using std::string;

const int KNumVAOs = 1;
GLuint renderingProgram;
GLuint vao[KNumVAOs];

GLfloat point_size = 30.0f;
GLfloat gs = 1.0f;

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

GLuint createShaderProgram() {
  GLint vertCompiled;
  GLint fragCompiled;
  GLint linked;
  const char *vshaderSource = "#version 450 \n"
                              "void main(void) \n"
                              "{ gl_Position = vec4(0.0, 0.0, 0.0, 1.0);}";
  const char *fshaderSource = "#version 450 \n"
                              "out vec4 color; \n"
                              "void main(void) \n"
                              "{ color = vec4(0.0, 0.0, 1.0, 1.0); }";
  GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
  GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(vShader, 1, &vshaderSource, nullptr);
  glShaderSource(fShader, 1, &fshaderSource, nullptr);
  glCompileShader(vShader);
  checkOpenGLError();
  glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
  if (vertCompiled != 1) {
    cout << "vertex shader compilation failed" << endl;
    printShaderLog(vShader);
  }
  glCompileShader(fShader);
  checkOpenGLError();
  glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
  if (fragCompiled != 1) {
    cout << "fragment shader compilation failed!" << endl;
    printShaderLog(fShader);
  }
  GLuint vfProgram = glCreateProgram();
  glAttachShader(vfProgram, vShader);
  glAttachShader(vfProgram, fShader);
  glLinkProgram(vfProgram);
  checkOpenGLError();
  glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
  if (linked != 1) {
    cout << "linking failed!" << endl;
    printProgramLog(vfProgram);
  }
  return vfProgram;
}

void init(GLFWwindow *window) {
  renderingProgram = createShaderProgram();

  // VAO : Vertex Array Objects, OpenGL requires at least one VAO
  glGenVertexArrays(KNumVAOs, vao);
  glBindVertexArray(vao[0]);
}

void display(GLFWwindow *window, double currentTime) {
  glClear(GL_DEPTH_BUFFER_BIT);
  glClearColor(0.0, 0.0, 0.0, 1);
  glClear(GL_COLOR_BUFFER_BIT); // clear background to black, each frame
  // loads the program containing the two compiled shaders into the OpenGL
  // pipeline stages (onto the GPU)
  glUseProgram(renderingProgram);
  glPointSize(point_size);
  cout<<point_size<<endl;
  point_size += gs;
  if (point_size >= 50.0f) {
    gs = -1.0f;
  } else if (point_size <= 5.0f) {
    gs = 1.0f;
  }

  // initiates pipeline processing
  // mode: GL_POINTS, from 0, one (point)
  glDrawArrays(GL_POINTS, 0, 1);
}

int main(int argc, char **argv) {
  if (!glfwInit()) {
    exit(EXIT_FAILURE);
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  auto p_window = glfwCreateWindow(800, 600, "2_2", nullptr, nullptr);
  glfwMakeContextCurrent(p_window);
  if (glewInit() != GLEW_OK) {
    exit(EXIT_FAILURE);
  }
  //   Vertical synchronization (VSync) is enabled
  glfwSwapInterval(1);
  init(p_window);
  while (!glfwWindowShouldClose(p_window)) {
    glfwPollEvents();
    display(p_window, glfwGetTime());
    glfwSwapBuffers(p_window);
  }

  glfwDestroyWindow(p_window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
