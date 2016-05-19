#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 TextCoords;
layout (location = 0) in vec3 objectPosition;



out vec2 vOutTextureCoords;
uniform mat4 Proj;
void main()
{
  gl_Position = Proj * vec4(position + objectPosition, 1.0f);
  gl_Position = vec4(position,1.0f);
  vOutTextureCoords =  TextCoords;
}
