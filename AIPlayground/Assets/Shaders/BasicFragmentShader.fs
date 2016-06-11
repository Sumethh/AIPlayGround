#version 330 core

in vec2 texCoord;

uniform sampler2D myTexture;
out vec4 Color;
void main()
{
  Color = texture(myTexture, texCoord);
}
