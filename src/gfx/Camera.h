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
    glm::vec3 position;
  
  public:
    Camera() : projection(glm::mat4(1.0f)), angleHor(0.0f), angleVer(0.0f) { }
    void setProjection(float fov, float ratio, float znear, float zfar);
    void adjustRatio(float ratio);
  
    const glm::mat4 projectionMatrix() { return projection; }
    const glm::mat4 cameraMatrix() { return camera; }
  
    void lookAt(glm::vec3 position, glm::vec3 target, glm::vec3 up);
  
    void translate(glm::vec3 delta);
    void rotate(float horizontal, float vertical);
  
  
    glm::vec3 directionUp() { return glm::vec3(glm::inverse(camera)*glm::vec4(0,1,0,0)); }
    glm::vec3 directionRight() { return glm::vec3(glm::inverse(camera)*glm::vec4(1,0,0,0)); }
    glm::vec3 directionForward() { return glm::vec3(glm::inverse(camera)*glm::vec4(0,0,-1,0)); }
  
};

#endif
