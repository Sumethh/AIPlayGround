#pragma once
#include <gl/glew.h>
#include "Common/Types.h"
#include <glm/glm.hpp>
#include "Shader.h"

#define MAX_SPRITE_RENDERS 30000
class Texture;
class SpriteBatchRenderer
{
public:
  SpriteBatchRenderer();
  ~SpriteBatchRenderer();
  void Init();
  void Begin();
  void Submit( glm::mat4& a_mat , glm::vec4 a_top , glm::vec4 a_bottom );
  void End();
  void Flush();
private:
  GLuint m_vao;
  GLuint m_vbo;
  uint m_spriteCount;
  Shader* m_shader;
  Texture* m_texture;
  struct SpriteInfo
  {
    glm::vec4 Top;
    glm::vec4 Bottom;
    glm::mat4 Model;
  };

  SpriteInfo* m_data;

  GLint m_projUniformLoc;
};
