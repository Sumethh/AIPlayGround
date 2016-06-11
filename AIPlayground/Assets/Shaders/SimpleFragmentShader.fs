#version 330 core
in vec2 out_textCoord;
out vec4 color;

uniform sampler2D ourTexture;
void main()
{
  color = texture(ourTexture, out_textCoord);
}
