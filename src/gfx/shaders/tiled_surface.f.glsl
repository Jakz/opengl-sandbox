#version 150

uniform sampler2D tex;

uniform mat4 pMatrix;
uniform mat4 vMatrix;
uniform mat4 mMatrix;

uniform vec3 normal;
in vec3 v_position;

in vec2 v_texCoord;
in vec4 v_color;

out vec4 fragColor;

vec3 calcLight(vec3 lightPosition, vec3 lightColor)
{
  vec3 surfaceColor = texture(tex, v_texCoord).rgb;

  vec3 position = v_position;
  vec3 surfaceToLight = normalize(lightPosition - position);
  float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
  
  float lightAttenuation = 1.0;
  float attenuation = 1.0 / (1.0 * 0.4 * pow(length(lightPosition - position),2));

  return attenuation * diffuseCoefficient * surfaceColor.rgb * lightColor;
}

void main() {
  vec3 diffuse = calcLight(vec3(2,0,-2.5), vec3(0.9,0.9,0.0)) + calcLight(vec3(-2,-1,-2.5), vec3(0.0,0.0,0.8));

  fragColor = vec4(texture(tex, v_texCoord).rgb*0.2 + diffuse, 1);

  //fragColor = texture(tex, v_texCoord);
}