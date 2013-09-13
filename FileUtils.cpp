//
//  FileUtils.cpp
//  GLFWTest
//
//  Created by Jack on 9/13/13.
//  Copyright (c) 2013 Jack. All rights reserved.
//

#include "FileUtils.h"


// TODO: check if file exists
u64 FileUtils::fileLength(FILE *file)
{
  u64 c = ftell(file);
  fseek(file, 0, SEEK_END);
  long s = ftell(file);
  fseek(file, c, SEEK_SET);
  return s;
}

u64 FileUtils::fileLength(const char *name)
{
  FILE *file = fopen(name, "rb");
  u64 length = fileLength(file);
  fclose(file);
  return length;
}

GLchar* FileUtils::fileContents(const char *name, GLint *length)
{
  FILE *in = fopen(name, "rb");
  u64 len = fileLength(in);
  GLchar *data = new GLchar[len];
  fread(data, sizeof(GLchar), len, in);
  *length = static_cast<GLint>(len);
  fclose(in);
  return data;
}