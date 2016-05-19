#include "Renderer2D.h"
#include <glm/glm.hpp>

glm::mat4 Renderer2D::m_projection;

glm::mat4 Renderer2D::GetProjection()
{
  return m_projection;
}


Renderer2D::Renderer2D()
{
}


Renderer2D::~Renderer2D()
{

}
