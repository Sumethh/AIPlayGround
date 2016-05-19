#pragma once
#include <glm/glm.hpp>
class Renderer2D
{
public:
  Renderer2D();
  ~Renderer2D();

  static void SetProjectionMatrix( glm::mat4 a_mat ) { m_projection = a_mat; }
  static glm::mat4 GetProjection();
private:

  static glm::mat4 m_projection;

};

