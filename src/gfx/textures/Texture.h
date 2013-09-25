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

class TextureTiled;


class Texture
{
  protected:  
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
  
    TextureTiled *asTiled() { return (TextureTiled*)this; }
  
  friend class TextureCache;
};

class TextureTiled : public Texture
{
  public:
    const u16 rows, cols;
    const u16 tileWidth, tileHeight;
  
    TextureTiled(GLuint ident, u16 width, u16 height, u16 tileWidth, u16 tileHeight) : Texture(ident, width, height), rows(width/tileWidth), cols(height/tileHeight), tileWidth(tileWidth), tileHeight(tileHeight) { }
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
