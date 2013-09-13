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
