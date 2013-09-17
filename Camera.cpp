//
//  Camera.cpp
//  GLFWTest
//
//  Created by Jack on 9/13/13.
//  Copyright (c) 2013 Jack. All rights reserved.
//

#include "Camera.h"

#include <iostream>

void Camera::setProjection(float fov, float ratio, float znear, float zfar)
{
  this->fov = fov;
  this->ratio = ratio;
  this->znear = znear;
  this->zfar = zfar;
  
  projection = glm::perspective(fov, ratio, znear, zfar);
}

void Camera::adjustRatio(float ratio)
{
  this->ratio = ratio;
  
  projection = glm::perspective(this->fov, this->ratio, this->znear, this->zfar);
}

void Camera::translate(glm::vec3 delta)
{
  position -= delta;
  camera = glm::translate(camera, -delta);
}

void Camera::rotate(float horizontal, float vertical)
{
  angleHor += horizontal;
  angleVer += vertical;
  
  if (angleHor > 360.0f) angleHor -= 360.0f;
  if (angleHor < 0.0f) angleHor += 360.0f;
  
  if (angleVer > 360.0f) angleVer -= 360.0f;
  if (angleVer < 0.0f) angleVer += 360.0f;
  
  std::cout << angleHor << ", " << angleVer << std::endl;
  
  //camera = glm::translate(glm::mat4(), position);
  camera = glm::rotate(camera, horizontal, glm::vec3(0,1,0));
  camera = glm::rotate(camera, vertical, glm::vec3(1,0,0));
  
  
  
  
  //camera = glm::rotate(camera, horizontal, glm::vec3(0,1,0));
  //camera = glm::rotate(camera, vertical, glm::vec3(1,0,0));
}

void Camera::lookAt(glm::vec3 position, glm::vec3 target, glm::vec3 up)
{
  this->position = position;
  camera = glm::lookAt(position, target, up);
}