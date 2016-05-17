in vec4 in_color;
in vec2 out_textCoord;
out vec4 color;

uniform sampler2D ourTexture;
void main()
{
  color = in_color;
  //color = texture(ourTexture, out_textCoord);
}
