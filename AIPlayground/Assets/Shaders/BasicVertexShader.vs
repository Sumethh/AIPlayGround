#version 330 core
layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoord;

uniform mat4 Model;
uniform mat4 Proj;

out vec2 texCoord;

void main()
{
  gl_Position = Proj * Model * vec4(Position.x, Position.y , Position.z, 1.0f);
  texCoord = TexCoord;
}
