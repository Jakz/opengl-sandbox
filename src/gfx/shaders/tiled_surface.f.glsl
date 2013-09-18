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

void main() {

  vec3 lightPosition = vec3(vec4(2.0,0,-2.5,1));
  vec3 surfaceColor = texture(tex, v_texCoord).rgb;

  vec3 position = v_position;
  vec3 surfaceToLight = normalize(lightPosition - position);
  float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
  
  float lightAttenuation = 1.0;
  float attenuation = 1.0 / (1.0 * 0.4 * pow(length(lightPosition - position),2));

  vec3 diffuse = diffuseCoefficient * surfaceColor.rgb * vec3(0.9,0.9,0.0);

  fragColor = vec4(surfaceColor*0.2 + attenuation*diffuse, 1);

  //fragColor = texture(tex, v_texCoord);
}