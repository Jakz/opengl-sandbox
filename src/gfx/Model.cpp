//
//  Model.cpp
//  GLFWTest
//
//  Created by Jack on 9/16/13.
//  Copyright (c) 2013 Jack. All rights reserved.
//

#include "Model.h"

#include <iostream>

using namespace std;
using namespace glm;

double Renderer::timer = 0.0;

void Renderer::render()
{
  timer = glfwGetTime();
  
  for (vector<AbstractObject*>::iterator it = instances.begin(); it != instances.end(); ++it)
    render(*it);
}

void Renderer::render(AbstractObject *instance)
{
  Program *program = instance->program;
  
  program->use();
  program->setUniform(UNIFORM_MATRIX_VIEW, cam->cameraMatrix());
  program->setUniform(UNIFORM_MATRIX_PROJECTION, cam->projectionMatrix());
  instance->render();
}



ObjectLines::ObjectLines(GLenum type, Program *program) : AbstractObject(program), type(type) , modelMatrix(mat4(1.0f))
{
  glGenVertexArrays(1, &vao);
  glGenBuffers(2, &vbo[0]);
}

void ObjectLines::addVertices(glm::vec4 v1, glm::vec4 v2, glm::vec4 c1, glm::vec4 c2)
{
  vertices.push_back(v1);
  vertices.push_back(v2);
  colors.push_back(c1);
  colors.push_back(c2);
}


void ObjectLines::addVertex(glm::vec4 v, glm::vec4 c)
{
  vertices.push_back(v);
  colors.push_back(c);
}


void ObjectLines::mapBuffers()
{
  glBindVertexArray(vao);
  
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(vec4), &vertices[0], GL_STATIC_DRAW);
  
  glEnableVertexAttribArray(program->attrib(ATTRIB_POSITION));
  glVertexAttribPointer(program->attrib(ATTRIB_POSITION), 4, GL_FLOAT, false, 0, (void*)0);
  
  glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
  glBufferData(GL_ARRAY_BUFFER, colors.size()*sizeof(vec4), &colors[0], GL_STATIC_DRAW);
  
  glEnableVertexAttribArray(program->attrib(ATTRIB_COLOR));
  glVertexAttribPointer(program->attrib(ATTRIB_COLOR), 4, GL_FLOAT, false, 0, (void*)0);
  
  glBindVertexArray(0);
}

void ObjectLines::render()
{
  //TODO: view and projection matrix should be altready set
  program->setUniform(UNIFORM_MATRIX_MODEL, modelMatrix);
  
  glBindVertexArray(vao);
  glDrawArrays(type, 0, static_cast<GLsizei>(vertices.size()));
  glBindVertexArray(0);
}




ObjectTiledSurface::ObjectTiledSurface(GLenum type, Program *program, ivec3 p1, ivec3 p2, s32 stepSize, TextureTiled *texture, vec2 tileBase, SurfaceDirection direction) : AbstractObject(program), type(type), texture(texture)
{
  // TODO: assertiamo che x1
  bool isFloor = p1.y == p2.y;
  bool isOrthogonal = p1.z == p2.z;
  
  ivec3 xDeltas, yDeltas;
  s32 xSteps, ySteps;
  
  if (direction == SURFACE_FLOOR || direction == SURFACE_CEIL)
  {
    xDeltas = ivec3(1,0,0),
    yDeltas = ivec3(0,0,1);
    xSteps = abs(p2.x - p1.x);
    ySteps = abs(p2.z - p1.z);
    
    if (direction == SURFACE_FLOOR)
      normal = vec3(0,1,0);
    else
      normal = vec3(0,-1,0);
  }
  else
  {
    if (direction == SURFACE_NORTH || direction == SURFACE_SOUTH)
    {
      xDeltas = ivec3(1,0,0),
      yDeltas = ivec3(0,-1,0);
      xSteps = abs(p2.x - p1.x);
      ySteps = abs(p2.y - p1.y);
      
      if (direction == SURFACE_NORTH)
        normal = vec3(0,0,1);
      else
        normal = vec3(0,0,-1);
    }
    else
    {
      xDeltas = ivec3(0,0,1);
      yDeltas = ivec3(0,-1,0),
      xSteps = abs(p2.z - p1.z);
      ySteps = abs(p2.y - p1.y);
      
      if (direction == SURFACE_EAST)
        normal = vec3(-1,0,0);
      else
        normal = vec3(1,0,0);
    }
  }
  
  xDeltas /= stepSize;
  yDeltas /= stepSize;
  xSteps /= stepSize;
  ySteps /= stepSize;
    
  for (s32 x = 0; x < xSteps; ++x)
  {
    for (s32 y = 0; y < ySteps; ++y)
    {
      ivec3 base = p1 + xDeltas*x + yDeltas*y;
      
      vertices.push_back(vec3(base));
      vertices.push_back(vec3(base+yDeltas));
      vertices.push_back(vec3(base+xDeltas));
      vertices.push_back(vec3(base+yDeltas));
      vertices.push_back(vec3(base+xDeltas+yDeltas));
      vertices.push_back(vec3(base+xDeltas));
      
      vec2 texBase = tileBase;
      
      if (rand()%2 == 0)
        texBase += vec2(0.25,0.0f);
        
        
      float tileSize = 0.25f;
      
      
      texCoords.push_back(texBase);
      texCoords.push_back(texBase + vec2(0, tileSize));
      texCoords.push_back(texBase + vec2(tileSize, 0));
      texCoords.push_back(texBase + vec2(0,tileSize));
      texCoords.push_back(texBase + vec2(tileSize,tileSize));
      texCoords.push_back(texBase + vec2(tileSize,0));
    }
  }
  
  glGenVertexArrays(1, &vao);
  glGenBuffers(2, &vbo[0]);
}

