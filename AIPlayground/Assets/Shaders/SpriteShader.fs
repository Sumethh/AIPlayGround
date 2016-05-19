#version 330 core
in vec4 in_color;
in vec2 out_textCoord;
out vec4 color;

uniform sampler2D ourTexture;
void main()
{
  //color = in_color;
  color = texture(ourTexture, out_textCoord);
  //color = vec4(out_textCoord.x ,out_textCoord.y,0.0f,1.0f);
}
