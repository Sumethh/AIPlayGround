#version 330 core

layout (location = 0) in vec4 Top;
layout (location = 1) in vec4 Bottom;
//layout (location = 2) in mat4 Model;
layout (location = 2) in vec3 Position;
layout (location = 3) in vec2 Scale;
layout (location = 4) in float Rotation;

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
  //gl_Position = vec4(Position, 1.0f);
  //gl_Position = Proj * vec4(Position.x, Position.y, Position.z, 1.0f);
  //gl_Position = vec4(Position.x, Position.y, Position.z, 1.0f);
  mat4 ModelMat, RotationMat, ScaleMat, TranslationMat;

  TranslationMat[0] = vec4(1,0,0,0);
  TranslationMat[1] = vec4(0,1,0,0);
  TranslationMat[2] = vec4(0,0,1,1);
  TranslationMat[3] = vec4(Position,1);

  ScaleMat[0] = vec4(Scale.x,0,0,0);
  ScaleMat[1] = vec4(0,Scale.y,0,0);
  ScaleMat[2] = vec4(0,0,1,0);
  ScaleMat[3] = vec4(0,0,0,1);

  RotationMat[0] = vec4(1*cos(Rotation), -sin(Rotation),0,0);
  RotationMat[1] = vec4(sin(Rotation), 1 * cos(Rotation),0,0);
  RotationMat[2] = vec4(0, 0,1,0);
  RotationMat[3] = vec4(0, 0,0,1);

  ModelMat = TranslationMat * RotationMat * ScaleMat;

  /*ModelMat[0][3] = 400;
  ModelMat[1][3] = 400;
  ModelMat[2][3] = 0;
  ModelMat[3][3] = 1;
  ModelMat[0][0] = cos(0);
  ModelMat[0][1] = -sin(0);
  ModelMat[1][0] = sin(0);
  ModelMat[1][1] = cos(0);
  ModelMat[0][0] *= 32;
  ModelMat[1][1] *= 32;
  ModelMat[2][2] = 1;*/
  gl_Position = vec4(0,0,0,1);
  vs_out.TopLeft = Top.xy;
  vs_out.TopRight = Top.zw;
  vs_out.BottomLeft = Bottom.xy;
  vs_out.BottomRight = Bottom.zw;
  vs_out.ProjectionMat = Proj;
  vs_out.ModelMat = ModelMat;
}
