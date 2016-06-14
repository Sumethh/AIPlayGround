#pragma once
#include <glm/glm.hpp>
#include "LineRenderer.h"
#include "SpriteBatchRenderer.h"
#include "Basic2DRenderer.h"
#include "StaticRenderer.h"
class Renderer2D
{
public:
  Renderer2D();
  ~Renderer2D();

  static void SetProjectionMatrix( glm::mat4 a_mat ) { m_projection = a_mat; }
  static glm::mat4 GetProjection();

  void Init();

  void Begin();
  void End(); 
  
  void Flush();

  static Renderer2D* GI() {
    if (!m_instance)
      m_instance = new Renderer2D;
    return m_instance;
  }

  LineRenderer& GetLineRenderer() { return m_lineRenderer; }
  StaticRenderer& GetStaticRenderer() { return m_staticRenderer; }
  SpriteBatchRenderer& GetSpriteBatchRenderer() { return m_batchRenderer; }
  Basic2DRenderer& GetBasicRenderer() { return m_basicRenderer;}

private:
  static Renderer2D* m_instance;
  LineRenderer m_lineRenderer;
  SpriteBatchRenderer m_batchRenderer;
  StaticRenderer m_staticRenderer;
  Basic2DRenderer m_basicRenderer;

  static glm::mat4 m_projection;
};
