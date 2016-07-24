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
  glEnable( GL_DEPTH_TEST );
  glDepthFunc( GL_LEQUAL );
  m_staticRenderer.Flush();
  m_basicRenderer.Flush();
  m_batchRenderer.Flush();
  m_lineRenderer.Flush();
  glDisable( GL_DEPTH_TEST );
}

Renderer2D::Renderer2D()
{
  m_instance = this;
}

Renderer2D::~Renderer2D()
{
}