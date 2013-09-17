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

void Renderer::render()
{
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




ObjectTiledSurface::ObjectTiledSurface(GLenum type, Program *program, ivec3 p1, ivec3 p2, s32 stepSize, TextureTiled *texture, vec2 tileBase) : AbstractObject(program), type(type), texture(texture)
{
  // TODO: assertiamo che x1
  bool isFloor = p1.y == p2.y;
  bool isOrthogonal = p1.z == p2.z;
  
  ivec3 xDeltas, yDeltas;
  s32 xSteps, ySteps;
  
  if (isFloor)
  {
    xDeltas = ivec3(1,0,0),
    yDeltas = ivec3(0,0,1);
    xSteps = abs(p2.x - p1.x);
    ySteps = abs(p2.z - p1.z);
  }
  else
  {
    if (isOrthogonal)
    {
      xDeltas = ivec3(1,0,0),
      yDeltas = ivec3(0,-1,0);
      xSteps = abs(p2.x - p1.x);
      ySteps = abs(p2.y - p1.y);
    }
    else
    {
      xDeltas = ivec3(0,0,1);
      yDeltas = ivec3(0,-1,0),
      xSteps = abs(p2.z - p1.z);
      ySteps = abs(p2.y - p1.y);
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
      
      vertices.push_back(vec4(base, 1.0f));
      vertices.push_back(vec4(base+yDeltas, 1.0f));
      vertices.push_back(vec4(base+xDeltas, 1.0f));
      vertices.push_back(vec4(base+yDeltas, 1.0f));
      vertices.push_back(vec4(base+xDeltas+yDeltas, 1.0f));
      vertices.push_back(vec4(base+xDeltas, 1.0f));
      
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
  glGetError();
  
  glBindVertexArray(vao);
  
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(vec4), &vertices[0], GL_STATIC_DRAW);
  
  glEnableVertexAttribArray(program->attrib(ATTRIB_POSITION));
  glVertexAttribPointer(program->attrib(ATTRIB_POSITION), 4, GL_FLOAT, false, 0, (void*)0);
  
  glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
  glBufferData(GL_ARRAY_BUFFER, texCoords.size()*sizeof(vec2), &texCoords[0], GL_STATIC_DRAW);
  
  glEnableVertexAttribArray(program->attrib(ATTRIB_TEX_COORDS));
  glVertexAttribPointer(program->attrib(ATTRIB_TEX_COORDS), 2, GL_FLOAT, false, 0, (void*)0);
  
  glBindVertexArray(0);
  
  GLenum error = glGetError();
  if (error != GL_NO_ERROR)
  {
    std::cout << "ERROR: " << gluErrorString(error) << std::endl;
  }
}

void ObjectTiledSurface::render()
{
  program->setUniform(UNIFORM_MATRIX_MODEL, modelMatrix);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture->ident);
  program->setUniform(UNIFORM_TEXTURE, 0);
  
  glBindVertexArray(vao);
  glDrawArrays(type, 0, static_cast<GLsizei>(vertices.size()));
  glBindVertexArray(0);
}