#version 150

uniform mat4 pMatrix;
uniform mat4 vMatrix;
uniform mat4 mMatrix;

in vec4 a_position;
in vec2 a_texCoord;

out vec2 v_texCoord;

void main()
{
  v_texCoord = a_texCoord;

  gl_Position = pMatrix * vMatrix * mMatrix * vec4(a_position);
}