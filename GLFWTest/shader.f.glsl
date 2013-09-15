#version 150

uniform sampler2D tex;

uniform mat4 pMatrix;
uniform mat4 vMatrix;
uniform mat4 mMatrix;

uniform float u_timer;

in vec4 v_color;
in vec3 v_position;
in vec2 v_texCoord;

out vec4 fragColor;

float rand(vec2 co){
  return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() {
  mat4 mvMatrix = vMatrix*mMatrix;
  //fragColor = vec4(v_color.rgb, 1.0);
  fragColor = texture(tex, v_texCoord);
}