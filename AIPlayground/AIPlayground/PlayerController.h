#pragma once
#include "Grid.h"
#include "GameObject.h"
#include <memory>
class Renderer2D;
class Camera;
class World;
class PlayerController
{
  enum class ESpawnableItemIDs : uint8
  {
    CoalDirt,
    CoalRock,
    CopperDirt,
    CopperRock,
    MaxCount
  };
public:
  PlayerController( Grid* a_grid , Camera* a_camera );
  ~PlayerController();
  
  void Init();
  void Update( float a_dt );
  void PreRender();
  void Render( Renderer2D* a_renderer);
  inline void SetWorld(World* a_world) { m_world = a_world; }
  inline World* GetWorld() { return m_world; }
private:
  void UpdateUI();

  void SpawningObject(ESpawnableItemIDs a_id);

  GameObject* m_selectionGO;
  GameObject* m_currentSpawningObj;
  const float m_moveSpeed = 512.0f;
  World* m_world;
  Grid* const  m_grid;
  Camera* const m_camera;
  Transform m_selectionTransform;
  std::vector<GameObject*> m_selectedGameObjects;
  float m_cameraMoveSpeed = 256.0f;
};
