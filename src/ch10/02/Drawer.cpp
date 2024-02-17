/*
 * @Author: jiyang Gui
 * @Date: 2023-03-27 22:25:34
 * @LastEditors: jiyang Gui
 * @LastEditTime: 2023-04-13 09:33:16
 * @Description:
 * guijiyang@163.com
 * Copyright (c) 2023 by jiyang Gui/GuisGame, All Rights Reserved.
 */
#include "Drawer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>
#include <yaml-cpp/yaml.h>
#include <lua.hpp>
namespace opengltest {

static glm::mat4 kProjMat{};
static glm::mat4 kViewMat{};
static glm::mat4 kViewProjInvMat{};
static glm::mat4 kNormMat{};
static int kWidth{600};
static int kHeight{600};
static float kAspect{static_cast<float>(kWidth) / kHeight};

void Drawer::setupVertices() {
  auto indices = sphere_.getIndices();
  auto verts = sphere_.getVertices();
  auto texcoords = sphere_.getTexCoords();
  auto norms = sphere_.getNormals();
  auto tangents = sphere_.getTangents();

  auto vert_nums = sphere_.getNumVertices();
  std::vector<float> pvalues(vert_nums * 3);
  std::vector<float> tvalues(vert_nums * 2);
  std::vector<float> nvalues(vert_nums * 3);
  std::vector<float> tangvalues(vert_nums * 3);

  for (uint32_t i = 0; i < vert_nums; i++) {
    pvalues[i * 3] = verts[i].x;
    pvalues[i * 3 + 1] = verts[i].y;
    pvalues[i * 3 + 2] = verts[i].z;

    tvalues[i * 2] = texcoords[i].s;
    tvalues[i * 2 + 1] = texcoords[i].t;

    nvalues[i * 3] = norms[i].x;
    nvalues[i * 3 + 1] = norms[i].y;
    nvalues[i * 3 + 2] = norms[i].z;

    tangvalues[i * 3] = tangents[i].x;
    tangvalues[i * 3 + 1] = tangents[i].y;
    tangvalues[i * 3 + 2] = tangents[i].z;
  }

  glGenVertexArrays(static_cast<GLsizei>(vert_arr_obj_.size()),
                    vert_arr_obj_.data());
  glBindVertexArray(vert_arr_obj_[0]);
  glGenBuffers(static_cast<GLsizei>(vert_buf_obj_.size()),
               vert_buf_obj_.data());

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vert_buf_obj_[0]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * 4, indices.data(),
               GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, vert_buf_obj_[1]);
  glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, pvalues.data(),
               GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, vert_buf_obj_[2]);
  glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, tvalues.data(),
               GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, vert_buf_obj_[3]);
  glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, nvalues.data(),
               GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, vert_buf_obj_[4]);
  glBufferData(GL_ARRAY_BUFFER, tangvalues.size() * 4, tangvalues.data(),
               GL_STATIC_DRAW);
}

GLuint Drawer::setupTexture(const char *filename) {
  GLuint texture_id;
  Image image{filename};
  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);

  // Set texture parameters
  setTextureParameters();

  // Check for anisotropic filtering extension
  setAnisotropicFiltering();

  // Generate mipmaps and upload texture data
  uploadTextureData(image);

  return texture_id;
}

void Drawer::setTextureParameters() {
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Drawer::setAnisotropicFiltering() {
  if (glewIsSupported("GL_EXT_texture_filter_anisotropic")) {
    float max_anisotropy;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &max_anisotropy);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT,
                    max_anisotropy);
  }
}

void Drawer::uploadTextureData(const Image &image) {
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0,
               GL_RGBA, GL_UNSIGNED_BYTE, image.data());
  glGenerateMipmap(GL_TEXTURE_2D);
}

