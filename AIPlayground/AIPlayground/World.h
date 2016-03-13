#pragma once
#include <vector>
#include <map>
#include "Grid.h"
#include "Pathfinder.h"
#include "Camera.h"
#include "PlayerController.h"
#include "GameObjectTypes.h"
#include "GameObject.h"
class GameObject;
class Window;

struct WorldLimits 
{
  glm::vec2 topLeft , bottomRight;
};

class World
{
public:
  World();
  virtual ~World();

  virtual void OnConstruct();
  virtual void OnDestroyed();
  virtual void BeginPlay();
  virtual void Update( float a_dt );
  virtual void PreRender();
  virtual void Render( Window* const a_window );
  virtual void PostFrame();

  GameObject* CreateGameObject(EGameObjectType a_type = EGameObjectType::GOT_Generic);
  inline PlayerController* GetPlayerController() { return &m_playerController; }
  inline Pathfinder* GetPathfinder() { return &m_pathfinder; }
  inline Camera* GetCamera() { return &m_camera; }
  inline WorldLimits GetWorldLimits()const { return m_worldLimits; }

protected:
  void SetGameObjectWorld( GameObject* a_gameobject );
private:
  std::vector<GameObject*> m_gameObjects;
  std::map<EGameObjectType , GameObjectConstructionDescriptor> m_gameObjectDescriptors;

  WorldLimits m_worldLimits;

  Grid m_grid;
  PlayerController m_playerController;
  Camera m_camera;
  Pathfinder m_pathfinder;
};

