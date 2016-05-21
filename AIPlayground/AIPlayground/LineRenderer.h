#pragma once
#include "Shader.h"

#include <Gl/glew.h>
#include <glm/glm.hpp>
#define MAX_LINE_RENDERS 30000

class LineRenderer
{
public:
  LineRenderer();
  ~LineRenderer();

  void Init();
  void Begin();
  void Submit( glm::vec2 a_startPosition , glm::vec2 a_endPosition , glm::vec4 a_color );
  void End();
  void Flush();
private:
  GLuint m_vao , m_vbo;
  Shader* m_shader;

  struct LineRenderData
  {
    LineRenderData( glm::vec2 a_vec2 , glm::vec4 a_vec4 ) :position( a_vec2 ) , color( a_vec4 ) {};
    glm::vec2 position;
    glm::vec4 color;
  };
  LineRenderData* m_data;
  unsigned int m_lineRenderCount;
  unsigned int m_projLocation;
};
