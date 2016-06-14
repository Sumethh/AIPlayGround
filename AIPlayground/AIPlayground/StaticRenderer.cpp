#include "StaticRenderer.h"
#include "Common/log.h"
#include "Renderer2D.h"
#include <glm/gtc/type_ptr.hpp>
#include "ShaderManager.h"
#include "TextureManager.h"
StaticRenderer::StaticRenderer()
{
}

StaticRenderer::~StaticRenderer()
{
}

void StaticRenderer::Init()
{
  m_freeIds.reserve(100);
  m_shader = ShaderManager::GI()->GetShader(EShaderID::StaticRender);
  m_texture = TextureManager::GI()->GetTexture(ETextureID::StaticSpriteSheet);
  glGenBuffers(1, &m_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(SpriteInfo) * MAX_STATIC_RENDERS, NULL, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);

  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(SpriteInfo), (GLvoid*)offsetof(SpriteInfo, Top));

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(SpriteInfo), (GLvoid*)offsetof(SpriteInfo, Bottom));

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(SpriteInfo), (GLvoid*)offsetof(SpriteInfo, Model));

  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(SpriteInfo), (GLvoid*)(offsetof(SpriteInfo, Model) + sizeof(glm::vec4)));

  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(SpriteInfo), (GLvoid*)(offsetof(SpriteInfo, Model) + sizeof(glm::vec4) * 2));

  glEnableVertexAttribArray(5);
  glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(SpriteInfo), (GLvoid*)(offsetof(SpriteInfo, Model) + sizeof(glm::vec4) * 3));

  glBindVertexArray(0);
}

uint StaticRenderer::Register()
{
  if (m_staticCount > MAX_STATIC_RENDERS)
  {
    LOGE("Have max static sprites");
    return 0;
  }
  uint newId = 0;
  if (m_freeIds.size())
  {
    newId = m_freeIds[0];
    m_freeIds.erase(m_freeIds.begin());
    m_staticCount++;
  }
  else
  {
    newId = m_staticCount++;
  }
  return newId;
}

void StaticRenderer::UpdateTexCoords(uint a_index, glm::vec4 a_top, glm::vec4 a_bottom)
{
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glm::vec4 data[2] = { a_top, a_bottom };
  glBufferSubData(GL_ARRAY_BUFFER, (GLintptr)(sizeof(SpriteInfo) * a_index), sizeof(glm::vec4) * 2, data);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void StaticRenderer::UpdatePosition(uint a_index, glm::mat4 a_model)
{
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferSubData(GL_ARRAY_BUFFER, (GLintptr)(sizeof(SpriteInfo) * a_index + offsetof(SpriteInfo, Model)), sizeof(glm::mat4), glm::value_ptr(a_model));
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void StaticRenderer::UnRegister(uint a_index)
{
  SpriteInfo verts = {};
  GLintptr offset = (GLintptr)(sizeof(SpriteInfo) *a_index);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(SpriteInfo), &verts);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  m_freeIds.push_back(a_index);
  std::sort(m_freeIds.begin(), m_freeIds.end());
}

void StaticRenderer::Flush()
{
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  m_shader->Bind();
  glUniformMatrix4fv(m_shader->GetProjectionUniformLoc(), 1, GL_FALSE, glm::value_ptr(Renderer2D::GetProjection()));
  m_texture->Bind();
  glBindVertexArray(m_vao);
  glDrawArrays(GL_POINTS, 0, m_staticCount);  
  m_texture->UnBind();
  glBindVertexArray(0);
  m_shader->UnBind();
}