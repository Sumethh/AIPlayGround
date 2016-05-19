#version 330 core

layout (location = 0) in vec2 textCoords;
layout (location = 1) in mat4 model;


out vec2 vOutTextureCoords;
uniform mat4 Proj;
void main()
{
  gl_Position = Proj * model;
  vOutTextureCoords =  TextCoords;
}