void Drawer::init(GLFWwindow *window) {

  // The following lines of code load the configuration settings from the
  // "config.yaml" file, which are used to determine texture filenames.
  YAML::Node config = YAML::LoadFile("config.yaml");
  auto texture_1_filename = config["texture_1_filename"].as<std::string>();
  // auto texture_2_filename = config["texture_2_filename"].as<std::string>();

  // read lua function
  // lua_State* L=lua

  // This line creates a shader program using `createShaderProgram()`, which
  // is not shown here.
  rendering_program_ = createShaderProgram();

  // This line sets up the vertices for rendering. This function is not shown
  // here, but it populates vertex buffer objects with vertex data.
  setupVertices();

  // This line sets up the texture for rendering, based on the filename from
  // the config file. It returns the ID of the generated OpenGL texture.
  texture_id_ = setupTexture(texture_1_filename.c_str());

  // These three lines set up perspective and view matrices, which define how
  // the scene is rendered. `kProjMat` defines the projection matrix for
  // projecting 3D points onto a 2D screen; `kViewMat` defines the view matrix
  // that transforms world space coordinates into camera space; and `mvmat_`
  // defines the model-view matrix, which positions and orients the objects
  // in the scene relative to the camera. Finally, `kViewProjInvMat`
  // defines an inverted transformation matrix that maps screen-space
  // coordinates back to world space coordinates (useful for picking).
  kProjMat = glm::perspective(glm::radians(60.0f), kAspect, 0.1f, 1000.0f);
  kViewMat = glm::translate(glm::mat4(1.0f),
                         glm::vec3(-cam_pos_.x, -cam_pos_.y, -cam_pos_.z));
  mvmat_ = kViewMat *
           glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) *
           glm::rotate(glm::mat4(1.f), glm::radians(25.f),
                       glm::vec3(1.0f, 0.0f, 0.0f));
  kNormMat = glm::transpose(glm::inverse(mvmat_));
  kViewProjInvMat = glm::inverse(kProjMat * kViewMat);
}

// displaying a sphere with lighting
void Drawer::display(GLFWwindow *window, double current_time) {
  clearBuffers(); // Clear depth and color buffers
  glUseProgram(rendering_program_);
  setMatrices(current_time); // Set the projection and model-view matrices
  setLightDirection(window); // Set the direction of the light source
  activateTexture(); // Activate the texture and bind it to a texture unit
  // pushViewMatrix(); // Push the view matrix onto the stack

  // Bind vertex buffer objects and enable vertex attributes
  enableVertexAttribute(0, 3, vert_buf_obj_[1]);
  enableVertexAttribute(1, 2, vert_buf_obj_[2]);
  enableVertexAttribute(2, 3, vert_buf_obj_[3]);
  enableVertexAttribute(3, 3, vert_arr_obj_[4]);

  enableDepthTest(); // Enable depth testing

  drawSphere(); // Draw the sphere
}

// Clear depth and color buffers
void Drawer::clearBuffers() {
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

/**
 * @brief set light and material properties
 *
 * @param window
 */
void Drawer::setLightDirection(GLFWwindow *window) {
  double mouseX, mouseY;
  glfwGetCursorPos(window, &mouseX, &mouseY);

  // Convert the mouse position from screen coordinates to normalized device
  // coordinates
  glm::vec3 ndc = glm::vec3((2.0f * mouseX) / kWidth - 1.0f,
                            1.0f - (2.0f * mouseY) / kHeight, 0.0f);
  // Convert the NDC coordinates to world coordinates using the inverse
  // view-projection matrix
  glm::vec4 worldPos = kViewProjInvMat * glm::vec4(ndc, 1.0f);
  worldPos /= worldPos.w;

  // set up lights to The resulting world position is in the last three
  // components of the `worldPos` vector
  current_light_pos_ = glm::vec3(worldPos);

  installLights();
}

void Drawer::installLights() {
  // convert light's position to view space, and save it in a float array
  light_pos_v = glm::vec3(kViewMat * glm::vec4(current_light_pos_, 1.0));

  // set the uniform light and material values in the shader
  static auto kGlobalAmbientLoc =
      glGetUniformLocation(rendering_program_, "globalAmbient");
  glProgramUniform4fv(rendering_program_, kGlobalAmbientLoc, 1,
                      global_ambient_.data());
  static auto kLightAmbientLoc =
      glGetUniformLocation(rendering_program_, "light.ambient");
  glProgramUniform4fv(rendering_program_, kLightAmbientLoc, 1,
                      light_ambient_.data());
  static auto kLightDiffuseLoc =
      glGetUniformLocation(rendering_program_, "light.diffuse");
  glProgramUniform4fv(rendering_program_, kLightDiffuseLoc, 1,
                      light_diffuse_.data());
  static auto kLightSpecularLoc =
      glGetUniformLocation(rendering_program_, "light.specular");
  glProgramUniform4fv(rendering_program_, kLightSpecularLoc, 1,
                      light_specular_.data());
  static auto kLightPositionLoc =
      glGetUniformLocation(rendering_program_, "light.position");
  glProgramUniform3fv(rendering_program_, kLightPositionLoc, 1,
                      glm::value_ptr(light_pos_v));
  static auto kMaterialAmbientLoc =
      glGetUniformLocation(rendering_program_, "material.ambient");
  glProgramUniform4fv(rendering_program_, kMaterialAmbientLoc, 1,
                      bronzeAmbient().data());
  static auto kMaterialDiffuseLoc =
      glGetUniformLocation(rendering_program_, "material.diffuse");
  glProgramUniform4fv(rendering_program_, kMaterialDiffuseLoc, 1,
                      bronzeDiffuse().data());
  static auto kMaterialSpecularLoc =
      glGetUniformLocation(rendering_program_, "material.specular");
  glProgramUniform4fv(rendering_program_, kMaterialSpecularLoc, 1,
                      bronzeSpecular().data());
  static auto kMaterialShininessLoc =
      glGetUniformLocation(rendering_program_, "material.shininess");
  glProgramUniform1f(rendering_program_, kMaterialShininessLoc,
                     bronzeShininess());
}

void Drawer::setMatrices(double current_time) {
  // caches the location of the "proj_mat" uniform variable
  // in the shader program, so that it can be accessed efficiently later.
  static auto kProjMatLoc =
      glGetUniformLocation(rendering_program_, "proj_mat");
  // sets the value of the "proj_mat" matrix uniform variable
  glUniformMatrix4fv(kProjMatLoc, 1, GL_FALSE, glm::value_ptr(kProjMat));

  // cache location of the "mv_mat" uniform variable in the shader program
  static auto kMvMatLoc = glGetUniformLocation(rendering_program_, "mv_mat");
  // sets the value of the "mv_mat" matrix uniform variable
  glUniformMatrix4fv(kMvMatLoc, 1, GL_FALSE, glm::value_ptr(mvmat_));

  // build the inverse-transpose of the MV matrix by concatenating matrices v
  // and m, as before
  static auto kNormMatrixLoc = glGetUniformLocation(rendering_program_, "norm_matrix");
  glUniformMatrix4fv(kNormMatrixLoc, 1,
                     GL_FALSE, glm::value_ptr(kNormMat));
}

// Activate the texture and bind it to a texture unit
void Drawer::activateTexture() {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture_id_);
}

