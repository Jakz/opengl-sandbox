//
//  Image.h
//  GLFWTest
//
//  Created by Jack on 9/15/13.
//  Copyright (c) 2013 Jack. All rights reserved.
//

#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "Includes.h"

class Image
{
  public:
    enum Format {
      FORMAT_RGB = 3,
      FORMAT_RGBA = 4
    };

  public:
    Image(const char *filename);
  
    u8 *pixels() { return data; }
    u32 width() { return w; }
    u32 height() { return h; }
    Format format() { return f; }
  
  
  private:
    u8 *data;
    s32 w;
    s32 h;
    Format f;
  
};

#endif
