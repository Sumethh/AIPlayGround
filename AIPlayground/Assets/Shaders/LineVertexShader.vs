#version 330 core

layout (location = 0) in vec2 pointPosition;
layout (location = 1) in vec4 color;

out vec4 vOutColor;

uniform mat4 Proj;

void main()
{
  gl_Position =  Proj * vec4(pointPosition,0,1.0f);

  vOutColor = color;
}
