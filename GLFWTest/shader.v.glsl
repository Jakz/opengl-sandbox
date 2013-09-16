#version 150

uniform float u_timer;

uniform mat4 pMatrix;
uniform mat4 vMatrix;
uniform mat4 mMatrix;

uniform mat4 normalMatrix;

in vec3 a_normal;

in vec4 a_position;
in vec4 a_color;
in vec2 a_texCoord;

in float a_shine;
in vec4 a_specular;

out vec4 v_color;
out vec2 v_texCoord;

out vec3 v_position;
out vec3 v_normal;

void main()
{
  mat4 mvMatrix = vMatrix * mMatrix;
  
  v_normal = normalize(transpose(inverse(mat3(mMatrix))) * a_normal);
  
  v_color = a_color;
  v_texCoord = a_texCoord;

  gl_Position = pMatrix * mvMatrix * vec4(a_position.xyz, 1.0);
  v_position = gl_Position.xyz;
}