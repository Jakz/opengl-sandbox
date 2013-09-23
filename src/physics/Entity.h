//
//  Camera.h
//  GLFWTest
//
//  Created by Jack on 9/13/13.
//  Copyright (c) 2013 Jack. All rights reserved.
//

#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "Includes.h"


struct PhysicsData
{
  glm::vec3 position;
  glm::vec3 velocity;
  glm::vec3 acceleration;

  float mass;
  float damping;
 
  glm::vec3 forceAccumulator;
  
  PhysicsData() : position(glm::vec3()), velocity(glm::vec3()), acceleration(glm::vec3()), mass(1.0f)
  {

  }
  
  void update(float dt)
  {
    position += velocity*dt;
    
    glm::vec3 newAcceleration = acceleration;
    newAcceleration += forceAccumulator*dt;
    
    velocity += newAcceleration*dt;
    
    velocity *= powf(damping, dt);
    
    clearAccumulator();
  }
  
  void clearAccumulator()
  {
    forceAccumulator = glm::vec3();
  }
  
  void addForce(const glm::vec3 &force)
  {
    forceAccumulator += force;
  }
};

class Entity
{
  private:
    PhysicsData data;
  
  public:
    Entity() : data(PhysicsData()) { }
  
  const PhysicsData* const physics() { return &data; }
};

class ForceGenerator
{
  virtual void updateForce(Entity *entity, float dt) = 0;
};

#endif
