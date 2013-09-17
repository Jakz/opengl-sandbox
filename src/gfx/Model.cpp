//
//  Model.cpp
//  GLFWTest
//
//  Created by Jack on 9/16/13.
//  Copyright (c) 2013 Jack. All rights reserved.
//

#include "Model.h"

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