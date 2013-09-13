//
//  ShaderCache.cpp
//  GLFWTest
//
//  Created by Jack on 9/12/13.
//  Copyright (c) 2013 Jack. All rights reserved.
//

#include "ShaderCache.h"

#include "FileUtils.h"

using namespace std;

vector<Shader*> ShaderCache::shaders;
vector<Program*> ShaderCache::programs;

unordered_map<string, Shader*> ShaderCache::mshaders;
unordered_map<string, Program*> ShaderCache::mprograms;

void ShaderCache::showErrorLog(GLuint object, PFNGLGETSHADERIVPROC glGet__iv, PFNGLGETSHADERINFOLOGPROC glGet__InfoLog)
{
  GLint log_length;
  char *log;
  
  glGet__iv(object, GL_INFO_LOG_LENGTH, &log_length);
  log = (char*)malloc(log_length);
  glGet__InfoLog(object, log_length, NULL, log);
  fprintf(stderr, "%s", log);
  free(log);
}

Shader *ShaderCache::shaderForName(std::string name)
{
  unordered_map<string, Shader*>::iterator it = mshaders.find(name);
  
  return it != mshaders.end() ? it->second : nullptr;
}

Program *ShaderCache::programForName(std::string name)
{
  unordered_map<string, Program*>::iterator it = mprograms.find(name);
  
  return it != mprograms.end() ? it->second : nullptr;
}



Shader* ShaderCache::compileShader(GLenum type, const char *filename)
{
  GLint length;
  GLchar *data = FileUtils::fileContents(filename, &length);
  GLuint ident;
  
  if (!data)
    return nullptr;
  
  ident = glCreateShader(type);
  glShaderSource(ident, 1, (const GLchar**)&data, &length);
  delete [] data;
  glCompileShader(ident);
  
  
  GLint shaderOk;
  glGetShaderiv(ident, GL_COMPILE_STATUS, &shaderOk);
  if (!shaderOk)
  {
    fprintf(stderr, "Failed to compile shader %s:\n", filename);
    showErrorLog(ident, glGetShaderiv, glGetShaderInfoLog);
    glDeleteShader(ident);
    return nullptr;
  }
  
  Shader* shader = new Shader(type, ident);
  //TODO: add to collection
  return shader;
}

Program *ShaderCache::linkProgram(Shader *vertex, Shader *fragment)
{
  GLuint ident = glCreateProgram();
  glAttachShader(ident, vertex->ident);
  glAttachShader(ident, fragment->ident);
  //glBindFragDataLocation(data.program, 0, "fragColor");
  glLinkProgram(ident);

  GLint result;
  glGetProgramiv(ident, GL_LINK_STATUS, &result);
  if (!result)
  {
    fprintf(stderr, "Failed to link shader program:\n");
    showErrorLog(ident, glGetProgramiv, glGetProgramInfoLog);
    glDeleteProgram(ident);

    return nullptr;
  }
  
  // TODO: always detach?
  
  Program *program = new Program(ident);
  // TODO add to collection
  return program;
}

Program *ShaderCache::linkProgramOnce(Shader *vertex, Shader *fragment)
{
  Program *program = linkProgram(vertex, fragment);
  
  if (program)
  {
    // TODO: maybe already and always in link?
    glDetachShader(program->ident, vertex->ident);
    glDetachShader(program->ident, fragment->ident);
    
    glDeleteShader(vertex->ident);
    glDeleteShader(fragment->ident);
  }
  
  delete vertex;
  delete fragment;
  
  return program;
}