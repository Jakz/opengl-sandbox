//
//  Model.h
//  GLFWTest
//
//  Created by Jack on 9/16/13.
//  Copyright (c) 2013 Jack. All rights reserved.
//

#ifndef _MODEL_H_
#define _MODEL_H_

#include "Includes.h"

#include "Camera.h"
#include "ShaderCache.h"

#include <vector>

class Instance;

class Renderer
{
  private:
    Camera *cam;
  
    std::vector<Instance*> instances;
  
    void render(Instance *instance);
  
  public:
    Renderer() : cam(new Camera()) { }
  
    Camera *camera() const { return cam; }
  
    void addInstance(Instance* instance) { instances.push_back(instance); }
    Instance *instanceAt(u32 index) { return instances[0]; }
  
    void render();
};

class Instance
{
  protected:
    Program *program;

  public:
    Instance(Program *program) : program(program) { }
  
    virtual void mapBuffers() = 0;
    virtual void render() = 0;
  
    virtual void setModelMatrix(glm::mat4 &m) = 0;
  
  friend class Renderer;
};

class InstanceLines : public Instance
{
  private:
    std::vector<glm::vec4> vertices;
    std::vector<glm::vec4> colors;
    glm::mat4 modelMatrix;
  
    const GLenum type;
  
    GLuint vao;
    GLuint vbo[2];
  
  public:
    InstanceLines(GLenum type, Program *program);
  
    void addVertices(glm::vec4 v1, glm::vec4 v2, glm::vec4 c1, glm::vec4 c2);
    void addVertex(glm::vec4 v, glm::vec4 c);
  
    void mapBuffers() override;
  
    void render() override;
  
    void translate(const GLfloat x, const GLfloat y, const GLfloat z) { glm::translate(modelMatrix, glm::vec3(x,y,z)); }
  
    void setModelMatrix(glm::mat4 &m) override { modelMatrix = m; }
  
  friend class Renderer;
  
};

#endif
