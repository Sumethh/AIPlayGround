#pragma once
#include <gl/glew.h>
#include "Common/Types.h"
#include <glm/glm.hpp>
#include "Shader.h"

#define MAX_SPRITE_RENDERS 20000



class SpriteBatchRenderer
{
public:
  SpriteBatchRenderer();
  ~SpriteBatchRenderer();
  void Init();
  void Begin();
  void Submit( glm::mat4& a_mat );
  void End();
  void Flush();
  Shader& GetShader() { return m_shader; }
private:
  GLuint m_vao;
  GLuint m_vbo;
  GLuint m_instanceVbo;
  uint m_spriteCount;
  Shader m_shader;
  struct SpriteRenderData
  {
    glm::mat4 Modelmatrix;
  };

  SpriteRenderData* m_data;

  GLint m_projUniformLoc;

};

