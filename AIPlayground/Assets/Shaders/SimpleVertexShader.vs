#version 330 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Color;
layout (location = 2) in vec2 TextCoord;
layout (location = 3) in mat4 Model;

out vec4 in_color;
out vec2 out_textCoord;
uniform mat4 Proj;
//uniform mat4 ModelUni;

void main()
{
  //gl_Position = vec4(Position, 1.0f);
  gl_Position = Proj * Model * vec4(Position.x, Position.y, Position.z, 1.0f);
  in_color = vec4(Color,1.0f);
  //out_textCoord = TextCoord;
}