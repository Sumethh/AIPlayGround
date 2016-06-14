#include "Renderer2D.h"
#include <glm/glm.hpp>

glm::mat4 Renderer2D::m_projection;
Renderer2D* Renderer2D::m_instance;
glm::mat4 Renderer2D::GetProjection()
{
  return m_projection;
}

void Renderer2D::Init()
{
  m_lineRenderer.Init();
  m_basicRenderer.Init();
  m_staticRenderer.Init();
  m_batchRenderer.Init();
}

void Renderer2D::Begin()
{
  m_lineRenderer.Begin();
  m_batchRenderer.Begin();
}

void Renderer2D::End()
{
  m_lineRenderer.End();
  m_batchRenderer.End();
}

void Renderer2D::Flush()
{
  m_staticRenderer.Flush();
  m_basicRenderer.Flush();
  m_batchRenderer.Flush();
  m_lineRenderer.Flush();
}

Renderer2D::Renderer2D()
{
  m_instance = this;
}

Renderer2D::~Renderer2D()
{
}