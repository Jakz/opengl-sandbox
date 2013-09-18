#version 150

uniform mat4 pMatrix;
uniform mat4 vMatrix;
uniform mat4 mMatrix;

in vec2 a_texCoord;
in vec4 a_position;

out vec2 v_texCoord;

out vec3 v_position;

void main()
{
  v_texCoord = a_texCoord;
  gl_Position = pMatrix * vMatrix * mMatrix * a_position;
  v_position = vec3(mMatrix * a_position);
}