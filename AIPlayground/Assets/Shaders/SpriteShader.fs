#version 330 core

in vec2 gTextureCoord;
//in vec4 testColor;
out vec4 color;

uniform sampler2D ourTexture;
void main()
{
  //color = testColor;
  color = texture(ourTexture, gTextureCoord);
//  color = vec4(out_textCoord.x ,out_textCoord.y,0.0f,1.0f);
}
