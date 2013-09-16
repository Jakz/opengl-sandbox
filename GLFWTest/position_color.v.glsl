#version 150

uniform mat4 pMatrix;
uniform mat4 vMatrix;
uniform mat4 mMatrix;

in vec4 a_position;
in vec4 a_color;

out vec4 v_color;

void main()
{
  v_color = a_color;

  gl_Position = pMatrix * vMatrix * mMatrix * vec4(a_position);
}