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

enum LocationType : u32
{
  LOCATION_ATTRIBUTE,
  LOCATION_UNIFORM
};

enum LocationAttrib : s32
{
  ATTRIB_NONE = -1,
  ATTRIB_POSITION = 0,
  ATTRIB_COLOR = 1,
  ATTRIB_NORMAL = 2,
  ATTRIB_TEX_COORDS = 3,
  

  ATTRIBS_COUNT
};
  
enum LocationUniform : s32
{
  UNIFORM_NONE = -1,
  UNIFORM_MATRIX_MODEL = 0,
  UNIFORM_MATRIX_VIEW,
  UNIFORM_MATRIX_PROJECTION,
  UNIFORM_NORMAL,
  UNIFORM_TEXTURE,
  UNIFORM_TIMER,
  
  UNIFORMS_COUNT
};

struct ShaderBinding
{
  const GLchar* name;
  const LocationType type;
  union
  {
    LocationAttrib attrib;
    LocationUniform uniform;
  };
  
  ShaderBinding(GLchar *name, LocationType type, LocationAttrib attrib) : name(name), type(type), attrib(attrib) { }
  ShaderBinding(GLchar *name, LocationType type, LocationUniform uniform) : name(name), type(type), uniform(uniform) { }
};
  
struct Shader
{
  const GLenum type;
  const GLuint ident;
  
  Shader(const GLenum type, const GLuint ident) : type(type), ident(ident) { }
};

class Program
{
  private:
    GLint attribs[ATTRIBS_COUNT];
    GLint uniforms[UNIFORMS_COUNT];
  
  public:
    const GLuint ident;
  
    Program() : ident(0) { }
    Program(const GLuint ident) : attribs{-2}, uniforms{-2}, ident(ident) { }
  
    void use() { glUseProgram(ident); }
    void unuse() { glUseProgram(0); }
  
    void enableAttrib(const GLchar *name, const LocationAttrib location)
    {
      attribs[location] = glGetAttribLocation(ident, name);
      //glBindAttribLocation(ident, location, name);
    }
  
    void enableAttribs(const GLchar *n1, const LocationAttrib l1, const GLchar *n2 = NULL, const LocationAttrib l2 = ATTRIB_NONE, const GLchar *n3 = NULL, const LocationAttrib l3 = ATTRIB_NONE, const GLchar *n4 = NULL, const LocationAttrib l4 = ATTRIB_NONE);

  
    void enableUniform(const GLchar *name, const LocationUniform location)
    {
      uniforms[location] = glGetUniformLocation(ident, name);
    }
  
    void enableUniforms(const GLchar *n1, const LocationUniform l1, const GLchar *n2 = NULL, const LocationUniform l2 = UNIFORM_NONE, const GLchar *n3 = NULL, const LocationUniform l3 = UNIFORM_NONE, const GLchar *n4 = NULL, const LocationUniform l4 = UNIFORM_NONE);
  
    void enableLocation(ShaderBinding binding)
    {
      if (binding.type == LOCATION_ATTRIBUTE)
        enableAttrib(binding.name, binding.attrib);
      else if (binding.type == LOCATION_UNIFORM)
        enableUniform(binding.name, binding.uniform);
    }
  
    void enableUniformMatrices(const GLchar *projectionMatrix, const GLchar *viewMatrix, const GLchar *modelMatrix)
    {
      uniforms[UNIFORM_MATRIX_PROJECTION] = glGetUniformLocation(ident, projectionMatrix);
      uniforms[UNIFORM_MATRIX_VIEW] = glGetUniformLocation(ident, viewMatrix);
      uniforms[UNIFORM_MATRIX_MODEL] = glGetUniformLocation(ident, modelMatrix);
    }
  
    GLint attrib(LocationAttrib location) const { return attribs[location]; }
    GLint uniform(LocationUniform location) const { return uniforms[location]; }
  
    GLint attrib(const GLchar* attribName) const;
    GLint uniform(const GLchar* uniformName) const;
  
    #define _SHADER_ATTRIB_UNIFORM_SETTERS(OGL_TYPE) \
    void setAttrib(const GLuint ident, OGL_TYPE v0); \
    void setAttrib(const GLuint ident, OGL_TYPE v0, OGL_TYPE v1); \
    void setAttrib(const GLuint ident, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2); \
    void setAttrib(const GLuint ident, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2, OGL_TYPE v3); \
    \
    void setAttrib1v(const GLuint ident, const OGL_TYPE* v); \
    void setAttrib2v(const GLuint ident, const OGL_TYPE* v); \
    void setAttrib3v(const GLuint ident, const OGL_TYPE* v); \
    void setAttrib4v(const GLuint ident, const OGL_TYPE* v); \
    \
    void setUniform(const GLuint ident, OGL_TYPE v0); \
    void setUniform(const GLuint ident, OGL_TYPE v0, OGL_TYPE v1); \
    void setUniform(const GLuint ident, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2); \
    void setUniform(const GLuint ident, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2, OGL_TYPE v3); \
    \
    void setUniform1v(const GLuint ident, const OGL_TYPE* v, GLsizei count=1); \
    void setUniform2v(const GLuint ident, const OGL_TYPE* v, GLsizei count=1); \
    void setUniform3v(const GLuint ident, const OGL_TYPE* v, GLsizei count=1); \
    void setUniform4v(const GLuint ident, const OGL_TYPE* v, GLsizei count=1); \

    _SHADER_ATTRIB_UNIFORM_SETTERS(GLfloat)
    _SHADER_ATTRIB_UNIFORM_SETTERS(GLdouble)
    _SHADER_ATTRIB_UNIFORM_SETTERS(GLint)
    _SHADER_ATTRIB_UNIFORM_SETTERS(GLuint)
  
    void setUniformMatrix2(const GLuint ident, const GLfloat* v, GLsizei count=1, GLboolean transpose=GL_FALSE);
    void setUniformMatrix3(const GLuint ident, const GLfloat* v, GLsizei count=1, GLboolean transpose=GL_FALSE);
    void setUniformMatrix4(const GLuint ident, const GLfloat* v, GLsizei count=1, GLboolean transpose=GL_FALSE);
    void setUniform(const GLuint ident, const glm::mat2& m, GLboolean transpose=GL_FALSE);
    void setUniform(const GLuint ident, const glm::mat3& m, GLboolean transpose=GL_FALSE);
    void setUniform(const GLuint ident, const glm::mat4& m, GLboolean transpose=GL_FALSE);
    void setUniform(const GLuint ident, const glm::vec3& v);
    void setUniform(const GLuint ident, const glm::vec4& v);
  
    template <typename T>
    void setUniform(const LocationUniform location, const T& t) {
      setUniform(uniforms[location], t);
    }
};


class ShaderCache
{
  private:
    //static std::vector<Shader*> shaders;
    static std::unordered_map<std::string, Shader*> mshaders;
  
    //static std::vector<Program*> programs;
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
  
    static Program *prelinkProgram(Shader *vertex, Shader *fragment);
    static bool linkProgram(Program *program);
  
    static void mapProgram(std::string name, Program *program) { mprograms[name] = program; }
    static Program *program(std::string name) { std::unordered_map<std::string, Program*>::iterator it = mprograms.find(name); return it != mprograms.end() ? it->second : nullptr; }
  
    static void compileAndLink(std::string name, const char *vertexFileName, const char *fragmentFileName);
};
  
class Colors
{
  public:
    static glm::vec4 RED, GREEN, BLUE;
};


#endif
