#version 150

uniform float u_timer;

uniform mat4 pMatrix;
uniform mat4 vMatrix;
uniform mat4 mMatrix;

in vec4 a_position;
in vec4 a_color;

in float a_shine;
in vec4 a_specular;
in vec3 a_normal;

out vec4 v_color;
out vec3 v_position;

out vec3 v_normal;
out float v_shine;
out vec4 v_specular;

void main()
{
  mat4 mvMatrix = vMatrix * mMatrix;
  
  v_color = a_color;

  vec4 eye = mvMatrix * vec4(a_position.xyz, 1.0);
  
  v_shine = a_shine;
  v_specular = a_specular;
  v_normal = (mvMatrix * vec4(a_normal, 0.0)).xyz;

  gl_Position = pMatrix * mvMatrix * vec4(a_position.xyz, 1.0);
  v_position = gl_Position.xyz;
}