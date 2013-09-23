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
#include "Texture.h"

#include <vector>

class AbstractObject;

class Renderer
{
  private:
    Camera *cam;
    static double timer;
  
    std::vector<AbstractObject*> instances;
  
    void render(AbstractObject *instance);
  
  public:
    Renderer() : cam(new Camera()) { }
  
    Camera *camera() const { return cam; }
  
    void addInstance(AbstractObject* instance) { instances.push_back(instance); }
    AbstractObject *instanceAt(u32 index) { return instances[0]; }
  
    void render();
};

class AbstractObject
{
  protected:
    Program *program;

  public:
    AbstractObject(Program *program) : program(program) { }
  
    virtual void mapBuffers() = 0;
    virtual void render() = 0;
  
    virtual void setModelMatrix(glm::mat4 &m) = 0;
  
  friend class Renderer;
};

class ObjectLines : public AbstractObject
{
  private:
    std::vector<glm::vec4> vertices;
    std::vector<glm::vec4> colors;
    glm::mat4 modelMatrix;
  
    const GLenum type;
  
    GLuint vao;
    GLuint vbo[2];
  
  public:
    ObjectLines(GLenum type, Program *program);
  
    void addVertices(glm::vec4 v1, glm::vec4 v2, glm::vec4 c1, glm::vec4 c2);
    void addVertex(glm::vec4 v, glm::vec4 c);
  
    void mapBuffers() override;
  
    void render() override;
  
    void translate(const GLfloat x, const GLfloat y, const GLfloat z) { glm::translate(modelMatrix, glm::vec3(x,y,z)); }
  
    void setModelMatrix(glm::mat4 &m) override { modelMatrix = m; }
  
  friend class Renderer;
  
};

enum SurfaceDirection
{
  SURFACE_CEIL,
  SURFACE_FLOOR,
  SURFACE_WEST,
  SURFACE_EAST,
  SURFACE_NORTH,
  SURFACE_SOUTH
};

class ObjectTiledSurface : public AbstractObject
{
  private:
    glm::mat4 modelMatrix;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texCoords;
    glm::vec3 normal;
  
    TextureTiled *texture;

    GLenum type;
  
    GLuint vao;
    GLuint vbo[3];
  
  public:
    ObjectTiledSurface(GLenum type, Program *program, glm::ivec3 p1, glm::ivec3 p2, s32 quadSize, TextureTiled *texture, glm::vec2 tileBase, SurfaceDirection direction);
  
    void mapBuffers() override;
  
    void render() override;
  
    void translate(const GLfloat x, const GLfloat y, const GLfloat z) { glm::translate(modelMatrix, glm::vec3(x,y,z)); }
  
    void setModelMatrix(glm::mat4 &m) override { modelMatrix = m; }
};


class ObjectParticleEmitter : public AbstractObject
{
  private:
    bool isFirst;
    u32 currentVB, currentTFB;
    GLuint vbo;
    GLuint vao;
    GLuint tfb[2];
    GLenum type;
  
  public:
    ObjectParticleEmitter(Program *program, GLenum type);
  
    void init(glm::vec3 p);
  
    void mapBuffers() override;
  
    void render() override;
};

#endif
