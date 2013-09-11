#version 150

uniform mat4 pMatrix;
uniform mat4 vMatrix;
uniform mat4 mMatrix;

uniform float u_timer;

in vec4 v_color;
in vec3 v_position;

out vec4 fragColor;


in vec3 v_normal;
in float v_shine;
in vec4 v_specular;

const vec3 light_direction = vec3(0.408248, -0.816497, 0.408248);
const vec4 light_diffuse = vec4(0.8, 0.8, 0.8, 0.0);
const vec4 light_ambient = vec4(0.2, 0.2, 0.2, 1.0);
const vec4 light_specular = vec4(1.0, 1.0, 1.0, 1.0);

float rand(vec2 co){
  return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() {
  mat4 mvMatrix = vMatrix*mMatrix;
  
  vec3 lightDirection = (mvMatrix * vec4(light_direction, 0.0)).xyz;
  vec3 normal = normalize(v_normal);
  vec3 eye = normalize(v_position);
  vec3 reflection = reflect(lightDirection, normal);
  
  vec4 fragDiffuse = v_color;
  vec4 diffuseFactor = max(-dot(normal, lightDirection), 0.0) * light_diffuse;
  vec4 ambientDiffuse = diffuseFactor + light_ambient;
  vec4 specularFactor = max(pow(-dot(reflection, eye), v_shine), 0.0) * light_specular;
  
  
  //fragColor = specularFactor * v_specular + ambientDiffuse * fragDiffuse;
  fragColor = vec4(v_color.rgb, 1.0);
}