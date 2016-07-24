#pragma once
#include <glm/glm.hpp>

class Camera
{
public:
  Camera();
  ~Camera();

  void Update();

  inline glm::vec2 GetPos() { return m_pos; }
  inline void SetPos( const glm::vec2 a_pos ) { m_pos = a_pos; }

  inline void MoveX( float a_x ) { m_pos.x += a_x; }
  inline void MoveY( float a_y ) { m_pos.y += a_y; }

  bool IsMainCamera() { return m_mainCamera; }
  void SetMainCamera( bool a_bool ) { m_mainCamera = a_bool; }
private:
  glm::vec2 m_pos;
  bool m_mainCamera;
};
