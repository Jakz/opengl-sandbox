//
//  Image.cpp
//  GLFWTest
//
//  Created by Jack on 9/15/13.
//  Copyright (c) 2013 Jack. All rights reserved.
//

#include "Image.h"
#include "stb_image.h"
#include <cstring>


Image::Image(const char *name)
{
  s32 ff;
  u8 *pixels = stbi_load(name, &w, &h, &ff, 0);
  f = static_cast<Format>(ff);
  
  if (pixels)
  {
    data = new u8[w*h*f];
    memcpy(data, pixels, w*h*f);
    stbi_image_free(pixels);
  }
  
}
