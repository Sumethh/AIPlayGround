#pragma once
#include "Grid.h"
#include "GameObject.h"
#include <memory>
class Renderer2D;
class Camera;
class PlayerController
{
public:
  PlayerController( std::shared_ptr<Grid>& a_grid , std::shared_ptr<Camera>& a_camera );
  ~PlayerController();

  void Init();
  void Update( float a_dt );
  void PreRender();
  void Render( Renderer2D* a_renderer);

private:

  sf::Sprite m_selectionSprite;
  sf::Texture m_selectionTexture;

  const float m_moveSpeed = 512.0f;

  std::weak_ptr<Grid> const  m_grid;
  std::weak_ptr<Camera> const m_camera;
  Transform m_selectionTransform;
};
