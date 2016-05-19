#version 330 core

in vec2 vOutTextureCoords;
out vec4 color;

sampler2D spriteSheet;
void main()
{
  color = texture(spriteSheet, vOutTextureCoords);
}
