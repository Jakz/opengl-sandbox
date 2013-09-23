#version 150

uniform sampler2D tex;

in vec2 v_texCoord;

out vec4 fragColor;

void main() {
  fragColor = texture(tex, v_texCoord);
}