#pragma once
#include "Shader.h"
#include "Common/Types.h"
#include <Gl/glew.h>
#include <glm/glm.hpp>
#include <vector>
#define MAX_STATIC_RENDERS 20000
class Texture;
struct StaticTexCoordData
{
  glm::vec2 m_bottomLeft;
  glm::vec2 m_bottomRight;
  glm::vec2 m_topLeft;
  glm::vec2 m_topRight;
};
class StaticRenderer
{
public:
  StaticRenderer();
  ~StaticRenderer();

  void Init();
  uint Register();
  void UpdateTexCoords( uint a_index , glm::vec4 a_top , glm::vec4 a_bottom );
  //TopLeft, TopRight, BottomLeft, BottomRight
  void UpdatePosition( uint a_index , glm::mat4 a_mat );
  void UnRegister( uint a_index );
  void Flush();
  void SetCameraPos( glm::vec2 a_camPos ) { m_cameraPos = a_camPos; }
private:
  GLuint m_vao , m_vbo;
  Shader* m_shader;
  Texture* m_texture;
  glm::vec2 m_cameraPos;
  struct SpriteInfo
  {
    glm::vec4 Top;
    glm::vec4 Bottom;
    glm::mat4 Model;
  };

  std::vector<uint> m_freeIds;
  uint m_staticCount;
  int m_projLoc;
};
