#pragma once
#include "World.h"

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
  void Render( Window* const a_window );
  void PostFrame();

private:
  World::SharedPtr m_world;
};
