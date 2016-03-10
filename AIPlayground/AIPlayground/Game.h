#pragma once
#include "Grid.h"
#include "PlayerController.h"
#include "Camera.h"
#include "Pathfinder.h"

class Window;
class Game
{
public:
  Game();
  ~Game();

  void Init();
  void Update(float a_dt);
  void PreRender();
  void Render( Window* const a_window );
  void PostFrame();

private:
  Grid m_grid;
  PlayerController m_playerController;
  Camera m_camera;
  Pathfinder m_pathfinder;
};

