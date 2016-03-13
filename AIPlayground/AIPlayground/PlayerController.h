#pragma once
#include "Grid.h"


class Camera;
class PlayerController
{
public:
  PlayerController( Grid* const a_grid , Camera* const a_camera );
  ~PlayerController();

  void Init();
  void Update( float a_dt );
  void PreRender();
  void Render( Window* const a_window );

private:

  sf::Sprite m_selectionSprite;
  sf::Texture m_selectionTexture;

  const float m_moveSpeed = 512.0f;

  Grid * const  m_grid;
  Camera* const m_camera;
};

