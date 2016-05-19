#pragma once
#include "Shader.h"
#include "Common/Types.h"
#include <Gl/glew.h>
#include <glm/glm.hpp>
#include <vector>
#define MAX_STATIC_RENDERS 20000
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
  void UpdateTexCoords( uint a_index , StaticTexCoordData a_texCoords );
  void UpdateVertices( uint a_index , glm::vec2 m_vertices[4] );
  void UnRegister( uint a_index );

private:
  GLuint m_vao , m_vbo;
  Shader m_shader;

  struct Vertex 
  {
    glm::vec3 pos;
    glm::vec2 texCoord;
  };

  std::vector<uint> m_freeIds;
  uint m_staticCount;
};