void Drawer::enableVertexAttribute(GLuint location, GLint size,
                                   GLuint vertex_obj) {
  glBindBuffer(GL_ARRAY_BUFFER, vertex_obj);
  glVertexAttribPointer(location, size, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(location);
}

// Enable depth testing
void Drawer::enableDepthTest() {
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
}

// Draw the sphere
void Drawer::drawSphere() {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vert_buf_obj_[0]);
  glDrawElements(GL_TRIANGLES, sphere_.getNumIndices(), GL_UNSIGNED_INT, 0);
}

void Drawer::run(const char *filename) {
  if (!glfwInit()) {
    throw std::runtime_error("failed to initialize glfw");
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  auto p_window = glfwCreateWindow(kWidth, kHeight, filename, nullptr, nullptr);
  glfwMakeContextCurrent(p_window);

  if (glewInit() != GLEW_OK) {
    throw std::runtime_error("failed to intilialize glew!");
  }

  glfwSwapInterval(1);

  auto window_reshape_callback = [](GLFWwindow *window, int new_width, int new_height) {
    kWidth = new_width;
    kHeight = new_height;
    kAspect = static_cast<float>(kWidth) / kHeight;
    kProjMat = glm::perspective(glm::radians(60.0f), kAspect, 0.1f, 1000.0f);
    kViewProjInvMat = glm::inverse(kProjMat * kViewMat);
    glViewport(0, 0, kWidth, kHeight);
  };

  glfwSetWindowSizeCallback(p_window, window_reshape_callback);
  init(p_window);

  while (!glfwWindowShouldClose(p_window)) {
    glfwPollEvents();
    display(p_window, glfwGetTime());
    glfwSwapBuffers(p_window);
  }
}

// void Drawer::windowReshapeCallback(GLFWwindow *window, int new_width,
//                                    int new_height) {
//   kWidth = new_width;
//   kHeight = new_height;
//   kAspect = static_cast<float>(kWidth) / kHeight;
//   kProjMat = glm::perspective(glm::radians(60.0f), kAspect, 0.1f, 1000.0f);
//   kViewProjInvMat=glm::inverse(kProjMat*kViewMat);
//   glViewport(0, 0, kWidth, kHeight);
// }

} // namespace opengltest