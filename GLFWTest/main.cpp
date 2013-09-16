//
//  main.cpp
//  GLFWTest
//
//  Created by Jack on 9/10/13.
//  Copyright (c) 2013 Jack. All rights reserved.
//


#include "Includes.h"


#include "FileUtils.h"
#include "ShaderCache.h"
#include "Model.h"

#include "Image.h"

#include <iostream>



static void error_callback(int error, const char* description)
{
  fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}

static Renderer* renderer = new Renderer();

/*typedef void (PFNGLGETSHADERIVPROC)(GLuint shader, GLenum pname, GLint *params);
typedef void (PFNGLGETSHADERINFOLOGPROC)(GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog);*/

struct Data
{
  GLuint vertexBuffer;
  GLuint colorBuffer;
  GLuint elementBuffer;
  
  GLuint vao;
  
  Program* program;
  
  Image *image;
  Texture *texture;
  
  float timer;
  
} data;


static GLuint createBuffer(GLenum kind, const void *data, GLsizei size)
{
  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(kind, buffer);
  glBufferData(kind, size, data, GL_STATIC_DRAW);
  
  return buffer;
}

static const GLfloat colorData[] = {
  1.0f, 0.0f, 0.0f, 1.0f, // 0 red
  1.0f, 1.0f, 0.0f, 1.0f, // 1 yellow
  1.0f, 0.0f, 1.0f, 1.0f, // 2 magenta
  0.0f, 1.0f, 0.0f, 1.0f, // 3 green
  0.0f, 0.0f, 1.0f, 1.0f, // 4 blue
  0.0f, 1.0f, 1.01, 1.0f, // 5 cyan
  0.6f, 0.2f, 0.9f, 1.0f, // 6 gray
  0.4f, 0.8f, 0.2f, 1.0f  // 7 black
};

static const GLushort bufferData[] = {
  0,1,2,1,2,3,
  4,5,6,5,6,7,
  0,5,4,0,1,5,
  2,3,6,3,6,7,
  3,5,7,3,5,1,
  4,6,0,2,6,0,
};

using namespace glm;

