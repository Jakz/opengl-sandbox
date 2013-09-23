//
//  Image.cpp
//  GLFWTest
//
//  Created by Jack on 9/15/13.
//  Copyright (c) 2013 Jack. All rights reserved.
//

#include "Texture.h"
#include <cstring>

u32 TextureCache::maxTextures;
GLint *TextureCache::cache;

void TextureCache::init()
{
  GLint max;
  glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max);
  
  maxTextures = max;
  
  
  cache = new GLint[max];
  
  for (int i = 0; i < maxTextures; ++i)
    cache[i] = -1;
}

void TextureCache::bind(Texture *texture, GLenum unit)
{
  u32 cacheIndex = unit - GL_TEXTURE0;
  
  if (cache[cacheIndex] != texture->ident)
  {
    glActiveTexture(cacheIndex);
    glBindTexture(GL_TEXTURE_2D, texture->ident);
    cache[cacheIndex] = texture->ident;
  }
}