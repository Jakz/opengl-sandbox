//
//  main.cpp
//  GLFWTest
//
//  Created by Jack on 9/10/13.
//  Copyright (c) 2013 Jack. All rights reserved.
//

#define GL3_PROTOTYPES 1

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm.hpp"
#include "matrix_transform.hpp"


static void error_callback(int error, const char* description)
{
  fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}

/*typedef void (PFNGLGETSHADERIVPROC)(GLuint shader, GLenum pname, GLint *params);
typedef void (PFNGLGETSHADERINFOLOGPROC)(GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog);*/

static void show_info_log(GLuint object, PFNGLGETSHADERIVPROC glGet__iv, PFNGLGETSHADERINFOLOGPROC glGet__InfoLog)
{
  GLint log_length;
  char *log;
  
  glGet__iv(object, GL_INFO_LOG_LENGTH, &log_length);
  log = (char*)malloc(log_length);
  glGet__InfoLog(object, log_length, NULL, log);
  fprintf(stderr, "%s", log);
  free(log);
}

struct Data
{
  GLuint vertexBuffer;
  GLuint colorBuffer;
  GLuint elementBuffer;
  
  GLuint vao;
  
  GLuint vshader, fshader;
  GLuint program;
  
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

static long fileLength(FILE *file)
{
  long c = ftell(file);
  fseek(file, 0, SEEK_END);
  long s = ftell(file);
  fseek(file, c, SEEK_SET);
  return s;
}

static GLchar *fileContents(const char *name, GLint *len)
{
  FILE *in = fopen(name, "rb");
  long length = fileLength(in);
  GLchar *data = new GLchar[length];
  fread(data, sizeof(GLchar), length, in);
  *len = static_cast<int>(length);
  fclose(in);
  return data;
}

static GLuint compileShader(GLenum kind, const char* filename)
{
  GLint length;
  GLchar *data = fileContents(filename, &length);
  GLuint shader;
  GLint shaderOk;
  
  if (!data)
    return 0;

  shader = glCreateShader(kind);
  glShaderSource(shader, 1, (const GLchar**)&data, &length);
  delete [] data;
  glCompileShader(shader);
  
  glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderOk);
  if (!shaderOk)
  {
    fprintf(stderr, "Failed to compile %s:\n", filename);
    show_info_log(shader, glGetShaderiv, glGetShaderInfoLog);
    glDeleteShader(shader);
    return 0;
  }
  
  return shader;
}

static GLuint linkProgram(GLuint vShader, GLuint fShader)
{
  GLint result;
  
  GLuint program = glCreateProgram();
  glAttachShader(program, vShader);
  glAttachShader(program, fShader);
  //glBindFragDataLocation(data.program, 0, "fragColor");
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &result);
  if (!result)
  {
    fprintf(stderr, "Failed to link shader program:\n");
    show_info_log(program, glGetProgramiv, glGetProgramInfoLog);
    glDeleteProgram(program);
    return 0;
  }
  
  return program;
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
  data.vshader = compileShader(GL_VERTEX_SHADER, "shader.v.glsl");
  data.fshader = compileShader(GL_FRAGMENT_SHADER, "shader.f.glsl");

  
  
  data.program = linkProgram(data.vshader, data.fshader);
  
  data.position = glGetAttribLocation(data.program, "a_position");
  data.color = glGetAttribLocation(data.program, "a_color");
  data.ptimer = glGetUniformLocation(data.program, "u_timer");
  
  data.pmatrixLoc = glGetUniformLocation(data.program, "pMatrix");
  data.vmatrixLoc = glGetUniformLocation(data.program, "vMatrix");
  data.mmatrixLoc = glGetUniformLocation(data.program, "mMatrix");
  
  
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
  
  
  glm::vec3 v1 = glm::vec3(-0.5f, -0.5f, -0.0f);
  glm::vec3 v2 = glm::vec3(0.5f, -0.5f, -0.0f);
  glm::vec3 v3 = glm::vec3(-0.5f, 0.5f, -0.0f);
  
  glm::vec3 t1 = v2-v1;
  glm::vec3 t2 = v3-v1;
  glm::vec3 normal = glm::normalize(glm::cross(t2, t1));
  glm::vec4 pts[] = {glm::vec4(glm::vec3(0.0f), 1.0f), glm::vec4(normal, 1.0f)};
  
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
    
    glUseProgram(data.program);
    glUniform1f(data.ptimer, data.timer);
    
    glUniformMatrix4fv(data.pmatrixLoc, 1, GL_FALSE, &projectionMatrix[0][0]);
    glUniformMatrix4fv(data.vmatrixLoc, 1, GL_FALSE, &viewMatrix[0][0]);
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

