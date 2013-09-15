//
//  ShaderCache.h
//  GLFWTest
//
//  Created by Jack on 9/12/13.
//  Copyright (c) 2013 Jack. All rights reserved.
//

#ifndef _SHADER_CACHE_H_
#define _SHADER_CACHE_H_

#include "Includes.h"

#include <vector>
#include <unordered_map>
#include <string>

struct Shader
{
  const GLenum type;
  const GLuint ident;
  
  Shader(const GLenum type, const GLuint ident) : type(type), ident(ident) { }
};

class Program
{
  private:
  
  public:
    const GLuint ident;
  
    Program() : ident(0) { }
    Program(const GLuint ident) : ident(ident) { }
  
    void use() { glUseProgram(ident); }
    void unuse() { glUseProgram(0); }
  
    GLint attrib(const GLchar* attribName) const;
    GLint uniform(const GLchar* uniformName) const;
  
    #define _SHADER_ATTRIB_UNIFORM_SETTERS(OGL_TYPE) \
    void setAttrib(const GLchar* attribName, OGL_TYPE v0); \
    void setAttrib(const GLchar* attribName, OGL_TYPE v0, OGL_TYPE v1); \
    void setAttrib(const GLchar* attribName, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2); \
    void setAttrib(const GLchar* attribName, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2, OGL_TYPE v3); \
    \
    void setAttrib1v(const GLchar* attribName, const OGL_TYPE* v); \
    void setAttrib2v(const GLchar* attribName, const OGL_TYPE* v); \
    void setAttrib3v(const GLchar* attribName, const OGL_TYPE* v); \
    void setAttrib4v(const GLchar* attribName, const OGL_TYPE* v); \
    \
    void setUniform(const GLchar* uniformName, OGL_TYPE v0); \
    void setUniform(const GLchar* uniformName, OGL_TYPE v0, OGL_TYPE v1); \
    void setUniform(const GLchar* uniformName, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2); \
    void setUniform(const GLchar* uniformName, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2, OGL_TYPE v3); \
    \
    void setUniform1v(const GLchar* uniformName, const OGL_TYPE* v, GLsizei count=1); \
    void setUniform2v(const GLchar* uniformName, const OGL_TYPE* v, GLsizei count=1); \
    void setUniform3v(const GLchar* uniformName, const OGL_TYPE* v, GLsizei count=1); \
    void setUniform4v(const GLchar* uniformName, const OGL_TYPE* v, GLsizei count=1); \

    _SHADER_ATTRIB_UNIFORM_SETTERS(GLfloat)
    _SHADER_ATTRIB_UNIFORM_SETTERS(GLdouble)
    _SHADER_ATTRIB_UNIFORM_SETTERS(GLint)
    _SHADER_ATTRIB_UNIFORM_SETTERS(GLuint)
  
    void setUniformMatrix2(const GLchar* uniformName, const GLfloat* v, GLsizei count=1, GLboolean transpose=GL_FALSE);
    void setUniformMatrix3(const GLchar* uniformName, const GLfloat* v, GLsizei count=1, GLboolean transpose=GL_FALSE);
    void setUniformMatrix4(const GLchar* uniformName, const GLfloat* v, GLsizei count=1, GLboolean transpose=GL_FALSE);
    void setUniform(const GLchar* uniformName, const glm::mat2& m, GLboolean transpose=GL_FALSE);
    void setUniform(const GLchar* uniformName, const glm::mat3& m, GLboolean transpose=GL_FALSE);
    void setUniform(const GLchar* uniformName, const glm::mat4& m, GLboolean transpose=GL_FALSE);
    void setUniform(const GLchar* uniformName, const glm::vec3& v);
    void setUniform(const GLchar* uniformName, const glm::vec4& v);
};


class ShaderCache
{
  private:
    static std::vector<Shader*> shaders;
    static std::unordered_map<std::string, Shader*> mshaders;
  
    static std::vector<Program*> programs;
    static std::unordered_map<std::string, Program*> mprograms;
  
    static void showErrorLog(GLuint object, PFNGLGETSHADERIVPROC glGet__iv, PFNGLGETSHADERINFOLOGPROC glGet__InfoLog);
  
  public:
    static Shader *shaderForName(std::string name);
    static Program *programForName(std::string name);
  
    static Shader *compileShader(GLenum type, const char *name);
    static Shader *compileVertexShader(const char *name) { return compileShader(GL_VERTEX_SHADER, name); }
    static Shader *compileFragmentShader(const char * name) { return compileShader(GL_FRAGMENT_SHADER, name); }
  
    static Program *linkProgram(Shader *vertex, Shader *fragment);
    static Program *linkProgramOnce(Shader *vertex, Shader *fragment);
};


#endif
