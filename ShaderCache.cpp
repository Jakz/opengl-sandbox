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

GLint Program::attrib(const GLchar* attribName) const {
  GLint attrib = glGetAttribLocation(ident, attribName);
  return attrib;
}

GLint Program::uniform(const GLchar* uniformName) const {
  GLint uniform = glGetUniformLocation(ident, uniformName);
  return uniform;
}


#define _SHADER_ATTRIB_UNIFORM_SETTERS_IMPL(OGL_TYPE, TYPE_PREFIX, TYPE_SUFFIX) \
\
void Program::setAttrib(const GLuint ident, OGL_TYPE v0) \
{ glVertexAttrib ## TYPE_PREFIX ## 1 ## TYPE_SUFFIX (ident, v0); } \
void Program::setAttrib(const GLuint ident, OGL_TYPE v0, OGL_TYPE v1) \
{ glVertexAttrib ## TYPE_PREFIX ## 2 ## TYPE_SUFFIX (ident, v0, v1); } \
void Program::setAttrib(const GLuint ident, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2) \
{ glVertexAttrib ## TYPE_PREFIX ## 3 ## TYPE_SUFFIX (ident, v0, v1, v2); } \
void Program::setAttrib(const GLuint ident, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2, OGL_TYPE v3) \
{  glVertexAttrib ## TYPE_PREFIX ## 4 ## TYPE_SUFFIX (ident, v0, v1, v2, v3); } \
\
void Program::setAttrib1v(const GLuint ident, const OGL_TYPE* v) \
{ glVertexAttrib ## TYPE_PREFIX ## 1 ## TYPE_SUFFIX ## v (ident, v); } \
void Program::setAttrib2v(const GLuint ident, const OGL_TYPE* v) \
{ glVertexAttrib ## TYPE_PREFIX ## 2 ## TYPE_SUFFIX ## v (ident, v); } \
void Program::setAttrib3v(const GLuint ident, const OGL_TYPE* v) \
{ glVertexAttrib ## TYPE_PREFIX ## 3 ## TYPE_SUFFIX ## v (ident, v); } \
void Program::setAttrib4v(const GLuint ident, const OGL_TYPE* v) \
{ glVertexAttrib ## TYPE_PREFIX ## 4 ## TYPE_SUFFIX ## v (ident, v); } \
\
void Program::setUniform(const GLuint ident, OGL_TYPE v0) \
{ glUniform1 ## TYPE_SUFFIX (ident, v0); } \
void Program::setUniform(const GLuint ident, OGL_TYPE v0, OGL_TYPE v1) \
{ glUniform2 ## TYPE_SUFFIX (ident, v0, v1); } \
void Program::setUniform(const GLuint ident, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2) \
{ glUniform3 ## TYPE_SUFFIX (ident, v0, v1, v2); } \
void Program::setUniform(const GLuint ident, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2, OGL_TYPE v3) \
{ glUniform4 ## TYPE_SUFFIX (ident, v0, v1, v2, v3); } \
\
void Program::setUniform1v(const GLuint ident, const OGL_TYPE* v, GLsizei count) \
{ glUniform1 ## TYPE_SUFFIX ## v (ident, count, v); } \
void Program::setUniform2v(const GLuint ident, const OGL_TYPE* v, GLsizei count) \
{ glUniform2 ## TYPE_SUFFIX ## v (ident, count, v); } \
void Program::setUniform3v(const GLuint ident, const OGL_TYPE* v, GLsizei count) \
{ glUniform3 ## TYPE_SUFFIX ## v (ident, count, v); } \
void Program::setUniform4v(const GLuint ident, const OGL_TYPE* v, GLsizei count) \
{ glUniform4 ## TYPE_SUFFIX ## v (ident, count, v); }

_SHADER_ATTRIB_UNIFORM_SETTERS_IMPL(GLfloat, , f);
_SHADER_ATTRIB_UNIFORM_SETTERS_IMPL(GLdouble, , d);
_SHADER_ATTRIB_UNIFORM_SETTERS_IMPL(GLint, I, i);
_SHADER_ATTRIB_UNIFORM_SETTERS_IMPL(GLuint, I, ui);

void Program::setUniformMatrix2(const GLuint ident, const GLfloat* v, GLsizei count, GLboolean transpose) {
  glUniformMatrix2fv(ident, count, transpose, v);
}

void Program::setUniformMatrix3(const GLuint ident, const GLfloat* v, GLsizei count, GLboolean transpose) {
  glUniformMatrix3fv(ident, count, transpose, v);
}

void Program::setUniformMatrix4(const GLuint ident, const GLfloat* v, GLsizei count, GLboolean transpose) {
  glUniformMatrix4fv(ident, count, transpose, v);
}

void Program::setUniform(const GLuint ident, const glm::mat2& m, GLboolean transpose) {
  glUniformMatrix2fv(ident, 1, transpose, &m[0][0]);
}

void Program::setUniform(const GLuint ident, const glm::mat3& m, GLboolean transpose) {
  glUniformMatrix3fv(ident, 1, transpose, &m[0][0]);
}

void Program::setUniform(const GLuint ident, const glm::mat4& m, GLboolean transpose) {
  glUniformMatrix4fv(ident, 1, transpose, &m[0][0]);
}

void Program::setUniform(const GLuint ident, const glm::vec3& v) {
  setUniform3v(ident, &v[0]);
}

void Program::setUniform(const GLuint ident, const glm::vec4& v) {
  setUniform4v(ident, &v[0]);
}



//vector<Shader*> ShaderCache::shaders;
//vector<Program*> ShaderCache::programs;

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

Program *ShaderCache::prelinkProgram(Shader *vertex, Shader *fragment)
{
  GLuint ident = glCreateProgram();
  glAttachShader(ident, vertex->ident);
  glAttachShader(ident, fragment->ident);
  
  return new Program(ident);
}

bool ShaderCache::linkProgram(Program *program)
{
  glLinkProgram(program->ident);
  
  GLint result;
  glGetProgramiv(program->ident, GL_LINK_STATUS, &result);
  if (!result)
  {
    fprintf(stderr, "Failed to link shader program:\n");
    showErrorLog(program->ident, glGetProgramiv, glGetProgramInfoLog);
    glDeleteProgram(program->ident);
    return false;
  }
  
  // TODO: always detach?
  
  // TODO add to collection
  return true;
}

void ShaderCache::compileAndLink(std::string name, const char *vertexFileName, const char *fragmentFileName)
{
  Shader *vertex = compileVertexShader(vertexFileName);
  Shader *fragment = compileFragmentShader(fragmentFileName);
  
  Program *program = linkProgram(vertex, fragment);
  
  mapProgram(name, program);
}




glm::vec4 Colors::RED = glm::vec4(1.0f,0.0f,0.0f,1.0f);
glm::vec4 Colors::GREEN = glm::vec4(0.0f,1.0f,0.0f,1.0f);
glm::vec4 Colors::BLUE = glm::vec4(0.0f,0.0f,1.0f,1.0f);
