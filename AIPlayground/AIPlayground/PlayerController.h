#pragma once
#include "Grid.h"

#include <memory>
class Camera;
class PlayerController
{
public:
  PlayerController( std::shared_ptr<Grid>& a_grid , std::shared_ptr<Camera>& a_camera );
  ~PlayerController();

  void Init();
  void Update( float a_dt );
  void PreRender();
  void Render( Window* const a_window );

private:

  sf::Sprite m_selectionSprite;
  sf::Texture m_selectionTexture;

  const float m_moveSpeed = 512.0f;

  std::weak_ptr<Grid> const  m_grid;
  std::weak_ptr<Camera> const m_camera;
};
