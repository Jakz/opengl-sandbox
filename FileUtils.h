//
//  FileUtils.h
//  GLFWTest
//
//  Created by Jack on 9/13/13.
//  Copyright (c) 2013 Jack. All rights reserved.
//

#ifndef _FILE_UTILS_H_
#define _FILE_UTILS_H_

#include "Includes.h"
#include <cstdio>

class FileUtils
{

  
  public:
    static u64 fileLength(const char *name);
    static u64 fileLength(FILE *file);
    static GLchar* fileContents(const char *name, GLint *length);
};


#endif