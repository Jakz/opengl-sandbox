//
//  Image.h
//  GLFWTest
//
//  Created by Jack on 9/15/13.
//  Copyright (c) 2013 Jack. All rights reserved.
//

#ifndef _TEXTURE_LIBRARY_H_
#define _TEXTURE_LIBRARY_H_

#include "Texture.h"
#include "Image.h"

enum TextureID
{
  TEXTURE_TILES = 0,
  TEXTURE_BULLETS,
  
  TEXTURES_COUNT
};

class TextureLibrary
{
  private:
    static Texture* textures[TEXTURES_COUNT];
  
  
    static GLuint prepareTexture(Image* image, GLint minMagFilter = GL_NEAREST, GLint wrap = GL_CLAMP_TO_EDGE)
    {
      GLenum format;
      switch(image->format())
      {
        case Image::FORMAT_RGB: format = GL_RGB; break;
        case Image::FORMAT_RGBA: format = GL_RGBA; break;
      }
      
      GLuint ident;
      glGenTextures(1, &ident);
      glBindTexture(GL_TEXTURE_2D, ident);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minMagFilter);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, minMagFilter);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
      glTexImage2D(GL_TEXTURE_2D, 0, format, image->width(), image->height(), 0, format, GL_UNSIGNED_BYTE, image->pixels());
      glBindTexture(GL_TEXTURE_2D, 0);
      
      return ident;
    }
  
  public:
    static void set(TextureID ident, Texture *texture) { textures[ident] = texture; }
    static Texture *get(TextureID ident) { return textures[ident]; }
  
    static Texture *generate(TextureID ident, Image* image, GLint minMagFilter = GL_NEAREST, GLint wrap = GL_CLAMP_TO_EDGE)
    {
      Texture *texture = new Texture(prepareTexture(image, minMagFilter, wrap), image->width(), image->height());
      set(ident, texture);
      return texture;
    }
  
    static TextureTiled *generateTiled(TextureID ident, Image* image, u16 tileWidth, u16 tileHeight, GLint minMagFilter = GL_NEAREST, GLint wrap = GL_CLAMP_TO_EDGE)
    {
      TextureTiled *texture = new TextureTiled(prepareTexture(image, minMagFilter, wrap), image->width(), image->height(), tileWidth, tileHeight);
      set(ident, texture);
      return texture;
    }
};

#endif
