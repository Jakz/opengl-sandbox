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
#include "Camera.h"
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

static Camera* camera = new Camera();

/*typedef void (PFNGLGETSHADERIVPROC)(GLuint shader, GLenum pname, GLint *params);
typedef void (PFNGLGETSHADERINFOLOGPROC)(GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog);*/

struct Data
{
  GLuint vertexBuffer;
  GLuint colorBuffer;
  GLuint elementBuffer;
  
  GLuint vao;
  
  Program* program;
  
  GLint position;
  GLint color;
  GLint ptimer;
  
  GLint pmatrixLoc;
  GLint vmatrixLoc;
  GLint mmatrixLoc;
  
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

static const GLfloat vertexData[] = {
  -0.5f, -0.5f, -0.0f, 1.0f,
  0.5f, -0.5f, -0.0f, 1.0f,
  -0.5f, 0.5f, -0.0f, 1.0f,
  0.5f, 0.5f, -0.0f, 1.0f
};

static const GLfloat colorData[] = {
  1.0f, 0.0f, 0.0f, 1.0f,
  1.0f, 1.0f, 0.0f, 1.0f,
  1.0f, 0.0f, 1.0f, 1.0f,
  0.0f, 1.0f, 0.0f, 1.0f
};

static const GLushort bufferData[] = { 0,1,2,3};

static void allocateResources()
{
  Shader *vertex = ShaderCache::compileVertexShader("shader.v.glsl");
  Shader *fragment = ShaderCache::compileFragmentShader("shader.f.glsl");
  
  data.program = ShaderCache::linkProgramOnce(vertex, fragment);
  
  data.position = glGetAttribLocation(data.program->ident, "a_position");
  data.color = glGetAttribLocation(data.program->ident, "a_color");
  data.ptimer = glGetUniformLocation(data.program->ident, "u_timer");
  
  data.pmatrixLoc = glGetUniformLocation(data.program->ident, "pMatrix");
  data.vmatrixLoc = glGetUniformLocation(data.program->ident, "vMatrix");
  data.mmatrixLoc = glGetUniformLocation(data.program->ident, "mMatrix");
  
  
  glGenVertexArrays(1, &data.vao);
  glBindVertexArray(data.vao);
  
  glGenBuffers(1, &data.vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, data.vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
  glEnableVertexAttribArray(data.position);
  glVertexAttribPointer(data.position, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glGenBuffers(1, &data.colorBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, data.colorBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_STATIC_DRAW);
  glEnableVertexAttribArray(data.color);
  glVertexAttribPointer(data.color, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
  
  glGenBuffers(1, &data.elementBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.elementBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(bufferData), bufferData, GL_STATIC_DRAW);
  
  glBindVertexArray(0);
}

void callBackResizedWindow(GLFWwindow *window, int width, int height)
{
  camera->adjustRatio(static_cast<float>(width)/height);
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
  
  glfwSetKeyCallback(window, key_callback);
  glfwSetFramebufferSizeCallback(window, callBackResizedWindow);
  
  glewExperimental = true;
  
  GLenum glewResult = glewInit();
  if (glewResult != GLEW_OK)
    exit(EXIT_FAILURE);

  allocateResources();
  
  glBindBuffer(GL_ARRAY_BUFFER, data.vertexBuffer);
  glEnableVertexAttribArray(data.position);

  glEnable(GL_DEPTH_TEST);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glDepthMask(GL_TRUE);
  glDepthFunc(GL_LESS);
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
  
  glm::mat4 projectionMatrix;
  glm::mat4 viewMatrix;
  glm::mat4 modelMatrix;
  
  
  projectionMatrix = glm::perspective(50.0f, 800.0f/600.0f, 0.1f, 100.0f);
  viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.0f));
  
  camera->setProjection(50.0f, 800.0f/600.0f, 0.1, 100.0f);
  camera->lookAt(glm::vec3(0.0f,0.0f,1.0f), glm::vec3(0.0f), glm::vec3(0,1,0));
  
  
  glm::vec3 v1 = glm::vec3(-0.5f, -0.5f, -0.0f);
  glm::vec3 v2 = glm::vec3(0.5f, -0.5f, -0.0f);
  glm::vec3 v3 = glm::vec3(-0.5f, 0.5f, -0.0f);
  
  glm::vec3 t1 = v2-v1;
  glm::vec3 t2 = v3-v1;
  glm::vec3 normal = - glm::normalize(glm::cross(t2, t1));
  glm::vec4 pts[] = {glm::vec4(glm::vec3(0.0f), 1.0f), glm::vec4(normal*0.5f, 1.0f)};
  
  GLuint vao2;
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
  
  glBindVertexArray(0);
  

  //glm::mat4 pvMatrix = projectionMatrix * viewMatrix;
  
  while (!glfwWindowShouldClose(window))
  {
    data.timer = glfwGetTime();
    
    modelMatrix = glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f)), data.timer*50.0f, glm::vec3(1.0,1.0,1.0));
    
    float ratio;
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float) height;
    
    glViewport(0, 0, width, height);
    
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    data.program->use();
    glUniform1f(data.ptimer, data.timer);
    
    glUniformMatrix4fv(data.pmatrixLoc, 1, GL_FALSE, &camera->projectionMatrix()[0][0]);
    glUniformMatrix4fv(data.vmatrixLoc, 1, GL_FALSE, &camera->cameraMatrix()[0][0]);
    glUniformMatrix4fv(data.mmatrixLoc, 1, GL_FALSE, &modelMatrix[0][0]);
    
    glBindVertexArray(data.vao);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, (void*)0);
    
    glBindVertexArray(vao2);
    glDrawElements(GL_LINES, 2, GL_UNSIGNED_SHORT, (void*)0);

    
    
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  
  glfwTerminate();
  return 0;
}