void ObjectTiledSurface::mapBuffers()
{
  //glGetError();
  
  glBindVertexArray(vao);
  
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(vec4), &vertices[0], GL_STATIC_DRAW);
  
  glEnableVertexAttribArray(program->attrib(ATTRIB_POSITION));
  glVertexAttribPointer(program->attrib(ATTRIB_POSITION), 3, GL_FLOAT, false, 0, (void*)0);
  
  glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
  glBufferData(GL_ARRAY_BUFFER, texCoords.size()*sizeof(vec2), &texCoords[0], GL_STATIC_DRAW);
  
  glEnableVertexAttribArray(program->attrib(ATTRIB_TEX_COORDS));
  glVertexAttribPointer(program->attrib(ATTRIB_TEX_COORDS), 2, GL_FLOAT, false, 0, (void*)0);
  
  glBindVertexArray(0);
  
  /*GLenum error = glGetError();
  if (error != GL_NO_ERROR)
  {
    std::cout << "ERROR: " << gluErrorString(error) << std::endl;
  }*/
}

void ObjectTiledSurface::render()
{
  program->setUniform(UNIFORM_MATRIX_MODEL, modelMatrix);
  program->setUniform(UNIFORM_NORMAL, normal);
  TextureCache::bind(texture, GL_TEXTURE0);
  program->setUniform(UNIFORM_TEXTURE, 0);
    
  glBindVertexArray(vao);
  glDrawArrays(type, 0, static_cast<GLsizei>(vertices.size()));
  glBindVertexArray(0);
}




ObjectParticleEmitter::ObjectParticleEmitter(Program *program, GLenum type) : AbstractObject(program), type(type)
{
  
}

void ObjectParticleEmitter::init(glm::vec3 p)
{
  
}

void ObjectParticleEmitter::mapBuffers()
{
  
}

void ObjectParticleEmitter::render()
{
  
}


Asset::Asset(GLenum type) : type(type)
{
  glGenVertexArrays(1, &vao);
  glGenBuffers(2, &vbo[0]);
}


void Asset::addOrthoQuad(vec3 pt1, vec3 pt2, TextureCoord coord)
{
  if (pt1.z == pt2.z)
  {
    vertices.push_back(pt1);
    vertices.push_back(pt1 + vec3(0,pt2.y,0));
    vertices.push_back(pt1 + vec3(pt2.x,0,0));
    vertices.push_back(pt2);
  }
  else if (pt1.x == pt2.x)
  {
    vertices.push_back(pt1);
    vertices.push_back(pt1 + vec3(0,pt2.y,0));
    vertices.push_back(pt1 + vec3(0,0,pt2.z));
    vertices.push_back(pt2);
  }
  else if (pt1.y == pt2.y)
  {
    vertices.push_back(pt1);
    vertices.push_back(pt1 + vec3(0,0,pt2.z));
    vertices.push_back(pt1 + vec3(pt2.x,0,0));
    vertices.push_back(pt2);
  }
  else
  {
    // TODO: should never happen
  }
  
  texCoords.push_back(coord.base);
  texCoords.push_back(coord.base + coord.offset.t);
  texCoords.push_back(coord.base + coord.offset.s);
  texCoords.push_back(coord.base + coord.offset);
}

void Asset::mapBuffers()
{
  glBindVertexArray(vao);
  
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(vec4), &vertices[0], GL_STATIC_DRAW);
  
  glEnableVertexAttribArray(program->attrib(ATTRIB_POSITION));
  glVertexAttribPointer(program->attrib(ATTRIB_POSITION), 3, GL_FLOAT, false, 0, (void*)0);
  
  glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
  glBufferData(GL_ARRAY_BUFFER, texCoords.size()*sizeof(vec2), &texCoords[0], GL_STATIC_DRAW);
  
  glEnableVertexAttribArray(program->attrib(ATTRIB_TEX_COORDS));
  glVertexAttribPointer(program->attrib(ATTRIB_TEX_COORDS), 2, GL_FLOAT, false, 0, (void*)0);
  
  glBindVertexArray(0);
}

void Asset::render()
{
  TextureCache::bind(texture, GL_TEXTURE0);
  program->setUniform(UNIFORM_TEXTURE, 0);
  
  glBindVertexArray(vao);
  glDrawArrays(type, 0, static_cast<GLsizei>(vertices.size()));
  glBindVertexArray(0);
}



void ObjectAsset::render()
{
  program->setUniform(UNIFORM_MATRIX_MODEL, modelMatrix);
  asset->render();
}