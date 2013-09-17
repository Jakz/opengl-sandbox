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
  
  vec3 lightColor = vec3(1.0f,1.0f,1.0f);
  vec3 lightPosition = vec3(mvMatrix * vec4(0,0,3,1));
  
  vec3 surfacePos = vec3(mMatrix * vec4(v_position, 1.0f));
  vec4 surfaceColor = texture(tex, v_texCoord/4.0 + vec2(0.0,0.25));
  vec3 surfaceToLight = normalize(lightPosition - surfacePos);
  float diffuseCoefficient = max(0.0, dot(v_normal, surfaceToLight));
  
  vec3 incidenceVector = -surfaceToLight;
  vec3 reflectionVector = reflect(incidenceVector, v_normal);
  vec3 surfaceToCamera = normalize(vec3(0,0,1) - surfacePos);
  float cosAngle = max(0.0, dot(surfaceToCamera, reflectionVector));
  float specularCoefficient = pow(cosAngle, 0.4);
  
  vec3 specularComponent = specularCoefficient*vec3(0.9,0.9,1.0);
  
  vec3 combinedLight = vec3(0.2,0.2,0.2) + specularComponent /*+ diffuseCoefficient*lightColor*/;

  fragColor = vec4((specularCoefficient + diffuseCoefficient * lightColor)*surfaceColor.rgb, 1);
  
  //fragColor = vec4(texture(tex, v_texCoord).rgb, v);
}