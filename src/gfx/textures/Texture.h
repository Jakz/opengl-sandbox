//
//  Image.h
//  GLFWTest
//
//  Created by Jack on 9/15/13.
//  Copyright (c) 2013 Jack. All rights reserved.
//

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "Includes.h"
#include "Image.h"

struct TextureCoord
{
  glm::vec2 base;
  glm::vec2 offset;
};


class Texture
{
  protected:
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
  
    const GLuint ident;

  public:
    
    const u16 width, height;
  
    Texture(GLuint ident, u16 width, u16 height) : ident(ident), width(width), height(height) { }
  
    TextureCoord coords(u16 x, u16 y, u16 w, u16 h)
    {
      glm::vec2 base = glm::vec2( static_cast<float>(x) / width, static_cast<float>(y) / height);
      glm::vec2 offset = glm::vec2( static_cast<float>(w)/this->width, static_cast<float>(h)/this->height);
      
      return TextureCoord{base, offset};
    }
  
    static Texture *generate(Image* image, GLint minMagFilter = GL_NEAREST, GLint wrap = GL_CLAMP_TO_EDGE)
    {
      return new Texture(prepareTexture(image, minMagFilter, wrap), image->width(), image->height());
    }
  
  friend class TextureCache;
};

class TextureTiled : public Texture
{
  public:
    const u16 rows, cols;
    const u16 tileWidth, tileHeight;
  
    TextureTiled(GLuint ident, u16 width, u16 height, u16 tileWidth, u16 tileHeight) : Texture(ident, width, height), rows(width/tileWidth), cols(height/tileHeight), tileWidth(tileWidth), tileHeight(tileHeight) { }
  
    static TextureTiled *generate(Image* image, u16 tileWidth, u16 tileHeight, GLint minMagFilter = GL_NEAREST, GLint wrap = GL_CLAMP_TO_EDGE)
    {
      return new TextureTiled(prepareTexture(image, minMagFilter, wrap), image->width(), image->height(), tileWidth, tileHeight);
    }
};

class TextureCache
{
  private:
    static GLint *cache;
    static u32 maxTextures;
  
  public:
    static void init();
  
    static void bind(Texture *texture, GLenum unit);
  
  
};

struct Material
{
  glm::vec4 emission;
  glm::vec4 ambient;
  glm::vec4 diffuse;
  glm::vec4 specular;
  float shininess;
};


#endif
