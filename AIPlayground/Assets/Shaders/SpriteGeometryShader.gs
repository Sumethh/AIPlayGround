#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in VS_OUT{
  vec2 TopLeft;
  vec2 TopRight;
  vec2 BottomLeft;
  vec2 BottomRight;
  mat4 ProjectionMat;
  mat4 ModelMat;
}gs_in[];

out vec2 gTextureCoord;
out vec4 testColor;
void main()
{
  //TopLeft
  vec4 newPosition = gl_in[0].gl_Position + vec4(-0.5f, -0.5f, 0.0f,0.0f);
  gl_Position = gs_in[0].ProjectionMat * gs_in[0].ModelMat * newPosition;
  gTextureCoord = gs_in[0].TopLeft;
  EmitVertex();

  //TopRight
  newPosition = gl_in[0].gl_Position + vec4(0.5f, -0.5f, 0.0f,0.0f);
  gl_Position = gs_in[0].ProjectionMat * gs_in[0].ModelMat *newPosition;
  gTextureCoord = gs_in[0].TopRight;
  EmitVertex();

  //BottomLeft
  newPosition = gl_in[0].gl_Position + vec4(-0.5f, 0.5f, 0.0f,0.0f);
  gl_Position = gs_in[0].ProjectionMat * gs_in[0].ModelMat * newPosition;
  gTextureCoord = gs_in[0].BottomLeft;
  EmitVertex();

  //BottomRight
  newPosition = gl_in[0].gl_Position + vec4(0.5f, 0.5f, 0.0f,0.0f);
  gl_Position = gs_in[0].ProjectionMat * gs_in[0].ModelMat * newPosition;
  gTextureCoord = gs_in[0].BottomRight;
  EmitVertex();
  EndPrimitive();
}
