#version 150

uniform sampler2D tex;

uniform mat4 pMatrix;
uniform mat4 vMatrix;
uniform mat4 mMatrix;

uniform float u_timer;

in vec4 v_color;
in vec2 v_texCoord;

out vec4 fragColor;

in vec3 v_position;
in vec3 v_normal;

float rand(vec2 co){
  return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() {
  mat4 mvMatrix = vMatrix*mMatrix;
  
  vec3 lightColor = vec3(0.9, 0.9, 0.6);
  vec3 lightPosition = vec3(mvMatrix * vec4(0,0,3,1));
  
  vec3 surfacePos = vec3(mMatrix * vec4(v_position, 1.0f));
  vec4 surfaceColor = texture(tex, v_texCoord);
  vec3 surfaceToLight = normalize(lightPosition - surfacePos);
  float diffuseCoefficient = max(0.0, dot(v_normal, surfaceToLight));
  
  fragColor = vec4(diffuseCoefficient * surfaceColor.rgb * lightColor, 1);
  
  //fragColor = vec4(texture(tex, v_texCoord).rgb, v);
}