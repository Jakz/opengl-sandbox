//
//  Camera.h
//  GLFWTest
//
//  Created by Jack on 9/13/13.
//  Copyright (c) 2013 Jack. All rights reserved.
//

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Includes.h"

class Camera
{
  private:
    glm::mat4 projection;
    glm::mat4 camera;
    float angleHor, angleVer;
  
    float fov;
    float ratio;
    float znear, zfar;
  
  public:
    Camera() : projection(glm::mat4(1.0f)), angleHor(0.0f), angleVer(0.0f) { }
    void setProjection(float fov, float ratio, float znear, float zfar);
    void adjustRatio(float ratio);
  
    const glm::mat4 projectionMatrix() { return projection; }
    const glm::mat4 cameraMatrix() { return camera; }
  
    void lookAt(glm::vec3 position, glm::vec3 target, glm::vec3 up);
  
};

#endif
