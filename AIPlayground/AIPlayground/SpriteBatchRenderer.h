#pragma once
#include <gl/glew.h>
#include "Common/Types.h"
#include <glm/glm.hpp>
#include "Shader.h"

#define MAX_SPRITE_RENDERS 20000



struct StaticTexCoordData
{
  //glm::vec2 m_bottomLeft;
  //glm::vec2 m_bottomRight;
  //glm::vec2 m_topLeft;
  //glm::vec2 m_topRight;
  glm::vec2 coords[ 6 ]; //TopRight, BottomRight, TopLeft, BottomRight, BottomLeft, TopLeft
};

class SpriteBatchRenderer
{
public:
  SpriteBatchRenderer();
  ~SpriteBatchRenderer();
  void Init();
  void Begin();
  void Submit( glm::mat4& a_mat , StaticTexCoordData& a_texCoords );
  void End();
  void Flush();
  Shader& GetShader() { return m_shader; }
private:
  GLuint m_vao;
  GLuint m_vbo;
  GLuint m_ibo;
  //GLuint m_instanceVbo;
  uint m_spriteCount;
  Shader m_shader;
  struct VertexData
  {
    glm::vec3 pos;
    glm::vec2 textCoord;
  };

  VertexData* m_data;

  GLint m_projUniformLoc;

};