static void allocateResources()
{
  data.image = new Image("tile.png");
  data.texture = Texture::generate(data.image, GL_NEAREST);
  
  Shader *vertex = ShaderCache::compileVertexShader("shader.v.glsl");
  Shader *fragment = ShaderCache::compileFragmentShader("shader.f.glsl");
  
  ShaderCache::compileAndLink("position_color", "position_color.v.glsl", "position_color.f.glsl");
  
  Program *program = ShaderCache::program("position_color");
  program->enableUniform("pMatrix", UNIFORM_MATRIX_PROJECTION);
  program->enableUniform("vMatrix", UNIFORM_MATRIX_VIEW);
  program->enableUniform("mMatrix", UNIFORM_MATRIX_MODEL);
  program->enableAttrib("a_position", ATTRIB_POSITION);
  program->enableAttrib("a_color", ATTRIB_COLOR);
  
  InstanceLines* lines = new InstanceLines(GL_LINES, ShaderCache::program("position_color"));
  lines->translate(0.0f, 0.0f, -5.0f);
  lines->addVertices(glm::vec4(-0.5f,0.0f,-0.5f,1.0f), glm::vec4(0.5f,0.0f,-0.5f,1.0f), glm::vec4(1.0f,0.0f,0.0f,1.0f), glm::vec4(0.0f,0.0f,1.0f,1.0f));
  lines->mapBuffers();
  renderer->addInstance(lines);
  
  
  data.program = ShaderCache::linkProgram(vertex, fragment);
  ShaderCache::linkProgram(data.program);
  data.program->enableAttrib("a_position", ATTRIB_POSITION);
  data.program->enableAttrib("a_color", ATTRIB_COLOR);
  data.program->enableAttrib("a_texCoord", ATTRIB_TEX_COORDS);
  data.program->enableAttrib("a_normal", ATTRIB_NORMAL);

  data.program->enableUniform("u_timer", UNIFORM_TIMER);
  data.program->enableUniform("pMatrix", UNIFORM_MATRIX_PROJECTION);
  data.program->enableUniform("vMatrix", UNIFORM_MATRIX_VIEW);
  data.program->enableUniform("mMatrix", UNIFORM_MATRIX_MODEL);
  data.program->enableUniform("tex", UNIFORM_TEXTURE);

  glGenVertexArrays(1, &data.vao);
  glBindVertexArray(data.vao);
  
  std::vector<glm::vec4> vertices;
  std::vector<glm::vec2> texCoords;
  std::vector<glm::vec3> normals;
  
  vec2 coords[6] = { vec2(0.0f,0.0f), vec2(1.0f,0.0f), vec2(0.0f,1.0f), vec2(1.0f,0.0f), vec2(0.0f,1.0f), vec2(1.0f,1.0f) };
  
  glm::vec4 face[6] = {
    glm::vec4(-1.0f,-1.0f,-1.0f,1.0f),
    glm::vec4(1.0f,-1.0f,-1.0f,1.0f),
    glm::vec4(-1.0f,1.0f,-1.0f,1.0f),
    
    glm::vec4(1.0f,-1.0f,-1.0f,1.0f),
    glm::vec4(-1.0f,1.0f,-1.0f,1.0f),
    glm::vec4(1.0f,1.0f,-1.0f,1.0f)
  };
  
  vec3 normal = vec3(0.0f,0.0f,1.0f);
  
  glm::mat4 rotations[6] = {
    mat4(),
    rotate(mat4(), 180.0f, vec3(0.0f,1.0f,0.0f)),
    rotate(mat4(), 90.0f, vec3(0.0f,1.0f,0.0f)),
    rotate(mat4(), 270.0f, vec3(0.0f,1.0f,0.0f)),
    rotate(mat4(), 90.0f, vec3(1.0f,0.0f,0.0f)),
    rotate(mat4(), 270.0f, vec3(1.0f,0.0f,0.0f))
  };
  
  for (int j = 0; j < 6; ++j)
  {
    for (int i = 0; i < 6; ++i)
    {
      vertices.push_back(rotations[j]*face[i]);
      texCoords.push_back(coords[i]);
      normals.push_back(vec3(rotations[j]*vec4(normal,1.0f)));
    }
  }
  
  glGenBuffers(1, &data.vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, data.vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(vec4), &vertices[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(data.program->attrib(ATTRIB_POSITION));
  glVertexAttribPointer(data.program->attrib(ATTRIB_POSITION), 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
  
  GLuint normalVBO;
  glGenBuffers(1, &normalVBO);
  glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
  glBufferData(GL_ARRAY_BUFFER, normals.size()*sizeof(vec3), &normals[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(data.program->attrib(ATTRIB_NORMAL));
  glVertexAttribPointer(data.program->attrib(ATTRIB_NORMAL), 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  

  /*glGenBuffers(1, &data.colorBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, data.colorBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_STATIC_DRAW);
  glEnableVertexAttribArray(data.program->attrib(ATTRIB_COLOR));
  glVertexAttribPointer(data.program->attrib(ATTRIB_COLOR), 4, GL_FLOAT, GL_FALSE, 0, (void*)0);*/
  
  GLuint texCoordsP;
  glGenBuffers(1, &texCoordsP);
  glBindBuffer(GL_ARRAY_BUFFER, texCoordsP);
  glBufferData(GL_ARRAY_BUFFER, texCoords.size()*sizeof(vec2), &texCoords[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(data.program->attrib(ATTRIB_TEX_COORDS));
  glVertexAttribPointer(data.program->attrib(ATTRIB_TEX_COORDS), 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
  
  /*glGenBuffers(1, &data.elementBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.elementBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(bufferData), bufferData, GL_STATIC_DRAW);*/
  
  glBindVertexArray(0);
}

void callBackResizedWindow(GLFWwindow *window, int width, int height)
{
  renderer->camera()->adjustRatio(static_cast<float>(width)/height);
}

int main(int argc, const char * argv[])
{
  GLFWwindow* window;
  
  /* Initialize the library */
  if (!glfwInit())
    exit(EXIT_FAILURE);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, 4);
  
  glfwSetErrorCallback(error_callback);
  
  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(800, 600, "Test", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  
  /* Make the window's context current */
  glfwMakeContextCurrent(window);
  
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
  glfwSetCursorPos(window, 0, 0);
  
  glfwSetKeyCallback(window, key_callback);
  glfwSetFramebufferSizeCallback(window, callBackResizedWindow);
  
  glewExperimental = true;
  
  GLenum glewResult = glewInit();
  if (glewResult != GLEW_OK)
    exit(EXIT_FAILURE);

  allocateResources();

  glEnable(GL_DEPTH_TEST);
  glClearColor(0.9f, 0.9f, 0.9f, 0.9f);
  glDepthMask(GL_TRUE);
  glDepthFunc(GL_LESS);
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
  
  glm::mat4 modelMatrix, baseModelMatrix;

  renderer->camera()->setProjection(50.0f, 800.0f/600.0f, 0.1, 100.0f);
  renderer->camera()->lookAt(glm::vec3(0.0f,0.0f,1.0f), glm::vec3(0.0f), glm::vec3(0,1,0));
  
  
  glm::vec3 v1 = glm::vec3(-0.5f, -0.5f, -0.0f);
  glm::vec3 v2 = glm::vec3(0.5f, -0.5f, -0.0f);
  glm::vec3 v3 = glm::vec3(-0.5f, 0.5f, -0.0f);
  
  glm::vec3 t1 = v2-v1;
  glm::vec3 t2 = v3-v1;
  glm::vec3 normal = - glm::normalize(glm::cross(t2, t1));
  glm::vec4 pts[] = {glm::vec4(glm::vec3(0.0f), 1.0f), glm::vec4(normal*0.5f, 1.0f)};
  
  /*GLuint vao2;
  GLuint bfs[3];
  glGenVertexArrays(1, &vao2);
  glBindVertexArray(vao2);
    
  glGenBuffers(3, &bfs[0]);
  glBindBuffer(GL_ARRAY_BUFFER, bfs[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(pts), &pts, GL_STATIC_DRAW);
  glEnableVertexAttribArray(data.position);
  glVertexAttribPointer(data.position, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
  
  GLfloat ccc[] = { 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f};
  
  glBindBuffer(GL_ARRAY_BUFFER, bfs[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(ccc), ccc, GL_STATIC_DRAW);
  glEnableVertexAttribArray(data.color);
  glVertexAttribPointer(data.color, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
  
  GLushort iii[] = {0,1};
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bfs[2]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(iii), iii, GL_STATIC_DRAW);
  
  glBindVertexArray(0);*/
  

  //glm::mat4 pvMatrix = projectionMatrix * viewMatrix;
  
  double mouseX, mouseY;
  double mouseXo = 400, mouseYo = 300;
  
  
  float ratio;
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  ratio = width / (float) height;
  glViewport(0, 0, width, height);
  
  while (!glfwWindowShouldClose(window))
  {
    data.timer = glfwGetTime();
    
    baseModelMatrix = glm::rotate(glm::translate(glm::scale(glm::mat4(1.0f),glm::vec3(0.5f,0.5f,0.5f)), glm::vec3(0.0f, 0.0f, -3.0f)), data.timer*50.0f, glm::vec3(1.0,1.0,1.0));

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    //renderer->render();
    
    data.program->use();
    
    data.program->setUniform<GLfloat>(UNIFORM_TIMER, data.timer);
    data.program->setUniform<glm::mat4>(UNIFORM_MATRIX_PROJECTION, renderer->camera()->projectionMatrix());
    data.program->setUniform<glm::mat4>(UNIFORM_MATRIX_VIEW, renderer->camera()->cameraMatrix());
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, data.texture->ident);
    data.program->setUniform(data.program->uniform(UNIFORM_TEXTURE), 0);
    
    glBindVertexArray(data.vao);
    modelMatrix = baseModelMatrix;
    data.program->setUniform<glm::mat4>(UNIFORM_MATRIX_MODEL, modelMatrix);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_SHORT, (void*)0);
    
    modelMatrix = glm::rotate(baseModelMatrix, 90.0f, glm::vec3(0.0f,1.0f,0.0f));
    data.program->setUniform<glm::mat4>(UNIFORM_MATRIX_MODEL, modelMatrix);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_SHORT, (void*)0);

    modelMatrix = glm::rotate(baseModelMatrix, 90.0f, glm::vec3(1.0f,0.0f,0.0f));
    data.program->setUniform<glm::mat4>(UNIFORM_MATRIX_MODEL, modelMatrix);
    
    glDrawArrays(GL_TRIANGLES, 0, 12*6);
    
    //glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_SHORT, (void*)0);
    
    
    glBindVertexArray(0);
    
    
    
    
    
    
    
    
    
    
    
    //GLenum error = glGetError();
    //const u8 *errors = gluErrorString(error);
    /*glBindVertexArray(vao2);
    glDrawElements(GL_LINES, 2, GL_UNSIGNED_SHORT, (void*)0);*/
    //error = glGetError();
    //errors = gluErrorString(error);
    
    glfwSwapBuffers(window);
    glfwPollEvents();
    
    /*glfwGetCursorPos(window, &mouseX, &mouseY);
    if (mouseX >= 0 && mouseX <= 800 && mouseY >= 0 && mouseY < 600)
    {
      double mouseXd = mouseX - mouseXo, mouseYd = mouseY - mouseYo;
      camera->rotate(mouseXd/5.0f, mouseYd/5.0f);
      
      mouseXo = mouseX;
      mouseYo = mouseY;
    }*/
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
      glm::vec3 forward = renderer->camera()->directionForward();
      renderer->camera()->translate(forward*0.03f);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
      glm::vec3 forward = renderer->camera()->directionForward();
      renderer->camera()->translate(-forward*0.03f);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
      glm::vec3 right = renderer->camera()->directionRight();
      renderer->camera()->translate(-right*0.03f);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
      glm::vec3 right = renderer->camera()->directionRight();
      renderer->camera()->translate(right*0.03f);
    }
  }
  
  glfwTerminate();
  return 0;
}

