#pragma once
#include <vector>
#include <map>
#include "Grid.h"
#include "Pathfinder.h"
#include "Camera.h"
#include "PlayerController.h"
#include "GameObjectTypes.h"
#include "GameObject.h"
#include <memory.h>
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
  inline std::weak_ptr<PlayerController> GetPlayerController() { return m_playerController; }
  inline std::weak_ptr<Pathfinder> GetPathfinder() { return m_pathfinder; }
  inline std::weak_ptr<Camera> GetCamera() { return m_camera; }
  inline WorldLimits GetWorldLimits()const { return m_worldLimits; }

protected:
  void SetGameObjectWorld( GameObject* a_gameobject );
private:
  std::vector<GameObject*> m_gameObjects;
  std::map<EGameObjectType , GameObjectConstructionDescriptor> m_gameObjectDescriptors;

  WorldLimits m_worldLimits;

  std::shared_ptr<Grid> m_grid;
  std::shared_ptr<Camera> m_camera;
  std::shared_ptr<PlayerController> m_playerController;
  std::shared_ptr<Pathfinder> m_pathfinder;
};

