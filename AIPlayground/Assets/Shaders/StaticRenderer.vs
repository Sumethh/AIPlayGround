#version 330 core

layout (location = 0) in vec4 Top;
layout (location = 1) in vec4 Bottom;
layout (location = 2) in mat4 Model;

out vec2 out_textCoord;
uniform mat4 Proj;
//uniform mat4 ModelUni;

out VS_OUT{
  vec2 TopLeft;
  vec2 TopRight;
  vec2 BottomLeft;
  vec2 BottomRight;
  mat4 ProjectionMat;
  mat4 ModelMat;
} vs_out;

void main()
{
  gl_Position = vec4(0,0,0,1);
  vs_out.TopLeft = Top.xy;
  vs_out.TopRight = Top.zw;
  vs_out.BottomLeft = Bottom.xy;
  vs_out.BottomRight = Bottom.zw;
  vs_out.ProjectionMat = Proj;
  vs_out.ModelMat = Model;
}
