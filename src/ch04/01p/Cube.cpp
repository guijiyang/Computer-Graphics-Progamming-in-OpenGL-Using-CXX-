#include "Cube.h"
// #include "glm/ext/quaternion_transform.hpp"
#include <Utils.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/matrix.hpp>
#include <stdexcept>
namespace opengltest {
void Cube::setupVertices() {
  float vertex_positions[72]={
    -1.0f,0.0f,-1.0f, //left middle back
    0.0f,1.0f,0.0f,   //middle top middle 
    1.0f,0.0f,-1.0f,  //right middle back

    -1.0f,0.0f,-1.0f, //left middle back
    1.0f,0.0f,-1.0f,  //right middle back
    0.0f,-1.0f,0.0f, //middle bottom middle

    -1.0f,0.0f,-1.0f, //left middle back
    -1.0f,0.0f,1.0f, //left middle front
    0.0f,1.0f,0.0f,   //middle top middle 

    -1.0f,0.0f,-1.0f, //left middle back
    0.0f,-1.0f,0.0f, //middle bottom middle
    -1.0f,0.0f,1.0f, //left middle front

    1.0f,0.0f,1.0f, //right middle front
    1.0f,0.0f,-1.0f,  //right middle back
    0.0f,1.0f,0.0f,   //middle top middle 

    1.0f,0.0f,-1.0f,  //right middle back
    1.0f,0.0f,1.0f, //right middle front
    0.0f,-1.0f,0.0f, //middle bottom middle

    -1.0f,0.0f,1.0f, //left middle front
    1.0f,0.0f,1.0f, //right middle front
    0.0f,1.0f,0.0f,   //middle top middle 

    1.0f,0.0f,1.0f, //right middle front
    -1.0f,0.0f,1.0f, //left middle front
    0.0f,-1.0f,0.0f, //middle bottom middle
  };
  // create VAO
  glGenVertexArrays(static_cast<GLsizei>(vert_arr_obj_.size()),
                    vert_arr_obj_.data());
  glBindVertexArray(vert_arr_obj_[0]);
  // create VBO
  glGenBuffers(static_cast<GLsizei>(vert_buff_obj_.size()),
               vert_buff_obj_.data());
  glBindBuffer(GL_ARRAY_BUFFER, vert_buff_obj_[0]);
  // copy vertices into active buffer (0th VBO in this case)
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions), vertex_positions,
               GL_STATIC_DRAW);
}

void Cube::init(GLFWwindow *window) {
  rendering_program_ = createShaderProgram();
  cam_pos_.x = 0.0f;
  cam_pos_.y = 0.0f;
  cam_pos_.z = 20.0f;
  cub_pos_.x = 0.0f;
  cub_pos_.y = -2.0f;
  cub_pos_.z = 0.0f;
  setupVertices();
}

void Cube::display(GLFWwindow *window, double current_time) {
  // clear depth buffer or it may result in completely black screen.
  glClear(GL_DEPTH_BUFFER_BIT);

  // clear color buffer
  glClear(GL_COLOR_BUFFER_BIT);

  glEnable(GL_CULL_FACE);
  // enable shader by install glsl code on the GPU.
  glUseProgram(rendering_program_);

  // get the uniform variables of the MV and projection matrices
  mv_loc_ = glGetUniformLocation(rendering_program_, "mv_matrix");
  proj_loc_ = glGetUniformLocation(rendering_program_, "proj_matrix");

  // build perspective matrix
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  float aspect = static_cast<float>(width) / height;
  // auto angles=glm::radians(60.0);
  auto pmat = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 1000.0f);

  // build view matrix, model matrix, and model-view matrix
  auto vmat = glm::translate(glm::mat4(1.0f),
                             glm::vec3(-cam_pos_.x, -cam_pos_.y, -cam_pos_.z));

    // use current time to compute different translations in x,y and z
    auto tmat = glm::translate(glm::mat4(1.0f),
                               glm::vec3(sin(0.35f * current_time) * 8.0f,
                                         cos(0.52f * current_time) * 8.0f,
                                         sin(0.7f * current_time) * 8.0f));
    // rotations
    auto rmat =
        glm::rotate(glm::mat4(1.0f), 1.75f * static_cast<float>(current_time),
                    glm::vec3(0.0f, 1.0f, 0.0f));
    rmat = glm::rotate(rmat, 1.75f * static_cast<float>(current_time),
                       glm::vec3(1.0f, 0.0f, 0.0f));
    rmat = glm::rotate(rmat, 1.75f * static_cast<float>(current_time),
                       glm::vec3(0.0f, 0.0f, 1.0f));

    // auto mmat = glm::translate(glm::mat4(1.0f),
    //                            glm::vec3(cub_pos_.x, cub_pos_.y, cub_pos_.z));
    auto mmat = tmat * rmat;
    auto mvmat = vmat * mmat;

    //  copy perspective matrix and mv matrix to corresponding uniform variables
    glUniformMatrix4fv(mv_loc_, 1, GL_FALSE, glm::value_ptr(mvmat));
    glUniformMatrix4fv(proj_loc_, 1, GL_FALSE, glm::value_ptr(pmat));

    // associate VBO with the corresponding vertex attribute in the vertex
    // shader
    glBindBuffer(GL_ARRAY_BUFFER, vert_buff_obj_[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // adjust opengl settings(preform hidden surface removal) and draw model
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CCW);
    // glDrawArrays(GL_TRIANGLES, 0, 36);
    glDrawArrays(GL_TRIANGLES, 0, 24);
}

void Cube::run() {
  if (!glfwInit()) {
    throw std::runtime_error("failed to initialize glfw");
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  p_window_ = glfwCreateWindow(600, 600, "4_1p", nullptr, nullptr);
  glfwMakeContextCurrent(p_window_);
  if (glewInit() != GLEW_OK) {
    throw std::runtime_error("failed to initialize glew");
  }

  glfwSwapInterval(1);
  init(p_window_);

  while (!glfwWindowShouldClose(p_window_)) {
    glfwPollEvents();
    display(p_window_, glfwGetTime());
    glfwSwapBuffers(p_window_);
  }

  // exit succeeded
  glfwDestroyWindow(p_window_);
  glfwTerminate();
}
} // namespace opengltest