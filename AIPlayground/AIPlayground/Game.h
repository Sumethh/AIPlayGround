#pragma once
#include "World.h"
#include "Renderer2D.h"
class Window;
class Game
{
public:
  Game();
  ~Game();

  void Init();
  void FixedUpdate( float a_dt );
  void Update( float a_dt );
  void PreRender();
  void Render();
  void PostFrame();

private:
  World::SharedPtr m_world;
  Renderer2D m_renderer;
};
